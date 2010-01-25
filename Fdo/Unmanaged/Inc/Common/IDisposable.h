#ifndef _IDISPOSABLE_H_
#define _IDISPOSABLE_H_

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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#ifdef UNICODE
#define GetClassNameW  GetClassName
#else
#define GetClassNameA  GetClassName
#endif // !UNICODE
#endif

/// \brief
/// FdoIDisposable is the base interface for all classes that implement this
/// standard interface for reference counting and object destruction.
class FdoIDisposable
{
protected:
    /// \brief
    /// Create an instance of the IDisposable object
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API_COMMON FdoIDisposable() : m_refCount(1), m_objectThreadLockingEnabled(false) {};

    /// \brief
    /// Default destructor for this class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API_COMMON virtual ~FdoIDisposable() {};

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API_COMMON virtual void Dispose() = 0;
	
public:
    /// \brief
    /// Sets a global threading state for all disposable objects. Indicates if thread-safe calls
    /// should be used to manage the lifespan of all disposable objects.
    /// 
    /// \param enable 
    /// Input A flag indicating if global level thread locking should be enabled
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API_COMMON static void EnableGlobalThreadLocking(bool enable) { m_globalThreadLockingEnabled = enable; }

public:
    /// \brief
    /// Increase the reference count.
    /// 
    /// \return
    /// Returns the new reference count (value for debugging use only).
    /// 
    FDO_API_COMMON virtual FdoInt32 AddRef();

    /// \brief
    /// Decrease the reference count.
    /// 
    /// \return
    /// Returns the new reference count (value for debugging use only).
    /// 
    FDO_API_COMMON virtual FdoInt32 Release();

    /// \brief
    /// Retrieves the reference count.
    /// 
    /// \return
    /// Returns the existing reference count value.
    /// 
    FDO_API_COMMON virtual FdoInt32 GetRefCount() { return m_refCount; }

    /// Enables threading support for this disposable object. Indicates if
    /// thread-safe calls should be used to manage the lifespan of this object.
    /// 
    /// \param enable 
    /// Input A flag indicating if object level thread locking should be enabled
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDO_API_COMMON virtual void EnableObjectThreadLocking(bool enable) { m_objectThreadLockingEnabled = enable; }

private:
    FdoInt32 m_refCount;
    bool m_objectThreadLockingEnabled;
    static bool m_globalThreadLockingEnabled;
};

#include <Common/Exception.h>

/// \brief
/// Default implementation of FdoIDisposable::AddRef()
///
inline
FdoInt32 FdoIDisposable::AddRef()
{
    if (m_globalThreadLockingEnabled || m_objectThreadLockingEnabled)
    {
#if defined(__GNUC__) && defined (__GNUC_MINOR__) && ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 1))
        int ret = 1;
        __asm__ __volatile__("lock;xaddl %0,%1"
            : "=r"(ret),"=m"(m_refCount)
            : "0"(ret),"m"(m_refCount)
            : "memory","cc");
        return ret + 1;
#elif defined (__GNUC__)
        return __sync_add_and_fetch(&m_refCount,1);
#elif defined(_WIN32)
        return InterlockedIncrement((LONG*)&m_refCount);
#endif
    }

    return ++m_refCount;
}

/// \brief
/// Default implementation of FdoIDisposable::Release()
///
inline
FdoInt32 FdoIDisposable::Release()
{
#ifdef _DEBUG
    if (m_refCount <= 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_MEMORY_DEALLOCATION_ERROR), 
                                                           L"FdoIDisposable::Release",
                                                           L"FdoIDisposable"));
#endif
    
    if (m_globalThreadLockingEnabled || m_objectThreadLockingEnabled)
    {
#if defined(__GNUC__) && defined (__GNUC_MINOR__) && ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 1))
        int ret = -1;
        __asm__ __volatile__("lock;xaddl %0,%1" 
            : "=r"(ret),"=m"(m_refCount) 
            : "0"(ret),"m"(m_refCount) 
            : "memory","cc");
#elif defined (__GNUC__)
        __sync_sub_and_fetch(&m_refCount, 1);
#elif defined(_WIN32)
        InterlockedDecrement((LONG*)&m_refCount);
#endif
    }
    else
    {
        --m_refCount;
    }

    if (0 != m_refCount)
        return m_refCount;

    Dispose();
    return 0;
}

/// \brief
/// Define helper macros FDO_SAFE_ADDREF/FDO_SAFE_RELEASE
///
#define FDO_SAFE_RELEASE(x) {if (x) (x)->Release(); (x) = NULL;}
#define FDO_SAFE_ADDREF(x)  ((x != NULL) ? (x)->AddRef(), (x) : (NULL))

#endif


