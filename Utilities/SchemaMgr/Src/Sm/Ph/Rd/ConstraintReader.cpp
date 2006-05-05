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
#include <Sm/Ph/Rd/ConstraintReader.h>

FdoSmPhRdConstraintReader::FdoSmPhRdConstraintReader(
    FdoSmPhReaderP reader
) :
	FdoSmPhReader(reader)
{
}


FdoSmPhRdConstraintReader::~FdoSmPhRdConstraintReader(void)
{
}

FdoSmPhRowsP FdoSmPhRdConstraintReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"ConstraintColumns", (FdoSmPhDbObject*) NULL  );
    rows->Add(row);
    
    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField(
        row, 
        L"constraint_name",
        row->CreateColumnDbObject(L"constraint_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"table_name",
        row->CreateColumnDbObject(L"table_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"column_name",
        row->CreateColumnDbObject(L"column_name",false)
    );

    // When constraint is a foreign key, the next three fields
    // indicate the referenced primary table column
    field = new FdoSmPhField(
        row, 
        L"r_owner_name",
        row->CreateColumnDbObject(L"r_owner_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"r_table_name",
        row->CreateColumnDbObject(L"r_table_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"r_column_name",
        row->CreateColumnDbObject(L"r_column_name",false)
    );

    return( rows );
}

