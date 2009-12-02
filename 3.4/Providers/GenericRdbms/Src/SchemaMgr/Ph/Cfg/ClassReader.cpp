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
#include "ClassReader.h"
#include "../Mgr.h"
#include <Sm/Ph/Rd/ClassReader.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>

FdoSmPhCfgGrdClassReader::FdoSmPhCfgGrdClassReader(FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr) :
    FdoSmPhCfgClassReader( (FdoSmPhReader*) NULL )
{
    SetSubReader( MakeReader(rows, schemaName, mgr) );
}


FdoSmPhCfgGrdClassReader::~FdoSmPhCfgGrdClassReader(void)
{
}

bool FdoSmPhCfgGrdClassReader::ReadNext()
{
    bool                                found = false;

    while ( !found ) {
        if ( !FdoSmPhReader::ReadNext() ) {
            SetEOF(true);
            break;
        }
 
        FdoStringP tableName = GetString(L"", L"tablename");

        // Skip tables that were already classified
        if ( GetManager()->GetDbObjectClassification(tableName) == L"" ) {

            // Classify the table.
            FdoStringP qClassName = GetManager().p->SmartCast<FdoSmPhGrdMgr>()->ClassifyDbObject( tableName, mConfigMapping, GetBoolean(L"", L"hasKey") );

            if ( qClassName != L"" ) {
                // table classifies into current table so make its class the current class. 
                found = true;
                FdoStringsP classTokens = FdoStringCollection::Create( qClassName, L":" );
                SetString( L"", L"classname", classTokens->GetString(1) );
                SetString( L"", L"tablelinkname", mDatabase );
                SetString( L"", L"tableowner", mOwner );

                // Record this table as having been classified.
                GetManager()->SetDbObjectClassification( tableName, qClassName );
            }
        }
    }

	return(!IsEOF());
}


FdoSmPhReaderP FdoSmPhCfgGrdClassReader::MakeReader(FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr)
{
    FdoSmPhReader*              pSubReader = NULL;
    FdoStringP                  providerName = mgr->GetProviderName();
    FdoSchemaMappingsP          mappings = mgr->GetConfigMappings();
    FdoFeatureSchemasP          schemas = mgr->GetConfigSchemas();
    FdoSmPhGrdMgrP              grdMgr = mgr.p->SmartCast<FdoSmPhGrdMgr>();

    // Get the schema override set for the current schema.
    if ( mappings )
        mConfigMapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(providerName, schemaName);
   
    // Record database and owner if schema is foreign.
    mDatabase = grdMgr->GetOverrideDatabase(mConfigMapping);
    mOwner = grdMgr->GetOverrideOwner(mConfigMapping);

    // Do a case-neutral match of owner name to existing owner
    if ( mOwner != L"" ) {
        FdoSmPhOwnerP pOwner = grdMgr->FindOwner( mOwner, mDatabase, false );
        if ( pOwner ) {
            // if owner found, update the owner and database names in case
            // the names didn't match exactly (character case difference).
            mOwner = pOwner->GetName();
            mDatabase = pOwner->GetParent()->GetName();
        }
    }

    // Skip Schemas that have no autogeneration directives (no classes to generate
    // in this case).
    if ( mConfigMapping ) {
        if ( !FdoRdbmsOvSchemaAutoGenerationP(mConfigMapping->GetAutoGeneration()) )
            mConfigMapping = NULL;
    }


    FdoSmPhRowP classRow = rows->GetItem(0);

    if ( mConfigMapping ) {
        // read all tables in the datastore. This class will filter out ones that 
        // don't belong to the current feature schema
        pSubReader = new FdoSmPhRdClassReader( 
            rows, 
            L"", 
            L"", 
            mgr,
            false,
            mDatabase,
            mOwner
        );
    }
    else {
        // Nothing to read, set an empty reader.
        pSubReader = new FdoSmPhReader( mgr, rows );
    }

    return pSubReader;
}

