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
#include "SchemaMgrTests.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/SpatialIndex.h>
#include <Sm/Ph/Rd/FkeyReader.h>
#include "../SchemaMgr/Ph/Owner.h"

FdoString* SchemaMgrTests::DB_NAME_SUFFIX =           L"_schema_mgr";
FdoString* SchemaMgrTests::DB_NAME_COPY_SUFFIX =      L"_schema_mgr_copy";
FdoString* SchemaMgrTests::DB_NAME_FOREIGN_SUFFIX =   L"_schema_mgr_f";
FdoString* SchemaMgrTests::DB_NAME_CONFIGERR_SUFFIX =   L"_schema_mgr_configerr";

SchemaMgrTests::SchemaMgrTests (void)
{
}

SchemaMgrTests::~SchemaMgrTests (void)
{
}

void SchemaMgrTests::setUp ()
{
}

void SchemaMgrTests::tearDown ()
{
}

void SchemaMgrTests::testGenDefault ()
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

        int lt_mode = atoi( UnitTestUtil::GetEnv("lt_mode","0") );

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

        FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"RTABLE1" ));
        SetLtLck(table, lt_mode);
        FdoSmPhColumnP column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );
        FdoSmPhColumnP fkeyColumn = table->CreateColumnInt32( L"TABLE1_ID", false );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        FdoSmPhFkeyP fkey = table->CreateFkey( "FK_RTABLE1_TABLE1", phMgr->GetDcDbObjectName("TABLE1") );
        fkey->AddFkeyColumn( fkeyColumn, L"ID" );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE1" ));
        SetLtLck(table, lt_mode);
        column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnBLOB( L"BLOB_COLUMN", true );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );
        column = table->CreateColumnDouble( L"DELETE_COLUMN", true );
        column = table->CreateColumnSingle( L"SINGLE_COLUMN", true );
        column = table->CreateColumnDouble( L"DOUBLE_COLUMN", true );
        column = table->CreateColumnDecimal( L"DECIMAL_COLUMN", true, 10, 5 );
        column = table->CreateColumnBool( L"BOOL_COLUMN", true );
        column = table->CreateColumnByte( L"BYTE_COLUMN", true );
        column = table->CreateColumnInt16( L"INT16_COLUMN", true );
        column = table->CreateColumnInt32( L"INT32_COLUMN", true );
        column = table->CreateColumnInt64( L"INT64_COLUMN", true );
        column = table->CreateColumnDbObject( L"OBJECT_NAME", true );
        column = table->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL, true, false );
        AddProviderColumns( table );
        FdoSmPhCheckConstraintP constraint = new FdoSmPhCheckConstraint( L"int16_check", L"INT16_COLUMN", L"INT16_COLUMN < 20000" );
        table->AddCkeyCol( constraint );
		constraint = new FdoSmPhCheckConstraint( L"decimal_check", L"DECIMAL_COLUMN", L"DECIMAL_COLUMN > BYTE_COLUMN" );
		table->AddCkeyCol( constraint );
        constraint = new FdoSmPhCheckConstraint( L"int32_check", L"INT32_COLUMN", L"INT32_COLUMN = 45 or INT32_COLUMN > 100" );
        table->AddCkeyCol( constraint );
        constraint = new FdoSmPhCheckConstraint( L"single_check", L"SINGLE_COLUMN", L"SINGLE_COLUMN = 45 or DOUBLE_COLUMN > 100" );
        table->AddCkeyCol( constraint );

        FdoSmPhTableP table2 = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE2" ));
        SetLtLck(table2, lt_mode);
        column = table2->CreateColumnInt32( L"ID", false );
        table2->AddPkeyCol( column->GetName() );
        column = table2->CreateColumnChar( L"STRING_COLUMN", false, 50 );
        column = table2->CreateColumnDouble( L"DOUBLE_COLUMN", true );
        column = table2->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL );

        FdoSmPhViewP view1 = owner->CreateView( phMgr->GetDcDbObjectName(L"VIEW1"), L"", owner->GetName(), phMgr->GetDcDbObjectName(L"TABLE1" ));
        column = view1->CreateColumnInt32( L"ID", false, false, L"ID" );
        if ( SupportsViewPkey() ) 
            view1->AddPkeyCol( L"ID" );
        column = view1->CreateColumnDecimal( L"DEC_COL_RENAME", true, 10, 5, L"DECIMAL_COLUMN" );
        column = view1->CreateColumnByte( L"BYTE_COLUMN", true, L"BYTE_COLUMN" );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE3" ));
        SetLtLck(table, lt_mode);
        column = table->CreateColumnChar( L"KEY1", false, 10 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"KEY2", false, 10 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnDouble( L"DOUBLE_COLUMN", true );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE4" ));
        SetLtLck(table, lt_mode);
        column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );
        column = table->CreateColumnInt16( L"INT16_COLUMN", true );
        column = table->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL );

        // Test the clearing of the owner's not found object list

        // Put RTABLE2 on the list.
        owner->FindDbObject( phMgr->GetDcDbObjectName(L"RTABLE2") );
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"RTABLE2" ));
        SetLtLck(table, lt_mode);
        column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );
        FdoSmPhColumnP fkeyColumn1 = table->CreateColumnInt32( L"TABLE1_ID", false );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );
        FdoSmPhColumnP fkeyColumn2 = table->CreateColumnChar( L"TABLE3_KEY1", false, 10 );
        FdoSmPhColumnP fkeyColumn3 = table->CreateColumnChar( L"TABLE3_KEY2", false, 10 );

        fkey = table->CreateFkey( "FK_RTABLE2_TABLE1", phMgr->GetDcDbObjectName("TABLE1" ));
        fkey->AddFkeyColumn( fkeyColumn1, L"ID" );

        fkey = table->CreateFkey( "FK_RTABLE2_TABLE3", phMgr->GetDcDbObjectName("TABLE3" ));
        fkey->AddFkeyColumn( fkeyColumn2, L"KEY1" );
        fkey->AddFkeyColumn( fkeyColumn3, L"KEY2" );

        FdoSmPhViewP view2 = owner->CreateView( phMgr->GetDcDbObjectName(L"VIEW2"), L"", owner->GetName(), phMgr->GetDcDbObjectName(L"TABLE2" ));
        column = view2->CreateColumnInt32( L"ID", false, false, L"ID" );
        column = view2->CreateColumnChar( L"STRING_COLUMN", false, 50, L"STRING_COLUMN" );

        column = view2->CreateColumnDouble( L"DOUBLE_COLUMN", true, L"DOUBLE_COLUMN" );
        column = view2->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL, true, true, false, L"GEOM_COLUMN" );

        FdoSmPhViewP view3 = owner->CreateView( phMgr->GetDcDbObjectName(L"VIEW3"), L"", owner->GetName(), phMgr->GetDcDbObjectName(L"TABLE4" ));
        column = view3->CreateColumnInt32( L"ID", false, false, L"ID" );
        column = view3->CreateColumnChar( L"STRING_COLUMN", false, 50, L"STRING_COLUMN" );
        if ( SupportsViewPkey() ) 
            view3->AddPkeyCol( column->GetName() );
        column = view3->CreateColumnInt16( L"INT16_COLUMN", true, false, L"INT16_COLUMN" );
        column = view3->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL, true, true, false, L"GEOM_COLUMN" );

#ifndef RDBI_DEF_SSQL
        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE5" ));
        column = table->CreateColumnDecimal( L"ID", false, 10, 2 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"RTABLE5" ));
        column = table->CreateColumnDecimal( L"ID", false, 10, 2 );
        table->AddPkeyCol( column->GetName() );
        FdoSmPhColumnP fkeyColumn5 = table->CreateColumnDouble( L"TABLE5_ID", false );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        fkey = table->CreateFkey( "FK_RTABLE5_TABLE5", phMgr->GetDcDbObjectName("TABLE5" ));
        fkey->AddFkeyColumn( fkeyColumn5, L"ID" );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE6" ));
        column = table->CreateColumnDecimal( L"ID1", false, 10, 2 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"ID2", false, 50 );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"RTABLE6" ));
        column = table->CreateColumnDecimal( L"ID", false, 10, 2 );
        table->AddPkeyCol( column->GetName() );
        FdoSmPhColumnP fkeyColumn6a = table->CreateColumnDouble( L"TABLE6_ID1", false );
        FdoSmPhColumnP fkeyColumn6b = table->CreateColumnChar( L"TABLE6_ID2", false, 50 );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        fkey = table->CreateFkey( "FK_RTABLE6_TABLE6", phMgr->GetDcDbObjectName("TABLE6" ));
        fkey->AddFkeyColumn( fkeyColumn6a, L"ID1" );
        fkey->AddFkeyColumn( fkeyColumn6b, L"ID2" );
#endif

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE7" ));
        column = table->CreateColumnInt64( L"ID", false, true);
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"RTABLE7" ));
        column = table->CreateColumnInt64( L"ID", false);
        table->AddPkeyCol( column->GetName() );
        FdoSmPhColumnP fkeyColumn7 = table->CreateColumnInt64( L"TABLE7_ID", false, true);
        column = table->CreateColumnChar( L"STRING_COLUMN", false, 50 );

        fkey = table->CreateFkey( "FK_RTABLE7_TABLE7", phMgr->GetDcDbObjectName("TABLE7" ));
        fkey->AddFkeyColumn( fkeyColumn7, L"ID" );

        FdoSmPhBatchColumnsP ukeys = table->GetUkeyColumns(); 
        FdoSmPhColumnsP	ukeyColumns = new FdoSmPhColumnCollection();
        ukeys->Add( ukeyColumns );
    	table->AddUkeyCol( ukeys->GetCount() - 1, L"TABLE7_ID" );

        database->Commit();
        
        owner->DiscardDbObject(table);
        // Make sure RTABLE2 got removed from the not found list.
        CPPUNIT_ASSERT( owner->FindDbObject(phMgr->GetDcDbObjectName(L"RTABLE2")) != NULL );

        printf( "Updating original schema ...\n" );

        view2->SetElementState( FdoSchemaElementState_Deleted );
        view2->Commit();

        table2->SetElementState( FdoSchemaElementState_Deleted );
        table2->Commit();

        table = owner->GetDbObject( phMgr->GetDcDbObjectName(L"TABLE1" ))->SmartCast<FdoSmPhTable>();
        column = table->GetColumns()->GetItem( L"DELETE_COLUMN" );
        column->SetElementState( FdoSchemaElementState_Deleted );
        column = table->CreateColumnChar( L"INSERT_COLUMN", true, 25 );
        column = table->GetColumns()->GetItem( L"GEOM_COLUMN" );
        column->SetElementState( FdoSchemaElementState_Deleted );
        column = table->CreateColumnGeom( L"NEW_GEOM_COLUMN", (FdoSmPhScInfo*) NULL, true, false );
        table->Commit();


        printf( "Dumping original schema ...\n" );

        phMgr->XMLSerialize( UnitTestUtil::GetOutputFileName( L"smtables_physical.xml" ) );

        printf( "Spot-checking Physical Schema retrieval from datastore ...\n" );

        mgr = conn->CreateSchemaManager();
        phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();
        view1 = phMgr->FindDbObject( phMgr->GetDcDbObjectName(L"VIEW1"), datastore, L"", false ).p->SmartCast<FdoSmPhView>();
        CPPUNIT_ASSERT( view1 != NULL );
        CPPUNIT_ASSERT( view1->GetColumns()->GetCount() == 3 );
        CPPUNIT_ASSERT( wcscmp(FdoSmPhColumnP(view1->GetColumns()->GetItem(0))->GetName(),L"ID") == 0 );
        CPPUNIT_ASSERT( wcscmp(FdoSmPhColumnP(view1->GetColumns()->GetItem(1))->GetName(),L"DEC_COL_RENAME") == 0 );
        CPPUNIT_ASSERT( wcscmp(FdoSmPhColumnP(view1->GetColumns()->GetItem(2))->GetName(),L"BYTE_COLUMN") == 0 );

        view2 = phMgr->FindDbObject( phMgr->GetDcDbObjectName(L"VIEW2"), datastore, L"", false ).p->SmartCast<FdoSmPhView>();
        CPPUNIT_ASSERT( view2 == NULL );

        VldLtReaders( conn, datastore );

		printf( "Exporting schema from datastore ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoIoMemoryStreamP stream1 = FdoIoMemoryStream::Create();

        UnitTestUtil::ExportDb( fdoConn, stream1 );
		UnitTestUtil::Stream2File( stream1, UnitTestUtil::GetOutputFileName( L"smtables_logical.xml" ) );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );
/* TODO: doesn't work on SqlServer
        owner->SetElementState( FdoSchemaElementState_Deleted );
        owner->Commit();
*/
        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;

        printf( "Creating copy schema ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            true,
            true,
            DB_NAME_COPY_SUFFIX,
            NULL,
            NULL,
            0
        );

        printf( "Copying spatial contexts ...\n" );

        stream1->Reset();
        FdoXmlSpatialContextSerializer::XmlDeserialize( 
            fdoConn,
            FdoXmlSpatialContextReaderP(
                FdoXmlSpatialContextReader::Create(
                    FdoXmlReaderP(
                        FdoXmlReader::Create(stream1)
                    )
                )
            )
        );

		printf( "Copying schema ...\n" );

        FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
        stream1->Reset();
        schemas->ReadXml( stream1 );   

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        stream1->Reset();
        mappings->ReadXml( stream1 );

    	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) fdoConn->CreateCommand(FdoCommandType_ApplySchema);

        FdoInt32 idx;

        for ( idx = 0; idx < schemas->GetCount(); idx++ ) {
            FdoFeatureSchemaP schema = schemas->GetItem(idx);
            pCmd->SetFeatureSchema( schema );

            if ( wcscmp(schema->GetName(),L"guest") == 0 ) {
                FdoPhysicalSchemaMappingP overrides = 
                    (FdoPhysicalSchemaMapping*) mappings->GetItem( fdoConn, schema->GetName() );
                if ( overrides )
                    pCmd->SetPhysicalMapping( overrides );
            }

            // Special case for MySQL. Classes from views do not get identity.
            // class "view3" is a feature class since it has geometry.
            // However, feature classes without identity are not allowed in FDO-enabled 
            // datastores, so remove this class when it has no identity.

            FdoClassesP classes = schema->GetClasses();
            FdoClassDefinitionP view3Class = classes->FindItem( L"view3" );
            if ( view3Class && (view3Class->GetClassType() == FdoClassType_FeatureClass) ) {
                FdoDataPropertiesP idProps = view3Class->GetIdentityProperties();
                if ( idProps->GetCount() == 0 )
                    classes->Remove( view3Class );
            }

            pCmd->Execute();
        }

        FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();

        UnitTestUtil::ExportDb( 
            fdoConn, 
            stream2, 
            NULL, 
            false, 
            FdoStringP(L"Fdo") + datastore,
            L"AutoGen"
        );

        UnitTestUtil::Stream2File( stream2, UnitTestUtil::GetOutputFileName( L"gen_default1.xml" ) );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_COPY_SUFFIX );

        // The generated XML file differs depending whether or not it is generated on a
        // SQL Server 2000 or 2005 instance. The difference is with a table system property
        // (text Filegroup) that is set in SQL Server 2000 when the test tables are created,
        // but not in 2005. To compensate for this difference, the generated XML file will be
        // checked against a second master file if the checking of the generated file against
        // the first master file fails. An error will be issued only in case the second check
        // fails. Since the content of the master files are identical with the exception of the
        // system property mentioned above, an error caused by an implementation issue should
        // be detected by both master files.

        try
        {
	        UnitTestUtil::CheckOutput( 
                FdoStringP::Format(L"gen_default1_%ls_master.txt", (FdoString*) providerName),
                UnitTestUtil::GetOutputFileName( L"gen_default1.xml" )
            );
        }
        catch (CppUnit::Exception exception)
        {
            // Only execute the second check if the provider is SQL Server. Otherwise issue the
            // exception.
            if ( providerName.ICompare(L"SqlServer") == 0 )
	            UnitTestUtil::CheckOutput( 
                    FdoStringP::Format(L"gen_default1_%ls2005_master.txt", (FdoString*) providerName),
                    UnitTestUtil::GetOutputFileName( L"gen_default1.xml" )
                );
            else
                throw exception;
        }

        printf( "Opening original db with config doc ...\n" );
        fdoConn = UnitTestUtil::GetProviderConnectionObject();
        stream1->Reset();

        fdoConn->SetConfiguration(stream1);
        FdoStringP dbConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_NAME_SUFFIX);
        fdoConn->SetConnectionString ( dbConnectString);
        fdoConn->Open();
 
        conn = CreateStaticConnection();
        conn->connect();
        mgr = conn->CreateSchemaManager();
        phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();
        FdoFeatureSchemaP schema = fsc->GetItem(0);
        FdoClassesP classes = schema->GetClasses();

        FdoClassDefinitionP classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"VIEW1") );
        FdoPtr<FdoClassCapabilities> cap = classDef->GetCapabilities();
        CPPUNIT_ASSERT( !cap->SupportsLongTransactions() );

        VldDefaultConfig( classes );

        FdoIoMemoryStreamP stream3 = FdoIoMemoryStream::Create();

        UnitTestUtil::ExportDb( 
            fdoConn, 
            stream3, 
            NULL, 
            false, 
            FdoStringP(L"Fdo") + datastore,
            L"AutoGen"
        );

        stream3->Reset();

        UnitTestUtil::Config2SortedFile( stream3, UnitTestUtil::GetOutputFileName(L"gen_default2.xml") );

        FdoStringP masterFileName = FdoStringP::Format( L"gen_default1_%ls_master.txt", (FdoString*) providerName );
        FdoIoFileStreamP masterStream = FdoIoFileStream::Create( masterFileName, L"rt" );
        UnitTestUtil::Config2SortedFile( masterStream, UnitTestUtil::GetOutputFileName(masterFileName) );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_SUFFIX );

        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;

#if 0 
//TODO: investigate file differences
//        - check constraints missing from output
//        - output has schema mappings, master does not.
        UnitTestUtil::CheckOutput( 
            UnitTestUtil::GetOutputFileName(masterFileName),
            UnitTestUtil::GetOutputFileName( L"gen_default2.xml" )
        );
#endif
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

void SchemaMgrTests::testGenGeom ()
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

        int lt_mode = atoi( UnitTestUtil::GetEnv("lt_mode","0") );

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

        CreateMultiGeomTable( owner, L"INDEX_WINS1", 8, 0x01, 0xa3 );
        CreateMultiGeomTable( owner, L"INDEX_WINS2", 3, 0x02, 0x00 );
        CreateMultiGeomTable( owner, L"INDEX_WINS3", 3, 0x01, 0x02 );
        CreateMultiGeomTable( owner, L"INDEX_WINS4", 3, 0x03, 0x02 );
        CreateMultiGeomTable( owner, L"INDEX_TIE1", 8, 0x11, 0x02 );
        CreateMultiGeomTable( owner, L"INDEX_TIE2", 8, 0x11, 0x22 );
        CreateMultiGeomTable( owner, L"INDEX_TIE3", 8, 0x11, 0x33 );
        CreateMultiGeomTable( owner, L"NNULL_WINS1", 1, 0x00, 0x01 );
        CreateMultiGeomTable( owner, L"NNULL_WINS2", 2, 0x00, 0x02 );
        CreateMultiGeomTable( owner, L"NNULL_TIE", 3, 0x00, 0x05 );
        CreateMultiGeomTable( owner, L"NO_WIN", 3, 0x00, 0x00 );
        CreateMultiGeomTable( owner, L"ONE_GEOM", 1, 0x00, 0x00 );

        owner->Commit();

        printf( "Reverse-engineering datastore ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();
        FdoFeatureSchemaP schema = fsc->GetItem(0);
        FdoClassesP classes = schema->GetClasses();

        FdoClassDefinitionP classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_WINS1") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_WINS2") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_WINS3") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_WINS4") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_TIE1") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_TIE2") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"INDEX_TIE3") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"NNULL_WINS1") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"NNULL_WINS2") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"NNULL_TIE") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"NO_WIN") );
        VldGenGeom( classDef );
        classDef = classes->FindItem( phMgr->GetDcDbObjectName(L"ONE_GEOM") );
        VldGenGeom( classDef );

        UnitTestUtil::CloseConnection( fdoConn, false, DB_NAME_COPY_SUFFIX );

        phMgr = NULL;
        mgr = NULL;
        delete conn;
        conn = NULL;

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

void SchemaMgrTests::testGenConfig1 ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        char prvenv[100];
        sprintf( prvenv, "provider=%s", conn->GetServiceName() );
        FdoStringP provider = conn->GetServiceName();
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        int lt_mode = atoi( UnitTestUtil::GetEnv("lt_mode","0") );

        printf( "\nOpening connection ...\n" );

        conn->connect();

        printf( "Predeleting original schema ...\n" );

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );

        FdoStringP fDatastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
        );

        FdoStringP datastorePrefix = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore")
        );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        printf( "Creating original schema ...\n" );

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );

        FdoSmPhOwnerP fOwner = phMgr->FindOwner( fDatastore, L"", false );
        if ( fOwner ) {
            fOwner->SetElementState( FdoSchemaElementState_Deleted );
            fOwner->Commit();
        }

        fOwner = database->CreateOwner(
            fDatastore, 
            false
        );
        fOwner->SetPassword( L"test" );

        CreateTableGroup( owner, L"AB_", 3, lt_mode );
        CreateTableGroup( owner, L"CD_", 5, lt_mode );
        CreateTableGroup( owner, L"EF_", 2, lt_mode );
        CreateTableGroup( owner, L"GH_", 8, lt_mode );
        CreateTableGroup( fOwner, L"KL_", 1, lt_mode );
        CreateTableGroup( fOwner, L"MN_", 1, lt_mode );

        CreateFkey( owner, L"CD_TABLE2", L"CD_TABLE1" );
        CreateFkey( owner, L"AB_TABLE2", L"AB_TABLE3" );
        CreateFkey( owner, L"EF_TABLE2", L"CD_TABLE4" );
        CreateFkey( owner, L"CD_TABLE5", L"GH_TABLE4" );
        CreateFkey( owner, L"CD_TABLE3", L"AB_TABLE2" );
        CreateFkey( owner, L"CD_TABLE1", L"EF_TABLE2" );
        CreateFkey( owner, L"GH_TABLE1", L"GH_TABLEWILD" );
        CreateFkey( owner, L"GH_TABLE2", L"CD_TABLEWILD" );

        // TODO: test associations between foreign classes.

        GdbiConnection* gdbiConn = phMgr->GetGdbiConnection();
        GdbiCommands* gdbiCommands = gdbiConn->GetCommands();

        gdbiCommands->tran_begin( "Owner" );
        owner->Commit();
        gdbiCommands->tran_end( "Owner" );

        gdbiCommands->tran_begin( "FOwner" );
        fOwner->Commit();
        gdbiCommands->tran_end( "FOwner" );

        phMgr = NULL;
        mgr = NULL;
        delete conn;

        printf( "Getting Feature Schemas using config doc ...\n" );

        FdoStringP dbConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, L"_schema_mgr");

        fdoConn = UnitTestUtil::GetProviderConnectionObject();
        FdoIoFileStreamP stream1 = FdoIoFileStream::Create( L"config1_in.xml", L"rt" );
        FdoIoStreamP stream2 = OverrideBend( stream1, L"(user)", datastorePrefix );

        fdoConn->SetConfiguration(CvtConf(stream2));
        fdoConn->SetConnectionString ( dbConnectString);
        fdoConn->Open();
 
        FdoIoMemoryStreamP stream3 = FdoIoMemoryStream::Create();
        UnitTestUtil::ExportDb( fdoConn, stream3 );
        FdoIoStreamP stream4 = OverrideBend( stream3, datastorePrefix, L"(user)" );
        UnitTestUtil::Stream2File( stream4, UnitTestUtil::GetOutputFileName( L"schemaGenConfig1.xml" ) );

        UnitTestUtil::CloseConnection( fdoConn, false, L"_schema_mgr" );

#ifdef RDBI_DEF_ORA
	    UnitTestUtil::CheckOutput( "schemaGenConfig1_master.txt", UnitTestUtil::GetOutputFileName( L"schemaGenConfig1.xml" ) );
#endif
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

void SchemaMgrTests::testGenKeys ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        char prvenv[100];
        sprintf( prvenv, "provider=%s", conn->GetServiceName() );
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif
        FdoStringP provider = conn->GetServiceName();

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        int lt_mode = atoi( UnitTestUtil::GetEnv("lt_mode","0") );

        printf( "Creating original schema ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        GdbiConnection* gdbiConn = phMgr->GetGdbiConnection();
        GdbiCommands* gdbiCommands = gdbiConn->GetCommands();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );
    
        // Tests choosing primary key over index as the identity    
        FdoSmPhTableP table = CreateIxTable(owner, L"TABLE_IX1", lt_mode );
        AddPkey( table );
        AddIndex( table, true, L"ALT_KEY1", L"INT16_COL1" );

        FdoSmPhViewP view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX1A"), table->GetName() );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX1B"), view->GetName() );
        FdoSmPhColumnsP columns = view->GetColumns();
        columns->RemoveAt( columns->IndexOf(phMgr->GetDcColumnName(L"ID")) );

        // Tests choosing heavier index with fewer columns
        table = CreateIxTable(owner, L"TABLE_IX2", 0 );
        AddIndex( table, true, L"IX_21", L"INT16_COL1 INT16_COL2" );
        AddIndex( table, true, L"IX_22", L"STRING_COL5" );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX2A"), table->GetName() );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX2B"), view->GetName() );
        columns = view->GetColumns();
        columns->RemoveAt( columns->IndexOf(phMgr->GetDcColumnName(L"STRING_COL5")) );

        FdoStringP createIx2cSql = FdoStringP::Format( 
            L"create view view_ix2c ( id, int16_col1 ) as select id, int16_col1 from %ls", 
            (FdoString*)(view->GetDbQName())
        );

        // Multi-column index tests
        table = CreateIxTable(owner, L"TABLE_IX3", 0 );
        AddIndex( table, true, L"IX_31", L"INT16_COL1 INT16_COL2" );
        AddIndex( table, true, L"IX_32", L"STRING_COL1 STRING_COL2" );
        AddIndex( table, true, L"IX_33", L"INT16_COL1 STRING_COL1" );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX3A"), table->GetName() );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX3B"), view->GetName() );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX3C"), view->GetName() );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX3D"), view->GetName() );

        // string column tests
        table = CreateIxTable(owner, L"TABLE_IX4", 0 );
        AddIndex( table, true, L"IX_41", L"STRING_COL1" );
        AddIndex( table, true, L"IX_42", L"INT16_COL1" );
        AddIndex( table, true, L"IX_43", L"STRING_COL2" );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX4A"), table->GetName() );

        // Test skipping index that is too heavy.
        // This index is too long to create on MySql or SqlServer so
        // test only for Oracle.
#ifdef RDBI_DEF_ORA
        table = CreateIxTable(owner, L"TABLE_IX5", 0 );
        AddIndex( table, true, L"IX_51", L"STRING_COL3 STRING_COL4" );
        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX5A"), table->GetName() );
#endif

        // Test skipping non-unique index
        table = CreateIxTable(owner, L"TABLE_IX6", 0 );
        AddIndex( table, true, L"IX_61", L"INT16_COL1 INT16_COL2" );
        AddIndex( table, true, L"IX_62", L"STRING_COL1 STRING_COL2" );
        AddIndex( table, false, L"IX_63", L"INT16_COL1 STRING_COL2" );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX6A"), table->GetName() );
        FdoStringP createIx6bSql = FdoStringP::Format( 
            L"create view view_ix6b ( icol1, icol2, string_col1, string_col2 ) as select int16_col1, int16_col2, string_col1, string_col2 from %ls", 
            (FdoString*)(view->GetDbQName())
        );

        FdoStringP createIx6cSql = FdoStringP::Format( 
            L"create view view_ix6c ( string_col1, int16_col2, int16_col1, string_col2 ) as select int16_col1, int16_col2, string_col1, string_col2 from %ls", 
            (FdoString*)(view->GetDbQName())
        );

        // Test date columns
        table = CreateIxTable(owner, L"TABLE_IX7", 0 );
        AddIndex( table, true, L"IX_71", L"DATE_COL1 STRING_COL2" );
        AddIndex( table, true, L"IX_72", L"STRING_COL5 STRING_COL2" );
        AddIndex( table, true, L"IX_73", L"STRING_COL6 STRING_COL2" );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX7A"), table->GetName() );

        // Test id'less table.
        table = CreateIxTable(owner, phMgr->GetDcDbObjectName(L"TABLE_IX8"), 0 );

        view = CreateIxView( owner, phMgr->GetDcDbObjectName(L"VIEW_IX8A"), table->GetName() );

        FdoStringP createJoinSql = L"create view view_join ( id, string_col5 ) as select a.id, b.string_col5 from table_ix1 a, table_ix2 b";

        database->Commit();

        FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();

        grdOwner->ActivateAndExecute( (FdoString*) createIx2cSql );
        grdOwner->ActivateAndExecute( (FdoString*) createIx6bSql );
        grdOwner->ActivateAndExecute( (FdoString*) createIx6cSql );
        grdOwner->ActivateAndExecute( (FdoString*) createJoinSql );

        mgr = NULL;
        phMgr = NULL;
        conn->disconnect();
        delete conn;
        conn = CreateStaticConnection();
        conn->connect();
        mgr = conn->CreateSchemaManager();
        phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        printf( "Autogenerating schema from datastore ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP schemas = pDescCmd->Execute();
        FdoFeatureSchemaP schema = schemas->GetItem(0);
        FdoClassesP classes = schema->GetClasses();

#ifdef RDBI_DEF_ORA
        CPPUNIT_ASSERT( classes->GetCount() == 25 );
#else
        CPPUNIT_ASSERT( classes->GetCount() == 23 );
#endif
        FdoInt32 pass;
        FdoClassDefinitionP featClass;
        FdoDataPropertiesP idProps;
        FdoDataPropertyP prop;

        for ( pass = 0; pass < 2; pass++ ) {
            featClass = classes->GetItem( table2class(phMgr, (pass==0) ? L"TABLE_IX1" : L"VIEW_IX1A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 1 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"ID")) == 0 );
            }
            else {
                CPPUNIT_ASSERT( idProps->GetCount() == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX2" : L"VIEW_IX2A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 1 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL5")) == 0 );
            }
            else {
                CPPUNIT_ASSERT( idProps->GetCount() == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX3" : L"VIEW_IX3A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 2 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );
                prop = idProps->GetItem(1);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL2")) == 0 );
            }
            else {
                CPPUNIT_ASSERT( idProps->GetCount() == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX4" : L"VIEW_IX4A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 1 );
                prop = idProps->GetItem(0);
#ifdef RDBI_DEF_ORA
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL2")) == 0 );
		        featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX5" : L"VIEW_IX5A") );
		        idProps = featClass->GetIdentityProperties();
		        CPPUNIT_ASSERT( idProps->GetCount() == 0 );        
#else
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );
#endif
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX6" : L"VIEW_IX6A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 2 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );
                prop = idProps->GetItem(1);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL2")) == 0 );
            }
            else {
                CPPUNIT_ASSERT( idProps->GetCount() == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX7" : L"VIEW_IX7A") );
            idProps = featClass->GetIdentityProperties();
            if ( SupportsBaseObjects() || (pass == 0) ) {
                CPPUNIT_ASSERT( idProps->GetCount() == 2 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL5")) == 0 );
                prop = idProps->GetItem(1);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL2")) == 0 );
            }
            else {
                CPPUNIT_ASSERT( idProps->GetCount() == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr,(pass==0) ? L"TABLE_IX8" : L"VIEW_IX8A") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 0 );        
        }

        if ( SupportsBaseObjects() ) {
            featClass = classes->GetItem( table2class(phMgr, L"VIEW_IX1B") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 1 );
            prop = idProps->GetItem(0);
            CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );

            featClass = classes->GetItem( table2class(phMgr, L"VIEW_IX2B") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 2 );
            prop = idProps->GetItem(0);
            CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );
            prop = idProps->GetItem(1);
            CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL2")) == 0 );

            featClass = classes->GetItem( table2class(phMgr, L"VIEW_IX2C") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 0 );

            for ( pass = 0; pass < 3; pass++ ) {
                FdoStringP viewName = FdoStringP::Format( L"VIEW_IX3%c", 'B' + pass );
                featClass = classes->GetItem( table2class(phMgr, viewName) );
                idProps = featClass->GetIdentityProperties();
                CPPUNIT_ASSERT( idProps->GetCount() == 2 );
                prop = idProps->GetItem(0);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL1")) == 0 );
                prop = idProps->GetItem(1);
                CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"INT16_COL2")) == 0 );
            }

            featClass = classes->GetItem( table2class(phMgr, L"VIEW_IX6B") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 2 );
            prop = idProps->GetItem(0);
            CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL1")) == 0 );
            prop = idProps->GetItem(1);
            CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL2")) == 0 );

            featClass = classes->GetItem( table2class(phMgr, L"VIEW_IX6C") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 0 );

            featClass = classes->GetItem( table2class(phMgr, L"VIEW_JOIN") );
            idProps = featClass->GetIdentityProperties();
            CPPUNIT_ASSERT( idProps->GetCount() == 0 );
        }

        UnitTestUtil::CloseConnection( fdoConn, false, L"_schema_mgr" );

        printf( "Updating original schema ...\n" );

        conn = CreateStaticConnection();
        conn->connect();
        mgr = conn->CreateSchemaManager();
        phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();
        database = phMgr->GetDatabase();
        owner = phMgr->FindOwner( datastore, L"", false );

        table = owner->FindDbObject( phMgr->GetDcDbObjectName(L"TABLE_IX1") ).p->SmartCast<FdoSmPhTable>();
        FdoSmPhIndexP index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"ALT_KEY1") );
        index->SetElementState( FdoSchemaElementState_Deleted );
        AddIndex( table, false, L"IX_11", L"STRING_COL2" );

        table = owner->FindDbObject( phMgr->GetDcDbObjectName(L"TABLE_IX2") ).p->SmartCast<FdoSmPhTable>();
        index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"IX_22") );
        CPPUNIT_ASSERT( index->GetIsUnique() );
        index->SetElementState( FdoSchemaElementState_Deleted );
        AddIndex( table, true, L"IX_23", L"STRING_COL2" );

        table = owner->FindDbObject( phMgr->GetDcDbObjectName(L"TABLE_IX4") ).p->SmartCast<FdoSmPhTable>();
        index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"IX_41") );
        index->SetElementState( FdoSchemaElementState_Deleted );
        index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"IX_42") );
        index->SetElementState( FdoSchemaElementState_Deleted );
        index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"IX_43") );
        index->SetElementState( FdoSchemaElementState_Deleted );

        table = owner->FindDbObject( phMgr->GetDcDbObjectName(L"TABLE_IX7") ).p->SmartCast<FdoSmPhTable>();
        index = table->GetIndexes()->GetItem( phMgr->GetDcColumnName(L"IX_72") );
        index->SetElementState( FdoSchemaElementState_Deleted );

        database->Commit();

        printf( "Autogenerating updated schema from datastore ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            L"_schema_mgr"
        );

        pDescCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        schemas = pDescCmd->Execute();
        schema = schemas->GetItem(0);
        classes = schema->GetClasses();

#ifdef RDBI_DEF_ORA
        CPPUNIT_ASSERT( classes->GetCount() == 25 );
#else
        CPPUNIT_ASSERT( classes->GetCount() ==  23);
#endif
        featClass = classes->GetItem( table2class(phMgr,L"TABLE_IX2") );
        idProps = featClass->GetIdentityProperties();
        CPPUNIT_ASSERT( idProps->GetCount() == 1 );
        prop = idProps->GetItem(0);
        CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL2")) == 0 );

        featClass = classes->GetItem( table2class(phMgr,L"TABLE_IX4") );
        idProps = featClass->GetIdentityProperties();
        CPPUNIT_ASSERT( idProps->GetCount() == 0 );

        featClass = classes->GetItem( table2class(phMgr,L"TABLE_IX7") );
        idProps = featClass->GetIdentityProperties();
        CPPUNIT_ASSERT( idProps->GetCount() == 2 );
        prop = idProps->GetItem(0);
        CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"DATE_COL1")) == 0 );
        prop = idProps->GetItem(1);
        CPPUNIT_ASSERT( wcscmp(prop->GetName(), phMgr->GetDcColumnName(L"STRING_COL2")) == 0 );

        phMgr = NULL;
        mgr = NULL;
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

void SchemaMgrTests::testFKeys ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        char prvenv[100];
        sprintf( prvenv, "provider=%s", conn->GetServiceName() );
        FdoStringP provider = conn->GetServiceName();
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        printf( "Opening connection ...\n" );

        conn->connect();

        printf( "Predeleting datastores ...\n" );

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

		FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );

		FdoSmPhDatabaseP database = phMgr->GetDatabase();

		FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
		if ( owner ) {
			owner->SetElementState( FdoSchemaElementState_Deleted );
			owner->Commit();
		}

		printf( "Creating foreign keys ...\n" );

		owner = database->CreateOwner(
			datastore, 
			false
		);
		owner->SetPassword( L"test" );
        owner->Commit();

        // The following creates a table with 3 foreign keys. The table also
        // contains columns named the same as the foreign keys. This tests 
        // the name adjustment of Association Properties reverse-engineered
        // from these foreign keys.

		FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"rtable1") );
		FdoSmPhColumnP column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"id"), false );
		table->AddPkeyCol( column->GetName() );
		FdoSmPhColumnP fkeyColumn = table->CreateColumnInt32( phMgr->GetDcColumnName(L"table1_id"), false );
		
        // Create 3 foreign keys plus columns with same names as foreign keys
        FdoSmPhFkeyP fkey = table->CreateFkey( phMgr->GetDcColumnName("table1_id"), phMgr->GetDcDbObjectName("table1") );
		fkey->AddFkeyColumn( fkeyColumn, phMgr->GetDcColumnName(L"id") );
		fkeyColumn = table->CreateColumnInt32( phMgr->GetDcColumnName(L"table2_id"), false );
		fkey = table->CreateFkey( phMgr->GetDcColumnName("table2_id"), phMgr->GetDcDbObjectName("table2") );
		fkey->AddFkeyColumn( fkeyColumn, phMgr->GetDcColumnName(L"id") );
		fkeyColumn = table->CreateColumnInt32( phMgr->GetDcColumnName(L"table3_id"), false );
		fkey = table->CreateFkey( phMgr->GetDcColumnName("table3_id"), phMgr->GetDcDbObjectName("table3") );
		fkey->AddFkeyColumn( fkeyColumn, phMgr->GetDcColumnName(L"id") );

        // Create column same as adjusted name for 2nd Association property. Tests
        // property name that needs 2 adjustments
		column = table->CreateColumnInt32( L"Assoctable2_id", false );
        // The following forces the 3rd association property name to be 
        // adjusted 3 times.
		column = table->CreateColumnInt32( L"Assoctable3_id", false );
		column = table->CreateColumnInt32( L"Assoctable3_id1", false );
        OnTestFkeysCreateTable( table );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table1") );
		column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"id"), false );
		table->AddPkeyCol( column->GetName() );
		column = table->CreateColumnChar( phMgr->GetDcColumnName(L"string_column"), false, 50 );
        OnTestFkeysCreateTable( table );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table2") );
		column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"id"), false );
		table->AddPkeyCol( column->GetName() );
		column = table->CreateColumnChar( phMgr->GetDcColumnName(L"string_column"), false, 50 );
        OnTestFkeysCreateTable( table );

        table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table3") );
		column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"id"), false );
		table->AddPkeyCol( column->GetName() );
		column = table->CreateColumnChar( phMgr->GetDcColumnName(L"string_column"), false, 50 );
        OnTestFkeysCreateTable( table );

        owner->Commit();

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP schemas = pDescCmd->Execute();
        FdoFeatureSchemaP schema = schemas->GetItem(0);
        FdoClassesP classes = schema->GetClasses();

        FdoClassDefinitionP classDef = classes->GetItem( table2class(phMgr,L"rtable1") );
        FdoPropertiesP props = classDef->GetProperties();

        FdoPropertyP prop = props->FindItem( AssocNameTestFkeys(phMgr, 1) );
        CPPUNIT_ASSERT( prop != NULL );
        CPPUNIT_ASSERT( prop->GetPropertyType() == FdoPropertyType_AssociationProperty );

        prop = props->FindItem( AssocNameTestFkeys(phMgr, 2) );
        CPPUNIT_ASSERT( prop != NULL );
        CPPUNIT_ASSERT( prop->GetPropertyType() == FdoPropertyType_AssociationProperty );

        prop = props->FindItem( AssocNameTestFkeys(phMgr, 3) );
        CPPUNIT_ASSERT( prop != NULL );
        CPPUNIT_ASSERT( prop->GetPropertyType() == FdoPropertyType_AssociationProperty );

        phMgr = NULL;
        mgr = NULL;
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

void SchemaMgrTests::testViews ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        char prvenv[100];
        sprintf( prvenv, "provider=%s", conn->GetServiceName() );
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif
        FdoStringP provider = conn->GetServiceName();

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        printf( "Creating original schema ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        GdbiConnection* gdbiConn = phMgr->GetGdbiConnection();
        GdbiCommands* gdbiCommands = gdbiConn->GetCommands();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );

        FdoStringP fdatastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
        );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );
    
        FdoSmPhOwnerP fowner = phMgr->FindOwner( fdatastore, L"", false );
        if ( fowner ) {
            fowner->SetElementState( FdoSchemaElementState_Deleted );
            fowner->Commit();
        }

        fowner = database->CreateOwner(
            fdatastore, 
            false
        );
        fowner->SetPassword( L"test" );
    
        FdoSmPhTableP tableA = owner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE1") );
        FdoSmPhColumnP column = tableA->CreateColumnInt32( phMgr->GetDcColumnName(L"ID"), false );
        tableA->AddPkeyCol( column->GetName() );
        column = tableA->CreateColumnInt32( phMgr->GetDcColumnName(L"PARENT_ID"), false );
        column = tableA->CreateColumnInt16( phMgr->GetDcColumnName(L"INT16_COL1"), false );

        FdoSmPhTableP tableB = fowner->CreateTable( phMgr->GetDcDbObjectName(L"TABLE1") );
        column = tableB->CreateColumnInt32( phMgr->GetDcColumnName(L"ID"), false );
        tableB->AddPkeyCol( column->GetName() );
        column = tableB->CreateColumnInt32( phMgr->GetDcColumnName(L"PARENT_ID"), false );
        column = tableB->CreateColumnInt16( phMgr->GetDcColumnName(L"INT16_COL1"), false );

        database->Commit();

        FdoSmPhGrdOwnerP grdOwner = fowner->SmartCast<FdoSmPhGrdOwner>();
#ifdef RDBI_DEF_ORA
        grdOwner->ActivateAndExecute( L"grant select on table1 to public" );
#endif

        FdoStringP createViewSql = FdoStringP::Format( 
            L"create view view1 ( id, int16_col1, int16_col2b ) as select a.id, a.int16_col1, b.int16_col1 from %ls a, %ls b where a.id = b.parent_id", 
            (FdoString*)(tableA->GetDbQName()),
            (FdoString*)(tableA->GetDbQName())
        );
        grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();
        grdOwner->ActivateAndExecute( (FdoString*) createViewSql );

        createViewSql = FdoStringP::Format( 
            L"create view view2 ( id, int16_col1, int16_col2b ) as select a.id, a.int16_col1, b.int16_col1 from %ls a, %ls b where a.id = b.parent_id", 
            (FdoString*)(tableA->GetDbQName()),
            (FdoString*)(tableB->GetDbQName())
        );
        grdOwner->ActivateAndExecute( (FdoString*) createViewSql );

        mgr = NULL;
        phMgr = NULL;
        conn->disconnect();
        delete conn;
        conn = CreateStaticConnection();
        conn->connect();
        mgr = conn->CreateSchemaManager();
        phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        printf( "Autogenerating schema from datastore ...\n" );

        fdoConn = UnitTestUtil::CreateConnection(
            false,
            false,
            DB_NAME_SUFFIX
        );

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) fdoConn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP schemas = pDescCmd->Execute();
        FdoFeatureSchemaP schema = schemas->GetItem(0);
        FdoClassesP classes = schema->GetClasses();

        CPPUNIT_ASSERT( classes->GetCount() == 3 );

        FdoClassDefinitionP classDef = classes->GetItem( table2class(phMgr, L"VIEW1") );
        FdoDataPropertiesP idProps = classDef->GetIdentityProperties();
        CPPUNIT_ASSERT( idProps->GetCount() == (SupportsBaseObjects() ? 1 : 0) );

        classDef = classes->GetItem( table2class(phMgr, L"VIEW2") );
        idProps = classDef->GetIdentityProperties();
#ifndef RDBI_DEF_SSQL
        CPPUNIT_ASSERT( idProps->GetCount() == 0 );
#endif

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

void SchemaMgrTests::testConfigError ()
{
    FdoPtr<FdoIConnection> fdoConn;
    StaticConnection* conn = CreateStaticConnection();
 
    FdoStringP datastoreName = UnitTestUtil::GetEnviron("datastore");

    try
    {
        char prvenv[100];
        sprintf( prvenv, "provider=%s", conn->GetServiceName() );
#ifdef _WIN32
        _putenv( prvenv );
#else
        putenv( prvenv );
#endif
        FdoStringP provider = conn->GetServiceName();

        // Sets the other env.
        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        fdoConn = UnitTestUtil::CreateConnection(false, true, DB_NAME_CONFIGERR_SUFFIX );
        fdoConn->Close();
        fdoConn = NULL;

        FdoStringP dbConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore);

        fdoConn = UnitTestUtil::GetProviderConnectionObject();
        FdoIoFileStreamP stream1 = FdoIoFileStream::Create( L"config1_in.xml", L"rt" );

        fdoConn->SetConfiguration(stream1);
        fdoConn->SetConnectionString ( dbConnectString);
        fdoConn->Open();
 
        FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
        UnitTestUtil::ExportDb( fdoConn, stream2 );

        CPPUNIT_FAIL( "Setting config doc for datastore with MetaSchema was supposed to fail" );
    }
    catch (FdoException* e ) 
    {
#ifdef _WIN32
            FdoStringP expectedMessage = FdoStringP::Format(
                L" Cannot use configuration document with current connection, datastore '%ls' has MetaSchema. ",
                (FdoString*) datastoreName
            );
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' );
            if (pMessage) pMessage++;
            CPPUNIT_ASSERT( pMessage && expectedMessage.ICompare(pMessage) == 0 );
#endif
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

void SchemaMgrTests::testGenConfigGeom()
{
    //TODO: add tests for autogenerating geometric property attributes.
}

void SchemaMgrTests::CreateTableGroup( FdoSmPhOwnerP owner, FdoStringP prefix, FdoInt32 count, int lt_mode )
{
    int i;

    for ( i = 1; i <= count; i++ ) {
        FdoStringP tablename = FdoStringP::Format( L"%lsTABLE%d", (FdoString*) prefix, i );
        FdoSmPhTableP table = owner->CreateTable( tablename );
        SetLtLck(table, lt_mode);
        FdoSmPhColumnP column = table->CreateColumnInt32( L"ID", false );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnGeom( L"GEOM_COLUMN", (FdoSmPhScInfo*) NULL );
        column = table->CreateColumnInt32( L"FOREIGN_COLUMN", false );
        column = table->CreateColumnDouble( L"DOUBLE_COLUMN", true );

        if ( (prefix.ICompare(L"gh_") == 0) && (i == 5) ) {
            FdoSmPhCheckConstraintP constraint = new FdoSmPhCheckConstraint( L"double_check", L"DOUBLE_COLUMN", L"DOUBLE_COLUMN < 100.35" );
            table->AddCkeyCol( constraint );

        	FdoSmPhBatchColumnsP ukeys = table->GetUkeyColumns(); 
            FdoSmPhColumnsP	ukeyColumns = new FdoSmPhColumnCollection();
            ukeys->Add( ukeyColumns );
    		table->AddUkeyCol( ukeys->GetCount() - 1, L"DOUBLE_COLUMN" );
        }
    }

    FdoStringP wildTablename = FdoStringP::Format( L"%lsTABLEWILD", (FdoString*) prefix, i );
    FdoSmPhTableP wildTable = owner->CreateTable( wildTablename );
    SetLtLck(wildTable, lt_mode);
    FdoSmPhColumnP wildColumn = wildTable->CreateColumnInt32( L"ID", false );
    wildTable->AddPkeyCol( wildColumn->GetName() );
    wildColumn = wildTable->CreateColumnChar( L"STRING_COLUMN", false, 50 );

    FdoStringP viewName = FdoStringP::Format( L"%lsVIEW1", (FdoString*) prefix );
    FdoStringP table1Name = FdoStringP::Format( L"%lsTABLE1", (FdoString*) prefix );
    FdoSmPhViewP view1 = owner->CreateView( viewName, L"", owner->GetName(), table1Name );
    FdoSmPhColumnP viewColumn = view1->CreateColumnInt32( L"ID", false, false, L"ID" );
    viewColumn = view1->CreateColumnDouble( L"DOUBLE_COLUMN", true, L"DOUBLE_COLUMN" );
}

void SchemaMgrTests::CreateFkey( FdoSmPhOwnerP owner, FdoStringP fTableName, FdoStringP pTableName, FdoStringP pOwnerName )
{
    FdoSmPhTableP fTable = owner->FindDbObject(fTableName).p->SmartCast<FdoSmPhTable>();
    FdoSmPhColumnP fColumn = fTable->GetColumns()->FindItem( L"FOREIGN_COLUMN" );

    FdoStringP fkeyName = FdoStringP::Format( L"FK_%ls_%ls", (FdoString*) fTableName, (FdoString*) pTableName );

    FdoSmPhFkeyP fkey = fTable->CreateFkey( fkeyName, pTableName, pOwnerName );
    fkey->AddFkeyColumn( fColumn, L"ID" );
}

void  SchemaMgrTests::VldDefaultConfig( FdoClassesP classes )
{
}

FdoSmPhTableP SchemaMgrTests::CreateIxTable( FdoSmPhOwnerP owner, FdoStringP tableName, int lt_mode )   
{
    FdoSmPhMgrP phMgr = owner->GetManager();

    FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(tableName) );
    SetLtLck(table, lt_mode);
    FdoSmPhColumnP column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"ID"), false );
    column = table->CreateColumnInt16( phMgr->GetDcColumnName(L"INT16_COL1"), false );
    column = table->CreateColumnInt16( phMgr->GetDcColumnName(L"INT16_COL2"), false);
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL1"), false, 10 );
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL2"), false, 7 );
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL3"), false, 2500 );
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL4"), false, 2501 );
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL5"), false, 49 );
    column = table->CreateColumnChar( phMgr->GetDcColumnName(L"STRING_COL6"), false, 51 );
    column = table->CreateColumnDate( phMgr->GetDcColumnName(L"DATE_COL1"), false);

    return table;
}

FdoSmPhViewP SchemaMgrTests::CreateIxView( FdoSmPhOwnerP owner, FdoStringP viewName, FdoStringP tableName )   
{
    FdoSmPhMgrP phMgr = owner->GetManager();

    FdoSmPhViewP view = owner->CreateView( phMgr->GetDcDbObjectName(viewName), L"", owner->GetName(), tableName );
    FdoStringP columnName = phMgr->GetDcColumnName(L"ID");
    FdoSmPhColumnP column = view->CreateColumnInt32( columnName, false, false, columnName );
    columnName = phMgr->GetDcColumnName(L"INT16_COL1");
    column = view->CreateColumnInt16( columnName, false, false, columnName );
    columnName = phMgr->GetDcColumnName(L"INT16_COL2");
    column = view->CreateColumnInt16( columnName, false, false, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL1");
    column = view->CreateColumnChar( columnName, false, 10, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL2");
    column = view->CreateColumnChar( columnName, false, 7, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL3");
    column = view->CreateColumnChar( columnName, false, 2500, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL4");
    column = view->CreateColumnChar( columnName, false, 2501, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL5");
    column = view->CreateColumnChar( columnName, false, 49, columnName );
    columnName = phMgr->GetDcColumnName(L"STRING_COL6");
    column = view->CreateColumnChar( columnName, false, 51, columnName );
    columnName = phMgr->GetDcColumnName(L"DATE_COL1");
    column = view->CreateColumnDate( columnName, false, columnName );

    return view;
}

void SchemaMgrTests::AddPkey( FdoSmPhTableP table )
{
    if ( FdoPtr<FdoSmPhColumn>(table->GetColumns()->FindItem(L"ID")) ) 
        table->AddPkeyCol( L"ID" );
    else
        table->AddPkeyCol( L"id" );
}

void SchemaMgrTests::AddIndex( FdoSmPhTableP table, bool unique, FdoStringP indexName, FdoStringP columns )
{
    FdoStringsP parsedColumns = FdoStringCollection::Create( columns, L" " );
    FdoInt32 idx;

    FdoSmPhIndexP index = table->CreateIndex( table->GetManager()->GetDcColumnName(indexName), unique );

    for ( idx = 0; idx < parsedColumns->GetCount(); idx++ ) {
        index->AddColumn( FdoSmPhColumnP(table->GetColumns()->GetItem( index->GetManager()->GetDcColumnName(parsedColumns->GetString(idx))) ) );
    }
}

void SchemaMgrTests::constraints ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        FdoSmPhOwnerP owner = database->FindOwner( L"fdo_br" );

        FdoSmPhRdConstraintReaderP rdr = owner->CreateConstraintReader( L"PK_ACDBENTITY" );

        while ( rdr->ReadNext() ) {
            printf( "got constraint %ls %ls %ls\n", 
                (FdoString*) rdr->GetString(L"",L"constraint_name"),
                (FdoString*) rdr->GetString(L"",L"table_name"),
                (FdoString*) rdr->GetString(L"",L"column_name")
            );
        }

        rdr = NULL;

        conn->disconnect();
        delete conn;
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

void SchemaMgrTests::delFdoBr ()
{
    StaticConnection* conn = CreateStaticConnection();
    FdoPtr<FdoIConnection> fdoConn;

    try
    {
        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhGrdMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        FdoSmPhOwnerP owner = database->FindOwner( L"fdo_br" );

        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        conn->disconnect();
        delete conn;
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

void SchemaMgrTests::SetLtLck( FdoSmPhTableP table, int lt_mode ) 
{
    if ( lt_mode > 0 ) {
        table->SetLtMode( (FdoLtLockModeType) lt_mode );
        table->SetLockingMode( (FdoLtLockModeType) lt_mode );
    }
}

FdoIoStreamP SchemaMgrTests::CvtConf( FdoIoStreamP streamIn ) 
{
    return streamIn;
}

void SchemaMgrTests::CreateMultiGeomTable( FdoSmPhOwnerP owner, FdoStringP tableName, FdoInt32 colCount, FdoInt32 indexMask, FdoInt32 nnullMask )
{
    FdoSmPhMgrP phMgr = owner->GetManager();
    FdoInt32 idx;
    FdoInt32 currBit = 1;

	owner->FindDbObject( phMgr->GetDcDbObjectName(tableName) );

    FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(tableName) );
    FdoSmPhColumnP column = table->CreateColumnInt32( phMgr->GetDcColumnName(L"ID"), false );
    table->AddPkeyCol( column->GetName() );

    for ( idx = 0; idx < colCount; idx++ ) {
        FdoStringP colName = phMgr->GetDcColumnName(
            FdoStringP::Format( L"GEOM%d", idx + 1 )
        );

        table->CreateColumnGeom( colName, (FdoSmPhScInfo*) NULL, !(nnullMask & currBit) ); 

        currBit = currBit * 2;
    }

    table->Commit();

    FdoSmPhColumnsP columns = table->GetColumns();
    currBit = 1;

    for ( idx = 0; idx < columns->GetCount(); idx++ ) {

        FdoSmPhColumnGeomP columnGeom = columns->GetItem(idx)->SmartCast<FdoSmPhColumnGeom>(true); 

        if ( columnGeom ) {
            FdoSmPhSpatialIndexP spatialIndex = columnGeom->GetSpatialIndex();

            if ( !(indexMask & currBit) && spatialIndex ) {
                columnGeom->DiscardSpatialIndex( spatialIndex );
                spatialIndex->SetElementState( FdoSchemaElementState_Deleted );
            }

            currBit = currBit * 2;
        }
    }

}

FdoStringP SchemaMgrTests::table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName )
{
    return mgr->GetDcDbObjectName( tableName );
}


void SchemaMgrTests::AddProviderColumns( FdoSmPhTableP table )
{
}

FdoStringP SchemaMgrTests::AssocNameTestFkeys( FdoSmPhGrdMgrP mgr, FdoInt32 assocNum )
{
    FdoStringP assocName;
    switch ( assocNum ) {
    case 1:
        assocName = FdoStringP::Format( L"Assoc%ls", (FdoString*)mgr->GetDcColumnName("table1_id") );
        break;
    case 2:
        assocName = FdoStringP::Format( L"Assoc%ls", (FdoString*)mgr->GetDcColumnName("table2_id1") );
        break;
    case 3:
        assocName = FdoStringP::Format( L"Assoc%ls", (FdoString*)mgr->GetDcColumnName("table3_id2") );
        break;
    }

    return assocName;
}

bool SchemaMgrTests::SupportsBaseObjects()
{
    return false;
}

bool SchemaMgrTests::SupportsViewPkey()
{
    return false;
}

SchemaMgrTests::ExpectedClassGeometricProperty::ExpectedClassGeometricProperty ()
{
    canBeGeomeytricTypePoint   = false;
    canBeGeomeytricTypeCurve   = false;
    canBeGeomeytricTypeSurface = false;
    canBeGeomeytricTypeSolid   = false;
    foundGeometricTypePoint    = false;
    foundGeometricTypeCurve    = false;
    foundGeometricTypeSurface  = false;
    foundGeometricTypeSolid    = false;
}

SchemaMgrTests::ExpectedClassGeometricProperty::~ExpectedClassGeometricProperty ()
{
}
