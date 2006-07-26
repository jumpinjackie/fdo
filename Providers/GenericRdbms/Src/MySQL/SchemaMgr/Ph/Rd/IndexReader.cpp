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
#include "IndexReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlIndexReader::FdoSmPhRdMySqlIndexReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdIndexReader((FdoSmPhReader*)NULL),
    mDbObject(dbObject)
{
    SetSubReader(MakeReader(mgr, (const FdoSmPhOwner*)(dbObject->GetParent()), dbObject));
}

FdoSmPhRdMySqlIndexReader::FdoSmPhRdMySqlIndexReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdIndexReader((FdoSmPhReader*) NULL)
{
    SetSubReader(MakeReader(mgr, (FdoSmPhOwner*)owner, (FdoSmPhDbObject*)NULL));
}

FdoSmPhRdMySqlIndexReader::~FdoSmPhRdMySqlIndexReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdMySqlIndexReader::MakeReader(
    FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoStringP objectName = dbObject ? dbObject->GetName() : L"";
    FdoStringP ownerName = owner->GetName();

    //mysql> desc INFORMATION_SCHEMA.statistics;
    //+---------------+--------------+------+-----+---------+-------+
    //| Field         | Type         | Null | Key | Default | Extra |
    //+---------------+--------------+------+-----+---------+-------+
    //| TABLE_CATALOG | varchar(512) | YES  |     | NULL    |       |
    //| TABLE_SCHEMA  | varchar(64)  | NO   |     |         |       |
    //| TABLE_NAME    | varchar(64)  | NO   |     |         |       |
    //| NON_UNIQUE    | bigint(1)    | NO   |     | 0       |       |
    //| INDEX_SCHEMA  | varchar(64)  | NO   |     |         |       |
    //| INDEX_NAME    | varchar(64)  | NO   |     |         |       |
    //| SEQ_IN_INDEX  | bigint(2)    | NO   |     | 0       |       |
    //| COLUMN_NAME   | varchar(64)  | NO   |     |         |       |
    //| COLLATION     | varchar(1)   | YES  |     | NULL    |       |
    //| CARDINALITY   | bigint(21)   | YES  |     | NULL    |       |
    //| SUB_PART      | bigint(3)    | YES  |     | NULL    |       |
    //| PACKED        | varchar(10)  | YES  |     | NULL    |       |
    //| NULLABLE      | varchar(3)   | NO   |     |         |       |
    //| INDEX_TYPE    | varchar(16)  | NO   |     |         |       |
    //| COMMENT       | varchar(16)  | YES  |     | NULL    |       |
    //+---------------+--------------+------+-----+---------+-------+

    // Generate SQL statement for selecting the indexes and their columns

    // information_schema tables use the utf8 character set with a 
    // case-insensitive collation. This causes problems with MySQL instances
    // on Linux, where database and table names are case-sensitive.
    // The following query overrides the collations to utf8_bin, which
    // is case-sensitive. 

    FdoStringP sql = FdoStringP::Format(
        L"select index_name, table_name, column_name, if(non_unique>0,'NONUNIQUE','UNIQUE') as uniqueness, index_type\n"
        L"  from INFORMATION_SCHEMA.statistics\n"
        L"  where\n"
        L"    table_schema collate utf8_bin = ?\n"
        L"    %ls\n"
        L"  order by table_name collate utf8_bin, index_name collate utf8_bin, seq_in_index",
        dbObject ? L"and table_name collate utf8_bin = ?" : L""
    );

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"table_schema",
        binds->CreateColumnDbObject(L"table_schema",false)
    );

    field->SetFieldValue(ownerName);

    if ( dbObject ) {
        field = new FdoSmPhField(
            binds,
            L"table_name",
            binds->CreateColumnDbObject(L"table_name",false)
        );

        field->SetFieldValue(objectName);
    }

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sql, mgr, binds );

    return( reader );
}
