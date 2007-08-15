/// 
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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include "LpPropertyDefinition.h"

FdoXmlLpPropertyDefinition::FdoXmlLpPropertyDefinition( FdoPropertyDefinition* propertyDefinition, FdoXmlElementMapping* mapping ) :
m_property(propertyDefinition), m_mapping(mapping) {
    FDO_SAFE_ADDREF(m_property.p);
    FDO_SAFE_ADDREF(m_mapping.p);
}

FdoXmlLpPropertyDefinition* FdoXmlLpPropertyDefinition::Create( 
        FdoPropertyDefinition* propertyDefinition, // can be NULL
        FdoXmlElementMapping* mapping // can be NULL
        ) {
    return new FdoXmlLpPropertyDefinition(propertyDefinition, mapping);
}



// Get the FDO Property definition.
// NULL if not available
FdoPropertyDefinition* FdoXmlLpPropertyDefinition::GetPropertyDefinition() const {
    return FDO_SAFE_ADDREF(m_property.p);
}

// Get the GML element mapping.
// NULL if not available
FdoXmlElementMapping* FdoXmlLpPropertyDefinition::GetElementMapping() const {
    return FDO_SAFE_ADDREF(m_mapping.p);
}

FdoXmlLpPropertyCollection* FdoXmlLpPropertyCollection::Create(FdoXmlLpSchemaElement* parent) {
    return new FdoXmlLpPropertyCollection(parent);
}

FdoString* FdoXmlLpPropertyDefinition::GetName() {
    return m_mapping->GetName();
}











