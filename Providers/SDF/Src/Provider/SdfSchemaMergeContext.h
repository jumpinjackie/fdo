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
#ifndef SDF_SCHEMA_MERGECONTEXT_H
#define SDF_SCHEMA_MERGECONTEXT_H

#include "SdfExtHashMap.h"

class SdfConnection;

class SdfSchemaMergeContext : public FdoSchemaMergeContext
{
public:
    /// Constructs a context for merging a schema, passed to the ApplySchema command,
    /// into the current schemas
    static SdfSchemaMergeContext* Create( 
        SdfConnection* connection,              // current connection
        FdoFeatureSchemaCollection* oldSchemas, // current schemas
        FdoFeatureSchema* newSchema,            // applied schema.
        bool ignoreStates                       // see FdoIApplySchema
    );

    // Schema element Modifications supported by the SDF Provider

    virtual bool CanAddClass( FdoClassDefinition* classDef );
    virtual bool CanDeleteClass( FdoClassDefinition* classDef );

    // Performs the schema merge
    //
    void Merge();

    // Handles class data updates that must be done before schema updates
    void PreUpdatePhysical();
    // Handles class data updates that must be done after schema updates
    void PostUpdatePhysical();

protected:
    SdfSchemaMergeContext() {}
    SdfSchemaMergeContext( 
        SdfConnection* connection,
        FdoFeatureSchemaCollection* oldSchemas, 
        FdoFeatureSchema* newSchema, 
        bool ignoreStates  
    );
    virtual ~SdfSchemaMergeContext();

private:
    SdfConnection* mSdfConnection;

    stdext::hash_map<void*, void*> m_hDelRTrees;    
    stdext::hash_map<void*, void*> m_hDelDataDbs;
    stdext::hash_map<void*, void*> m_hDelKeyDbs;
};

/// \brief
/// FdoSchemaMergeContextP is a FdoPtr on FdoSchemaMergeContext, provided for convenience.
typedef FdoPtr<SdfSchemaMergeContext> SdfSchemaMergeContextP;

class SdfISchemaMergeContextFactory 
{
public:
    virtual SdfSchemaMergeContext* CreateMergeContext( 
        FdoFeatureSchemaCollection* oldSchemas, 
        FdoFeatureSchema* newSchema, 
        bool ignoreStates  
    ) = 0;
};

#endif


