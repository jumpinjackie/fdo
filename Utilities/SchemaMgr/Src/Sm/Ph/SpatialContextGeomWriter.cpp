/*
 * Copyright (C) 2006  Autodesk, Inc.
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
#include <Sm/Error.h>
#include <Sm/Ph/SpatialContextGeomWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/CommandWriter.h>


FdoSmPhSpatialContextGeomWriter::FdoSmPhSpatialContextGeomWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhSpatialContextGeomWriter::~FdoSmPhSpatialContextGeomWriter(void)
{
}

FdoInt64 FdoSmPhSpatialContextGeomWriter::GetScId()
{
	return( (FdoInt64) GetLong(L"", L"scid") );
}

FdoStringP FdoSmPhSpatialContextGeomWriter::GetGeomTableName()
{
	return(GetString(L"", L"geomtablename"));
}

FdoStringP FdoSmPhSpatialContextGeomWriter::GetGeomColumnName()
{
	return(GetString(L"", L"geomcolumnname"));
}

FdoInt32 FdoSmPhSpatialContextGeomWriter::GetDimensionality()
{
    return( GetInteger(L"", L"dimensionality") );
}

void FdoSmPhSpatialContextGeomWriter::SetScId( FdoInt64 lValue)
{
    // TODO: pass FdoInt64 when underlying support exists.
	SetLong(L"", L"scid", (long) lValue);
}

void FdoSmPhSpatialContextGeomWriter::SetGeomTableName( FdoStringP sValue )
{
	SetString(L"",  L"geomtablename", GetManager()->DbObject2MetaSchemaName(sValue) );
}

void FdoSmPhSpatialContextGeomWriter::SetGeomColumnName( FdoStringP sValue )
{
	SetString(L"",  L"geomcolumnname", sValue );
}

void FdoSmPhSpatialContextGeomWriter::SetDimensionality( FdoInt32 iValue )
{
	SetInteger(L"", L"dimensionality", iValue);
}

void FdoSmPhSpatialContextGeomWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhSpatialContextGeomWriter::Modify( FdoStringP geomTableName, FdoStringP geomColumnName )
{
    FdoStringP localGeomTableName = GetManager()->DbObject2MetaSchemaName(geomTableName);

    FdoSmPhWriter::Modify(
   		FdoStringP::Format( 
        L"where geomtablename in ( %ls, %ls ) and geomcolumnname = %ls",
        (FdoString *) GetManager()->FormatSQLVal(geomTableName,FdoSmPhColType_String), 
        (FdoString *) GetManager()->FormatSQLVal(localGeomTableName,FdoSmPhColType_String), 
        (FdoString *) GetManager()->FormatSQLVal(geomColumnName,FdoSmPhColType_String) )
	);
}

void FdoSmPhSpatialContextGeomWriter::Delete( FdoStringP geomTableName, FdoStringP geomColumnName )
{
    FdoStringP localGeomTableName = GetManager()->DbObject2MetaSchemaName(geomTableName);

    FdoSmPhWriter::Delete( 
        FdoStringP::Format(
        L"where geomtablename in ( %ls, %ls ) and geomcolumnname = %ls",
        (FdoString *) GetManager()->FormatSQLVal(geomTableName,FdoSmPhColType_String), 
        (FdoString *) GetManager()->FormatSQLVal(localGeomTableName,FdoSmPhColType_String), 
        (FdoString *) GetManager()->FormatSQLVal(geomColumnName,FdoSmPhColType_String) )
    );
}

FdoSmPhRowP FdoSmPhSpatialContextGeomWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = mgr->GetOwner()->GetHasMetaSchema();
    FdoStringP scgDefTable = mgr->GetDcDbObjectName(L"f_spatialcontextgeom");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_spatialcontextgeom", 
        hasMs ? mgr->FindDbObject(scgDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"scid" );
    field = new FdoSmPhField( row, L"geomtablename" );
    field = new FdoSmPhField( row, L"geomcolumnname" );
    field = new FdoSmPhField( row, L"dimensionality" );

    return( row );
}

FdoSmPhWriterP FdoSmPhSpatialContextGeomWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
