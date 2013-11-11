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
#include "../../SchemaMgr/Ph/Owner.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerSchemaMgrTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerSchemaMgrTests, "SchemaMgrTests");

static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;
    propertyValue = propertyValues->FindItem( name );

    if ( !propertyValue )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}

void SqlServerSchemaMgrTests::testSpatialContextsGeog()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

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
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
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
        owner->Commit();

        FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table1" ));
        FdoSmPhColumnP column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );

        FdoSmPhScInfoP scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
        column->SetTypeName(L"geography");
        
        scinfo = CreateSc( GetSrid(0), -1001, -1002, 1001, 1002, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geom_column2", scinfo, true, false );
        
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table2" ));
        column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );
        
        scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
        column->SetTypeName(L"geography");
        
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table3" ));
        column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );
        
        scinfo = CreateSc( GetSrid(0), -100, 45, -90, 55, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geom_column1", scinfo, true, true );
        
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table4" ));
        column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );
        
        scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
        column->SetTypeName(L"geography");
        
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table5" ));
        column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );
        
        scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
        column->SetTypeName(L"geography");
        
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table6" ));
        column = table->CreateColumnInt32( L"id", false );
        table->AddPkeyCol( column->GetName() );
        
        scinfo = CreateSc( GetSrid(4), -1001, -1002, 1001, 1002, 0.0333, 0.0111 );
        column = table->CreateColumnGeom( L"geom_column1", scinfo, true, true );
               
        owner->Commit();


        FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();

        FdoStringP sqlStmt = FdoStringP::Format( 
            L"insert into table3 ( id, geom_column1 ) values ( 1, geometry::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(3)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );

        sqlStmt = FdoStringP::Format( 
            L"insert into table4 ( id, geog_column1 ) values ( 1, geography::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(2)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );

        sqlStmt = FdoStringP::Format( 
            L"insert into table5 ( id, geog_column1 ) values ( 1, geography::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(1)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );

        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;

		printf( "Connecting and Describing ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoIoMemoryStreamP stream1 = FdoIoMemoryStream::Create();

        FdoXmlSpatialContextFlagsP flags = FdoXmlSpatialContextFlags::Create(
            L"fdo.osgeo.org/schemas/feature",
            FdoXmlFlags::ErrorLevel_Normal,
            true,
            FdoXmlSpatialContextFlags::ConflictOption_Add,
            true
        );

        UnitTestUtil::ExportDb( 
            fdoConn, 
            stream1, 
            flags,
            false, 
            FdoStringP(L"Fdo") + datastore
        );
		UnitTestUtil::Stream2File( stream1, UnitTestUtil::GetOutputFileName( L"spatial_contexts3.xml" ) );

        UnitTestUtil::CheckOutput( 
            FdoStringP::Format(L"spatial_contexts3_%ls_master.txt", (FdoString*) providerName),
            UnitTestUtil::GetOutputFileName( L"spatial_contexts3.xml" )
        );

//        InsertSridRow( fdoConn, L"table1", L"geom_column2", 0, 1 );
//        InsertSridRow( fdoConn, L"table2", L"geog_column1", 0, 1 );
        InsertSridRow( fdoConn, L"table3", L"geom_column1", 3, 2 );
//        InsertSridRow( fdoConn, L"table4", L"geog_column1", 2, 2 );
//        InsertSridRow( fdoConn, L"table5", L"geog_column1", 1, 2 );
        InsertSridRow( fdoConn, L"table6", L"geom_column1", 4, 0 );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

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

void SqlServerSchemaMgrTests::testSynonyms()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

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

        FdoStringP datastore;
        FdoStringP fdatastore;

        {
            FdoSchemaManagerP mgr = conn->CreateSchemaManager();

            FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

            printf( "(Re)Creating datastores ...\n" );

            datastore = phMgr->GetDcOwnerName(
                UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
            );

            fdatastore = phMgr->GetDcOwnerName(
                UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
            );

            UnitTestUtil::CreateDBNoMeta(mgr, datastore);

            UnitTestUtil::CreateDBNoMeta(mgr, fdatastore);

            printf( "Creating Schema ...\n" );

            FdoSmPhOwnerP fowner = phMgr->FindOwner( fdatastore, L"", false );

            FdoSmPhTableP ftable1 = fowner->CreateTable( phMgr->GetDcDbObjectName(L"ftable1" ));
            FdoSmPhColumnP column = ftable1->CreateColumnInt32( L"fid", false );
            ftable1->AddPkeyCol( column->GetName() );
            column = ftable1->CreateColumnDate(L"stamp", true);
            FdoSmPhScInfoP scinfo = CreateSc( GetSrid(3), 0, 0, 200000, 100000, 0.0333, 0.0111 );
            column = ftable1->CreateColumnGeom( L"geometry1", scinfo, true, false );

            FdoSmPhSynonymP fsynonym1 = fowner->CreateSynonym(L"synonym1", ftable1 );

            FdoSmPhTableP ftable2 = fowner->CreateTable( phMgr->GetDcDbObjectName(L"f[]table2" ));
            column = ftable2->CreateColumnInt32( L"fid2", false );
            ftable2->AddPkeyCol( column->GetName() );
            column = ftable2->CreateColumnChar(L"value", false, 20);
            column = ftable2->CreateColumnByte(L"flags", true);
            FdoSmPhColumnP fColumn = ftable2->CreateColumnInt32( L"ftable1_fid", true );
            scinfo = CreateSc( GetSrid(5), 0, 0, 200000, 100000, 0.0333, 0.0111 );
            column = ftable2->CreateColumnGeom( L"geometry2", scinfo, true, false );
            FdoSmPhFkeyP fkey = ftable2->CreateFkey(L"fk_ftable1", L"dbo.ftable1");
            fkey->AddFkeyColumn( fColumn, L"fid" );

            FdoSmPhTableP ftable3 = fowner->CreateTable( phMgr->GetDcDbObjectName(L"ftable3" ));
            column = ftable3->CreateColumnInt32( L"fid3", false );
            ftable3->AddPkeyCol( column->GetName() );
            column = ftable3->CreateColumnChar(L"value", false, 20);
            scinfo = CreateSc( GetSrid(3), 0, 0, 200000, 100000, 0.0333, 0.0111 );
            column = ftable3->CreateColumnGeom( L"geometry", scinfo, true, false );
            
            FdoSmPhViewP fview3 = fowner->CreateView(L"view3", L"", L"", ftable3->GetName() );
            column = fview3->CreateColumnInt32( L"fid3", false, false, L"fid3" );
            column = fview3->CreateColumnChar(L"value", false, 20, L"value");
            column = fview3->CreateColumnGeom( L"geometry", scinfo, true, false, false, L"geometry" );

            fowner->Commit();


            FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );

            FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"[table]" ));
            column = table->CreateColumnInt32( L"id", false );
            table->AddPkeyCol( column->GetName() );

            scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
            column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
            column->SetTypeName(L"geography");
            
            scinfo = CreateSc( GetSrid(0), -1001, -1002, 1001, 1002, 0.0333, 0.0111 );
            column = table->CreateColumnGeom( L"geom_column2", scinfo, true, false );
            
            FdoSmPhSynonymP synonym = owner->CreateSynonym(L"synonym1", table );

            FdoSmPhViewP view = owner->CreateView(L"view1", L"", L"", table->GetName() );
            column = view->CreateColumnInt32( L"id", false, false, L"id" );
            column = view->CreateColumnGeom( L"geom_column2", scinfo, true, false, false, L"geom_column2" );

            synonym = owner->CreateSynonym(L"synonym1b", view );

            table = owner->CreateTable( phMgr->GetDcDbObjectName(L"]table[" ));
            column = table->CreateColumnInt32( L"id2", false );
            table->AddPkeyCol( column->GetName() );
            
            scinfo = CreateSc( GetSrid(0), -90, -180, 90, 180, 0.0333, 0.0111 );
            column = table->CreateColumnGeom( L"geog_column1", scinfo, true, true );
            column->SetTypeName(L"geography");
            
            column = table->CreateColumnChar( L"name", true, 50);

            synonym = owner->CreateSynonym(L"synonym2", table );
            synonym = owner->CreateSynonym(L"synonym2b", synonym );

            table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table[]embedded" ));
            column = table->CreateColumnChar( L"first name", false, 50 );
            table->AddPkeyCol( column->GetName() );

            column = table->CreateColumnChar( L"last name", false, 50 );
            table->AddPkeyCol( column->GetName() );
            
            column = table->CreateColumnDouble( L"amount", true );
            fColumn = table->CreateColumnInt32( L"table2_id", false );
            fkey = table->CreateFkey(L"table2", L"dbo.]table[");
            fkey->AddFkeyColumn( fColumn, L"id2" );
                   
            synonym = owner->CreateSynonym(L"synonym3", table );

            view = owner->CreateView(L"view3", L"", L"", synonym->GetName() );
            column = view->CreateColumnChar( L"first name", false, 50, L"first name" );
            column = view->CreateColumnChar( L"last name", false, 50, L"last name" );
            column = view->CreateColumnDouble( L"amount", true, L"amount" );

            synonym = owner->CreateSynonym(L"synonym4", fsynonym1 );
            synonym = owner->CreateSynonym(L"ftable2", ftable2 );
            synonym = owner->CreateSynonym(L"synonym6", fview3 );

            owner->Commit();

            FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();

            // Create some synonyms where the name for the referenced object does not 
            // include database name.
            grdOwner->ActivateAndExecute( "create synonym dbo.synonym7 for \"dbo\".\"[table]\" " );
            grdOwner->ActivateAndExecute( "create synonym dbo.synonym8 for \"dbo\".\"]table[\" " );
            grdOwner->ActivateAndExecute( "create synonym dbo.synonym9 for \"dbo\".\"table[]embedded\" " );
        }
            
        delete conn;
        conn = NULL;

        printf( "Connecting and Describing ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoIoMemoryStreamP stream1 = FdoIoMemoryStream::Create();

        FdoXmlSpatialContextFlagsP flags = FdoXmlSpatialContextFlags::Create(
            L"fdo.osgeo.org/schemas/feature",
            FdoXmlFlags::ErrorLevel_Normal,
            true,
            FdoXmlSpatialContextFlags::ConflictOption_Add,
            true
        );

        UnitTestUtil::ExportDb( 
            fdoConn, 
            stream1, 
            flags,
            false, 
            FdoStringP(L"Fdo") + datastore
        );
		UnitTestUtil::Stream2File( stream1, UnitTestUtil::GetOutputFileName( L"synonyms1.xml" ) );

        UnitTestUtil::CheckOutput( 
            FdoStringP::Format(L"synonyms1_%ls_master.xml", (FdoString*) providerName),
            UnitTestUtil::GetOutputFileName( L"synonyms1.xml" )
        );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

        printf( "Doing partial Describe ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoStringsP classNames = FdoStringCollection::Create();
        classNames->Add( L"dbo:synonym2");
        classNames->Add( L"dbo:synonym4");

        FdoPtr<FdoIDescribeSchema> cmd = (FdoIDescribeSchema*)(fdoConn->CreateCommand( FdoCommandType_DescribeSchema )); 
        cmd->SetClassNames(classNames);
        FdoFeatureSchemasP schemas = cmd->Execute();
        FdoFeatureSchemaP schema = schemas->FindItem(L"dbo");
        CPPUNIT_ASSERT(schema);

        FdoClassesP classes = schema->GetClasses();
        CPPUNIT_ASSERT(classes->GetCount() == 2 );

        cmd = (FdoIDescribeSchema*)(fdoConn->CreateCommand( FdoCommandType_DescribeSchema )); 
        schemas = cmd->Execute();
        VldSynonymSchema(schemas);

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

        printf( "Doing list and Describe ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoIGetClassNames> listCmd = (FdoIGetClassNames*)(fdoConn->CreateCommand( FdoCommandType_GetClassNames )); 
        FdoStringsP listClassNames = listCmd->Execute();

        CPPUNIT_ASSERT(listClassNames->GetCount() == 16 );

        cmd = (FdoIDescribeSchema*)(fdoConn->CreateCommand( FdoCommandType_DescribeSchema )); 
        schemas = cmd->Execute();
        VldSynonymSchema(schemas);

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

        printf( "Performing select and describe ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoISelect> selCmd = (FdoISelect*)(fdoConn->CreateCommand( FdoCommandType_Select )); 
        selCmd->SetFeatureClassName(L"dbo:synonym1");
        FdoPtr<FdoIFeatureReader> rdr = selCmd->Execute();
        FdoClassDefinitionP classDef = rdr->GetClassDefinition();
        CPPUNIT_ASSERT(classDef);

        FdoPropertiesP props = classDef->GetProperties();
        CPPUNIT_ASSERT(props->GetCount() == 3);
        CPPUNIT_ASSERT(props->Contains(L"id"));
        CPPUNIT_ASSERT(props->Contains(L"geog_column1"));
        CPPUNIT_ASSERT(props->Contains(L"geom_column2"));

        cmd = (FdoIDescribeSchema*)(fdoConn->CreateCommand( FdoCommandType_DescribeSchema )); 
        schemas = cmd->Execute();
        
        VldSynonymSchema(schemas);

        rdr = NULL;

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

        printf( "Done\n" );
    }
    catch (FdoException* e ) 
    {
        if ( conn ) delete conn;
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        if ( conn ) delete conn;
        throw exception;
    }
    catch (...)
    {
        if ( conn ) delete conn;
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void SqlServerSchemaMgrTests::VldSynonymSchema( FdoFeatureSchemasP schemas )
{
    FdoFeatureSchemaP schema = schemas->FindItem(L"dbo");
    CPPUNIT_ASSERT(schema);

    FdoClassesP classes = schema->GetClasses();
    CPPUNIT_ASSERT(classes->GetCount() == 16 );

    int propCount = 0;
    for ( int i = 0; i < classes->GetCount(); i++ ) 
    {
        FdoClassDefinitionP classDef = classes->GetItem(i);
        FdoPropertiesP props = classDef->GetProperties();
        propCount += props->GetCount();
    }

    CPPUNIT_ASSERT(propCount == 54 );
}

int SqlServerSchemaMgrTests::GenKeysCreateSpecific( FdoSmPhGrdOwner* grdOwner )
{
    FdoStringP createJoinSql = 
        L"create view dbo.view_join_ix1 ( id, int16_col1, string_col5 ) "
        L"with schemabinding "
        L"as select a.id, a.int16_col1, b.string_col5 from dbo.table_ix1 a, dbo.table_ix2 b";


    grdOwner->ActivateAndExecute( L"set ansi_nulls on" );
    grdOwner->ActivateAndExecute( L"set quoted_identifier on" );
    grdOwner->ActivateAndExecute( (FdoString*) createJoinSql );
    grdOwner->ActivateAndExecute( L"create unique clustered index view_join_ix1_ix on dbo.view_join_ix1 (string_col5)" );

    createJoinSql = 
        L"create view dbo.view_join_ix2 ( id, int16_col1, string_col5 ) "
        L"with schemabinding "
        L"as select a.id, a.int16_col1, b.string_col5 from dbo.table_ix1 a, dbo.table_ix2 b";


    grdOwner->ActivateAndExecute( (FdoString*) createJoinSql );
    grdOwner->ActivateAndExecute( L"create unique clustered index view_join_ix2_ix on dbo.view_join_ix2 (id, int16_col1)" );

    return 2;
}

void SqlServerSchemaMgrTests::GenKeysVldSpecific( FdoClassCollection* classes )
{
    FdoClassDefinitionP featClass = classes->GetItem( L"view_join_ix1" );
    FdoDataPropertiesP idProps = featClass->GetIdentityProperties();
    CPPUNIT_ASSERT( idProps->GetCount() == 1 );
    FdoDataPropertyP prop = idProps->GetItem(0);
    CPPUNIT_ASSERT( wcscmp(prop->GetName(), L"string_col5") == 0 );

    featClass = classes->GetItem( L"view_join_ix2" );
    idProps = featClass->GetIdentityProperties();
    CPPUNIT_ASSERT( idProps->GetCount() == 2 );
    prop = idProps->GetItem(0);
    CPPUNIT_ASSERT( wcscmp(prop->GetName(), L"id") == 0 );
    prop = idProps->GetItem(1);
    CPPUNIT_ASSERT( wcscmp(prop->GetName(), L"int16_col1") == 0 );
}



void SqlServerSchemaMgrTests::CreateMultiGeomTable( FdoSmPhOwnerP owner, FdoStringP tableName, FdoInt32 colCount, FdoInt32 indexMask, FdoInt32 nnullMask )
{
    SchemaMgrTests::CreateMultiGeomTable( owner, tableName, colCount, indexMask, nnullMask );

    FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();
    if ( tableName == L"INDEX_WINS2" )
    {
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"insert into index_wins2 ( id, geom3 ) values ( 1, geometry::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(5)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );
    }

    if ( tableName == L"BASE_GEOM" )
    {
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"insert into base_geom ( id, geom1, geom2 ) values ( 1, geometry::STGeomFromText('POINT ( 10 15 )', %I64d), geometry::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(5),
            GetSrid(3)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );
        
        sqlStmt = FdoStringP::Format( 
            L"insert into base_geom ( id, geom1, geom2 ) values ( 2, geometry::STGeomFromText('POINT ( 10 15 )', %I64d), geometry::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(6),
            GetSrid(3)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );
        
        sqlStmt = FdoStringP::Format( 
            L"insert into base_geom ( id, geom1, geom2 ) values ( 3, geometry::STGeomFromText('POINT ( 10 15 )', %I64d), geometry::STGeomFromText('POINT ( 10 15 )', %I64d) )", 
            GetSrid(3),
            GetSrid(3)
        );
        grdOwner->ActivateAndExecute( (FdoString*) sqlStmt );

    }

}

void SqlServerSchemaMgrTests::InsertSridRow( FdoIConnection* fdoConn, FdoStringP tableName, FdoStringP geomColumnName, int sridIndex, int expectedCount )
{
    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) fdoConn->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(tableName);

    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

    FdoPtr<FdoDataValue> dataValue = FdoInt32Value::Create(2);
    FdoPtr<FdoPropertyValue> propertyValue = AddNewProperty( propertyValues, L"id" );
    propertyValue->SetValue(dataValue);

    double       coordsBuffer[2];
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    coordsBuffer[0] = 1;
    coordsBuffer[1] = 2;

    propertyValue = AddNewProperty( propertyValues, geomColumnName );
    FdoPtr<FdoIPoint> point1 = gf->CreatePoint(FdoDimensionality_XY, coordsBuffer);
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(point1);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

    FdoPtr<FdoISQLCommand> sqlCommand = (FdoISQLCommand *) fdoConn->CreateCommand(FdoCommandType_SQLCommand);
    sqlCommand->SetSQLStatement( 
        FdoStringP::Format(
            L"select geom_column1.STSrid as srid from %ls",
            (FdoString*) tableName
        )
    );

    FdoPtr<FdoISQLDataReader> sqlRdr = sqlCommand->ExecuteReader();
    int count = 0;
    while ( sqlRdr->ReadNext() ) {
        CPPUNIT_ASSERT( sqlRdr->GetInt32(L"srid") == GetSrid(sridIndex) );
        count++;
    }

    CPPUNIT_ASSERT( count >=expectedCount );
}

StaticConnection* SqlServerSchemaMgrTests::CreateStaticConnection()
{
    return new SqlServerStaticConnection();
}

FdoIoStream* SqlServerSchemaMgrTests::OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix )
{
    FdoIoMemoryStream* stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::OverrideBend( stream1, stream2, 
        "OSGeo.SQLServerSpatial.3.9", 
        "http://www.autodesk.com/isd/fdo/SQLServerSpatialProvider",
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
        L"NVARCHAR_MAX_COLUMN",
        L"nvarchar(max)",
        true,
        0,
        0
    );

    table->CreateColumnUnknown(
        L"VARCHAR_MAX_COLUMN",
        L"varchar(max)",
        true,
        0,
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

void SqlServerSchemaMgrTests::VldGenGeom( FdoIConnection* conn, FdoClassDefinitionP classDef )
{
    CPPUNIT_ASSERT( classDef->GetClassType() == FdoClassType_FeatureClass );
    FdoFeatureClass* featClass = static_cast<FdoFeatureClass*>(classDef.p);

    FdoStringP className = featClass->GetName();
    FdoGeometricPropertyP geomProp = featClass->GetGeometryProperty();
    FdoStringP geomPropName = geomProp ? geomProp->GetName() : L"";

    CPPUNIT_ASSERT( (className == L"index_wins1") ? (geomPropName == L"geom1" ) : true );
    CPPUNIT_ASSERT( (className == L"index_wins2") ? (geomPropName == L"geom2" ) : true );
    CPPUNIT_ASSERT( (className == L"index_wins3") ? (geomPropName == L"geom1" ) : true );
    CPPUNIT_ASSERT( (className == L"index_wins4") ? (geomPropName == L"geom2" ) : true );
    CPPUNIT_ASSERT( (className == L"index_tie1") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"index_tie2") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"index_tie3") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"nnull_wins1") ? (geomPropName == L"geom1" ) : true );
    CPPUNIT_ASSERT( (className == L"nnull_wins2") ? (geomPropName == L"geom2" ) : true );
    CPPUNIT_ASSERT( (className == L"nnull_tie") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"no_win") ? (geomPropName == L"" ) : true );
    CPPUNIT_ASSERT( (className == L"one_geom") ? (geomPropName == L"geom1" ) : true );

// TODO: determine geometryColumn from root table.
//    CPPUNIT_ASSERT( (className == L"aview_index_wins1") ? (geomPropName == L"geom1" ) : true );
//    CPPUNIT_ASSERT( (className == L"view_index_wins2") ? (geomPropName == L"geom1" ) : true );

    if ( (className == L"index_wins1") || 
         (className == L"aview_index_wins1")
    ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"", 0, 0, 0, 0 );
    }

    if ( (className == L"index_wins2") || 
         (className == L"view_index_wins2")
    ) {
        VldGeomSC(conn, classDef, L"geom1", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom3", L"CA-I", -2000000, -2000000, 2000000, 2000000 );
    }

    if ( (className == L"aview_index_wins3") || 
         (className == L"bview_index_wins3")
    ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geom3", L"", 0, 0, 0, 0 );
    }

    if ( (className == L"view_index_wins4") || 
         (className == L"viewb_index_wins4")
    ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom3", L"", 0, 0, 0, 0 );
    }

    if ( className == L"f_index_wins1" ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"", 0, 0, 0, 0 );
    }

    if ( className == L"f_index_wins2" ) {
        VldGeomSC(conn, classDef, L"geom1", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom3", L"CA-I", -2000000, -2000000, 2000000, 2000000 );
    }

    if ( className == L"f_index_wins3" ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geom3", L"", 0, 0, 0, 0 );
    }

    if ( className == L"f_index_wins4" ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom3", L"", 0, 0, 0, 0 );
    }

    if ( className == L"rview_index_wins4" ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geomb", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geom3", L"", 0, 0, 0, 0 );
    }

    if ( className == L"viewa_base_geom" ) {
        VldGeomSC(conn, classDef, L"geom1", L"CA-I", -2000000, -2000000, 2000000, 2000000 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
    }

    if ( className == L"viewb_base_geom" ) {
        VldGeomSC(conn, classDef, L"geom1", L"CANQ-M5", -2000000, -2000000, 2000000, 2000000 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
    }

    if ( className == L"viewc_base_geom" ) {
        VldGeomSC(conn, classDef, L"geom1", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
        VldGeomSC(conn, classDef, L"geom2", L"Antigua43.BWIgrid", -1001, -1002, 1001, 1002 );
    }

    if ( className == L"viewm_index_wins3" ) {
        VldGeomSC(conn, classDef, L"geom1", L"", 0, 0, 0, 0 );
        VldGeomSC(conn, classDef, L"geoma", L"", 0, 0, 0, 0 );
    }
}

void SqlServerSchemaMgrTests::VldGeomSC( 
    FdoIConnection* conn, 
    FdoClassDefinitionP classDef,
    FdoStringP propName,
    FdoStringP expCoordSys,
    double expminx,
    double expminy,
    double expmaxx,
    double expmaxy
)
{
    FdoPropertiesP props = classDef->GetProperties();
    FdoGeometricPropertyP geomProp = static_cast<FdoGeometricPropertyDefinition*>(props->FindItem(propName));
    FdoStringP scName = geomProp->GetSpatialContextAssociation();
    CPPUNIT_ASSERT( scName != L"" );

    FdoStringP description;
    FdoStringP coordSys;
    FdoStringP wkt;
    FdoPtr<FdoByteArray> extent;
    FdoSpatialContextExtentType extentType;
    double xyTol;
    double zTol;

    CPPUNIT_ASSERT( 
        TestCommonMiscUtil::GetSpatialContext(
            conn,
            scName,
            description,
            coordSys,
            wkt,
            extent,
            extentType,
            xyTol,
            zTol
        )
    );

    CPPUNIT_ASSERT( coordSys == expCoordSys );

    if ( coordSys != L"" )
        TestCommonMiscUtil::VldExtent( scName, extent, expminx, expminy, expmaxx, expmaxy );
}

FdoStringP SqlServerSchemaMgrTests::table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName )
{
    return mgr->GetDcDbObjectName( tableName ).Right(L".");
}

FdoStringP SqlServerSchemaMgrTests::table2qclass( FdoSmPhGrdMgrP mgr, FdoStringP datastoreName, FdoStringP tableName )
{
    return FdoStringP(L"dbo:") + table2class(mgr, tableName);
}

bool SqlServerSchemaMgrTests::SupportsBaseObjects()
{
    return true;
}

FdoInt64 SqlServerSchemaMgrTests::GetSrid( int index )
{
    FdoInt64 srid = 0;

    switch ( index ) {
    case 1: 
        srid = 4120;
        break;
    case 2: 
        srid = 4236;
        break;
    case 3: 
        srid = 2001;
        break;
    case 4: 
        srid = 1000;
        break;
    case 5: 
        srid = 26741;
        break;
    case 6: 
        srid = 32185;
        break;
    }

    return srid;
}

