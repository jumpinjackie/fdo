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
#include "DbObjectReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Owner.h"

FdoSmPhRdMySqlDbObjectReader::FdoSmPhRdMySqlDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*)NULL, owner, objectName)
{
    SetSubReader(MakeQueryReader(owner,objectName));
}

FdoSmPhRdMySqlDbObjectReader::FdoSmPhRdMySqlDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*) NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner,L"",join));
}

FdoSmPhRdMySqlDbObjectReader::~FdoSmPhRdMySqlDbObjectReader(void)
{
}

FdoSmPhDbObjType FdoSmPhRdMySqlDbObjectReader::GetType()
{
    FdoStringP type;
    FdoSmPhDbObjType ret;

    type = GetString (L"", L"type");
    if (type == L"base table")
        ret = FdoSmPhDbObjType_Table;
    else if (type == L"view")
        ret = FdoSmPhDbObjType_View;
    else
        ret = FdoSmPhDbObjType_Unknown;

    return (ret);
}


FdoSmPhReaderP FdoSmPhRdMySqlDbObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName,
    FdoSmPhRdTableJoinP join
)
{
    bool                 dblink_set = true;
    bool                 object_set = true;
    FdoStringP           sqlString;
    FdoStringP           ownerName = owner->GetName();
    FdoStringP           dblinkName = owner->GetParent()->GetName();

    // Use temporary table when not selecting all tables for this owner. When repeated selects
    // done against information_schema.tables, it is more efficient to build and use a temporary table.
    FdoStringP           tablesTableName = ((FdoSmPhMySqlOwner*)(FdoSmPhOwner*)owner)->GetTablesTable( join || (objectName != L"") );
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhMySqlMgr*       pMgr = (FdoSmPhMySqlMgr*)(FdoSmPhMgr*)mgr;

    if (dblinkName.GetLength() == 0 )
        dblink_set = false;

    if (objectName.GetLength() == 0 )
        object_set = false;

    FdoSmPhReaderP reader;
//TODO: cache the queries for performance
/*
    if ( object_set )
        reader = pMgr->GetDbObjectReader(dblink_set);
    else
        reader = pMgr->GetDbObjectsReader(dblink_set);
*/
    if ( !reader ) {
        // Generate sql statement if not already done

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        if ( (join != NULL) && (objectName == L"") ) 
            joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );

        FdoStringP qualification;

        if ( objectName != L"" ) {
            // Selecting single object, qualify by this object.
            qualification = L"  and ist.table_name collate utf8_bin = ?\n";
        } 
        else {
            if ( join != NULL )
                // Otherwise, if joining to another table, generated join clause.
                qualification = FdoStringP::Format( L"  and (%ls)\n", (FdoString*) join->GetWhere(L"ist.table_name") );
        }

        //mysql> describe INFORMATION_SCHEMA.tables;
        //+-----------------+--------------+------+-----+---------+-------+
        //| Field           | Type         | Null | Key | Default | Extra |
        //+-----------------+--------------+------+-----+---------+-------+
        //| TABLE_CATALOG   | varchar(512) | YES  |     | NULL    |       |<- always NULL
        //| TABLE_SCHEMA    | varchar(64)  | NO   |     |         |       |
        //| TABLE_NAME      | varchar(64)  | NO   |     |         |       |
        //| TABLE_TYPE      | varchar(64)  | NO   |     |         |       |
        //| ENGINE          | varchar(64)  | YES  |     | NULL    |       |
        //| VERSION         | bigint(21)   | YES  |     | NULL    |       |
        //| ROW_FORMAT      | varchar(10)  | YES  |     | NULL    |       |
        //| TABLE_ROWS      | bigint(21)   | YES  |     | NULL    |       |
        //| AVG_ROW_LENGTH  | bigint(21)   | YES  |     | NULL    |       |
        //| DATA_LENGTH     | bigint(21)   | YES  |     | NULL    |       |
        //| MAX_DATA_LENGTH | bigint(21)   | YES  |     | NULL    |       |
        //| INDEX_LENGTH    | bigint(21)   | YES  |     | NULL    |       |
        //| DATA_FREE       | bigint(21)   | YES  |     | NULL    |       |
        //| AUTO_INCREMENT  | bigint(21)   | YES  |     | NULL    |       |
        //| CREATE_TIME     | datetime     | YES  |     | NULL    |       |
        //| UPDATE_TIME     | datetime     | YES  |     | NULL    |       |
        //| CHECK_TIME      | datetime     | YES  |     | NULL    |       |
        //| TABLE_COLLATION | varchar(64)  | YES  |     | NULL    |       |
        //| CHECKSUM        | bigint(21)   | YES  |     | NULL    |       |
        //| CREATE_OPTIONS  | varchar(255) | YES  |     | NULL    |       |
        //| TABLE_COMMENT   | varchar(80)  | NO   |     |         |       |
        //+-----------------+--------------+------+-----+---------+-------+

        // information_schema tables use the utf8 character set with a 
        // case-insensitive collation. This causes problems with MySQL instances
        // on Linux, where database and table names are case-sensitive.
        // The following query overrides the collations to utf8_bin, which
        // is case-sensitive. 

        //TODO: find a way to read data/index directory; found no mention of how to do this in MySQL manual.
		//TODO: read autoincrement_column_name from the column reader instead of this table reader
        sqlString = FdoStringP::Format(
            L"select %ls ist.table_name as name, lower(ist.table_type) as type,\n"
            L"  ' ' as autoincrement_column_name, \n"
            L"  ist.auto_increment as autoincrement_column_seed, \n"
            L"  ist.engine as storage_engine, \n"
            L"  ' ' as data_directory, \n"
            L"  ' ' as index_directory \n"
            L"  from %ls ist%ls \n"
            L"  where ist.table_schema collate utf8_bin = ?\n"
            L"  %ls\n"
            L"  and ist.table_type in ('BASE TABLE', 'VIEW')\n"
            L"  order by ist.table_name collate utf8_bin asc",
            join ? L"distinct" : L"",
            (FdoString*) tablesTableName,
            (FdoString*) joinFrom,
            (FdoString*) qualification
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, MakeBinds(mgr,ownerName,objectName) );
/*
        if ( object_set )
            pMgr->SetDbObjectReader(reader, dblink_set);
        else
            pMgr->SetDbObjectsReader(reader, dblink_set);
*/
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        FdoSmPhRowP binds = pReader->GetBinds();
		FdoSmPhFieldsP	fields = binds->GetFields();

        FdoSmPhFieldP(fields->GetItem(L"table_schema"))->SetFieldValue(ownerName);
        if ( object_set )
            FdoSmPhFieldP(fields->GetItem(L"table_name"))->SetFieldValue(objectName);

        pReader->Execute();
    }

    return reader;
}


FdoSmPhRowP FdoSmPhRdMySqlDbObjectReader::MakeBinds( FdoSmPhMgrP mgr, FdoStringP ownerName, FdoStringP objectName )
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    FdoSmPhFieldP field = new FdoSmPhField(
        row,
        L"table_schema",
        rowObj->CreateColumnDbObject(L"table_schema",false)
    );

    field->SetFieldValue(ownerName);

    if ( objectName.GetLength() > 0 ) {
        field = new FdoSmPhField(
            row,
            L"table_name",
            rowObj->CreateColumnDbObject(L"table_name",false)
        );

        field->SetFieldValue(objectName);
    }

    return( row );
}

FdoSmPhRowsP FdoSmPhRdMySqlDbObjectReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Let superclass make its rows:
    FdoSmPhRowsP rows = FdoSmPhRdDbObjectReader::MakeRows(mgr);
    FdoSmPhRowP row = rows->GetItem(0);

    // Add our own MySQL-specific rows:
    FdoSmPhFieldP pField = new FdoSmPhField(row, L"autoincrement_column_name", row->CreateColumnChar(L"autoincrement_column_name",true, 64));
    pField = new FdoSmPhField(row, L"autoincrement_column_seed", row->CreateColumnInt64(L"autoincrement_column_seed",true));
    pField = new FdoSmPhField(row, L"storage_engine", row->CreateColumnChar(L"storage_engine",true, 64)); // can be NULL (e.g. for views)
    pField = new FdoSmPhField(row, L"data_directory", row->CreateColumnChar(L"data_directory",true, 256));
    pField = new FdoSmPhField(row, L"index_directory", row->CreateColumnChar(L"index_directory",true, 256));

    return rows;
}
