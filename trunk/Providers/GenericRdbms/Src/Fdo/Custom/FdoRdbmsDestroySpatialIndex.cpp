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
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsDestroySpatialIndex.h"

#define DBI_INDEX_NAME_TYPE	3
#define DBI_INDEX_NAME_SIZE 32
//#include "SchemaMgr/LogicalPhysical/ClassDefinition.h"
//#include "SchemaMgr/LogicalPhysical/ObjectPropertyDefinition.h"
//#include "SchemaMgr/LogicalPhysical/GeometricPropertyDefinition.h"


FdoRdbmsDestroySpatialIndex::FdoRdbmsDestroySpatialIndex(FdoIConnection *connection)
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();

    mSIName = NULL;
    mGeometricProperty = NULL;
}

FdoRdbmsDestroySpatialIndex::~FdoRdbmsDestroySpatialIndex()
{
    if( mSIName != NULL )
    {
        delete[] mSIName;
        mSIName = NULL;
    }
    FDO_SAFE_RELEASE( mGeometricProperty );
    mGeometricProperty = NULL;
}

/// <summary>Gets the name of the spatial index to destroy as a String.</summary>
/// <returns>Returns the name of the spatial index to be drop</returns>
const wchar_t* FdoRdbmsDestroySpatialIndex::GetName()
{
    return mSIName;
}

/// <summary>Sets the name of the spatial index to drop as a String.</summary>
/// <param name="value">Input the name of the spatial index to be drop</param>
/// <returns>Returns nothing</returns>
void FdoRdbmsDestroySpatialIndex::SetName(const wchar_t* value)
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

void FdoRdbmsDestroySpatialIndex::SetGeometricProperty(FdoGeometricPropertyDefinition *value)
{
    FDO_SAFE_RELEASE( mGeometricProperty );
    mGeometricProperty = FDO_SAFE_ADDREF( value );
}

FdoGeometricPropertyDefinition* FdoRdbmsDestroySpatialIndex::GetGeometricProperty()
{
    return FDO_SAFE_ADDREF( mGeometricProperty );
}

/// <summary>Executes the DestroySpatialIndex command dropping the specified
/// spatial index. An exception is thrown if the spatial index does not exist.</summary>
/// <returns>Returns nothing</returns>
void FdoRdbmsDestroySpatialIndex::Execute()
{
    char    db_name[GDBI_DB_NAME_SIZE];
    char    si_name[DBI_INDEX_NAME_SIZE];
    char    si_name2[DBI_INDEX_NAME_SIZE];
    char    geom_tbl_name[GDBI_TABLE_NAME_SIZE];
    char    geom_col_name[GDBI_COLUMN_NAME_SIZE];
    char    type;
    int     found;

    try
    {
        strcpy(db_name, mConnection->GetUtility()->UnicodeToUtf8(mConnection->GetDbSchemaName()));

        // The user is supposed to provide either a SI name or a geometry property object
        if ( mSIName == NULL && mGeometricProperty == NULL )
        {
            throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_103, "Invalid parameter") );
        }
        else if ( mSIName != NULL && mGeometricProperty == NULL )
        {
            // Truncate the name if necessary
            memset(si_name, '\0', DBI_INDEX_NAME_SIZE);
            strncpy(si_name, mConnection->GetUtility()->UnicodeToUtf8(mSIName), DBI_INDEX_NAME_SIZE-1);

            // Make sure the name exists
            mConnection->dbi_object_find((char*)(const char *)mConnection->GetDbSchemaName(), si_name, (char *) NULL, &found, &type);

            if ( !found )
                throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_324,
                                                "Spatial Index '%1$ls' not found", mSIName ) )
                                                );

            // Execute drop index
            bool found = findSpatialIndexTableName( db_name, si_name, geom_tbl_name );
            if ( found )
            {
                dropSpatialIndex( geom_tbl_name, si_name );
            }
        }
        else if ( mSIName == NULL && mGeometricProperty != NULL )
        {
            // Geometric property passed in. Find the SI name
            getPhysicalTableColumn( geom_tbl_name, geom_col_name );

            bool found = findSpatialIndexName( db_name, geom_tbl_name, geom_col_name, si_name );

            if ( found )
            {
                dropSpatialIndex( geom_tbl_name, si_name );
            }
        }
        else // ( mSIName != NULL && mGeometricProperty != NULL )
        {
            // Truncate the name if necessary
            memset(si_name, '\0', DBI_INDEX_NAME_SIZE);
            strncpy(si_name, mConnection->GetUtility()->UnicodeToUtf8(mSIName), DBI_INDEX_NAME_SIZE-1);

            // Make sure the name exists
            mConnection->dbi_object_find((char*)(const char *)mConnection->GetDbSchemaName(), si_name, (char *) NULL, &found, &type);

            if ( !found )
                throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_324,
                                                "Spatial Index '%1$ls' not found", mSIName ) )
                                                );

            // Check if the SI passed in matches the one associated with this geom. property.

            (void) getPhysicalTableColumn( geom_tbl_name, geom_col_name );

            bool found2 = findSpatialIndexName( db_name, geom_tbl_name, geom_col_name, si_name2 );

            if ( !found2 )
                throw FdoRdbmsException::Create( NlsMsgGet(FDORDBMS_326,
                                                 "The geometry property specified has no Spatial Index")
                                               );
#ifdef _WIN32
            if ( _stricmp( si_name, si_name2 ) != 0 )
#else
            if ( strcasecmp( si_name, si_name2 ) != 0)
#endif
                throw FdoRdbmsException::Create( FdoStringP( NlsMsgGet1( FDORDBMS_327,
                                                "Spatial Index '%1$ls' unrelated to the specified geometry property", mSIName ) )
                                               );
            // Execute drop index
            dropSpatialIndex( geom_tbl_name, si_name );
        }
    }
    catch (FdoRdbmsException *ex)
    {
        throw ex;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsDestroySpatialIndex::getPhysicalTableColumn( char *tbl_name, char *col_name)
{
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

    const wchar_t* tableName = geomProp->GetContainingDbObjectName();
    const wchar_t* columnName = geomProp->GetColumnName();

    strcpy( tbl_name, mConnection->GetUtility()->UnicodeToUtf8(tableName) );
    strcpy( col_name, mConnection->GetUtility()->UnicodeToUtf8(columnName) );
}

///////////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsDestroySpatialIndex::dropSpatialIndex( char *table_name, char * si_name )
{

//    mConnection->dbi_index_drop_sptl( table_name, NULL, si_name );

}

///////////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsDestroySpatialIndex::findSpatialIndexName( char *db_name, char *tbl_name, char *col_name, char *si_name )
{
#pragma message ("ToDo: FdoRdbmsDestroySpatialIndex::findSpatialIndexName")
throw "TODO";
#if 0
    int     qid = -1;
    int     num_rows, ccode;
    char    sql[ADB_SQL_LEN];
    char    si_name2[DBI_INDEX_NAME_SIZE];
    char    tbl_name2[DBI_TABLE_NAME_SIZE];
    int     is_ws_enabled;

    bool    found = false;

    sprintf( sql,
        "SELECT /*+ all_rows ordered push_subq */ SDO_INDEX_NAME        \n\
        from ALL_SDO_INDEX_METADATA                                     \n\
        where SDO_INDEX_OWNER is not NULL and SDO_INDEX_OWNER=upper(:1) \n\
        and all_sdo_index_metadata.sdo_index_name in                    \n\
        (select INDEX_NAME from ALL_SDO_INDEX_INFO where                \n\
            sdo_index_owner=all_sdo_index_metadata.sdo_index_owner      \n\
            and TABLE_NAME = :2 and COLUMN_NAME = UPPER(:3))");

    // OWM enabled tables have _LT suffix.
    strcpy( tbl_name2, tbl_name );
    mConnection->dbi_version_ws_is_enabled (db_name,
                                            tbl_name2,
                                            &is_ws_enabled);
    if ( is_ws_enabled )
        strcat( tbl_name2, ADB_LT_SUFFIX );

    mConnection->dbi_gql( sql, &qid );
    mConnection->dbi_bind( qid, "1", RDBI_STRING, DBI_DB_NAME_SIZE, db_name, NULL, NULL);
    mConnection->dbi_bind( qid, "2", RDBI_STRING, DBI_TABLE_NAME_SIZE, tbl_name2, NULL, NULL);
    mConnection->dbi_bind( qid, "3", RDBI_STRING, DBI_COLUMN_NAME_SIZE, col_name, NULL, NULL);
    mConnection->dbi_execute( qid, &num_rows, &ccode );
    mConnection->dbi_fetch( qid, &ccode );

    // It is fine if there is no spatial index to drop
    if ( ccode != DBI_END_OF_FETCH )
    {
        mConnection->dbi_get_val_a(qid, "SDO_INDEX_NAME",  sizeof(si_name2), si_name2, NULL, NULL );
        strcpy( si_name, si_name2 );
        found = true;
    }
    mConnection->dbi_free( qid, NULL );

    return found;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsDestroySpatialIndex::findSpatialIndexTableName( char *db_name, char *si_name, char *table_name )
{
    int     qid = -1;
//    char    column_name[GDBI_COLUMN_NAME_SIZE];
    bool    found = false;
//    int                         outType;
//    int                      start;
//    int                       eof;
/*
    mConnection->dbi_index_act(si_name, DBI_INDEX_NAME_TYPE);
    mConnection->dbi_index_get(si_name, table_name, column_name, &outType, &start, &eof );
    if (eof == 0)
        found = true;

    mConnection->dbi_index_deac();
*/
    return found;
}

