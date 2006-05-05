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
#include "FdoRdbmsSpatialIndexReader.h"
#include "FdoRdbmsGetSpatialIndexes.h"

FdoRdbmsGetSpatialIndexes::FdoRdbmsGetSpatialIndexes() :
    mConnection( NULL ),
    mActiveOnly( true )
{
}

FdoRdbmsGetSpatialIndexes::FdoRdbmsGetSpatialIndexes(FdoIConnection *connection):
FdoRdbmsCommand<FdoIGetSpatialIndexes>(connection)
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
        mConnection = conn->GetDbiConnection();
}

FdoRdbmsGetSpatialIndexes::~FdoRdbmsGetSpatialIndexes()
{
}

const bool FdoRdbmsGetSpatialIndexes::GetActiveOnly()
{
    return mActiveOnly;
}

void FdoRdbmsGetSpatialIndexes::SetActiveOnly(const bool value)
{
    mActiveOnly = value;
}

FdoISpatialIndexReader* FdoRdbmsGetSpatialIndexes::Execute()
{
#pragma message ("ToDo: FdoRdbmsGetSpatialIndexes::Execute")
throw "TODO";
#if 0
    int     qid = -1;
    int     found;
    char    sql[ADB_SQL_LEN];
    char    sql_geom[DBI_TABLE_NAME_SIZE + 50];
    dbi_plan_info_def   plan_info;

    try
    {
        if ( mActiveOnly )
        {
            // Get info on active plan
            mConnection->dbi_plan_get(-1L, &plan_info, &found);
            sprintf( sql_geom, " and TABLE_NAME = :2 and COLUMN_NAME = UPPER(:3)");
        }

        sprintf( sql,
            "SELECT /*+ all_rows ordered push_subq */ SDO_INDEX_NAME, SDO_INDEX_TYPE, SDO_INDEX_DIMS                \n\
            from ALL_SDO_INDEX_METADATA                                           \n\
            where SDO_INDEX_OWNER is not NULL and SDO_INDEX_OWNER=upper(:1)     \n\
            and all_sdo_index_metadata.sdo_index_name in    \n\
            (select INDEX_NAME from ALL_SDO_INDEX_INFO where \n\
                sdo_index_owner=all_sdo_index_metadata.sdo_index_owner %s)",
                mActiveOnly ? sql_geom : "");

        mConnection->dbi_gql( sql, &qid );
        mConnection->dbi_option(qid, "define array size 150", NULL);
    }
    catch (FdoRdbmsException *ex)
    {
        if ( qid != -1 )
            mConnection->dbi_free( qid, NULL );
        throw ex;
    }

    FdoISpatialIndexReader* reader = new FdoRdbmsSpatialIndexReader(FdoPtr<FdoIConnection>(GetConnection()), qid,
                                          mActiveOnly ? mConnection->GetUtility()->Utf8ToUnicode(plan_info.name) : L"");

    return (FdoISpatialIndexReader*)reader;
#endif
}

