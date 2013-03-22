/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#include "stdafx.h"
#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"

FdoSmLpGrdSchema::FdoSmLpGrdSchema(FdoSmPhSchemaReaderP rdr,  FdoSmPhMgrP physicalSchema, FdoSmLpSchemaCollection* schemas) :
    FdoSmLpSchema(rdr, physicalSchema, schemas )
{
    FdoStringP tableMapping = rdr->GetTableMapping();

    if ( tableMapping != L"" ) 
        SetTableMapping(FdoSmOvTableMappingTypeMapper::String2Type(tableMapping));
}

FdoSmLpGrdSchema::FdoSmLpGrdSchema(
	FdoFeatureSchema* pFeatSchema,
    bool bIgnoreStates,
    FdoSmPhMgrP physicalSchema, 
	FdoSmLpSchemaCollection* schemas
) :
    FdoSmLpSchema( pFeatSchema, bIgnoreStates, physicalSchema, schemas )
{
}

FdoSmLpGrdSchema::~FdoSmLpGrdSchema(void)
{
}

FdoSmPhSchemaWriterP FdoSmLpGrdSchema::GetPhysicalAddWriter()
{
    FdoSmPhSchemaWriterP pWriter = FdoSmLpSchema::GetPhysicalAddWriter();

    pWriter->SetTableMapping( FdoSmOvTableMappingTypeMapper::Type2String(GetTableMapping()) );

    return pWriter;
}

void FdoSmLpGrdSchema::Update(
    FdoFeatureSchema* pFeatSchema,
	FdoSchemaElementState elementState, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoRdbmsOvPhysicalSchemaMapping* pRdbmsOverrides = dynamic_cast<FdoRdbmsOvPhysicalSchemaMapping*>(pOverrides);

    // Extract any default physical overrides

    if ( pRdbmsOverrides ) {
        if ( pRdbmsOverrides->GetTableMapping() != FdoSmOvTableMappingType_Default ) 
            SetTableMapping( pRdbmsOverrides->GetTableMapping() );
    }
 
    // Call parent's Update() first, to set elementState properly:
    FdoSmLpSchema::Update(pFeatSchema, elementState, pOverrides, bIgnoreStates);

}

bool FdoSmLpGrdSchema::SetSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const
{
    FdoInt32 i;
    bool bHasMappings = false;
    FdoRdbmsOvPhysicalSchemaMapping* grdSchemaMapping = (FdoRdbmsOvPhysicalSchemaMapping*)(FdoPhysicalSchemaMapping*)schemaMapping;

    if ( bIncludeDefaults || (GetTableMapping() != FdoSmOvTableMappingType_Default) ) {
        grdSchemaMapping->SetTableMapping( GetTableMapping() );
        bHasMappings = true;
    }

    // Get the mappings for each class, if any.
    for ( i = 0; i < RefClasses()->GetCount(); i++ ) {
        const FdoSmLpClassDefinition* pClass = RefClasses()->RefItem(i);

        bHasMappings = bHasMappings | pClass->AddSchemaMappings( schemaMapping, bIncludeDefaults );
    }
    
    return bHasMappings;
}

