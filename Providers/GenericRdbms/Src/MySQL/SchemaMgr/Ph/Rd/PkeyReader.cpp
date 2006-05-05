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
#include "PkeyReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlPkeyReader::FdoSmPhRdMySqlPkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdPkeyReader(MakeReader(mgr,dbObject)),
    mDbObject(dbObject)
{
}

FdoSmPhRdMySqlPkeyReader::~FdoSmPhRdMySqlPkeyReader(void)
{
}

FdoStringP FdoSmPhRdMySqlPkeyReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{

    FdoStringP value = FdoSmPhRdPkeyReader::GetString( tableName, fieldName );

    if ( (fieldName.ICompare(L"constraint_name") == 0) && (value.ICompare(L"PRIMARY") == 0) )
        value = L"";

    return value;        
}

FdoSmPhReaderP FdoSmPhRdMySqlPkeyReader::MakeReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoStringP objectName = dbObject->GetName();
    FdoStringP owner = dbObject->GetOwner();
    FdoStringP database = dbObject->GetDatabase();

    bool dbSet = database.GetLength() > 0;

    // Generate SQL statement to get the primary key columns.
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

    FdoStringP sqlString = L"select tc.constraint_name as constraint_name,\n"
      L" tc.table_name, kcu.column_name as column_name\n"
      L" from INFORMATION_SCHEMA.table_constraints tc, INFORMATION_SCHEMA.key_column_usage kcu\n"
      L" where (tc.constraint_schema collate utf8_bin = kcu.constraint_schema\n"
      L"     and tc.constraint_name collate utf8_bin = kcu.constraint_name\n"
      L"     and tc.table_schema collate utf8_bin = kcu.table_schema\n"
      L"     and tc.table_name collate utf8_bin = kcu.table_name\n"
      L"     and tc.table_schema collate utf8_bin = ?\n"
      L"     and tc.table_name collate utf8_bin = ?\n"
      L"     and tc.constraint_type = 'PRIMARY KEY')\n"
      L" order by kcu.ordinal_position";

    // Create a field object for each field in the select list.
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = binds->GetDbObject();

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"owner_name",
        rowObj->CreateColumnDbObject(L"owner_name",false)
    );

    field->SetFieldValue(owner);

    field = new FdoSmPhField(
        binds,
        L"object_name",
        rowObj->CreateColumnDbObject(L"object_name",false)
    );

    field->SetFieldValue(objectName);

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sqlString, mgr, binds );

    return( reader );
}
