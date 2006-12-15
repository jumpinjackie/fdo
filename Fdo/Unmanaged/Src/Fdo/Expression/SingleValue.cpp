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
#include <Fdo/Expression/SingleValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"
#include "Internal.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoSingleValue* FdoSingleValue::Create()
{
	return new FdoSingleValue();
}

// Constructs an instance of a single precision floating point DataValue
// using the specified arguments.
FdoSingleValue* FdoSingleValue::Create(float value)
{
	return new FdoSingleValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoSingleValue::FdoSingleValue()
{
    m_data = 0.0;
}

// Constructs an instance of a single precision floating point DataValue
// using the specified arguments.
FdoSingleValue::FdoSingleValue(float value)
{
	m_isNull = false;
	m_data = value;
}

FdoSingleValue::~FdoSingleValue()
{
}

void FdoSingleValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoSingleValue::GetDataType()
{
	return FdoDataType_Single;
}

// Gets the DataValue as a single precision floating point number.
float FdoSingleValue::GetSingle()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_18_SINGLEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a single precision floating point number.
void FdoSingleValue::SetSingle(float value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoSingleValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessSingleValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoSingleValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoStringUtility::FormatSingle(GetSingle(), szBuf, iBufLen);
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

FdoSingleValue* FdoInternalSingleValue::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoSingleValue* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoSingleValue::Create( (FdoFloat)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Int16:
        ret = FdoSingleValue::Create( (FdoFloat)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoSingleValue::Create( (FdoFloat)(static_cast<FdoInt32Value*>(src)->GetInt32()) );
        break;

    case FdoDataType_Int64:
        ret = FdoSingleValue::Create( (FdoFloat)(static_cast<FdoInt64Value*>(src)->GetInt64()) );
        break;

    case FdoDataType_Single:
        ret = FdoSingleValue::Create( static_cast<FdoSingleValue*>(src)->GetSingle() );
        break;
    }

    return ret;
}

FdoCompareType FdoInternalSingleValue::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> otherValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Single:
        {
            FdoFloat num1 = (*this)->GetSingle();
            FdoFloat num2 = static_cast<FdoSingleValue*>(other)->GetSingle();

            compare = FdoCompare( num1, num2 );
        }
        break;
 
    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
        otherValue = FdoInternalSingleValue::Create( other );
        compare = Compare( otherValue );
        break;

    // Other value's type has larger range or finer precision, invoke that type to do a reverse comparison.
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Int32:
    case FdoDataType_Int64:
        compare = ReverseCompare( otherValue );
        break;
    }

    return compare;
}
