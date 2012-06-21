// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SDFX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SDFX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _WIN32
    #ifdef SLT_EXPORTS
        #define SLT_API __declspec(dllexport)
    #else
        #define SLT_API __declspec(dllimport)
    #endif
#else
    #define SLT_API
#endif

//Templates? What templates?
#if defined(__GNUC__) && defined (__GNUC_MINOR__) && ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 1))
#define SLT_IMPLEMENT_REFCOUNTING \
public:                                                                      \
    virtual void                Dispose()           { delete this; }         \
    virtual FdoInt32            AddRef()                                     \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag()){                   \
            int ret = 1;                                                     \
            __asm__ __volatile__("lock;xaddl %0,%1"                          \
                : "=r"(ret),"=m"(m_refCount)                                 \
                : "0"(ret),"m"(m_refCount)                                   \
                : "memory","cc");                                            \
            return ret + 1;                                                  \
        }else                                                                \
            return ++m_refCount;                                             \
    }                                                                        \
    virtual FdoInt32            Release()                                    \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag()){                   \
            int ret = -1;                                                    \
            __asm__ __volatile__("lock;xaddl %0,%1"                          \
                : "=r"(ret),"=m"(m_refCount)                                 \
                : "0"(ret),"m"(m_refCount)                                   \
                : "memory","cc");                                            \
            if (m_refCount)                                                  \
                return m_refCount;                                           \
        }else{                                                               \
            if (--m_refCount)                                                \
                return m_refCount;                                           \
        }                                                                    \
        Dispose();                                                           \
        return 0;                                                            \
    }                                                                        \
    virtual FdoInt32            GetRefCount()       { return m_refCount; }   \
private:                        int m_refCount;
#elif defined (__GNUC__)
#define SLT_IMPLEMENT_REFCOUNTING \
public:                                                                      \
    virtual void                Dispose()           { delete this; }         \
    virtual FdoInt32            AddRef()                                     \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag())                    \
            return __sync_add_and_fetch(&m_refCount,1);                      \
        else                                                                 \
            return ++m_refCount;                                             \
    }                                                                        \
    virtual FdoInt32            Release()                                    \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag()){                   \
            if (__sync_sub_and_fetch(&m_refCount, 1))                        \
                return m_refCount;                                           \
        }else{                                                               \
            if (--m_refCount)                                                \
                return m_refCount;                                           \
        }                                                                    \
        Dispose();                                                           \
        return 0;                                                            \
    }                                                                        \
    virtual FdoInt32            GetRefCount()       { return m_refCount; }   \
private:                        int m_refCount;
#elif defined(_WIN32)
#define SLT_IMPLEMENT_REFCOUNTING \
public:                                                                      \
    virtual void                Dispose()           { delete this; }         \
    virtual FdoInt32            AddRef()                                     \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag())                    \
            return InterlockedIncrement((LONG*)&m_refCount);                 \
        else                                                                 \
            return ++m_refCount;                                             \
    }                                                                        \
    virtual FdoInt32            Release()                                    \
    {                                                                        \
        if (FdoIDisposable::GetGlobalThreadLockingFlag()){                   \
            if (InterlockedDecrement((LONG*)&m_refCount))                    \
                return m_refCount;                                           \
        }else{                                                               \
            if (--m_refCount)                                                \
                return m_refCount;                                           \
        }                                                                    \
        Dispose();                                                           \
        return 0;                                                            \
    }                                                                        \
    virtual FdoInt32            GetRefCount()       { return m_refCount; }   \
private:                        int m_refCount;
#endif

typedef std::vector<std::string> SltStringList;
