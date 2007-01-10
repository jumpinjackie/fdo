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
#include <Sm/Ph/Mt/SchemaReader.h>

FdoSmPhMtSchemaReader::FdoSmPhMtSchemaReader(FdoSmPhRowsP froms, FdoSmPhOwnerP owner, bool dsInfo) : 
	FdoSmPhReader( MakeReader(froms, owner, dsInfo) )
{
}

FdoSmPhMtSchemaReader::~FdoSmPhMtSchemaReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtSchemaReader::MakeReader( FdoSmPhRowsP froms, FdoSmPhOwnerP owner, bool dsInfo )
{
    FdoStringP whereClause;

    if ( dsInfo ) {
        // Generate the where clause for retrieving datastore information
        whereClause = FdoStringP::Format( 
            L"where upper(schemaname) = '%ls'", 
	        (FdoString*) FdoStringP(owner->GetName()).Upper()
        );
    }
    else {
        // Generate the where clause for retrieving feature schemas
        whereClause = FdoStringP::Format( 
            L"where %ls order by schemaname", 
	        owner->GetDbNameClause(false)
        );
    }

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = FdoSmPhMgrP(owner->GetManager())->CreateQueryReader( froms, whereClause );

    return FDO_SAFE_ADDREF((FdoSmPhRdQueryReader*)pSubReader);
}

