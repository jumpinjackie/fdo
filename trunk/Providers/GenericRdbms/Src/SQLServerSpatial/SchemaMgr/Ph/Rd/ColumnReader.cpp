/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "DbObjectBinds.h"
#include "../ColTypeMapper.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdSqsColumnReader::FdoSmPhRdSqsColumnReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdColumnReader(MakeQueryReader(mgr, (const FdoSmPhOwner*)(dbObject->GetParent()), DbObject2Objects(dbObject)), dbObject)
{
}

FdoSmPhRdSqsColumnReader::FdoSmPhRdSqsColumnReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdColumnReader(MakeQueryReader(owner->GetManager(), (FdoSmPhOwner*)owner, objectNames), (FdoSmPhDbObject*)NULL)
{
}

FdoSmPhRdSqsColumnReader::FdoSmPhRdSqsColumnReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdColumnReader(MakeQueryReader(owner->GetManager(), (FdoSmPhOwner*)owner, DbObject2Objects((FdoSmPhDbObject*)NULL), join), (FdoSmPhDbObject*)NULL)
{
}

FdoSmPhRdSqsColumnReader::~FdoSmPhRdSqsColumnReader(void)
{
}

FdoStringP FdoSmPhRdSqsColumnReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdColumnReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdColumnReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdColumnReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

bool FdoSmPhRdSqsColumnReader::ReadNext()
{
    bool gotRow = FdoSmPhReader::ReadNext();

    if ( gotRow ) {
        bool isUnsigned = (GetLong( L"",L"isunsigned") != 0);

        mColType = FdoSmPhSqsColTypeMapper::String2Type( 
            GetString(L"", L"type_string"), 
            isUnsigned,
            GetLong( L"", L"size"), 
            GetLong( L"", L"scale" )
        );
    }

    return gotRow;
}


FdoSmPhReaderP FdoSmPhRdSqsColumnReader::MakeQueryReader (
    FdoSmPhMgrP mgr, 
    const FdoSmPhOwner* owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP sql;
    FdoSmPhRowsP rows;
    FdoSmPhRowP row;
    FdoSmPhRdGrdQueryReader* pReader;
    FdoSmPhRowP binds;
    FdoSmPhSqsMgrP sqsMgr = mgr->SmartCast<FdoSmPhSqsMgr>();

    FdoStringP ownerName = owner->GetName();

    FdoSmPhReaderP reader;
    // todo: cache the reader
    if (!reader)
    {
        // Generate sql statement if not already done

        // Create binds for owner and optional object names
        FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
            mgr,
            L"d.name",
            L"user_name",
            L"a.name",
            L"object_name",
            objectNames
        );

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        if ( join != NULL ) 
            joinFrom = FdoStringP::Format( 
                L"INNER JOIN %ls ON ((d.name = 'dbo' and a.name = %ls) or ((d.name + '.' + a.name) = %ls))\n", 
                (FdoString*) join->GetFrom(),
                (FdoString*) join->GetJoinColumn(),
                (FdoString*) join->GetJoinColumn()
            );

        // Get where clause for owner and object name binds.
        FdoStringP qualification = binds->GetSQL();

        //SqlServer> desc INFORMATION_SCHEMA.COLUMNS;
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
        // ... others not used
        //+--------------------------+--------------+------+-----+---------+-------+

        // Note: The data type for geometry and geography columns, reported by the Information Schema,
        // is always NULL.
        // The current workaround is to select directly from the system tables.
        // TODO: When the data type problems is fixed, check if selecting from Information Schema is faster.
        // If it is then switch back to selecting from Information Schema.
/*
        sql = FdoStringP::Format(
                    L"select %ls C.TABLE_NAME collate latin1_general_bin as table_name, C.COLUMN_NAME as name, 1 as type,\n"
                    L" size = CASE lower(C.DATA_TYPE) \n"
                    L"             WHEN 'text' THEN 2147483647 \n"
                    L"             WHEN 'ntext' THEN 1073741823 \n"
                    L"             WHEN 'uniqueidentifier' THEN 36 \n"
                    L"             ELSE isnull(C.CHARACTER_MAXIMUM_LENGTH,C.NUMERIC_PRECISION) \n"
                    L"         END,                   \n"
                    L" C.NUMERIC_SCALE as scale,\n"
                    L" nullable = CASE C.IS_NULLABLE \n"
                    L"             WHEN 'YES' THEN 1 \n"
                    L"             WHEN 'NO'  THEN 0 \n"
                    L"         END,                   \n"
                    L" lower(C.DATA_TYPE) as type_string,\n"
                    L" 0 as isunsigned, \n"
                    L" COLUMNPROPERTY(OBJECT_ID(C.TABLE_NAME), C.COLUMN_NAME, 'IsComputed') as has_computed_expression, \n"
                    L" COLUMNPROPERTY(OBJECT_ID(C.TABLE_NAME), C.COLUMN_NAME, 'IsIdentity') as is_autoincremented, \n"
                    L" C.TABLE_SCHEMA collate latin1_general_bin as table_schema, C.ORDINAL_POSITION as ordinal_position,\n"
					L" C.COLUMN_DEFAULT as default_value \n"
                    L" from %ls.INFORMATION_SCHEMA.COLUMNS C%ls\n"
                    L" where \n"
                    L"%ls\n"
                    L" order by C.TABLE_SCHEMA collate latin1_general_bin asc, C.TABLE_NAME collate latin1_general_bin asc, C.ORDINAL_POSITION asc",
                 join ? L"distinct" : L"",
                (FdoString *)ownerName,
                (FdoString *)joinFrom,
                (FdoString *)qualification
              );
*/
        sql = FdoStringP::Format(
                    L"select %ls a.name collate latin1_general_bin as table_name, b.name as name, 1 as type,\n"
                    L" size = CASE lower(c.name) \n"
                    L"             WHEN 'text' THEN 2147483647 \n"
                    L"             WHEN 'ntext' THEN 1073741823 \n"
                    L"             WHEN 'nchar' THEN (b.max_length / 2) \n"
                    L"             WHEN 'nvarchar' THEN (CASE b.max_length WHEN -1 THEN 1073741823 ELSE (b.max_length / 2) END) \n"
                    L"             WHEN 'uniqueidentifier' THEN 36 \n"
                    L"             WHEN 'decimal' THEN b.precision \n"
                    L"             WHEN 'money' THEN b.precision \n"
                    L"             WHEN 'numeric' THEN b.precision \n"
                    L"             WHEN 'smallmoney' THEN b.precision \n"
                    L"             WHEN 'varchar' THEN (CASE b.max_length WHEN -1 THEN 2147483647 ELSE (b.max_length) END) \n"
                    L"             ELSE b.max_length \n"
                    L"         END,                   \n"
                    L" b.scale as scale,\n"
                    L" b.is_nullable as nullable, \n"
                    L" lower(c.name) as type_string,\n"
                    L" 0 as isunsigned, \n"
                    L" COLUMNPROPERTY(a.object_id, b.name, 'IsComputed') as has_computed_expression, \n"
                    L" COLUMNPROPERTY(a.object_id, b.name, 'IsIdentity') as is_autoincremented, \n"
                    L" d.name collate latin1_general_bin as table_schema, b.object_id as ordinal_position,\n"
					L" e.definition as default_value, \n"
					L" b.column_id as position \n"
                    L" from %ls.sys.objects  a\n"
                    L"  INNER JOIN %ls.sys.columns b ON ( a.object_id = b.object_id ) \n"
                    L"  INNER JOIN %ls.sys.types  c ON ( b.user_type_id = c.user_type_id ) \n"
                    L"  INNER JOIN %ls.sys.schemas d ON ( a.schema_id = d.schema_id ) \n"
                    L" %ls\n"
                    L"  LEFT OUTER JOIN %ls.sys.default_constraints e ON ( b.default_object_id = e.object_id) \n"
                    L" %ls %ls\n"
                    L" order by d.name collate latin1_general_bin asc, a.name collate latin1_general_bin asc, b.column_id asc",
                 join ? L"distinct" : L"",
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)ownerName,
                (FdoString *)joinFrom,
                (FdoString *)ownerName,
                (qualification == L"") ? L"" : L"where",
                (FdoString *)qualification
              );

        rows = MakeRows (mgr);
        row = rows->GetItem (0);

        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"type_string",
            row->CreateColumnDbObject(L"type_string",false)
        );

        // Retrieve whether column is unsigned. This affects the valid
        // value range and thus the corresponding FDO property type for the column.
        field = new FdoSmPhField(
            row, 
            L"isunsigned",
            row->CreateColumnInt64(L"isunsigned",false)
        );

        field = new FdoSmPhField(
            row, 
            L"has_computed_expression",
            row->CreateColumnInt32(L"has_computed_expression",true)
        );

        field = new FdoSmPhField(
            row, 
            L"table_schema",
            row->CreateColumnDbObject(L"table_schema",false)
        );

        reader = new FdoSmPhRdGrdQueryReader (row, sql, mgr, binds->GetBinds());
    }
    else {
        // Re-executing so update bind variables first.
        pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        FdoSmPhRowP bindRow = pReader->GetBinds ();

        FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
            mgr,
            L"C.TABLE_SCHEMA",
            L"user_name",
            L"C.TABLE_NAME",
            L"object_name",
            objectNames,
            bindRow,
            true
        );

        pReader->Execute();
    }

    return reader;
}

FdoSmPhColType FdoSmPhRdSqsColumnReader::GetType()
{
    CheckGet();

    return mColType;
}

FdoStringsP FdoSmPhRdSqsColumnReader::DbObject2Objects( FdoSmPhDbObjectP dbObject )
{
    FdoStringsP tableNames = FdoStringCollection::Create();
    if ( dbObject ) 
        tableNames->Add( dbObject->GetName() );

    return tableNames;
}
