//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ColumnGeom.h"
#include "SpatialIndex.h"
#include "Rd/ColumnReader.h"
#include <FdoCommonStringUtil.h>
#include "Mgr.h"

FdoSmPhPostGisColumnGeom::FdoSmPhPostGisColumnGeom(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    FdoSmPhScInfoP associatedSCInfo,
    bool isNullable,
    bool hasElevation,
    bool hasMeasure,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
      FdoSmPhColumn(columnName, L"geometry", state, parentObject,
        isNullable, rootColumnName),
      FdoSmPhColumnGeom(associatedSCInfo, hasElevation, hasMeasure)
{
    mScGeomLoaded = false;

    mFdoGeomType = (FdoGeometricType_Point
                     | FdoGeometricType_Curve
                     | FdoGeometricType_Surface);

    mFdoGeometryType = FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Point) | 
                    FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint) | 
                    FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_LineString) | 
                    FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString) | 
                    FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_Polygon) | 
                    FdoCommonGeometryUtil::MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon);

    mSRID = -1;
    if ( associatedSCInfo )
        mSRID = associatedSCInfo->mSrid;

    if ( state == FdoSchemaElementState_Added ) 
    {
        //Create a spatial index if this is a new geometric column
        FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        if ( !currIndex ) {
            CreateSpatialIndex();
        }
    }
}

FdoSmPhPostGisColumnGeom::~FdoSmPhPostGisColumnGeom()
{
    // idle
}

int FdoSmPhPostGisColumnGeom::GetRdbType()
{
    return RDBI_GEOMETRY;
}

FdoInt64 FdoSmPhPostGisColumnGeom::GetSRID()
{
    LoadScGeom();

    return (mSRID == 0) ? -1L : mSRID;
}

FdoStringP FdoSmPhPostGisColumnGeom::GetBestFdoType()
{
    return FdoStringP::Format(L"%d", mFdoGeomType);
}

FdoStringP FdoSmPhPostGisColumnGeom::GetBestFdoGeometryType()
{
    LoadScGeom();

    return FdoStringP::Format(L"%d", mFdoGeometryType);
}

FdoInt32 FdoSmPhPostGisColumnGeom::GetGeometryType()
{
    return mFdoGeometryType;
}

FdoStringP FdoSmPhPostGisColumnGeom::GetAddSql()
{
    FdoSmPhMgrP mgr = GetManager();
    FdoStringP sqlString;

    if ( GetParent()->GetElementState() != FdoSchemaElementState_Added ) {
        FdoSmPhColumnP baseColumn = GetBaseColumn();

        if ( !baseColumn || (baseColumn->GetElementState() == FdoSchemaElementState_Deleted) ) {
            FdoStringP objName = GetParent()->GetName();
            FdoStringP schemaName = "public";
            FdoStringP tableName = objName;
            if( objName.Contains(L".") )
            {
                schemaName = objName.Left(L".");
                tableName = objName.Right(L".");
            }
            sqlString = FdoStringP::Format(
                L"select AddGeometryColumn( %ls, %ls, %ls, %ls, 'GEOMETRY', %d)",
                (FdoString*) mgr->FormatSQLVal((FdoString*)schemaName, FdoSmPhColType_String),
                (FdoString*) mgr->FormatSQLVal((FdoString*)tableName, FdoSmPhColType_String),
                (FdoString*) mgr->FormatSQLVal(GetName(), FdoSmPhColType_String),
                (FdoString*) FdoCommonStringUtil::Int64ToString(GetSRID()),
                GetHasElevation() ? 3 : 2
            );
        }
    }

    return sqlString;
}

FdoSmPhSpatialIndexP FdoSmPhPostGisColumnGeom::CreateSpatialIndex( FdoStringP indexName)
{

    // Error if column already indexed
    FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

    if ( currIndex && (currIndex->GetElementState() != FdoSchemaElementState_Deleted) ) 
        throw FdoSchemaException::Create( 
			NlsMsgGet1(
				FDORDBMS_131,
				"Cannot add spatial index to column %1$ls, it already has one.", 
                (FdoString*) GetQName()
            )
		);

    FdoSmPhTableP table = GetContainingDbObject().p->SmartCast<FdoSmPhTable>();

    if ( table ) {
        FdoSmPhPostGisSpatialIndexP newIndex = new FdoSmPhPostGisSpatialIndex(
            (indexName == L"") ? UniqueIndexName() : indexName,
            table,
            FdoSchemaElementState_Added
        );

        currIndex = newIndex->SmartCast<FdoSmPhSpatialIndex>();

        SetSpatialIndex( currIndex );
    }

    return currIndex;
}

void FdoSmPhPostGisColumnGeom::RegenSpatialIndex()
{
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        // If spatial index exists, delete and re-create it.
        if ( currIndex && (currIndex->GetElementState() != FdoSchemaElementState_Deleted) ) {
            currIndex->SetElementState( FdoSchemaElementState_Deleted );

             CreateSpatialIndex();
        }
    }
}

bool FdoSmPhPostGisColumnGeom::Add()
{
    if ( dynamic_cast<FdoSmPhTable*>((FdoSmSchemaElement*) GetParent()) ) {
        FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());

        GdbiConnection* gdbiConn = NULL;
        gdbiConn = mgr->GetGdbiConnection();

        FdoStringP sqlStmt = GetAddSql();

        if ( sqlStmt != L"" ) {
            gdbiConn->ExecuteNonQuery(
                static_cast<const char*>(sqlStmt), true);
        }
    }

    return true;
}

void FdoSmPhPostGisColumnGeom::PostFinalize()
{
    SetHasElevation(false);
    SetHasMeasure(false);
}

void FdoSmPhPostGisColumnGeom::LoadScGeom()
{
    // If new column, there will be no associated scGeom
    if ( (!mScGeomLoaded) && (GetElementState() != FdoSchemaElementState_Added) ) {
        // Check scGeom only once
        mScGeomLoaded = true;

        FdoSmPhOwner* owner = (FdoSmPhOwner*)(this->GetParent()->GetParent());

        // Get associated scGeom
        FdoSmPhSpatialContextGeomP scGeom = owner->FindSpatialContextGeom( GetParent()->GetName(), GetName() );
        if ( scGeom ) {
            // Get srid and allowed geometry types from scGeom 
            if ( mSRID == -1 ) 
                mSRID = scGeom->GetSpatialContext()->GetSrid();
            mFdoGeometryType = scGeom->GetGeometryType();
        }
        else {
            // No scGeom, check if this is an inherited column
            FdoSmPhPostGisColumnGeomP baseColumn = GetBaseColumn()->SmartCast<FdoSmPhPostGisColumnGeom>();
            if ( baseColumn ) {
                // Column is inherited, get SRID and allowed geometry types 
                // from base column.
                if ( mSRID == -1 ) 
                    mSRID = baseColumn->GetSRID();
                mFdoGeometryType = baseColumn->GetGeometryType();
            }
        }
    }
}
