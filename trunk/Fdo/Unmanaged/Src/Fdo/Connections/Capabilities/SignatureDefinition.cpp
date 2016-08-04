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
#include <Fdo/Connections/Capabilities/SignatureDefinition.h>
#include "StringUtility.h"


/// \brief
/// Gets an array of FdoArgumentDefinition objects required for the function. 
FdoReadOnlyArgumentDefinitionCollection* FdoSignatureDefinition::GetArguments()
{
    return FDO_SAFE_ADDREF(m_arguments);
}

/// \brief
/// Gets the FdoPropertyType of the function return value.
FdoPropertyType FdoSignatureDefinition::GetReturnPropertyType()
{
    return m_returnPropertyType;
}
/// \brief
/// Gets the FdoDataType of the function return value. This is only valid if the function return property type is
/// FdoPropertyType_DataProperty.
FdoDataType FdoSignatureDefinition::GetReturnType ()
{
    return m_returnDataType;
}


// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoSignatureDefinition* FdoSignatureDefinition::Create (FdoDataType                     returnType,
                                                        FdoArgumentDefinitionCollection *arguments)
{
    return new FdoSignatureDefinition(returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoSignatureDefinition* FdoSignatureDefinition::Create (FdoPropertyType                 returnPropertyType,
                                                        FdoDataType                     returnType,
                                                        FdoArgumentDefinitionCollection *arguments)
{
	return new FdoSignatureDefinition(returnPropertyType, returnType, arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoSignatureDefinition::FdoSignatureDefinition (FdoDataType                     returnType,
                                                FdoArgumentDefinitionCollection *arguments)
{
    m_returnPropertyType = FdoPropertyType_DataProperty;
	m_returnDataType     = returnType;
    m_arguments          = FdoReadOnlyArgumentDefinitionCollection::Create(arguments);
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoSignatureDefinition::FdoSignatureDefinition (FdoPropertyType                 returnPropertyType,
                                                FdoDataType                     returnType,
                                                FdoArgumentDefinitionCollection *arguments)
{
	m_returnPropertyType = returnPropertyType;
	m_returnDataType     = returnType;
    m_arguments          = FdoReadOnlyArgumentDefinitionCollection::Create(arguments);
}

// Constructs an instance of a FunctionDefinition using default values.
FdoSignatureDefinition::FdoSignatureDefinition()
{
    m_returnPropertyType = FdoPropertyType_DataProperty;
    m_returnDataType     = FdoDataType_String;
    m_arguments          = NULL;
}

FdoSignatureDefinition::~FdoSignatureDefinition()
{
    FDO_SAFE_RELEASE(m_arguments);
}

void FdoSignatureDefinition::Dispose()
{
    delete this;
}

// -----------------------------------------------------------------------------------------------------

FdoSignatureDefinitionCollection *FdoSignatureDefinitionCollection::Create()
{
	return new FdoSignatureDefinitionCollection();
}

FdoSignatureDefinitionCollection *FdoSignatureDefinitionCollection::Create(FdoSignatureDefinition **arguments, FdoInt32 length)
{
    FdoSignatureDefinitionCollection *pColl = new FdoSignatureDefinitionCollection();

    try
    {
        for (FdoInt32 i = 0; i < length; i++)
            pColl->Add(arguments[i]);
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pColl);
        throw pExcept;
    }

    return pColl;
}


// -----------------------------------------------------------------------------------------------------

// Constructs an instance of a FunctionDefinition using default values.
FdoReadOnlySignatureDefinitionCollection::FdoReadOnlySignatureDefinitionCollection()
{
}


FdoReadOnlySignatureDefinitionCollection::~FdoReadOnlySignatureDefinitionCollection()
{
}

// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoReadOnlySignatureDefinitionCollection *FdoReadOnlySignatureDefinitionCollection::Create (FdoSignatureDefinitionCollection *col)
{
    FdoReadOnlySignatureDefinitionCollection *readOnlyCol;
	readOnlyCol = new FdoReadOnlySignatureDefinitionCollection();
    readOnlyCol->SetBaseCollection(col);
    return readOnlyCol;
}
