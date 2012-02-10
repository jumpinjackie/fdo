 //
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ColumnReader.h"
#include "../ColTypeMapper.h"
#include "../Mgr.h"
#include "../Owner.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

#include <cassert>

FdoSmPhRdPostGisColumnReader::FdoSmPhRdPostGisColumnReader(FdoSmPhOwnerP owner,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdColumnReader(
        (FdoSmPhReader*) NULL,
        dbObject
    )
{
    SetSubReader(
        MakeQueryReader(
            owner,
            DbObject2Objects(dbObject)
        )
    );
}

FdoSmPhRdPostGisColumnReader::FdoSmPhRdPostGisColumnReader(FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
    : FdoSmPhRdColumnReader(
        (FdoSmPhReader*) NULL,
        (FdoSmPhDbObject*) NULL
    )
{
    SetSubReader(
        MakeQueryReader(
            owner,
            objectNames
        )
    );
}

FdoSmPhRdPostGisColumnReader::FdoSmPhRdPostGisColumnReader(FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join)
    : FdoSmPhRdColumnReader(
        (FdoSmPhReader*) NULL,
        (FdoSmPhDbObject*) NULL
    )
{
    SetSubReader(
        MakeQueryReader(
            owner, 
            DbObject2Objects((FdoSmPhDbObject*)NULL), 
            join
        )
    );
}

FdoSmPhRdPostGisColumnReader::~FdoSmPhRdPostGisColumnReader()
{
    // idle
}

bool FdoSmPhRdPostGisColumnReader::ReadNext()
{
    bool gotRow = FdoSmPhReader::ReadNext();

    if (gotRow)
    {
        mSize = 0;
        mScale = 0;

        FdoStringP colType = GetString(L"", L"type_string");
        FdoInt32 typmod = GetLong(L"", L"typmod");

        mColType = FdoSmPhPostGisColTypeMapper::String2Type(
            colType,
            typmod,
            mSize,
            mScale
        );

    }

    return gotRow;
}

FdoStringP FdoSmPhRdPostGisColumnReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;
   if ( fieldName == L"default_value" ) {
        FdoStringP defValue = FdoSmPhRdColumnReader::GetString( tableName, fieldName ); // '2001-10-01 00:00:00'::timestamp without time zone
       if( defValue != NULL && defValue.GetLength() != 0 ) {
            if ( GetType() == FdoSmPhColType_Date )  {
                // reformat the string to the Fdo expected DateTime default format
                fieldValue = defValue = defValue.Left(L"::");  
                if( fieldValue != NULL && fieldValue.GetLength() != 0 )
                    fieldValue = FdoStringP(L"TIMESTAMP ") + defValue;
                else
                    fieldValue = L"";
            }
            else {
                // Remove any type casting at the end of the default value.
                FdoString* defPtr = (FdoString*) defValue;
                for ( int i = (wcslen(defPtr) - 1); i > 0; i-- ) {
                    // Don't remove anything inside brackets or quote delimiters
                    if ( (defPtr[i] == ')')|| (defPtr[i] == '\'') )
                        break;
 
                    // type casting part starts with ::
                    if ( wcsncmp(&defPtr[i], L"::", 2) == 0 ) {
                        defValue = defValue.Mid(0,i);
                        break;
                    }
                }

                fieldValue = defValue;
            }
        } 
    }
    else if ( fieldName == L"size" ) {
        fieldValue = FdoStringP::Format( L"%d", mSize );
    }
    else if ( fieldName == L"scale" ) {
        fieldValue = FdoStringP::Format( L"%d", mScale );
    }
    else {
        fieldValue = FdoSmPhRdColumnReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhReaderP FdoSmPhRdPostGisColumnReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join)
{
    mSize = 0;
    mScale = 0;

    FdoStringP sql;

    FdoStringP ownerName(owner->GetName());

    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();

    FdoSmPhPostGisOwnerP pgOwner = owner->SmartCast<FdoSmPhPostGisOwner>();
    assert(NULL != pgOwner);

    FdoStringP columnsTableName(pgOwner->GetColumnsTable());

    // template_postgis=# \d information_schema.columns
    //          Column          |     Type        | 
    // -------------------------+-----------------+
    // table_catalog            | sql_identifier  | <- Always the current database
    // table_schema             | sql_identifier  | <- Schema in the current database
    // table_name               | sql_identifier  |
    // column_name              | sql_identifier  |
    // ordinal_position         | cardinal_number | <- Count starts at 1
    // column_default           | character_data  |
    // is_nullable              | character_data  |
    // data_type                | character_data  |
    // character_maximum_length | cardinal_number | <- NULL for non-text/non-bit-string types 
    // character_octet_length   | cardinal_number | <- NULL for non-text types 
    // numeric_precision        | cardinal_number |
    // numeric_precision_radix  | cardinal_number |
    // numeric_scale            | cardinal_number |
    // character_set_name       | sql_identifier  | <- Not avilable in PostgreSQL
    // collation_name           | sql_identifier  | <- Not avilable in PostgreSQL
    // is_updatable             | character_data  | <- YES always for BASE TABLE

    // FDO needs a size (length) for types that become strings.
    // For such types that don't allow size to be set, the following query
    // returns the  max length for the type, as size.

    // Length for bit columns is not stored in any of the lenght or precision
    // fields so it is parsed out of column_type, which will have
    // format of bit(n) where n is the number of bits.

    // TODO: mloskot - Currently, if there is no length available for
    // text types, 64 KB is assumed as max text size.
    // Although, in PostgreSQL, the longest possible character string that
    // can be stored is about 1 GB.

    // $1 - name of table

    sql = FdoStringP::Format(
        L" SELECT %ls n.nspname || '.' || r.relname AS table_name, c.attname AS name, 1 AS type,"
	    L" d.adsrc as default_value, \n"
        L" c.atttypmod AS typmod, "
        L" CASE WHEN c.attnotnull THEN 0 ELSE 1 END AS nullable,"
        L" 0 as is_autoincremented, "
        L" lower(t.typname) AS type_string,"
        L" cast (null as text) as geom_type,"
        L" c.attnum as ordinal_position, "
        L" %ls as collate_schema_name, "
        L" %ls as collate_name "
        L" FROM pg_catalog.pg_attribute as c "
        L" JOIN pg_catalog.pg_class r on c.attrelid = r.oid "
        L" JOIN pg_catalog.pg_namespace n on r.relnamespace = n.oid and n.nspname not in ('pg_catalog','information_schema','pg_toast','pg_temp1','pg_toast_temp1') "
        L" JOIN pg_catalog.pg_type t on c.atttypid = t.oid "
        L" $(JOIN_CLAUSE) "
        L" LEFT JOIN pg_catalog.pg_attrdef d ON d.adrelid = r.oid and d.adnum = c.attnum "
        L" where c.attnum > 0 "
        L" $(AND) $(QUALIFICATION) "
        L" ORDER BY collate_schema_name, collate_name, c.attnum ASC",
        (join ? L"distinct" : L""),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"n.nspname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"r.relname"),
        static_cast<FdoString*>(columnsTableName)
    );

    FdoSmPhReaderP reader = FdoSmPhRdColumnReader::MakeQueryReader(
        L"",
        owner,
        sql,
        L"n.nspname",
        L"r.relname",
        objectNames,
        join
    );

    return reader;
}

FdoSmPhRowsP FdoSmPhRdPostGisColumnReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Call superclass to populate generic row/columns:
    FdoSmPhRowsP rows = FdoSmPhRdColumnReader::MakeRows(mgr);
    FdoSmPhRowP row = rows->GetItem(0);

    FdoSmPhFieldP field(new FdoSmPhField(row,
        L"type_string",
        row->CreateColumnDbObject(L"type_string", false)));

    field = new FdoSmPhField(row,
        L"typmod",
        row->CreateColumnInt32(L"typmod", true));

    field = new FdoSmPhField(row,
        L"ordinal_position",
        row->CreateColumnInt32(L"ordinal_position", true));

    return( rows);
}

FdoSmPhColType FdoSmPhRdPostGisColumnReader::GetType()
{
    CheckGet();

    return mColType;
}

