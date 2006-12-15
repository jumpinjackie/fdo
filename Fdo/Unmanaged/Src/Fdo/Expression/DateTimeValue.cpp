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
#include "ExpressionInternal.h"

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

FdoCompareType FdoInternalDateTimeValue::DoCompare( FdoDataValue* other )
{
    // Only DateTime to DateTime comparisons currently supported
    if ( other->GetDataType() == (*this)->GetDataType() ) {
        FdoDateTime dt1 = (*this)->GetDateTime();
        FdoDateTime dt2 = static_cast<FdoDateTimeValue*>(other)->GetDateTime();

        // Can't compare if one value has date component and the other does not
        if ( (dt1.IsDateTime() || dt1.IsDate()) != (dt2.IsDateTime() || dt2.IsDate()) )
            return FdoCompareType_Undefined;

        // If values have date component; compare year, month and day.
        if ( dt1.IsDateTime() || dt1.IsDate() ) {
            if ( dt1.year < dt2.year ) 
                return FdoCompareType_Less;
            else if ( dt1.year > dt2.year )
                return FdoCompareType_Greater;
            else if ( dt1.month < dt2.month )
                return FdoCompareType_Less;
            else if ( dt1.month > dt2.month )
                return FdoCompareType_Greater;
            else if ( dt1.day < dt2.day )
                return FdoCompareType_Less;
            else if ( dt1.day > dt2.day )
                return FdoCompareType_Greater;
        }

        // If neither value has a date componen t ...
        if ( (!dt1.IsDateTime()) && (!dt1.IsDate()) ){
            // Can't compare if one has time component and the other does not.
            if ( dt1.IsTime() != dt2.IsTime() ) 
                return FdoCompareType_Undefined;
        }

        // Extract time components for the two values. Assume 0's if neither has a time component. 
        FdoInt8 dt1_hour = (dt1.IsDateTime() || dt1.IsTime()) ? dt1.hour : 0;
        FdoInt8 dt2_hour = (dt2.IsDateTime() || dt2.IsTime()) ? dt2.hour : 0;
        FdoInt8 dt1_minute = (dt1.IsDateTime() || dt1.IsTime()) ? dt1.minute : 0;
        FdoInt8 dt2_minute = (dt2.IsDateTime() || dt2.IsTime()) ? dt2.minute : 0;
        FdoFloat dt1_seconds = (dt1.IsDateTime() || dt1.IsTime()) ? dt1.seconds : (float) 0;
        FdoFloat dt2_seconds = (dt2.IsDateTime() || dt2.IsTime()) ? dt2.seconds : (float) 0;

        // Compare hour, minute and seconds
        if ( dt1.hour < dt2.hour ) 
            return FdoCompareType_Less;
        else if ( dt1.hour > dt2.hour )
            return FdoCompareType_Greater;
        else if ( dt1.minute < dt2.minute )
            return FdoCompareType_Less;
        else if ( dt1.minute > dt2.minute )
            return FdoCompareType_Greater;
        else if ( dt1.seconds < dt2.seconds)
            return FdoCompareType_Less;
        else if ( dt1.seconds > dt2.seconds )
            return FdoCompareType_Greater;

        return FdoCompareType_Equal;
    }

    // Other value is not a date.
    return FdoCompareType_Undefined;
}
