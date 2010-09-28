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
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

#include <cassert>

FdoSmPhRdPostGisColumnReader::FdoSmPhRdPostGisColumnReader(FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdColumnReader(
        (FdoSmPhReader*) NULL,
        dbObject)
{
    SetSubReader(
        MakeQueryReader(mgr,
            static_cast<const FdoSmPhOwner*>(dbObject->GetParent()),
            dbObject)
    );
}

FdoSmPhRdPostGisColumnReader::FdoSmPhRdPostGisColumnReader(FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join)
    : FdoSmPhRdColumnReader( (FdoSmPhReader*) NULL,
    NULL)
{
    SetSubReader(
        MakeQueryReader(owner->GetManager(), owner.p, NULL, join)
    );
    // idle
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

FdoSmPhReaderP FdoSmPhRdPostGisColumnReader::MakeQueryReader(FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP dbObject,
    FdoSmPhRdTableJoinP join)
{
    FdoStringP sql;
    FdoSmPhRowP binds;
    FdoSmPhRowP row;
    FdoSmPhRowsP rows;
    FdoSmPhRdGrdQueryReader* queryReader = NULL;

    FdoStringP objectName(dbObject ? dbObject->GetName() : L"");
    FdoStringP ownerName(owner->GetName());

    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();

    const FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = static_cast<const FdoSmPhPostGisOwner*>(owner);
    assert(NULL != pgOwner);

    FdoStringP columnsTableName(pgOwner->GetColumnsTable());

    // TODO: cache the reader
    FdoSmPhReaderP reader;
    if (!reader)
    {
        // Generate sql statement if not already done

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        if ((NULL != join) && (0 == objectName.GetLength()))
        {
            joinFrom = FdoStringP::Format(L", %ls", 
                static_cast<FdoString*>(join->GetFrom()));
        }

        FdoStringP qualification;

        if (objectName.GetLength() > 0)
        {
            // Selecting single object, qualify by this object.
            if ( objectName.Contains(L".") ) 
                qualification = L" AND c.table_schema = $1 AND c.table_name = $2 ";
            else
                qualification = L" AND c.table_name = $1 ";
        } 
        else
        {
            if (NULL != join)
            {
                // Otherwise, if joining to another table, generated join clause.
                qualification = FdoStringP::Format(L" AND (%ls) ",
                    static_cast<FdoString*>(join->GetWhere(L"c.table_name")));
            }
        }


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
            L" FROM %ls AS c %ls "
            L" WHERE 1 = 1"
            L" %ls "
            L" ORDER BY collate_schema_name, collate_name, c.ordinal_position ASC",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"c.table_schema"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"c.table_name"),
            static_cast<FdoString*>(columnsTableName),
            static_cast<FdoString*>(joinFrom),
            static_cast<FdoString*>(qualification));

        rows = MakeRows(mgr);
        row = rows->GetItem(0);

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

        reader = new FdoSmPhRdGrdQueryReader(row, sql, mgr,
                    MakeBinds(mgr, ownerName, objectName));
    }
    else
    {

        queryReader = static_cast<FdoSmPhRdGrdQueryReader*>(reader.p);
        binds = queryReader->GetBinds();
		FdoSmPhFieldsP fields(binds->GetFields());

	   // FdoSmPhFieldP tblField(fields->GetItem(L"table_schema"));
       // tblField->SetFieldValue(ownerName);
        if (objectName != L"")
        {
            FdoSmPhFieldP tblField;
            tblField = fields->GetItem(L"table_name");
            tblField->SetFieldValue(objectName);
        }

        queryReader->Execute();
    }

    return reader;
}

FdoSmPhRowP FdoSmPhRdPostGisColumnReader::MakeBinds(FdoSmPhMgrP mgr,
    FdoStringP tableOwner, FdoStringP tableName)
{
    FdoSmPhRowP row(new FdoSmPhRow(mgr, L"Binds"));
    FdoSmPhDbObjectP rowObj(row->GetDbObject());

    if (tableName.GetLength() > 0)
    {
        FdoStringP schemaName;
        FdoStringP objectName;

        if ( tableName.Contains(L".") ) 
        {
            schemaName = tableName.Left(L".");
            objectName = tableName.Right(L".");
            FdoSmPhFieldP field(new FdoSmPhField(row, L"table_schema",
            rowObj->CreateColumnDbObject(L"table_schema", false)));
            field->SetFieldValue(schemaName);
        }
        else
            objectName = tableName;

        FdoSmPhFieldP field = new FdoSmPhField(row, L"table_name",
                rowObj->CreateColumnDbObject(L"table_name", false));
        field->SetFieldValue(objectName);
    }

    return row;
}


FdoSmPhColType FdoSmPhRdPostGisColumnReader::GetType()
{
    CheckGet();

    return mColType;
}

