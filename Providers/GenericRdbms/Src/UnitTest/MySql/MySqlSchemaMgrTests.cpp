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
#include "MySqlSchemaMgrTests.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "MySqlConnectionUtil.h"
#include "../MySQL/SchemaMgr/Ph/Table.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlSchemaMgrTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlSchemaMgrTests, "SchemaMgrTests");

#define   DB_NAME_SUFFIX            "_schema_mgr"

StaticConnection* MySqlSchemaMgrTests::CreateStaticConnection()
{
    return new MySqlStaticConnection();
}

FdoIoStream* MySqlSchemaMgrTests::OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix )
{
    FdoIoMemoryStream* stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::OverrideBend( 
        stream1, 
        stream2, 
        "OSGeo.MySQL.3.2", 
        "http://fdomysql.osgeo.org/schemas",
        oldOwnerPrefix,
        newOwnerPrefix
    );

    return stream2;
}

// Tests the creating of unique constraints that are too large for 
// MySQL. Verifies that the MySQL provider properly truncates
// constraint columns to get the total size within the maximum allowed.
void MySqlSchemaMgrTests::testWideConstraint ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;
    FdoInt32 ix;

    try
    {
        char prvenv[100];
        FdoStringP providerName = conn->GetServiceName();
        sprintf( prvenv, "provider=%ls", (FdoString*) providerName );
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        printf( "\nOpening Connection ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        printf( "Predeleting schema ...\n" );

        FdoStringP datastore = phMgr->GetDcOwnerName(
            FdoStringP::Format(
                L"%hs",
                UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
            )
        );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        printf( "Creating schema ...\n" );

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );

        // Table1 tests various column types and constraints 
        // with various numbers and sizes of columns.
        // Primary Keys, indexes and unique constraints are tested.

        FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE1" ));
        FdoSmPhColumnP column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );

        // Columns of various types (tested by adding to primary key) 
        column = table->CreateColumnBLOB( L"BLOB_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnSingle( L"SINGLE_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDouble( L"DOUBLE_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL_COLUMN", true, 65, 0 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnBool( L"BOOL_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnByte( L"BYTE_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnInt16( L"INT16_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnInt32( L"INT32_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnInt64( L"INT64_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDate( L"DATE_COLUMN", true );
        table->AddPkeyCol( column->GetName() );
        AddProviderColumns( table );
        column = table->GetColumns()->GetItem( L"BINARY_COLUMN" );
        table->AddPkeyCol( column->GetName() );
        column = table->GetColumns()->GetItem( L"MEDIUMBLOB_COLUMN" );
        column = table->GetColumns()->GetItem( L"TINYBLOB_COLUMN" );
        column = table->GetColumns()->GetItem( L"VARBINARY_COLUMN" );
        table->AddPkeyCol( column->GetName() );
        column = table->GetColumns()->GetItem( L"DATE2_COLUMN" );
        table->AddPkeyCol( column->GetName() );
        column = table->GetColumns()->GetItem( L"TIMESTAMP_COLUMN" );
        table->AddPkeyCol( column->GetName() );

        // Columns of various length.

        column = table->CreateColumnChar( L"STRING100000", false, 100000 );
        column = table->CreateColumnChar( L"STRING2000", false, 2000 );
        column = table->CreateColumnChar( L"STRING700_1", false, 700 );
        column = table->CreateColumnChar( L"STRING700_2", false, 700 );

        for ( ix = 0; ix < 10; ix++ ) {
            column = table->CreateColumnChar( 
                FdoStringP::Format(L"STRING300_%d", ix + 1),
                false,
                300 
            );
        }

        for ( ix = 0; ix < 10; ix++ ) {
            column = table->CreateColumnChar( 
                FdoStringP::Format(L"STRING100_%d", ix + 1),
                false,
                100 
            );
        }

        for ( ix = 0; ix < 10; ix++ ) {
            column = table->CreateColumnChar( 
                FdoStringP::Format(L"STRING40_%d", ix + 1),
                false,
                40 
            );
        }

        // Various unique constraints

        // One large (will be truncated) column, one small (not truncated)
        FdoSmPhColumnsP	ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100000" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100_1" );

        // Two large, one small. 
        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING2000" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING700_1" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING300_1" );

        // Three large, two small
        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100000" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING2000" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING700_1" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100_3" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100_4" );

        // Constraint will maximum number of columns (16): 11 large, 5 small.
        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING2000" );

        for ( ix = 0; ix < 10; ix++ ) {
            table->AddUkeyCol( 
                table->GetUkeyColumns()->GetCount() - 1, 
                FdoStringP::Format(L"STRING300_%d", ix + 1)
            );
        }

        for ( ix = 0; ix < 4; ix++ ) {
            table->AddUkeyCol( 
                table->GetUkeyColumns()->GetCount() - 1, 
                FdoStringP::Format(L"STRING100_%d", ix + 1)
            );
        }

        for ( ix = 0; ix < 1; ix++ ) {
            table->AddUkeyCol( 
                table->GetUkeyColumns()->GetCount() - 1, 
                FdoStringP::Format(L"STRING40_%d", ix + 1)
            );
        }

        // No large columns, no truncation.
        // Tests columns whose size is over the initial truncation size but
        // under final truncation size.
        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100_3" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING100_4" );

        // No large columns, no truncation.
        // Tests columns whose size is under the initial truncation size.
        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING40_3" );
        table->AddUkeyCol( table->GetUkeyColumns()->GetCount() - 1, L"STRING40_4" );

        // Test an index.
        FdoSmPhIndexP index = table->CreateIndex(L"table1_ix1", true );
        index->GetColumns()->Add( table->GetColumns()->GetItem(L"STRING2000") );
        index->GetColumns()->Add( table->GetColumns()->GetItem(L"STRING700_1") );

        FdoStringP pkeyClause = table->GetAddPkeySql();

        // Verify that blob column got truncated to expected amount
        // (1000 - total size of other columns).
        CPPUNIT_ASSERT( pkeyClause == L"constraint \"pk_table1\" primary key ( \"ID\", \"BLOB_COLUMN\"(630), \"STRING_COLUMN\", \"SINGLE_COLUMN\", \"DOUBLE_COLUMN\", \"DECIMAL_COLUMN\", \"BOOL_COLUMN\", \"BYTE_COLUMN\", \"INT16_COLUMN\", \"INT32_COLUMN\", \"INT64_COLUMN\", \"DATE_COLUMN\", \"BINARY_COLUMN\", \"VARBINARY_COLUMN\", \"DATE2_COLUMN\", \"TIMESTAMP_COLUMN\" )" );

        FdoStringP ukeyClause = table->GetAddUkeySql();

        // Verify that all unique key columns got truncate to expected amounts.
        CPPUNIT_ASSERT( ukeyClause == L"UNIQUE (\"STRING100000\"(760), \"STRING100_1\"), UNIQUE (\"STRING2000\"(350), \"STRING700_1\"(350), \"STRING300_1\"(300)), UNIQUE (\"STRING100000\"(266), \"STRING2000\"(266), \"STRING700_1\"(266), \"STRING100_3\", \"STRING100_4\"), UNIQUE (\"STRING2000\"(64), \"STRING300_1\"(64), \"STRING300_2\"(64), \"STRING300_3\"(64), \"STRING300_4\"(64), \"STRING300_5\"(64), \"STRING300_6\"(64), \"STRING300_7\"(64), \"STRING300_8\"(64), \"STRING300_9\"(64), \"STRING300_10\"(64), \"STRING100_1\"(64), \"STRING100_2\"(64), \"STRING100_3\"(64), \"STRING100_4\"(64), \"STRING40_1\"), UNIQUE (\"STRING100_3\", \"STRING100_4\"), UNIQUE (\"STRING40_3\", \"STRING40_4\")" );

        // Table 2 tests a case where 3 constraint columns are 
        // over initial truncation limit, 2 are over the next limit
        // and only 1 over the final limit.
        // Later on we verify truncation by trying some data inserts.
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE2" ));
        column = table->CreateColumnChar( L"STRING2000", false, 2000 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING400", false, 400 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING100", false, 100 );
        table->AddPkeyCol( column->GetName() );

        pkeyClause = table->GetAddPkeySql();
        
        CPPUNIT_ASSERT( pkeyClause == L"constraint \"pk_table2\" primary key ( \"STRING2000\"(500), \"STRING400\"(400), \"STRING100\" )" );

        // Calculating the size of MySql decimal columns is 
        // complicated. Table3 tests creating constraints
        // with decimal columns of various sizes
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE3" ));
        FdoSmPhMySqlTableP mysqlTable = table->SmartCast<FdoSmPhMySqlTable>();
        mysqlTable->SetStorageEngine( MySQLOvStorageEngineType_MyISAM );
        
        // Add a couple of large strings to verify that they
        // are truncated to the expected amount. This verifies
        // that the decimal column size calculations are 
        // correct (i.e. enough space was reserved for the 
        // decimal columns).
        column = table->CreateColumnChar( L"STRING1", false, 2000 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING2", false, 2000 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL1", true, 65, 0 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL2", true, 65, 10 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL3", true, 45, 0 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL4", true, 44, 0 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL5", true, 21, 12 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL6", true, 10, 5 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL7", true, 5, 0 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDecimal( L"DECIMAL8", true, 1, 0 );
        table->AddPkeyCol( column->GetName() );

        pkeyClause = table->GetAddPkeySql();
        
        CPPUNIT_ASSERT( pkeyClause == L"constraint \"pk_table3\" primary key ( \"STRING1\"(440), \"STRING2\"(440), \"DECIMAL1\", \"DECIMAL2\", \"DECIMAL3\", \"DECIMAL4\", \"DECIMAL5\", \"DECIMAL6\", \"DECIMAL7\", \"DECIMAL8\" )" );
        
        database->Commit();

        // Test column truncation by inserting some data values.
        wchar_t prefix500[500];
        wchar_t prefix400[400];
        wchar_t prefix100[100];

        // Fill in every character except the last 
        // non-truncated one for each constraint column.
        for ( ix = 0; ix < 499; ix++ ) 
            prefix500[ix] = 'a';
        for ( ix = 0; ix < 399; ix++ ) 
            prefix400[ix] = 'a';
        for ( ix = 0; ix < 99; ix++ ) 
            prefix100[ix] = 'a';

        prefix500[499] = 0;
        prefix400[399] = 0;
        prefix100[99] = 0;

        GdbiConnection* gdbiConn = phMgr->GetGdbiConnection();

        // Insert first object 
        gdbiConn->ExecuteNonQuery(
            (FdoString*) FdoStringP::Format( 
                L"insert into table2 ( STRING2000, STRING400, STRING100 ) values ( '%ls', '%ls', '%ls' )",
                (FdoString*) (FdoStringP(prefix500) + L"a"),
                (FdoString*) (FdoStringP(prefix400) + L"a"),
                (FdoString*) (FdoStringP(prefix100) + L"a")
            )
        );

        // Insert three more objects. In each case the truncated
        // key column values are identical exception for the 
        // last character of one of the columns.
        gdbiConn->ExecuteNonQuery(
            (FdoString*) FdoStringP::Format( 
                L"insert into table2 ( STRING2000, STRING400, STRING100 ) values ( '%ls', '%ls', '%ls' )",
                (FdoString*) (FdoStringP(prefix500) + L"b"),
                (FdoString*) (FdoStringP(prefix400) + L"a"),
                (FdoString*) (FdoStringP(prefix100) + L"a")
            )
        );

        gdbiConn->ExecuteNonQuery(
            (FdoString*) FdoStringP::Format( 
                L"insert into table2 ( STRING2000, STRING400, STRING100 ) values ( '%ls', '%ls', '%ls' )",
                (FdoString*) (FdoStringP(prefix500) + L"a"),
                (FdoString*) (FdoStringP(prefix400) + L"b"),
                (FdoString*) (FdoStringP(prefix100) + L"a")
            )
        );

        gdbiConn->ExecuteNonQuery(
            (FdoString*) FdoStringP::Format( 
                L"insert into table2 ( STRING2000, STRING400, STRING100 ) values ( '%ls', '%ls', '%ls' )",
                (FdoString*) (FdoStringP(prefix500) + L"a"),
                (FdoString*) (FdoStringP(prefix400) + L"a"),
                (FdoString*) (FdoStringP(prefix100) + L"b")
            )
        );

        bool duplicate = true;

        try {
            // try inserting a duplicate. The key column values are not duplicate
            // but the truncated values are.
            gdbiConn->ExecuteNonQuery(
                (FdoString*) FdoStringP::Format( 
                    L"insert into table2 ( STRING2000, STRING400, STRING100 ) values ( '%ls', '%ls', '%ls' )",
                    (FdoString*) (FdoStringP(prefix500) + L"ab"),
                    (FdoString*) (FdoStringP(prefix400) + L"ab"),
                    (FdoString*) (FdoStringP(prefix100) + L"ab")
                )
            );
        }
        catch ( ... ) 
        {
            duplicate = false;
        }

        CPPUNIT_ASSERT( !duplicate );

        conn->disconnect();

        delete conn;

        printf( "Done\n" );
    }
    catch (FdoException* e ) 
    {
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}


void MySqlSchemaMgrTests::AddProviderColumns( FdoSmPhTableP table )
{

    table->CreateColumnUnknown(
        L"BINARY_COLUMN",
        L"binary",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"MEDIUMBLOB_COLUMN",
        L"mediumblob",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"TINYBLOB_COLUMN",
        L"tinyblob",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"VARBINARY_COLUMN",
        L"varbinary",
        true,
        100,
        0
    );

    table->CreateColumnUnknown(
        L"DATE2_COLUMN",
        L"date",
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
        L"POINT_COLUMN",
        L"point",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"LINESTRING_COLUMN",
        L"linestring",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"POLGON_COLUMN",
        L"polygon",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"GEOMETRYCOLLECTION_COLUMN",
        L"geometrycollection",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"MULTIPOINT_COLUMN",
        L"multipoint",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"MULTILINESTRING_COLUMN",
        L"multilinestring",
        true,
        0,
        0
    );


    table->CreateColumnUnknown(
        L"MULTIPOLYGON_COLUMN",
        L"multipolygon",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"MEDIUMINT_COLUMN",
        L"mediumint",
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
        L"TEXT_COLUMN",
        L"text",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"LONGTEXT_COLUMN",
        L"longtext",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"MEDIUMTEXT_COLUMN",
        L"mediumtext",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"ENUM_COLUMN",
        L"enum('0','1','2')",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"SET_COLUMN",
        L"set('red','green','blue')",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"TIME_COLUMN",
        L"time",
        true,
        0,
        0
    );
    
    table->CreateColumnUnknown(
        L"YEAR_COLUMN",
        L"year",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"UINT_COLUMN",
        L"int unsigned",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"USMALLINT_COLUMN",
        L"smallint unsigned",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"BIT1_COLUMN",
        L"bit",
        true,
        1,
        0
    );

    table->CreateColumnUnknown(
        L"BIT2_COLUMN",
        L"bit",
        true,
        2,
        0
    );

    table->CreateColumnUnknown(
        L"BIT8_COLUMN",
        L"bit",
        true,
        8,
        0
    );

    table->CreateColumnUnknown(
        L"BIT9_COLUMN",
        L"bit",
        true,
        9,
        0
    );

    table->CreateColumnUnknown(
        L"BIT15_COLUMN",
        L"bit",
        true,
        15,
        0
    );

    table->CreateColumnUnknown(
        L"BIT16_COLUMN",
        L"bit",
        true,
        16,
        0
    );

    table->CreateColumnUnknown(
        L"BIT31_COLUMN",
        L"bit",
        true,
        31,
        0
    );

    table->CreateColumnUnknown(
        L"BIT32_COLUMN",
        L"bit",
        true,
        32,
        0
    );
}

void MySqlSchemaMgrTests::VldGenGeom( FdoClassDefinitionP classDef )
{
    CPPUNIT_ASSERT( classDef->GetClassType() == FdoClassType_FeatureClass );
    FdoFeatureClass* featClass = static_cast<FdoFeatureClass*>(classDef.p);

    FdoStringP className = featClass->GetName();
    FdoGeometricPropertyP geomProp = featClass->GetGeometryProperty();
    FdoStringP geomPropName = geomProp ? geomProp->GetName() : L"";

    CPPUNIT_ASSERT( (className == L"INDEX_WINS1") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_WINS2") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_WINS3") ? (geomPropName == L"GEOM2" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_WINS4") ? (geomPropName == L"GEOM2" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_TIE1") ? (geomPropName == L"GEOM2" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_TIE2") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"INDEX_TIE3") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"NNULL_WINS1") ? (geomPropName == L"GEOM1" ) : true );
    CPPUNIT_ASSERT( (className == L"NNULL_WINS2") ? (geomPropName == L"GEOM2" ) : true );
    CPPUNIT_ASSERT( (className == L"NNULL_TIE") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"NO_WIN") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"ONE_GEOM") ? (geomPropName == L"GEOM1" ) : true );
}
