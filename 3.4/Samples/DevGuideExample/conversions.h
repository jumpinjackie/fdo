#ifndef _CONVERSIONS_H_
#define _CONVERSIONS_H_
//
// Copyright (C) 2008  Autodesk, Inc.
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

#include <cstring>
#include <wchar.h>
#ifdef _WIN32
#include <cstdio>
#else
#include <stdio.h>
#endif
#include <ostream>
#include <sstream>
#include <Common/FdoTypes.h>
#ifndef _WIN32
#include <wctype.h>
#endif


using namespace std;

class Conversions {
public:
	char * wchar_tStarToCharStar(const wchar_t *wchar);
	char * wchar_tStarStarToCharStar(const wchar_t ** wcharList, int listCount);
	char * boolToCharStar(const bool b);
	char * FdoInt32ToCharStar(const FdoInt32 i);
	char * FdoInt64ToCharStar(const FdoInt64 i);
	wchar_t * charStarToWchar_tStar(const char *str);
	wchar_t * wchar_tStarToUpperCase(const wchar_t *wchar);

private:
     char formatStringBuf[4096];
	 wchar_t upperCaseWcharBuf[512];
	 wchar_t charToWcharBuf[512];
	 char scratchBuf[4096];
};

#endif

