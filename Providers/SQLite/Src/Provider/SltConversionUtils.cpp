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

std::string W2A_SLOW(const wchar_t* input)
{
    size_t wlen = wcslen(input);
    int mbslen = (int) wlen * 4 + 1;
    char* mbs = (char*)alloca(mbslen);
    //WideCharToMultiByte(CP_UTF8, 0, input, -1, mbs, mbslen, 0, 0);
    _ut_utf8_from_unicode(input, wlen, mbs, mbslen);
    return std::string(mbs);
}

std::wstring A2W_SLOW(const char* input)
{
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


FdoDateTime DateFromString(const wchar_t* val)
{
    // to avoid stack corruption declare following variables
    // since swscanf expects "Type of argument" = "Pointer to int"
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    int minute = -1;
    float seconds = 0.0f;

    int res = swscanf(val, L"%d-%d-%dT%d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);

    if (res != 3 && res != 5 && res != 6)
        throw FdoException::Create(L"Failed to parse DateTime.");
    
    return FdoDateTime((FdoInt16)year, (FdoInt8)month, (FdoInt8)day, (FdoInt8)hour, (FdoInt8)minute, seconds);
}

FdoDateTime DateFromString(const char* val)
{
    // to avoid stack corruption declare following variables
    // since swscanf expects "Type of argument" = "Pointer to int"
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    int minute = -1;
    float seconds = 0.0f;

    int res = sscanf(val, "%d-%d-%dT%d:%d:%f", &year, &month, &day, &hour, &minute, &seconds);

    if (res != 3 && res != 5 && res != 6)
        throw FdoException::Create(L"Failed to parse DateTime.");
    
    return FdoDateTime((FdoInt16)year, (FdoInt8)month, (FdoInt8)day, (FdoInt8)hour, (FdoInt8)minute, seconds);
}

void DateToString(FdoDateTime* dt, char* s, int nBytes)
{
    if (dt->IsDate())
    {
        _snprintf(s, nBytes, "%04d-%02d-%02d", dt->year, dt->month, dt->day);
    }
    else if (dt->IsTime())
    {
        _snprintf(s, nBytes, "%02d:%02d:%0.3f", dt->hour, dt->minute, dt->seconds);
    }
    else
    {
        _snprintf(s, nBytes, "%04d-%02d-%02dT%02d:%02d:%0.3f", dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->seconds);
    }
}

//binds specific values to the parameters in a pre-compiled
//sql statement -- assumes parameters in the sql are in the
//same order as the given property value collection.
void BindPropVals(FdoPropertyValueCollection* props, sqlite3_stmt* stmt)
{
    for (int i=1; i<=props->GetCount(); i++)
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
        BindPropValue(stmt, i, lv );
    }
}

void BindPropVals(FdoParameterValueCollection* props, sqlite3_stmt* stmt)
{
    for(int i=1; i<=props->GetCount(); i++)
    {
        FdoPtr<FdoParameterValue> fdoParm = props->GetItem(i-1);
        FdoPtr<FdoLiteralValue> parmValue = fdoParm->GetValue();
        if (parmValue.p == NULL)
        {
            sqlite3_bind_null(stmt, i);
            continue;
        }
        BindPropValue(stmt, i, parmValue );
    }
}

void BindPropValue(sqlite3_stmt* stmt, int i, FdoLiteralValue* lv)
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
                    char s[64];

                    DateToString(&dtm, s, 64);
                    
                    rc = sqlite3_bind_text(stmt, i, s, -1, SQLITE_TRANSIENT);
                }
                break;
            case FdoDataType_Decimal:
                {
                    FdoDoubleValue* v = (FdoDoubleValue*)dv;
                    double d = v->GetDouble();
                    rc = sqlite3_bind_int(stmt, i, (int)d);
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
        rc = sqlite3_bind_blob(stmt, i, ba->GetData(), ba->GetCount(), SQLITE_TRANSIENT);
    }
}


