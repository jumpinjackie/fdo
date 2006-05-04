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
#include <Fdo/Expression/DateTimeValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>
#include <stdio.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDateTimeValue* FdoDateTimeValue::Create()
{
	return new FdoDateTimeValue();
}

// Constructs an instance of a date time DataValue using the specified
// arguments.
FdoDateTimeValue* FdoDateTimeValue::Create(FdoDateTime value)
{
	return new FdoDateTimeValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoDateTimeValue::FdoDateTimeValue()
{
}

// Constructs an instance of a date time DataValue using the specified
// arguments.
FdoDateTimeValue::FdoDateTimeValue(FdoDateTime value)
{
	m_isNull = false;
	m_data = value;
}

FdoDateTimeValue::~FdoDateTimeValue()
{
}

void FdoDateTimeValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoDateTimeValue::GetDataType()
{
	return FdoDataType_DateTime;
}

// Gets the DataValue as a date time.
FdoDateTime FdoDateTimeValue::GetDateTime()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_5_DATETIMEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a date time.
void FdoDateTimeValue::SetDateTime(FdoDateTime value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoDateTimeValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessDateTimeValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoDateTimeValue::ToString()
{
    static const FdoInt32    iBufLen = 256;
    wchar_t szBuf[iBufLen];

    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(FdoStringUtility::NullString);
        return m_toString;
    }

    FdoDateTime time = GetDateTime();
    if (time.IsDate())
    {
#ifdef _WIN32
        _snwprintf(szBuf, iBufLen, L"DATE '%04d-%02d-%02d'", 
#else
        swprintf(szBuf, iBufLen, L"DATE '%04d-%02d-%02d'", 
#endif
            time.year, time.month, time.day);
    }
    else if (time.IsTime())
    {
#ifdef _WIN32
        _snwprintf(szBuf, iBufLen, L"TIME '%02d:%02d:%02g'", 
#else
        swprintf(szBuf, iBufLen, L"TIME '%02d:%02d:%02g'", 
#endif
            time.hour, time.minute, time.seconds);
    }
    else
    {
#ifdef _WIN32
        _snwprintf(szBuf, iBufLen, L"TIMESTAMP '%04d-%02d-%02d %02d:%02d:%02g'", 
#else
        swprintf(szBuf, iBufLen, L"TIMESTAMP '%04d-%02d-%02d %02d:%02d:%02g'", 
#endif
            time.year, time.month, time.day, 
            time.hour, time.minute, time.seconds);
    }

    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

