// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <memory.h>

#include <FdoCommon.h>

#define MIN_ALLOC 1
#define GROWTH_FACTOR 1     /* Proportion of array size to grow when needed. */
#define POOL_ENTRY_BYTESIZE_LIMIT   (8*1024)    // 8KB limit on pooled FdoByteArray values, to suppress retention of large values in memory.

FdoArrayHelper::GenericArray* FdoArrayHelper::Append(GenericArray* array, FdoInt32 numElements, FdoByte* elements, FdoInt32 elementSize)
{
	// Disallow possible re-allocation if object is shared.
	if (array->m_metadata.refCount > 1)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_12_SHAREDARRAY)));

	if ( array->m_metadata.size + numElements > array->m_metadata.alloc )
		array = AllocMore(array, numElements, false /*exactly*/, elementSize);   // We could calculate the difference here, allocating less.

    FdoByte* appendHere = (array->GetData() + (array->m_metadata.size * elementSize));
	(void) memcpy( appendHere, elements, numElements * elementSize );

	array->m_metadata.size += numElements;

	return array;
}

FdoArrayHelper::GenericArray* FdoArrayHelper::SetSize(GenericArray* array, FdoInt32 numElements, FdoInt32 elementSize)
{
	// Disallow possible re-allocation if object is shared.
	if (array->m_metadata.refCount > 1)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_12_SHAREDARRAY)));

    if ( numElements > array->m_metadata.alloc )
	{
	    FdoInt32 oldSize = array->m_metadata.size;
	    FdoInt32 diff = numElements - oldSize;

		array = AllocMore(array, diff, true /*exactly*/, elementSize);

		// Zero out the new storage.
		(void) memset( array->GetData() + (oldSize * elementSize), 0, diff * elementSize );
	}

	// reset size/count downward without changing allocation
	array->m_metadata.size = numElements;

	return array;
}

FdoArrayHelper::GenericArray* FdoArrayHelper::SetAlloc(GenericArray* array, FdoInt32 numElements, FdoInt32 elementSize)
{
	// Disallow possible re-allocation if object is shared.
	if (array->m_metadata.refCount > 1)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_12_SHAREDARRAY)));

    if (numElements < array->m_metadata.size || numElements < MIN_ALLOC)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_13_INVALIDRESIZE)));

	if (array->m_metadata.alloc != numElements)
	{
        // Force new allocation of exactly the requested size.
		GenericArray* newArray = AllocMore(0, numElements, true /*exactly*/, elementSize);

		// Copy data from old array.
		newArray->m_metadata = array->m_metadata;
		newArray->m_metadata.alloc = numElements;
		if (array->m_metadata.size > 0)
		{
        	// Copy data from old array.
			(void) memcpy( newArray->GetData(), array->GetData(), array->m_metadata.size * elementSize );
		}
		delete [] ((FdoByte*)array);
		array = 0;  // Just to be obvious that the pointer is stale.
        array = newArray;
	}

	return array;
}


////////////////////////////////////////////////////////////////////////////
// Implement thread-local storage for pooled FdoByteArray instances.
////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>
typedef int FdoCommonThreadDataKey;
#else
#include <pthread.h>
typedef pthread_key_t FdoCommonThreadDataKey;
#endif
#include "../Geometry/Fgf/Pool.h"

FDOPOOL_DEFINE(FdoPoolFgfByteArray, FdoByteArray)

static FdoCommonThreadDataKey threadDataKey_S = 0;

struct FdoCommonThreadData
{
    FdoPtr<FdoPoolFgfByteArray>         byteArrayPool;

    static FdoCommonThreadData *        GetValue();
    static void                         ReleaseValue();
    static FdoCommonThreadDataKey       GetKey()                           { return threadDataKey_S; }
    static void                         SetKey(FdoCommonThreadDataKey key) { threadDataKey_S = key; }
};


#ifdef _WIN32
#define GET_VALUE(key)          ((FdoCommonThreadData *) TlsGetValue(key))
#define SET_VALUE(key, value)   (TlsSetValue((key), (value)))
#else
#define GET_VALUE(key)          ((FdoCommonThreadData *) pthread_getspecific(key))
#define SET_VALUE(key, value)   (pthread_setspecific((key), (value)))
#endif

FdoCommonThreadData *
FdoCommonThreadData::GetValue()
{
    FdoCommonThreadData *data = GET_VALUE(threadDataKey_S);

	if (NULL == data)
	{
        data = new FdoCommonThreadData;
        if (NULL == data)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        SET_VALUE(threadDataKey_S, data);
	}
	return data;
}

void FdoCommonThreadData::ReleaseValue()
{
    FdoCommonThreadData *data = GET_VALUE(threadDataKey_S);

    if (NULL != data)
    {
        // in case we set thread value to null before delete thread object
        // the thread object will be recreated and all objects will readded to new pools
        delete data;
        SET_VALUE(threadDataKey_S, NULL);
    }
}

/*
 * Allocate some more memory for the array.
 * If the given array is NULL, a new array of zero size 
 * but the requested allocation is returned.
 * The returned array will always have at least one new
 * free element allocated. Thus, passing atLeastThisMuch < 1, 
 * while not an error, still results in some new storage.
 */
FdoArrayHelper::GenericArray* FdoArrayHelper::AllocMore(GenericArray* array, FdoInt32 atLeastThisMuch, bool exactly, FdoInt32 elementSize)
{
	FdoInt32 oldSize = (0==array) ? 0 : array->m_metadata.size;
	FdoInt32 hopefulNewSize = oldSize + atLeastThisMuch;
	FdoInt32 oldAlloc = (0==array) ? 0 : array->m_metadata.alloc;
	FdoInt32 newAlloc = oldAlloc < MIN_ALLOC ? MIN_ALLOC : oldAlloc;    // There's always some room.

	if (exactly)
	{
		newAlloc = ( newAlloc > hopefulNewSize ) ? newAlloc : hopefulNewSize;
	}
	else // Infrequent-reallocation policy: increase the array size when needed
	{
        while ( newAlloc < hopefulNewSize )
			newAlloc *= 1 + GROWTH_FACTOR;
	}

    FdoInt32 newAllocBytes = (newAlloc*elementSize) + sizeof(GenericArray);

	/*
	 * The main trick: GenericArray's only real "member" data is the metadata.
	 * The rest of the allocated space is actually used for the array content itself.
	 * This is why GetData() doesn't look for some member data, but simply
	 * looks into "this" and skips over the metadata.  Because Metadata is a real
     * struct, skipping past its "sizeof" gives correct alignment in case the
     * array content is also composed of struct or class types.
	 */

    GenericArray* newArray = NULL;

    if (sizeof(FdoByte) == elementSize)
    {
        // Try to acquire an FdoByteArray from the pool that is in thread-local storage.
        // This is faster than going to the heap with the "new" operator.
        FdoCommonThreadData * threadData = FdoCommonThreadData::GetValue();
        FdoPoolFgfByteArray * pool = threadData->byteArrayPool;
        if (NULL == pool)
        {
            threadData->byteArrayPool = FdoPoolFgfByteArray::Create(10);
        }
        else
        {
            GenericArray * pooledArray = (GenericArray*)pool->FindReusableItem();
            if (NULL != pooledArray)
            {
                // The pooling template only knows that the entry is available.  It
                // doesn't know other attributes, such as adequate size.  We'll test
                // it here.  If it is not beg enough, we'll delete it and fall through
                // to a new allocation.
                if (pooledArray->m_metadata.alloc >= newAlloc)
                {
                    newArray = (GenericArray*) pooledArray;
                }
                else
                {
		            (void) memset(pooledArray, 0xfa, sizeof(GenericArray) + (pooledArray->m_metadata.alloc*elementSize));
		            delete [] ((FdoByte*) pooledArray);
		            pooledArray = 0;
                }
            }
        }
    }

    if (NULL == newArray)
	    newArray = (GenericArray*) new FdoByte[newAllocBytes];
	if (0==newArray)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

	// Copy data from old array.
	if (0!=array)
	{
        // Copy all metadata.
        *newArray = *array;
        // Copy elements.
        if (oldSize > 0)
			(void) memcpy( newArray->GetData(), array->GetData(), oldSize*elementSize );
    	// Make sure that stale pointers cannot do much.
		(void) memset(array, 0xfd, sizeof(GenericArray) + (array->m_metadata.alloc*elementSize));
        // Delete old storage.
		delete [] ((FdoByte*) array);
		array = 0;
	}
	else
	{
		newArray->m_metadata.SetSizes(1,0,0);
	}

	newArray->m_metadata.alloc = newAlloc;

	return newArray;
}

void FdoArrayHelper::DisposeOfArray(GenericArray* array, FdoInt32 elementSize)
{
    bool isPooled = false;

    if (sizeof(FdoByte) == elementSize && array->m_metadata.alloc <= POOL_ENTRY_BYTESIZE_LIMIT)
    {
        // This is an FdoByteArray (or something with the same element size, 
        // which is all that matters).  See if we can send it into the object pool,
        // rather that actually freeing it.
        FdoCommonThreadData * threadData = FdoCommonThreadData::GetValue();
        if (threadData->byteArrayPool != NULL)
            isPooled = threadData->byteArrayPool->AddItem((FdoByteArray *)array);
    }
    if (!isPooled)
    {
	    // Make sure that stale pointers cannot do much.
        void * destination = array;
        size_t size = sizeof(GenericArray) + (array->m_metadata.alloc*elementSize);
        (void) memset(destination, 0xfc, size);

        // Free the memory.
	    delete [] ((FdoByte*)array);
    }
}


#ifdef _WIN32

#include <windows.h>

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];

/// <summary>
/// Main entry point for the dll.
/// </summary>
/// <param name="hModule">Handle to the DLL module.</param> 
/// <param name="ul_reason_for_call">Reason for calling function.</param> 
/// <param name="lpReserved">Reserved.</param> 
/// <returns>TRUE if it succeeds or FALSE if initialization fails.</returns> 
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    DWORD nchars;
    wchar_t* last;
    BOOL ret;

    ret = TRUE;
    switch (ul_reason_for_call)
    {

        case DLL_PROCESS_ATTACH:
        {
            // hModule - The value is the base address of the DLL.
            // The HINSTANCE of a DLL is the same as the HMODULE of the DLL,
            // so hinstDLL can be used in calls to functions that require a module handle.
            nchars = GetModuleFileNameW ((HINSTANCE)hModule, module, MAX_PATH);
            if (0 == nchars)
                ret = FALSE;
            else
            {   
                // scan the string for the last occurrence of a slash
                wcscpy (home_dir, module);
                last = wcsrchr (home_dir, L'\\');
                if (NULL == last)
                    ret = FALSE;
                else
                {
                    last++; // move past the slash
                    *last = L'\0'; // null terminate it there
                }
            }
            FdoCommonThreadDataKey tlsIndex = TlsAlloc();
            FdoCommonThreadData::SetKey(tlsIndex);
            break;
        }


        case DLL_THREAD_ATTACH: 
 
            // Nothing essential here.  We to defer any pre-allocation
            // until someone calls FdoCommonThreadData::GetValue().
 
            break; 
 
        case DLL_THREAD_DETACH: 

            FdoCommonThreadData::ReleaseValue();
            break; 
 
        case DLL_PROCESS_DETACH: 
 
            FdoCommonThreadData::ReleaseValue();
            TlsFree(FdoCommonThreadData::GetKey());
            break; 

    }

    return (ret);
}

#else   // no _WIN32

// This destructor function is called automatically when the
// threading library destroys a thread key.
void _FdoCommonThreadDataDestructor(void * voidData)
{
    FdoCommonThreadData *data = (FdoCommonThreadData *) voidData;
                                                                                                           
    if (NULL != data)
    {
        delete data;
    }
}

void _loadFdoCommon()
{
    FdoCommonThreadDataKey key=0;
    pthread_key_create(&key, _FdoCommonThreadDataDestructor);
    FdoCommonThreadData::SetKey(key);
}


void _unloadFdoCommon()
{
    FdoCommonThreadDataKey key = FdoCommonThreadData::GetKey();
    FdoCommonThreadData::ReleaseValue();
    pthread_key_delete(key);
}

#endif

