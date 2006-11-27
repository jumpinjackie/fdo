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
#include <Fdo/Expression/DoubleValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDoubleValue* FdoDoubleValue::Create()
{
	return new FdoDoubleValue();
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDoubleValue* FdoDoubleValue::Create(double value)
{
	return new FdoDoubleValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDoubleValue::FdoDoubleValue()
{
    m_data = 0.0;
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDoubleValue::FdoDoubleValue(double value)
{
    m_isNull = false;
    m_data = value;
}

FdoDoubleValue::~FdoDoubleValue()
{
}

void FdoDoubleValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoDoubleValue::GetDataType()
{
	return FdoDataType_Double;
}

// Gets the DataValue as a double.
double FdoDoubleValue::GetDouble()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_7_DOUBLEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a double precision floating point number.
void FdoDoubleValue::SetDouble(double value)
{
    m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoDoubleValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessDoubleValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoDoubleValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoStringUtility::FormatDouble(GetDouble(), szBuf, iBufLen);
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

