/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "ConstraintReader.h"
#include "DbObjectBinds.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqsConstraintReader::FdoSmPhRdSqsConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,constraintName)),
    mConstraintName(constraintName),
    mOwner(owner)
{
}

FdoSmPhRdSqsConstraintReader::FdoSmPhRdSqsConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,Table2Tables(tableName),(FdoSmPhRdTableJoin*) NULL,constraintType)),
    mConstraintName(constraintType.Upper()),
	mTableName(tableName),
    mOwner(owner)
{
}

FdoSmPhRdSqsConstraintReader::FdoSmPhRdSqsConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringsP tableNames,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,tableNames,(FdoSmPhRdTableJoin*) NULL,constraintType)),
    mConstraintName(constraintType.Upper()),
    mOwner(owner)
{
}

FdoSmPhRdSqsConstraintReader::FdoSmPhRdSqsConstraintReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,Table2Tables(L""),join,constraintType)),
    mConstraintName(constraintType.Upper()),
    mOwner(owner)
{
}

FdoSmPhRdSqsConstraintReader::~FdoSmPhRdSqsConstraintReader(void)
{
}

FdoStringP FdoSmPhRdSqsConstraintReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdConstraintReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdConstraintReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdConstraintReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhReaderP FdoSmPhRdSqsConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
)
{
    FdoSmPhSqsMgrP sqsMgr = owner->GetManager()->SmartCast<FdoSmPhSqsMgr>();
    FdoStringP ownerName = owner->GetName();

    //SqlServer> desc INFORMATION_SCHEMA.TABLE_CONSTRAINTS;
    //+--------------------+--------------+------+-----+---------+-------+
    //| Field              | Type         | Null | Key | Default | Extra |
    //+--------------------+--------------+------+-----+---------+-------+
    //| CONSTRAINT_CATALOG | varchar(512) | YES  |     | NULL    |       |
    //| CONSTRAINT_SCHEMA  | varchar(64)  | NO   |     |         |       |
    //| CONSTRAINT_NAME    | varchar(64)  | NO   |     |         |       |
    //| TABLE_SCHEMA       | varchar(64)  | NO   |     |         |       |
    //| TABLE_NAME         | varchar(64)  | NO   |     |         |       |
    //| CONSTRAINT_TYPE    | varchar(64)  | NO   |     |         |       |
    //+--------------------+--------------+------+-----+---------+-------+
    //SqlServer> desc INFORMATION_SCHEMA.key_column_usage;
    //+-------------------------------+--------------+------+-----+---------+-------+
    //| Field                         | Type         | Null | Key | Default | Extra |
    //+-------------------------------+--------------+------+-----+---------+-------+
    //| CONSTRAINT_CATALOG            | varchar(512) | YES  |     | NULL    |       |
    //| CONSTRAINT_SCHEMA             | varchar(64)  | NO   |     |         |       |
    //| CONSTRAINT_NAME               | varchar(64)  | NO   |     |         |       |
    //| TABLE_CATALOG                 | varchar(512) | YES  |     | NULL    |       |
    //| TABLE_SCHEMA                  | varchar(64)  | NO   |     |         |       |
    //| TABLE_NAME                    | varchar(64)  | NO   |     |         |       |
    //| COLUMN_NAME                   | varchar(64)  | NO   |     |         |       |
    //| ORDINAL_POSITION              | bigint(10)   | NO   |     | 0       |       |
    //| POSITION_IN_UNIQUE_CONSTRAINT | bigint(10)   | YES  |     | NULL    |       |
    //| REFERENCED_TABLE_SCHEMA       | varchar(64)  | YES  |     | NULL    |       |
    //| REFERENCED_TABLE_NAME         | varchar(64)  | YES  |     | NULL    |       |
    //| REFERENCED_COLUMN_NAME        | varchar(64)  | YES  |     | NULL    |       |
    //+-------------------------------+--------------+------+-----+---------+-------+

    // The following query gets the constraints from T and joins to K1 to get the
    // constraint columns.
    // TODO: get referenced constraint information when constraint is foreign key.

    FdoStringP sqlString = FdoStringP::Format(
        L"select T.CONSTRAINT_NAME collate latin1_general_bin as constraint_name, T.TABLE_NAME as table_name,\n"
        L"         K1.COLUMN_NAME as column_name, K1.ORDINAL_POSITION as position,\n"
        L"         NULL as r_owner,\n"
        L"         NULL as r_table_name,\n"
        L"         NULL as r_column_name\n"
        L"  from %ls.INFORMATION_SCHEMA.TABLE_CONSTRAINTS T,\n"
        L"       %ls.INFORMATION_SCHEMA.KEY_COLUMN_USAGE K1",
        (FdoString*) ownerName,
        (FdoString*) ownerName
    );

    sqlString = sqlString +
        L" where\n"
        L"       T.CONSTRAINT_CATALOG    = ? and\n"
        L"       T.CONSTRAINT_SCHEMA    = ? and\n"
        L"       T.CONSTRAINT_NAME = ? and\n"
        L"       T.CONSTRAINT_CATALOG = K1.CONSTRAINT_CATALOG and\n"
        L"       T.CONSTRAINT_SCHEMA = K1.CONSTRAINT_SCHEMA and\n"
        L"       T.CONSTRAINT_NAME = K1.CONSTRAINT_NAME\n"
        L"       order by T.CONSTRAINT_NAME collate latin1_general_bin asc, K1.ORDINAL_POSITION asc ";

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows( owner->GetManager() );

    // Create the bind variables
    FdoSmPhRowP binds = sqsMgr->MakeByDbObjectBinds( ownerName, constraintName );
	FdoSmPhRowP row = rows->GetItem(0);
//TODO: cache this query to make full use of the binds.

    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( row, sqlString, owner->GetManager(), binds );

    return( reader );
}

FdoSmPhReaderP FdoSmPhRdSqsConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
	FdoStringsP tableNames,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType
)
{
    FdoStringP ownerName = owner->GetName();
	
	FdoStringP sqlString;

    // Create a field object for each field in the select list.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow( owner->GetManager(), L"ConstraintColumns", (FdoSmPhDbObject*) NULL  );
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

    field = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    // Create binds for owner and optional object names
    FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
        owner->GetManager(),
        L"CK.TABLE_SCHEMA",
        L"user_name",
        L"CK.TABLE_NAME",
        L"object_name",
        tableNames
    );

    // If joining to another table, generated from sub-clause for table.
    FdoStringP joinFrom;
    if ( join != NULL ) 
        joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );

    // Get where clause for owner and object name binds.
    FdoStringP qualification = binds->GetSQL();

    if ( join != NULL ) {
        // If joining to another table, add join clause.
        qualification = FdoStringP::Format( 
            L"  %ls and ((CK.TABLE_SCHEMA = 'dbo' and CK.TABLE_NAME = %ls) or ((CK.TABLE_SCHEMA + '.' + CK.TABLE_NAME) = %ls))\n", 
            (FdoString*) ((FdoSmPhRdJoin*)(join.p))->GetWhere(),
            (FdoString*) join->GetJoinColumn(),
            (FdoString*) join->GetJoinColumn()
        );
    }

	if ( constraintType == L"C" ) 
	{
		// The following query gets the CHECK() constraints for a table T
		sqlString = FdoStringP::Format(
			L"select %ls CK.CONSTRAINT_NAME collate latin1_general_bin as constraint_name, \n"
			L"	  CK.TABLE_NAME collate latin1_general_bin as table_name, \n"
			L"	  CK.COLUMN_NAME as column_name, \n"
			L"    CCK.CHECK_CLAUSE as check_clause, \n"
			L"    CK.TABLE_SCHEMA collate latin1_general_bin as table_schema \n"
			L"    from %ls.INFORMATION_SCHEMA.CONSTRAINT_COLUMN_USAGE CK, \n"
			L"		   %ls.INFORMATION_SCHEMA.CHECK_CONSTRAINTS CCK, \n"
			L"		   %ls.INFORMATION_SCHEMA.TABLE_CONSTRAINTS TK \n"
            L"         %ls"
			L"    where (\n"
			L"			%ls %ls\n"
			L"			CK.CONSTRAINT_NAME = CCK.CONSTRAINT_NAME and \n"
			L"			CK.CONSTRAINT_NAME = TK.CONSTRAINT_NAME and \n"
			L"			TK.CONSTRAINT_TYPE = 'CHECK' )"			
			L"    order by CK.TABLE_SCHEMA collate latin1_general_bin asc, CK.TABLE_NAME collate latin1_general_bin asc, CK.CONSTRAINT_NAME collate latin1_general_bin asc",
            join ? L"distinct" : L"",
			(FdoString*) ownerName,
			(FdoString*) ownerName,
			(FdoString*) ownerName,
            (FdoString*) joinFrom,
			(FdoString*) qualification,
            (qualification == L"") ? L"" : L"and"
    	);

		field = new FdoSmPhField(
			row, 
            L"check_clause",
			row->CreateColumnDbObject(L"check_clause",false)
		);
	}
	else if ( constraintType == L"U" )
	{
		// The following query gets the UNIQUE constraints for a table T
		sqlString = FdoStringP::Format(
			L"select %ls CK.CONSTRAINT_NAME collate latin1_general_bin as constraint_name, \n"
			L"	  CK.TABLE_NAME collate latin1_general_bin as table_name, \n"
			L"	  CK.COLUMN_NAME collate latin1_general_bin as column_name, \n"
			L"	  CK.TABLE_SCHEMA collate latin1_general_bin as table_schema \n"
			L"    from %ls.INFORMATION_SCHEMA.CONSTRAINT_COLUMN_USAGE CK, \n"
			L"		   %ls.INFORMATION_SCHEMA.TABLE_CONSTRAINTS TK \n"
            L"         %ls"
			L"    where (\n"
			L"			%ls %ls\n"
			L"			CK.CONSTRAINT_NAME = TK.CONSTRAINT_NAME and \n"
			L"			TK.CONSTRAINT_TYPE = 'UNIQUE' )"			
			L"    order by CK.TABLE_SCHEMA collate latin1_general_bin asc, CK.TABLE_NAME collate latin1_general_bin asc, CK.CONSTRAINT_NAME collate latin1_general_bin asc, CK.COLUMN_NAME collate latin1_general_bin asc",
            join ? L"distinct" : L"",
			(FdoString*) ownerName,
			(FdoString*) ownerName,
            (FdoString*) joinFrom,
			(FdoString*) qualification,
            (qualification == L"") ? L"" : L"and"
		);
	}

//TODO: cache this query to make full use of the binds.
	FdoSmPhRowP bindRow = binds->GetBinds();
	row = rows->GetItem(0);

    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( row, sqlString, owner->GetManager(), bindRow );

    return( reader );
}

FdoStringsP FdoSmPhRdSqsConstraintReader::Table2Tables( FdoStringP tableName )
{
    FdoStringsP tableNames = FdoStringCollection::Create();
    if ( tableName != L"" ) 
        tableNames->Add( tableName );

    return tableNames;
}
