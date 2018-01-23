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
#include "FdoRdbmsException.h"
#include "FdoRdbmsSpatialIndexReader.h"


/// <summary>Implementation class for ISpatialIndexReader</summary>
FdoRdbmsSpatialIndexReader::FdoRdbmsSpatialIndexReader(FdoIConnection *connection,
                                                       const int qid,
                                                       const wchar_t *scName):
    mConnection( NULL ),
    mQid (qid),
    mIndexType(SpatialIndexType_RTree),
    mDimType(SpatialIndexDimension_Default),
    mSpatialIndexFound(false),
    mFirstRead(true),
    mGeomColIndex(0),
    mEof(false)
{
    wcscpy( mSCName, scName );
    mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(connection);
    if( mFdoConnection )
    {
        mFdoConnection->AddRef();
        mConnection = mFdoConnection->GetDbiConnection();
    }
}

FdoRdbmsSpatialIndexReader::~FdoRdbmsSpatialIndexReader()
{
    if( mFdoConnection )
        mFdoConnection->Release();
}

/// <summary>Gets the name of the spatial index currently being read.</summary>
/// <returns>Returns the name of the spatial context.</returns>
const wchar_t* FdoRdbmsSpatialIndexReader::GetName()
{
    return (wchar_t*)mName;
}

/// <summary>Gets the name of the current spatial context as a String.</summary>
/// <returns>Returns the description of the spatial context </returns>
const wchar_t* FdoRdbmsSpatialIndexReader::GetSpatialContextName()
{
    return mSCName;
}

/// <summary>Gets the type of the current spatial Index, either Quad-Tree or R-Tree.</summary>
/// <returns>Returns the spatial index type</returns>
SpatialIndexType FdoRdbmsSpatialIndexReader::GetSpatialIndexType()
{
    return mIndexType;
}

/// <summary>Gets the number of dimensions of the current spatial Index.</summary>
/// <returns>Returns the extent type</returns>
SpatialIndexDimensionType FdoRdbmsSpatialIndexReader::GetNumDimensions()
{
    return mDimType;
}

/// <summary>Advances the reader to the next item. The default position of the reader
/// is prior to the first item. Therefore you must call ReadNext to begin
/// accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns>
bool FdoRdbmsSpatialIndexReader::ReadNextSI()
{
#pragma message ("ToDo: FdoRdbmsSpatialIndexReader::ReadNextSI")
throw "TODO";
#if 0
    try
    {
        int     ccode;
        char    ora_type[33];
        char    ora_name[33];
        double  ora_dims; // NUMBER
        int     num_rows;
        int     is_ws_enabled;
        bool    active_sc_only = ( wcscmp(mSCName, L"") != 0 );

        // Fast exit if no more geometry table.column entries
        if ( mEof )
            return false;

        if ( mFirstRead )
        {
            strcpy(mDbName, mConnection->GetUtility()->UnicodeToUtf8(mConnection->GetDbSchemaName()));

            mConnection->dbi_bind(mQid, "1", RDBI_STRING, DBI_DB_NAME_SIZE, mDbName, NULL, NULL);

            // Activate for fetching all the geometry table.columns or only those associated with the active SC.
            mSpatialContextId = active_sc_only ? -1 : -2;

            if ( active_sc_only )
            {
                mConnection->dbi_bind(mQid, "2", RDBI_STRING, DBI_TABLE_NAME_SIZE, mDbiPlanGeom.geom_table_name, NULL, NULL);
                mConnection->dbi_bind(mQid, "3", RDBI_STRING, DBI_COLUMN_NAME_SIZE,  mDbiPlanGeom.geom_column_name, NULL, NULL);
            }
        }

        // Expect multiple table.column entries for the an active SC. Therefore rebind.
        if ( active_sc_only )
        {
            mConnection->dbi_plan_geom_fetch2( mSpatialContextId, &mGeomColIndex, &mDbiPlanGeom, &mEof);

            if ( !mEof )
            {
                mConnection->dbi_version_ws_is_enabled (mDbName,
                                                        mDbiPlanGeom.geom_table_name,
                                                        &is_ws_enabled);
                if ( is_ws_enabled )
                    strcat( mDbiPlanGeom.geom_table_name, ADB_LT_SUFFIX );
            }
        }

        // Run the query for these bound values
        if ( !mEof && ( mFirstRead || active_sc_only ) )
        {
            mConnection->dbi_execute(mQid, &num_rows, &ccode);
        }

        mFirstRead = false;
        mSpatialIndexFound = false;

        if ( !mEof )
        {
            mConnection->dbi_fetch (mQid, &ccode);

            mSpatialIndexFound = (ccode != DBI_END_OF_FETCH);

            if ( mSpatialIndexFound )
            {
                mConnection->dbi_get_val_a(mQid, "SDO_INDEX_NAME",  sizeof(ora_name), ora_name, NULL, NULL );
                mConnection->dbi_get_val_a(mQid, "SDO_INDEX_TYPE",  sizeof(ora_type), ora_type, NULL, NULL );

                if ( strcmp(ora_type, "RTREE") == 0 )
                    mIndexType = SpatialIndexType_RTree;
                else if ( strcmp(ora_type, "QTREE") == 0 )
                    mIndexType =  SpatialIndexType_QuadTree;
                else {
                    throw FdoRdbmsException::Create(
                                                FdoStringP::Format( L"fetched unknown spatial index type %ls",
                                                ora_type) );
                }

                mConnection->dbi_get_val_b(mQid, "SDO_INDEX_DIMS",  sizeof(double), (char *)&ora_dims, NULL, NULL );

                if ( ora_dims == 2 )
                    mDimType = SpatialIndexDimension_2D;
                else if ( ora_dims == 3 )
                    mDimType = SpatialIndexDimension_3D;
                else {
                    throw FdoRdbmsException::Create(
                                                FdoStringP::Format( L"fetched unknown spatial index dimension type %ld",
                                                ora_dims) );
                }

                const wchar_t *wdbname_tmp = mConnection->GetUtility()->Utf8ToUnicode(ora_name);
                wcscpy( mName, wdbname_tmp );
            }
        }
    }
    catch (FdoRdbmsException *ex)
    {
         throw ex;
    }

    return mSpatialIndexFound;
#endif
}

////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsSpatialIndexReader::ReadNext()
{
    bool    active_sc_only = ( wcscmp(mSCName, L"") != 0 );

    if ( active_sc_only )
    {
        // Skip geometric properties with no spatial index
        while( !mEof  )
        {
            ReadNextSI();
            if ( mSpatialIndexFound )
                break;
        }
    }
    else
    {
        ReadNextSI();
    }

    return mSpatialIndexFound;
}

/// <summary>Closes the ISpatialIndexReader object freeing any resources it may
/// be holding.</summary>
/// <returns>Returns nothing</returns>
void FdoRdbmsSpatialIndexReader::Dispose()
{
    if ( mQid != -1 )
        mConnection->dbi_free( mQid, NULL );
#pragma message ("ToDo: FdoRdbmsSpatialIndexReader::Dispose")
throw "TODO";
 //   if ( !mEof )
 //       mConnection->dbi_plan_geom_deact();

    delete this;
}

