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
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

#include <math.h>
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

FdoInt64Value* FdoInt64Value::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoInt64Value* ret = NULL;
    FdoDouble int64DblMax = (pow((FdoDouble)2, (FdoDouble)52) - 1) * pow((FdoDouble)2, (FdoDouble)11);
    FdoFloat int64SglMax = (pow((FdoFloat)2, (FdoFloat)23) - 1) * pow((FdoFloat)2, (FdoFloat)40);

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) 
        {
        case FdoDataType_Boolean:
            // Convert to numeric
            ret = FdoInt64Value::Create( static_cast<FdoBooleanValue*>(src)->GetBoolean() ? 1 : 0 );
            break;

        case FdoDataType_Byte:
            // copy without truncation or shifting
            ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoByteValue*>(src)->GetByte()) );
            break;

        case FdoDataType_Decimal:
            // Copy with possible truncation and rounding
            {
                FdoDecimalValue* src2 = static_cast<FdoDecimalValue*>(src);

                ret = Convert<FdoDecimalValue, FdoInt64Value, FdoDouble, FdoInt64>(
                    src2,
                    src2->GetDecimal(), 
                    FdoInt64Min, 
                    FdoInt64Max, 
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int64)
                );
            }
            break;

        case FdoDataType_Double:
            // Copy with possible truncation and rounding
            {
                FdoDoubleValue* src2 = static_cast<FdoDoubleValue*>(src);

                ret = Convert<FdoDoubleValue, FdoInt64Value, FdoDouble, FdoInt64>(
                    src2,
                    src2->GetDouble(), 
                    FdoInt64Min, 
                    FdoInt64Max, 
                    0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int64)
                );
            }
            break;

        case FdoDataType_Int16:
            // copy without truncation or shifting
            ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
            break;

        case FdoDataType_Int32:
            // copy without truncation or shifting
            ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
            break;

        case FdoDataType_Int64:
            // Same types, simple copy.
            ret = FdoInt64Value::Create( static_cast<FdoInt64Value*>(src)->GetInt64() );
            break;

        case FdoDataType_Single:
            // Copy with possible truncation and rounding
            {
                FdoSingleValue* src2 = static_cast<FdoSingleValue*>(src);

                ret = Convert<FdoSingleValue, FdoInt64Value, FdoFloat, FdoInt64>(
                    src2,
                    src2->GetSingle(), 
                    FdoInt64Min, 
                    FdoInt64Max, 
                    (FdoFloat) 0.5,
                    nullIfIncompatible, 
                    shift,
                    truncate, 
                    FdoDataTypeMapper::Type2String(FdoDataType_Int64)
                );
            }
            break;

        case FdoDataType_String:
            // convert to numeric with possible truncation and rounding.
            ret = static_cast<FdoStringValue*>(src)->ConvertFrom<FdoInt64Value>
            (
                nullIfIncompatible, 
                shift,
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Int64)
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
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_Int64)
                    )
                );
            // else return null value 
            break;
        }
    }

    if ( !ret ) 
        // return null data value instead of NULL pointer.
        ret = FdoInt64Value::Create();

    return ret;
}

FdoCompareType FdoInt64Value::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> thisValue;
    FdoPtr<FdoDataValue> otherValue;
    FdoDouble doubleValue;
    FdoInt64Value roundValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Int64:
        {
            FdoInt64 num1 = GetInt64();
            FdoInt64 num2 = static_cast<FdoInt64Value*>(other)->GetInt64();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
    case FdoDataType_Int32:
        otherValue = FdoInt64Value::Create( other );
        compare = thisValue->Compare( otherValue );
        break;

    // These floating-point type have larger range but less precision.
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Single:
        // Convert this int64 to the other type and compare.
        switch ( other->GetDataType() ) {
        case FdoDataType_Decimal:
            thisValue = FdoDecimalValue::Create( this );
            doubleValue = static_cast<FdoDecimalValue*>((FdoDataValue*)thisValue)->GetDecimal();
            break;

        case FdoDataType_Double:
            thisValue = FdoDoubleValue::Create( this );
            doubleValue = static_cast<FdoDoubleValue*>((FdoDataValue*)thisValue)->GetDouble();
            break;

        case FdoDataType_Single:
            thisValue = FdoSingleValue::Create( this );
            doubleValue = (FdoDouble) (static_cast<FdoSingleValue*>((FdoDataValue*)thisValue)->GetSingle());
            break;
        }

        compare = thisValue->Compare( other );

        // Int64 value many have been rounded by conversion. Rounded value might be equal to 
        // other value when Int64 value is not. 
        if ( compare == FdoCompareType_Equal ) {
            // Convert rounded value back to Int64

            if ( doubleValue > (FdoDouble) LLONG_MAX ) {
                // Value rounded above int64 range, so it is less than single value
                compare = FdoCompareType_Less;
            }
            else if ( doubleValue < (FdoDouble) LLONG_MIN ) {
                // Value rounded below int64 range, so it is greater than single value
                compare = FdoCompareType_Greater;
            }
            else {
                // rounded value in int64 range
                roundValue = (FdoInt64) doubleValue;

                if ( roundValue < GetInt64() ) 
                    // Int64 value was rounded down so it is actually greater than other value
                    compare = FdoCompareType_Greater;
                if ( roundValue > GetInt64() ) 
                    // Int64 value was rounded up so it is actually less than single value
                    compare = FdoCompareType_Less;
            }
        }

        break;
    }

    return compare;
}
