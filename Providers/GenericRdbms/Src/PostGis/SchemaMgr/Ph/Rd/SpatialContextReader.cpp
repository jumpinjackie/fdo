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
#include "SpatialContextReader.h"
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Database.h"
#include "FdoCommonStringUtil.h"

FdoSmPhRdPostGisSpatialContextReader::FdoSmPhRdPostGisSpatialContextReader( )
{
}

FdoSmPhRdPostGisSpatialContextReader::FdoSmPhRdPostGisSpatialContextReader( FdoSmPhOwnerP owner )
{
    FdoStringsP objectNames = FdoStringCollection::Create();
	SetSubReader( MakeQueryReader(owner, objectNames) );
}

FdoSmPhRdPostGisSpatialContextReader::FdoSmPhRdPostGisSpatialContextReader( FdoSmPhOwnerP owner, FdoStringP dbObjectName )
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObjectName);
	SetSubReader( MakeQueryReader(owner, objectNames) );
}

FdoSmPhRdPostGisSpatialContextReader::~FdoSmPhRdPostGisSpatialContextReader(void)
{
}

FdoString* FdoSmPhRdPostGisSpatialContextReader::GetName()
{
	return L"SC"; // generic
}

FdoString* FdoSmPhRdPostGisSpatialContextReader::GetDescription()
{
	return L"Spatial context from PostGis metadata";
}

FdoStringP FdoSmPhRdPostGisSpatialContextReader::GetGeomTableName()
{
	return mGeomTableName;
}

FdoStringP FdoSmPhRdPostGisSpatialContextReader::GetGeomColumnName()
{
	return mGeomColumnName;
}

bool FdoSmPhRdPostGisSpatialContextReader::GetHasElevation()
{
	return (mDimension > 2);
}

bool FdoSmPhRdPostGisSpatialContextReader::GetHasMeasure()
{
	return (mDimension > 3);
}

FdoInt32 FdoSmPhRdPostGisSpatialContextReader::GetGeometryType()
{
    FdoStringP geomType = FdoSmPhReader::GetString(L"", L"geomtype");

    if ( (geomType.ICompare(L"POINT") == 0) || (geomType.ICompare(L"POINTM") == 0) ) 
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Point);
    else if ( (geomType.ICompare(L"MULTIPOINT") == 0) || (geomType.ICompare(L"MULTIPOINTM") == 0)  )
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint);
    else if ( (geomType.ICompare(L"LINESTRING") == 0) || (geomType.ICompare(L"LINESTRINGM") == 0) )
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_LineString);
    else if ( (geomType.ICompare(L"MULTILINESTRING") == 0) || (geomType.ICompare(L"MULTILINESTRINGM") == 0) )
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString);
    else if ( (geomType.ICompare(L"POLYGON") == 0) || (geomType.ICompare(L"POLYGONM") == 0) )
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Polygon);
    else if ( (geomType.ICompare(L"MULTIPOLYGON") == 0) || (geomType.ICompare(L"MULTIPOLYGONM") == 0) )
        return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon);

    return FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Point) | 
           FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint) | 
           FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_LineString) | 
           FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString) | 
           FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Polygon) | 
           FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon);
}

FdoString* FdoSmPhRdPostGisSpatialContextReader::GetCoordinateSystem()
{
	return mCSname;
}

FdoString* FdoSmPhRdPostGisSpatialContextReader::GetCoordinateSystemWkt()
{
	return mWKT;
}

FdoInt64 FdoSmPhRdPostGisSpatialContextReader::GetSrid()
{
	return mSrid;
}

FdoSpatialContextExtentType FdoSmPhRdPostGisSpatialContextReader::GetExtentType()
{
	return FdoSpatialContextExtentType_Static;
}

FdoByteArray* FdoSmPhRdPostGisSpatialContextReader::GetExtent()
{
    return FDO_SAFE_ADDREF(mExtents.p);
}

const double FdoSmPhRdPostGisSpatialContextReader::GetXYTolerance()
{
	return mTolXY;
}

const double FdoSmPhRdPostGisSpatialContextReader::GetZTolerance()
{
	return mTolZ;
}

const bool FdoSmPhRdPostGisSpatialContextReader::IsActive()
{
	return false;
}

bool FdoSmPhRdPostGisSpatialContextReader::ReadNext()
{
	bool	ret = ret = FdoSmPhReader::ReadNext();

	if ( ret ) 
	{
		// Srid, Table and column info
		mSrid = FdoSmPhReader::GetInt64(L"", L"srid");
		mGeomTableName = FdoSmPhReader::GetString(L"", L"geomtablename");
		mGeomColumnName = FdoSmPhReader::GetString(L"", L"geomcolumnname");
		mWKT = FdoSmPhReader::GetString(L"", L"wktext");
		mDimension = FdoSmPhReader::GetLong(L"", L"dimension");

/* TODO - Decide if always should set csname to srid
        if ( mWKT.Contains(L"[\"") ) {
            mCSname = mWKT.Right( L"[\"" ).Left( L"\"" );
        }
        else {
            mCSname = L"";
        }
*/
        mCSname = FdoCommonStringUtil::Int64ToString(mSrid);

		double xmin = -2000000;
		double xmax = 2000000;
		double ymin = -2000000;
		double ymax = 2000000;

		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIEnvelope>          env = gf->CreateEnvelopeXY( xmin, ymin, xmax, ymax );
		FdoPtr<FdoIGeometry>		  geom = gf->CreateGeometry(env); 

		mExtents = gf->GetFgf(geom);

		// Tolerances
		mTolXY	= 0.0001;
        mTolZ = 0.0001;
	}

    return(ret);
}

FdoSmPhReaderP FdoSmPhRdPostGisSpatialContextReader::MakeQueryReader( FdoSmPhOwnerP owner, FdoStringsP objectNames )
{
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoStringP			 ownerName = owner->GetName();
    FdoStringP           databaseName =  owner->GetParent()->GetName();  

    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();
    
    // Create binds for object names
    FdoSmPhRdSchemaDbObjectBindsP binds = new FdoSmPhRdSchemaDbObjectBinds(
        mgr,
        L"a.f_table_schema",
        L"schema_name",
        L"a.f_table_name",
        L"name",
        objectNames
    );

    // Get where clause for owner and object name binds.
    FdoStringP qualification = binds->GetSQL();

    // Generate sql statement if not already done
    sqlString = FdoStringP::Format(
          L"select b.srid, a.f_table_schema||'.'||a.f_table_name as geomtablename, a.f_geometry_column as geomcolumnname, \n"
		  L" b.srtext as wktext, \n"
		  L" a.coord_dimension as dimension,\n"
		  L" a.type as geomtype\n"
          L" from  geometry_columns a\n"
		  L" left outer join spatial_ref_sys b on a.srid = b.srid\n"
          L" %ls %ls order by %ls, %ls, %ls\n",
          (qualification == L"") ? L"" : L"where",
          (FdoString *) qualification,
          (FdoString*) pgMgr->FormatCollateColumnSql(L"a.f_table_schema"),
          (FdoString*) pgMgr->FormatCollateColumnSql(L"a.f_table_name"),
          (FdoString*) pgMgr->FormatCollateColumnSql(L"a.f_geometry_column")
    );

    

    FdoSmPhRowsP rows = MakeRows( mgr );
    FdoSmPhRowP row = rows->GetItem(0);

    FdoSmPhReaderP reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );

    return reader;
}

FdoSmPhRowsP FdoSmPhRdPostGisSpatialContextReader::MakeRows( FdoSmPhMgrP mgr)
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
        L"geomtablename",
        row->CreateColumnChar(L"geomtablename",true, 32)
    );

    pField = new FdoSmPhField(
        row, 
        L"geomcolumnname",
        row->CreateColumnChar(L"geomcolumnname",true, 32)
    );

    pField = new FdoSmPhField(
        row, 
        L"wktext",
        row->CreateColumnChar(L"wktext",true, 2048)
    );

    pField = new FdoSmPhField(
        row, 
        L"dimension",
        row->CreateColumnInt32(L"dimension",true)  
    );

    pField = new FdoSmPhField(
        row, 
        L"geomtype",
        row->CreateColumnChar(L"geomtype",true, 30)
    );

    return( rows);
}



