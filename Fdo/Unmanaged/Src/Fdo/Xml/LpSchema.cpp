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
#include "LpSchema.h"
#include "LpClassDefinition.h"
#include "LpGmlElementDefinition.h"
#include "SchemaManager.h"

FdoXmlLpSchema::FdoXmlLpSchema( FdoFeatureSchema* schema, FdoXmlSchemaMapping* mapping) :
m_schema(schema), m_mapping(mapping) {
    FDO_SAFE_ADDREF(m_schema.p);
    FDO_SAFE_ADDREF(m_mapping.p);
}

FdoXmlLpSchema::~FdoXmlLpSchema() {
}

void FdoXmlLpSchema::Dispose() {
    delete this;
}

FdoXmlLpSchema* FdoXmlLpSchema::Create( 
    FdoFeatureSchema* schema,           // can be NULL
    FdoXmlSchemaMapping* mapping) {       // can be NULL 
    return new FdoXmlLpSchema(schema, mapping);
}

// returns the feature schema.
// NULL if not available
FdoFeatureSchema* FdoXmlLpSchema::GetFeatureSchema() {
    return FDO_SAFE_ADDREF(m_schema.p);
}
// returns the schema mappings.
// NULL if not available
FdoXmlSchemaMapping* FdoXmlLpSchema::GetMappings() {
    return FDO_SAFE_ADDREF(m_mapping.p);
}

FdoString* FdoXmlLpSchema::GetName() {
    return m_schema->GetName();
}

// Given a URI and local name for a GML element, return its Element Mapping definition
FdoXmlLpGmlElementDefinition* FdoXmlLpSchema::ElementFromGmlName( FdoString* uri, FdoString* localName ) {
    FdoXmlLpGmlElementCollection* elements = _elements();
    FdoInt32 count = elements->GetCount();
    FdoPtr<FdoXmlLpGmlElementDefinition> element;
    for (int i = 0; i < count; i++) {
        element = elements->GetItem(i);
        FdoPtr<FdoXmlElementMapping> elementMapping = element->GetElementMapping();
        if (wcscmp(elementMapping->GetGmlUri(), uri) == 0 &&
            wcscmp(elementMapping->GetGmlLocalName(), localName) == 0)
            break;
        element = NULL;
    }
    return FDO_SAFE_ADDREF(element.p);
}

// Given a URI and local name for a GML type, return the LogicalPhysical class definition
FdoXmlLpClassDefinition* FdoXmlLpSchema::ClassFromGmlType( FdoString* uri, FdoString* localName ) {
    FdoXmlLpClassCollection* classes = _classes();
    FdoInt32 count = classes->GetCount();
    FdoPtr<FdoXmlLpClassDefinition> classDef;
    for (int i = 0; i < count; i++) {
        classDef = classes->GetItem(i);
        FdoPtr<FdoXmlClassMapping> mapping = classDef->GetClassMapping();
        if (mapping != NULL) {
            // we can not get the uri from the class mapping
            // so we have to get it from the schema mapping
            FdoPtr<FdoXmlSchemaMapping> schemaMapping = static_cast<FdoXmlSchemaMapping*>(mapping->GetSchemaMapping());
            if (wcscmp(mapping->GetGmlName(), localName) == 0 &&
                wcscmp(schemaMapping->GetTargetNamespace(), uri) == 0) 
                break;
        }
        classDef = NULL;
    }
    return FDO_SAFE_ADDREF(classDef.p);
}

// Given an FDO Feature class name, return the LogicalPhysical class definition.
FdoXmlLpClassDefinition* FdoXmlLpSchema::ClassFromName( FdoString* className ) {
    FdoPtr<FdoXmlLpClassDefinition> classDef = _classes()->FindItem(className);
    return FDO_SAFE_ADDREF(classDef.p);
}

FdoXmlLpGmlElementCollection* FdoXmlLpSchema::_elements() {
    if (m_elements == NULL) {
        m_elements = FdoXmlLpGmlElementCollection::Create(this);
        FdoPtr<FdoXmlElementMappingCollection> elementMappings = m_mapping->GetElementMappings();
        FdoXmlLpClassCollection* lpClasses = _classes();
        FdoInt32 count = elementMappings->GetCount();
        FdoInt32 count1 = lpClasses->GetCount();
        for (int i = 0; i < count; i++) {
            FdoPtr<FdoXmlElementMapping> elementMapping = elementMappings->GetItem(i);
            FdoPtr<FdoXmlClassMapping> classMapping = elementMapping->GetClassMapping();
            // find its LpClassDefinition
            for (int j = 0; j < count1; j++) {
                FdoPtr<FdoXmlLpClassDefinition> lpClass = lpClasses->GetItem(j);
                FdoPtr<FdoXmlClassMapping> classMapping1 = lpClass->GetClassMapping();
                if (classMapping.p == classMapping1.p) {
                    FdoPtr<FdoXmlLpGmlElementDefinition> lpElement = FdoXmlLpGmlElementDefinition::Create(lpClass, elementMapping);
                    m_elements->Add(lpElement);
                    break;
                }
            }
        }
    }
    return m_elements.p;
}

FdoXmlLpClassCollection* FdoXmlLpSchema::_classes() {
    if (m_classes == NULL) {
        m_classes = FdoXmlLpClassCollection::Create(this);
        FdoPtr<FdoClassCollection> fdoClasses = m_schema->GetClasses();
        FdoPtr<FdoXmlClassMappingCollection> classMappings = m_mapping->GetClassMappings();
        FdoInt32 count = fdoClasses->GetCount();
        FdoInt32 count1 = classMappings->GetCount();
        for (int i = 0; i < count; i++) {
            FdoPtr<FdoClassDefinition> fdoClass = fdoClasses->GetItem(i);
            FdoPtr<FdoXmlClassMapping> classMapping;
            for (int j = 0; j < count1; j++) {
                classMapping = classMappings->GetItem(j);
                if (wcscmp(fdoClass->GetName(), classMapping->GetName()) == 0)
                    break;
                classMapping = NULL;
            }
            FdoPtr<FdoXmlLpClassDefinition> classDef = FdoXmlLpClassDefinition::Create(fdoClass, classMapping);
            m_classes->Add(classDef);
        }

        // then build up the inheritance
        for (int i = 0; i < count; i++) {
            FdoPtr<FdoXmlLpClassDefinition> iClass = m_classes->GetItem(i);
            FdoPtr<FdoClassDefinition> fdoClass = iClass->GetClassDefinition();
            FdoPtr<FdoClassDefinition> fdoBaseClass = fdoClass->GetBaseClass();
            // first try to look up in current schema
            bool found = false;
            for (int j = 0; j < count; j++) {
                FdoPtr<FdoXmlLpClassDefinition> jClass = m_classes->GetItem(j);
                FdoPtr<FdoClassDefinition> fdoClass1 = jClass->GetClassDefinition();
                if (fdoClass1.p == fdoBaseClass.p) {
                    iClass->SetBaseClass(jClass);
                    found = true;
                    break;
                }
            }
            // try other schemas
            if (!found) {
                FdoPtr<FdoXmlSchemaManager> schemaManager = static_cast<FdoXmlSchemaManager*>(GetParent());
                FdoXmlLpSchemaCollection* schemas = schemaManager->_schemas();
                FdoInt32 schemaCount = schemas->GetCount();
                for (int k = 0; k < schemaCount; k++) {
                    FdoPtr<FdoXmlLpSchema> schm = schemas->GetItem(k);
                    if (schm.p == this)
                        continue;
                    FdoXmlLpClassCollection* clsses = schm->_classes();
                    FdoInt32 clssCount = clsses->GetCount();
                    for (int k1 = 0; k1 < clssCount; k1++) {
                        FdoPtr<FdoXmlLpClassDefinition> clss = clsses->GetItem(k1);
                        FdoPtr<FdoClassDefinition> fdoClass = clss->GetClassDefinition();
                        if (fdoClass.p == fdoBaseClass.p) {
                            iClass->SetBaseClass(clss);
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }

            }
        }
    }

    return m_classes.p;
}

FdoXmlLpSchemaCollection::~FdoXmlLpSchemaCollection() {
}

void FdoXmlLpSchemaCollection::Dispose() {
    delete this;
}

FdoXmlLpSchemaCollection* FdoXmlLpSchemaCollection::Create(FdoXmlLpSchemaElement* parent) {
    return new FdoXmlLpSchemaCollection(parent);
}





