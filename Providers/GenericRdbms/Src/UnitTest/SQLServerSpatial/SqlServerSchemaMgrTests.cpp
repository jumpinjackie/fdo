/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "Pch.h"
#include "SqlServerSchemaMgrTests.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "SqlServerConnectionUtil.h"
#include "../../SQLServerSpatial/SchemaMgr/Ph/Mgr.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerSchemaMgrTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerSchemaMgrTests, "SchemaMgrTests");

StaticConnection* SqlServerSchemaMgrTests::CreateStaticConnection()
{
    return new SqlServerStaticConnection();
}

FdoIoStream* SqlServerSchemaMgrTests::OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix )
{
    FdoIoMemoryStream* stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::OverrideBend( stream1, stream2, 
        "Autodesk.SqlServer.3.3", 
        "http://www.autodesk.com/isd/fdo/SQLServerProvider",
        oldOwnerPrefix,newOwnerPrefix);

    return stream2;
}

void SqlServerSchemaMgrTests::AddProviderColumns( FdoSmPhTableP table )
{

    table->CreateColumnUnknown(
        L"BINARY_COLUMN",
        L"binary",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"VARBINARY_COLUMN",
        L"varbinary",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"SMALLDATETIME_COLUMN",
        L"smalldatetime",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"TIMESTAMP_COLUMN",
        L"timestamp",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"NUMERIC_COLUMN",
        L"numeric",
        true,
        10,
        5
    );

    table->CreateColumnUnknown(
        L"MONEY_COLUMN",
        L"money",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"SMALLMONEY_COLUMN",
        L"smallmoney",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"REAL_COLUMN",
        L"real",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"NVARCHAR_COLUMN",
        L"nvarchar",
        true,
        50,
        0
    );

    table->CreateColumnUnknown(
        L"INT_COLUMN",
        L"int",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"TINYINT_COLUMN",
        L"tinyint",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"CHAR1_COLUMN",
        L"char",
        true,
        1,
        0
    );

    table->CreateColumnUnknown(
        L"CHAR5_COLUMN",
        L"char",
        true,
        5,
        0
    );

    table->CreateColumnUnknown(
        L"NCHAR_COLUMN",
        L"nchar",
        true,
        10,
        0
    );

    table->CreateColumnUnknown(
        L"TEXT_COLUMN",
        L"text",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"NTEXT_COLUMN",
        L"ntext",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"UNIQUEID_COLUMN",
        L"uniqueidentifier",
        true,
        0,
        0
    );

    table->CreateColumnChar(
        L"LONG_STRING",
        true,
        5000
    );

/*
TODO    table->CreateColumnUnknown(
        L"UNKNOWN_COLUMN",
        ?,
        true,
        0,
        0
    );
*/

    FdoSmPhSqsMgrP sqsMgr = table->GetManager()->SmartCast<FdoSmPhSqsMgr>();
 
    if ( sqsMgr->GetDbVersion().Left( L"." ).ToLong() > 8 ) {
        FdoInt32 ix;
        FdoSmPhOwnerP owner = FDO_SAFE_ADDREF((FdoSmPhOwner*) table->GetParent());
 
        for ( ix = 1; ix <= 5; ix++ ) {
            FdoStringP vTableName = FdoStringP::Format(L"guest.vertable%d",ix);
            FdoStringP vlTableName = FdoStringP::Format(L"guest.vertablelookup%d",ix);
            FdoStringP vlbTableName = FdoStringP::Format(L"guest.vertablelookupb%d",ix);

            FdoStringP fkeyName = FdoStringP::Format(L"guest.vertable_vertablelookup%d",ix);;
            FdoStringP fkeyBName = FdoStringP::Format(L"guest.vertable_vertablelookupb%d",ix);;

            FdoSmPhTableP vtable = owner->CreateTable( vTableName );
            FdoSmPhColumnP column = vtable->CreateColumnInt32( L"id1", false );
            vtable->AddPkeyCol( column->GetName() );
            FdoSmPhColumnP fkeyColumn = vtable->CreateColumnInt32( L"id2", false );
            FdoSmPhColumnP fkeyB1Column = vtable->CreateColumnInt32( L"idb1", false );
            FdoSmPhColumnP fkeyB2Column = vtable->CreateColumnInt32( L"idb2", false );
            column = vtable->CreateColumnChar( L"name", false, 50 );

            FdoStringP consClause = FdoStringP::Format( L"id1 < %d", ix );
            FdoSmPhCheckConstraintP constraint = new FdoSmPhCheckConstraint( L"id1_check", L"id1", consClause );
            vtable->AddCkeyCol( constraint );

            FdoSmPhColumnsP	ukeyColumns;

            if ( ix > 3 ) {
                ukeyColumns = new FdoSmPhColumnCollection();
	            vtable->GetUkeyColumns()->Add( ukeyColumns );
                vtable->AddUkeyCol( 0, L"id2" );
                vtable->AddUkeyCol( 0, L"idb1" );
                vtable->AddUkeyCol( 0, L"idb2" );

                ukeyColumns = new FdoSmPhColumnCollection();
	            vtable->GetUkeyColumns()->Add( ukeyColumns );
                vtable->AddUkeyCol( 1, L"name" );
            }
            else {
                ukeyColumns = new FdoSmPhColumnCollection();
	            vtable->GetUkeyColumns()->Add( ukeyColumns );
                vtable->AddUkeyCol( 0, L"name" );
            }

            FdoSmPhTableP vltable = owner->CreateTable( vlTableName );
            column = vltable->CreateColumnInt32( L"id2", false );
            vltable->AddPkeyCol( column->GetName() );
            column = vltable->CreateColumnChar( L"name", false, 50 );

            ukeyColumns = new FdoSmPhColumnCollection();
	        vltable->GetUkeyColumns()->Add( ukeyColumns );
            vltable->AddUkeyCol( 0, L"name" );

            FdoSmPhTableP vlbtable = owner->CreateTable( vlbTableName );
            column = vlbtable->CreateColumnInt32( L"id1", false );
            vlbtable->AddPkeyCol( column->GetName() );
            column = vlbtable->CreateColumnInt32( L"id2", false );
            vlbtable->AddPkeyCol( column->GetName() );
            column = vlbtable->CreateColumnChar( L"name", false, 50 );

            FdoSmPhFkeyP fkey = vtable->CreateFkey( fkeyName, vlTableName );
            fkey->AddFkeyColumn( fkeyColumn, L"id2" );

            fkey = vtable->CreateFkey( fkeyBName, vlbTableName );
            fkey->AddFkeyColumn( fkeyB1Column, L"id1" );
            fkey->AddFkeyColumn( fkeyB2Column, L"id2" );
        }
    }
}

FdoStringP SqlServerSchemaMgrTests::table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName )
{
    return mgr->GetDcDbObjectName( tableName ).Right(L".");
}

bool SqlServerSchemaMgrTests::SupportsBaseObjects()
{
    return true;
}

