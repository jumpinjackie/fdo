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

void SdfSchemaMergeContext::Merge()
{
    CommitSchemas();
}

void SdfSchemaMergeContext::PreUpdatePhysical()
{
    FdoInt32 idx;

// Gather up lists of the Data, Key and Rtree tables that need to be deleted.
// These are deleted by PostUpdatePhysical. 

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

        // For each new class.
        for ( idx = 0; idx < classes->GetCount(); idx++ ) {
            FdoClassDefinitionP classDef = classes->GetItem(idx);

            // Get corresponding current class. Need current class address to find tables.
            FdoClassDefinitionP connClassDef = connClasses->FindItem(classDef->GetName());

            // No current class, nothing to do.
            if ( connClassDef ) {
                FdoClassDefinitionP baseClass = connClassDef->GetBaseClass();

                // If class is marked for delete then set up its tables for delete.
                // Base and Sub classes share tables so delete tables only if this
                // class has no base class.
                if ( (!baseClass) && (classDef->GetElementState() == FdoSchemaElementState_Deleted) ) {
                    DataDb* dataDb = mSdfConnection->GetDataDb( connClassDef );
                    if ( dataDb )
                        m_hDelDataDbs[dataDb] = dataDb;

                    SdfRTree* rtree = mSdfConnection->GetRTree( connClassDef );
                    if ( rtree ) 
                        m_hDelRTrees[rtree] = rtree;

                    KeyDb* keyDb = mSdfConnection->GetKeyDb( connClassDef );
                    if ( keyDb ) 
                        m_hDelKeyDbs[keyDb] = keyDb;
                }
            }
        }
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

