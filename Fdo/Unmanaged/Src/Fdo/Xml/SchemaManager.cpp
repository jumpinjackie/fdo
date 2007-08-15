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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include "SchemaManager.h"
#include "LpSchema.h"
//#include "LpGmlElementDefinition.h"
//#include <Fdo/Xml/FeatureFlags.h>

FdoXmlSchemaManager::FdoXmlSchemaManager() {
}

FdoXmlSchemaManager::FdoXmlSchemaManager( FdoFeatureSchemaCollection* schemas, FdoXmlFlags* flags):
m_fdoSchemas(schemas), m_flags(flags) {
    FDO_SAFE_ADDREF(m_fdoSchemas.p);
    FDO_SAFE_ADDREF(m_flags.p);
}

FdoXmlSchemaManager::~FdoXmlSchemaManager() {
}

void FdoXmlSchemaManager::Dispose() {
    delete this;
}

FdoXmlSchemaManager* FdoXmlSchemaManager::Create( FdoFeatureSchemaCollection* schemas, FdoXmlFlags* flags) {
    return new FdoXmlSchemaManager(schemas, flags);
}

FdoXmlLpSchemaCollection* FdoXmlSchemaManager::_schemas() {
    if (m_lpSchemas == NULL) {
        m_lpSchemas = FdoXmlLpSchemaCollection::Create(this);
        FdoInt32 count = m_fdoSchemas->GetCount();
        FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = m_flags->GetSchemaMappings();
        FdoInt32 count1 = mappings->GetCount();
        FdoPtr<FdoXmlSchemaMapping> tempXmlMapping = FdoXmlSchemaMapping::Create(L"temp");
        for (int i = 0; i < count; i++) {
            FdoPtr<FdoFeatureSchema> schema = m_fdoSchemas->GetItem(i);
            FdoPtr<FdoXmlSchemaMapping> mapping = static_cast<FdoXmlSchemaMapping*>(mappings->GetItem(tempXmlMapping->GetProvider(), schema->GetName()));
            FdoPtr<FdoXmlLpSchema> lpSchema = FdoXmlLpSchema::Create(schema, mapping);
            m_lpSchemas->Add(lpSchema);
        }
    }
    return m_lpSchemas.p;

}

// Given a GML URI name, return the LogicalPhysical schema object, which contains the
// Feature Schema and corresponding Schema Mapping set.
// Returns NULL if the schema is not present.
FdoXmlLpSchema* FdoXmlSchemaManager::UriToSchema( FdoString* schemaUri ) {
    FdoXmlLpSchemaCollection* lpSchemas = _schemas();
    FdoInt32 count = lpSchemas->GetCount();
    FdoPtr<FdoXmlLpSchema> lpSchema;
    for (int i = 0; i < count; i++) {
        lpSchema = lpSchemas->GetItem(i);
        FdoPtr<FdoXmlSchemaMapping> mapping = lpSchema->GetMappings();
        if (wcscmp(mapping->GetTargetNamespace(), schemaUri) == 0)
            break;
        lpSchema = NULL;
    }
    return FDO_SAFE_ADDREF(lpSchema.p);
}

// Given an FDO Feature Schema name, return the LogicalPhysical schema object
// Returns NULL if the schema is not present.
FdoXmlLpSchema* FdoXmlSchemaManager::NameToSchema( FdoString* schemaName ) {

    FdoPtr<FdoXmlLpSchema> lpSchema = _schemas()->FindItem(schemaName);
    return FDO_SAFE_ADDREF(lpSchema.p);
}






