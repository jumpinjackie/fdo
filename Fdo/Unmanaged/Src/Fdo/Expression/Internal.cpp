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
#include <Fdo/Expression/DataValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "Internal.h"

#include <time.h>

FdoInternalDataValue* FdoInternalDataValue::Create( FdoDataValue* dataValue ) 
{
    if ( !dataValue )
        return new FdoInternalDataValue(dataValue);

    switch ( dataValue->GetDataType() ) 
    {
    case FdoDataType_Byte:
        return new FdoInternalByteValue( (FdoByteValue*) dataValue );
    case FdoDataType_DateTime:
        return new FdoInternalDateTimeValue( (FdoDateTimeValue*) dataValue );
    case FdoDataType_Decimal:
        return new FdoInternalDecimalValue( (FdoDecimalValue*) dataValue );
    case FdoDataType_Double:
        return new FdoInternalDoubleValue( (FdoDoubleValue*) dataValue );
    case FdoDataType_Int16:
        return new FdoInternalInt16Value( (FdoInt16Value*) dataValue );
    case FdoDataType_Int32:
        return new FdoInternalInt32Value( (FdoInt32Value*) dataValue );
    case FdoDataType_Int64:
        return new FdoInternalInt64Value( (FdoInt64Value*) dataValue );
    case FdoDataType_Single:
        return new FdoInternalSingleValue( (FdoSingleValue*) dataValue );
    case FdoDataType_String:
        return new FdoInternalStringValue( (FdoStringValue*) dataValue );
    }

    return (FdoInternalDataValue*) NULL;
}

FdoInternalDataValue::FdoInternalDataValue( FdoDataValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalByteValue::FdoInternalByteValue( FdoByteValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalByteValue::~FdoInternalByteValue()
{
}

FdoInternalDateTimeValue::FdoInternalDateTimeValue( FdoDateTimeValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalDateTimeValue::~FdoInternalDateTimeValue()
{
}

FdoInternalDecimalValue::FdoInternalDecimalValue( FdoDecimalValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalDecimalValue::~FdoInternalDecimalValue()
{
}

FdoInternalDoubleValue::FdoInternalDoubleValue( FdoDoubleValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalDoubleValue::~FdoInternalDoubleValue()
{
}

FdoInternalInt16Value::FdoInternalInt16Value( FdoInt16Value* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalInt16Value::~FdoInternalInt16Value()
{
}

FdoInternalInt32Value::FdoInternalInt32Value( FdoInt32Value* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalInt32Value::~FdoInternalInt32Value()
{
}

FdoInternalInt64Value::FdoInternalInt64Value( FdoInt64Value* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalInt64Value::~FdoInternalInt64Value()
{
}

FdoInternalSingleValue::FdoInternalSingleValue( FdoSingleValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalSingleValue::~FdoInternalSingleValue()
{
}

FdoInternalStringValue::FdoInternalStringValue( FdoStringValue* dataValue)
{
    mDataValue = dataValue;
}

FdoInternalStringValue::~FdoInternalStringValue()
{
}
