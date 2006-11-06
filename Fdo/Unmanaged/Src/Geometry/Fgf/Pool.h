#ifndef _FDOPOOL_H_
#define _FDOPOOL_H_

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//



#ifdef _WIN32
#pragma once
#endif

#include <Common/Collection.h>

#define GET_REFCOUNT(obj)   ((obj)->GetRefCount())

/// <library>FdoGeometry.lib</library>

/// <summary>FdoPool supports a fixed-size cache of objects.  Because this type extends
/// FdoCollection, the cached objects must descend from FdoIDisposable.
/// </summary>
template <class OBJ, class EXC> class FdoPool : public FdoCollection <OBJ, EXC>
{
public:
    ///<summary>Find a re-usable item from the cache, if cache is full.</summary>
    /// <remarks> 
    /// This method actually tries NOT to find a re-usable item, so that the cache
    /// can fill up with them during heavy use.  A re-usable item
    /// is one which only the cache owns (its reference count is 1).  This method will 
    /// behave as follows (in order):
	/// <ol>
    /// <li> If the cache is not full, it returns NULL.
    /// <li> If the cache is full, but a re-usable item is found, that item is returned,
    /// and is RELEASED from the cache.
    /// <li> If the cache is full with non-re-usable items, NULL is returned.
	/// </ol>
    ///
    /// From the caller's perspective, receiving NULL from this method should be
    /// followed by the creation of a new object; receiving a re-usable item from this 
    /// method should be followed by whatever is required to re-use the object.  In BOTH
    /// cases, the caller should call AddItem() to put the object back into the cache.
    ///
    /// Performance will be best if interaction with this class is one object at a time, 
    /// i.e. call FindReusableItem() and then AddItem() for the same object.
    ///
    /// When returning a re-usable object, this method releases it from the cache in order
    /// to give the caller unshared ownership.  This allows the caller full authority
    /// to change the object's state as needed, or even to re-allocate it if this is later
    /// found to be necessary.
    /// </remarks>
    /// <returns>A re-usable item from the cache, or NULL if the caller should create a new object.</returns> 
    virtual OBJ* FindReusableItem()
    {
        FdoInt32 freeIndex = -1;
        FdoInt32 reusableIndex = -1;
        FdoInt32 startIndex = m_LastVisitedIndex+1;
        FdoPtr<OBJ> reusableItem;
        FdoInt32 count = FdoCollection <OBJ, EXC>::GetCount();

        // Nothing notably clever here -- linear search for re-usable item, also
        // noting a free entry, if found.
        for (FdoInt32 i=startIndex;  -1 == reusableIndex && i < count;  i++)
        {
            FdoPtr<OBJ> candidate = FdoCollection<OBJ, EXC>::GetItem(i);
            if (candidate != NULL && -1 == reusableIndex && 2 == GET_REFCOUNT(candidate.p))   // '2' == 1 for cache + 1 for 'candidate'
            {
                reusableIndex = i;
                reusableItem = candidate;
            }
            else if (candidate == NULL && -1 == freeIndex)
            {
                freeIndex = i;
            }
        }
        for (FdoInt32 i=0;  -1 == reusableIndex && i < startIndex;  i++)
        {
            FdoPtr<OBJ> candidate = FdoCollection<OBJ, EXC>::GetItem(i);
            if (candidate != NULL && -1 == reusableIndex && 2 == GET_REFCOUNT(candidate.p))   // '2' == 1 for cache + 1 for 'candidate'
            {
                reusableIndex = i;
                reusableItem = candidate;
            }
            else if (candidate == NULL && -1 == freeIndex)
            {
                freeIndex = i;
            }
        }

        if (reusableItem != NULL)
        {
            // We found a re-usable item.
            // Remember where we found it and release our ownership.
            m_LastVisitedIndex = reusableIndex;
            FDO_SAFE_ADDREF(reusableItem.p);    // This increases share for the return value.
            FdoCollection<OBJ, EXC>::SetItem(reusableIndex, NULL);
#ifdef EXTRA_DEBUG
            m_ReusableHits++;
#endif
        }
        else if (-1 != freeIndex)
        {
            // We found an empty entry in the cache.  We'll return NULL,
            // and internally remember where we found the entry for when
            // the caller calls AddItem().
            reusableItem = NULL;
            m_LastVisitedIndex = freeIndex;
#ifdef EXTRA_DEBUG
            m_EmptyHits++;
#endif
        }
        else
        {
            // We didn't find anything we wanted.  Release the next entry,
            // effecting a round-robin procedure for a full pool.
            m_LastVisitedIndex++;
            if (m_LastVisitedIndex >= count)
                m_LastVisitedIndex = 0;
            FdoCollection<OBJ, EXC>::SetItem(m_LastVisitedIndex, NULL);
#ifdef EXTRA_DEBUG
            m_Misses++;
#endif
        }

        return reusableItem;
    }

    ///<summary>Adds the specified item to the cache. Depending on the state of the
    /// cache, this may result in an actual addition to the cache, or the replacement
    /// of a cache entry.</summary>
    /// <param name="value">Input value</param> 
    /// <returns>Returns nothing</returns> 
    virtual void AddItem(OBJ* value)
    {
        FdoPtr<OBJ> oldItem = FdoCollection<OBJ, EXC>::GetItem(m_LastVisitedIndex);

        if (oldItem != NULL)
        {
            // The entry is not empty.  The caller didn't precede with
            // FindReusableItem().  We'll do it anyway, ignoring the 
            // result and using the intended hint in m_LastVisitedIndex.
            FdoPtr<OBJ> dummyItem = FindReusableItem();
        }

        FdoCollection<OBJ, EXC>::SetItem(m_LastVisitedIndex, value);
    }

#ifdef EXTRA_DEBUG
    void GetStats(FdoInt32 * emptyHits, FdoInt32 * reusableHits, FdoInt32 * misses)
    {
        if (NULL != emptyHits)
            *emptyHits = m_EmptyHits;
        if (NULL != emptyHits)
            *reusableHits = m_ReusableHits;
        if (NULL != emptyHits)
            *misses = m_Misses;
    }
#endif

protected:
    FdoPool( FdoInt32 size )
        : m_LastVisitedIndex(0)
#ifdef EXTRA_DEBUG
        ,m_EmptyHits(0),
        m_ReusableHits(0),
        m_Misses(0)
#endif
    {
        if (size <= 0)
		    throw EXC::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                          L"FdoPool",
                                                          L"size"));

        // Set exact size.

        if (size < FdoCollection <OBJ, EXC>::GetCount())
            FdoCollection <OBJ, EXC>::Clear();

        while (FdoCollection <OBJ, EXC>::GetCount() < size)
            FdoCollection <OBJ, EXC>::Add(NULL);
    }

    FdoPool() {} // Default constructor to satisfy FdoPtr on Linux

    virtual ~FdoPool(void)
    {
    }
private:
    FdoInt32    m_LastVisitedIndex; // Slight performance help, plus guarantees rotating cache.
#ifdef EXTRA_DEBUG
    FdoInt32    m_EmptyHits;
    FdoInt32    m_ReusableHits;
    FdoInt32    m_Misses;   // No available, re-usable entries.
#endif
};

#define FDOPOOL_DEFINE(POOL_TYPE, OBJECT_TYPE) \
    class POOL_TYPE : public FdoPool<OBJECT_TYPE, FdoException> \
    { \
    public: \
        static POOL_TYPE * Create(FdoInt32 size) \
        { \
            POOL_TYPE * value = new POOL_TYPE(size); \
            if (NULL == value) \
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC))); \
 \
            return value; \
        } \
 \
    protected: \
        POOL_TYPE() \
            : FdoPool<OBJECT_TYPE, FdoException>(10) \
        { \
        } \
        POOL_TYPE(FdoInt32 size) \
            : FdoPool<OBJECT_TYPE, FdoException>(size) \
        { \
        } \
 \
        void Dispose() \
        { \
            delete this; \
        } \
    };

#define FDOPOOL_CREATE_OBJECT( pool_I, poolType_I, poolSize_I, objectType_I, constructorCall_I, resetCall_I ) \
{ \
    if ((pool_I) == NULL) \
        (pool_I) = poolType_I::Create(poolSize_I); \
    objectType_I * ret = (pool_I)->FindReusableItem(); \
    if (NULL == ret) \
    { \
        try \
        { \
            ret = new constructorCall_I; \
            if ( NULL == ret ) throw; \
        } \
        catch (...) \
        { \
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC))); \
        } \
    } \
    else    /* Re-use an object from the pool.*/ \
    { \
        ret->resetCall_I; \
    } \
 \
    (pool_I)->AddItem(ret); \
 \
    return ret; \
}


#endif

