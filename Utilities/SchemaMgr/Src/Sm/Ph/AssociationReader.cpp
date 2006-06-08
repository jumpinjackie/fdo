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
#include <Sm/Ph/AssociationReader.h>
#include <Sm/Ph/AssociationWriter.h>
#include <Sm/Ph/Mt/AssociationReader.h>
#include <Sm/Ph/Rd/AssociationReader.h>

FdoSmPhAssociationReader::FdoSmPhAssociationReader(long classId, FdoStringP fkTableName, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader(
            mgr,
            classId, 
            fkTableName
        )
    )
{
}

FdoSmPhAssociationReader::FdoSmPhAssociationReader(FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader(
            mgr,
            pkTableName, 
            fkTableName, 
            bAnd
        )
    )
{
}

FdoSmPhAssociationReader::~FdoSmPhAssociationReader(void)
{
}

FdoStringP FdoSmPhAssociationReader::GetPkTableName()
{
	return(GetString(L"", L"pktablename"));
}

FdoSmPhColumnListP FdoSmPhAssociationReader::GetPkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create( GetManager(), GetString(L"", L"pkcolumnnames"), L" ") );
}

FdoStringP FdoSmPhAssociationReader::GetFkTableName()
{
	return(GetString(L"", L"fktablename"));
}

FdoSmPhColumnListP FdoSmPhAssociationReader::GetFkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create( GetManager(), GetString(L"", L"fkcolumnnames"), L" ") );
}

FdoStringP FdoSmPhAssociationReader::GetMultiplicity()
{
	return(GetString(L"", L"multiplicity"));
}

FdoStringP FdoSmPhAssociationReader::GetReverseMultiplicity()
{
	return(GetString(L"", L"reversemultiplicity"));
}

FdoStringP FdoSmPhAssociationReader::GetReverseName()
{
	return(GetString(L"", L"reversename"));
}

bool FdoSmPhAssociationReader::GetCascadelock()
{
    bool ret = false;

    if ( GetString(L"", L"cascadelock").ICompare(L"1") == 0 )
        ret = true;

	return ret;
}

int FdoSmPhAssociationReader::GetDeleteRule()
{
	return GetInteger(L"", L"deleterule");
}

FdoStringP FdoSmPhAssociationReader::GetPseudoColumnName()
{
	return(GetString(L"", L"pseudocolname"));
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeReader( FdoSmPhMgrP mgr, long classId, FdoStringP fkTableName )
{
    FdoSmPhReaderP subReader;
    FdoSmPhRowsP rows = MakeRows(mgr);
    FdoSmPhRowP row = rows->GetItem(0);

    if ( row->GetDbObject()->GetExists() ) {
        subReader = MakeMtReader( mgr, rows, classId, fkTableName );
    }
    else {
        subReader = MakeRdReader( mgr, rows, classId, fkTableName );
    }

    return subReader;
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeReader( FdoSmPhMgrP mgr, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
    FdoSmPhReaderP subReader;
    FdoSmPhRowsP rows = MakeRows(mgr);
    FdoSmPhRowP row = rows->GetItem(0);

    if ( row->GetDbObject()->GetExists() ) {
        // F_ASSOCIATIONDEFINITION exists, read from it
        subReader = MakeMtReader( mgr, rows, pkTableName, fkTableName, bAnd );
    }
    else {
        // F_ASSOCIATIONDEFINITION does not exist, generate associations from native physical
        // schema.
        subReader = MakeRdReader( mgr, rows, pkTableName, fkTableName, bAnd );
    }

    return subReader;
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeMtReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName )
{
    return new FdoSmPhMtAssociationReader( mgr, rows, classId, fkTableName );
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeMtReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
    return new FdoSmPhMtAssociationReader( mgr, rows, pkTableName, fkTableName, bAnd );
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeRdReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName )
{
    return new FdoSmPhReader( mgr, rows );
}

FdoSmPhReaderP FdoSmPhAssociationReader::MakeRdReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
    return new FdoSmPhRdAssociationReader( rows, pkTableName, fkTableName, bAnd, mgr );
}

FdoSmPhRowsP FdoSmPhAssociationReader::MakeRows( FdoSmPhMgrP mgr, bool bAddClassDef)
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Make F_ATTRIBUTEDEFINITION row
    FdoSmPhRowP row = FdoSmPhAssociationWriter::MakeRow( mgr );
    rows->Add( row );

    // Join to F_CLASSDEFINITION for classid
    if ( bAddClassDef ) {
        FdoSmPhRowP classRow = new FdoSmPhRow( mgr, L"F_CLASSDEFINITION", mgr->FindDbObject(L"F_CLASSDEFINITION") );
        rows->Add( classRow );
    }

    return rows;
}
