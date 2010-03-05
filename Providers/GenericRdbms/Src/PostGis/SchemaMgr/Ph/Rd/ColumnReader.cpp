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
        bool isUnsigned = (GetLong(L"", L"isunsigned") != 0);

        SetBoolean( L"", L"is_autoincremented", false );

        FdoStringP GeomColType = GetString(L"", L"geom_type");
        FdoStringP colType = GetString(L"", L"type_string");
        if ( GeomColType != L"" ) 
            colType = GeomColType;

        mColType = FdoSmPhPostGisColTypeMapper::String2Type(
            colType,
            GetLong(L"", L"size"), 
            GetLong(L"", L"scale"));

        if ( (GeomColType != L"") && (mColType == FdoSmPhColType_Unknown) ) {
            mColType = FdoSmPhPostGisColTypeMapper::String2Type(
                L"geometry",
                GetLong(L"", L"size"), 
                GetLong(L"", L"scale"));
        }
    }

    return gotRow;
}

FdoStringP FdoSmPhRdPostGisColumnReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"default_value" && GetType() == FdoSmPhColType_Date )  {
        // reformat the string to the Fdo expected DateTime default format
        FdoStringP defValue = FdoSmPhRdColumnReader::GetString( tableName, fieldName ); // '2001-10-01 00:00:00'::timestamp without time zone
        if( defValue != NULL && defValue.GetLength() != 0 )
        {
            fieldValue = defValue = defValue.Left(L"::");  
            if( fieldValue != NULL && fieldValue.GetLength() != 0 )
                fieldValue = FdoStringP(L"TIMESTAMP ") + defValue;
            else
                fieldValue = L"";
        }
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
        L" SELECT %ls c.table_schema || '.' || c.table_name AS table_name, c.column_name AS name, 1 AS type,"
		L" COLUMN_DEFAULT as default_value, \n"
        L" COALESCE(c.character_maximum_length, c.character_octet_length,"
        L"         c.numeric_precision) AS size,"
        L" c.numeric_scale AS scale  ,"
        L" CASE WHEN c.is_nullable = 'YES' THEN 1 ELSE 0 END AS nullable,"
        L" lower(c.data_type) AS type_string,"
        L" 0 AS isunsigned,"
        L" 0 AS is_autoincremented, "
        L" (select d.typname from pg_type d, pg_attribute e, pg_class f, pg_namespace g "
        L"    where c.table_schema = g.nspname and c.table_name = f.relname and c.column_name = e.attname"
        L"    and d.typname = 'geometry' "
        L"    and e.atttypid = d.oid and e.attrelid = f.oid and f.relnamespace = g.oid ) as geom_type,"
        L" c.ordinal_position as ordinal_position, "
        L" %ls as collate_schema_name, "
        L" %ls as collate_name "
        L" FROM %ls AS c $(JOIN_FROM) "
        L" $(WHERE) $(QUALIFICATION) "
        L" ORDER BY collate_schema_name, collate_name, c.ordinal_position ASC",
        (join ? L"distinct" : L""),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"c.table_schema"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"c.table_name"),
        static_cast<FdoString*>(columnsTableName)
    );

    FdoSmPhReaderP reader = FdoSmPhRdColumnReader::MakeQueryReader(
        L"",
        owner,
        sql,
        L"c.table_schema",
        L"c.table_name",
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

    // Retrieve whether column is unsigned. This affects the valid
    // value range and thus the corresponding FDO property type for the column.
    field = new FdoSmPhField(row, L"isunsigned",
        row->CreateColumnInt64(L"isunsigned", false));

    field = new FdoSmPhField(row, L"isautoincremented",
        row->CreateColumnInt64(L"isautoincremented", false));

    field = new FdoSmPhField(row,
        L"geom_type",
        row->CreateColumnDbObject(L"geom_type", true));

    return( rows);
}

FdoSmPhColType FdoSmPhRdPostGisColumnReader::GetType()
{
    CheckGet();

    return mColType;
}

