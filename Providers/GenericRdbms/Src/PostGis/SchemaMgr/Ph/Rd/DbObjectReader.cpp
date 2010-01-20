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
#include "DbObjectReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Owner.h"

FdoSmPhRdPostGisDbObjectReader::FdoSmPhRdPostGisDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName)
    : FdoSmPhRdDbObjectReader(NULL, owner, objectName)
{
    SetSubReader(MakeQueryReader(owner, objectName));
}

FdoSmPhRdPostGisDbObjectReader::FdoSmPhRdPostGisDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join)
    : FdoSmPhRdDbObjectReader(NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner, L"", join));
}

FdoSmPhRdPostGisDbObjectReader::~FdoSmPhRdPostGisDbObjectReader()
{
    // idle
}

FdoSmPhDbObjType FdoSmPhRdPostGisDbObjectReader::GetType()
{
    // Default value returned if type not recognized.
    FdoSmPhDbObjType retType = FdoSmPhDbObjType_Unknown;

    FdoStringP type;
    type = GetString(L"", L"type");

    if (type == L"base table")
        retType = FdoSmPhDbObjType_Table;
    else if (type == L"view")
        retType = FdoSmPhDbObjType_View;

    return retType;
}

//FdoStringP FdoSmPhRdPostGisDbObjectReader::GetString( FdoStringP tableName, FdoStringP fieldName )
//{
//    FdoStringP fieldValue;
//
//    if ( fieldName == L"name" ) {
//        FdoStringP dbObjectName = FdoSmPhRdDbObjectReader::GetString( tableName, fieldName ); 
//        FdoStringP userName = FdoSmPhRdDbObjectReader::GetString( tableName, L"schema" ); 
//
//        fieldValue = userName + L"." + dbObjectName;        
//    }
//    else {
//        fieldValue = FdoSmPhRdDbObjectReader::GetString( tableName, fieldName );        
//    }
//
//    return fieldValue;
//}


FdoSmPhReaderP FdoSmPhRdPostGisDbObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName,
    FdoSmPhRdTableJoinP join)
{
    bool isDblinkSet = true;
    bool isObjectSet = true;

    FdoStringP sqlString;
    FdoStringP ownerName(owner->GetName());
    FdoStringP dblinkName(owner->GetParent()->GetName());

    FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = static_cast<FdoSmPhPostGisOwner*>(owner.p);

    FdoStringP tablesTableName(pgOwner->GetTablesTable());

    FdoSmPhMgrP mgr(owner->GetManager());
    
    FdoSmPhPostGisMgr* pgMgr = NULL;
    pgMgr = static_cast<FdoSmPhPostGisMgr*>(mgr.p);

    if (0 == dblinkName.GetLength())
        isDblinkSet = false;

    if (0 == objectName.GetLength())
        isObjectSet = false;

    FdoSmPhReaderP reader;

    if (!reader)
    {
        // Generate SQL statement if not already done

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        if ((NULL != join) && (0 == objectName.GetLength()))
        {
            joinFrom = FdoStringP::Format(L", %ls ",
                static_cast<FdoString*>(join->GetFrom()));
        }

        FdoStringP qualification;

        if (objectName.GetLength() > 0)
        {
            // Selecting single object, qualify by this object.
            // $2 - qualification takes a second place in the SQL query
            qualification = L" AND t.table_name = $1 ";
        } 
        else
        {
            if (NULL != join)
            {
                // Otherwise, if joining to another table, generated join clause.
                qualification = FdoStringP::Format(L" AND (%ls) ",
                    static_cast<FdoString*>(join->GetWhere(L"t.table_name")));
            }
        }

        // template_postgis=# \d information_schema.tables
        //                        View "information_schema.tables"
        //        Column        |      Type      |
        // ---------------------+----------------+
        //  table_catalog       | sql_identifier |
        //  table_schema        | sql_identifier |
        //  table_name          | sql_identifier |
        //  table_type          | character_data |
        //  is_insertable_into  | character_data | <- Always YES for BASE TABLE
        //  commit_action       | character_data | <- NULL, for temp tables is PRESERVE 


        // Names of PostgreSQL objects: database, table, field and columns
        // are case-independent, unless you created them with double-quotes
        // around their name, in which case they are case-sensitive.
        // NOTE: In MySQL, table names can be case-sensitive or not,
        //       depending on which operating system you are using.
        //       On Linux, names are case-sensitive.

        // TODO: mloskot - Remove unused fields.

        sqlString = FdoStringP::Format(
            L" SELECT %ls t.table_schema  || '.' || t.table_name AS name, "
            //L" SELECT %ls t.table_name AS name, t.table_schema AS schema, "
            L" lower(t.table_type) AS type, "
            L" %ls as collate_schema_name, "
            L" %ls as collate_table_name "
            L" FROM %ls AS t %ls "
            L" WHERE t.table_schema not in ( 'information_schema' ,'pg_catalog') "           
            L" %ls"
            L" AND t.table_type IN ('BASE TABLE','VIEW') "
            L" AND t.table_name not in ( 'geometry_columns' ,'spatial_ref_sys') "
            L" ORDER BY collate_schema_name, collate_table_name ASC ",
            (join ? L"distinct" : L""),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"t.table_schema"),
            (FdoString*) pgMgr->FormatCollateColumnSql(L"t.table_name"),
            static_cast<FdoString*>(tablesTableName),
            static_cast<FdoString*>(joinFrom),
            static_cast<FdoString*>(qualification));

        FdoSmPhRowsP rows = MakeRows(mgr);
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr,
            (objectName.GetLength() > 0)?MakeBinds(mgr, ownerName, objectName):NULL);
    }
    else
    {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* queryReader = NULL;
        queryReader = static_cast<FdoSmPhRdGrdQueryReader*>(reader.p);
        
        FdoSmPhRowP binds(queryReader->GetBinds());
		FdoSmPhFieldsP fields(binds->GetFields());

        FdoSmPhFieldP field(fields->GetItem(L"table_schema"));
        field->SetFieldValue(ownerName);

        if (isObjectSet)
        {
            FdoSmPhFieldP field(fields->GetItem(L"table_name"));
            field->SetFieldValue(objectName);
        }

        queryReader->Execute();
    }

    return reader;
}


FdoSmPhRowP FdoSmPhRdPostGisDbObjectReader::MakeBinds(FdoSmPhMgrP mgr,
    FdoStringP ownerName, FdoStringP objectName)
{
    FdoSmPhRowP row(new FdoSmPhRow(mgr, L"Binds"));
    FdoSmPhDbObjectP rowDbObj(row->GetDbObject());

    //FdoSmPhFieldP field(new FdoSmPhField(row, L"table_schema",
    //    rowDbObj->CreateColumnDbObject(L"table_schema", false)));

   // field->SetFieldValue(ownerName);

    if (objectName.GetLength() > 0)
    {
        FdoSmPhFieldP field = new FdoSmPhField(row, L"table_name",
            rowDbObj->CreateColumnDbObject(L"table_name", false));

        field->SetFieldValue(objectName);
    }

    return row;
}
/*
FdoSmPhRowsP FdoSmPhRdPostGisDbObjectReader::MakeRows(FdoSmPhMgrP mgr)
{
    // Let superclass make its rows:
    FdoSmPhRowsP rows(FdoSmPhRdDbObjectReader::MakeRows(mgr));
    FdoSmPhRowP row(rows->GetItem(0));

    // Add our own PostGIS-specific rows:
    

    FdoSmPhFieldP field = new FdoSmPhField(row, L"schema", 
        row->CreateColumnDbObject(L"schema", false));

    return rows;
}
*/