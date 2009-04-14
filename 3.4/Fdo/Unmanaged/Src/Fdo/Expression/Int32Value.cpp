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
#include <Fdo/Expression/Int32Value.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

#include <math.h>
#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt32Value* FdoInt32Value::Create()
{
	return new FdoInt32Value();
}

// Constructs an instance of a 32 bit integer DataValue using the specified
// arguments.
FdoInt32Value* FdoInt32Value::Create(FdoInt32 value)
{
	return new FdoInt32Value(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoInt32Value::FdoInt32Value()
{
    m_data = 0;
}

// Constructs an instance of a 32 bit integer DataValue using the specified
// arguments.
FdoInt32Value::FdoInt32Value(FdoInt32 value)
{
    m_isNull = false;
	m_data = value;
}

FdoInt32Value::~FdoInt32Value()
{
}

void FdoInt32Value::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoInt32Value::GetDataType()
{
	return FdoDataType_Int32;
}

// Gets the DataValue as a 32 bit integer.
FdoInt32 FdoInt32Value::GetInt32()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_13_INT32VALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a 32 bit integer.
void FdoInt32Value::SetInt32(FdoInt32 value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoInt32Value::Process(FdoIExpressionProcessor* p)
{
    p->ProcessInt32Value(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoInt32Value::ToString()
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
    _itow(GetInt32(), szBuf, 10);
#else
	 swprintf(szBuf, sizeof(szBuf)/sizeof(wchar_t), L"%d", GetInt32());
#endif
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoInt32Value* FdoInt32Value::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoInt32Value* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) 
        {
        case FdoDataType_Boolean:
            // Convert to numeric
            ret = FdoInt32Value::Create( static_cast<FdoBooleanValue*>(src)->GetBoolean() ? 1 : 0 );
            break;

        case FdoDataType_Byte:
            // copy without truncation or shifting
            ret = FdoInt32Value::Create( (FdoInt32)(static_cast<FdoByteValue*>(src)->GetByte()) );
            break;

        case FdoDataType_Decimal:
            // Copy with possible truncation and rounding
            {
                FdoDecimalValue* src2 = static_cast<FdoDecimalValue*>(src);

                ret = Convert<FdoDecimalValue, FdoInt32Value, FdoDouble, FdoInt32>(
                    src2,
                    src2->GetDecimal(), 
                    LONG_MIN, 
                    LONG_MAX, 
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int32)
                );
            }
            break;

        case FdoDataType_Double:
            // Copy with possible truncation and rounding
            {
                FdoDoubleValue* src2 = static_cast<FdoDoubleValue*>(src);

                ret = Convert<FdoDoubleValue, FdoInt32Value, FdoDouble, FdoInt32>(
                    src2,
                    src2->GetDouble(), 
                    LONG_MIN, 
                    LONG_MAX,
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int32)
                );
            }
            break;

        case FdoDataType_Int16:
            // copy without truncation or shifting
            ret = FdoInt32Value::Create( (FdoInt32)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
            break;

        case FdoDataType_Int32:
            // Same types, simple copy.
            ret = FdoInt32Value::Create( static_cast<FdoInt32Value*>(src)->GetInt32() );
            break;

        case FdoDataType_Int64:
            // Copy with possible truncation
            ret = Convert<FdoInt32Value, FdoInt64, FdoInt32>( 
                static_cast<FdoInt64Value*>(src)->GetInt64(), 
                LONG_MIN, 
                LONG_MAX, 
                nullIfIncompatible, 
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int32)
            );
            break;

        case FdoDataType_Single:
            // Copy with possible truncation and rounding
            {
                FdoSingleValue* src2 = static_cast<FdoSingleValue*>(src);

                ret = Convert<FdoSingleValue, FdoInt32Value, FdoFloat, FdoInt32>(
                    src2,
                    src2->GetSingle(), 
                    LONG_MIN, 
                    LONG_MAX,
                    (FdoFloat) 0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int32)
                );
            }
            break;

        case FdoDataType_String:
            // convert to numeric with possible truncation and rounding.
            ret = static_cast<FdoStringValue*>(src)->ConvertFrom<FdoInt32Value>
            (
                nullIfIncompatible, 
                shift,
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int32)
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
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_Int32)
                    )
                );
            // else return null value 
            break;
        }
    }

    if ( !ret ) 
        // return null data value instead of NULL pointer.
        ret = FdoInt32Value::Create();

    return ret;
}

FdoCompareType FdoInt32Value::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> thisValue;
    FdoPtr<FdoDataValue> otherValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Int32:
        {
            FdoInt32 num1 = GetInt32();
            FdoInt32 num2 = static_cast<FdoInt32Value*>(other)->GetInt32();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
        otherValue = FdoInt32Value::Create( other );
        compare = thisValue->Compare( otherValue );
        break;

    // Other value's type has larger range, invoke that type to do a reverse comparison.
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Int64:
        compare = ReverseCompare( other );
        break;

    // Single type has larger range but less precision.
    case FdoDataType_Single:
        // Double has larger range and precision that either int32 or single so convert
        // both to double and compare.
        thisValue = FdoDoubleValue::Create( this );
        otherValue = FdoDoubleValue::Create( other );
        compare = thisValue->Compare( otherValue );
        break;
    }

    return compare;
}
