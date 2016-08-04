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
#include "PropertyReader.h"
#include "../Mgr.h"

FdoSmPhCfgGrdPropertyReader::FdoSmPhCfgGrdPropertyReader(
    FdoStringP schemaName, 
    FdoStringP className, 
    FdoSmPhDbObjectP dbObject, 
    FdoSmPhMgrP mgr
) :
	FdoSmPhCfgPropertyReader(schemaName, className, dbObject, mgr),
    mRowLimit(0)
{
    FdoSchemaMappingsP mappings = mgr->GetConfigMappings();
    FdoStringP providerName = mgr->GetProviderName();

    // Extract the row sampling limit from the schema overrides
    if ( mappings ) {
        FdoRdbmsOvSchemaMappingP mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(schemaName, providerName);

        if ( mapping  ) {
            FdoRdbmsOvSchemaAutoGenerationP autoGen = mapping->GetAutoGeneration();
            if ( autoGen ) 
                mRowLimit = autoGen->GetMaxSampleRows();
        }
    }
}


FdoSmPhCfgGrdPropertyReader::~FdoSmPhCfgGrdPropertyReader(void)
{
}

FdoStringP FdoSmPhCfgGrdPropertyReader::GetAssocClassName( FdoStringP tableName )
{
    FdoSchemaMappingsP mappings = GetManager()->GetConfigMappings();
    // If table already classified, its class can be retrieved from the classification
    // cache.
    FdoStringP className = GetManager()->GetDbObjectClassification( tableName );
    int idx;

    if ( mappings && (className == L"") ) {
        // Not in classification cache so must go through all of the schema override
        // autogeneration directives to determine tables's class name
        for ( idx = 0; idx < mappings->GetCount(); idx++ ) { 
            FdoRdbmsOvSchemaMappingP mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(idx);
            FdoRdbmsOvSchemaMappingP mapping2 = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem(
               GetManager()->GetProviderName(),
                mapping->GetName()
            );

            if ( mapping2 ) {
                // Assume primary key table has identity (otherwise it can't be a primary key table).
                className = GetManager().p->SmartCast<FdoSmPhGrdMgr>()->ClassifyDbObject( tableName, mapping2, true );
                
                if ( className != L"" )
                    break; 
            }
        }
    }

    return className;
}
