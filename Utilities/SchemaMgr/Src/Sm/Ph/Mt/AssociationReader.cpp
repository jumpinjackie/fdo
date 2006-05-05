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
#include <Sm/Ph/Mt/AssociationReader.h>

FdoSmPhMtAssociationReader::FdoSmPhMtAssociationReader(FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName) : 
    FdoSmPhReader( 
        MakeReader(
            mgr,
            rows,
            classId, 
            fkTableName
        )
    )
{
}

FdoSmPhMtAssociationReader::FdoSmPhMtAssociationReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd ) : 
    FdoSmPhReader( 
        MakeReader(
            mgr,
            rows,
            pkTableName, 
            fkTableName, 
            bAnd
        )
    )
{
}

FdoSmPhMtAssociationReader::~FdoSmPhMtAssociationReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtAssociationReader::MakeReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName )
{
    // Generate the where clause
	FdoStringP where = FdoStringP::Format( 
        L"where f_associationdefinition.pktablename = f_classdefinition.tablename and f_classdefinition.classid = %d and fktablename = '%ls'", 
        classId, 
        (FdoString*) (fkTableName) 
    );

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP subReader = mgr->CreateQueryReader( rows, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*)subReader );
}

FdoSmPhReaderP FdoSmPhMtAssociationReader::MakeReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
	FdoStringP where;

    // Generate the where clause
    if ( pkTableName.GetLength() == 0 ) 
		where = FdoStringP::Format( L" where fktablename = '%ls'", (FdoString*) (fkTableName) );
	else if ( fkTableName.GetLength() == 0 ) 
		where = FdoStringP::Format( L" where pktablename = '%ls'", (FdoString*) (pkTableName) );
	else if ( bAnd ) 
		where = FdoStringP::Format( L" where pktablename = '%ls' and fktablename = '%ls'", (FdoString*) (pkTableName), (FdoString*) (fkTableName) );
	else 
		where = FdoStringP::Format( L" where pktablename = '%ls' or fktablename = '%ls'", (FdoString*) (pkTableName), (FdoString*) (fkTableName) );

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP subReader = mgr->CreateQueryReader( rows, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*)subReader );
}
