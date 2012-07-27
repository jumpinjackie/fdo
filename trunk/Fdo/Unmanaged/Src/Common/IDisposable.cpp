//
// Copyright (C) 2004-2009  Autodesk, Inc.
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

#include <Common/Std.h>
#include <Common/FdoTypes.h>
#include <Common/IDisposable.h>
#include <Common/Exception.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#ifdef UNICODE
#define GetClassNameW  GetClassName
#else
#define GetClassNameA  GetClassName
#endif // !UNICODE
#endif

/// Define the global state of the FdoIDisposable thread locking
///
bool FdoIDisposable::m_globalThreadLockingEnabled = false;

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

