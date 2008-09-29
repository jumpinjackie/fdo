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
#include <Fdo/Expression/DecimalValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDecimalValue* FdoDecimalValue::Create()
{
	return new FdoDecimalValue();
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDecimalValue* FdoDecimalValue::Create(double value)
{
	return new FdoDecimalValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDecimalValue::FdoDecimalValue()
{
    m_data = 0.0;
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDecimalValue::FdoDecimalValue(double value)
{
    m_data = value;
    m_isNull = false;
}

FdoDecimalValue::~FdoDecimalValue()
{
}

void FdoDecimalValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoDecimalValue::GetDataType()
{
	return FdoDataType_Decimal;
}

// Gets the DataValue as a decimal.
double FdoDecimalValue::GetDecimal()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_6_DECIMALVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a decimal.
void FdoDecimalValue::SetDecimal(double value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoDecimalValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessDecimalValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoDecimalValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoStringUtility::FormatDouble(GetDecimal(), szBuf, iBufLen);
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoDecimalValue* FdoDecimalValue::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoDecimalValue* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoDecimalValue::Create( (double)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Decimal:
        ret = FdoDecimalValue::Create( static_cast<FdoDecimalValue*>(src)->GetDecimal() );
        break;

    case FdoDataType_Double:
        ret = FdoDecimalValue::Create( static_cast<FdoDoubleValue*>(src)->GetDouble() );
        break;

    case FdoDataType_Int16:
        ret = FdoDecimalValue::Create( (double)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoDecimalValue::Create( (double)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
        break;

    case FdoDataType_Int64:
        ret = FdoDecimalValue::Create( (double)(static_cast<FdoInt64Value*>(src)->GetInt64()) );
        break;

    case FdoDataType_Single:
        ret = FdoDecimalValue::Create( (double)(static_cast<FdoSingleValue*>(src)->GetSingle()) );
        break;
    }

    return ret;
}

FdoCompareType FdoDecimalValue::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;
    FdoPtr<FdoDataValue> otherValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Decimal:
        {
            FdoDouble num1 = GetDecimal();
            FdoDouble num2 = static_cast<FdoDecimalValue*>(other)->GetDecimal();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
    case FdoDataType_Int32:
    case FdoDataType_Single:
        otherValue = FdoDecimalValue::Create( other );
        compare = Compare( otherValue );
        break;

    // Other value's type has more precision, invoke that type to do a reverse comparison.
    // Decimal and Double actually have same range and precision. Decimal-Double comparison has been
    // arbitrarily implemented in FdoDoubleValue
    case FdoDataType_Int64:
    case FdoDataType_Double:
        compare = ReverseCompare( other );
        break;
    }

    return compare;
}
