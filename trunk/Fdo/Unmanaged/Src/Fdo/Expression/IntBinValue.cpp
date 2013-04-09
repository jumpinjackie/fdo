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
#include <Fdo/Expression/IntBinValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a FdoIntBinValue with data type string and a
// value of null.
FdoIntBinValue* FdoIntBinValue::Create()
{
	return new FdoIntBinValue();
}

// Constructs an instance of a 32 bit binary integer using the specified
// arguments.
FdoIntBinValue* FdoIntBinValue::Create(FdoInt64 value)
{
	return new FdoIntBinValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoIntBinValue::FdoIntBinValue()
{
}

// Constructs an instance of a FdoIntBinValue using the specified
// arguments.
FdoIntBinValue::FdoIntBinValue(FdoInt64 value) : FdoInt64Value(value)
{
}

FdoIntBinValue::~FdoIntBinValue()
{
}

void FdoIntBinValue::Dispose()
{
    delete this;
}

// Returns the well defined text representation of this expression.
FdoString* FdoIntBinValue::ToString()
{
    static const FdoInt32    iBufLen = 68;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }
    szBuf[iBufLen - 1] = '\0';
    szBuf[iBufLen - 2] = '\'';
    
    FdoInt64 val = GetInt64();
    int idx = iBufLen - 3;
    if (val != 0)
    {
        while(val != 0 && (idx - 2) > 0)
        {
            szBuf[idx--] = (val%2 != 0) ? '1' : '0';
            val = val >> 1;
        }
        szBuf[idx--] = '\'';
        szBuf[idx--] = 'B';
    }
    else
    {
        szBuf[idx--] = '0';
        szBuf[idx--] = '\'';
        szBuf[idx--] = 'B';
    }
    m_toString = FdoStringUtility::MakeString(&szBuf[idx+1]);
    return m_toString;
}

FdoIntBinValue* FdoIntBinValue::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoIntBinValue* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoIntBinValue::Create( (FdoInt64)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Int16:
        ret = FdoIntBinValue::Create( (FdoInt64)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoIntBinValue::Create( (FdoInt64)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
        break;

    case FdoDataType_Int64:
        ret = FdoIntBinValue::Create( static_cast<FdoInt64Value*>(src)->GetInt64() );
        break;
    }

    return ret;
}
