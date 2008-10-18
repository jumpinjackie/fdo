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
#include <Sm/Error.h>
#include <Sm/Ph/SpatialContextWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/CommandWriter.h>


FdoSmPhSpatialContextWriter::FdoSmPhSpatialContextWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhSpatialContextWriter::~FdoSmPhSpatialContextWriter(void)
{
}

FdoInt64 FdoSmPhSpatialContextWriter::GetId()
{
	return( (FdoInt64) GetLong(L"", L"scid") );
}

FdoStringP FdoSmPhSpatialContextWriter::GetName()
{
	return(GetString(L"", L"scname"));
}

FdoStringP FdoSmPhSpatialContextWriter::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoInt64 FdoSmPhSpatialContextWriter::GetGroupId()
{
	return( (FdoInt64) GetLong(L"", L"scgid") );
}

void FdoSmPhSpatialContextWriter::SetId( FdoInt64 lValue)
{
    // TODO: pass FdoInt64 when underlying support exists.
	SetLong(L"", L"scid", (long) lValue);
}

void FdoSmPhSpatialContextWriter::SetName( FdoStringP sValue )
{
	SetString(L"",  L"scname", sValue );
}

void FdoSmPhSpatialContextWriter::SetDescription( FdoStringP sValue )
{
	SetString(L"",  L"description", sValue );
}

void FdoSmPhSpatialContextWriter::SetGroupId( FdoInt64 lValue )
{
    // TODO: pass FdoInt64 when underlying support exists.
	SetLong(L"", L"scgid", (long) lValue);
}

void FdoSmPhSpatialContextWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhSpatialContextWriter::Modify( FdoInt64 scId )
{
    FdoSmPhWriter::Modify(
   		FdoStringP::Format( 
#ifdef _WIN32
        L"where scid = %I64d",
#else
        L"where scid = %lld",
#endif
        scId )
	);
}

void FdoSmPhSpatialContextWriter::Delete( FdoInt64 scId )
{
    if ( scId >= 0 ) {
        FdoSmPhWriter::Delete( 
            FdoStringP::Format(
#ifdef _WIN32
                L"where scid = %I64d",
#else
                L"where scid = %lld",
#endif
                scId
            ) 
        );
	}
}

FdoSmPhRowP FdoSmPhSpatialContextWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema();
    FdoStringP scDefTable = mgr->GetDcDbObjectName(L"f_spatialcontext");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_spatialcontext", 
        hasMs ? mgr->FindDbObject(scDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"scid" );
    field = new FdoSmPhField( row, L"scname", (FdoSmPhColumn*) NULL, L"Default" );
    field = new FdoSmPhField( row, L"description", (FdoSmPhColumn*) NULL, L"Default Database Spatial Context" );
    field = new FdoSmPhField( row, L"scgid" );

    return( row );
}

FdoSmPhWriterP FdoSmPhSpatialContextWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
