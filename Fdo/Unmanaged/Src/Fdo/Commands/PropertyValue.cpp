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
#include <Fdo/Commands/PropertyValue.h>
#include <Fdo/Expression/StringValue.h>

// Constructs a default instance of a PropertyValue.
FdoPropertyValue* FdoPropertyValue::Create()
{
    return new FdoPropertyValue();
}

// Constructs an instance of a PropertyValue using the specified arguments.
FdoPropertyValue* FdoPropertyValue::Create(FdoIdentifier* name, FdoValueExpression* value)
{
    return new FdoPropertyValue(name, value);
}

// Constructs an instance of a PropertyValue using the specified arguments.
FdoPropertyValue* FdoPropertyValue::Create(FdoString* name, FdoValueExpression* value)
{
    return new FdoPropertyValue(name, value);
}

FdoPropertyValue::~FdoPropertyValue()
{
    FDO_SAFE_RELEASE(m_Name);
    FDO_SAFE_RELEASE(m_Value);
    FDO_SAFE_RELEASE(m_StreamReader);
}

void FdoPropertyValue::Dispose()
{
    delete this;
}

// Constructs a default instance of a PropertyValue.
FdoPropertyValue::FdoPropertyValue(): m_Name(NULL),m_Value(NULL), m_StreamReader(NULL)
{
}

// Constructs an instance of a PropertyValue using the specified arguments.
FdoPropertyValue::FdoPropertyValue(FdoIdentifier* name, FdoValueExpression* value)
{
	m_Name = FDO_SAFE_ADDREF(name);
	m_Value = FDO_SAFE_ADDREF(value);
    m_StreamReader = NULL;
}

// Constructs an instance of a PropertyValue using the specified arguments.
FdoPropertyValue::FdoPropertyValue(FdoString* name, FdoValueExpression* value)
{
	m_Name = FdoIdentifier::Create(name);
	m_Value = FDO_SAFE_ADDREF(value);
    m_StreamReader = NULL;
}

// Gets the property name as an Identifier.
FdoIdentifier* FdoPropertyValue::GetName()
{
    return FDO_SAFE_ADDREF(m_Name);
}

// Sets the property name as an Identifier.
void FdoPropertyValue::SetName(FdoIdentifier* value)
{
    FDO_SAFE_RELEASE(m_Name);
	m_Name = FDO_SAFE_ADDREF(value);
}

// Sets the property name as an String.
void FdoPropertyValue::SetName(FdoString* value)
{
    FDO_SAFE_RELEASE(m_Name);
	m_Name = FdoIdentifier::Create(value);
}

// Gets the value of the property as a ValueExpression.
FdoValueExpression* FdoPropertyValue::GetValue()
{
    return FDO_SAFE_ADDREF(m_Value);
}

// Sets the value of the property as a ValueExpression.
void FdoPropertyValue::SetValue(FdoValueExpression* value)
{
    FDO_SAFE_RELEASE(m_Value);
    FDO_SAFE_RELEASE(m_StreamReader);
	m_Value = FDO_SAFE_ADDREF(value);
}

// Gets the value of the property as a String. The String content 
// will be parsed to produce the required ValueExpression.
void FdoPropertyValue::SetValue(FdoString* value)
{
    FDO_SAFE_RELEASE(m_Value);
    FDO_SAFE_RELEASE(m_StreamReader);

    FdoPtr<FdoExpression> expr = FdoExpression::Parse(value);
    m_Value = FDO_SAFE_ADDREF(dynamic_cast<FdoValueExpression*>(expr.p));
    if (NULL == m_Value)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_20_STRINGNOTVALUEEXPRESSION)));
}

// Sets the value of the property as a Stream Reader. 
void FdoPropertyValue::SetStreamReader(FdoIStreamReader *stream)
{
    FDO_SAFE_RELEASE(m_Value);
    FDO_SAFE_RELEASE(m_StreamReader);
	m_StreamReader = FDO_SAFE_ADDREF(stream);
}

// Sets the value of the property as a Stream Reader. 
FdoIStreamReader* FdoPropertyValue::GetStreamReader()
{
	return FDO_SAFE_ADDREF(m_StreamReader);
}


