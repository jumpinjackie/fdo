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

#ifndef SQLITE_UTILS_H
#define SQLITE_UTILS_H

struct DBounds;
struct sqlite3_stmt;

SLT_API std::string W2A_SLOW(const wchar_t* input);

SLT_API std::wstring A2W_SLOW(const char* input);

SLT_API int A2W_FAST(wchar_t* dst, int wlen, const char* src, int clen);

SLT_API int W2A_FAST(char* dst, int clen, const wchar_t* src, int wlen);


SLT_API FdoDateTime DateFromString(const wchar_t* val);
SLT_API FdoDateTime DateFromString(const char* val);
SLT_API void DateToString(FdoDateTime* dt, char* str, int nBytes);

SLT_API void BindPropVals(FdoPropertyValueCollection* props, sqlite3_stmt* stmt);
SLT_API void BindPropVals(FdoParameterValueCollection* props, sqlite3_stmt* stmt);
SLT_API void BindPropValue(sqlite3_stmt* stmt, int i, FdoLiteralValue* lv);

#endif

