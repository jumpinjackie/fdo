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
#include <Fdo/Expression/IntHexValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a FdoIntHexValue with data type string and a
// value of null.
FdoIntHexValue* FdoIntHexValue::Create()
{
	return new FdoIntHexValue();
}

// Constructs an instance of a 32 bit hex integer using the specified
// arguments.
FdoIntHexValue* FdoIntHexValue::Create(FdoInt64 value)
{
	return new FdoIntHexValue(value);
}

// Constructs a default instance of a FdoIntHexValue with data type string and a
// value of null.
FdoIntHexValue::FdoIntHexValue()
{
}

// Constructs an instance of a 32 bit hex integer using the specified
// arguments.
FdoIntHexValue::FdoIntHexValue(FdoInt64 value) : FdoInt64Value(value)
{
}

FdoIntHexValue::~FdoIntHexValue()
{
}

void FdoIntHexValue::Dispose()
{
    delete this;
}

// Returns the well defined text representation of this expression.
FdoString* FdoIntHexValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

	swprintf(szBuf, sizeof(szBuf)/sizeof(wchar_t), L"0x%X", GetInt64());
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoIntHexValue* FdoIntHexValue::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoIntHexValue* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoIntHexValue::Create( (FdoInt64)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Int16:
        ret = FdoIntHexValue::Create( (FdoInt64)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoIntHexValue::Create( (FdoInt64)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
        break;

    case FdoDataType_Int64:
        ret = FdoIntHexValue::Create( static_cast<FdoInt64Value*>(src)->GetInt64() );
        break;
    }

    return ret;
}
