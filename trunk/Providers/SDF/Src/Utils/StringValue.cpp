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
#include "stdafx.h"
#include "StringValue.h"
#include "DataValuePool.h"

const wchar_t* EMPTY_STRING = L"";

StringValue::StringValue(wchar_t* val, bool dispose)
{
    if (val)
    {
        m_value = val;
        m_bDispose = dispose;
    }
    else
    {
        m_value = (wchar_t*)EMPTY_STRING;
        m_bDispose = false;
    }
}

StringValue::~StringValue()
{
    if (m_bDispose)
        delete [] m_value;
}

void StringValue::Set(wchar_t* val, bool dispose)
{
    if (m_bDispose && m_value)
        delete [] m_value;

    if (val)
    {
        m_value = val;
        m_bDispose = dispose;
    }
    else
    {
        m_value = (wchar_t*)EMPTY_STRING;
        m_bDispose = false;
    }
}

DataValue* StringValue::Clone(DataValuePool* pPool)
{
    return pPool->ObtainStringValue(m_value);
}

DataValueType StringValue::GetType()
{
    return Dvt_String;
}

DataValue* StringValue::Negate(DataValuePool* pPool)
{
    throw FdoException::Create(L"Cannot perform arithmetic operation on string");
    return NULL;
}


DataValue* StringValue::Add(DataValue& v2, DataValuePool* pPool)
{
    throw FdoException::Create(L"Cannot perform arithmetic operation on string");
    return NULL;
}

DataValue* StringValue::Subtract(DataValue& v2, DataValuePool* pPool)
{
    throw FdoException::Create(L"Cannot perform arithmetic operation on string");
    return NULL;
}

DataValue* StringValue::Multiply(DataValue& v2, DataValuePool* pPool)
{
    throw FdoException::Create(L"Cannot perform arithmetic operation on string");
    return NULL;
}

DataValue* StringValue::Divide(DataValue& v2, DataValuePool* pPool)
{
    throw FdoException::Create(L"Cannot perform arithmetic operation on string");
    return NULL;
}

bool StringValue::IsEqualTo(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) == 0;
}

bool StringValue::IsNotEqualTo(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) != 0;
}

bool StringValue::IsGreaterThan(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) > 0;
}

bool StringValue::IsGreaterThanOrEqualTo(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) >= 0;
}

bool StringValue::IsLessThan(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) < 0;
}

bool StringValue::IsLessThanOrEqualTo(DataValue& v2)
{
    _ASSERT(m_value);
    return wcscmp(m_value, v2.GetAsString()) <= 0;
}

double StringValue::GetAsDouble()
{
    double d = 0.0;

    _ASSERT(m_value);

    //d = _wtof(m_value);

    int ret = swscanf(m_value, L"%lf", &d);
    _ASSERT(ret == 1);
        
    return d;
}

FdoInt64 StringValue::GetAsInt64()
{
    FdoInt64 n = 0;

    _ASSERT(m_value);

    //n = _wtoi64(m_value);

    int ret = swscanf(m_value, L"%ld", &n);
    _ASSERT(ret == 1);
        
    return n;
}

bool StringValue::GetAsBoolean()
{
    _ASSERT(m_value);

    if (_wcsnicmp(m_value, L"true", 4) == 0)
        return true;

    return false;    
}

FdoDateTime StringValue::GetAsDateTime()
{
    throw FdoException::Create(L"Cannot convert string to DateTime");
    FdoDateTime t;
    return t;
}


wchar_t* StringValue::GetAsString()
{
    return m_value;
}

