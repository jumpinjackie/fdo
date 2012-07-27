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
#include <Fdo/Commands/ParameterValue.h>
#include "StringUtility.h"

// Constructs a default instance of a ParameterValue.
FdoParameterValue* FdoParameterValue::Create()
{
	return new FdoParameterValue();
}

// Constructs an instance of a ParameterValue using the specified arguments.
FdoParameterValue* FdoParameterValue::Create(FdoString* name)
{
	return new FdoParameterValue(name);
}

// Constructs an instance of a ParameterValue using the specified arguments.
FdoParameterValue* FdoParameterValue::Create(FdoString* name, FdoLiteralValue* value)
{
	return new FdoParameterValue(name, value);
}

// Constructs a default instance of a ParameterValue.
FdoParameterValue::FdoParameterValue()
{
	m_name = NULL;
	m_value = NULL;
    m_parameterDirection = FdoParameterDirection_Input;
}

// Constructs an instance of a ParameterValue using the specified arguments.
FdoParameterValue::FdoParameterValue(FdoString* name)
{
    m_name = FdoStringUtility::MakeString(name);
	m_value = NULL;
    m_parameterDirection = FdoParameterDirection_Input;
}

// Constructs an instance of a ParameterValue using the specified arguments.
FdoParameterValue::FdoParameterValue(FdoString* name, FdoLiteralValue* value)
{
    m_name = FdoStringUtility::MakeString(name);
	m_value = FDO_SAFE_ADDREF(value);
    m_parameterDirection = FdoParameterDirection_Input;
}

FdoParameterValue::~FdoParameterValue()
{
    FdoStringUtility::ClearString(m_name);
    FDO_SAFE_RELEASE(m_value);
}

void FdoParameterValue::Dispose()
{
    delete this;
}

// Gets the parameter name the value should bind to as a string.
FdoString* FdoParameterValue::GetName()
{
    return m_name;
}

// Sets the parameter name the value should bind to as a string.
void FdoParameterValue::SetName(FdoString* value)
{
    FdoStringUtility::ClearString(m_name);
	m_name = FdoStringUtility::MakeString(value);
}

// Gets the value to bind to the parameter as a LiteralValue.
FdoLiteralValue* FdoParameterValue::GetValue()
{
    return FDO_SAFE_ADDREF(m_value);
}

// Sets the value to bind to the parameter as a LiteralValue.
void FdoParameterValue::SetValue(FdoLiteralValue* value)
{
	FDO_SAFE_RELEASE(m_value);
	m_value = FDO_SAFE_ADDREF(value);
}

// Gets the value to bind to the parameter as a String.
void FdoParameterValue::SetValue(FdoString* value)
{
	FDO_SAFE_RELEASE(m_value);
	m_value = FdoStringValue::Create(value);
}

/// Sets the function direction of the parameter value.
/// Values can be Input, Output, InputOutput and Return.
/// The default value for direction is Input.
void FdoParameterValue::SetDirection(FdoParameterDirection value)
{
    m_parameterDirection = value;
}

/// Gets the function direction of the the command parameter.
/// Values can be Input, Output, InputOutput and Return.
FdoParameterDirection FdoParameterValue::GetDirection()
{
    return m_parameterDirection;
}

