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
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

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
    m_allocatedSize = 0;

}

// Constructs an instance of a string DataValue using the specified
// arguments.
FdoStringValue::FdoStringValue(FdoString* value)
{
	m_data = FdoStringUtility::MakeString(value);
	m_isNull = (m_data == NULL);
    if (value)
        m_allocatedSize = wcslen(value);
    else
        m_allocatedSize = 0;
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
    if (value)
    {
        size_t size = wcslen(value);
        if ((size > m_allocatedSize) || (m_data == NULL))
        {
            FdoStringUtility::ClearString(m_data);

	        m_data = FdoStringUtility::MakeString(value);
            m_allocatedSize = size;
        }
        else
        {
            wcscpy(m_data, value);
        }
    }
    else
    {
        if (m_data)
            m_data[0] = '\0';
    }
	m_isNull = (value == NULL);
}

// Sets the DataValue to a null value of the specified type.
void FdoStringValue::SetNull()
{
    if (m_data)
        m_data[0] = '\0';
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

FdoStringValue* FdoStringValue::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoStringValue* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) 
        {
        case FdoDataType_Decimal:
            // Convert to string with possible shifting
            ret = FdoStringValue::Create( src->ToString() );
            if ( !shift ) 
            {
                FdoPtr<FdoDecimalValue> check = FdoDecimalValue::Create( ret );
                VldShift(src, check, nullIfIncompatible, shift );
                if ( check->IsNull() )
                    ret->SetNull();
            }
            break;

        case FdoDataType_Double:
            // Convert to string with possible shifting
            ret = FdoStringValue::Create( src->ToString() );
            if ( !shift ) 
            {
                FdoPtr<FdoDoubleValue> check = FdoDoubleValue::Create( ret );
                VldShift(src, check, nullIfIncompatible, shift );
                if ( check->IsNull() )
                    ret->SetNull();
            }
            break;

        case FdoDataType_Single:
            // Convert to string with possible shifting
            ret = FdoStringValue::Create( src->ToString() );
            if ( !shift ) 
            {
                FdoPtr<FdoSingleValue> check = FdoSingleValue::Create( ret );
                VldShift(src, check, nullIfIncompatible, shift );
                if ( check->IsNull() )
                    ret->SetNull();
            }
            break;

        case FdoDataType_String:
            // same types, simple copy
            ret = FdoStringValue::Create( (static_cast<FdoStringValue*>(src)->GetString()) );
            break;

        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
            // FDO has yet to define string representation for BLOB and CLOB
            if ( !nullIfIncompatible )
                throw FdoExpressionException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(EXPRESSION_22_INCOMPATIBLEDATATYPES),
                        src->ToString(),
                        (FdoString*) FdoDataTypeMapper::Type2String(src->GetDataType()),
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_String)
                    )
                );
            // else return null value 
            break;

        default:
            // For other types, just call ToString()
            ret = FdoStringValue::Create( src->ToString() );
            break;
        }
    }

    if ( !ret ) 
        ret = FdoStringValue::Create();

    return ret;
}

FdoCompareType FdoStringValue::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    // Only comparison with other strings currently supported.
    if ( other->GetDataType() == GetDataType() ) {
        FdoString* str1 = GetString();
        FdoString* str2 = static_cast<FdoStringValue*>(other)->GetString();

        compare = FdoCompare( FdoStringP(str1,true), FdoStringP(str2,true) );
    }

    return compare;
}

FdoDataValue* FdoStringValue::Parse()
{
    FdoDataValue* ret = NULL;
        
    try 
    {
        FdoPtr<FdoExpression> expr = FdoExpression::Parse( GetString() );
        
        ret = FDO_SAFE_ADDREF(dynamic_cast<FdoDataValue*>(expr.p));
    }
    catch ( FdoException* /*ex*/ ) {
    }

    return ret;
}

