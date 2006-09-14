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
#include "stdafx.h"
#include "SdfExtHashMap.h"
#include "SdfSchemaMergeContext.h"
#include "SdfConnection.h"
#include "DataDb.h"
#include "KeyDb.h"

SdfSchemaMergeContext::SdfSchemaMergeContext( 
    SdfConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
) :
    FdoSchemaMergeContext( oldSchemas ),
    mSdfConnection(connection)
{
    SetConnection( connection );
    SetUpdSchema( newSchema );
    SetIgnoreStates( ignoreStates );

    mTableReformatters = new TableReformatterCollection();
}

SdfSchemaMergeContext::~SdfSchemaMergeContext(void)
{
}

SdfSchemaMergeContext* SdfSchemaMergeContext::Create( 
    SdfConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
)
{
    return new SdfSchemaMergeContext( connection, oldSchemas, newSchema, ignoreStates );
}

bool SdfSchemaMergeContext::CanAddClass( FdoClassDefinition* classDef )
{
    return true;
}

bool SdfSchemaMergeContext::CanDeleteClass( FdoClassDefinition* classDef )
{
    return true;
}

bool SdfSchemaMergeContext::CanAddProperty( FdoPropertyDefinition* prop )
{
    return true;
}

bool SdfSchemaMergeContext::CanDeleteProperty( FdoPropertyDefinition* prop )
{
    return true;
}

bool SdfSchemaMergeContext::CanModElementDescription( FdoSchemaElement* element )
{
    return true;
}

bool SdfSchemaMergeContext::CanDeleteSchema( FdoFeatureSchema* schema )
{
    return true;
}

void SdfSchemaMergeContext::Merge()
{
    CommitSchemas();
}

void SdfSchemaMergeContext::PreAcceptChanges()
{
    FdoInt32 idx;
    SchemaDb* schemaDb = mSdfConnection->GetSchemaDb();

// Gather up lists of the Data, Key and Rtree tables that need to be deleted.
// These are deleted by PostUpdatePhysical. 
//
// Also gather up lists of data tables whose blobs need updating due to class id changes.

    FdoFeatureSchemasP schemas = GetSchemas();

    // SdfConnection hash tables keyed by ClassDefinition address so need the
    // connection's current schemas. 
    FdoFeatureSchema* connSchema = mSdfConnection->GetSchema();

    // No tables to delete if no current schema.
    if ( connSchema ) {
        FdoClassesP connClasses = connSchema->GetClasses();

        // Get the new (merged schema) that will replace the current schema.
        FdoFeatureSchemaP schema = schemas->GetItem(connSchema->GetName());
        FdoClassesP classes = schema->GetClasses();

        bool modClassIds = false;

        // For each new class.
        for ( idx = 0; idx < classes->GetCount(); idx++ ) {
            FdoClassDefinitionP classDef = classes->GetItem(idx);

            // Get corresponding current class. Need current class address to find tables.
            FdoClassDefinitionP connClassDef = connClasses->FindItem(classDef->GetName());

            // No current class, nothing to do.
            if ( connClassDef ) {
                DataDb* dataDb = mSdfConnection->GetDataDb( connClassDef );
                SdfRTree* rtree = mSdfConnection->GetRTree( connClassDef );
                KeyDb* keyDb = mSdfConnection->GetKeyDb( connClassDef );

                FdoClassDefinitionP baseClass = connClassDef->GetBaseClass();

                // If class is marked for delete then set up its tables for delete.
                // Base and Sub classes share tables so delete tables only if this
                // class has no base class.
                if ( classDef->GetElementState() == FdoSchemaElementState_Deleted ) {
                    if ( !baseClass ) {
                        if ( dataDb )
                            m_hDelDataDbs[dataDb] = dataDb;

                        if ( rtree ) 
                            m_hDelRTrees[rtree] = rtree;

                        if ( keyDb ) 
                            m_hDelKeyDbs[keyDb] = keyDb;
                    }

                    // ClassId is class position, so deleting a class effectively
                    // decrements the class ids of subsequent classes. Tables for all 
                    // subsequent classes need their rows updated to the new class ids.
                    modClassIds = true;
                }
                else {
                    if ( modClassIds ) {

                        // Class Id update required.
                        // Make sure everything is flushed before doing anything
                        mSdfConnection->FlushAll( connClassDef, true );

                        // Check if table already listed for reformatting it may be 
                        // referenced by multiple classes since base and sub-classes
                        // share the same table.
                        TableReformatterP reformatter = mTableReformatters->FindItem( dataDb->GetDbName() );

                        if ( !reformatter )  {
                            // Not on reformatting list, so add it.
                            reformatter = new TableReformatter(
                                dataDb->GetDbName(),
                                mSdfConnection,
                                dataDb,
                                keyDb,
                                rtree,
                                schema
                            );

                            mTableReformatters->Add( reformatter );
                        }

                        // Tell reformatter to update class id references.
                        reformatter->SetModClassid(true);
                    }
               
					// Check if new properties were added
					FdoInt32 i;
					FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
					for(i=0; i<properties->GetCount(); i++ )
					{
						FdoPtr<FdoPropertyDefinition> prop = properties->GetItem( i );
						if( prop->GetElementState() == FdoSchemaElementState_Deleted ) 
							prop = properties->GetItem( i );
						if( prop->GetElementState() == FdoSchemaElementState_Added ) 
						{

							// Class Id update required.
							// Make sure everything is flushed before doing anything
							mSdfConnection->FlushAll( connClassDef, true );

							// Check if table already listed for reformatting it may be 
							// referenced by multiple classes since base and sub-classes
							// share the same table.
							TableReformatterP reformatter = mTableReformatters->FindItem( dataDb->GetDbName() );

							if ( !reformatter )  {
								// Not on reformatting list, so add it.
								reformatter = new TableReformatter(
									dataDb->GetDbName(),
									mSdfConnection,
									dataDb,
									keyDb,
									rtree,
									schema
								);

								mTableReformatters->Add( reformatter );
							}

							reformatter->SetAddedProperties( true );
						}
					}
				}
            }
        }
    }

}

void SdfSchemaMergeContext::PostAcceptChanges()
{
    try {
        // reformat all tables requiring blob updates
        ReformatTables();
    }
    catch ( ... ) {
        try {
            // Something went wrong. Undo the reformatting
            // that was done sofar. Schema modifications
            // will not go ahead so tables need to 
            // be consistent with old schemas.
            ReformatTables(true);
        }
        catch ( ... ) {
        }
        throw;
    }
}

void SdfSchemaMergeContext::PostUpdatePhysical()
{
    stdext::hash_map<void*, void*>::iterator dblist;
    
    // Drop each data, rtree and key table gathered in the PreUpdatePhysical step. 

    for (dblist = m_hDelDataDbs.begin(); dblist != m_hDelDataDbs.end(); dblist++)
    {
        ((DataDb*)dblist->second)->Drop();
    }

    for (dblist = m_hDelRTrees.begin(); dblist != m_hDelRTrees.end(); dblist++)
    {
        ((SdfRTree*)dblist->second)->Drop();
    }

    for (dblist = m_hDelKeyDbs.begin(); dblist != m_hDelKeyDbs.end(); dblist++)
    {
        ((KeyDb*)dblist->second)->Drop();
    }
}

void SdfSchemaMergeContext::RollbackPhysical()
{
    // roll back table blob updates
    ReformatTables(true);
}

void SdfSchemaMergeContext::ReformatTables( bool rollback )
{
    FdoInt32 idx;

    for ( idx = 0; idx < mTableReformatters->GetCount(); idx++ ) {
        try { 
            TableReformatterP reformatter = mTableReformatters->GetItem(idx);
            if ( !rollback ) 
                // Reformat each table
                reformatter->Reformat( );
            else
                // Rollback the reformatting
                reformatter->Rollback( );
        }
        catch ( ... ) {
            if ( !rollback ) 
                throw;
        }
    }
}

void SdfSchemaMergeContext::DeleteSchema( FdoFeatureSchema* schema )
{
    FdoClassesP classes = schema->GetClasses();
    FdoFeatureSchema* connSchema = mSdfConnection->GetSchema();

    // No tables to delete if no current schema.
    if ( connSchema ) {
        FdoClassesP connClasses = connSchema->GetClasses();

        // For each new class.
        for ( int idx = 0; idx < classes->GetCount(); idx++ ) 
        {
            FdoClassDefinitionP classDef = classes->GetItem(idx);

            // Get corresponding current class. Need current class address to find tables.
            FdoClassDefinitionP connClassDef = connClasses->FindItem(classDef->GetName());
            if ( connClassDef ) 
            {
                DataDb* dataDb = mSdfConnection->GetDataDb( connClassDef );
                SdfRTree* rtree = mSdfConnection->GetRTree( connClassDef );
                KeyDb* keyDb = mSdfConnection->GetKeyDb( connClassDef );
                if( dataDb )
                    dataDb->Drop();
                if( rtree )
                    rtree->Drop();
                if( keyDb )
                    keyDb->Drop();
            }
        }
    }
}