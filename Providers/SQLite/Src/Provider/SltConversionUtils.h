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

enum TokenDateFormatType
{
    TokenDateFormatType_Unknown,
    TokenDateFormatType_Separator,
    TokenDateFormatType_Year2,
    TokenDateFormatType_Year4,
    TokenDateFormatType_Month_AbbName_All_Lower,
    TokenDateFormatType_Month_AbbName_All_Upper,
    TokenDateFormatType_Month_AbbName_First_Upper,
    TokenDateFormatType_Month_FullName_All_Lower,
    TokenDateFormatType_Month_FullName_All_Upper,
    TokenDateFormatType_Month_FullName_First_Upper,
    TokenDateFormatType_Month_Number,
    TokenDateFormatType_Day_AbbName_All_Lower,
    TokenDateFormatType_Day_AbbName_All_Upper,
    TokenDateFormatType_Day_AbbName_First_Upper,
    TokenDateFormatType_Day_FullName_All_Lower,
    TokenDateFormatType_Day_FullName_All_Upper,
    TokenDateFormatType_Day_FullName_First_Upper,
    TokenDateFormatType_Day_Number,
    TokenDateFormatType_Hour24,
    TokenDateFormatType_Hour12,
    TokenDateFormatType_Minute,
    TokenDateFormatType_Second,
    TokenDateFormatType_am,
    TokenDateFormatType_pm
};

SLT_API std::string W2A_SLOW(const wchar_t* input);

SLT_API std::wstring A2W_SLOW(const char* input);

SLT_API int A2W_FAST(wchar_t* dst, int wlen, const char* src, int clen);

SLT_API int W2A_FAST(char* dst, int clen, const wchar_t* src, int wlen);


SLT_API FdoDateTime DateFromString(const wchar_t* val, bool excOnErr = true);
SLT_API FdoDateTime DateFromString(const char* val, bool excOnErr = true);
SLT_API void DateToString(FdoDateTime* dt, char* str, int nBytes, bool useFdoStyle = false);

SLT_API void BindPropVals(FdoPropertyValueCollection* props, sqlite3_stmt* stmt, int geomFormat);
SLT_API void BindPropVals(FdoParameterValueCollection* props, sqlite3_stmt* stmt, bool useParmName, int geomFormat);
SLT_API void BindPropValue(sqlite3_stmt* stmt, int i, FdoLiteralValue* lv, int geomFormat);
SLT_API int StringContains(const char* str, const char* val);

SLT_API TokenDateFormatType StringToDateFormat(const char* specifier);

SLT_API char* EnsureNoIsLocalIndep(char* str);
SLT_API wchar_t* EnsureNoIsLocalIndep(wchar_t* str);
SLT_API const char* ExtractDbName(const char* v, int& stStr, int& lenStr);

#endif

