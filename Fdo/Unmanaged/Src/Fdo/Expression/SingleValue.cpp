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
#include <Fdo/Expression/SingleValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoSingleValue* FdoSingleValue::Create()
{
	return new FdoSingleValue();
}

// Constructs an instance of a single precision floating point DataValue
// using the specified arguments.
FdoSingleValue* FdoSingleValue::Create(float value)
{
	return new FdoSingleValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoSingleValue::FdoSingleValue()
{
    m_data = 0.0;
}

// Constructs an instance of a single precision floating point DataValue
// using the specified arguments.
FdoSingleValue::FdoSingleValue(float value)
{
	m_isNull = false;
	m_data = value;
}

FdoSingleValue::~FdoSingleValue()
{
}

void FdoSingleValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoSingleValue::GetDataType()
{
	return FdoDataType_Single;
}

// Gets the DataValue as a single precision floating point number.
float FdoSingleValue::GetSingle()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_18_SINGLEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a single precision floating point number.
void FdoSingleValue::SetSingle(float value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoSingleValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessSingleValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoSingleValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoStringUtility::FormatSingle(GetSingle(), szBuf, iBufLen);
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

