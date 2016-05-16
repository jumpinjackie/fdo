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

#include "Fdo/Connections/IConnection.h"
#include "Fdo/Commands/ICommand.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsCreateSpatialIndex.h"
#include "FdoRdbmsSchemaUtil.h"

/// Constructor
FdoRdbmsCreateSpatialIndex::FdoRdbmsCreateSpatialIndex(FdoIConnection *connection)
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();

    mSIName = NULL;
    mSIType = SpatialIndexType_RTree;
    mSIDimensions = SpatialIndexDimension_2D;
    mSpatialContextName = NULL;
    mGeometricProperty = NULL;
}

FdoRdbmsCreateSpatialIndex::~FdoRdbmsCreateSpatialIndex()
{
    if( mSIName != NULL )
    {
        delete[] mSIName;
        mSIName = NULL;
    }
    if( mSpatialContextName != NULL )
    {
        delete[] mSpatialContextName;
        mSpatialContextName = NULL;
    }
    FDO_SAFE_RELEASE( mGeometricProperty );
    mGeometricProperty = NULL;
}

const wchar_t* FdoRdbmsCreateSpatialIndex::GetName()
{
    return mSIName;
}

void FdoRdbmsCreateSpatialIndex::SetName(const wchar_t* value)
{
    if( mSIName != NULL )
    {
        delete[] mSIName;
        mSIName = NULL;
    }

    if( value != NULL )
    {
        mSIName = new wchar_t[ wcslen( value ) + 1];
        wcscpy( mSIName, value );
    }
}

const wchar_t* FdoRdbmsCreateSpatialIndex::GetSpatialContextName()
{
    return mSpatialContextName;
}

void FdoRdbmsCreateSpatialIndex::SetSpatialContextName(const wchar_t* value)
{
    if( mSpatialContextName != NULL )
    {
        delete[] mSpatialContextName;
        mSpatialContextName = NULL;
    }

    if( value != NULL )
    {
        mSpatialContextName = new wchar_t[ wcslen( value ) + 1];
        wcscpy( mSpatialContextName, value );
    }
}

SpatialIndexType FdoRdbmsCreateSpatialIndex::GetSpatialIndexType()
{
    return mSIType;
}

void FdoRdbmsCreateSpatialIndex::SetSpatialIndexType(SpatialIndexType value)
{
    mSIType = value;
}

SpatialIndexDimensionType FdoRdbmsCreateSpatialIndex::GetNumDimensions()
{
    return mSIDimensions;
}

void FdoRdbmsCreateSpatialIndex::SetNumDimensions(SpatialIndexDimensionType value)
{
    mSIDimensions = value;
}

void FdoRdbmsCreateSpatialIndex::Execute()
{
#pragma message ("ToDo: FdoRdbmsCreateSpatialIndex::Execute")
throw "TODO";
#if 0
    char    db_name[DBI_DB_NAME_SIZE];
    char    plan_name[DBI_PLAN_NAME_SIZE];
    char    dbi_geom_tbl_name[DBI_TABLE_NAME_SIZE];
    char    dbi_geom_col_name[DBI_COLUMN_NAME_SIZE];
    FdoStringP    si_name(mSIName);
    int     eof = false;
    int     found;
    bool    completed = false;
    char    type;

    const   char* fdo_geom_tbl_name = NULL;
    const   char* fdo_geom_col_name = NULL;
    dbi_plan_info_def  plan_info;

    try
    {
        strcpy( db_name, mConnection->GetUtility()->UnicodeToUtf8(mConnection->GetDbSchemaName()));

        // Validate the Spatial Context
        if ( mSpatialContextName != NULL )
            strcpy( plan_name, mConnection->GetUtility()->UnicodeToUtf8(mSpatialContextName) );
        else
            throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_103, "Invalid parameter") );

        // Check if the SC exists
        mConnection->dbi_plan_find( plan_name, &plan_info, &found );

        if ( !found )
            throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_322,
                                            "Spatial Context '%1$ls' not found", mSpatialContextName ) )
                                            );

        // The user is supposed to provide an unique name
        if ( mSIName == NULL )
            throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_103, "Invalid parameter") );

        // Make sure the name is unique in this schema
        mConnection->dbi_object_find((char*)(const char*)mConnection->GetDbSchemaName(),
                                     (char*)(const char*)si_name, (char *)NULL,
                                     &found, &type);

        if ( found )
            throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_328,
                                            "Cannot create Spatial Index '%1$ls', it already exists", mSIName ) )
                                            );

        // Validate the Geometry property
        if ( mGeometricProperty == NULL )
            throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_103, "Invalid parameter") );

        // Get the related table.column based on className.propName
        FdoSchemaElement *parentClass = mGeometricProperty->GetParent();

        if ( parentClass == NULL )
            throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_103, "Invalid parameter") );

        const wchar_t* className = parentClass->GetName();
        const wchar_t* geomPropName = mGeometricProperty->GetName();

        const FdoSmLpSchema *schema = mConnection->GetSchemaUtil()->GetSchema(className);
        const FdoSmLpClassDefinition *classDefinition = schema->RefClasses()->RefItem(className);
        const FdoSmLpPropertyDefinition *propertyDefinition = classDefinition->RefProperties()->RefItem( geomPropName );
        const FdoSmLpGeometricPropertyDefinition* geomProp = static_cast<const FdoSmLpGeometricPropertyDefinition*> (propertyDefinition);

        const wchar_t* fdoTable = geomProp->GetContainingDbObjectName();
        const wchar_t* fdoColumn = geomProp->GetColumnName();

        // Activate for fetching all the table.columns associated with this Spatial context.
        // Try to match by name
        mConnection->dbi_plan_geom_act( plan_info.plan );

        found = false;

        // Get the geometry table.column
        while ( !found )
        {
            mConnection->dbi_plan_geom_fetch( dbi_geom_tbl_name, dbi_geom_col_name, &eof);

            if (eof ) break;

            found = strcmp( mConnection->GetUtility()->UnicodeToUtf8(fdoTable), dbi_geom_tbl_name ) == 0 &&
                    strcmp( mConnection->GetUtility()->UnicodeToUtf8(fdoColumn), dbi_geom_col_name ) == 0;
        }

        // Create the Spatial Index, Create/recompute statistics
        if ( found )
        {
            mConnection->dbi_index_add_sptl( dbi_geom_tbl_name,
                                             dbi_geom_col_name,
                                             (char*)(const char*)si_name,
                                             ( mSIType == SpatialIndexType_RTree ),
                                             ( mSIDimensions == SpatialIndexDimension_3D ) ? 3 : 2 );

            mConnection->dbi_stats_gather( dbi_geom_tbl_name );
        }
        else
        {
            // Let the user know the command failed
            throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_323,
                                            "Spatial Index creation failed. The specified geometry property is not associated to Spatial Context '%1$ls'", mSpatialContextName ) )
                                            );
        }

        // Deactivate geometry table.column retrieval
        mConnection->dbi_plan_geom_deact();
    }
    catch (FdoRdbmsException *ex)
    {
        throw ex;
    }
#endif
}

void FdoRdbmsCreateSpatialIndex::SetGeometricProperty(FdoGeometricPropertyDefinition *value)
{
    FDO_SAFE_RELEASE( mGeometricProperty );
    mGeometricProperty = FDO_SAFE_ADDREF( value );
}

FdoGeometricPropertyDefinition* FdoRdbmsCreateSpatialIndex::GetGeometricProperty()
{
    return FDO_SAFE_ADDREF( mGeometricProperty );
}


