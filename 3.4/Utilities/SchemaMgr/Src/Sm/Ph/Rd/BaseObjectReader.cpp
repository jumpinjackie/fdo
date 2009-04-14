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
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdBaseObjectReader::FdoSmPhRdBaseObjectReader(
    FdoSmPhReaderP reader,
    FdoSmPhOwnerP owner
) :
	FdoSmPhReader(reader),
    mOwner(owner)
{
}

FdoSmPhRdBaseObjectReader::FdoSmPhRdBaseObjectReader(
    FdoSmPhReaderP reader,
    FdoSmPhDbObjectP dbObject
) :
	FdoSmPhReader(reader),
    mDbObject(dbObject)
{
}

FdoSmPhRdBaseObjectReader::~FdoSmPhRdBaseObjectReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdBaseObjectReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"BaseObjectFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"base_name",
        row->CreateColumnDbObject(L"base_name",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"base_owner",
        row->CreateColumnDbObject(L"base_owner",false)
    );

    pField = new FdoSmPhField(
        row, 
        L"base_database",
        row->CreateColumnDbObject(L"base_database",true)
    );

    return( rows);
}


