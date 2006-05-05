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
#include <Sm/Ph/Mt/ClassReader.h>

FdoSmPhMtClassReader::FdoSmPhMtClassReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr ) : 
	FdoSmPhReader( MakeReader(froms, schemaName, mgr) )
{
}

FdoSmPhMtClassReader::~FdoSmPhMtClassReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtClassReader::MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr )
{
    // Generate the where clause
    FdoStringP where = FdoStringP::Format( 
        L"where schemaname = %ls and f_classdefinition.classtype = f_classtype.classtype order by f_classdefinition.classname", 
    	(FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String)
    );

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = mgr->CreateQueryReader( froms, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*)pSubReader );
}


