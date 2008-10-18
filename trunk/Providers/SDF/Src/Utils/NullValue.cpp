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
#include "NullValue.h"
#include "DataValuePool.h"

NullValue::NullValue()
{
}

NullValue::~NullValue()
{
}

DataValue* NullValue::Clone(DataValuePool* pPool)
{
    return pPool->ObtainNullValue();
}

DataValueType NullValue::GetType()
{
    return (DataValueType)Dvt_Null;
}

DataValue* NullValue::Negate(DataValuePool* pPool)
{
    return pPool->ObtainNullValue();
}


DataValue* NullValue::Add(DataValue& v2, DataValuePool* pPool)
{
    return pPool->ObtainNullValue();
}

DataValue* NullValue::Subtract(DataValue& v2, DataValuePool* pPool)
{
   return pPool->ObtainNullValue();
}

DataValue* NullValue::Multiply(DataValue& v2, DataValuePool* pPool)
{
    return pPool->ObtainNullValue();
}

DataValue* NullValue::Divide(DataValue& v2, DataValuePool* pPool)
{
    return pPool->ObtainNullValue();
}

bool NullValue::IsEqualTo(DataValue& v2)
{
    return (v2.GetType() == Dvt_Null);
}

bool NullValue::IsNotEqualTo(DataValue& v2)
{
    return (v2.GetType() != Dvt_Null);
}

bool NullValue::IsGreaterThan(DataValue& v2)
{
    return false;
}

bool NullValue::IsGreaterThanOrEqualTo(DataValue& v2)
{
    return false;
}

bool NullValue::IsLessThan(DataValue& v2)
{
    return false;
}

bool NullValue::IsLessThanOrEqualTo(DataValue& v2)
{
    return false;
}

double NullValue::GetAsDouble()
{
    throw FdoException::Create(L"Cannot convert a Null value to a double");
    return 0.0;
}

FdoInt64 NullValue::GetAsInt64()
{
    throw FdoException::Create(L"Cannot convert a Null value to a FdoInt64");
    return 0;
}

bool NullValue::GetAsBoolean()
{
   throw FdoException::Create(L"Cannot convert a Null value to a bool");
   return false;    
}

FdoDateTime NullValue::GetAsDateTime()
{
    throw FdoException::Create(L"Cannot convert a Null value to DateTime");
    FdoDateTime t;
    return t;
}


wchar_t* NullValue::GetAsString()
{
	return NULL;
}

