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
#include <Fdo/Connections/Capabilities/ArgumentDefinition.h>
#include "StringUtility.h"

// Constructs an instance of a ArgumentDefinition using the specified arguments.
FdoArgumentDefinition *FdoArgumentDefinition::Create (FdoString   *name,
                                                      FdoString   *description,
                                                      FdoDataType dataType)
{
	return new FdoArgumentDefinition(name, description, dataType);
}

// Constructs an instance of a ArgumentDefinition using the specified arguments.
FdoArgumentDefinition *FdoArgumentDefinition::Create (FdoString       *name,
                                                      FdoString       *description,
                                                      FdoPropertyType propertyType,
                                                      FdoDataType     dataType)
{
	return new FdoArgumentDefinition(name, description, propertyType, dataType);
}

// Constructs an instance of a ArgumentDefinition using the specified arguments.
FdoArgumentDefinition::FdoArgumentDefinition (FdoString   *name,
                                              FdoString   *description,
                                              FdoDataType dataType)
{
	m_name = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_propertyType = FdoPropertyType_DataProperty;
	m_dataType = dataType;
    m_argumentValueList = NULL;
}

// Constructs an instance of a ArgumentDefinition using the specified arguments.
FdoArgumentDefinition::FdoArgumentDefinition (FdoString       *name,
                                              FdoString       *description,
                                              FdoPropertyType propertyType,
                                              FdoDataType     dataType)
{
	m_name = FdoStringUtility::MakeString(name);
	m_description = FdoStringUtility::MakeString(description);
    m_propertyType = propertyType;
	m_dataType = dataType;
    m_argumentValueList = NULL;
}

// Constructs an instance of a ArgumentDefinition using default property values.
FdoArgumentDefinition::FdoArgumentDefinition ()
{
	m_name = NULL;
	m_description = NULL;
    m_propertyType = FdoPropertyType_DataProperty;
    m_dataType = FdoDataType_String;
    m_argumentValueList = NULL;
}


FdoArgumentDefinition::~FdoArgumentDefinition ()
{
	FdoStringUtility::ClearString(m_name);
	FdoStringUtility::ClearString(m_description);
    FDO_SAFE_RELEASE(m_argumentValueList);
}

void FdoArgumentDefinition::Dispose ()
{
    delete this;
}

// Gets the name of the argument.
FdoString *FdoArgumentDefinition::GetName ()
{
    return m_name;
}

// Gets a brief description of the argument.
FdoString *FdoArgumentDefinition::GetDescription ()
{
    return m_description;
}

// Gets the DataType of the argument.
FdoPropertyType FdoArgumentDefinition::GetPropertyType ()
{
    return m_propertyType;
}

// Gets the DataType of the argument.
FdoDataType FdoArgumentDefinition::GetDataType ()
{
    return m_dataType;
}

// Returns the argument value list of the argument
FdoPropertyValueConstraintList *FdoArgumentDefinition::GetArgumentValueList ()
{
    return FDO_SAFE_ADDREF(m_argumentValueList);
}

// Sets the argument value list of the argument
void FdoArgumentDefinition::SetArgumentValueList (FdoPropertyValueConstraintList *argumentValueList)
{
    FDO_SAFE_RELEASE(m_argumentValueList);
    m_argumentValueList = NULL;

    if (argumentValueList == NULL)
        return;

    m_argumentValueList = FdoPropertyValueConstraintList::Create();

    FdoPtr<FdoDataValueCollection> dest_values = m_argumentValueList->GetConstraintList();
    FdoPtr<FdoDataValueCollection> src_values = argumentValueList->GetConstraintList();
    FdoInt32 count = src_values->GetCount();
    for (FdoInt32 i = 0; i < count; i++)
    {
        FdoPtr<FdoDataValue> data_value = src_values->GetItem(i);
        dest_values->Add(data_value);
    }
}

