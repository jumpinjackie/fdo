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
#include <Sm/Ph/Rd/ViewRelObjectReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdViewRelationsObjectReader::FdoSmPhRdViewRelationsObjectReader(
    FdoSmPhReaderP reader,
    FdoSmPhOwnerP owner
) :
	FdoSmPhRdReader(reader),
    mOwner(owner)
{
}

FdoSmPhRdViewRelationsObjectReader::~FdoSmPhRdViewRelationsObjectReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdViewRelationsObjectReader::MakeRows( FdoSmPhMgrP mgr)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow(mgr, L"ViewRelObjectFields"); 
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
        row->CreateColumnDbObject(L"base_name",true)
    );

    pField = new FdoSmPhField(
        row, 
        L"column_name",
        row->CreateColumnDbObject(L"column_name",false)
    );

    return( rows);
}

