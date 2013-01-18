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
#include "ViewRelObjectReader.h"
#include "DbObjectBinds.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqlViewRelationsObjectReader::FdoSmPhRdSqlViewRelationsObjectReader(
    FdoSmPhOwnerP owner
) :
    FdoSmPhRdViewRelationsObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqlViewRelationsObjectReader::FdoSmPhRdSqlViewRelationsObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdViewRelationsObjectReader((FdoSmPhReader*) NULL, owner)
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqlViewRelationsObjectReader::~FdoSmPhRdSqlViewRelationsObjectReader(void)
{
}

FdoStringP FdoSmPhRdSqlViewRelationsObjectReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"name" ) {
        FdoStringP dbObjectName = FdoSmPhRdViewRelationsObjectReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdViewRelationsObjectReader::GetString( tableName, L"view_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else if ( fieldName == L"base_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdViewRelationsObjectReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdViewRelationsObjectReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdViewRelationsObjectReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhRowsP FdoSmPhRdSqlViewRelationsObjectReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Call superclass to populate generic row/columns:
    FdoSmPhRowsP rows = FdoSmPhRdViewRelationsObjectReader::MakeRows(mgr);
        
    FdoSmPhRowP row = rows->GetItem(L"ViewRelObjectFields");

    // Each field adds itself to the row.
    FdoSmPhFieldP pField;
    pField = new FdoSmPhField(row, L"view_schema", row->CreateColumnDbObject(L"view_schema", false));
    pField = new FdoSmPhField(row, L"table_schema", row->CreateColumnDbObject(L"table_schema", false));

    return( rows);
}

FdoSmPhReaderP FdoSmPhRdSqlViewRelationsObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
)
{
    FdoStringP           sqlString;
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;

    FdoSmPhReaderP reader;

    if ( !reader ) {
        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        if ( owner->GetElementState() == FdoSchemaElementState_Added ) {
            // The BaseObject query selects from owner.INFORMATION_SCHEMA tables so
            // it will fail when the owner does not yet exist. In this case create
            // an empty reader that returns no rows.
            reader = new FdoSmPhReader( mgr, rows );
        }
        else {
            // Generate sql statement if not already done

            // Create binds for owner and optional object names
            FdoSmPhRdSqsDbObjectBindsP binds;
            FdoStringP qualification;
            if (objectNames)
            {
                binds = new FdoSmPhRdSqsDbObjectBinds(mgr, L"f.view_schema", L"user_name", L"f.view_name", L"name", objectNames);
                // Get where clause for owner and object name binds.
                qualification = binds->GetSQL();
            }

            FdoStringP dbName = owner->GetDbName();
            sqlString = FdoStringP::Format(
                L"select  f.view_name collate latin1_general_bin as name, a.name collate latin1_general_bin as base_name, b.name as column_name,\n"
                L" f.view_schema collate latin1_general_bin as view_schema, d.name collate latin1_general_bin as table_schema\n"
	            L" from %ls.sys.objects  a \n"
                L" INNER JOIN %ls.sys.columns b ON ( a.object_id = b.object_id )\n"
                L" INNER JOIN %ls.sys.types  c ON ( b.user_type_id = c.user_type_id )\n"
                L" INNER JOIN %ls.sys.schemas d ON ( a.schema_id = d.schema_id )\n"
                L" INNER JOIN %ls.INFORMATION_SCHEMA.VIEW_COLUMN_USAGE f ON ( d.name = f.table_schema and a.name = f.table_name and b.name = f.column_name)\n"
	            L" where a.type in ('U', 'V','SN') %ls %ls\n"
	            L" order by f.view_schema collate latin1_general_bin asc, f.view_name collate latin1_general_bin asc, d.name collate latin1_general_bin asc, a.name collate latin1_general_bin asc",
                (FdoString*)dbName,
                (FdoString*)dbName,
                (FdoString*)dbName,
                (FdoString*)dbName,
                (FdoString*)dbName,
                (binds) ? L" and " : L"",
                (qualification != NULL) ? (FdoString*)qualification : L""
            );

            reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds ? binds->GetBinds() : FdoSmPhRowP() );
        }
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = dynamic_cast<FdoSmPhRdGrdQueryReader*>((FdoSmPhReader*) reader);

        // pReader is NULL when it is an empty reader. No need to re-execute in this case.
        if ( pReader ) {
            FdoSmPhRowP bindRow = pReader->GetBinds();

            if (objectNames)
            {
                FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                    mgr,
                    L"f.view_schema",
                    L"user_name",
                    L"f.view_name",
                    L"name",
                    objectNames,
                    bindRow,
                    true
                );
            }

            pReader->Execute();
        }
    }

    return reader;
}
