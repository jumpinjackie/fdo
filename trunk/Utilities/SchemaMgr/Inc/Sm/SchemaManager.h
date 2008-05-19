#ifndef FDOSMSCHEMAMANAGER_H
#define FDOSMSCHEMAMANAGER_H		1
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
/*****************************************************************************
* DESCRIPTION
* 
*	Gemeric Schema Manager for the FDO Providers. Provides the following
*	functions:
*		- Update physical schema for current datastore from FDO Feature schemas.
*		- Translate physical schema into an FDP Feature Schema.
*		- Describe the mappings between the Feature and Physical Schemas.
*
*/

#ifdef _WIN32
#pragma once
#endif
#include <FdoCommonThreadMutex.h>
#include <Sm/Disposable.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Lp/SpatialContextMgr.h>

class FdoSchemaManager : public FdoSmDisposable
{
public:

	/*****************************************************************************
	* DESCRIPTION:
	*	Gets the current Logical to Physical Mappings 
	*
	* FUNCTION VALUE:
	*	Collection of all the schemas in the current connection. The collection 
	*	contains the LogicalPhysical versions of these schemas.
	*****************************************************************************/
	
	const FdoSmLpSchemaCollection* RefLogicalPhysicalSchemas(void) const;

    // Gets the current LogicalPhysical Spatial Context Manager
    FdoSmLpSpatialContextMgrP GetLpSpatialContextMgr();

    // Gets all LogicalPhysical Spatial Contexts
    virtual FdoSmLpSpatialContextsP GetLpSpatialContexts();


	/*****************************************************************************
	* DESCRIPTION:
	*	Gets the current Physical Schema Manager
	*
	* FUNCTION VALUE:
	*	The Physical Schema Manager.
	*****************************************************************************/
	
	FdoSmPhMgrP GetPhysicalSchema();


	
    // Converts this Schema Manager's logical-physical schemas into a collection
    // of FDO (logical) schemas.
    FdoFeatureSchemasP GetFdoSchemas( 
        FdoStringP schemaName
            // if specified, convert and return only the specified schema plus 
            // any it depends on. If "" then convert all schemas.
    );
    
	/*****************************************************************************
	* DESCRIPTION:
	*	Gets the Schema Mappings for the feature schemas on this connection.
	*
	* FUNCTION VALUE:
	*	The Physical Schema Manager.
	*****************************************************************************/

    FdoSchemaMappingsP GetSchemaMappings( 
        FdoStringP schemaName, 
    /// If specified, return only the mappings for this schema.
    /// If L"" then return mappings for all schemas except the 
    /// MetaClass schema
        bool bIncludeDefaults 
    /// true: include all schema mappings.
    /// false: include only non-default mappings.
    );

    // Given a Spatial Context id, return the spatial context.
    // Returns NULL if the spatial contexts does not exist.
    FdoSmLpSpatialContextP FindSpatialContext( FdoInt64 scId );

    /// Apply updates to the given schema to the FDO DataStore.
    //
    /// Parameters
    ///      pFeatSchema: the Feature schema.
    ///      pOverrides: Fdo to physical mapping overrides for the feature schema.
	virtual void ApplySchema( 
        FdoFeatureSchemaP pFeatSchema, 
        FdoPhysicalSchemaMappingP pOverrides,
        bool bIgnoreStates = false
    );

    virtual void CreateSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
		bool   update
    );

    /// Synchronize the physical tables and columns with the MetaSchema.
    /// Parameters:
    ///      schemaName - the feature schema to synchronize. If "" then all feature schemas
    ///          are synchronized.
    ///      bRollbackOnly - 
    ///          true - only synchronize elements that may have been rolled back..
    ///          false - synchronize all elements.
    virtual void SynchPhysical( const wchar_t* schemaName = L"", bool bRollbackOnly = true );
	
    /// Delete the named schema from the FDO Database;
	void DestroySchema( const wchar_t* schemaName );

    void DestroySpatialContext( FdoString * spatialContextName );

    /// Clear out the logicalphysical and physical schemas to force 
    /// a retrieve from database, the next time these schemas are accessed.
	void Clear( bool bClearAll = false );

protected:
	FdoSchemaManager();
	virtual ~FdoSchemaManager(void);

    FdoSmLpSchemasP GetLogicalPhysicalSchemas();

    /// Creates a LogicalPhysical Schema collection
    virtual FdoSmLpSchemasP CreateLogicalPhysicalSchemas(FdoSmPhMgrP physMgr, FdoSmLpSpatialContextMgrP scMgr) = 0;
    
    /// Creates a Spatial Context Manager
    virtual FdoSmLpSpatialContextMgrP CreateLpSpatialContextMgr(FdoSmPhMgrP physMgr);
    
    /// Creates a Physical Schema Manager.
    virtual FdoSmPhMgrP CreatePhysicalSchema() = 0;

    /// Add the given schema to the FDO datastore.
	void NewSchema( 
        FdoFeatureSchemaP pFeatSchema, 
        FdoPhysicalSchemaMappingP pOverrides,
        bool bIgnoreStates = false
    );

    /// Remove the given schema from the FDO datastore.
	void DestroySchema( FdoFeatureSchemaP pFeatSchema );

    /// apply changes to the given schema to the FDO datastore.
	void UpdateSchema( 
        FdoFeatureSchemaP pFeatSchema, 
        FdoPhysicalSchemaMappingP pOverrides,
        bool bIgnoreStates = false
    );

    /// Check if a schema update has been performed since the last time
    /// this schema manager's schemas were loaded. If there was an update
    /// then force a refresh by clearing the schemas.
	void SynchRevision();

	FdoSmLpSchemasP mLpSchemas;
	FdoSmPhMgrP		mPhysicalSchema;
	long					mRevision;

    FdoSmLpSpatialContextMgrP mSpatialContextMgr;

	static long				mCurrRevision;
    static FdoCommonThreadMutex mMutex;

};

typedef FdoPtr<FdoSchemaManager> FdoSchemaManagerP;

#endif


