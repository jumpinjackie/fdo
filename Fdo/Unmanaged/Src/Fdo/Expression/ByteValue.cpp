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
#include <Fdo/Expression/ByteValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

#include <time.h>
#include <stdio.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoByteValue* FdoByteValue::Create()
{
	return new FdoByteValue();
}

// Constructs an instance of a byte DataValue using the specified
// arguments.
FdoByteValue* FdoByteValue::Create(FdoByte value)
{
	return new FdoByteValue(value);
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoByteValue::FdoByteValue()
{
    m_data = 0;
}

// Constructs an instance of a byte DataValue using the specified
// arguments.
FdoByteValue::FdoByteValue(FdoByte value)
{
	m_isNull = false;
	m_data = value;
}

FdoByteValue::~FdoByteValue()
{
}

void FdoByteValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoByteValue::GetDataType()
{
	return FdoDataType_Byte;
}

// Gets the DataValue as a byte.
FdoByte FdoByteValue::GetByte()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_3_BYTEVALUENULL)));
        throw pExcept;
    }

	return m_data;
}

// Sets the DataValue as a byte.
void FdoByteValue::SetByte(FdoByte value)
{
	m_isNull = false;
	m_data = value;
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoByteValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessByteValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoByteValue::ToString()
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
    _snwprintf(szBuf, iBufLen, L"\\0x%02X", (FdoInt32)GetByte());
#else
    swprintf(szBuf, iBufLen, L"\\0x%02X", (FdoInt32)GetByte());
#endif
    m_toString = FdoStringUtility::MakeString(szBuf);
    return m_toString;
}

