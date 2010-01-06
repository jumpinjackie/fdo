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
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdFkeyReader(MakeReader(mgr, (const FdoSmPhOwner*) (dbObject->GetParent()), dbObject)),
    mDbObject(dbObject)
{
}

FdoSmPhRdSqsFkeyReader::FdoSmPhRdSqsFkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdFkeyReader(MakeReader(mgr, (FdoSmPhOwner*) owner, (FdoSmPhDbObject*) NULL))
{
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
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoStringP objectName = dbObject ? dbObject->GetName() : L"";

    FdoStringP ownerName = owner->GetName();

    FdoSmPhSqsMgrP sqsMgr = mgr->SmartCast<FdoSmPhSqsMgr>();

    FdoStringP readerName = FdoStringP::Format(L"FkeySqsReader_%ls_%ls", (FdoString*) ownerName, 
												     dbObject ? L"O" : L"");
    FdoSmPhReaderP reader = mgr->GetStaticReader( readerName );

    if ( !reader )	{


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
	    // Generate SQL statement to get foreign keys
	    FdoStringP sql = FdoStringP::Format(
		    L"select R.CONSTRAINT_NAME as constraint_name,\n" 
            L" K1.TABLE_NAME as table_name,\n"
            L" K1.COLUMN_NAME as column_name,\n"
		    L" R.UNIQUE_CONSTRAINT_CATALOG as r_owner_name,\n"
		    L" K2.TABLE_NAME as r_table_name,\n"
		    L" K2.COLUMN_NAME as r_column_name,\n"
		    L" K1.TABLE_SCHEMA as table_schema,\n"
            L" K2.TABLE_SCHEMA as r_table_schema\n"
		    L" from %ls.INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS R,\n"
		    L" %ls.INFORMATION_SCHEMA.KEY_COLUMN_USAGE K1,\n"
		    L" %ls.INFORMATION_SCHEMA.KEY_COLUMN_USAGE K2\n"
		    L" where R.CONSTRAINT_CATALOG = K1.CONSTRAINT_CATALOG\n"
		    L"   and R.CONSTRAINT_SCHEMA = K1.CONSTRAINT_SCHEMA\n"
		    L"   and R.CONSTRAINT_NAME = K1.CONSTRAINT_NAME\n"
		    L"   and R.UNIQUE_CONSTRAINT_CATALOG = K2.CONSTRAINT_CATALOG\n"
		    L"   and R.UNIQUE_CONSTRAINT_SCHEMA = K2.CONSTRAINT_SCHEMA\n"
		    L"   and R.UNIQUE_CONSTRAINT_NAME = K2.CONSTRAINT_NAME\n"
		    L"   and K1.ORDINAL_POSITION = K2.ORDINAL_POSITION\n"
		    L"   and R.CONSTRAINT_CATALOG = ?\n"
		    L"%ls"
		    L" order by K1.TABLE_SCHEMA collate latin1_general_bin asc, K1.TABLE_NAME collate latin1_general_bin asc, R.CONSTRAINT_NAME collate latin1_general_bin asc, K1.ORDINAL_POSITION asc",
		     (FdoString*) ownerName,
		     (FdoString*) ownerName,
		     (FdoString*) ownerName,
		     dbObject ? L"   and K1.TABLE_SCHEMA = ? and K1.TABLE_NAME = ?\n" : L""
	    );

	    // Create a field object for each field in the select list
	    FdoSmPhRowsP rows = MakeRows(mgr);
	    FdoSmPhRowP row = rows->GetItem(0);
	    FdoSmPhFieldP pField = new FdoSmPhField(row, L"table_schema", row->CreateColumnDbObject(L"table_schema", false));
	    pField = new FdoSmPhField(row, L"r_table_schema", row->CreateColumnDbObject(L"r_table_schema", false));

        // Create and set the bind variables
	    FdoSmPhRowP binds = sqsMgr->MakeByDbObjectBinds(ownerName, objectName);
	    FdoSmPhDbObjectP rowObj = binds->GetDbObject();

	    reader = new FdoSmPhRdGrdQueryReader( row, sql, mgr, binds );
	    /*if ( reader )
		    mgr->SetStaticReader( readerName, reader );*/
    }
    else	{
	    // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        FdoSmPhRowP binds = pReader->GetBinds();
	    sqsMgr->SetByDbObjectBinds( binds, ownerName, objectName);
	    pReader->Execute();
	}

    return( reader );
}
