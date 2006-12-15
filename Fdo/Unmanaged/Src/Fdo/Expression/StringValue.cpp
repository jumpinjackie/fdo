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
#include <Fdo/Expression/StringValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"
#include "ExpressionInternal.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoStringValue* FdoStringValue::Create()
{
	return new FdoStringValue();
}

// Constructs an instance of a string DataValue using the specified
// arguments.
FdoStringValue* FdoStringValue::Create(FdoString* value)
{
	return new FdoStringValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoStringValue::FdoStringValue()
{
    m_data = NULL;
}

// Constructs an instance of a string DataValue using the specified
// arguments.
FdoStringValue::FdoStringValue(FdoString* value)
{
	m_data = FdoStringUtility::MakeString(value);
	m_isNull = (m_data == NULL);
}

FdoStringValue::~FdoStringValue()
{
    FdoStringUtility::ClearString(m_data);
}

void FdoStringValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoStringValue::GetDataType()
{
	return FdoDataType_String;
}

// Gets the DataValue as a string.
FdoString* FdoStringValue::GetString()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_19_STRINGVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a string.
void FdoStringValue::SetString(FdoString* value)
{
    FdoStringUtility::ClearString(m_data);

	m_data = FdoStringUtility::MakeString(value);
	m_isNull = (m_data == NULL);
}

// Sets the DataValue to a null value of the specified type.
void FdoStringValue::SetNull()
{
    FdoStringUtility::ClearString(m_data);
    FdoDataValue::SetNull();
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoStringValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessStringValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoStringValue::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    FdoString*  pStr = NULL;
    
    if (!IsNull())
        pStr = GetString(); // don't call if IsNull(), can throw exception.

    if (pStr == NULL)
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    m_toString = FdoStringUtility::QuoteString(pStr, L'\'');
    return m_toString;
}

FdoCompareType FdoInternalStringValue::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    // Only comparison with other strings currently supported.
    if ( other->GetDataType() == (*this)->GetDataType() ) {
        FdoString* str1 = (*this)->GetString();
        FdoString* str2 = static_cast<FdoStringValue*>(other)->GetString();

        compare = FdoCompare( FdoStringP(str1,true), FdoStringP(str2,true) );
    }

    return compare;
}
