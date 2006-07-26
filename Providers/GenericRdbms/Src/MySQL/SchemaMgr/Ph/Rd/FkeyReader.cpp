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
#include "FkeyReader.h"
#include "../Owner.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlFkeyReader::FdoSmPhRdMySqlFkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdFkeyReader(MakeReader(mgr, (const FdoSmPhOwner*) (dbObject->GetParent()), dbObject)),
    mDbObject(dbObject)
{
}

FdoSmPhRdMySqlFkeyReader::FdoSmPhRdMySqlFkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdFkeyReader(MakeReader(mgr, (FdoSmPhOwner*) owner, (FdoSmPhDbObject*) NULL))
{
}

FdoSmPhRdMySqlFkeyReader::~FdoSmPhRdMySqlFkeyReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdMySqlFkeyReader::MakeReader(
    FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoSmPhMySqlOwner* mqlOwner = (FdoSmPhMySqlOwner*) owner;
    FdoStringP objectName = dbObject ? dbObject->GetName() : L"";
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
    // Generate SQL statement to get foreign keys

    // information_schema tables use the utf8 character set with a 
    // case-insensitive collation. This causes problems with MySQL instances
    // on Linux, where database and table names are case-sensitive.
    // The following query overrides the collations to utf8_bin, which
    // is case-sensitive. 

    FdoStringP sql = FdoStringP::Format(
        L"select tc.constraint_name, tc.table_name, kcu.column_name,\n"
        L" kcu.referenced_table_schema as r_owner_name,\n"
        L" kcu.referenced_table_name as r_table_name,\n"
        L" kcu.referenced_column_name as r_column_name\n"
        L" from %ls tc,\n"
        L" %ls kcu\n"
        L" where tc.constraint_schema collate utf8_bin  = kcu.constraint_schema\n"
        L"   and tc.constraint_name collate utf8_bin = kcu.constraint_name\n"
        L"   and tc.constraint_schema collate utf8_bin = ?\n"
        L"   %ls\n"
        L"   and tc.constraint_type='FOREIGN KEY'\n"
        L" order by tc.table_name collate utf8_bin, tc.constraint_name collate utf8_bin, kcu.ordinal_position",
        (FdoString*) mqlOwner->GetTableConstraintsTable(),
        (FdoString*) mqlOwner->GetKeyColumnUsageTable(),
        dbObject ? L"and tc.table_name collate utf8_bin = ?" : L""
    );

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = binds->GetDbObject();

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"constraint_schema",
        rowObj->CreateColumnDbObject(L"constraint_schema",false)
    );

    field->SetFieldValue(ownerName);

    if ( dbObject ) {
        field = new FdoSmPhField(
            binds,
            L"table_name",
            rowObj->CreateColumnDbObject(L"table_name",false)
        );

        field->SetFieldValue(objectName);
    }

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sql, mgr, binds );

    return( reader );
}
