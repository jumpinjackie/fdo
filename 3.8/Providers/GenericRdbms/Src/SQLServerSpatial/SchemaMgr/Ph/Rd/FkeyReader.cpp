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
#include "FkeyReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqsFkeyReader::FdoSmPhRdSqsFkeyReader(
    FdoSmPhOwnerP owner,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdFkeyReader((FdoSmPhReader*) NULL),
    mDbObject(dbObject)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObject->GetName());
    SetSubReader(MakeReader(owner->GetManager(), owner, objectNames));
}

FdoSmPhRdSqsFkeyReader::FdoSmPhRdSqsFkeyReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdFkeyReader((FdoSmPhReader*) NULL)
{
    SetSubReader(MakeReader(owner->GetManager(), owner, objectNames));
}

FdoSmPhRdSqsFkeyReader::FdoSmPhRdSqsFkeyReader(
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdFkeyReader((FdoSmPhReader*) NULL)
{
    FdoStringsP objectNames = FdoStringCollection::Create();

    SetSubReader(MakeReader(owner->GetManager(), owner, objectNames));
}

FdoSmPhRdSqsFkeyReader::FdoSmPhRdSqsFkeyReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdFkeyReader((FdoSmPhReader*) NULL)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeReader(owner->GetManager(), owner,objectNames,join));
}

FdoSmPhRdSqsFkeyReader::~FdoSmPhRdSqsFkeyReader(void)
{
	EndSelect();
}

FdoStringP FdoSmPhRdSqsFkeyReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdFkeyReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdFkeyReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else if ( fieldName == L"r_table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdFkeyReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdFkeyReader::GetString( tableName, L"r_table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdFkeyReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhReaderP FdoSmPhRdSqsFkeyReader::MakeReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP sql = FdoStringP::Format(
	    L"select F.name as constraint_name,\n" 
        L" T.name as table_name,\n"
        L" TC.name as column_name,\n"
	    L" %s as r_owner_name,\n"
	    L" R.name as r_table_name,\n"
	    L" RC.name as r_column_name,\n"
	    L" TS.name as table_schema,\n"
        L" RS.name as r_table_schema, \n"
        L" FC.constraint_column_id as position \n"
	    L" from %ls.sys.foreign_keys F, \n"
	    L" %ls.sys.objects T, %ls.sys.objects R, \n"
	    L" %ls.sys.schemas TS, %ls.sys.schemas RS, \n"
	    L" %ls.sys.foreign_key_columns FC, \n"
	    L" %ls.sys.columns TC, %ls.sys.columns RC \n"
        L" $(JOIN_FROM) \n"
	    L" where F.parent_object_id = T.object_id\n"
	    L"   and T.schema_id = TS.schema_id\n"
	    L"   and F.referenced_object_id = R.object_id\n"
	    L"   and R.schema_id = RS.schema_id\n"
	    L"   and F.object_id = FC.constraint_object_id\n"
	    L"   and FC.parent_object_id = TC.object_id\n"
	    L"   and FC.parent_column_id = TC.column_id\n"
	    L"   and FC.referenced_object_id = RC.object_id\n"
	    L"   and FC.referenced_column_id = RC.column_id\n"
	    L"   $(AND) $(QUALIFICATION)"
	    L" order by TS.name collate latin1_general_bin asc, T.name collate latin1_general_bin asc, F.name collate latin1_general_bin asc, FC.constraint_column_id asc",
         (FdoString*)(mgr->FormatSQLVal(owner->GetName(), FdoSmPhColType_String)),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName()),
         (FdoString*)(owner->GetDbName())
    );

    FdoSmPhReaderP reader = FdoSmPhRdFkeyReader::MakeQueryReader(
        L"",
        owner,
        sql,
        L"TS.name",
        L"T.name",
        objectNames,
        join
    );

    return( reader );
}

FdoSmPhRowsP FdoSmPhRdSqsFkeyReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = FdoSmPhRdFkeyReader::MakeRows( mgr );

    // Single row, no joins
    FdoSmPhRowP row = rows->GetItem(0);
    
    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    field = new FdoSmPhField(row, L"r_table_schema", row->CreateColumnDbObject(L"r_table_schema", false));
    
    return( rows );
}

