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
#include "PostGisSchemaMgrTests.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "PostGisConnectionUtil.h"
#include "../PostGis/SchemaMgr/Ph/Table.h"
#include "../PostGis/SchemaMgr/Ph/Owner.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisSchemaMgrTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisSchemaMgrTests, "SchemaMgrTests");

StaticConnection* PostGisSchemaMgrTests::CreateStaticConnection()
{
    return new PostGisStaticConnection();
}

FdoIoStream* PostGisSchemaMgrTests::OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix )
{
    FdoIoMemoryStream* stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::OverrideBend( 
        stream1, 
        stream2, 
        "OSGeo.PostgreSQL.3.2", 
        "http://fdoPostgreSQL.osgeo.org/schemas",
        oldOwnerPrefix,
        newOwnerPrefix
    );

    return stream2;
}

#if 0
// Tests character set and collation retrieval
void PostGisSchemaMgrTests::testCharacterSets ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoInt32 idx;

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

        for ( idx = 0; idx < 1; idx++ ) {
            FdoSmPhCharacterSetP charSet = database->FindCharacterSet( L"latin1" );
            CPPUNIT_ASSERT( charSet != NULL );
            FdoSmPhPostGisCharacterSetP PostGisCharSet = charSet->SmartCast<FdoSmPhPostGisCharacterSet>();
            CPPUNIT_ASSERT( PostGisCharSet->GetCharLen() == 1 );

            charSet = database->GetCharacterSet( L"utf8" );
            PostGisCharSet = charSet->SmartCast<FdoSmPhPostGisCharacterSet>();
            CPPUNIT_ASSERT( PostGisCharSet->GetCharLen() == 3 );

            FdoSmPhCollationP collation = database->FindCollation( L"latin1_bin" );
            CPPUNIT_ASSERT( collation != NULL );
            charSet = collation->GetCharacterSet();
            CPPUNIT_ASSERT( wcscmp(charSet->GetName(), L"latin1") == 0 );

            collation = database->GetCollation( L"utf8_bin" );
            charSet = collation->GetCharacterSet();
            CPPUNIT_ASSERT( wcscmp(charSet->GetName(), L"utf8") == 0 );
        }


        CPPUNIT_ASSERT( database->FindCharacterSet(L"noexist") == NULL );
        CPPUNIT_ASSERT( database->FindCollation(L"noexist") == NULL );

        bool succeeded = true;
        try {
            FdoSmPhCharacterSetP charSet = database->GetCharacterSet( L"noexist" );
        }
        catch ( FdoException* exc ) 
        {
            exc->Release();
            succeeded = false;
        }
        CPPUNIT_ASSERT( !succeeded );

        succeeded = true;
        try {
            FdoSmPhCollationP collation = database->GetCollation( L"noexist" );
        }
        catch ( FdoException* exc ) 
        {
            exc->Release();
            succeeded = false;
        }
        CPPUNIT_ASSERT( !succeeded );

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
#endif

void PostGisSchemaMgrTests::AddProviderColumns( FdoSmPhTableP table )
{

    FdoSmPhColumnP column = table->CreateColumnUnknown(
        L"character_column",
        L"character",
        true,
        15,
        0
    );

    column = table->CreateColumnUnknown(
        L"character_column2",
        L"character",
        true,
        0,
        0
    );

   column = table->CreateColumnUnknown(
        L"character_varying_column",
        L"character varying",
        true,
        45,
        0
    );

    column = table->CreateColumnUnknown(
        L"character_varying_column2",
        L"character varying",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"varchar_column",
        L"varchar",
        true,
        60,
        0
    );

    column = table->CreateColumnUnknown(
        L"name_column",
        L"name",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"text_column",
        L"text",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"char_column",
        L"char",
        true,
        10,
        0
    );

    column = table->CreateColumnUnknown(
        L"bpchar_column",
        L"bpchar",
        true,
        50,
        0
    );

    column = table->CreateColumnUnknown(
        L"bit_column",
        L"bit",
        true,
        30,
        0
    );

    column = table->CreateColumnUnknown(
        L"bit_column2",
        L"bit",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"bit_varying_column",
        L"bit varying",
        true,
        40,
        0
    );

    column = table->CreateColumnUnknown(
        L"bit_varying_column2",
        L"bit varying",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"numeric_column",
        L"numeric",
        true,
        8,
        3
    );

    column = table->CreateColumnUnknown(
        L"numeric_column2",
        L"numeric",
        true,
        11,
        0
    );

    column = table->CreateColumnUnknown(
        L"numeric_column3",
        L"numeric",
        true,
        0,
        0
    );

    column = table->CreateColumnUnknown(
        L"decimal_column2",
        L"decimal",
        true,
        5,
        0
    );

    column = table->CreateColumnUnknown(
        L"decimal_column3",
        L"decimal",
        true,
        0,
        0
    );

}

void PostGisSchemaMgrTests::VldGenGeom( FdoClassDefinitionP classDef )
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

FdoStringP PostGisSchemaMgrTests::table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName )
{
    return mgr->GetDcDbObjectName( tableName ).Right(L".");
}

FdoStringP PostGisSchemaMgrTests::table2qclass( FdoSmPhGrdMgrP mgr, FdoStringP datastoreName, FdoStringP tableName )
{
    return FdoStringP(L"public:") + table2class(mgr, tableName);
}

FdoStringP PostGisSchemaMgrTests::GetIndexName( FdoSmPhMgrP mgr, FdoStringP indexName )
{
    return mgr->GetDcDbObjectName( indexName );
}

void PostGisSchemaMgrTests::testMaterializedViews()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf(" >>> ... creating test database \n");
        if (UnitTestUtil::DatastoreExists(L"_materialized_views"))
            UnitTestUtil::DropDb(L"_materialized_views");

        UnitTestUtil::CreateDB(false, false, L"_materialized_views", 0, false, false);

        // Connect and create the test schema.
        connection = UnitTestUtil::GetConnection(L"_materialized_views");

        // Create our user if it doesn't exist and map it to this database
        printf(" >>> ... creating table \n");
        FdoStringP sql = L"CREATE TABLE vec (id INTEGER PRIMARY KEY, a INTEGER, b INTEGER)";
        UnitTestUtil::Sql2Db(sql, connection);
        printf(" >>> ... creating view \n");
        sql = L"CREATE VIEW vec_view AS SELECT * FROM vec";
        UnitTestUtil::Sql2Db(sql, connection);
        printf(" >>> ... creating materialized view \n");
        sql = L"CREATE MATERIALIZED VIEW hypot AS SELECT id, sqrt(a*a + b*b) as c FROM vec";
        UnitTestUtil::Sql2Db(sql, connection);

        FdoPtr<FdoIDescribeSchema> cmdDescribe = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribe->Execute();
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoInt32 vecIdx = classes->IndexOf(L"vec");
        FdoInt32 vecViewIdx = classes->IndexOf(L"vec_view");
        FdoInt32 hypotIdx = classes->IndexOf(L"hypot");
        CPPUNIT_ASSERT(vecIdx >= 0);
        CPPUNIT_ASSERT(vecViewIdx >= 0);
        CPPUNIT_ASSERT(hypotIdx >= 0);
        CPPUNIT_ASSERT_MESSAGE("Expected 3 classes (a table, a view and a materialized view)", classes->GetCount() == 3);

        FdoPtr<FdoClassDefinition> klassVec = classes->GetItem(vecIdx);
        FdoPtr<FdoClassDefinition> klassVecView = classes->GetItem(vecViewIdx);
        FdoPtr<FdoClassDefinition> klassHypot = classes->GetItem(hypotIdx);

        //Test vec
        FdoPtr<FdoPropertyDefinitionCollection> vecProps = klassVec->GetProperties();
        CPPUNIT_ASSERT(3 == vecProps->GetCount());
        FdoPtr<FdoPropertyDefinition> idValue = vecProps->FindItem(L"id");
        CPPUNIT_ASSERT(idValue->GetPropertyType() == FdoPropertyType_DataProperty);
        FdoPtr<FdoPropertyDefinition> aValue = vecProps->FindItem(L"a");
        CPPUNIT_ASSERT(aValue->GetPropertyType() == FdoPropertyType_DataProperty);
        FdoPtr<FdoPropertyDefinition> bValue = vecProps->FindItem(L"b");
        CPPUNIT_ASSERT(bValue->GetPropertyType() == FdoPropertyType_DataProperty);

        FdoPtr<FdoDataPropertyDefinitionCollection> vecIdProps = klassVec->GetIdentityProperties();
        CPPUNIT_ASSERT(1 == vecIdProps->GetCount());
        idValue = vecIdProps->FindItem(L"id");
        CPPUNIT_ASSERT(idValue->GetPropertyType() == FdoPropertyType_DataProperty);

        //Test vec_view
        FdoPtr<FdoPropertyDefinitionCollection> vecViewProps = klassVecView->GetProperties();
        CPPUNIT_ASSERT(3 == vecViewProps->GetCount());
        FdoPtr<FdoPropertyDefinition> vvidValue = vecViewProps->FindItem(L"id");
        CPPUNIT_ASSERT(vvidValue->GetPropertyType() == FdoPropertyType_DataProperty);
        FdoPtr<FdoPropertyDefinition> vvaValue = vecViewProps->FindItem(L"a");
        CPPUNIT_ASSERT(vvaValue->GetPropertyType() == FdoPropertyType_DataProperty);
        FdoPtr<FdoPropertyDefinition> vvbValue = vecViewProps->FindItem(L"b");
        CPPUNIT_ASSERT(vvbValue->GetPropertyType() == FdoPropertyType_DataProperty);

        FdoPtr<FdoDataPropertyDefinitionCollection> vecViewIdProps = klassVecView->GetIdentityProperties();
        CPPUNIT_ASSERT(1 == vecViewIdProps->GetCount());
        vvidValue = vecViewIdProps->FindItem(L"id");
        CPPUNIT_ASSERT(idValue->GetPropertyType() == FdoPropertyType_DataProperty);
        
        //Test hypot
        FdoPtr<FdoPropertyDefinitionCollection> hypotProps = klassHypot->GetProperties();
        CPPUNIT_ASSERT(2 == hypotProps->GetCount());
        FdoPtr<FdoPropertyDefinition> cValue = hypotProps->FindItem(L"c");
        CPPUNIT_ASSERT(cValue->GetPropertyType() == FdoPropertyType_DataProperty);
        FdoPtr<FdoPropertyDefinition> hypotIdValue = hypotProps->FindItem(L"id");
        CPPUNIT_ASSERT(hypotIdValue->GetPropertyType() == FdoPropertyType_DataProperty);

        //FdoPtr<FdoDataPropertyDefinitionCollection> hypotIdProps = klassHypot->GetIdentityProperties();
        //hypotIdValue = hypotIdProps->FindItem(L"id");
        //CPPUNIT_ASSERT(1 == hypotIdProps->GetCount());

        connection->Close();
    }
    catch (FdoException *exp)
    {
        printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
        if (connection)
            connection->Close();
        TestCommonFail(exp);
    }
    catch (...)
    {
        if (connection) connection->Close();
        throw;
    }
}