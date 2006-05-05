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
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdOdbcConstraintReader::FdoSmPhRdOdbcConstraintReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,constraintName)),
    mConstraintName(constraintName),
    mOwner(owner)
{
}

FdoSmPhRdOdbcConstraintReader::FdoSmPhRdOdbcConstraintReader(
    FdoSmPhOwnerP owner,
	FdoStringP tableName,
    FdoStringP constraintType
) :
    FdoSmPhRdConstraintReader(MakeReader(owner,tableName,constraintType)),
    mConstraintName(constraintType.Upper()),
	mTableName(tableName),
    mOwner(owner)
{
}

FdoSmPhRdOdbcConstraintReader::~FdoSmPhRdOdbcConstraintReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdOdbcConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringP constraintName
)
{
    FdoStringP ownerName = owner->GetName();

    //Odbc> desc INFORMATION_SCHEMA.table_constraints;
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
    //Odbc> desc INFORMATION_SCHEMA.key_column_usage;
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
        L"select T.constraint_name as constraint_name, T.table_name as table_name,\n"
        L"         K1.column_name as column_name, K1.ordinal_position as position,\n"
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
        L"       T.constraint_catalog    = ? and\n"
        L"       T.constraint_name = ? and\n"
        L"       T.constraint_catalog = K1.constraint_catalog and\n"
        L"       T.constraint_schema = K1.constraint_schema and\n"
        L"       T.constraint_name = K1.constraint_name\n"
        L"       order by T.constraint_name, K1.ordinal_position";

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows( owner->GetManager() );

    // Create the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( owner->GetManager(), L"Binds" );

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"constraint_catalog",
        binds->CreateColumnDbObject(L"constraint_catalog",false)
    );

    field->SetFieldValue(ownerName.Lower());

    field = new FdoSmPhField(
        binds,
        L"constraint_name",
        binds->CreateColumnDbObject(L"constraint_name",false)
    );

    field->SetFieldValue(constraintName);

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sqlString, owner->GetManager(), binds );

    return( reader );
}

FdoSmPhReaderP FdoSmPhRdOdbcConstraintReader::MakeReader(
    FdoSmPhOwnerP owner,
	FdoStringP	tableName,
    FdoStringP constraintName
)
{
    // No constraints supported.
    return( (FdoSmPhReader *)NULL );
}
