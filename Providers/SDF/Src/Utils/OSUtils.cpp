/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */

#include "stdafx.h"
#include <math.h>
#include <time.h>

#include "OSUtils.h"

#ifdef _WIN32
#include "mbstring.h" // for multibyte functions
#else //_WIN32
#include <wctype.h>  // for wide-char functions
#endif

bool fuzzyEqual(const double d1, const double d2)
{
    if ((d1==0.0) || (d2==0.0))
        return 1e-5 > fabs(d1 - d2);
    else
        return 1e-5 > fabs(1.0 - (d2 / d1));
}


int OS_swprintf(wchar_t* buffer, size_t bufferSize, const wchar_t* format, ...)
{
    int retVal = 0;
    va_list  varargs;
    
    va_start(varargs, format);

    retVal = vswprintf(buffer, bufferSize, format, varargs);

    va_end(varargs);

    return retVal;
}


char *OS_gcvt(double number, size_t ndigits, char *buf)
{
#ifdef _WIN32
    return _gcvt (number, (int)ndigits, buf);
#else
    return gcvt (number, (int)ndigits, buf);
#endif
}


int OS_stricmp(const char *s1, const char *s2)
{
#ifdef _WIN32
    return _stricmp(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}

int OS_wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
#ifdef _WIN32
    return _wcsicmp(s1, s2);
#else
    return wcscasecmp(s1, s2);
#endif
}

int OS_wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t len)
{
#ifdef _WIN32
    return _wcsnicmp (s1, s2, len);
#else
    return wcsncasecmp (s1, s2, len);
#endif
}

char* OS_itoa(int value, char* buffer)
{
#ifdef _WIN32
    return _itoa(value, buffer, 10); // radix is always 10
#else
    sprintf(buffer, "%d", value);
    return buffer;
#endif
}

char* OS_ltoa(long value, char* buffer)
{
#ifdef _WIN32
    return _ltoa(value, buffer, 10); // radix is always 10
#else
    sprintf(buffer, "%ld", value);
    return buffer;
#endif
}

wchar_t* OS_itow(int value, wchar_t* buffer, size_t bufferSize)
{
#ifdef _WIN32
    return _itow(value, buffer, 10); // radix is always 10
#else
    OS_swprintf(buffer, bufferSize, L"%d", value);
    return buffer;
#endif
}

wchar_t* OS_ltow(long value, wchar_t* buffer, size_t bufferSize)
{
#ifdef _WIN32
    return _ltow(value, buffer, 10); // radix is always 10
#else
    OS_swprintf(buffer, bufferSize, L"%ld", value);
    return buffer;
#endif
}


int OS_wtoi(const wchar_t* value)
{
#ifdef _WIN32
    return _wtoi(value);
#else
    return (int)wcstol(value, NULL, 10);  // radix is always 10
#endif
}

int OS_ismbslead(const unsigned char *string, const unsigned char *current)
{
#ifdef _WIN32
    return _ismbslead(string, current);
#else // _WIN32
    mbstate_t pstate;
    int i = 0;
    int curPos = 0;
    int inquiryPos = (int)(current - string);
    int ret = 0;

    do
    {
        if (inquiryPos == curPos)
	    ret = -1;

        i = mbrlen((const char*)string, strlen((const char*)string), &pstate);
        if (i > 0)
	    curPos += i;
    }
    while (i > 0);

    return ret;
#endif
}

int OS_ismbstrail(const unsigned char *string, const unsigned char *current)
{
#ifdef _WIN32
    return _ismbstrail(string, current);
#else // _WIN32
    return OS_ismbslead(string, current)==-1 ? 0 : -1;
#endif
}

int OS_ismbcalnum(const unsigned char *string, long stringLength, const unsigned char *current)
{
    wchar_t wBuffer;
    mbstate_t pstate;
    memset(&pstate, 0, sizeof(pstate));
    size_t lStatus = mbrtowc(&wBuffer, (const char*)current, stringLength - (current-string), &pstate);
    if (lStatus<0)
        throw FdoException::Create(L"Unexpected error encountered.");
    return iswalnum(wBuffer) ? -1 : 0;
}

int OS_ismbcalpha(const unsigned char *string, long stringLength, const unsigned char *current)
{
    wchar_t wBuffer;
    mbstate_t pstate;
    memset(&pstate, 0, sizeof(pstate));
    size_t lStatus = mbrtowc(&wBuffer, (const char*)current, stringLength - (current-string), &pstate);
    if (lStatus<0)
        throw FdoException::Create(L"Unexpected error encountered.");
    return iswalpha(wBuffer) ? -1 : 0;
}

void OS_getsystime(struct tm *systime)
{
#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    systime->tm_sec = st.wSecond; 
    systime->tm_min = st.wMinute; 
    systime->tm_hour = st.wHour; 
    systime->tm_mday = st.wDay; 
    systime->tm_mon = st.wMonth - 1; 
    systime->tm_year = st.wYear - 1900; 
    systime->tm_wday = st.wDayOfWeek; 
#else
     time_t current;
     time (&current);
     localtime_r (&current, systime);
#endif
}


wchar_t* OS_wcstok(wchar_t *tokens, const wchar_t *delimiters, wchar_t **state)
{
#ifdef _WIN32
    return wcstok(tokens, delimiters);
#else
    return wcstok(tokens, delimiters, state);
#endif
}

