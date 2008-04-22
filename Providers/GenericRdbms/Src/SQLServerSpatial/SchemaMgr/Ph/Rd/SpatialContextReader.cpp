/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <float.h>
#include "SpatialContextReader.h"
#include <Sm/Ph/Table.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "DbObjectBinds.h"
#include "../Mgr.h"
#include "../Database.h"

FdoSmPhRdSqsSpatialContextReader::FdoSmPhRdSqsSpatialContextReader( )
{
}

FdoSmPhRdSqsSpatialContextReader::FdoSmPhRdSqsSpatialContextReader( FdoSmPhOwnerP owner )
{
    FdoStringsP objectNames = FdoStringCollection::Create();
	SetSubReader( MakeQueryReader(owner,objectNames) );
}

FdoSmPhRdSqsSpatialContextReader::FdoSmPhRdSqsSpatialContextReader( FdoSmPhOwnerP owner, FdoStringP dbObjectName )
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObjectName);
	SetSubReader( MakeQueryReader(owner, objectNames) );
}

FdoSmPhRdSqsSpatialContextReader::~FdoSmPhRdSqsSpatialContextReader(void)
{
}

FdoString* FdoSmPhRdSqsSpatialContextReader::GetName()
{
	return L"SC"; // generic
}

FdoString* FdoSmPhRdSqsSpatialContextReader::GetDescription()
{
	return L"Spatial context from SqlServer metadata";
}

FdoStringP FdoSmPhRdSqsSpatialContextReader::GetGeomTableName()
{
	return mGeomTableName;
}

FdoStringP FdoSmPhRdSqsSpatialContextReader::GetGeomColumnName()
{
	return mGeomColumnName;
}

bool FdoSmPhRdSqsSpatialContextReader::GetHasElevation()
{
	return mHasElevation;
}

bool FdoSmPhRdSqsSpatialContextReader::GetHasMeasure()
{
	return mHasMeasure;
}

FdoString* FdoSmPhRdSqsSpatialContextReader::GetCoordinateSystem()
{
	return mCSname;
}

FdoString* FdoSmPhRdSqsSpatialContextReader::GetCoordinateSystemWkt()
{
	return mWKT;
}

FdoInt64 FdoSmPhRdSqsSpatialContextReader::GetSrid()
{
	return mSrid;
}

FdoSpatialContextExtentType FdoSmPhRdSqsSpatialContextReader::GetExtentType()
{
	return FdoSpatialContextExtentType_Static;
}

FdoByteArray* FdoSmPhRdSqsSpatialContextReader::GetExtent()
{
    return FDO_SAFE_ADDREF(mExtents.p);
}

const double FdoSmPhRdSqsSpatialContextReader::GetXYTolerance()
{
	return mTolXY;
}

const double FdoSmPhRdSqsSpatialContextReader::GetZTolerance()
{
	return mTolZ;
}

const bool FdoSmPhRdSqsSpatialContextReader::IsActive()
{
	return false;
}

bool FdoSmPhRdSqsSpatialContextReader::ReadNext()
{
	bool	ret = FdoSmPhReader::ReadNext();

    if ( ret ) {
        // SqlServer can store 3D geometries but provider doesn't support yet.
        mHasElevation = false;
        mHasMeasure = false;
        mTolXY = 0.0;
        mTolZ = 0.0;

        // Build full name from user and table name.
		FdoStringP geomUserName = FdoSmPhReader::GetString(L"", L"geomusername");
		FdoStringP geomTableName = FdoSmPhReader::GetString(L"", L"geomtablename");

        mGeomTableName = geomUserName + L"." + geomTableName;

		mGeomColumnName = FdoSmPhReader::GetString(L"", L"geomcolumnname");

        FdoStringP colType = FdoSmPhReader::GetString(L"", L"type");

        // Determine associated SRID by sampling first geometry for the column.
        FdoInt64 srid = mOwner->SampleColumnSrid( mGeomTableName, mGeomColumnName );

        mSrid = 0;
        mWKT = L"";
        mCSname = L"";

        if ( srid >= 0 ) {
            // We have a SRID, get the coordinate system.
            mSrid = srid;
            FdoSmPhCoordinateSystemP coordSys = mOwner->FindCoordinateSystem(mSrid);
            if ( coordSys ) {
                // Extract name and wkt from coordinate system.
                mCSname = coordSys->GetName();
                mWKT = coordSys->GetWkt();
            }
        }
        else {
            // No SRID, so no WKT if column type is geometry
            // For geography columns, SRID defaults to 4326 (WGS 84).
            if ( colType == L"geography" ) {
                mSrid = 4326;
                FdoSmPhCoordinateSystemP coordSys = mOwner->FindCoordinateSystem(mSrid);
                if ( coordSys ) {
                    mCSname = coordSys->GetName();
                    mWKT = coordSys->GetWkt();
                }
            }
        }

        // Get bounds from column's spatial index
        double xmin = FdoSmPhReader::GetDouble(L"", colType == L"geography" ? L"ymin" : L"xmin");
        double ymin = FdoSmPhReader::GetDouble(L"", colType == L"geography" ? L"xmin" : L"ymin");
        double xmax = FdoSmPhReader::GetDouble(L"", colType == L"geography" ? L"ymax" : L"xmax");
        double ymax = FdoSmPhReader::GetDouble(L"", colType == L"geography" ? L"xmax" : L"ymax");

        if ( _isnan(xmin) || _isnan(ymin) || _isnan(xmax) || _isnan(ymax) ) {
            // Bounds not set so use defaults.
            if ( mSrid > 0 ) {
                // lat/long coordinate system so default to whole world
                xmin = -180;
                ymin = -90;
                xmax = 180;
                ymax = 90;
            }
            else
            {
                // Use Generic defaults for planar system
                xmin = mDefaultXMin;
                ymin = mDefaultYMin;
                xmax = mDefaultXMax;
                ymax = mDefaultYMax;
            }
        }

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	    FdoPtr<FdoIEnvelope>          env = gf->CreateEnvelopeXY( xmin, ymin, xmax, ymax );
	    FdoPtr<FdoIGeometry>		  geom = gf->CreateGeometry(env); 

	    mExtents = gf->GetFgf(geom);
	}

    return(ret);
}

FdoSmPhReaderP FdoSmPhRdSqsSpatialContextReader::MakeQueryReader( FdoSmPhOwnerP owner, FdoStringsP objectNames )
{
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;
    FdoStringP			 ownerName = owner->GetName();
    FdoStringP           databaseName =  owner->GetParent()->GetName();  
/*
	FdoStringP readerName = FdoStringP::Format(
        L"ScGeomReader_%ls", 
        objectNames->GetCount() > 0 ? (objectNames->GetCount() > 1 ? L"MO" : L"SO") : L"NO"
    );
	if ( databaseName.GetLength() > 0 )
		readerName += FdoStringP::Format(L"_%ls", (FdoString*)databaseName );

	FdoSmPhReaderP reader = mgr->GetStaticReader( readerName );
*/

    mOwner = FDO_SAFE_ADDREF(static_cast<FdoSmPhSqsOwner*>(owner.p));

    FdoSmPhReaderP reader;

    if ( !reader ) {
        FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
            mgr,
            L"c.name",
            L"user_name",
            L"a.name",
            L"object_name",
            objectNames
        );


        // Get where clause for owner and object name binds.
        FdoStringP qualification = binds->GetSQL();
		
        // Build statement to retrieve each spatial index in the datastore.
        sqlString = FdoStringP::Format(
                    L"select 0 as srid, \n"
                    L" c.name collate latin1_general_bin as geomusername,\n"
                    L" a.name collate latin1_general_bin as geomtablename,\n"
                    L" b.name as geomcolumnname,\n"
                    L" e.bounding_box_xmin as xmin, \n"
                    L" e.bounding_box_ymin as ymin, \n"
                    L" e.bounding_box_xmax as xmax, \n"
                    L" e.bounding_box_ymax as ymax, \n"
                    L" f.name as type \n"
                    L" from %ls.sys.objects  a\n"
                    L"  INNER JOIN %ls.sys.columns b ON ( a.object_id = b.object_id ) \n"
                    L"  INNER JOIN %ls.sys.schemas c ON ( a.schema_id = c.schema_id ) \n"
                    L"  INNER JOIN %ls.sys.index_columns d ON ( a.object_id = d.object_id and b.column_id = d.column_id ) \n"
                    L"  INNER JOIN %ls.sys.spatial_index_tessellations e ON ( d.object_id = e.object_id and d.index_id = e.index_id ) \n"
                    L"  INNER JOIN %ls.sys.types  f ON ( b.user_type_id = f.user_type_id ) \n"
                    L" %ls %ls\n"
                    L" order by c.name collate latin1_general_bin asc, a.name collate latin1_general_bin asc, b.column_id asc",
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (qualification == L"") ? L"" : L"where",
                (FdoString *)qualification
              );


        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );

//        mgr->SetStaticReader( readerName, reader );
    }
    else {

        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
		pReader->EndSelect();

		FdoSmPhRowP bindRows = pReader->GetBinds();
		if ( bindRows )
			FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
				mgr,
                L"a.TABLE_SCHEMA",
                L"user_name",
                L"a.TABLE_NAME",
                L"object_name",
                objectNames,
				bindRows,
				true
			);

        pReader->Execute();
    }

    return reader;
}

FdoSmPhRowsP FdoSmPhRdSqsSpatialContextReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"CoordSysFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP  pField = new FdoSmPhField(
        row,
        L"srid",
        row->CreateColumnInt64(L"srid",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"geomusername",
        row->CreateColumnChar(L"geomusername",true, 128)
    );

    pField = new FdoSmPhField(
        row, 
        L"geomtablename",
        row->CreateColumnChar(L"geomtablename",true, 128)
    );

    pField = new FdoSmPhField(
        row, 
        L"geomcolumnname",
        row->CreateColumnChar(L"geomcolumnname",true, 32)
    );

    pField = new FdoSmPhField(
        row,
        L"xmin",
        row->CreateColumnDouble(L"xmin",false)
    );

    pField = new FdoSmPhField(
        row,
        L"ymin",
        row->CreateColumnDouble(L"ymin",false)
    );

    pField = new FdoSmPhField(
        row,
        L"xmax",
        row->CreateColumnDouble(L"xmax",false)
    );

    pField = new FdoSmPhField(
        row,
        L"ymax",
        row->CreateColumnDouble(L"ymax",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"type",
        row->CreateColumnChar(L"type",true, 128)
    );

    return( rows);
}



