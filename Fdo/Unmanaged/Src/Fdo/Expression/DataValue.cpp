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
#include "StringUtility.h"

#include <time.h>

// Constructs an instance of a null DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoDataType dataType)
{
    switch (dataType)
    {
    case FdoDataType_Boolean:
        return FdoBooleanValue::Create();
    case FdoDataType_Byte:
        return FdoByteValue::Create();
    case FdoDataType_DateTime:
        return FdoDateTimeValue::Create();
    case FdoDataType_Decimal:
        return FdoDecimalValue::Create();
    case FdoDataType_Double:
        return FdoDoubleValue::Create();
    case FdoDataType_Int16:
        return FdoInt16Value::Create();
    case FdoDataType_Int32:
        return FdoInt32Value::Create();
    case FdoDataType_Int64:
        return FdoInt64Value::Create();
    case FdoDataType_Single:
        return FdoSingleValue::Create();
    case FdoDataType_String:
        return FdoStringValue::Create();
    case FdoDataType_BLOB:
        return FdoBLOBValue::Create();
    case FdoDataType_CLOB:
        return FdoCLOBValue::Create();
    }

	FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
	throw pExcept;

    return (FdoDataValue*) NULL; // to suppress compiler warning.
}

FdoDataValue* FdoDataValue::Create(FdoString* value, FdoDataType dataType)
{
    switch (dataType)
    {
    case FdoDataType_Boolean:
		return FdoBooleanValue::Create( (FdoStringP(value)).ToBoolean() );
    case FdoDataType_Byte:
        return FdoByteValue::Create( (FdoByte)(FdoStringP(value)).ToLong() );
    case FdoDataType_DateTime:
    {
        // Construct a FdoDateTimeValue from a standard XML Schema dateTime string
        // Current support format: YYYY-MM-DDThh:mm:ss
        FdoStringP sDateTime(value);
        FdoInt16 year = (FdoInt16) sDateTime.Left(L"-").ToLong();

        sDateTime = sDateTime.Right(L"-");
        FdoInt8 month = (FdoInt8) sDateTime.Left(L"-").ToLong();

        sDateTime = sDateTime.Right(L"-");
        FdoInt8 day = (FdoInt8) sDateTime.Left(L"T").ToLong();

        sDateTime = sDateTime.Right(L"T");
        FdoInt8 hour = (FdoInt8) sDateTime.Left(L":").ToLong();

        sDateTime = sDateTime.Right(L":");
        FdoInt8 minutes = (FdoInt8) sDateTime.Left(L":").ToLong();

        sDateTime = sDateTime.Right(L":");
        FdoFloat seconds = (FdoFloat) sDateTime.Left(L":").ToDouble();

        return FdoDateTimeValue::Create(FdoDateTime(year, month, day, hour, minutes, seconds));
    }
    case FdoDataType_Decimal:
        return FdoDecimalValue::Create( (FdoStringP(value)).ToDouble() );
    case FdoDataType_Double:
		return FdoDoubleValue::Create( (FdoStringP(value)).ToDouble() );
    case FdoDataType_Int16:
        return FdoInt16Value::Create( (FdoInt16)(FdoStringP(value)).ToLong() );
    case FdoDataType_Int32:
		return FdoInt32Value::Create( (FdoStringP(value)).ToLong() );
    case FdoDataType_Int64:
    {
        FdoInt64 nValue =
        #ifdef _WIN32
                _wtoi64( FdoStringP(value) );
        #else
               atoll ( (const char*) FdoStringP(value) );   
        #endif

        return FdoInt64Value::Create(nValue);
    }
    case FdoDataType_Single:
        return FdoSingleValue::Create( (FdoFloat)(FdoStringP(value)).ToDouble() );
    case FdoDataType_String:
        return FdoStringValue::Create(value);
    //case FdoDataType_BLOB:
    //    return FdoCLOBValue::Create();
    //case FdoDataType_CLOB:
    //    return FdoCLOBValue::Create();
    }

	FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
	throw pExcept;

    return (FdoDataValue*) NULL; //to suppress compiler warning.
}

FdoString* FdoDataValue::GetStringValue(FdoDataValue* dataValue)
{
    switch (dataValue->GetDataType())
    {
    case FdoDataType_Boolean:
    case FdoDataType_Byte:
    case FdoDataType_Decimal:
    case FdoDataType_Double:
    case FdoDataType_Int16:
    case FdoDataType_Int32:
    case FdoDataType_Int64:
    case FdoDataType_Single:
    case FdoDataType_BLOB:
    case FdoDataType_CLOB:
        return dataValue->ToString();
    case FdoDataType_DateTime:
    {
        // Return standard XML Schema dateTime string
        // Current support format: YYYY-MM-DDThh:mm:ss
        FdoDateTime dateTime = (static_cast<FdoDateTimeValue*>(dataValue))->GetDateTime();
        FdoStringP sDateTime = FdoStringP::Format(L"%d-%d-%dT%d:%d:%.0f", dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.seconds);
        return FdoStringUtility::MakeString(sDateTime);
    }
    case FdoDataType_String:
        return (static_cast<FdoStringValue*>(dataValue))->GetString();
    }

	FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
	throw pExcept;

    return (FdoString*) NULL; //to suppress compiler warning.
}

// Constructs an instance of a boolean DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(bool value)
{
	return FdoBooleanValue::Create(value);
}

// Constructs an instance of a byte DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoByte value)
{
	return FdoByteValue::Create(value);
}

// Constructs an instance of a date time DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoDateTime value)
{
	return FdoDateTimeValue::Create(value);
}

// Constructs an instance of a decimal or double DataValue using the
// specified arguments.
FdoDataValue* FdoDataValue::Create(double value, FdoDataType dataType)
{
	if (dataType != FdoDataType_Decimal && dataType != FdoDataType_Double)
	{
		FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
		throw pExcept;
	}

    if (dataType == FdoDataType_Decimal)
        return FdoDecimalValue::Create(value);
    else
        return FdoDoubleValue::Create(value);
}

// Constructs an instance of a 16 bit integer DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoInt16 value)
{
	return FdoInt16Value::Create(value);
}

// Constructs an instance of a 32 bit integer DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoInt32 value)
{
	return FdoInt32Value::Create(value);
}

// Constructs an instance of a 64 bit integer DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoInt64 value)
{
	return FdoInt64Value::Create(value);
}

// Constructs an instance of a single precision floating point DataValue
// using the specified arguments.
FdoDataValue* FdoDataValue::Create(float value)
{
	return FdoSingleValue::Create(value);
}

// Constructs an instance of a string DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoString* value)
{
	return FdoStringValue::Create(value);
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoByte* value, FdoInt32 length, FdoDataType dataType)
{
	if (dataType != FdoDataType_CLOB && dataType != FdoDataType_BLOB)
	{
		FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
		throw pExcept;
	}

	FdoByteArray* pByteArray = FdoByteArray::Create(value, length);
	if (pByteArray == NULL)
	{
		FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
		throw pExcept;
	}

	FdoDataValue*	returnValue = NULL;
    try
    {
        if (dataType == FdoDataType_BLOB)
	        returnValue = FdoBLOBValue::Create(pByteArray);
        else
	        returnValue = FdoCLOBValue::Create(pByteArray);
    }
    catch (FdoException* pExcept)
    {
        pByteArray->Release();
        throw pExcept;
    }

	pByteArray->Release();
	return returnValue;
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoDataValue* FdoDataValue::Create(FdoByteArray* value, FdoDataType dataType)
{
	if (dataType != FdoDataType_CLOB && dataType != FdoDataType_BLOB)
	{
		FdoExpressionException*	pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_15_INVALIDDATAVALUE)));
		throw pExcept;
	}

    if (dataType == FdoDataType_BLOB)
	    return FdoBLOBValue::Create(value);
    else
	    return FdoCLOBValue::Create(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDataValue::FdoDataValue()
{
	m_isNull = true;
}

// Returns true if the DataValue represents a null value.
bool FdoDataValue::IsNull()
{
	return m_isNull;
}

// Sets the DataValue to a null value of the specified type.
void FdoDataValue::SetNull()
{
	m_isNull = true;
}


