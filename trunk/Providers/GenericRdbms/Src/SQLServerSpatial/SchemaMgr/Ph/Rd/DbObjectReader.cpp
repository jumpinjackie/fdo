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
#include "DbObjectReader.h"
#include "DbObjectBinds.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqsDbObjectReader::FdoSmPhRdSqsDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName
) :
    FdoSmPhRdDbObjectReader(NULL, owner, objectName)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    if ( objectName != L"" ) 
        objectNames->Add(objectName);

    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsDbObjectReader::FdoSmPhRdSqsDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdDbObjectReader(NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsDbObjectReader::FdoSmPhRdSqsDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*) NULL, owner, L"")
{
    FdoStringsP objectNames = FdoStringCollection::Create();

    SetSubReader(MakeQueryReader(owner,objectNames,join));
}

FdoSmPhRdSqsDbObjectReader::~FdoSmPhRdSqsDbObjectReader(void)
{
}

FdoSmPhDbObjType FdoSmPhRdSqsDbObjectReader::GetType()
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

FdoStringP FdoSmPhRdSqsDbObjectReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"name" ) {
        FdoStringP dbObjectName = FdoSmPhRdDbObjectReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdDbObjectReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdDbObjectReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhRowsP FdoSmPhRdSqsDbObjectReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Call superclass to populate generic row/columns:
    FdoSmPhRowsP rows = FdoSmPhRdDbObjectReader::MakeRows(mgr);
        
    FdoSmPhRowP row = rows->GetItem(L"DbObjectFields");

    // Each field adds itself to the row.
    FdoSmPhFieldP pField;
    pField = new FdoSmPhField(row, L"table_schema", row->CreateColumnDbObject(L"table_schema", false));
    pField = new FdoSmPhField(row, L"tablefilegroup_name", row->CreateColumnDbObject(L"tablefilegroup_name", false));
    pField = new FdoSmPhField(row, L"textfilegroup_name", row->CreateColumnDbObject(L"textfilegroup_name", false));
    pField = new FdoSmPhField(row, L"idcolumn_name", row->CreateColumnDbObject(L"idcolumn_name", false));
    pField = new FdoSmPhField(row, L"idcolumn_seed", row->CreateColumnDbObject(L"idcolumn_seed", false));
    pField = new FdoSmPhField(row, L"idcolumn_increment", row->CreateColumnDbObject(L"idcolumn_increment", false));
    pField = new FdoSmPhField(row, L"idcolumn_isrowguid", row->CreateColumnDbObject(L"idcolumn_isrowguid", false));
    pField = new FdoSmPhField(row, L"textinrow", row->CreateColumnDbObject(L"textinrow", false));

    return( rows);
}


FdoSmPhReaderP FdoSmPhRdSqsDbObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP           sqlString;
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;

    FdoSmPhReaderP reader;

//TODO: cache the queries for performance
/*
    if ( object_set )
        reader = pMgr->GetDbObjectReader(dblink_set);
    else
        reader = pMgr->GetDbObjectsReader(dblink_set);
*/
    if ( !reader ) {
        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        if ( owner->GetElementState() == FdoSchemaElementState_Added ) {
            // The DbObject query selects from owner.INFORMATION_SCHEMA tables so
            // it will fail when the owner does not yet exist. In this case create
            // an empty reader that returns no rows.
            reader = new FdoSmPhReader( mgr, rows );
        }
        else {
            // Generate sql statement if not already done

            // Create binds for owner and optional object names
            FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                mgr,
                L"a.TABLE_SCHEMA",
                L"user_name",
                L"a.TABLE_NAME",
                L"object_name",
                objectNames
            );

            // If joining to another table, generated from sub-clause for table.
            FdoStringP joinFrom;
            if ( join != NULL ) 
                joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );

            // Get where clause for owner and object name binds.
            FdoStringP qualification = binds->GetSQL();

            if ( join != NULL ) {
                // If joining to another table, add join clause.
                qualification = FdoStringP::Format( 
                    L"  %ls and ((a.TABLE_SCHEMA = 'dbo' and a.TABLE_NAME = %ls) or ((a.TABLE_SCHEMA + '.' + a.TABLE_NAME) = %ls))\n", 
                    (FdoString*) ((FdoSmPhRdJoin*)(join.p))->GetWhere(),
                    (FdoString*) join->GetJoinColumn(),
                    (FdoString*) join->GetJoinColumn()
                );
            }

            //SqlServer> describe INFORMATION_SCHEMA.TABLES;
            //+-----------------+--------------+------+-----+---------+-------+
            //| Field           | Type         | Null | Key | Default | Extra |
            //+-----------------+--------------+------+-----+---------+-------+
            //| TABLE_CATALOG   | varchar(512) | YES  |     | NULL    |       |
            //| TABLE_SCHEMA    | varchar(64)  | NO   |     |         |       |
            //| TABLE_NAME      | varchar(64)  | NO   |     |         |       |
            //| TABLE_TYPE      | varchar(64)  | NO   |     |         |       |
            //+-----------------+--------------+------+-----+---------+-------+

            // b is the table's entry in sysindexes, where indid = 
            //  0 - when the table is stored as a heap
            //  1 - when the table is stored as a clustered index.
            //
            // c is the entry for the text, ntext or image column if the table has any.

            // When reading multiple database objects, a couple of system views are skipped 
            // (sysconstraints, syssegments). These seem to be only present for SqlServer 2000.

            sqlString = FdoStringP::Format(
                L"select %ls a.TABLE_NAME collate latin1_general_bin as name, lower(a.TABLE_TYPE) as type, a.TABLE_SCHEMA collate latin1_general_bin as table_schema, \n"
                L"  FILEGROUP_NAME(b.groupid) as tablefilegroup_name, \n"
                L"  FILEGROUP_NAME(c.groupid) as textfilegroup_name, \n"
                L"  d.name as idcolumn_name, IDENT_SEED(a.TABLE_NAME) as idcolumn_seed, IDENT_INCR(a.TABLE_NAME) as idcolumn_increment, \n"
                L"  COLUMNPROPERTY(b.id, d.name, 'IsRowGuidCol') as idcolumn_isrowguid, \n"
                L"  OBJECTPROPERTY(b.id, 'TableTextInRowLimit') as textinrow \n"
                L"  from %ls.INFORMATION_SCHEMA.TABLES a\n"
                L"  LEFT OUTER JOIN %ls.dbo.sysindexes b ON (b.indid in (0,1) and object_id(a.TABLE_CATALOG + '.' + a.TABLE_SCHEMA + '.' + a.TABLE_NAME) = b.id) \n"
                L"  LEFT OUTER JOIN %ls.dbo.sysindexes c ON (c.indid = 255 AND c.id = b.id) \n"
                L"  LEFT OUTER JOIN %ls.dbo.syscolumns d ON (b.id = d.id and COLUMNPROPERTY(d.id, d.name, 'IsIdentity') = 1) \n"
                L"%ls\n"
                L" where\n"
                L" %ls %ls %ls %ls\n"
                L" order by a.TABLE_SCHEMA collate latin1_general_bin asc, a.TABLE_NAME collate latin1_general_bin asc",
                join ? L"distinct" : L"",
                (FdoString*)ownerName,
                (FdoString*)ownerName,
                (FdoString*)ownerName,
                (FdoString*)ownerName,
                (FdoString*)joinFrom,
                L"a.TABLE_TYPE in ('BASE TABLE', 'VIEW')", //This was done because format string got too long.
                (qualification == L"") ? L"" : L" and ",
                (FdoString*) qualification,

                (objectNames->GetCount() == 1) ? L"" : L" and (TABLE_SCHEMA != 'dbo' or TABLE_NAME not in ( 'sysconstraints', 'syssegments')) "
            );

            reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );
    /*
            if ( object_set )
                pMgr->SetDbObjectReader(reader, dblink_set);
            else
                pMgr->SetDbObjectsReader(reader, dblink_set);
    */
        }
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = dynamic_cast<FdoSmPhRdGrdQueryReader*>((FdoSmPhReader*) reader);

        // pReader is NULL when it is an empty reader. No need to re-execute in this case.
        if ( pReader ) {
            FdoSmPhRowP bindRow = pReader->GetBinds();

            FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                mgr,
                L"a.TABLE_SCHEMA",
                L"user_name",
                L"a.TABLE_NAME",
                L"object_name",
                objectNames,
                bindRow,
                true
            );

            pReader->Execute();
        }
    }

    return reader;
}
