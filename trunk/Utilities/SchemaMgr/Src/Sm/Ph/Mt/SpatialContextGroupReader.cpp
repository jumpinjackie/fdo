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
#include <Sm/Ph/Mt/SpatialContextGroupReader.h>
#include <Sm/Ph/Rd/QueryReader.h>

FdoSmPhMtSpatialContextGroupReader::FdoSmPhMtSpatialContextGroupReader( FdoSmPhRowsP froms, FdoSmPhMgrP mgr ) : 
	FdoSmPhReader( MakeReader(froms, mgr) )
{
}

FdoSmPhMtSpatialContextGroupReader::~FdoSmPhMtSpatialContextGroupReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtSpatialContextGroupReader::MakeReader( FdoSmPhRowsP froms, FdoSmPhMgrP mgr )
{
    // Generate the where clause (don't really need a "where", but let's at least set the order).
    FdoStringP where = L"order by f_spatialcontextgroup.scgid";

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = mgr->CreateQueryReader( froms, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*)pSubReader );
}


