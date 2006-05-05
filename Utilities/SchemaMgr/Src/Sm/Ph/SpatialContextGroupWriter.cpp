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
#include <Sm/Ph/SpatialContextGroupWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/CommandWriter.h>


FdoSmPhSpatialContextGroupWriter::FdoSmPhSpatialContextGroupWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhSpatialContextGroupWriter::~FdoSmPhSpatialContextGroupWriter(void)
{
}

FdoInt64 FdoSmPhSpatialContextGroupWriter::GetId()
{
	return( (FdoInt64) GetLong(L"", L"scgid") );
}

FdoStringP FdoSmPhSpatialContextGroupWriter::GetCrsName()
{
	return(GetString(L"", L"crsname"));
}

FdoStringP FdoSmPhSpatialContextGroupWriter::GetCrsWkt()
{
	return(GetString(L"", L"crswkt"));
}

FdoInt64 FdoSmPhSpatialContextGroupWriter::GetSrid()
{
    // TODO: get FdoInt64 when underlying support exists.
	return( (FdoInt64) GetLong(L"", L"srid") );
}

double FdoSmPhSpatialContextGroupWriter::GetXTolerance()
{
    return( GetDouble(L"", L"xtolerance") );
}

double FdoSmPhSpatialContextGroupWriter::GetZTolerance()
{
    return( GetDouble(L"", L"ztolerance") );
}

double FdoSmPhSpatialContextGroupWriter::GetXMin()
{
    return( GetDouble(L"", L"xmin") );
}

double FdoSmPhSpatialContextGroupWriter::GetYMin()
{
    return( GetDouble(L"", L"ymin") );
}

double FdoSmPhSpatialContextGroupWriter::GetZMin()
{
    return( GetDouble(L"", L"zmin") );
}

double FdoSmPhSpatialContextGroupWriter::GetXMax()
{
    return( GetDouble(L"", L"xmax") );
}

double FdoSmPhSpatialContextGroupWriter::GetYMax()
{
    return( GetDouble(L"", L"ymax") );
}

double FdoSmPhSpatialContextGroupWriter::GetZMax()
{
    return( GetDouble(L"", L"zmax") );
}

FdoStringP FdoSmPhSpatialContextGroupWriter::GetExtentType()
{
	return(GetString(L"", L"extenttype"));
}

void FdoSmPhSpatialContextGroupWriter::SetId( FdoInt64 lValue)
{
    // TODO: pass FdoInt64 when underlying support exists.
	SetLong(L"", L"scgid", (long) lValue);
}

void FdoSmPhSpatialContextGroupWriter::SetCrsName(FdoStringP sValue)
{
	SetString(L"",  L"crsname", sValue );
}

void FdoSmPhSpatialContextGroupWriter::SetCrsWkt(FdoStringP sValue)
{
	SetString(L"",  L"crswkt", sValue );
}

void FdoSmPhSpatialContextGroupWriter::SetSrid(FdoInt64 lValue)
{
    // TODO: pass FdoInt64 when underlying support exists.
	SetLong(L"", L"srid", (long) lValue);
}

void FdoSmPhSpatialContextGroupWriter::SetXTolerance(double dValue)
{
	SetDouble(L"", L"xtolerance", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetZTolerance(double dValue)
{
	SetDouble(L"", L"ztolerance", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetXMin(double dValue)
{
	SetDouble(L"", L"xmin", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetYMin(double dValue)
{
	SetDouble(L"", L"ymin", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetZMin(double dValue)
{
	SetDouble(L"", L"zmin", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetXMax(double dValue)
{
	SetDouble(L"", L"xmax", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetYMax(double dValue)
{
	SetDouble(L"", L"ymax", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetZMax(double dValue)
{
	SetDouble(L"", L"zmax", dValue);
}

void FdoSmPhSpatialContextGroupWriter::SetExtentType(FdoStringP sValue)
{
	SetString(L"",  L"extenttype", sValue );
}

void FdoSmPhSpatialContextGroupWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhSpatialContextGroupWriter::Modify( FdoInt64 scId )
{
    FdoSmPhWriter::Modify(
   		FdoStringP::Format( 
#ifdef _WIN32
        L"where scgid = %I64d",
#else
        L"where scgid = %lld",
#endif
        scId )
	);
}

void FdoSmPhSpatialContextGroupWriter::Delete( FdoInt64 scId )
{
    if ( scId >= 0 ) {
        FdoSmPhWriter::Delete( 
            FdoStringP::Format(
#ifdef _WIN32
                L"where scgid = %I64d",
#else
                L"where scgid = %lld",
#endif
                scId
            ) 
        );
	}
}

FdoSmPhRowP FdoSmPhSpatialContextGroupWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = mgr->GetOwner()->GetHasMetaSchema();
    FdoStringP scgDefTable = mgr->GetDcDbObjectName(L"f_spatialcontextgroup");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_spatialcontextgroup", 
        hasMs ? mgr->FindDbObject(scgDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"scgid", (FdoSmPhColumn*) NULL, L"0" );
    field = new FdoSmPhField( row, L"crsname", (FdoSmPhColumn*) NULL, L"" );
    field = new FdoSmPhField( row, L"crswkt", row->CreateColumnChar(L"crswkt", true, 2048 ) );
    field = new FdoSmPhField( row, L"srid", (FdoSmPhColumn*) NULL, L"" );
    field = new FdoSmPhField( row, L"xtolerance", (FdoSmPhColumn*) NULL, L"0.001" );
    field = new FdoSmPhField( row, L"ztolerance", (FdoSmPhColumn*) NULL, L"0.001" );
    field = new FdoSmPhField( row, L"xmin", (FdoSmPhColumn*) NULL, L"-2000000" );
    field = new FdoSmPhField( row, L"ymin", (FdoSmPhColumn*) NULL, L"-2000000" );
    field = new FdoSmPhField( row, L"zmin", (FdoSmPhColumn*) NULL, L"-2000000" );
    field = new FdoSmPhField( row, L"xmax", (FdoSmPhColumn*) NULL, L"2000000" );
    field = new FdoSmPhField( row, L"ymax", (FdoSmPhColumn*) NULL, L"2000000" );
    field = new FdoSmPhField( row, L"zmax", (FdoSmPhColumn*) NULL, L"2000000" );
    field = new FdoSmPhField( row, L"extenttype", (FdoSmPhColumn*) NULL, L"S" );

    return( row );
}

FdoSmPhWriterP FdoSmPhSpatialContextGroupWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
