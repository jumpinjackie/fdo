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
#include <Sm/Ph/OptionsReader.h>
#include <Sm/Ph/OptionsWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>

FdoSmPhOptionsReader::FdoSmPhOptionsReader( FdoSmPhMgrP mgr, FdoStringP owner) : 
	FdoSmPhReader( MakeReader(mgr, owner) )
{
}

FdoSmPhOptionsReader::~FdoSmPhOptionsReader(void)
{
}

FdoStringP FdoSmPhOptionsReader::GetName()
{
	return(GetString(L"", L"name"));
}

FdoStringP FdoSmPhOptionsReader::GetValue()
{
	return(GetString(L"", L"value"));
}

FdoSmPhReaderP FdoSmPhOptionsReader::MakeReader( FdoSmPhMgrP mgr, FdoStringP owner )
{
    // Create the row collection describing options attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();
    FdoSmPhReaderP pSubReader;

	FdoSmPhRowP row = FdoSmPhOptionsWriter::MakeRow(mgr, owner);
    rows->Add( row );

    if ( row->GetDbObject()->GetExists() ) {
        // F_OPTIONS exists, read from MetaSchema
        pSubReader = mgr->CreateQueryReader( rows, L"" ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // F_OPTIONS does not exist, make an empty reader.
        pSubReader = new FdoSmPhReader( mgr, rows );
    }

    return pSubReader;
}
