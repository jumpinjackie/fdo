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
#include <Fdo/Expression/BooleanValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoBooleanValue* FdoBooleanValue::Create()
{
	return new FdoBooleanValue();
}

// Constructs an instance of a boolean DataValue using the specified
// arguments.
FdoBooleanValue* FdoBooleanValue::Create(bool value)
{
	return new FdoBooleanValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoBooleanValue::FdoBooleanValue()
{
	m_isNull = true;
    m_data = false;
}

// Constructs an instance of a boolean DataValue using the specified
// arguments.
FdoBooleanValue::FdoBooleanValue(bool value)
{
	m_isNull = false;
	m_data = value;
}

FdoBooleanValue::~FdoBooleanValue()
{
}

void FdoBooleanValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoBooleanValue::GetDataType()
{
	return FdoDataType_Boolean;
}

// Gets the DataValue as a boolean.
bool FdoBooleanValue::GetBoolean()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_2_BOOLEANVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a boolean.
void FdoBooleanValue::SetBoolean(bool value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoBooleanValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessBooleanValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoBooleanValue::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    m_toString = FdoStringUtility::MakeString(GetBoolean() ? L"TRUE" : L"FALSE");
    return m_toString;

}

