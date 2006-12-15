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
#include "StringUtility.h"
#include "ExpressionInternal.h"

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

FdoInt32Value* FdoInternalInt32Value::Create(
    FdoDataValue* src, 
    FdoBoolean truncate, 
    FdoBoolean nullIfIncompatible
)
{
    FdoInt32Value* ret = NULL;

    switch ( src->GetDataType() ) {
    case FdoDataType_Byte:
        ret = FdoInt32Value::Create( (FdoInt32)(static_cast<FdoByteValue*>(src)->GetByte()) );
        break;

    case FdoDataType_Int16:
        ret = FdoInt32Value::Create( (FdoInt32)(static_cast<FdoInt16Value*>(src)->GetInt16()) );
        break;

    case FdoDataType_Int32:
        ret = FdoInt32Value::Create( static_cast<FdoInt32Value*>(src)->GetInt32() );
        break;
    }

    return ret;
}

FdoCompareType FdoInternalInt32Value::DoCompare( FdoDataValue* other )
{
    FdoCompareType compare = FdoCompareType_Undefined;

    FdoPtr<FdoDataValue> thisValue;
    FdoPtr<FdoDataValue> otherValue;
    
    switch ( other->GetDataType() ) {
    // Same type, do simple comparison
    case FdoDataType_Int32:
        {
            FdoInt32 num1 = (*this)->GetInt32();
            FdoInt32 num2 = static_cast<FdoInt32Value*>(other)->GetInt32();

            compare = FdoCompare( num1, num2 );
        }
        break;

    // Other values's type has smaller range. Convert other value to this value's type and compare.
    case FdoDataType_Byte:
    case FdoDataType_Int16:
        otherValue = FdoInternalInt32Value::Create( other );
        compare = FdoPtr<FdoInternalDataValue>(FdoInternalDataValue::Create(thisValue))->Compare( otherValue );
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
        thisValue = FdoInternalDoubleValue::Create( (FdoDataValue*) this );
        otherValue = FdoInternalDoubleValue::Create( other );
        compare = FdoPtr<FdoInternalDataValue>(FdoInternalDataValue::Create(thisValue))->Compare( otherValue );
        break;
    }

    return compare;
}
