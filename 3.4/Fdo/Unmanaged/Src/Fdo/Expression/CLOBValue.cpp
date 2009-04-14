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
#include <Fdo/Expression/CLOBValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoCLOBValue* FdoCLOBValue::Create()
{
	return new FdoCLOBValue();
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoCLOBValue* FdoCLOBValue::Create(FdoByteArray* value)
{
	return new FdoCLOBValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoCLOBValue::FdoCLOBValue()
{
    m_data = NULL;
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoCLOBValue::FdoCLOBValue(FdoByteArray* value)
{
    if (value == NULL)
    {
		m_isNull = true;
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_4_CLOBVALUENULL)));
        throw pExcept;
    }
	m_isNull = false;
	m_data = FDO_SAFE_ADDREF(value);
}

FdoCLOBValue::~FdoCLOBValue()
{
    FDO_SAFE_RELEASE(m_data);
}

void FdoCLOBValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoCLOBValue::GetDataType()
{
	return FdoDataType_CLOB;
}

// Gets the DataValue as a CLOB.
FdoByteArray* FdoCLOBValue::GetData()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_4_CLOBVALUENULL)));
        throw pExcept;
    }
	return FDO_SAFE_ADDREF(m_data);
}

// Sets the DataValue as a CLOB.
void FdoCLOBValue::SetData(FdoByteArray* value)
{
	// NULL is valid so no exception
	FDO_SAFE_RELEASE(m_data);
	m_isNull = (value == NULL);
	m_data = FDO_SAFE_ADDREF(value);
}

// Sets the DataValue to a null value of the specified type.
void FdoCLOBValue::SetNull()
{
    SetData(NULL);
    FdoDataValue::SetNull();
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoCLOBValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessCLOBValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoCLOBValue::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(L"{NULL}");
        return m_toString;
    }

//TODO:    m_toString = FdoStringUtility::MakeString(m_data, m_length);
    m_toString = FdoStringUtility::MakeString(L"{UNDEFINED}");
    return m_toString;
}

