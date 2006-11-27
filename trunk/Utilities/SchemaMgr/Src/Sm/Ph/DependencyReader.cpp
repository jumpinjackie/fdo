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
        MakeReader( MakeClauses(mgr, classId, fkTableName), mgr, true )
    )
{
}

FdoSmPhDependencyReader::FdoSmPhDependencyReader(FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader( MakeClauses(mgr, pkTableName, fkTableName, bAnd), mgr )

    )
{
}

FdoSmPhDependencyReader::FdoSmPhDependencyReader(FdoSmPhRdTableJoinP join, FdoSmPhMgrP mgr) : 
    FdoSmPhReader( 
        MakeReader( join, mgr )

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
	return(GetManager()->GetRealDbObjectName(GetString(L"",L"pktablename")));
}

FdoSmPhColumnListP FdoSmPhDependencyReader::GetPkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create(GetManager(), GetString(L"",L"pkcolumnnames"), L" ,") );
}

FdoStringP FdoSmPhDependencyReader::GetFkTableName()
{
	return(GetManager()->GetRealDbObjectName(GetString(L"",L"fktablename")));
}

FdoSmPhColumnListP FdoSmPhDependencyReader::GetFkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create(GetManager(), GetString(L"",L"fkcolumnnames"), L" ,") );
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

    if ( FdoSmPhDbObjectP(depRow->GetDbObject())->GetExists() ) {
        // F_ATTRIBUTEDEPENDENCY exists, read from MetaSchema
        pSubReader = mgr->CreateQueryReader( rows, where ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // F_ATTRIBUTEDEPENDENCY does not exist, nothing to read.
        pSubReader = new FdoSmPhReader( mgr, rows);
    }

    return pSubReader;
}

FdoSmPhReaderP FdoSmPhDependencyReader::MakeReader( FdoSmPhRdTableJoinP join, FdoSmPhMgrP mgr )
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing dependency attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Add f_attributedependency row
    FdoSmPhRowP depRow = FdoSmPhDependencyWriter::MakeRow(mgr);
    rows->Add( depRow );

    FdoSmPhDbObjectP dbObject = join->GetDbObject();

    // Add row for joined table.
    FdoSmPhRowP row = new FdoSmPhRow( mgr, join->GetName(), dbObject );
    rows->Add( row );

    if ( FdoSmPhDbObjectP(depRow->GetDbObject())->GetExists() ) {
        // f_attributedependency exists, read from MetaSchema.
        // Create a where clause that joins f_attributedependency and the join table
        FdoStringP whereClause = FdoStringP::Format( 
            L" where (%ls) order by f_attributedependencies.fktablename asc\n", (FdoString*) join->GetWhere(L"f_attributedependencies.fktablename") );

        pSubReader = mgr->CreateQueryReader( rows, whereClause ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // f_attributedependency does not exist, nothing to read.
        pSubReader = new FdoSmPhReader( mgr, rows);
    }

    return pSubReader;
}

FdoStringP FdoSmPhDependencyReader::MakeClauses( FdoSmPhMgrP mgr, long classId, FdoStringP fkTableName )
{
    FdoStringP localFkTableName = mgr->DbObject2MetaSchemaName(fkTableName);

    return( 
        FdoStringP::Format( 
            L"where f_attributedependencies.pktablename = f_classdefinition.tablename and f_classdefinition.classid = %d and fktablename in ( %ls, %ls )", 
            classId, 
            (FdoString*) (mgr->FormatSQLVal(fkTableName,FdoSmPhColType_String)),
            (FdoString*) (mgr->FormatSQLVal(localFkTableName,FdoSmPhColType_String))
        ) 
    );
}

FdoStringP FdoSmPhDependencyReader::MakeClauses( FdoSmPhMgrP mgr, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd )
{
	FdoStringP stmt;

	if ( (pkTableName.GetLength() == 0) && (fkTableName.GetLength() == 0) )
		return(stmt);

    FdoStringP localPkTableName = mgr->DbObject2MetaSchemaName(pkTableName);
    FdoStringP localFkTableName = mgr->DbObject2MetaSchemaName(fkTableName);

   	if ( pkTableName.GetLength() == 0 ) 
		return( stmt + FdoStringP::Format( 
            L" where fktablename in ( %ls, %ls )", 
            (FdoString*) mgr->FormatSQLVal(fkTableName,FdoSmPhColType_String),
            (FdoString*) mgr->FormatSQLVal(localFkTableName,FdoSmPhColType_String) 
        ) );
   
   	if ( fkTableName.GetLength() == 0 ) 
		return( stmt + FdoStringP::Format( 
        L" where pktablename in ( %ls, %ls )", 
        (FdoString*) mgr->FormatSQLVal(pkTableName,FdoSmPhColType_String), 
        (FdoString*) mgr->FormatSQLVal(localPkTableName,FdoSmPhColType_String) 
     ) );
   
   	if ( bAnd ) 
		return( stmt + FdoStringP::Format( 
            L" where pktablename in ( %ls, %ls ) and fktablename in ( %ls, %ls )", 
            (FdoString*) mgr->FormatSQLVal(localPkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(pkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(fkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(localFkTableName,FdoSmPhColType_String) 
        ) );
	else 
		return( stmt + FdoStringP::Format( 
            L" where pktablename in ( %ls, %ls ) or fktablename in ( %ls, %ls )", 
            (FdoString*) mgr->FormatSQLVal(pkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(localPkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(fkTableName,FdoSmPhColType_String), 
            (FdoString*) mgr->FormatSQLVal(localFkTableName,FdoSmPhColType_String) 
        ) );
}
