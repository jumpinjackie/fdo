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
#include <Sm/Ph/Rd/ViewReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdViewReader::FdoSmPhRdViewReader(
    FdoSmPhReaderP reader,
    FdoSmPhOwnerP owner
) :
	FdoSmPhReader(reader),
    mOwner(owner)
{
}

FdoSmPhRdViewReader::FdoSmPhRdViewReader(
    FdoSmPhReaderP reader,
    FdoSmPhDbObjectP dbObject
) :
	FdoSmPhReader(reader),
    mDbObject(dbObject)
{
}

FdoSmPhRdViewReader::~FdoSmPhRdViewReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdViewReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"ViewFields"); 
    rows->Add(row);

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"name",
        row->CreateColumnDbObject(L"name",false)
    );

    // "Select" statement that was used to create the view.
    pField = new FdoSmPhField(
        row, 
        L"sql",
        row->CreateColumnChar(L"sql",false,4000)
    );

    return( rows);
}


