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

#include <Fdo.h>
#include "LpClassDefinition.h"
#include "LpPropertyDefinition.h"

FdoXmlLpClassDefinition::FdoXmlLpClassDefinition( FdoClassDefinition* classDefinition, FdoXmlClassMapping* classMapping ) :
m_class(classDefinition), m_mapping(classMapping) {
    FDO_SAFE_ADDREF(m_class.p);
    FDO_SAFE_ADDREF(m_mapping.p);
}

FdoXmlLpClassDefinition::~FdoXmlLpClassDefinition() {
}

FdoXmlLpClassDefinition* FdoXmlLpClassDefinition::Create( 
    FdoClassDefinition* classDefinition,    // can be NULL
    FdoXmlClassMapping* mapping             // can be NULL
) {
    return new FdoXmlLpClassDefinition(classDefinition, mapping);
}


// returns the FDO class definition.
// NULL if not available
FdoClassDefinition* FdoXmlLpClassDefinition::GetClassDefinition() {
    return FDO_SAFE_ADDREF(m_class.p);
}

// returns the class mapping.
// NULL if not available
FdoXmlClassMapping* FdoXmlLpClassDefinition::GetClassMapping() {
    return FDO_SAFE_ADDREF(m_mapping.p);
}

FdoString* FdoXmlLpClassDefinition::GetName() {
    return m_class->GetName();
}

// Given a GML URI and local name, return the LogicalPhysical property definition.
// The property can be one inherited from a base class.
// Returns NULL if this class does not contain this property.
FdoXmlLpPropertyDefinition* FdoXmlLpClassDefinition::PropertyFromGml( FdoString* uri, FdoString* localName ) {
    // first find in current class
    FdoInt32 count = _properties()->GetCount();
    FdoPtr<FdoXmlLpPropertyDefinition> prop;
    for (int i = 0; i < count; i++) {
        prop = _properties()->GetItem(i);
        FdoPtr<FdoXmlElementMapping> elementMapping = prop->GetElementMapping();
        if (elementMapping != NULL && wcscmp(elementMapping->GetGmlUri(), uri) == 0 &&
            wcscmp(elementMapping->GetGmlLocalName(), localName) == 0)
            break;
        prop = NULL;
    }
    // if not found, find it in base class
    if (prop == NULL && m_baseClass != NULL) {
        FdoPtr<FdoXmlLpPropertyCollection> props = m_baseClass->GetProperties();
        FdoInt32 count = props->GetCount();
        for (int i = 0; i < count; i++) {
            prop = props->GetItem(i);
            FdoPtr<FdoXmlElementMapping> elementMapping = prop->GetElementMapping();
            if (elementMapping != NULL && wcscmp(elementMapping->GetGmlUri(), uri) == 0 &&
                wcscmp(elementMapping->GetGmlLocalName(), localName) == 0)
                break;
            prop = NULL;
        }
    }

    return FDO_SAFE_ADDREF(prop.p);
}

// Given an FDO property name, return the LogicalPhysical property definition.
// The property can be one inherited from a base class.
// Returns NULL if this class does not contain this property.
FdoXmlLpPropertyDefinition* FdoXmlLpClassDefinition::PropertyFromName( FdoString* propertyName ) {
    // first find in current class
    FdoInt32 count = _properties()->GetCount();
    FdoPtr<FdoXmlLpPropertyDefinition> prop;
    for (int i = 0; i < count; i++) {
        prop = _properties()->GetItem(i);
        FdoPtr<FdoPropertyDefinition> fdoProp = prop->GetPropertyDefinition();
        if (wcscmp(fdoProp->GetName(), propertyName) == 0)
            break;
        prop = NULL;
    }
    // if not found, find it in base class
    if (prop == NULL && m_baseClass != NULL) {
        FdoPtr<FdoXmlLpPropertyCollection> props = m_baseClass->GetProperties();
        FdoInt32 count = props->GetCount();
        for (int i = 0; i < count; i++) {
            prop = props->GetItem(i);
            FdoPtr<FdoPropertyDefinition> fdoProp = prop->GetPropertyDefinition();
            if (wcscmp(fdoProp->GetName(), propertyName) == 0)
                break;
            prop = NULL;
        }
    }

    return FDO_SAFE_ADDREF(prop.p);
}

// Get the LogicalPhysical definition for the base class.
// NULL if this class has no base class.
FdoXmlLpClassDefinition* FdoXmlLpClassDefinition::GetBaseClass() {
    return FDO_SAFE_ADDREF(m_baseClass.p);
}

void FdoXmlLpClassDefinition::SetBaseClass(FdoXmlLpClassDefinition* baseClass) {
    m_baseClass = FDO_SAFE_ADDREF(baseClass);
}


// Get the list of properties (in LogicalPhysical form) for this class.
// Includes inherited properties.
FdoXmlLpPropertyCollection* FdoXmlLpClassDefinition::GetProperties() {
    // this funtion will be implemented recursively
    // first call itself on the base class to get all properties including inherited propertes
    // then add all properties of current class to the collection
    FdoPtr<FdoXmlLpPropertyCollection> props; 
    // first get all properties from base class including all inherited properties
    if (m_baseClass != NULL)
        props = m_baseClass->GetProperties();

    if (props == NULL)
        props = FdoXmlLpPropertyCollection::Create(NULL);

    // then add all properties of current class to the return collection
    FdoInt32 count = _properties()->GetCount();
    for (int i = 0; i < count; i++) {
        FdoPtr<FdoXmlLpPropertyDefinition> prop = _properties()->GetItem(i);
        props->Add(prop);
    }
    return FDO_SAFE_ADDREF(props.p);
}

FdoXmlLpPropertyCollection* FdoXmlLpClassDefinition::_properties() {
    if (m_properties == NULL) {
        m_properties = FdoXmlLpPropertyCollection::Create(this);
        FdoPtr<FdoPropertyDefinitionCollection> props = m_class->GetProperties();
        FdoPtr<FdoXmlElementMappingCollection> elements = m_mapping->GetElementMappings();
        FdoInt32 count = props->GetCount();
        FdoInt32 count1 = elements->GetCount();
        for (int i = 0; i < count1; i++) {
            FdoPtr<FdoXmlElementMapping> elementMapping = elements->GetItem(i);
            FdoPtr<FdoPropertyDefinition> prop;
            for (int j = 0; j < count; j++) {
                prop = props->GetItem(j);
                if (wcscmp(prop->GetName(), elementMapping->GetName()) == 0) 
                    break;
                prop = NULL;
            }
            FdoPtr<FdoXmlLpPropertyDefinition> lpXmlProperty = FdoXmlLpPropertyDefinition::Create(prop, elementMapping);
            m_properties->Add(lpXmlProperty);
        }
    }
    return m_properties.p;
}

FdoXmlLpClassCollection* FdoXmlLpClassCollection::Create(FdoXmlLpSchemaElement* parent) {
    return new FdoXmlLpClassCollection(parent);
}






