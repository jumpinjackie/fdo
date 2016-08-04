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
#include "../Schema/DataTypeMapper.h"
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

FdoBooleanValue* FdoBooleanValue::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoBooleanValue* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) {
        case FdoDataType_Boolean:
            // Same types, simple copy.
            ret = FdoBooleanValue::Create( static_cast<FdoBooleanValue*>(src)->GetBoolean() );
            break;

        case FdoDataType_Byte:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoByte>(
                src,
                static_cast<FdoByteValue*>(src)->GetByte(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Decimal:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoDouble>(
                src,
                static_cast<FdoDecimalValue*>(src)->GetDecimal(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Double:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoDouble>(
                src,
                static_cast<FdoDoubleValue*>(src)->GetDouble(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Int16:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoInt16>(
                src,
                static_cast<FdoInt16Value*>(src)->GetInt16(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Int32:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoInt32>(
                src,
                static_cast<FdoInt32Value*>(src)->GetInt32(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Int64:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoInt64>(
                src,
                static_cast<FdoInt64Value*>(src)->GetInt64(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_Single:
            // Copy with possible truncation
            ret = FdoBooleanValue::Convert<FdoFloat>(
                src,
                static_cast<FdoSingleValue*>(src)->GetSingle(), 
                nullIfIncompatible, 
                truncate
            );
            break;

        case FdoDataType_String:
            // Convert with possible truncation
            ret = static_cast<FdoStringValue*>(src)->ConvertFrom<FdoBooleanValue>
            (
                nullIfIncompatible, 
                shift,
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Boolean)
            );
            break;

        default:
            // src and dest types incompatible
            if ( !nullIfIncompatible )
                throw FdoExpressionException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(EXPRESSION_22_INCOMPATIBLEDATATYPES),
                        src->ToString(),
                        (FdoString*) FdoDataTypeMapper::Type2String(src->GetDataType()),
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_Boolean)
                    )
                );
            // else return null value 
            break;
        }
    }

    if ( !ret )
        // return null data value instead of NULL pointer.
        ret = FdoBooleanValue::Create();

    return ret;
}

