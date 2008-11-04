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
#include <Fdo/Expression/Int16Value.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt16Value* FdoInt16Value::Create()
{
	return new FdoInt16Value();
}

// Constructs an instance of a 16 bit integer DataValue using the specified
// arguments.
FdoInt16Value* FdoInt16Value::Create(FdoInt16 value)
{
	return new FdoInt16Value(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt16Value::FdoInt16Value()
{
    m_data = 0;
}

// Constructs an instance of a 16 bit integer DataValue using the specified
// arguments.
FdoInt16Value::FdoInt16Value(FdoInt16 value)
{
    m_isNull = false;
	m_data = value;
}

FdoInt16Value::~FdoInt16Value()
{
}

void FdoInt16Value::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoInt16Value::GetDataType()
{
	return FdoDataType_Int16;
}

// Gets the DataValue as a 16 bit integer.
FdoInt16 FdoInt16Value::GetInt16()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_12_INT16VALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a 16 bit integer.
void FdoInt16Value::SetInt16(FdoInt16 value)
{
    m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoInt16Value::Process(FdoIExpressionProcessor* p)
{
    p->ProcessInt16Value(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoInt16Value::ToString()
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
    _itow((FdoInt32)GetInt16(), szBuf, 10);
#else
	swprintf(szBuf, sizeof(szBuf)/sizeof(wchar_t), L"%d", (FdoInt32)GetInt16());

#endif		
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoInt16Value* FdoInt16Value::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoInt16Value* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) {
        case FdoDataType_Boolean:
            // Convert to numeric
            ret = FdoInt16Value::Create( static_cast<FdoBooleanValue*>(src)->GetBoolean() ? 1 : 0 );
            break;

        case FdoDataType_Byte:
            // copy without truncation or shifting
            ret = FdoInt16Value::Create( (FdoInt16)(static_cast<FdoByteValue*>(src)->GetByte()) );
            break;

        case FdoDataType_Decimal:
            // Copy with possible truncation and rounding
            {
                FdoDecimalValue* src2 = static_cast<FdoDecimalValue*>(src);

                ret = Convert<FdoDecimalValue, FdoInt16Value, FdoDouble, FdoInt16>(
                    src2,
                    src2->GetDecimal(), 
                    SHRT_MIN, 
                    SHRT_MAX, 
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int16)
                );
            }
            break;

        case FdoDataType_Double:
            // Copy with possible truncation and rounding
            {
                FdoDoubleValue* src2 = static_cast<FdoDoubleValue*>(src);

                ret = Convert<FdoDoubleValue, FdoInt16Value, FdoDouble, FdoInt16>(
                    src2,
                    src2->GetDouble(), 
                    SHRT_MIN, 
                    SHRT_MAX, 
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int16)
                );
            }
            break;

        case FdoDataType_Int16:
            // Same types, simple copy.
            ret = FdoInt16Value::Create( static_cast<FdoInt16Value*>(src)->GetInt16() );
            break;

        case FdoDataType_Int32:
            // Copy with possible truncation
            ret = Convert<FdoInt16Value, FdoInt32, FdoInt16>( 
                static_cast<FdoInt32Value*>(src)->GetInt32(), 
                SHRT_MIN, 
                SHRT_MAX, 
                nullIfIncompatible, 
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int16)
            );
            break;

        case FdoDataType_Int64:
            // Copy with possible truncation
            ret = Convert<FdoInt16Value, FdoInt64, FdoInt16>( 
                static_cast<FdoInt64Value*>(src)->GetInt64(), 
                SHRT_MIN, 
                SHRT_MAX, 
                nullIfIncompatible, 
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int16)
            );
            break;

        case FdoDataType_Single:
            // Copy with possible truncation and rounding
            {
                FdoSingleValue* src2 = static_cast<FdoSingleValue*>(src);

                ret = Convert<FdoSingleValue, FdoInt16Value, FdoFloat, FdoInt16>(
                    src2,
                    src2->GetSingle(), 
                    SHRT_MIN, 
                    SHRT_MAX, 
                    (FdoFloat) 0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int16)
                );
            }
            break;

        case FdoDataType_String:
            // convert to numeric with possible truncation and rounding.
            ret = static_cast<FdoStringValue*>(src)->ConvertFrom<FdoInt16Value>
            (
                nullIfIncompatible, 
                shift,
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int16)
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
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_Int16)
                    )
                );
            // else return null value 
            break;
        }
    }

    if ( !ret ) 
        // return null data value instead of NULL pointer.
        ret = FdoInt16Value::Create();

    return ret;
}

FdoCompareType FdoInt16Value::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> otherValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Int16:
        {
            FdoInt16 num1 = GetInt16();
            FdoInt16 num2 = static_cast<FdoInt16Value*>(other)->GetInt16();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
        otherValue = FdoInt16Value::Create( other );
        compare = Compare( otherValue );
        break;

    // Other value's type has larger range, invoke that type to do a reverse comparison.
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Int32:
    case FdoDataType_Int64:
    case FdoDataType_Single:
        compare = ReverseCompare( other );
        break;
    }

    return compare;
}
