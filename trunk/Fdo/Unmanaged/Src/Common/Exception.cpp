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
#include <FdoCommon.h>
#include <stdarg.h>
#include "StringUtility.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif


#ifdef _WIN32
#define CATALOG		"FDOMessage.dll"
#else
#define CATALOG		"FDOMessage.cat"
#endif
#ifdef _DEBUG
    static const    FdoInt32    countBuffers = 4;
    static const    FdoInt32    sizeBuffers = 5319;
    static          wchar_t     szDebugMessage[countBuffers][sizeBuffers];
    static          FdoInt32    iDebugMessage;
#endif

#ifdef _WIN32
extern "C" {CRITICAL_SECTION NlsMsgGetCriticalSection;}
#else
extern "C" {pthread_mutex_t NlsMsgGetCriticalSection = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;}
#endif


class NlsMsgGetAccess
{
public:
    NlsMsgGetAccess()
    {
#ifdef _WIN32
        InitializeCriticalSection (&NlsMsgGetCriticalSection);
#else
    /* nothing */
#endif
    }
    ~NlsMsgGetAccess()
    {
#ifdef _WIN32
    DeleteCriticalSection (&NlsMsgGetCriticalSection);
#else
    pthread_mutex_destroy(&NlsMsgGetCriticalSection);
#endif
    }
};

static NlsMsgGetAccess NlsMsgGetInitializer;

extern "C" 
wchar_t * nls_msg_get_W2(wchar_t *msg_string, 
                        char *cat_name, 
                        int set_num, 
                        unsigned long msg_num, // was DWORD 
                        char *default_msg, 
                        va_list arguments); 

FdoException* FdoException::Create()
{
	return new FdoException();
}

FdoException* FdoException::Create(FdoString* message)
{
	return new FdoException(message);
}

FdoException* FdoException::Create(FdoString* message, FdoException* cause)
{
	return new FdoException(message, cause);
}

FdoException::FdoException()
{
	m_message = NULL;
	m_cause = NULL;
}

FdoException::FdoException(FdoString* message)
{
    m_message = FdoStringUtility::MakeString(message);
	m_cause = NULL;
}

FdoException::FdoException(FdoString* message, FdoException* cause)
{
	m_message = FdoStringUtility::MakeString(message);
	m_cause = FDO_SAFE_ADDREF(cause);
}

FdoException::~FdoException()
{
    FdoStringUtility::ClearString(m_message);
    FDO_SAFE_RELEASE(m_cause);
}

void FdoException::Dispose()
{
    delete this;
}

FdoString* FdoException::GetExceptionMessage()
{
    return m_message;
}

FdoException* FdoException::GetCause()
{
    return FDO_SAFE_ADDREF(m_cause);
}

FdoException* FdoException::GetRootCause()
{
	FdoException*	pRoot = NULL;
	
	if (m_cause != NULL)
		pRoot = m_cause->GetRootCause();
	else 
		pRoot = FDO_SAFE_ADDREF(this);
    return pRoot;
}

void FdoException::SetCause(FdoException* cause)
{
    FDO_SAFE_ADDREF(cause);
    FDO_SAFE_RELEASE(m_cause);
    m_cause = cause;
}

FdoString* FdoException::ToString()
{
    return NULL;
}

FdoString* FdoException::NLSGetMessage(FdoInt32 msgNum, char* defMsg, char* file, int line, ...)
{
    va_list arguments;

    va_start(arguments, line);
    FdoString*  result = NLSGetMessage(msgNum, defMsg, file, line, CATALOG, arguments);
    va_end(arguments);
    return result;
}

FdoString* FdoException::NLSGetMessage(FdoInt32 msgNum, char* defMsg, char* file, int line, char* catalog, va_list arguments)
{
#ifdef  _DEBUG

    FdoString*  pMsg = nls_msg_get_W2(NULL, catalog, 1, msgNum, defMsg, arguments);
    wchar_t*    pBuffer = szDebugMessage[iDebugMessage];
    iDebugMessage = (iDebugMessage + 1) % countBuffers;

    _snwprintf(pBuffer, sizeBuffers - 1, L"(%S: %i) %s", file, line, pMsg);
    pBuffer[sizeBuffers - 1] = 0;
    return pBuffer;
#else
	// eliminate unreferenced parameter warning
	(void)file;
	(void)line;
    return nls_msg_get_W2(NULL, catalog, 1, msgNum, defMsg, arguments); 
#endif
}

FdoString* FdoException::NLSGetMessage(FdoInt32 msgNum, char* defMsg, ...)
{
    va_list arguments;

    va_start(arguments, defMsg);
    FdoString*  result = NLSGetMessage(msgNum, defMsg, CATALOG, arguments);
    va_end(arguments);
    return result;
}

FdoString* FdoException::NLSGetMessage(FdoInt32 msgNum, char* defMsg, char* catalog, va_list arguments)
{
    return nls_msg_get_W2(NULL, catalog, 1, msgNum, defMsg, arguments); 
}

