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
#include <Fdo/Expression/DoubleValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDoubleValue* FdoDoubleValue::Create()
{
	return new FdoDoubleValue();
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDoubleValue* FdoDoubleValue::Create(double value)
{
	return new FdoDoubleValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDoubleValue::FdoDoubleValue()
{
    m_data = 0.0;
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDoubleValue::FdoDoubleValue(double value)
{
    m_isNull = false;
    m_data = value;
}

FdoDoubleValue::~FdoDoubleValue()
{
}

void FdoDoubleValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoDoubleValue::GetDataType()
{
	return FdoDataType_Double;
}

// Gets the DataValue as a double.
double FdoDoubleValue::GetDouble()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_7_DOUBLEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a double precision floating point number.
void FdoDoubleValue::SetDouble(double value)
{
    m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoDoubleValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessDoubleValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoDoubleValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoStringUtility::FormatDouble(GetDouble(), szBuf, iBufLen);
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoDoubleValue* FdoDoubleValue::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoDoubleValue* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) 
        {
        case FdoDataType_Boolean:
            // Convert to numeric
            ret = FdoDoubleValue::Create( static_cast<FdoBooleanValue*>(src)->GetBoolean() ? 1 : 0 );
            break;

        case FdoDataType_Byte:
            // copy without truncation or shifting
            ret = FdoDoubleValue::Create( (double)(static_cast<FdoByteValue*>(src)->GetByte()) );
            break;

        case FdoDataType_Decimal:
            // Same value types, simple copy.
            ret = FdoDoubleValue::Create( static_cast<FdoDecimalValue*>(src)->GetDecimal() );
            break;

        case FdoDataType_Double:
            // Same types, simple copy.
            ret = FdoDoubleValue::Create( static_cast<FdoDoubleValue*>(src)->GetDouble() );
            break;

        case FdoDataType_Int16:
            // copy without truncation or shifting
            ret = FdoDoubleValue::Create( (double)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
            break;

        case FdoDataType_Int32:
            // copy without truncation or shifting
            ret = FdoDoubleValue::Create( (double)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
            break;

        case FdoDataType_Int64:
            // copy with possible shifting
            ret = FdoDoubleValue::Create( (double)(static_cast<FdoInt64Value*>(src)->GetInt64()) );
            VldShift( src, (FdoDataValue*) ret, nullIfIncompatible, shift );
            break;

        case FdoDataType_Single:
            // copy without truncation or shifting
            ret = FdoDoubleValue::Create( (double)(static_cast<FdoSingleValue*>(src)->GetSingle()) );
            break;

        case FdoDataType_String:
            // convert to numeric with possible shifting
            ret = static_cast<FdoStringValue*>(src)->ConvertFrom<FdoDoubleValue>
            (
                nullIfIncompatible, 
                shift,
                truncate, 
                FdoDataTypeMapper::Type2String(FdoDataType_Double)
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
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_Double)
                    )
                );
            // else return null value 
            break;
        }
    }

    if ( !ret ) 
        // return null data value instead of NULL pointer.
        ret = FdoDoubleValue::Create();

    return ret;
}

FdoCompareType FdoDoubleValue::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> otherValue = FDO_SAFE_ADDREF(other);
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Double:
        {
            FdoDouble num1 = GetDouble();
            FdoDouble num2 = static_cast<FdoDoubleValue*>(other)->GetDouble();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    // Decimal and Double actually have same range and precision. Decimal-Double comparison has been
    // arbitrarily implemented in FdoDoubleValue
    case FdoDataType_Byte:
    case FdoDataType_Decimal:
    case FdoDataType_Int16:
    case FdoDataType_Int32:
    case FdoDataType_Single:
        otherValue = FdoDoubleValue::Create( other );
        compare = Compare( otherValue );
        break;

    // Other value's type has more precision, invoke that type to do a reverse comparison.
    case FdoDataType_Int64:
        compare = ReverseCompare( other );
        break;
    }

    return compare;
}
