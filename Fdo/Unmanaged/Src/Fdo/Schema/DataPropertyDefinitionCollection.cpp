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
#include <Fdo/Schema/DataPropertyDefinitionCollection.h>

// Constructs an instance of a DataPropertyDefinitionCollection using the specified argument.
FDO_API FdoDataPropertyDefinitionCollection* FdoDataPropertyDefinitionCollection::Create(FdoSchemaElement* parent)
{
    return new FdoDataPropertyDefinitionCollection(parent);
}

FDO_API void FdoDataPropertyDefinitionCollection::SetItem(FdoInt32 index, FdoDataPropertyDefinition* value)
{
    // verify the Obj is already in the parent's Property collection
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING) && // don't verify during Accept/RejectChanges()
        m_parent && !m_parent->IsProperty(value))
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_4_IDENTITYPROPERTYNOTPROPERTY)));

    FdoSchemaCollection<FdoDataPropertyDefinition>::SetItem(index, value);
}

FDO_API FdoInt32 FdoDataPropertyDefinitionCollection::Add(FdoDataPropertyDefinition* value)
{
    // verify the Obj is already in the parent's Property collection
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING) && // don't verify during Accept/RejectChanges()
		m_parent) 
	{
		// Identity Property must be a property of the parent
		if ( !m_parent->IsProperty(value))
			throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_4_IDENTITYPROPERTYNOTPROPERTY)));

		// A class with base class cannot have its own identity properties.
		if ( m_parent->IsSubClass())
			throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_13_IDENTITYPROPERTYONSUBCLASS)));
	}
    return FdoSchemaCollection<FdoDataPropertyDefinition>::Add(value);
}

FDO_API void FdoDataPropertyDefinitionCollection::Insert(FdoInt32 index, FdoDataPropertyDefinition* value)
{
    // verify the Obj is already in the parent's Property collection
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING) && // don't verify during Accept/RejectChanges()
        m_parent && !m_parent->IsProperty(value))
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_4_IDENTITYPROPERTYNOTPROPERTY)));

    return FdoSchemaCollection<FdoDataPropertyDefinition>::Insert(index, value);
}

