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
#include "ConstraintReader.h"
#include "../Owner.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlConstraintReader::FdoSmPhRdMySqlConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
) :
    FdoSmPhRdConstraintReader(),
    mConstraintName(constraintName),
    mOwner(owner)
{
    SetSubReader(MakeReader(owner,constraintName));
}

FdoSmPhRdMySqlConstraintReader::FdoSmPhRdMySqlConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(),
    mConstraintName(constraintType),
	mTableName(tableName),
    mOwner(owner)
{
    FdoStringsP tableNames = FdoStringCollection::Create();
    if ( tableName != L"" ) 
        tableNames->Add(tableName);

    SetSubReader(
        MakeReader(
            owner,
            tableNames,
            (FdoSmPhRdTableJoin*) NULL,
            constraintType
        )
    );
}

FdoSmPhRdMySqlConstraintReader::FdoSmPhRdMySqlConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringsP tableNames,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(),
    mConstraintName(constraintType),
    mOwner(owner)
{
    SetSubReader(
        MakeReader(
            owner,
            tableNames,
            (FdoSmPhRdTableJoin*) NULL,
            constraintType
        )
    );
}

FdoSmPhRdMySqlConstraintReader::FdoSmPhRdMySqlConstraintReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(),
    mConstraintName(constraintType.Upper()),
    mOwner(owner)
{
    FdoStringsP tableNames = FdoStringCollection::Create();

    SetSubReader(
        MakeReader(owner,tableNames,join,constraintType)
    );
}

FdoSmPhRdMySqlConstraintReader::~FdoSmPhRdMySqlConstraintReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdMySqlConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
)
{
    FdoStringP ownerName = owner->GetName();

    //mysql> desc INFORMATION_SCHEMA.table_constraints;
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
    //mysql> desc INFORMATION_SCHEMA.key_column_usage;
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

    // information_schema tables use the utf8 character set with a 
    // case-insensitive collation. This causes problems with MySQL instances
    // on Linux, where database and table names are case-sensitive.
    // The following query overrides the collations to utf8_bin, which
    // is case-sensitive. 

    FdoStringP sqlString =
        L"select constraint_name, table_name, column_name,\n"
        L"  referenced_table_schema as r_owner, referenced_table_name as r_table_name,\n"
        L"  referenced_column_name as r_column_name\n"
        L"  from INFORMATION_SCHEMA.key_column_usage kcu\n"
        L"    where kcu.table_schema collate utf8_bin = ? and kcu.constraint_name collate utf8_bin = ?\n"
        L"    order by kcu.ordinal_position";

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows( owner->GetManager() );

    // Create the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( owner->GetManager(), L"Binds" );

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"table_schema",
        binds->CreateColumnDbObject(L"table_schema",false)
    );

    field->SetFieldValue(ownerName);

    field = new FdoSmPhField(
        binds,
        L"constraint_name",
        binds->CreateColumnDbObject(L"constraint_name",false)
    );

    field->SetFieldValue(constraintName);

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( FdoSmPhRowP(rows->GetItem(0)), sqlString, owner->GetManager(), binds );

    return( reader );
}

FdoSmPhReaderP FdoSmPhRdMySqlConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
	FdoStringsP	tableNames,
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType
)
{
    FdoSmPhMgrP mgr = owner->GetManager();
    FdoSmPhMySqlOwnerP mqlOwner = owner->SmartCast<FdoSmPhMySqlOwner>();

	// MySql doesn't support CHECK()...
	if ( constraintType == L"C" )
		return (FdoSmPhReader*)NULL;

    FdoStringP ownerName = owner->GetName();
 
    FdoStringP sqlString = FdoStringP::Format(
      L"select %ls tc.constraint_name as constraint_name,\n"
      L" tc.table_name as table_name,\n"
      L" kcu.column_name as column_name\n"
      L" from %ls tc, %ls kcu $(JOIN_FROM)\n"
      L" where (tc.constraint_schema collate utf8_bin = kcu.constraint_schema\n"
      L"     and tc.constraint_name collate utf8_bin = kcu.constraint_name\n"
      L"     and tc.table_schema collate utf8_bin = kcu.table_schema\n"
      L"     and tc.table_name collate utf8_bin = kcu.table_name\n"
      L"     $(AND) $(QUALIFICATION)\n"
      L"     and tc.constraint_type = 'UNIQUE')\n"
      L" order by tc.table_name collate utf8_bin , tc.constraint_name collate utf8_bin",
      join ? L"distinct" : L"",
      (FdoString*) mqlOwner->GetTableConstraintsTable(),
      (FdoString*) mqlOwner->GetKeyColumnUsageTable()
    );

    FdoSmPhReaderP reader = FdoSmPhRdConstraintReader::MakeQueryReader(
        L"",
        mgr,
        sqlString,
        L"tc.table_schema collate utf8_bin",
        L"tc.table_name collate utf8_bin",
        ownerName,
        tableNames,
        join
    );

    return reader;
}

