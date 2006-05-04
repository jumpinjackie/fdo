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
#include <Fdo/Connections/Capabilities/FunctionDefinition.h>
#include "StringUtility.h"

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition* FdoFunctionDefinition::Create(FdoString* name, FdoString* description, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments)
{
	return new FdoFunctionDefinition(name, description, returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition* FdoFunctionDefinition::Create(FdoString* name, FdoString* description, FdoPropertyType returnPropertyType, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments)
{
	return new FdoFunctionDefinition(name, description, returnPropertyType, returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition::FdoFunctionDefinition(FdoString* name, FdoString* description, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments)
{
	m_name = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_returnPropertyType = FdoPropertyType_DataProperty;
	m_returnDataType = returnType;
    m_arguments = FdoReadOnlyArgumentDefinitionCollection::Create(arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition::FdoFunctionDefinition(FdoString* name, FdoString* description, FdoPropertyType returnPropertyType, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments)
{
	m_name = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
	m_returnPropertyType = returnPropertyType;
	m_returnDataType = returnType;
    m_arguments = FdoReadOnlyArgumentDefinitionCollection::Create(arguments);
}

// Constructs an instance of a FunctionDefinition using default values.
FdoFunctionDefinition::FdoFunctionDefinition()
{
	m_name = NULL;
	m_description = NULL;
    m_returnPropertyType = FdoPropertyType_DataProperty;
    m_returnDataType = FdoDataType_String;
    m_arguments = NULL;
}


FdoFunctionDefinition::~FdoFunctionDefinition()
{
	FdoStringUtility::ClearString(m_name);
	FdoStringUtility::ClearString(m_description);
    FDO_SAFE_RELEASE(m_arguments);
}

void FdoFunctionDefinition::Dispose()
{
    delete this;
}

// Gets the name of the function.
FdoString* FdoFunctionDefinition::GetName()
{
    return m_name;
}

// Gets a brief description of the function.
FdoString* FdoFunctionDefinition::GetDescription()
{
    return m_description;
}

// Gets an array of ArgumentDefinitions required for the function.
FdoReadOnlyArgumentDefinitionCollection* FdoFunctionDefinition::GetArguments()
{
    return FDO_SAFE_ADDREF(m_arguments);
}

//  Gets the FdoPropertyType of the function return value.
FdoPropertyType FdoFunctionDefinition::GetReturnPropertyType()
{
    return m_returnPropertyType;
}

// Gets the DataType of the function return value.
FdoDataType FdoFunctionDefinition::GetReturnType()
{
	return m_returnDataType;
}
