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
#include "ColumnReader.h"
#include "../ColTypeMapper.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdMySqlColumnReader::FdoSmPhRdMySqlColumnReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdColumnReader(MakeQueryReader(mgr, dbObject), dbObject)
{
}

FdoSmPhRdMySqlColumnReader::~FdoSmPhRdMySqlColumnReader(void)
{
}

bool FdoSmPhRdMySqlColumnReader::ReadNext()
{
    bool gotRow = FdoSmPhReader::ReadNext();

    if ( gotRow ) {
        bool isUnsigned = (GetLong( L"",L"isunsigned") != 0);

        mColType = FdoSmPhMySqlColTypeMapper::String2Type( 
            GetString(L"", L"type_string"), 
            isUnsigned,
            GetLong( L"", L"size"), 
            GetLong( L"", L"scale" )
        );
    }

    return gotRow;
}


FdoSmPhReaderP FdoSmPhRdMySqlColumnReader::MakeQueryReader (FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject)
{
    FdoStringP table_name;
    FdoStringP table_owner;
    FdoStringP database;
    FdoStringP sql;
    FdoSmPhRowsP rows;
    FdoSmPhRowP row;
    FdoSmPhRdGrdQueryReader* pReader;
    FdoSmPhRowP binds;

    table_name = dbObject->GetName ();
    table_owner = dbObject->GetOwner ();
    database = dbObject->GetDatabase (); // ??

    FdoSmPhReaderP reader;
    // todo: cache the reader
    if (!reader)
    {
        // Generate sql statement if not already done
        //mysql> desc INFORMATION_SCHEMA.columns;
        //+--------------------------+--------------+------+-----+---------+-------+
        //| Field                    | Type         | Null | Key | Default | Extra |
        //+--------------------------+--------------+------+-----+---------+-------+
        //| TABLE_CATALOG            | varchar(512) | YES  |     | NULL    |       |<- always NULL
        //| TABLE_SCHEMA             | varchar(64)  | NO   |     |         |       |
        //| TABLE_NAME               | varchar(64)  | NO   |     |         |       |
        //| COLUMN_NAME              | varchar(64)  | NO   |     |         |       |
        //| ORDINAL_POSITION         | bigint(21)   | NO   |     | 0       |       |
        //| COLUMN_DEFAULT           | varchar(64)  | YES  |     | NULL    |       |
        //| IS_NULLABLE              | varchar(3)   | NO   |     |         |       |
        //| DATA_TYPE                | varchar(64)  | NO   |     |         |       |
        //| CHARACTER_MAXIMUM_LENGTH | bigint(21)   | YES  |     | NULL    |       |
        //| CHARACTER_OCTET_LENGTH   | bigint(21)   | YES  |     | NULL    |       |
        //| NUMERIC_PRECISION        | bigint(21)   | YES  |     | NULL    |       |
        //| NUMERIC_SCALE            | bigint(21)   | YES  |     | NULL    |       |
        //| CHARACTER_SET_NAME       | varchar(64)  | YES  |     | NULL    |       |
        //| COLLATION_NAME           | varchar(64)  | YES  |     | NULL    |       |
        //| COLUMN_TYPE              | longtext     | NO   |     |         |       |
        //| COLUMN_KEY               | varchar(3)   | NO   |     |         |       |
        //| EXTRA                    | varchar(20)  | NO   |     |         |       |
        //| PRIVILEGES               | varchar(80)  | NO   |     |         |       |
        //| COLUMN_COMMENT           | varchar(255) | NO   |     |         |       |
        //+--------------------------+--------------+------+-----+---------+-------+

        // FDO needs a size (length) for types that become strings. For such types 
        // that don't allow size to be set, the following query returns the 
        // max length for the type, as size.

        // Length for bit columns is not stored in any of the lenght or precision
        // fields so it is parsed out of column_type, which will have format
        // "bit(n)" where n is the number of bits.

        // Note: the maximum length for longtext is actually 4294967295 but
        // the max length of an FDO Data Property is 2147483647.

        // information_schema tables use the utf8 character set with a 
        // case-insensitive collation. This causes problems with MySQL instances
        // on Linux, where database and table names are case-sensitive.
        // The following query overrides the collations to utf8_bin, which
        // is case-sensitive. 

        sql = L"select column_name as name, 1 as type,\n"
              L" CASE lower(data_type) \n"
              L"             WHEN 'tinytext' THEN 255 \n"
              L"             WHEN 'text' THEN 65535 \n"
              L"             WHEN 'mediumtext' THEN 16777215 \n"
              L"             WHEN 'longtext' THEN 2147483647  \n"
              L"             WHEN 'enum' THEN 255  \n"
              L"             WHEN 'set' THEN 255  \n"
              L"             When 'bit' THEN CONVERT( MID(column_type,INSTR(column_type,'(')+1,INSTR(column_type,')')-INSTR(column_type,'(')-1), UNSIGNED) \n"
              L"             ELSE ifnull(character_octet_length,numeric_precision) \n"
              L" END as size, \n"
              L" ifnull(character_octet_length,numeric_precision) as size,\n"
              L" numeric_scale as scale,\n"
              L" if(is_nullable='YES',1,0) as nullable,\n"
              L" lower(data_type) as type_string,\n"
              L" instr(column_type,'unsigned') as isunsigned,\n"
			  L" if(extra='auto_increment',1,0) as is_autoincremented\n"
              L" from INFORMATION_SCHEMA.columns\n"
              L" where table_schema collate utf8_bin = ? \n"
              L" and table_name collate utf8_bin = ? \n"
              L" order by ordinal_position asc";

        rows = MakeRows (mgr);
        row = rows->GetItem (0);

        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"type_string",
            row->CreateColumnDbObject(L"type_string",false)
        );

        // Retrieve whether column is unsigned. This affectes the valid
        // value range and thus the corresponding FDO property type for the column.
        field = new FdoSmPhField(
            row, 
            L"isunsigned",
            row->CreateColumnInt64(L"isunsigned",false)
        );

        reader = new FdoSmPhRdGrdQueryReader (row, sql, mgr, MakeBinds (mgr, table_owner, table_name));

    }
    else {
        // Re-executing so update bind variables first.
        pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        binds = pReader->GetBinds ();

        binds->GetFields ()->GetItem (L"table_schema")->SetFieldValue (table_owner);
        binds->GetFields ()->GetItem (L"table_name")->SetFieldValue (table_name);

        pReader->Execute();
    }

    return reader;
}

FdoSmPhRowP FdoSmPhRdMySqlColumnReader::MakeBinds (FdoSmPhMgrP mgr, FdoStringP table_owner, FdoStringP table_name)
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    FdoSmPhFieldP field = new FdoSmPhField(
        row,
        L"table_schema",
        rowObj->CreateColumnDbObject(L"table_schema",false)
    );

    // Schema and table entries in INFORMATION_SCHEMA are always lower case
    field->SetFieldValue(table_owner);

    field = new FdoSmPhField(
        row,
        L"table_name",
        rowObj->CreateColumnDbObject(L"table_name",false)
    );
    field->SetFieldValue(table_name);

    return( row );
}


FdoSmPhColType FdoSmPhRdMySqlColumnReader::GetType()
{
    CheckGet();

    return mColType;
}
