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
#include <Sm/Ph/Rd/CoordSysReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdCoordSysReader::FdoSmPhRdCoordSysReader(
    FdoSmPhReaderP reader,
    FdoSmPhOwnerP owner,
    FdoStringP csysName
) :
	FdoSmPhReader(reader),
    mOwner(owner),
    mCsysName(csysName)
{
}

FdoSmPhRdCoordSysReader::FdoSmPhRdCoordSysReader(
    FdoSmPhMgrP         mgr,
    FdoSmPhRowsP        rows,
    FdoSmPhOwnerP       owner,
    FdoStringP          csysName
) :
	FdoSmPhReader(mgr, rows),
    mOwner(owner),
    mCsysName(csysName)
{
}

FdoSmPhRdCoordSysReader::~FdoSmPhRdCoordSysReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdCoordSysReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"CoordSysFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"srid",
        row->CreateColumnInt64(L"srid",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"wktext",
        row->CreateColumnChar(L"wktext",true,3000)
    );

    pField = new FdoSmPhField(
        row, 
        L"bounds",
        row->CreateColumnGeom(L"bounds", (FdoSmPhScInfo*) NULL, true)
    );

    return( rows);
}


