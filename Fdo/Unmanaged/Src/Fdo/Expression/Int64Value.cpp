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
#include <Fdo/Expression/Int64Value.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt64Value* FdoInt64Value::Create()
{
	return new FdoInt64Value();
}

// Constructs an instance of a 64 bit integer DataValue using the specified
// arguments.
FdoInt64Value* FdoInt64Value::Create(FdoInt64 value)
{
	return new FdoInt64Value(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt64Value::FdoInt64Value()
{
    m_data = 0;
}

// Constructs an instance of a 64 bit integer DataValue using the specified
// arguments.
FdoInt64Value::FdoInt64Value(FdoInt64 value)
{
	m_isNull = false;
	m_data = value;
}

FdoInt64Value::~FdoInt64Value()
{
}

void FdoInt64Value::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoInt64Value::GetDataType()
{
	return FdoDataType_Int64;
}

// Gets the DataValue as a 64 bit integer.
FdoInt64 FdoInt64Value::GetInt64()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_14_INT64VALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a 64 bit integer.
void FdoInt64Value::SetInt64(FdoInt64 value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoInt64Value::Process(FdoIExpressionProcessor* p)
{
    p->ProcessInt64Value(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoInt64Value::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

#ifdef _WIN32
    _i64tow(GetInt64(), szBuf, 10);
#else
	swprintf(szBuf, sizeof(szBuf)/sizeof(wchar_t), L"%lld", GetInt64());
#endif
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

