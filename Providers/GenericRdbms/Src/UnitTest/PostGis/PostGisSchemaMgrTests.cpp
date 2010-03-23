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

