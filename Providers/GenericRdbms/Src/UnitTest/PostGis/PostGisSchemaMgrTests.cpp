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
        "OSGeo.PostGIS.3.2", 
        "http://fdoPostGis.osgeo.org/schemas",
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
#if 0
    FdoSmPhColumnP column = table->CreateColumnInt64(
        L"AUTOINCREMENT_COLUMN",
        false,
        true
    );

    FdoSmPhColumnsP ukeyColumns = new FdoSmPhColumnCollection();
	table->GetUkeyColumns()->Add( ukeyColumns );
    table->AddUkeyCol( 0, L"AUTOINCREMENT_COLUMN" );

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
#endif
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
