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

/// <library>FdoGeometry.lib</library>

/// <summary>
/// FdoPool supports a fixed-size pool of instantiated objects.  This is
/// appropriate for writing high-performance code in a style that would
/// normally involve many expensive heap interactions.
/// </summary>
/// <remarks>
/// Don't panic.  This template just augments FdoCollection with two new methods
/// to use it as a stack.  Some handy macros are provided to simplify calling
/// code.
///
/// An FdoPool can be used in two distinct ways:
/// <ol>
/// <li> The pool owner can manually control it.  Contained objects
/// do not control being pooled.
/// <li> The pool owner can set it up and fetch from it, but the objects
/// add themselves to it when warranted.
/// </ol>
/// The conventions for pooled objects are:
/// <ol>
/// <li> Support the required methods of FdoIDisposable -- 
/// AddRef, Release, GetRefCount and Dispose().  Actually deriving from
/// FdoIDisposable is not required.
/// <li> Support at least one method to reset the object for re-use, possibly
/// with parameters matching the object's constructors.  Ideally, the matching
/// constructor would just call the matching reset method too.
/// <li> If the object-adds-itself approach is used, Dispose() should try
/// calling FdoPool's "AddItem" method before using "delete".
/// E.g. "if (!myPool->AddItem(this)) delete this;".
/// To support this, the object should either keep a lightweight pointer to
/// its pool, or use thread-local storage for the pool.
/// </ol>
/// 
/// See the FDOPOOL_* macros below, which  provide some shorthand for using
/// pools in calling code.
/// </remarks>
template <class OBJ, class EXC> class FdoPool : public FdoCollection <OBJ, EXC>
{
public:
    ///<summary>Find a re-usable item from the pool.</summary>
    /// <remarks> 
    /// This method finds a re-usable item in the pool, if there is one.
    /// It returns NULL if there are none available.
    ///
    /// From the caller's perspective, receiving NULL from this method should be
    /// followed by the creation of a new object; receiving a re-usable item from this 
    /// method should be followed by calling the appropriate "reset" method, if that
    /// convention has been followed.
    ///
    /// When returning a re-usable object, this method releases it from the pool in order
    /// to give the caller unshared ownership.  This method will ONLY return an object
    /// that is unshared.
    /// </remarks>
    /// <returns>A re-usable item from the pool, or NULL if the caller should create a new object.</returns> 
    virtual OBJ* FindReusableItem()
    {
        OBJ * reusableItem = NULL;
        FdoInt32 count = FdoCollection<OBJ, EXC>::GetCount();
        FdoInt32 reusableIndex = count - 1;

        // Search backwards from the end of the collection.
        // Get rid of any NULL or shared entries; they only would have gotten
        // here due to a caller's activities that are not appropriate for pooling.

        while ( reusableIndex >= 0 &&
                NULL == reusableItem )
        {
            reusableItem = FdoCollection<OBJ, EXC>::GetItem(reusableIndex);

            if (NULL != reusableItem && reusableItem->GetRefCount() > 2)
            {
                // Item is shared.
                reusableItem->Release();
                reusableItem = NULL;
            }
            FdoCollection<OBJ, EXC>::RemoveAt(reusableIndex);
            reusableIndex--;
        }

#ifdef EXTRA_DEBUG
        if (NULL != reusableItem)
            m_ReusableHits++;
        else
            m_Misses++;
#endif

        return reusableItem;
    }

    ///<summary>Adds the specified item to the pool, if there is room.
    /// </summary>
    /// <remarks> 
    /// We do not check for a shared object here, because the caller might still have
    /// a brief hold on it.  We check later, though, in FindReusableItem().
    /// </remarks>
    /// <param name="value">Input value</param> 
    /// <returns>Returns true if the object was added; false otherwise</returns> 
    virtual bool AddItem(OBJ* value)
    {
        bool ret = false;
        if (m_AllowAdds && value->GetRefCount() <= 1)
        {
            FdoInt32 count = FdoCollection <OBJ, EXC>::GetCount();
            if (count < m_MaxSize)
            {
                FdoCollection <OBJ, EXC>::Add(value);
                ret = true;
            }
        }
        return ret;
    }

#ifdef EXTRA_DEBUG
    void GetStats(FdoInt32 * emptyHits, FdoInt32 * reusableHits, FdoInt32 * misses)
    {
        if (NULL != reusableHits)
            *reusableHits = m_ReusableHits;
        if (NULL != misses)
            *misses = m_Misses;
    }
#endif

protected:
    // Constructor.  As a debugging aid, m_AllowAdds to false to disable pooling.
    FdoPool( FdoInt32 size )
        : m_MaxSize(size), m_AllowAdds(true)
#ifdef EXTRA_DEBUG
        , m_ReusableHits(0),
        m_Misses(0)
#endif
    {
        if (size <= 0)
		    throw EXC::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                          L"FdoPool",
                                                          L"size"));

        // FdoCollection doesn't support manual setting of allocation size
        //, but we can do the same by adding NULL values.

        while (FdoCollection <OBJ, EXC>::GetCount() < size)
            FdoCollection <OBJ, EXC>::Add(NULL);

        // Clear collection for zero size (though allocated space remains).
        FdoCollection <OBJ, EXC>::Clear();
    }

    FdoPool() {} // Default constructor to satisfy FdoPtr on Linux

    virtual ~FdoPool(void)
    {
        m_AllowAdds = false;
        FdoCollection <OBJ, EXC>::Clear();
    }
private:
    FdoInt32    m_MaxSize;  // Maximum number of objects in the pool.
    bool        m_AllowAdds;    // Suppress new items (during destruction)
#ifdef EXTRA_DEBUG
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
        catch (FdoException *ex) \
        { \
            throw ex; \
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
    return ret; \
}


#endif

