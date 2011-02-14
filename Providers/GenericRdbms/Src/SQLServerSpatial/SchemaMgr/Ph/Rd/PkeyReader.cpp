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
#include "PkeyReader.h"
#include "DbObjectBinds.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"


FdoSmPhRdSqsPkeyReader::FdoSmPhRdSqsPkeyReader(
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL),
    mDbObject(dbObject)
{
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF((FdoSmPhOwner*)(FdoSmPhDbElement*)(dbObject->GetParent()));

    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObject->GetName());

    SetSubReader(MakeReader(owner,objectNames));
}

FdoSmPhRdSqsPkeyReader::FdoSmPhRdSqsPkeyReader(
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL)
{
    FdoStringsP objectNames = FdoStringCollection::Create();

    SetSubReader(MakeReader(owner,objectNames));
}

FdoSmPhRdSqsPkeyReader::FdoSmPhRdSqsPkeyReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL)
{
    SetSubReader(MakeReader(owner,objectNames));
}

FdoSmPhRdSqsPkeyReader::FdoSmPhRdSqsPkeyReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL)
{
    FdoStringsP objectNames = FdoStringCollection::Create();

    SetSubReader(MakeReader(owner,objectNames,join));
}

FdoStringP FdoSmPhRdSqsPkeyReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdPkeyReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdPkeyReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdPkeyReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhRdSqsPkeyReader::~FdoSmPhRdSqsPkeyReader(void)
{
}

FdoSmPhReaderP FdoSmPhRdSqsPkeyReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP ownerName = owner->GetName();
    FdoSmPhMgrP mgr = owner->GetManager();

	FdoSmPhReaderP reader;
    FdoStringP readerName = FdoStringP::Format(
        L"PkeySqsReader_%ls_%ls", 
        (FdoString*) ownerName,
        objectNames->GetCount() > 0 ? (objectNames->GetCount() == 1 ? L"SO" : L"MO") : L"NO"
    );

    if ( !join )
    	FdoSmPhReaderP reader = mgr->GetStaticReader( readerName );

	if ( !reader )	{

        // Create binds for owner and optional object names
        FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
            mgr,
            L"s.name",
            L"user_name",
            L"t.name",
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
                L"%ls and ((s.name = 'dbo' and t.name = %ls) or ((s.name + '.' + t.name) = %ls))", 
                (FdoString*) ((FdoSmPhRdJoin*)(join.p))->GetWhere(),
                (FdoString*) join->GetJoinColumn(),
                (FdoString*) join->GetJoinColumn()
            );
        }

		// Generate SQL statement to get the primary key columns.
		//SqlServer> desc INFORMATION_SCHEMA.TABLE_CONSTRAINTS;
		//+--------------------+--------------+------+-----+---------+-------+
		//| Field              | Type         | Null | Key | Default | Extra |
		//+--------------------+--------------+------+-----+---------+-------+
		//| CONSTRAINT_CATALOG | varchar(512) | YES  |     | NULL    |       |
		//| CONSTRAINT_SCHEMA  | varchar(64)  | NO   |     |         |       |
		//| CONSTRAINT_NAME    | varchar(64)  | NO   |     |         |       |
		//| TABLE_SCHEMA       | varchar(64)  | NO   |     |         |       |
		//| TABLE_NAME         | varchar(64)  | NO   |     |         |       |
		//| CONSTRAINT_TYPE    | varchar(64)  | NO   |     |         |       |
		//+--------------------+--------------+------+-----+---------+-------+
		//SqlServer> desc INFORMATION_SCHEMA.KEY_COLUMN_USAGE;
		//+-------------------------------+--------------+------+-----+---------+-------+
		//| Field                         | Type         | Null | Key | Default | Extra |
		//+-------------------------------+--------------+------+-----+---------+-------+
		//| CONSTRAINT_CATALOG            | varchar(512) | YES  |     | NULL    |       |
		//| CONSTRAINT_SCHEMA             | varchar(64)  | NO   |     |         |       |
		//| CONSTRAINT_NAME               | varchar(64)  | NO   |     |         |       |
		//| TABLE_CATALOG                 | varchar(512) | YES  |     | NULL    |       |
		//| TABLE_SCHEMA                  | varchar(64)  | NO   |     |         |       |
		//| TABLE_NAME                    | varchar(64)  | NO   |     |         |       |
		//| COLUMN_NAME                   | varchar(64)  | NO   |     |         |       |
		//| ORDINAL_POSITION              | bigint(10)   | NO   |     | 0       |       |
		//| POSITION_IN_UNIQUE_CONSTRAINT | bigint(10)   | YES  |     | NULL    |       |
		//| REFERENCED_TABLE_SCHEMA       | varchar(64)  | YES  |     | NULL    |       |
		//| REFERENCED_TABLE_NAME         | varchar(64)  | YES  |     | NULL    |       |
		//| REFERENCED_COLUMN_NAME        | varchar(64)  | YES  |     | NULL    |       |
		//+-------------------------------+--------------+------+-----+---------+-------+
		// TODO: change back to bind variables.
        FdoStringP sqlString = FdoStringP::Format(
            L"select %ls ix.name collate latin1_general_bin as constraint_name, \n"
            L"   t.name collate latin1_general_bin as table_name, \n"
            L"   c.name as column_name, \n"
            L"   s.name collate latin1_general_bin as table_schema, \n"
            L"   ic.index_column_id as position \n"
            L"   from %ls.sys.objects t \n"
            L"   INNER JOIN %ls.sys.indexes ix on (ix.object_id = t.object_id)\n"
            L"   INNER JOIN %ls.sys.index_columns ic on (ix.object_id = ic.object_id and ix.index_id = ic.index_id)\n"
            L"   INNER JOIN %ls.sys.columns c on (ic.object_id = c.object_id and ic.column_id = c.column_id)\n"
            L"   INNER JOIN %ls.sys.schemas s on (t.schema_id = s.schema_id)\n"
            L"   %ls \n"
            L"   where is_primary_key = 1 %ls %ls \n"
            L"   order by s.name collate latin1_general_bin asc, t.name collate latin1_general_bin asc, ix.name collate latin1_general_bin asc, ic.index_column_id asc",
            join ? L"distinct" : L"",
            (FdoString*)(owner->GetDbName()),
            (FdoString*)(owner->GetDbName()),
            (FdoString*)(owner->GetDbName()),
            (FdoString*)(owner->GetDbName()),
            (FdoString*)(owner->GetDbName()),
            (FdoString *)joinFrom,
            (qualification == L"") ? L"" : L"and",
            (FdoString *)qualification
        );

		// Create a field object for each field in the select list.
		FdoSmPhRowsP rows = MakeRows(mgr);
		FdoSmPhRowP row = rows->GetItem(0);
		FdoSmPhFieldP pField = new FdoSmPhField(row, L"table_schema", row->CreateColumnDbObject(L"table_schema", false));

		//TODO: cache this query to make full use of the binds.
		reader = new FdoSmPhRdGrdQueryReader( row, sqlString, mgr, binds->GetBinds() );
		/*if ( reader && !join )
			mgr->SetStaticReader( readerName, reader );*/
	}
	else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = dynamic_cast<FdoSmPhRdGrdQueryReader*>((FdoSmPhReader*) reader);

        // pReader is NULL when it is an empty reader. No need to re-execute in this case.
        if ( pReader ) {
            FdoSmPhRowP bindRow = pReader->GetBinds();

            FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                mgr,
                L"tc.TABLE_SCHEMA",
                L"user_name",
                L"tc.TABLE_NAME",
                L"object_name",
                objectNames,
                bindRow,
                true
            );

            pReader->Execute();
        }
	}

    return( reader );
}
