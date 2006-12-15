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
#include "StringUtility.h"
#include "Internal.h"

#include <time.h>

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

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

FdoInt64Value* FdoInternalInt64Value::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoInt64Value* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Int16:
        ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoInt64Value::Create( (FdoInt64)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
        break;

    case FdoDataType_Int64:
        ret = FdoInt64Value::Create( static_cast<FdoInt64Value*>(src)->GetInt64() );
        break;
    }

    return ret;
}

FdoCompareType FdoInternalInt64Value::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> thisValue;
    FdoPtr<FdoDataValue> otherValue;
    FdoDouble doubleValue;
    FdoInt64 roundValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Int64:
        {
            FdoInt64 num1 = (*this)->GetInt64();
            FdoInt64 num2 = static_cast<FdoInt64Value*>(other)->GetInt64();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
    case FdoDataType_Int32:
        otherValue = FdoInternalInt64Value::Create( other );
        compare = FdoPtr<FdoInternalDataValue>(FdoInternalDataValue::Create(thisValue))->Compare( otherValue );
        break;

    // These floating-point type have larger range but less precision.
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Single:
        // Convert this int64 to the other type and compare.
        switch ( other->GetDataType() ) {
        case FdoDataType_Decimal:
            thisValue = FdoInternalDecimalValue::Create( (FdoDataValue*) this );
            doubleValue = static_cast<FdoDecimalValue*>((FdoDataValue*)thisValue)->GetDecimal();
            break;

        case FdoDataType_Double:
            thisValue = FdoInternalDoubleValue::Create( (FdoDataValue*) this );
            doubleValue = static_cast<FdoDoubleValue*>((FdoDataValue*)thisValue)->GetDouble();
            break;

        case FdoDataType_Single:
            thisValue = FdoInternalSingleValue::Create( (FdoDataValue*) this );
            doubleValue = (FdoDouble) (static_cast<FdoSingleValue*>((FdoDataValue*)thisValue)->GetSingle());
            break;
        }

        compare = FdoPtr<FdoInternalDataValue>(FdoInternalDataValue::Create(thisValue))->Compare( other );

        // Int64 value many have been rounded by conversion. Rounded value might be equal to 
        // other value when Int64 value is not. 
        if ( compare == FdoCompareType_Equal ) {
            // Convert rounded value back to Int64

            if ( doubleValue >= (FdoDouble) LLONG_MAX ) 
                // Value rounded above int64 range, snap back to max
                roundValue = LLONG_MAX;
            else if ( doubleValue <= (FdoDouble) LLONG_MIN )
                // Value rounded below int64 range, snap back to min
                roundValue = LLONG_MIN;
            else
                // rounded value in int64 range
                roundValue = (FdoInt64) doubleValue;

            if ( roundValue < (*this)->GetInt64() ) 
                // Int64 value was rounded down so it is actually greater than other value
                compare = FdoCompareType_Greater;
            if ( roundValue > (*this)->GetInt64() ) 
                // Int32 value was rounded down so it is actually greater than single value
                compare = FdoCompareType_Less;
        }

        break;
    }

    return compare;
}
