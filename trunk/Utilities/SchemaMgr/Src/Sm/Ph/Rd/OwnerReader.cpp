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
#include <Sm/Ph/Rd/OwnerReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdOwnerReader::FdoSmPhRdOwnerReader(
    FdoSmPhReaderP reader,
    FdoSmPhDatabaseP database,
    FdoStringP ownerName
) :
	FdoSmPhReader(reader),
    mDatabase(database),
    mOwnerName(ownerName)
{
}

FdoSmPhRdOwnerReader::FdoSmPhRdOwnerReader(
    FdoSmPhMgrP         mgr,
    FdoSmPhRowsP        rows,
    FdoSmPhDatabaseP    database,
    FdoStringP          ownerName
) :
	FdoSmPhReader(mgr, rows),
    mDatabase(database),
    mOwnerName(ownerName)
{
}

FdoSmPhRdOwnerReader::~FdoSmPhRdOwnerReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdOwnerReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"OwnerFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    // Each field adds itself to the row.
    pField = new FdoSmPhField(
        row, 
        L"schemas_table",
        row->CreateColumnDbObject(L"schemas_table",false)
    );
    return( rows);
}


