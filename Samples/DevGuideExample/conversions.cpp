// conversions.cpp
//
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
//

#include "stdafx.h"
#include "conversions.h"

char *
Conversions::wchar_tStarToCharStar(const wchar_t *wchar) {
	sprintf(formatStringBuf, "%ls", wchar);
	return formatStringBuf;
}

char *
Conversions::wchar_tStarStarToCharStar(const wchar_t ** wcharList, int listCount) {
	for(int i = 0; i < listCount; i++) {
		strcat(scratchBuf, wchar_tStarToCharStar(wcharList[i]));
		strcat(scratchBuf, " ");
	}
	strcpy(formatStringBuf, scratchBuf);
	return formatStringBuf;
}

wchar_t *
Conversions::charStarToWchar_tStar(const char *str) {
#ifdef _WIN32
	swprintf(charToWcharBuf, L"%hs", str);
#else
	swprintf(charToWcharBuf, sizeof(charToWcharBuf)/sizeof(wchar_t), L"%s", str);
#endif
	return charToWcharBuf;
}

wchar_t *
Conversions::wchar_tStarToUpperCase(const wchar_t *wchar) {
#ifdef _WIN32
	wcscpy(upperCaseWcharBuf, wchar);
	return _wcsupr(upperCaseWcharBuf);
#else
	int len = wcslen(wchar);
	wint_t a_wchar;
	wcscpy(upperCaseWcharBuf, wchar);
	for(int i = 0; i<len; i++) {
		a_wchar = towupper(upperCaseWcharBuf[i]);
		upperCaseWcharBuf[i] = a_wchar;
	}
	return upperCaseWcharBuf;
#endif
}


char *
Conversions::boolToCharStar(const bool b) {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

char *
Conversions::FdoInt32ToCharStar(const FdoInt32 i) {
	sprintf(formatStringBuf, "%d", i);
	return formatStringBuf;
}

char *
Conversions::FdoInt64ToCharStar(const FdoInt64 i) {
	sprintf(formatStringBuf, "%ld", i);
	return formatStringBuf;
}

