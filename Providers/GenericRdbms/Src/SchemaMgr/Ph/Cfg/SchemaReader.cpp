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
#include "SchemaReader.h"
#include "../Mgr.h"

FdoSmPhCfgGrdSchemaReader::FdoSmPhCfgGrdSchemaReader(FdoSmPhRowsP rows, FdoSmPhMgrP mgr) :
	FdoSmPhCfgSchemaReader(rows, mgr),
    mIdx(-1)
{
}


FdoSmPhCfgGrdSchemaReader::~FdoSmPhCfgGrdSchemaReader(void)
{
}

bool FdoSmPhCfgGrdSchemaReader::ReadNext()
{
    bool                        found = false;
    FdoSchemaMappingsP          mappings = GetManager()->GetConfigMappings();
    FdoFeatureSchemasP          schemas = GetManager()->GetConfigSchemas();
    FdoStringP                  providerName = GetManager()->GetProviderName();
    FdoFeatureSchemaP           schema;
    FdoRdbmsOvSchemaMappingP    mapping;
    FdoRdbmsOvSchemaMappingP    mapping2;
    FdoStringP                  schemaName;
    FdoSmPhGrdMgrP              grdMgr = GetManager().p->SmartCast<FdoSmPhGrdMgr>();

    while ( !IsEOF() && !found ) {
        // Check the next schema override set.
        mIdx++;

        if ( mIdx >= mappings->GetCount() ) {
            SetEOF(true);
            break;
		}

        mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(mIdx);
        schemaName = mapping->GetName();
        mapping2 = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(
            providerName,
            schemaName
        );
        schema = schemas->FindItem( schemaName );

        if ( mapping2 && FdoRdbmsOvSchemaAutoGenerationP(mapping2->GetAutoGeneration()) && (!schema) ) {
            // Schema has autogeneration directives so make it the current one
            FdoSmPhRowsP rows = GetRows();
            FdoSmPhRowP  row = rows ? rows->GetItem(0) : NULL;

            if ( row ) {
                // Set some fields from the config document.
                FdoSmPhFieldP field = row->GetFields()->GetItem(L"schemaname");
                field->SetFieldValue( schemaName );

                field = row->GetFields()->GetItem(L"tablemapping");
                field->SetFieldValue( FdoSmOvTableMappingTypeMapper::Type2String(mapping2->GetTableMapping()) );

                field = row->GetFields()->GetItem(L"tablelinkname");
                field->SetFieldValue( grdMgr->GetOverrideDatabase(mapping2) );

                field = row->GetFields()->GetItem(L"tableowner");
                field->SetFieldValue( grdMgr->GetOverrideOwner(mapping2) );
            }
            else {
                SetEOF(true);
                break;
            }

            found = true;
            SetBOF(false);
        }
    }

	return(!IsEOF());
}

