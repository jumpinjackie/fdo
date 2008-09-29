/*
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
 */

#ifndef OSUTILS_H
#define OSUTILS_H

#include <malloc.h>

///////////////////////////////////////////
// Operating System-dependent functionality:
///////////////////////////////////////////

// c-style declarations:
#ifdef _WIN32
#define OS__cdecl __cdecl
#else // not _WIN32
#define OS__cdecl
#endif

// format a wide-char string to a wide-char string:
int OS_swprintf(wchar_t* buffer, size_t bufferSize, const wchar_t* format, ...);

// convert a double to an ascii string:
char* OS_gcvt(double number, size_t ndigit, char *buf);

// compare two ascii strings in a case-insensitive manner:
int OS_stricmp(const char *s1, const char *s2);

// compare two unicode strings in a case-insensitive manner:
int OS_wcsicmp(const wchar_t *s1, const wchar_t *s2);
int OS_wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t len);

// convert an int to an ascii string (radix is always 10):
char* OS_itoa(int value, char* buffer);

// convert a long to an ascii string (radix is always 10):
char* OS_ltoa(long value, char* buffer);

// convert an int to a wide string (radix is always 10):
wchar_t* OS_itow(int value, wchar_t* buffer, size_t bufferSize);

// convert a long to a wide string (radix is always 10):
wchar_t* OS_ltow(long value, wchar_t* buffer, size_t bufferSize);


// convert a string to an int (radix is always 10):
int OS_wtoi(const wchar_t* value);

// multibyte character string functions:
int OS_ismbslead(const unsigned char *string, const unsigned char *current);
int OS_ismbstrail(const unsigned char *string, const unsigned char *current);
int OS_ismbcalnum(const unsigned char *string, long stringLength, const unsigned char *current);
int OS_ismbcalpha(const unsigned char *string, long stringLength, const unsigned char *current);

// get current ssytem time:
void OS_getsystime(struct tm *systime);

// tokenize a unicode string:
wchar_t* OS_wcstok(wchar_t *tokens, const wchar_t *delimiters, wchar_t **state);



// check that 2 doubles are roughly equal:
bool fuzzyEqual(const double d1, const double d2);

#endif // OSUTILS_H

