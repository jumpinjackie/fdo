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

#include "stdafx.h"
#include "SpatialIndex.h"
#include "SltConversionUtils.h"
#include "StringUtil.h"
#include "SltMetadata.h"
#include "SltGeomUtils.h"
#include <locale.h>

std::string W2A_SLOW(const wchar_t* input)
{
    assert( input!=NULL );
    size_t wlen = wcslen(input);
    int mbslen = (int) wlen * 4 + 1;
    char* mbs = (char*)alloca(mbslen);
    //WideCharToMultiByte(CP_UTF8, 0, input, -1, mbs, mbslen, 0, 0);
    _ut_utf8_from_unicode(input, wlen, mbs, mbslen);
    return std::string(mbs);
}

std::wstring A2W_SLOW(const char* input)
{
    assert( input!=NULL );
    int wlen = (int)strlen(input) + 1;
    wchar_t* ws = (wchar_t*)alloca(sizeof(wchar_t)*wlen);
    //MultiByteToWideChar(CP_UTF8, 0, input, -1, ws, wlen);
    _ut_utf8_to_unicode(input, wlen, ws, wlen);
    return std::wstring(ws);
}

int A2W_FAST(wchar_t* dst, int wlen, const char* src, int clen)
{
    return _ut_utf8_to_unicode(src, clen, dst, wlen);
}

int W2A_FAST(char* dst, int clen, const wchar_t* src, int wlen)
{
    return _ut_utf8_from_unicode(src, wlen, dst, clen);
}

int StringContains(const char* str, const char* val)
{
    assert( str!=NULL );
    const char* strTmp = str;
    while (*strTmp != '\0')
    {
        const char* valTmp = val;
        int ret = (int)(strTmp - str);
        if(sqlite3UpperToLower[*strTmp] == sqlite3UpperToLower[*valTmp])
        {
            while(sqlite3UpperToLower[*strTmp++] == sqlite3UpperToLower[*valTmp++])
            {
                if (*valTmp == '\0')
                    return ret;
            }
        }
        else
            strTmp++;
    }
    return -1;
}

FdoDateTime DateFromString(const wchar_t* val, bool excOnErr)
{
    assert( val!=NULL );
    // to avoid stack corruption declare following variables
    // since swscanf expects "Type of argument" = "Pointer to int"
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    int minute = -1;
    float seconds = 0.0f;

    const wchar_t* valtmp = val;
    int formatDate = 0x00;
    while(*valtmp != '\0' && valtmp-val < 30)
    {
        if (*valtmp == '-')
            formatDate |= 0x01;
        else if (*valtmp == ':')
        {
            formatDate |= 0x02;
            break;
        }
        else if (*valtmp == ' ')
        {
            formatDate |= 0x04;
            break;
        }
        else if (*valtmp == 'T')
        {
            formatDate |= 0x08;
            break;
        }
        valtmp++;
    }
    
    int res = 0;
    if (formatDate != 0x00)
    {
        if ((formatDate&(~0x01)) == 0x00) // it's a date
            res = swscanf(val, L"%d-%d-%d", &year, &month, &day);
        else if ((formatDate&(~0x02)) == 0x00) // it's a time
            res = swscanf(val, L"%d:%d:%f", &hour, &minute, &seconds);
        else if ((formatDate&(~0x07)) == 0x00) // it's a datetime using space
            res = swscanf(val, L"%d-%d-%d %d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);
        else  if ((formatDate&(~0x0B)) == 0x00) // it's a datetime using T
            res = swscanf(val, L"%d-%d-%dT%d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);
    }
    if (res != 3 && res != 5 && res != 6)
    {
        if (excOnErr)
            throw FdoException::Create(L"Failed to parse DateTime.");
        else
            return FdoDateTime();
    }
    
    return FdoDateTime((FdoInt16)year, (FdoInt8)month, (FdoInt8)day, (FdoInt8)hour, (FdoInt8)minute, seconds);
}

FdoDateTime DateFromString(const char* val, bool excOnErr)
{
    assert( val!=NULL );
    // to avoid stack corruption declare following variables
    // since swscanf expects "Type of argument" = "Pointer to int"
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    int minute = -1;
    float seconds = 0.0f;

    const char* valtmp = val;
    int formatDate = 0x00;
    while(*valtmp != '\0' && valtmp-val < 30)
    {
        if (*valtmp == '-')
            formatDate |= 0x01;
        else if (*valtmp == ':')
        {
            formatDate |= 0x02;
            break;
        }
        else if (*valtmp == ' ')
        {
            formatDate |= 0x04;
            break;
        }
        else if (*valtmp == 'T')
        {
            formatDate |= 0x08;
            break;
        }
        valtmp++;
    }
    
    int res = 0;
    if (formatDate != 0x00)
    {
        if ((formatDate&(~0x01)) == 0x00) // it's a date
            res = sscanf(val, "%d-%d-%d", &year, &month, &day);
        else if ((formatDate&(~0x02)) == 0x00) // it's a time
            res = sscanf(val, "%d:%d:%f", &hour, &minute, &seconds);
        else if ((formatDate&(~0x07)) == 0x00) // it's a datetime using space
            res = sscanf(val, "%d-%d-%d %d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);
        else  if ((formatDate&(~0x0B)) == 0x00) // it's a datetime using T
            res = sscanf(val, "%d-%d-%dT%d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);
    }
    if (res != 3 && res != 5 && res != 6)
    {
        if (excOnErr)
            throw FdoException::Create(L"Failed to parse DateTime.");
        else
            return FdoDateTime();
    }
    
    return FdoDateTime((FdoInt16)year, (FdoInt8)month, (FdoInt8)day, (FdoInt8)hour, (FdoInt8)minute, seconds);
}

void DateToString(FdoDateTime* dt, char* s, int nBytes, bool useFdoStyle)
{
    if (dt->IsDate())
    {
        _snprintf(s, nBytes, "%04d-%02d-%02d", dt->year, dt->month, dt->day);
    }
    else if (dt->IsTime())
    {
        if (useFdoStyle)
            _snprintf(s, nBytes, "%02d:%02d:%02g", dt->hour, dt->minute, dt->seconds);
        else 
            _snprintf(s, nBytes, "%02d:%02d:%s%0.3f", dt->hour, dt->minute, (dt->seconds >10.0)?"":"0",dt->seconds);

        EnsureNoIsLocalIndep(s);
    }
    else
    {
        if (useFdoStyle)
            _snprintf(s, nBytes, "%04d-%02d-%02d %02d:%02d:%02g", dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->seconds);
        else
            _snprintf(s, nBytes, "%04d-%02d-%02dT%02d:%02d:%s%0.3f", dt->year, dt->month, dt->day, dt->hour, dt->minute, (dt->seconds >10.0)?"":"0",dt->seconds);
        
        EnsureNoIsLocalIndep(s);
    }
}

//binds specific values to the parameters in a pre-compiled
//sql statement -- assumes parameters in the sql are in the
//same order as the given property value collection.
void BindPropVals(FdoPropertyValueCollection* props, sqlite3_stmt* stmt, int geomFormat)
{
    int max = props->GetCount();
    for (int i=1; i<=max; i++)
    {
        FdoPtr<FdoPropertyValue> pv = props->GetItem(i-1);
        FdoPtr<FdoValueExpression> ve = pv->GetValue();

        //Assume it is a literal value : this seems like
        //a reasonable assumption
        FdoLiteralValue* lv = (FdoLiteralValue*) ve.p;


        //not sure if this is allowed by the API, but let's
        //handle it anyway -- it's pretty obvious what to do.
        if (ve.p == NULL)
        {
            sqlite3_bind_null(stmt, i);
            continue;
        }
        BindPropValue(stmt, i, lv, geomFormat );
    }
}

void BindPropVals(FdoParameterValueCollection* props, sqlite3_stmt* stmt, bool useParmName, int geomFormat )
{
    StringBuffer sb;
    for(int i=1; i<=props->GetCount(); i++)
    {
        FdoPtr<FdoParameterValue> fdoParm = props->GetItem(i-1);
        FdoPtr<FdoLiteralValue> parmValue = fdoParm->GetValue();
        
        int index = i;
        if( useParmName )
        {         
            sb.Reset();
            sb.Append(":");
            sb.Append(fdoParm->GetName());
            index = sqlite3_bind_parameter_index(stmt, sb.Data());
            if( index == 0 )
                continue;
        }

        if (parmValue.p == NULL)
        {          
            sqlite3_bind_null(stmt, index);
            continue;
        }
        BindPropValue(stmt, index, parmValue, geomFormat );
    }
}

void BindPropValue(sqlite3_stmt* stmt, int i, FdoLiteralValue* lv, int geomFormat)
{
    int rc;
    if (lv->GetLiteralValueType() == FdoLiteralValueType_Data)
    {
        FdoDataValue* dv = (FdoDataValue*)lv;
        FdoDataType dt = dv->GetDataType();

        if (dv->IsNull())
        {
            sqlite3_bind_null(stmt, i);
            return;
        }

        switch (dt)
        {
            case FdoDataType_BLOB:
                {
                    FdoLOBValue* v = (FdoLOBValue*)dv;
                    FdoPtr<FdoByteArray> ba = v->GetData();
                    rc = sqlite3_bind_blob(stmt, i, ba->GetData(), ba->GetCount(), SQLITE_TRANSIENT);
                }
                break;
            case FdoDataType_Boolean:
                {
                    FdoBooleanValue* v = (FdoBooleanValue*)dv;
                    bool b = v->GetBoolean();
                    int val = b ? 1 : 0;
                    rc = sqlite3_bind_int(stmt, i, val);
                }
                break;
            case FdoDataType_Byte:
                {
                    FdoByteValue* v = (FdoByteValue*)dv;
                    int b = v->GetByte();
                    rc = sqlite3_bind_int(stmt, i, b);
                }
                break;
            case FdoDataType_CLOB:
                printf ("CLOB not implemented!\n");
                throw;
                break;
            case FdoDataType_DateTime:
                {
                    FdoDateTimeValue* v = (FdoDateTimeValue*)dv;
                    FdoDateTime dtm = v->GetDateTime();
                    char s[31];

                    DateToString(&dtm, s, 31);
                    
                    rc = sqlite3_bind_text(stmt, i, s, -1, SQLITE_TRANSIENT);
                }
                break;
            case FdoDataType_Decimal:
                {
                    FdoDoubleValue* v = (FdoDoubleValue*)dv;
                    double d = v->GetDouble();
                    rc = sqlite3_bind_double(stmt, i, d);
                }
            case FdoDataType_Double:
                {
                    FdoDoubleValue* v = (FdoDoubleValue*)dv;
                    double d = v->GetDouble();
                    rc = sqlite3_bind_double(stmt, i, d);
                }
                break;
            case FdoDataType_Int16:
                {
                    FdoInt16Value* v = (FdoInt16Value*)dv;
                    int val = v->GetInt16();
                    rc = sqlite3_bind_int(stmt, i, val);
                }
                break;
            case FdoDataType_Int32:
                {
                    FdoInt32Value* v = (FdoInt32Value*)dv;
                    int val = v->GetInt32();
                    rc = sqlite3_bind_int(stmt, i, val);
                }
                break;
            case FdoDataType_Int64:
                {
                    FdoInt64Value* v = (FdoInt64Value*)dv;
                    sqlite3_int64 val = v->GetInt64();
                    rc = sqlite3_bind_int64(stmt, i, val);
                }
                break;
            case FdoDataType_Single:
                {
                    FdoSingleValue* v = (FdoSingleValue*)dv;
                    double d = v->GetSingle();
                    rc = sqlite3_bind_double(stmt, i, d);
                }
                break;
            case FdoDataType_String:
                {
                    FdoStringValue* v = (FdoStringValue*)dv;
                    FdoString* s = v->GetString();
                    size_t wlen = wcslen(s);
                    size_t clen = wlen*4+1;
                    char* mbs = (char*)alloca(clen);
                    W2A_FAST(mbs, clen, s, wlen);
                    rc = sqlite3_bind_text(stmt, i, mbs, -1, SQLITE_TRANSIENT);
                }
                break;
        }
    }
    else
    {
        FdoGeometryValue* gv = (FdoGeometryValue*)lv;

        if (gv->IsNull())
        {
            sqlite3_bind_null(stmt, i);
            return;
        }

        FdoPtr<FdoByteArray> ba = gv->GetGeometry();

        //Convert geometry byte array to specified target internal binary format
        if (geomFormat == eFGF)
        {
            rc = sqlite3_bind_blob(stmt, i, ba->GetData(), ba->GetCount(), SQLITE_TRANSIENT);
        }
        else if (geomFormat == eWKB)
        {
            unsigned char* wkb = new unsigned char[ba->GetCount()]; //WKB is a bit smaller than FGF so space should be enough here
            int len = Fgf2Wkb(ba->GetData(), wkb);
            rc = sqlite3_bind_blob(stmt, i, wkb, len, SQLITE_TRANSIENT);
            delete [] wkb;
        }
        else if (geomFormat == eWKT)
        {
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> g = gf->CreateGeometryFromFgf(ba);
            std::string wkt = W2A_SLOW(g->GetText());
            rc = sqlite3_bind_blob(stmt, i, wkt.c_str(), -1, SQLITE_TRANSIENT); 
        }

    }
}

SLT_API TokenDateFormatType StringToDateFormat(const char* specifier)
{
    if (strncmp(specifier, "YYYY", 4) == 0)
        return TokenDateFormatType_Year4;

    if (strncmp(specifier, "YY", 2) == 0)
        return TokenDateFormatType_Year2;

    if (strncmp(specifier, "MONTH", 5) == 0)
        return TokenDateFormatType_Month_FullName_All_Upper;

    if (strncmp(specifier, "Month", 5) == 0)
        return TokenDateFormatType_Month_FullName_First_Upper;

    if (strncmp(specifier, "month", 5) == 0)
        return TokenDateFormatType_Month_FullName_All_Lower;

    if (strncmp(specifier, "MON", 3) == 0)
        return TokenDateFormatType_Month_AbbName_All_Upper;

    if (strncmp(specifier, "mon", 3) == 0)
        return TokenDateFormatType_Month_AbbName_All_Lower;

    if (strncmp(specifier, "Mon", 3) == 0)
        return TokenDateFormatType_Month_AbbName_First_Upper;

    if (strncmp(specifier, "MM", 2) == 0)
        return TokenDateFormatType_Month_Number;

    if (strncmp(specifier, "DAY", 3) == 0)
        return TokenDateFormatType_Day_FullName_All_Upper;

    if (strncmp(specifier, "Day", 3) == 0)
        return TokenDateFormatType_Day_FullName_First_Upper;

    if (strncmp(specifier, "day", 3) == 0)
        return TokenDateFormatType_Day_FullName_All_Lower;

    if (strncmp(specifier, "DY", 2) == 0)
        return TokenDateFormatType_Day_AbbName_All_Upper;

    if (strncmp(specifier, "dy", 2) == 0)
        return TokenDateFormatType_Day_AbbName_All_Lower;

    if (strncmp(specifier, "Dy", 2) == 0)
        return TokenDateFormatType_Day_AbbName_First_Upper;

    if (strncmp(specifier, "DD", 2) == 0)
        return TokenDateFormatType_Day_Number;

    if (strncmp(specifier, "hh24", 4) == 0 || strncmp(specifier, "hh", 2) == 0)
        return TokenDateFormatType_Hour24;

    if (strncmp(specifier, "hh12", 4) == 0)
        return TokenDateFormatType_Hour12;

    if (strncmp(specifier, "mm", 2) == 0)
        return TokenDateFormatType_Minute;

    if (strncmp(specifier, "ss", 2) == 0)
        return TokenDateFormatType_Second;

    if (strncmp(specifier, "am", 2) == 0 || strncmp(specifier, "AM", 2) == 0)
        return TokenDateFormatType_am;

    if (strncmp(specifier, "pm", 2) == 0 || strncmp(specifier, "PM", 2) == 0)
        return TokenDateFormatType_pm;

    // should we thow an exception !?
    // we lose custom formats like "My date YY/MM/DD"
    return TokenDateFormatType_Unknown;
}

char* EnsureNoIsLocalIndep(char* str)
{
    char* strtmp = str;
    struct lconv* nls = localeconv();
    char radix = (nls != NULL && *nls->decimal_point != '\0') ? *nls->decimal_point : '.';
    if (radix == '.')
    {
        while(*strtmp != '\0' && *strtmp != radix) strtmp++;
        if (*strtmp == radix)
            *strtmp = '.';
    }
    return str;
}

wchar_t* EnsureNoIsLocalIndep(wchar_t* str)
{
    wchar_t* strtmp = str;
    struct lconv* nls = localeconv();
    wchar_t radix = (nls != NULL && *nls->decimal_point != L'\0') ? *nls->decimal_point : L'.';
    if (radix == L'.')
    {
        while(*strtmp != L'\0' && *strtmp != radix) strtmp++;
        if (*strtmp == radix)
            *strtmp = L'.';
    }
    return str;
}
