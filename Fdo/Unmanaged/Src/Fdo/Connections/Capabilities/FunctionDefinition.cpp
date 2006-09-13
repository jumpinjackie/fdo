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
FdoFunctionDefinition* FdoFunctionDefinition::Create (FdoString                       *name,
                                                      FdoString                       *description,
                                                      FdoDataType                     returnType,
                                                      FdoArgumentDefinitionCollection *arguments)
{
	return new FdoFunctionDefinition(name, description, returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition* FdoFunctionDefinition::Create (FdoString                       *name,
                                                      FdoString                       *description,
                                                      FdoPropertyType                 returnPropertyType,
                                                      FdoDataType                     returnType,
                                                      FdoArgumentDefinitionCollection *arguments)
{
	return new FdoFunctionDefinition(name, description, returnPropertyType, returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition* FdoFunctionDefinition::Create (FdoString                        *name,
                                                      FdoString                        *description,
                                                      bool                             isAggregate,
                                                      FdoSignatureDefinitionCollection *signatures)
{
	return new FdoFunctionDefinition(name, description, isAggregate, signatures);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition::FdoFunctionDefinition (FdoString                       *name,
                                              FdoString                       *description,
                                              FdoDataType                     returnType,
                                              FdoArgumentDefinitionCollection *arguments)
{
	m_name        = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_isAggregate = false;
    FdoSignatureDefinition *newSignatureDefinition = FdoSignatureDefinition::Create(returnType, arguments);
    FdoSignatureDefinitionCollection *newSignatureDefinitionCollection = FdoSignatureDefinitionCollection::Create();
    newSignatureDefinitionCollection->Add(newSignatureDefinition);
    newSignatureDefinition->Release();
    m_signatures = FdoReadOnlySignatureDefinitionCollection::Create(newSignatureDefinitionCollection);
    newSignatureDefinitionCollection->Release();
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition::FdoFunctionDefinition (FdoString                       *name,
                                              FdoString                       *description,
                                              FdoPropertyType                 returnPropertyType,
                                              FdoDataType                     returnType,
                                              FdoArgumentDefinitionCollection *arguments)
{
	m_name        = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_isAggregate = false;
    FdoSignatureDefinition *newSignatureDefinition = FdoSignatureDefinition::Create(returnPropertyType, returnType, arguments);
    FdoSignatureDefinitionCollection *newSignatureDefinitionCollection = FdoSignatureDefinitionCollection::Create();
    newSignatureDefinitionCollection->Add(newSignatureDefinition);
    newSignatureDefinition->Release();
    m_signatures = FdoReadOnlySignatureDefinitionCollection::Create(newSignatureDefinitionCollection);
    newSignatureDefinitionCollection->Release();
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinition::FdoFunctionDefinition (FdoString                        *name,
                                              FdoString                        *description,
                                              bool                             isAggregate,
                                              FdoSignatureDefinitionCollection *signatures)
{
	m_name        = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_isAggregate = isAggregate;
    m_signatures  = FdoReadOnlySignatureDefinitionCollection::Create(signatures);
}

// Constructs an instance of a FunctionDefinition using default values.
FdoFunctionDefinition::FdoFunctionDefinition()
{
	m_name        = NULL;
	m_description = NULL;
    m_isAggregate = false;
    m_signatures  = NULL;
}


FdoFunctionDefinition::~FdoFunctionDefinition()
{
	FdoStringUtility::ClearString(m_name);
	FdoStringUtility::ClearString(m_description);
    FDO_SAFE_RELEASE(m_signatures);
}

void FdoFunctionDefinition::Dispose()
{
    delete this;
}

// Gets the name of the function.
FdoString *FdoFunctionDefinition::GetName()
{
    return m_name;
}

// Gets a brief description of the function.
FdoString *FdoFunctionDefinition::GetDescription()
{
    return m_description;
}

// Gets an array of ArgumentDefinitions required for the function.
FdoReadOnlyArgumentDefinitionCollection *FdoFunctionDefinition::GetArguments()
{
    if (m_signatures == NULL)
        return NULL;
    else
        return FDO_SAFE_ADDREF(FdoPtr<FdoSignatureDefinition>(m_signatures->GetItem(0))->GetArguments());
}

//  Gets the FdoPropertyType of the function return value.
FdoPropertyType FdoFunctionDefinition::GetReturnPropertyType()
{
    if (m_signatures == NULL)
        return FdoPropertyType_DataProperty;
    else
        return FdoPtr<FdoSignatureDefinition>(m_signatures->GetItem(0))->GetReturnPropertyType();
}

// Gets the DataType of the function return value.
FdoDataType FdoFunctionDefinition::GetReturnType()
{
    if (m_signatures == NULL)
        return FdoDataType_String;
    else
        return FdoPtr<FdoSignatureDefinition>(m_signatures->GetItem(0))->GetReturnType();
}

FdoReadOnlySignatureDefinitionCollection *FdoFunctionDefinition::GetSignatures()
{
    return FDO_SAFE_ADDREF(m_signatures);
}

bool FdoFunctionDefinition::IsAggregate()
{
    return m_isAggregate;
}
