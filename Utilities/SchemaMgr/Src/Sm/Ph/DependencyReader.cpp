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
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/DependencyWriter.h>

FdoSmPhDependencyReader::FdoSmPhDependencyReader(long classId, FdoStringP fkTableName, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader( MakeClauses(classId, fkTableName), mgr, true )
    )
{
}

FdoSmPhDependencyReader::FdoSmPhDependencyReader(FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader( MakeClauses(pkTableName, fkTableName, bAnd), mgr )
    )
{
}

FdoSmPhDependencyReader::~FdoSmPhDependencyReader(void)
{
}

FdoSmPhDependencyP FdoSmPhDependencyReader::GetDependency( FdoSmPhSchemaElement* pParent )
{
	FdoSmPhDependencyP pDep = 
		new FdoSmPhDependency(
			GetPkTableName(),
			GetPkColumnNames(),
			GetFkTableName(),
			GetFkColumnNames(),
			GetIdentityColumn(),
			GetOrderType(),
			GetCardinality(),
			pParent
		);

    return( pDep );
}

FdoStringP FdoSmPhDependencyReader::GetPkTableName()
{
	return(GetString(L"",L"pktablename"));
}

FdoStringsP FdoSmPhDependencyReader::GetPkColumnNames()
{
	// Parse the column list into a collection of column names.
    return( FdoStringCollection::Create(GetString(L"",L"pkcolumnnames"), L" ,", false) );
}

FdoStringP FdoSmPhDependencyReader::GetFkTableName()
{
	return(GetString(L"",L"fktablename"));
}

FdoStringsP FdoSmPhDependencyReader::GetFkColumnNames()
{
	// Parse the column list into a collection of column names.
    return( FdoStringCollection::Create(GetString(L"",L"fkcolumnnames"), L" ,", false) );
}

FdoStringP FdoSmPhDependencyReader::GetIdentityColumn()
{
	return(GetString(L"",L"identitycolumn"));
}

FdoStringP FdoSmPhDependencyReader::GetOrderType()
{
	return(GetString(L"",L"ordertype"));
}

long FdoSmPhDependencyReader::GetCardinality()
{
	return((long) GetDouble(L"",L"fkcardinality"));
}


FdoSmPhReaderP FdoSmPhDependencyReader::MakeReader( FdoStringP where, FdoSmPhMgrP mgr, bool bAddClassDef )
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing class attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP depRow = FdoSmPhDependencyWriter::MakeRow(mgr);
    rows->Add( depRow );

    if ( bAddClassDef ) {
        FdoStringP classDefTable = mgr->GetDcDbObjectName(L"f_classdefinition");
        FdoSmPhRowP row = new FdoSmPhRow( mgr, L"f_classdefinition", mgr->FindDbObject(classDefTable) );
        rows->Add( row );
    }

    if ( depRow->GetDbObject()->GetExists() ) {
        // F_ATTRIBUTEDEPENDENCY exists, read from MetaSchema
        pSubReader = mgr->CreateQueryReader( rows, where ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // F_ATTRIBUTEDEPENDENCY does not exist, nothing to read.
        pSubReader = new FdoSmPhReader( mgr, rows);
    }

    return pSubReader;
}

FdoStringP FdoSmPhDependencyReader::MakeClauses( long classId, FdoStringP fkTableName )
{
	return( 
        FdoStringP::Format( 
            L"where f_attributedependencies.pktablename = f_classdefinition.tablename and f_classdefinition.classid = %d and fktablename = '%ls'", 
            classId, 
            (FdoString*) (fkTableName)
        ) 
    );
}

FdoStringP FdoSmPhDependencyReader::MakeClauses( FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
	FdoStringP stmt;

	if ( (pkTableName.GetLength() == 0) && (fkTableName.GetLength() == 0) )
		return(stmt);

	if ( pkTableName.GetLength() == 0 ) 
		return( stmt + FdoStringP::Format( L" where fktablename = '%ls'", (FdoString*) fkTableName ) );

	if ( fkTableName.GetLength() == 0 ) 
		return( stmt + FdoStringP::Format( L" where pktablename = '%ls'", (FdoString*) pkTableName ) );

	if ( bAnd ) 
		return( stmt + FdoStringP::Format( L" where pktablename = '%ls' and fktablename = '%ls'", (FdoString*) pkTableName, (FdoString*) fkTableName ) );
	else 
		return( stmt + FdoStringP::Format( L" where pktablename = '%ls' or fktablename = '%ls'", (FdoString*) pkTableName, (FdoString*) fkTableName ) );
}
