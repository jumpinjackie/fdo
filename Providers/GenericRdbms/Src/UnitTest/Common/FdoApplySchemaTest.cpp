 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "Pch.h"
#include "FdoApplySchemaTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#include "ConnectionUtil.h"
#include <Sm/Ph/Rd/DbObjectReader.h>
#include "../SchemaMgr/Ph/Owner.h"

FdoString*    FdoApplySchemaTest::DB_NAME_SUFFIX                    = L"_apply_schema";
FdoString*    FdoApplySchemaTest::DB_NAME_COPY_SUFFIX               = L"_apply_copy";
FdoString*    FdoApplySchemaTest::DB_NAME_OVERRIDE_SUFFIX           = L"_apply_override";
FdoString*    FdoApplySchemaTest::DB_NAME_OVERRIDE_DEFAULT_SUFFIX   = L"_apply_overridedef";
FdoString*    FdoApplySchemaTest::DB_NAME_FOREIGN_SUFFIX            = L"_apply_foreign";
FdoString*    FdoApplySchemaTest::DB_NAME_CONFIG_SUFFIX             = L"_apply_config";
FdoString*    FdoApplySchemaTest::DB_NAME_NO_META_SUFFIX            = L"_apply_no_meta";

FdoString*    FdoApplySchemaTest::LT_NAME                           = L"ApplyTest";
FdoString*    FdoApplySchemaTest::DB_NAME_LT_SUFFIX                 = L"_apply_lt";


FdoPropertyValue* FdoApplySchemaTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = propertyValues->FindItem( name );

    if ( !propertyValue ) 
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}


FdoApplySchemaTest::FdoApplySchemaTest(void) :
    mIsLowerDatastoreName(false)
{
}

FdoApplySchemaTest::~FdoApplySchemaTest(void)
{
}

void FdoApplySchemaTest::setUp ()
{
    set_provider();
}

void FdoApplySchemaTest::TestSchema ()
{
	FdoPtr<FdoIConnection> connection;
//	FdoPtr<FdoIConnection> copyConnection;
    StaticConnection* staticConn = NULL;

	try {
        FdoSchemaManagerP mgr;
        const FdoSmLpSchemaCollection* lp = NULL;

        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_SUFFIX,
            0,
            NULL,
            0
		);

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_SUFFIX );

		// The following tests must be run in the following order to get the expected results.

        printf( "Trying to modify MetaClass schema ... \n" );
		ModMetaClassSchema( connection );

		printf( "Creating System Schema ... \n" );
		CreateSystemSchema( connection);

		printf( "Deleting Acad Schema ... \n" );
		DeleteAcadSchema( connection);

		printf( "Creating Acad Schema ... \n" );
		CreateAcadSchema( connection );

		printf( "Creating Electric Schema ... \n" );

        CreateElectricSchema( connection );
		printf( "Create complete \n" );

    	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    	FdoPtr<FdoFeatureSchemaCollection> pCopySchemas = pDescCmd->Execute();

		printf( "Creating Error Schema ... \n" );
		CreateErrorSchema( connection );

        bool succeeded = false;

		printf( "Bad Electric schema create (already exists) ... \n" );
		try {
			CreateElectricSchema( connection );
			succeeded = true;
		}
		catch ( FdoSchemaException* e )
		{
			UnitTestUtil::PrintException(e, SchemaTestErrFile(1,false), true );
			FDO_SAFE_RELEASE(e);
		}

		if ( succeeded ) 
			CPPUNIT_FAIL( "2nd Electric schema create was supposed to fail" );

        printf( "Modifying Electric Schema ... \n" );
   		ModElectricSchema( connection );

		printf( "Redefining a geometry ... \n" );
		RedefineGeometry( connection );

		printf( "Creating Land Schema ... \n" );
		CreateLandSchema( connection );
        FdoPtr<FdoStringCollection> classNames = FdoStringCollection::Create();
        classNames->Add( L"Person" );
        classNames->Add( L"Parcel" );
        classNames->Add( L"Zoning" );
        classNames->Add( L"County" );
		GetClassCapabilities( connection, L"Land", classNames );

        printf( "Deleting Properties with data ... \n" );
		DelPropertyError( connection );

        printf( "Writing 1st LogicalPhysical Schema ... \n" );

        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test1.xml" ) );

        printf( "Applying mixed updates and deletes ... \n" );
		ModDelSchemas( connection );

		succeeded = false;

		printf( "Deleting Acad Schema ... \n" );
		try {
 			DeleteAcadSchema( connection );
			succeeded = true;
		}
		catch ( FdoSchemaException* e )
		{
			UnitTestUtil::PrintException(e, SchemaTestErrFile(2,false), true);
			FDO_SAFE_RELEASE(e);
		}

		if ( succeeded ) 
			CPPUNIT_FAIL( "2nd Acad schema delete was supposed to fail" );

       	FdoPtr<FdoFeatureSchemaCollection> pCopySchemas2 = pDescCmd->Execute();

        printf( "Deleting Land Schema ... \n" );
		DeleteLandSchema( connection );

        printf( "Writing 2nd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test2.xml" ) );

        printf( "Re-Adding some elements ... \n" );
 		ReAddElements(connection);

        printf( "Re-Adding Land Schema ... \n" );
		CreateLandSchema(connection);

		printf( "Modifying Land Schema ... \n" );
		ModLandSchema(connection);

		printf( "Trying some invalid modifications ... \n" );

		ModErrors( connection );
		ModErrors2( connection );

		printf( "Testing long element names ... \n" );

        //TODO: element names are too long for other providers so just
        //test against Oracle for now
#ifdef RDBI_DEF_ORA
        CreateLongStringSchema( connection );
#endif

        bool delayNLSSchema = DelayNLSSchema();

        if ( !delayNLSSchema ) {
            printf( "Testing non-ASCII7 element names ... \n" );
            CreateNLSSchema( connection, staticConn );
        }
        printf( "Writing 3rd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test3.xml" ) );

        if ( delayNLSSchema ) {
            printf( "Testing non-ASCII7 element names ... \n" );
            CreateNLSSchema( connection, staticConn );
        }
/*
		printf( "Testing Join Tree retrieval ... \n" );
		GetJoinTree( sm );
*/

		printf( "Testing Class Capabilities ... \n" );
        classNames->Clear();
        classNames->Add( L"AcDbEntity" );
        classNames->Add( L"AcXData" );
		GetClassCapabilities( connection, L"Acad", classNames );

		printf( "Testing Base Properties ... \n" );
		CheckBaseProperties( connection );

        printf( "Writing schema overrides ... \n" );

        WriteXmlOverrides( connection, false, L"", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov1.xml" ) );
        WriteXmlOverrides( connection, false, L"Electric'l", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov2.xml" ) );
        WriteXmlOverrides( connection, true, L"Electric'l", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov3.xml" ) );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_SUFFIX
		);

        connection = NULL;

        CopySchemas( pCopySchemas, pCopySchemas2 );

		// Compare output files with expected results.

		// First do xml dumps of LogicalPhysical schema

        FdoStringP out1master = LogicalPhysicalBend(L"apply_schema_test1_master.txt");
        FdoStringP out1       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test1.xml" ) );
        FdoStringP out2master = LogicalPhysicalBend(L"apply_schema_test2_master.txt");
        FdoStringP out2       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test2.xml" ) );
        FdoStringP out3master = LogicalPhysicalBend(L"apply_schema_test3_master.txt");
        FdoStringP out3       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test3.xml" ) );
        FdoStringP out4master = LogicalPhysicalBend(L"apply_schema_test4_master.txt");
        FdoStringP out4       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test4.xml" ) );
        FdoStringP out5master = LogicalPhysicalBend(L"apply_schema_test5_master.txt");
        FdoStringP out5       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test5.xml" ) );

        UnitTestUtil::CheckOutput( (const char*) out1master,(const char*) out1 );
        UnitTestUtil::CheckOutput( (const char*) out2master,(const char*) out2 );
        UnitTestUtil::CheckOutput( (const char*) out3master,(const char*) out3 );
        UnitTestUtil::CheckOutput( (const char*) out4master,(const char*) out4 );
#ifdef RDBI_DEF_ORA
        // TODO: This comparison gets differences on MySQL and SqlServer
        // The differences are due to different  table name max lengths in 
        // Oracle, MySQL, SqlServer. Some enhancements to the LogicalPhysicalBender
        // might resolve this one.
        UnitTestUtil::CheckOutput( (const char*) out5master,(const char*) out5 );
#endif

#ifdef RDBI_DEF_ORA
        // Next do the Schema Mapping XML exports
        // TODO: for SqlServer, lots of non-default stuff gets 
        // written so some fixes required before can compare to master.
	    UnitTestUtil::CheckOutput( "apply_schema_test_ov1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov1.xml" ) );
	    UnitTestUtil::CheckOutput( "apply_schema_test_ov2_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov2.xml" ) );
	    UnitTestUtil::CheckOutput( "apply_schema_test_ov3_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_test_ov3.xml" ) );
#endif
        // Next do the error messages. Some checks are not done for some providers
        // since their expect output is different from the master
        // TODO: create special masters for these cases.

#ifdef _WIN32
        if ( SchemaTestErrFile(1,true).GetLength() > 0 )
    		UnitTestUtil::CheckOutput( SchemaTestErrFile(1,true), SchemaTestErrFile(1,false) );
        if ( SchemaTestErrFile(2,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(2,true), SchemaTestErrFile(2,false) );
        if ( SchemaTestErrFile(3,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(3,true), SchemaTestErrFile(3,false) );
        if ( SchemaTestErrFile(4,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(4,true), SchemaTestErrFile(4,false) );
        if ( SchemaTestErrFile(5,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(5,true), SchemaTestErrFile(5,false) );
        if ( SchemaTestErrFile(6,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(6,true), SchemaTestErrFile(6,false) );
        if ( SchemaTestErrFile(7,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(7,true), SchemaTestErrFile(7,false) );
        if ( SchemaTestErrFile(8,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(8,true), SchemaTestErrFile(8,false) );
        if ( SchemaTestErrFile(9,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(9,true), SchemaTestErrFile(9,false) );
        if ( SchemaTestErrFile(10,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(10,true), SchemaTestErrFile(10,false) );
        if ( SchemaTestErrFile(11,true).GetLength() > 0 )
		    UnitTestUtil::CheckOutput( SchemaTestErrFile(11,true), SchemaTestErrFile(11,false) );
#endif
    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
        try {
		    if (connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
        }
        catch ( ... ) 
		{
		}
		throw;
	}
   	catch (...)
   	{
        try {
		    if (connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
        }
        catch ( ... ) 
		{
		}
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
    if ( staticConn ) delete staticConn;

	printf( "Done\n" );
}

void FdoApplySchemaTest::TestOverrides ()
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
	
    mDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX);

    try {
        FdoSchemaManagerP mgr;
        const FdoSmLpSchemaCollection* lp = NULL;
        FdoFeatureSchemaP  pSchema;

        if ( SupportsCrossDatastoreDependencies() ) {
            // Re-create foreign test datastore first to destroy
            // any dependent views on the overrides database.
		    printf( "Initializing Foreign Connection ... \n" );
		    connection = UnitTestUtil::CreateConnection(
			    true,
			    true,
                DB_NAME_FOREIGN_SUFFIX,
                0,
                NULL,
                0,
                false
		    );

            UnitTestUtil::CloseConnection(
			    connection,
			    false,
                DB_NAME_FOREIGN_SUFFIX
		    );
        }

        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_OVERRIDE_SUFFIX,
            0,
            NULL,
            0,
            false
		);

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_OVERRIDE_SUFFIX );

        mgr = staticConn->CreateSchemaManager();
        FdoSmPhMgrP ph = mgr->GetPhysicalSchema();
        FdoSmPhOwnerP owner = ph->GetOwner();

        FdoSmPhTableP table = owner->CreateTable( ph->GetDcDbObjectName(L"viewbase") );
        FdoSmPhColumnP column = table->CreateColumnInt64( ph->GetDcColumnName(L"id"), false );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnChar( ph->GetDcColumnName(L"col1"), true, 50 );
        column = table->CreateColumnInt16( ph->GetDcColumnName(L"col2"), true );
        column = table->CreateColumnChar( ph->GetDcColumnName(L"view_op_col1"), true, 50 );
        column = table->CreateColumnInt16( ph->GetDcColumnName(L"view_op_col2"), true );
        column = table->CreateColumnGeom( ph->GetDcColumnName(L"geometry"), (FdoSmPhScInfo*) NULL );

        FdoSmPhViewP view = owner->CreateView( ph->GetDcDbObjectName(L"view1"), L"", owner->GetName(), ph->GetDcDbObjectName(L"viewbase" ));
        column = view->CreateColumnInt64(ph->GetDcColumnName(L"id"), false, true, ph->GetDcColumnName(L"id") );
        column = view->CreateColumnChar(ph->GetDcColumnName(L"col1"), true, 50, ph->GetDcColumnName(L"col1") );
        column = view->CreateColumnInt16( ph->GetDcColumnName(L"col2"), true, false, ph->GetDcColumnName(L"col2") );
        column = view->CreateColumnChar(ph->GetDcColumnName(L"view_op_col1"), true, 50, ph->GetDcColumnName(L"view_op_col1") );
        column = view->CreateColumnInt16( ph->GetDcColumnName(L"view_op_col2"), true, false, ph->GetDcColumnName(L"view_op_col2") );
        column = view->CreateColumnGeom( ph->GetDcColumnName(L"geometry"), (FdoSmPhScInfo*) NULL, true, true, false, ph->GetDcColumnName(L"geometry") );

        view = owner->CreateView( ph->GetDcDbObjectName(L"view2"), L"", owner->GetName(), ph->GetDcDbObjectName(L"viewbase" ));
        column = view->CreateColumnInt64(ph->GetDcColumnName(L"id"), false, true, ph->GetDcColumnName(L"id") );
        column = view->CreateColumnChar(ph->GetDcColumnName(L"col1"), true, 50, ph->GetDcColumnName(L"col1") );
        column = view->CreateColumnInt16( ph->GetDcColumnName(L"col2"), true, false, ph->GetDcColumnName(L"col2") );

        table = owner->CreateTable( ph->GetDcDbObjectName(L"viewbaseop") );
        column = table->CreateColumnInt64( ph->GetDcColumnName(L"id"), false );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnInt32( ph->GetDcColumnName(L"seq"), false );
        column = table->CreateColumnChar( ph->GetDcColumnName(L"col1"), true, 50 );
        column = table->CreateColumnInt16( ph->GetDcColumnName(L"col2"), true );

        view = owner->CreateView( ph->GetDcDbObjectName(L"view_op"), L"", owner->GetName(), ph->GetDcDbObjectName(L"viewbaseop" ));
        column = view->CreateColumnInt64(ph->GetDcColumnName(L"id"), false, true, ph->GetDcColumnName(L"id") );
        column = view->CreateColumnInt32(ph->GetDcColumnName(L"seq"), false, true, ph->GetDcColumnName(L"seq") );
        column = view->CreateColumnChar(ph->GetDcColumnName(L"col1"), true, 50, ph->GetDcColumnName(L"col1") );
        column = view->CreateColumnInt16( ph->GetDcColumnName(L"col2"), true, false, ph->GetDcColumnName(L"col2") );

        owner->Commit();

        // Create RDBMS-specific elements (such as filegroups on SQL Server, tablespaces on Oracle, etc):
        FdoStringP wDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX);
        CreateRdbmsSpecificElements(connection, wDatastore);

		// The following tests must be run in the following order to get the expected results.
		printf( "Creating Override Schema ... \n" );
		CreateOverrideSchema( 
            connection, 
            FdoRdbmsOvSchemaMappingP(CreateOverrides(connection, 1))
        );

		printf( "Writing 1st LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_overrides1.xml" ) );

        printf( "Doing 1st modification to Schema ... \n" );
		ModOverrideSchema1( 
            connection, 
            FdoRdbmsOvSchemaMappingP( CreateOverrides(connection, 2) )
        );

        printf( "Writing 2nd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_overrides2.xml" ) );

		printf( "Doing 2nd modification to Schema ... \n" );
		ModOverrideSchema2( 
            connection, 
            FdoRdbmsOvSchemaMappingP( CreateOverrides(connection, 3) )
        );

        printf( "Writing 3rd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_overrides3.xml" ) );

        FdoSmLpQClassesP classes;
        classes = lp->TableToClasses( ph->GetRealDbObjectName(L"ovclassa") );
        CPPUNIT_ASSERT( classes->GetCount() == 1 );
        CPPUNIT_ASSERT( wcscmp(classes->RefClassDefinition(0)->GetName(), L"OvClassA") == 0 );

        classes = lp->TableToClasses( ph->GetRealDbObjectName(L"ov_table_c") );
		CPPUNIT_ASSERT( classes->GetCount() == 5 );
        CPPUNIT_ASSERT( wcscmp(classes->RefClassDefinition(0)->GetName(), L"OvClassC") == 0 );
        CPPUNIT_ASSERT( wcscmp(classes->RefClassDefinition(2)->GetName(), L"OvClassE") == 0 );
        
        CPPUNIT_ASSERT( classes->RefClassDefinition( L"OverridesA:OvClassD" ) != NULL );
        CPPUNIT_ASSERT( classes->RefClassDefinition( L"OverridesA:OvClassJ" ) == NULL );

        WriteXmlOverrides( connection, false, L"", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out1.xml" ) );
        WriteXmlOverrides( connection, true, L"", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out2.xml" ) );

       	FdoPtr<FdoIDescribeSchemaMapping>  pDescMappingCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
        pDescMappingCmd->SetSchemaName( L"NoExist" );
        FdoSchemaMappingsP mappings = pDescMappingCmd->Execute();
        CPPUNIT_ASSERT( mappings->GetCount() == 0 );

		if (CompareGeometrySI())
		{
			const FdoSmLpGeometricPropertyDefinition* geomProp = FdoSmLpGeometricPropertyDefinition::Cast(
				lp->RefItem(L"OverridesA")->RefClasses()->RefItem(L"OvClassA")->RefProperties()->RefItem(L"GeomA")
			);

			CPPUNIT_ASSERT( wcscmp(geomProp->GetColumnNameSi1(), L"GEOMA_SI_1") == 0 );
			CPPUNIT_ASSERT( wcscmp(geomProp->GetColumnNameSi2(), L"GEOMA_SI_2") == 0 );
		}

        if ( SupportsCrossDatastoreDependencies() ) {
#ifdef RDBI_DEF_ORA
		    // For the test it is necessary to execute the following grant:
            //  grant select on "<db_prefix>_APPLY_OVERRIDE"."FNESTED_DA" to <db_prefix>_apply_foreign
            // For this to work, it is necessary to connect to the data store "<db_prefix>_APPLY_OVERRIDE"
            // directly and execute the statement. Afterwards the connection must be revoked to the 
            // current FDO user again.
            FdoPtr<FdoIConnection> directConnection = GetDirectConnection(connection);
            directConnection->Open();
		    UnitTestUtil::GrantDatastore( directConnection, mgr, L"select", UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX) );
            directConnection->Close();
#endif

            owner = ph->GetOwner();
            table = owner->CreateTable( ph->GetDcDbObjectName(L"Storage") );
            column = table->CreateColumnInt64( ph->GetDcColumnName(L"ID"), false );
		    table->AddPkeyCol(column->GetName());
            column = table->CreateColumnChar( ph->GetDcColumnName(L"Storage"), true, 50 );
            column = table->CreateColumnGeom( ph->GetDcColumnName(L"Floor"), (FdoSmPhScInfo*) NULL );
            column = table->CreateColumnInt16( ph->GetDcColumnName(L"Extra"), !CanAddNotNullCol() );

            table = owner->CreateTable( ph->GetDcDbObjectName(L"oneforeign") );
            column = table->CreateColumnInt64( ph->GetDcColumnName(L"ID"), false );
            column = table->CreateColumnChar( ph->GetDcColumnName(L"Data"), true, 50 );

            owner->Commit();

#ifdef RDBI_DEF_SSQL
            UnitTestUtil::Sql2Db( 
                L"insert into storage values ( 1, 'a string', NULL, 2)",
                connection
            );
#else
            UnitTestUtil::Sql2Db( 
                L"insert into storage values ( 1, 'a string', NULL, 2)",
                connection
            );
#endif

#ifdef RDBI_DEF_ORA
            directConnection = GetDirectConnection(connection);
            directConnection->Open();

            UnitTestUtil::Sql2Db( 
                FdoStringP::Format( 
                            L"grant select on %ls.storage to %ls",
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX),
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
                        ),
                        directConnection
                    );

            UnitTestUtil::Sql2Db( 
                FdoStringP::Format( 
                            L"grant select on %ls.oneforeign to %ls",
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX),
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
                        ),
                        directConnection
                    );
            directConnection->Close();
#endif

            table = owner->CreateTable( ph->GetDcDbObjectName(L"Storage_Floor") );
            column = table->CreateColumnInt64( ph->GetDcColumnName(L"ID"), false );
            column = table->CreateColumnInt64( ph->GetDcColumnName(L"Storage_ID"), false );
            column = table->CreateColumnChar( ph->GetDcColumnName(L"Storage"), true, 50 );
            table->Commit();

#ifdef RDBI_DEF_ORA
            directConnection = GetDirectConnection(connection);
            directConnection->Open();

            UnitTestUtil::Sql2Db( 
                FdoStringP::Format( 
                            L"grant select on %ls.storage_floor to %ls",
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX),
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
                ),
                directConnection
            );
            directConnection->Close();
#endif

            table = owner->CreateTable( ph->GetDcDbObjectName(L"NOFEATID") );
            column = table->CreateColumnChar( ph->GetDcColumnName(L"ID"), false, 20 );
		    table->AddPkeyCol(column->GetName());
            column = table->CreateColumnChar( ph->GetDcColumnName(L"DATA"), true, 50 );
            column = table->CreateColumnGeom( ph->GetDcColumnName(L"GEOMETRY"), (FdoSmPhScInfo*) NULL );
		    if (CreateGeometrySICol())
		    {
			    column = table->CreateColumnChar( L"GEOMETRY_SI_1", true, 255 );
			    column = table->CreateColumnChar( L"GEOMETRY_SI_2", true, 255 );
		    }

            table->Commit();

#ifdef RDBI_DEF_ORA
            directConnection = GetDirectConnection(connection);
            directConnection->Open();

            UnitTestUtil::Sql2Db( 
                FdoStringP::Format( 
                            L"grant select, insert, update, delete on %ls.nofeatid to %ls",
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX),
                            (FdoString*) UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX)
                ),
                directConnection
            );
            directConnection->Close();
#endif

            // Grab schemas and overrides to apply to foreign datastore

            FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	        pDescCmd->SetSchemaName( L"OverridesA" );
	        FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	        pSchema = pSchemas->GetItem( L"OverridesA" );

      	    pDescMappingCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
            pDescMappingCmd->SetIncludeDefaults(true);
            mappings = pDescMappingCmd->Execute();
        }
        mgr = NULL;
        lp = NULL;
        ph = NULL;
        staticConn->disconnect();
        delete staticConn;
        staticConn = NULL;

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_OVERRIDE_SUFFIX
		);

        // Compare output files with expected results.

#ifndef RDBI_DEF_SSQL
        FdoStringP ovMaster1 = LogicalPhysicalBend(L"apply_schema_overrides1_master.txt");
        FdoStringP ov1       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides1.xml" ) );
        FdoStringP ovMaster2 = LogicalPhysicalBend(L"apply_schema_overrides2_master.txt");
        FdoStringP ov2       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides2.xml" ) );
        FdoStringP ovMaster3 = LogicalPhysicalBend(L"apply_schema_overrides3_master.txt");
        FdoStringP ov3       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides3.xml" ) );

	    // First do xml dumps of LogicalPhysical schema
        // TODO: get this to work on SQL Server
        UnitTestUtil::CheckOutput( (const char*)ovMaster1, (const char*)ov1 );
        UnitTestUtil::CheckOutput( (const char*)ovMaster2, (const char*)ov2 );
        UnitTestUtil::CheckOutput( (const char*)ovMaster3, (const char*)ov3 );
#endif

        // Next, compare described schema mappings.
        // TODO: get this to work on MySQL and SQL Server
#ifdef RDBI_DEF_ORA
        UnitTestUtil::CheckOutput( "apply_schema_overrides_out1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out1.xml" ) );
        UnitTestUtil::CheckOutput( "apply_schema_overrides_out2_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out2.xml" ) );
#endif

        if ( SupportsCrossDatastoreDependencies() ) {
            printf( "Opening Foreign Connection ... \n" );
		    connection = UnitTestUtil::CreateConnection(
			    false,
			    false,
                DB_NAME_FOREIGN_SUFFIX,
                0,
                NULL,
                0,
                false
		    );

            staticConn = UnitTestUtil::NewStaticConnection();
            staticConn->connect();
            staticConn->SetSchema( DB_NAME_FOREIGN_SUFFIX );

            // Modify the schema mappings to create foreign schema
		    printf( "Creating Foreign Schema ... \n" );

            ModOverrideSchemaForeign( pSchema );
            // TODO: Can't yet handle properties whose columns are missing in the foreign table.
            FdoClassesP fdoClasses = pSchema->GetClasses();
            fdoClasses->RemoveAt( fdoClasses->IndexOf(L"view1") );
            fdoClasses->RemoveAt( fdoClasses->IndexOf(L"view2") );
            fdoClasses->RemoveAt( fdoClasses->IndexOf(L"view_op") );

            FdoRdbmsOvSchemaMappingP mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem( connection, pSchema->GetName());
            SchemaOvSetOwner(mapping, UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX) );

            // Apply the foreign schema
            FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
            pApplyCmd->SetFeatureSchema( pSchema );
            pApplyCmd->SetPhysicalMapping( mapping );
            pApplyCmd->SetIgnoreStates(true);
            pApplyCmd->Execute();

            CreateForeignBasedSchema( 
                connection, 
                pSchema, 
                FdoRdbmsOvSchemaMappingP(CreateForeignBasedOverrides(connection))
            );

            printf( "Writing 1st LogicalPhysical Schema ... \n" );
            mgr = staticConn->CreateSchemaManager();
            lp = mgr->RefLogicalPhysicalSchemas();
            lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_foreign1.xml" ) );
            ph = mgr->GetPhysicalSchema();

            WriteXmlOverrides( 
                connection, 
                false, 
                L"", 
                UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out1.xml" ),
                ph->GetDcOwnerName(DB_NAME_OVERRIDE_SUFFIX)
            );

            WriteXmlOverrides( 
                connection, 
                true, 
                L"", 
                UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out2.xml" ),
                ph->GetDcOwnerName(DB_NAME_OVERRIDE_SUFFIX)
            );


            printf( "Modifying foreign schema ... \n" );
            ModOverrideSchemaForeign2( connection, mapping );

		    printf( "Writing 2nd LogicalPhysical Schema ... \n" );
            mgr = staticConn->CreateSchemaManager();
            lp = mgr->RefLogicalPhysicalSchemas();
            lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_foreign2.xml" ) );

            printf( "Checking access to feature class non-numeric id ... \n" );
            CheckNonNumericForeignClass( connection );

            printf( "Destroying foreign schemas ... \n" );
      	    FdoPtr<FdoIDestroySchema>  pDestCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
    	    pDestCmd->SetSchemaName( L"ForeignBased" );
    	    pDestCmd->Execute();
    	    pDestCmd->SetSchemaName( L"OverridesA" );
    	    pDestCmd->Execute();

		    printf( "Writing 3rd LogicalPhysical Schema ... \n" );
            mgr = staticConn->CreateSchemaManager();
            lp = mgr->RefLogicalPhysicalSchemas();
            lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_foreign3.xml" ) );

            FdoStringP ovfMaster1 = LogicalPhysicalBend(L"apply_schema_foreign1_master.txt");
            FdoStringP ovf1       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign1.xml" ) );
            FdoStringP ovfMaster2 = LogicalPhysicalBend(L"apply_schema_foreign2_master.txt");
            FdoStringP ovf2       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign2.xml" ) );
            FdoStringP ovfMaster3 = LogicalPhysicalBend(L"apply_schema_foreign3_master.txt");
            FdoStringP ovf3       = LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign3.xml" ) );

#ifndef RDBI_DEF_SSQL
            UnitTestUtil::CheckOutput( (const char*)ovfMaster1, (const char*)ovf1 );
            UnitTestUtil::CheckOutput( (const char*)ovfMaster2, (const char*)ovf2 );
#endif
            UnitTestUtil::CheckOutput( (const char*)ovfMaster3, (const char*)ovf3 );

            //TODO: handle master comparison for other providers
#ifdef RDBI_DEF_ORA
            UnitTestUtil::CheckOutput( "apply_schema_foreign_out1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out1.xml" ) );
	        UnitTestUtil::CheckOutput( "apply_schema_foreign_out2_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out2.xml" ) );
#endif

            mgr = NULL;
            lp = NULL;
            ph = NULL;
            staticConn->disconnect();
            delete staticConn;
            staticConn = NULL;

            printf( "Closing Connection ... \n" );
		    UnitTestUtil::CloseConnection(
			    connection,
			    false,
                DB_NAME_FOREIGN_SUFFIX
		    );
        }
    }
	catch ( FdoException* e ) 
	{
		try {
            if ( staticConn ) delete staticConn;
			if (connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		if (connection) connection->Close(); 
        if ( staticConn ) delete staticConn;
		throw;
	}
   	catch (...)
   	{
		if (connection) connection->Close(); 
        if ( staticConn ) delete staticConn;
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}

    printf( "Done\n" );
}

void FdoApplySchemaTest::TestOverrideDefaults ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

    mDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_DEFAULT_SUFFIX);

	try {
        FdoSchemaManagerP mgr;
        const FdoSmLpSchemaCollection* lp = NULL;

        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_OVERRIDE_DEFAULT_SUFFIX,
            0,
            NULL,
            0,
            false
		);

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_OVERRIDE_DEFAULT_SUFFIX );

        // Create RDBMS-specific elements (such as filegroups on SQL Server, tablespaces on Oracle, etc):
        FdoStringP wDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_DEFAULT_SUFFIX);
        CreateRdbmsSpecificElements(connection, wDatastore);

		// The following tests must be run in the following order to get the expected results.

		printf( "Creating Override Schema ... \n" );
		CreateOverrideSchema( 
            connection, 
            FdoRdbmsOvSchemaMappingP( CreateOverrideDefaults(connection, 1) ),
            false,
            false
        );

		printf( "Writing 1st LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
		lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_def1.xml" ) );

        WriteXmlOverrides( connection, false, L"", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_defout1.xml" ) );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_OVERRIDE_DEFAULT_SUFFIX
		);

		// Compare output files with expected results.

		// First do xml dumps of LogicalPhysical schema

#ifndef RDBI_DEF_SSQL
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_overrides_def1_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_def1.xml" ) )
        );
#endif

        // Next, compare described schema mappings.
        // TODO: get this to work on MySQL and SQL Server
#ifdef RDBI_DEF_ORA
        // Next, compare described schema mappings.
        UnitTestUtil::CheckOutput( "apply_schema_overrides_defout1_master.txt",
			UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_defout1.xml" ) );
#endif

        lp = NULL;
        mgr = NULL;
        delete staticConn;
        staticConn = NULL;
	}
	catch ( FdoException* e ) 
	{
		try {
			if (connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		if (connection) connection->Close(); 
		throw;
	}
   	catch (...)
   	{
		if (connection) connection->Close(); 
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );

}

void FdoApplySchemaTest::TestOverrideErrors ()
{
	FdoPtr<FdoIConnection> connection;

	try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_SUFFIX,
            0,
            NULL,
            0
		);

        bool succeeded = false;

        try {
		    printf( "Creating Override Schema ... \n" );
		    CreateOverrideSchema( 
                connection, 
                FdoRdbmsOvSchemaMappingP( CreateErrorOverrides(connection) ),
                true
            );
            succeeded = true;
        }
		catch ( FdoSchemaException* e )
		{
			UnitTestUtil::PrintException(
                e, 
                UnitTestUtil::GetOutputFileName( SchemaOvErrFile(1,false) ), 
                true 
            );
			FDO_SAFE_RELEASE(e);
		}

        succeeded = false;

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_SUFFIX
		);

		// Compare output files with expected results.
	    FdoStringP provider = UnitTestUtil::GetEnviron("provider");
      
#ifdef _WIN32
   		UnitTestUtil::CheckOutput( 
            SchemaOvErrFile(1,true),
            UnitTestUtil::GetOutputFileName( SchemaOvErrFile(1,false) )
        );
#endif

	}
	catch ( FdoException* e ) 
	{
		try {
			if (connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		if (connection) connection->Close(); 
		throw;
	}
   	catch (...)
   	{
		if (connection) connection->Close(); 
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );

}

void FdoApplySchemaTest::TestLT ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

	try {
        FdoSchemaManagerP mgr;
        const FdoSmLpSchemaCollection* lp = NULL;
        FdoSmPhMgrP ph;

        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();

		// The following tests must be run in the following order to get the expected results.

        mgr = staticConn->CreateSchemaManager();
        ph = mgr->GetPhysicalSchema();

        int LtLckMethod = GetLtLockMethod( ph );

        ph = NULL;
        lp = NULL;
        mgr = NULL;
        staticConn->disconnect();

		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_LT_SUFFIX,
            0,
            NULL,
            LtLckMethod,
            true
		);

	    printf( "Creating LT Schema ... \n" );
	    CreateLTSchema( connection );

        staticConn->connect();
        staticConn->SetSchema( DB_NAME_LT_SUFFIX );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        ph = mgr->GetPhysicalSchema();

        const FdoSmLpClassDefinition* pFeatClass = lp->RefItem(L"LT")->RefClasses()->RefItem(L"DelStatus");
        CPPUNIT_ASSERT( pFeatClass );

        const FdoSmPhTable* table = 
            dynamic_cast<const FdoSmPhTable*>(pFeatClass->RefDbObject()->RefDbObject());

        CPPUNIT_ASSERT( table );

        const FdoSmPhColumn* column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"LTID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"nextltid") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"lockid") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"locktype") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        if ( LtLckMethod == 1  ) 
            CPPUNIT_ASSERT( table->RefPkeyColumns()->GetCount() == 2 );
        else
            CPPUNIT_ASSERT( table->RefPkeyColumns()->GetCount() == 1 );

        column = table->RefPkeyColumns()->RefItem( ph->GetDcColumnName(L"FeatId") );
        CPPUNIT_ASSERT( column != NULL );
        column = table->RefPkeyColumns()->RefItem( 0 );
        CPPUNIT_ASSERT( FdoStringP(column->GetName()).ICompare(ph->GetDcColumnName(L"FeatId")) == 0 );

        column = table->RefPkeyColumns()->RefItem( ph->GetDcColumnName(L"ltid") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        ph = mgr->GetPhysicalSchema();
        FdoSmPhDbObjectP pTable;
        
#ifdef RDBI_DEF_ORA
        pTable = ph->FindDbObject(L"CIRCLE_LT1_GRIP",L"",L"",false);
#else
        if ( ph->IsRdbObjNameAscii7() ) 
            pTable = ph->FindDbObject(L"CIRCLE_LT_GRIP",L"",L"",false);
        else
            pTable = ph->FindDbObject(ph->GetDcDbObjectName(L"Circle Lt_Grip"),L"",L"",false);
#endif
        CPPUNIT_ASSERT(pTable);
        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"ltid") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"nextltid") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"lockid") );
        CPPUNIT_ASSERT( column == NULL );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"locktype") );
        CPPUNIT_ASSERT( column == NULL );

	    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
        FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"LT" );

        FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"DelStatus" );
        VldClassCapabilities( LtLckMethod, LtLckMethod, pClass );

        if ( LtLckMethod > 0 ) {
            InsertObject( connection, true, L"LT", L"Circle Lt", L"NextVer", L"1", NULL );
            InsertObject( connection, true, L"LT", L"DelStatus", L"LtLock", L"1", NULL );

            bool succeeded = false;

            try {
                printf( "Live Modification ... \n" );
	            ModLTSchema( connection );
                succeeded = true;
            }
	        catch ( FdoSchemaException* e )
	        {
		        UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName(L"apply_schema_lt_err1.txt"), true );
		        FDO_SAFE_RELEASE(e);
	        }

	        if ( succeeded ) 
		        CPPUNIT_FAIL( "1st Modification was supposed to fail." );

            CPPUNIT_ASSERT( GetActiveLongTransaction(connection) == L"LIVE" );
            StartLongTransaction( connection, L"LT" );

            try {
	            printf( "Conditional Modification ... \n" );
	            ModLTSchema( connection ); 
                succeeded = true;
            }
	        catch ( FdoSchemaException* e )
	        {
		        UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName(L"apply_schema_lt_err2.txt"), true );
		        FDO_SAFE_RELEASE(e);
	        }

	        if ( succeeded ) 
		        CPPUNIT_FAIL( "2nd Modification was supposed to fail." );

            CPPUNIT_ASSERT( GetActiveLongTransaction(connection) == GenLtName(L"LT") );
            EndLongTransaction( connection );

            RollbackLongTransaction(connection, L"LT" );

            printf( "Last Modification, should succeed ... \n" );
	        ModLTSchema( connection ); 
        }

        printf( "Closing Connection ... \n" );
	    UnitTestUtil::CloseConnection(
	        connection,
		    false,
            DB_NAME_SUFFIX
		);

		// Compare output files with expected results.
        if ( LtLckMethod > 0 ) {
#ifdef _WIN32
    		UnitTestUtil::CheckOutput( "apply_schema_lt_err1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_lt_err1.txt" ) );
	    	UnitTestUtil::CheckOutput( "apply_schema_lt_err1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_lt_err2.txt" ) );
#endif
        }

        ph = NULL;
        lp = NULL;
        mgr = NULL;
        staticConn->disconnect();
        delete staticConn;
    }
	catch ( FdoException* e ) 
	{
		try {
			if (connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		if (connection) connection->Close(); 
		throw;
	}
   	catch (...)
   	{
		if (connection) connection->Close(); 
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );

}

// The following XSL stylesheet is used in TestConfigDoc, it modifies the 
// LogicalPhysical Schema XML written from a datastore with MetaSchema, so that
// it looks like one from a datastore without. For example, system schemas and
// properties are removed. It also sorts the schemas, classes and properties
// so it can be compared with other XML documents without false differences
// due to different element ordering

static char* pRmvLpMetaSchema = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<xsl:stylesheet version=\"1.0\" \
xmlns=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" \
xmlns:lp=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" \
xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:gml=\"http://www.opengis.net/gml\" \
xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:fdo=\"http://www.autodesk.com/isd/fdo\" \
>\
<xsl:template match=\"lp:schemas\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:schema[not(@name='F_MetaClass')]\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:class\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:attribute name=\"tblCreator\">\
            <xsl:value-of select=\"'False'\"/>\
        </xsl:attribute>\
        <xsl:attribute name=\"fixedTbl\">\
            <xsl:value-of select=\"'True'\"/>\
        </xsl:attribute>\
        <xsl:apply-templates select=\"node()\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:properties\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:property[not(@system='True') and not(@name = 'ClassId' or @name = 'RevisionNumber' or @name = 'ClassName' or @name = 'SchemaName' or @name='Bounds')]\"  >\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:uniqueConstraints\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"lp:uniqueConstraint\">\
		<xsl:sort select=\"count(lp:property)\"/>\
    </xsl:apply-templates>\
  </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:uniqueConstraint\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"lp:property\">\
		<xsl:sort select=\"@name\"/>\
    </xsl:apply-templates>\
  </xsl:copy>\
</xsl:template>";

static char* pRmvLpMetaSchema2 = 
"<xsl:template match=\"lp:property\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:if test=\"@featId\" >\
            <xsl:attribute name=\"featId\">\
                <xsl:value-of select=\"'False'\"/>\
            </xsl:attribute>\
            </xsl:if>\
        <xsl:if test=\"@colCreator\" >\
            <xsl:attribute name=\"colCreator\">\
                <xsl:value-of select=\"'False'\"/>\
            </xsl:attribute>\
        </xsl:if>\
        <xsl:if test=\"@fixedCol\" >\
            <xsl:attribute name=\"fixedCol\">\
                <xsl:value-of select=\"'True'\"/>\
            </xsl:attribute>\
        </xsl:if>\
        <xsl:apply-templates select=\"node()\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:table\">\
    <xsl:if test=\"not(@name = 'F_CLASSDEFINITION') and not(@name = 'f_classdefinition') and not(@name = 'dbo.f_classdefinition') and not(@name = 'public.f_classdefinition')\">\
        <xsl:variable name=\"tableName\" select=\"@name\"/>\
        <xsl:copy>\
            <xsl:apply-templates select=\"@*\"/>\
            <xsl:apply-templates select=\"lp:property[not(@name = 'FeatId' and ($tableName = 'PARCEL' or $tableName = 'ZONING' or $tableName = 'parcel' or $tableName = 'zoning' or $tableName = 'dbo.Parcel' or $tableName = 'dbo.Zoning' or $tableName = 'dbo.parcel' or $tableName = 'dbo.zoning')) and not(@name = 'ClassId' or @name = 'RevisionNumber' or @name = 'ClassName' or @name = 'SchemaName' or @name='Bounds')]\">\
            <xsl:sort select=\"@name\" />\
            </xsl:apply-templates>\
            <xsl:apply-templates select=\"lp:column\">\
                <xsl:sort select=\"@name\" />\
            </xsl:apply-templates>\
        </xsl:copy>\
    </xsl:if>\
</xsl:template>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
</xsl:template>\
</xsl:stylesheet>";

// Tests the setting of feature schemas from a configuration document.

void FdoApplySchemaTest::TestConfigDoc ()
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoIoMemoryStreamP configStream1 = FdoIoMemoryStream::Create();
    FdoIoMemoryStreamP configStream2 = FdoIoMemoryStream::Create();

    FdoStringP RmvMetaSheet = FdoStringP(pRmvLpMetaSchema) + FdoStringP(pRmvLpMetaSchema2);

    FdoXmlSpatialContextFlagsP scFlags = FdoXmlSpatialContextFlags::Create();
    scFlags->SetIncludeDefault( true );

	try {
        FdoSchemaManagerP mgr;
        const FdoSmLpSchemaCollection* lp = NULL;
        FdoSmPhMgrP ph;

        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_CONFIG_SUFFIX,
            0,
            NULL,
            0
		);
    	
		FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();
        bool supportsRange = schemaCap->SupportsExclusiveValueRangeConstraints();

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_CONFIG_SUFFIX );

        // Create the feature schemas. Just a few from ApplySchemaTest

		printf( "Creating Acad Schema ... \n" );
		CreateAcadSchema( connection );
		printf( "Creating Electric Schema ... \n" );

        CreateElectricSchema( connection );
		printf( "Create complete \n" );

		printf( "Modifying Electric Schema ... \n" );
   		ModElectricSchema( connection );

        printf( "Creating Land Schema ... \n" );
		CreateLandSchema( connection );

        // Take a snapshot of the LogicalPhysical schema
        printf( "Writing original LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();

        // Adjust it to look like it came from a datastore without MetaSchema
        // (for comparing later, read on).
        UnitTestUtil::WriteSortedLp( lp, UnitTestUtil::GetOutputFileName( L"config_lp1a.xml" ), (char*) (const char*) RmvMetaSheet );

        // Generate a config document from the datastore
        printf( "Exporting Logical Schema ... \n" );
        UnitTestUtil::ExportDb( connection, configStream1, scFlags, true );

        // Save config doc to a file
        configStream1->Reset();
        FdoIoFileStreamP fileStream1 = FdoIoFileStream::Create( UnitTestUtil::GetOutputFileName( L"config_logical1a.xml" ),L"wt");

        XmlFormatter formatter1( 
            configStream1, 
            fileStream1
        );
        formatter1.Format();
        fileStream1 = NULL;

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONFIG_SUFFIX
		);

		// remove the MetaSchema tables from the datastore so that
        // we can try the config document against it.
		printf( "Deleting MetaSchema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        ph = mgr->GetPhysicalSchema();

        FdoSmPhOwnerP owner = ph->GetOwner();
        FdoSmPhRdDbObjectReaderP rdr = owner->CreateDbObjectReader();

        while ( rdr->ReadNext() ) {
            FdoStringP objName = rdr->GetString(L"", L"name");

            if ( (objName.Mid( 0, 2 ).ICompare(L"F_") == 0) ||
                 (objName.Mid( 0, 6 ) == L"dbo.f_") ||
                 (objName.Mid( 0, 9 ) == L"public.f_")
            ) {
                FdoSmPhDbObjectP dbObject = owner->CacheDbObject(rdr); 
       
                FdoSmPhTableP table = dbObject.p->SmartCast<FdoSmPhTable>();
                if ( table ) 
                    table->ClearRows();

                if ( dbObject ) 
                    dbObject->SetElementState( FdoSchemaElementState_Deleted );
            }
        }

        ph->Commit();

        configStream1->Reset();

        // Reconnect to datastore. This time set schemas from config doc since
        // MetaSchema no longer present.
		printf( "Reconnecting ... \n" );
        FdoStringP dbConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_NAME_CONFIG_SUFFIX);
        connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConfiguration( configStream1 );
        connection->SetConnectionString ( dbConnectString);
        connection->Open();

        // Export schemas from datastore
        printf( "Exporting Logical Schema ... \n" );
        UnitTestUtil::ExportDb( connection, configStream2, NULL, true );

        // Save to a file
        configStream2->Reset();
        FdoIoFileStreamP fileStream2 = FdoIoFileStream::Create( UnitTestUtil::GetOutputFileName( L"config_logical1b.xml" ),L"wt");

        XmlFormatter formatter2( 
            configStream2, 
            fileStream2
        );
        formatter2.Format();
        fileStream2 = NULL;

        // Take a snapshot of the resulting LogicalPhysical schema.
		printf( "Writing LogicalPhysical schema from config file ... \n" );
        mgr = staticConn->CreateSchemaManager();
        ph = mgr->GetPhysicalSchema();

        // First, must set config document on static connection
        // This means parsing out the schemas and schema mappings
        configStream1->Reset();

        FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
        schemas->ReadXml(configStream1);

        configStream1->Reset();

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        mappings->ReadXml( configStream1 );

        ph->SetConfiguration( 
            FdoPhysicalSchemaMappingP(connection->CreateSchemaMapping())->GetProvider(), 
            FDO_SAFE_ADDREF(configStream1.p),
            schemas, 
            mappings 
        );
        
        lp = mgr->RefLogicalPhysicalSchemas();
        // Sort the LogicalPhysical schemas for easy comparison against original
        UnitTestUtil::WriteSortedLp( lp, UnitTestUtil::GetOutputFileName( L"config_lp1b.xml" ) );

        // Testing config file without overrides.
		printf( "Testing config document without overrides .. \n" );
        mgr = staticConn->CreateSchemaManager();
        ph = mgr->GetPhysicalSchema();
        ph->SetConfiguration( 
            FdoPhysicalSchemaMappingP(connection->CreateSchemaMapping())->GetProvider(), 
            (FdoIoStream *) NULL,
            schemas, 
            (FdoPhysicalSchemaMappingCollection*) NULL 
        );
        lp = mgr->RefLogicalPhysicalSchemas();

        const FdoSmLpClassDefinition* lpClass = lp->FindClass( L"Acad", L"AcDb3dPolyline" );
        CPPUNIT_ASSERT( lpClass );

        const FdoSmLpDbObject* lpTable = lpClass->RefDbObject();
        CPPUNIT_ASSERT( lpTable );
        CPPUNIT_ASSERT( ph->GetDcDbObjectName(L"AcDb3dPolyline") == lpTable->GetName() );

        const FdoSmLpDataPropertyDefinition* lpProp = 
            FdoSmLpDataPropertyDefinition::Cast(lpClass->RefProperties()->RefItem(L"Closed"));
        CPPUNIT_ASSERT(lpProp);

        const FdoSmPhColumn* lpColumn = lpProp->RefColumn();
        CPPUNIT_ASSERT(lpColumn);
        CPPUNIT_ASSERT( ph->GetDcColumnName(L"Closed") == lpColumn->GetName() );

        lpClass = lp->FindClass( L"Electric'l", L"Transformer" );
        CPPUNIT_ASSERT( lpClass );

        const FdoSmLpGeometricPropertyDefinition* lpGeomProp = 
            FdoSmLpGeometricPropertyDefinition::Cast(lpClass->RefProperties()->RefItem(L"Geometry"));
        CPPUNIT_ASSERT(lpProp);

        lpColumn = lpGeomProp->RefColumn();
        CPPUNIT_ASSERT(lpColumn);
        CPPUNIT_ASSERT( ph->GetDcColumnName(L"Geometry") == lpColumn->GetName() );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONFIG_SUFFIX
		);
		printf( "Adding some constraints .. \n" );

        owner = ph->FindOwner();
        FdoSmPhGrdOwner* grdOwner = (FdoSmPhGrdOwner*)(FdoSmPhOwner*) owner;
        FdoSmPhTableP table;
        FdoSmPhCheckConstraintP constraint;
        if ( supportsRange ) {
            table = owner->GetDbObject( ph->GetDcDbObjectName(L"Parcel") )->SmartCast<FdoSmPhTable>();
            constraint = new FdoSmPhCheckConstraint( 
                L"value_check", 
                ph->GetDcColumnName(L"Value"), 
                FdoStringP::Format( L"%ls < 10000000", (FdoString*)(ph->GetDcColumnName(L"Value")) ) 
            );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( 
                FdoStringP::Format(
                    L"alter table %ls add constraint value_check check ( %ls < 10000000 )",
                    (FdoString*)(ph->GetDcDbObjectName(L"Parcel")),
                    (FdoString*)(ph->GetDcColumnName(GetValueColumnName()))
                )
            );
        }

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"Zoning") )->SmartCast<FdoSmPhTable>();

        FdoSmPhColumnsP	ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		int numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"Authority") );
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"ZoningType") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"Transformer") )->SmartCast<FdoSmPhTable>();

        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"Phase") );
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"PartNum") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);

        if ( supportsRange ) {
            constraint = new FdoSmPhCheckConstraint( L"partnum_check", ph->GetDcColumnName(L"PartNum"), 
                FdoStringP::Format( L"%ls > 0", (FdoString*)(ph->GetDcColumnName(L"PartNum")) )
            );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( 
                FdoStringP::Format(
                    L"alter table %ls add constraint partnum_check check ( %ls > 0 )",
                    (FdoString*)(ph->GetDcDbObjectName(L"Transformer")),
                    (FdoString*)(ph->GetDcColumnName(L"PartNum"))
                )
            );
        }

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"Pole") )->SmartCast<FdoSmPhTable>();

        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"Height") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);

        if ( supportsRange ) {
            constraint = new FdoSmPhCheckConstraint( 
                L"height_check", 
                ph->GetDcColumnName(L"height"), 
                FdoStringP::Format(L"%ls > 5", (FdoString*)(ph->GetDcColumnName(L"height")))
            );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( 
                FdoStringP::Format(
                    L"alter table %ls add constraint height_check check ( %ls > 5.1 )",
                    (FdoString*)(ph->GetDcDbObjectName(L"Pole")),
                    (FdoString*)(ph->GetDcColumnName(L"Height"))
                )
            );
        }

        owner->Commit();

        // Reconnect to datastore. This time set schemas from config doc since
        // MetaSchema no longer present.
		printf( "Reconnecting ... \n" );
        connection->SetConfiguration( configStream1 );
        connection->Open();

		printf( "Describing Schemas and verifying added constraints ... \n" );

    	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    	schemas = pDescCmd->Execute();

        FdoFeatureSchemaP schema = schemas->GetItem( L"Land" );
        FdoClassesP classes = schema->GetClasses();

        FdoClassDefinitionP classDef ;
        FdoPropertiesP properties;
        FdoDataPropertyP dataProp;
        FdoPtr<FdoPropertyValueConstraintRange> valCons;
        FdoPtr<FdoDoubleValue> dblVal;
        FdoPtr<FdoDecimalValue> decimalVal;
        FdoPtr<FdoInt32Value> intVal;
        FdoPtr<FdoInt16Value> int16Val;
        FdoInt32 ix;

        if ( supportsRange ) {
            classDef = classes->GetItem( L"Parcel" );
            properties = classDef->GetProperties();
            dataProp = (FdoDataPropertyDefinition*)(properties->GetItem(L"Value"));
            valCons = (FdoPropertyValueConstraintRange*)(dataProp->GetValueConstraint());
            if ( !valCons ) 
                CPPUNIT_FAIL( "Parcel check constraint missing; if on SqlServer 2005 then this is expected" );
            decimalVal = (FdoDecimalValue*)(valCons->GetMaxValue());
            CPPUNIT_ASSERT( decimalVal->GetDecimal() == 10000000 );
        }

        classDef = classes->GetItem( L"Zoning" );
        FdoPtr<FdoUniqueConstraintCollection> constraints = classDef->GetUniqueConstraints();
        FdoPtr<FdoUniqueConstraint> ucons;
        FdoDataPropertiesP dataProps;

		// SchemaMgr is adding an unique constraint on auto-incremented columns
		CPPUNIT_ASSERT( constraints->GetCount() == 2);

        bool constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 2 )
                constraintFound = ( FdoPtr<FdoDataPropertyDefinition>(dataProps->FindItem(L"Authority")) && 
                                    FdoPtr<FdoDataPropertyDefinition>(dataProps->FindItem(L"ZoningType")) );
        }

        CPPUNIT_ASSERT( constraintFound );

        schema = schemas->GetItem( L"Electric'l" );
        classes = schema->GetClasses();

        classDef = classes->GetItem( L"Transformer" );
        constraints = classDef->GetUniqueConstraints();

		CPPUNIT_ASSERT( constraints->GetCount() == 1);

        constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 2 )
                constraintFound = ( FdoPtr<FdoDataPropertyDefinition>(dataProps->FindItem(L"Phase")) && 
                FdoPtr<FdoDataPropertyDefinition>(dataProps->FindItem(L"PartNum")) );
        }

        CPPUNIT_ASSERT( constraintFound );

        if ( supportsRange ) {
            properties = classDef->GetProperties();
            dataProp = (FdoDataPropertyDefinition*)(properties->GetItem(L"PartNum"));
            valCons = (FdoPropertyValueConstraintRange*)(dataProp->GetValueConstraint());
            int16Val = (FdoInt16Value*)(valCons->GetMinValue());
            CPPUNIT_ASSERT( int16Val->GetInt16() == 0 );
        }

        classDef = classes->GetItem( L"Pole" );
        constraints = classDef->GetUniqueConstraints();

		CPPUNIT_ASSERT( constraints->GetCount() == 1);

        constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 1 )
                constraintFound = ( FdoPtr<FdoDataPropertyDefinition>(dataProps->FindItem(L"Height")) != NULL  );
        }

        CPPUNIT_ASSERT( constraintFound );

        if ( supportsRange ) {
            properties = classDef->GetProperties();
            dataProp = (FdoDataPropertyDefinition*)(properties->GetItem(L"Height"));
            valCons = (FdoPropertyValueConstraintRange*)(dataProp->GetValueConstraint());
            dblVal = (FdoDoubleValue*)(valCons->GetMinValue());
            CPPUNIT_ASSERT( dblVal->GetDouble() == 5.1 );
        }

		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONFIG_SUFFIX
		);

        // Compare LogicalPhysical schemas for datastore with MetaSchema and
        // for datastore without MetaSchema but with equivalent config doc.
        // They should be close, the one for datastore with MetaSchema will
        // have some extra system properties. However these were stripped
        // out of config_lp1a.xml to make it identical to config_lp1b.xml
	    UnitTestUtil::CheckOutput( UnitTestUtil::GetOutputFileName( L"config_lp1a.xml" ), 
			UnitTestUtil::GetOutputFileName( L"config_lp1b.xml" ) );

        // Compare Logical Schemas to ensure that config document got set 
        // properly on the FDO connection.
	    // TODO: need to sort files first since property ordering is different
        // in each.
        //UnitTestUtil::CheckOutput( UnitTestUtil::GetOutputFileName( L"config_logical1a.xml" ), 
		//	UnitTestUtil::GetOutputFileName( L"config_logical1b.xml" ));
  
    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
        try {
		    if (connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
        }
        catch ( ... ) 
		{
		}
		throw;
	}
   	catch (...)
   	{
        try {
		    if (connection) connection->Close(); 
            if ( staticConn ) delete staticConn;
        }
        catch ( ... ) 
		{
		}
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
    if ( staticConn ) delete staticConn;

	printf( "Done\n" );
}

void FdoApplySchemaTest::TestNoMeta ()
{
    StaticConnection* staticConn = NULL;
	FdoPtr<FdoIConnection> connection;

	try {
        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        FdoSmPhOwnerP owner = UnitTestUtil::CreateDBNoMeta( 
            staticConn->CreateSchemaManager(),
            UnitTestUtil::GetEnviron("datastore", DB_NAME_NO_META_SUFFIX)
        );


        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_NO_META_SUFFIX
		);

        if ( CanApplyWithoutMetaSchema() ) 
            ApplyNoMetaSuccess( connection, staticConn );
        else
            ApplyNoMetaFailure( connection, staticConn );


        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_SUFFIX
		);

        delete staticConn;
        staticConn = NULL;

	}
	catch ( FdoException* e ) 
	{
		try {
            if ( staticConn ) delete staticConn;
			if (connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
        if ( staticConn ) delete staticConn;
		if (connection) connection->Close(); 
		throw;
	}
   	catch (...)
   	{
        if ( staticConn ) delete staticConn;
		if (connection) connection->Close(); 
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );
}

static char* delLandSheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<stylesheet version=\"1.0\" \
xmlns=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:gml=\"http://www.opengis.net/gml\" \
xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:fdo=\"http://fdo.osgeo.org/schemas\" \
xmlns:ora=\"http://www.autodesk.com/isd/fdo/OracleProvider\" \
xmlns:mql=\"http://fdomysql.osgeo.org/schemas\" \
xmlns:sqs=\"http://www.autodesk.com/isd/fdo/SQLServerProvider\">\
<xsl:template match=\"xs:schema[@targetNamespace='http://fdo.osgeo.org/schemas/feature/Land']\"/>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
</xsl:template>\
</stylesheet>";

void FdoApplySchemaTest::ApplyNoMetaSuccess( FdoIConnection* connection, StaticConnection* staticConn )
{
    FdoStringP providerName = UnitTestUtil::GetEnviron("provider");

    FdoPtr<FdoIGetSpatialContexts> gscCmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
    gscCmd->SetActiveOnly(false);

    FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();

    if  ( !reader->ReadNext() ) {
        UnitTestUtil::CreateSpatialContext( connection, L"Default", L"", -1000, -1000, 1000, 1000 );
    }

    printf( "Creating Non-Default Schema ... \n" );
    bool succeeded = false;
    try {
        CreateAcadSchema(connection, true, true);
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(
            e, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(2,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Creating non-default schema was supposed to fail" );

    printf( "Creating System Schemas ... \n" );
    CreatePhSystemSchemas( connection );

    printf( "Creating Acad Schema ... \n" );
    CreateAcadSchema(connection, false);

    printf( "Creating Electric Schema ... \n" );
    CreateElectricSchema(connection, false);

    printf( "Modifying Electric Schema ... \n" );
	ModElectricSchema( connection, false );

    printf( "Creating Land Schema ... \n" );
    CreateLandSchema( connection, false );

    printf( "Applying mixed updates and deletes ... \n" );
    ModDelSchemas( connection, false );

    printf( "Re-Adding some elements ... \n" );
    ReAddElements(connection, false);

    printf( "Modifying Land Schema ... \n" );
    ModLandSchema(connection, false);

    printf( "Testing non-ASCII7 element names ... \n" );
    CreateNLSSchema( connection, staticConn, false );

    printf( "Deleting Default Schema by DestroySchema ... \n" );
    succeeded = false;
    try {
        DeleteDefaultSchema(connection, false);
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(
            e, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(3,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Deleting default schema was supposed to fail" );

    printf( "Deleting Default Schema by ApplySchema ... \n" );
    succeeded = false;
    try {
        DeleteDefaultSchema(connection, false);
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(
            e, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(4,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Deleting default schema was supposed to fail" );

    printf( "Deleting provider-specific system Schemas ... \n" );
    DeletePhSystemSchemas( staticConn );

    if ( CanCreateSchemaWithoutMetaSchema() ) {
        printf( "Deleting schema with features ... \n" );
        succeeded = false;
        try {
            DeleteAcadSchema(connection);
            succeeded = true;
        }
	    catch ( FdoSchemaException* e )
	    {
		    UnitTestUtil::PrintException(
                e, 
                UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(7,false) ), 
                true 
            );
		    FDO_SAFE_RELEASE(e);
	    }

	    if ( succeeded ) 
		    CPPUNIT_FAIL( "Deleting Acad schema was supposed to fail" );
    }

    printf( "Creating Long String Schema ... \n" );
    succeeded = false;
    try {
        CreateLongStringSchema(connection, false);
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(
            e, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(5,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Creating long string schema was supposed to fail" );

    printf( "Creating  Schema with Overrides ... \n" );
    succeeded = false;
    try {
		CreateOverrideSchema( 
            connection, 
            FdoRdbmsOvSchemaMappingP(CreateOverrides(connection, 1)),
            false,
            true,
            false
        );

        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(
            e, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(6,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Creating overrides schema was supposed to fail" );

    FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
	FdoPtr<FdoIDescribeSchema> cmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoFeatureSchemasP schemas = cmd->Execute();

    schemas->WriteXml(stream);
    stream->Reset();

    FdoStringP masterFile = FdoStringP::Format( L"apply_no_meta_test1_%ls_master.xml", (FdoString*) providerName );
    FdoStringP sortedMasterFile = UnitTestUtil::GetOutputFileName( L"apply_no_meta_test1_master.xml" );
    FdoStringP resultsFile = UnitTestUtil::GetOutputFileName( L"apply_no_meta_test1.xml" );
    FdoStringP masterFile2 = UnitTestUtil::GetOutputFileName( L"apply_no_meta_test2_master.xml" );
    FdoStringP resultsFile2 = UnitTestUtil::GetOutputFileName( L"apply_no_meta_test2.xml" );

    FdoIoFileStreamP masterStream = FdoIoFileStream::Create(masterFile, L"r");
    UnitTestUtil::Config2SortedFile(masterStream, sortedMasterFile );

    UnitTestUtil::Config2SortedFile(stream, resultsFile );

    if ( CanCreateSchemaWithoutMetaSchema() ) {
        printf( "Deleting Land Schema ... \n" );
        DeleteLandSchema( connection, false );
    
        FdoIoMemoryStreamP stylesheetStream = FdoIoMemoryStream::Create();
        stylesheetStream->Write( (FdoByte*) delLandSheet, strlen(delLandSheet) );
        stylesheetStream->Reset();

        stream = FdoIoMemoryStream::Create();

        FdoXslTransformerP tfmr = FdoXslTransformer::Create(
            FdoXmlReaderP( FdoXmlReader::Create(masterFile) ),
            FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
            FdoXmlWriterP( FdoXmlWriter::Create(stream, false) )
        );

        tfmr->Transform();

        stream->Reset();
        UnitTestUtil::Config2SortedFile(stream, masterFile2 );

	    schemas = cmd->Execute();

        stream = FdoIoMemoryStream::Create();
        schemas->WriteXml(stream);
        stream->Reset();

        UnitTestUtil::Config2SortedFile(stream, resultsFile2 );
    }

    UnitTestUtil::CheckOutput( sortedMasterFile,(const char*) resultsFile );

    if ( CanCreateSchemaWithoutMetaSchema() )
        UnitTestUtil::CheckOutput( (const char*) masterFile2,(const char*) resultsFile2 );

#ifdef _WIN32
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(2,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(2,false) )
    );
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(3,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(3,false) )
    );
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(3,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(4,false) )
    );
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(5,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(5,false) )
    );
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(6,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(6,false) )
    );
    if ( CanCreateSchemaWithoutMetaSchema() ) {
	    UnitTestUtil::CheckOutput( 
            SchemaNoMetaErrFile(7,true),
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(7,false) )
        );
    }
#endif

}

void FdoApplySchemaTest::ApplyNoMetaFailure( FdoIConnection* connection, StaticConnection* staticConn )
{
    bool succeeded = false;

    try {
        CreateAcadSchema(connection);
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
        FdoStringP msg = FdoStringP(e->GetExceptionMessage()).Lower();

        FdoPtr<FdoException> e2 = FdoException::Create(msg, FdoPtr<FdoException>(e->GetCause()));

		UnitTestUtil::PrintException(
            e2, 
            UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(1,false) ), 
            true 
        );
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "ApplySchema to non-FDO datastore was supposed to fail" );

#ifdef _WIN32
	UnitTestUtil::CheckOutput( 
        SchemaNoMetaErrFile(1,true),
        UnitTestUtil::GetOutputFileName( SchemaNoMetaErrFile(1,false) )
    );
#endif

}

void FdoApplySchemaTest::ModMetaClassSchema( FdoIConnection* connection )
{

	/* Test some bad modifications */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    pDescCmd->SetSchemaName( L"F_MetaClass" );
    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"F_MetaClass" );

	/* Various class and property mods. Currently, only descriptions can be changed */

	FdoFeatureClassP pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( 0 );
	pClass->SetDescription( L"bad modification" );

	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
#ifdef WIN32
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' );
        if (pMessage) pMessage += 2;
	    CPPUNIT_ASSERT(pMessage && wcscmp( pMessage, L"MetaClass schema F_MetaClass is read-only; cannot modify it ") == 0);
#else
        FdoString* pMessage = e->GetExceptionMessage();
#endif
	    CPPUNIT_ASSERT(pMessage && wcscmp( pMessage, L"MetaClass schema F_MetaClass is read-only; cannot modify it ") == 0);
#else
        FdoString* pMessage = e->GetExceptionMessage();

        CPPUNIT_ASSERT(pMessage && wcscmp( pMessage, L"MetaClass schema F_MetaClass is read-only; cannot modify it") == 0);
#endif
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "F_MetaClass modification was supposed to fail" );
}

void FdoApplySchemaTest::DeleteAcadSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);

	pCmd->SetSchemaName( L"Acad" );
    CPPUNIT_ASSERT(wcscmp(pCmd->GetSchemaName(), L"Acad") == 0);

	pCmd->Execute();
}

void FdoApplySchemaTest::DeleteLandSchema( FdoIConnection* connection, bool hasMetaSchema )
{
    DeleteObjects( connection, L"Land", L"1-8 School" );
    DeleteObjects( connection, L"Land", L"Driveway" );

    if ( hasMetaSchema ) {
#ifdef RDBI_DEF_SSQL
        UnitTestUtil::Sql2Db( L"delete from \"parcel_person\"", connection );
#else
	    UnitTestUtil::Sql2Db( L"delete from parcel_person", connection );
#endif
    }

	FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);

	pCmd->SetSchemaName( L"Acad" );
	pCmd->SetSchemaName( L"Land" );

	pCmd->Execute();
}

void FdoApplySchemaTest::DeleteDefaultSchema( FdoIConnection* connection, bool update )
{
    FdoFeatureSchemaP schema = GetDefaultSchema( connection );

#if 0
    DeleteObjects( connection, L"Land", L"1-8 School" );
    DeleteObjects( connection, L"Land", L"Driveway" );
#endif

    if ( update ) {
	    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        schema->Delete();
        pCmd->SetFeatureSchema( schema );
        pCmd->Execute();
    }
    else {
	    FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);

	    pCmd->SetSchemaName( schema->GetName() );
	    pCmd->Execute();
    }
}


void FdoApplySchemaTest::CreateSystemSchema( FdoIConnection* connection )
{
	FdoStringP datastoreName;

	/* Try to create schema with same name as the datastore.
	   This should fail since each datastore has a system schema
	   with this name.
     */

    datastoreName = UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX );
    datastoreName = datastoreName.Lower();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( datastoreName, L"System schema" );

	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;

	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(10,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "System schema create (lower name) was supposed to fail" );

    pSchema->SetName( datastoreName.Upper() );
    succeeded = false;

    try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(11,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "System schema create (upper name) was supposed to fail" );

}

void FdoApplySchemaTest::DeletePhSystemSchemas( StaticConnection* staticConn )
{
}

void FdoApplySchemaTest::CreatePhSystemSchemas( FdoIConnection* connection )
{
}

void FdoApplySchemaTest::CreateAcadSchema( FdoIConnection* connection, bool hasMetaSchema, bool addSAD )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema;
    
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );
    else
        pSchema = GetDefaultSchema( connection );

	// Id'less class for Object Properties only

	FdoPtr<FdoClass> pXData = FdoClass::Create( L"AcXData", L"Xdata" );
	pXData->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pXDataSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pXDataSeq->SetDataType( FdoDataType_Int32 );
	pXDataSeq->SetNullable(false);
	FdoPropertiesP(pXData->GetProperties())->Add( pXDataSeq  );

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"Datavalue", L"datavalue" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(4000);
	pProp->SetNullable(false);
	FdoPropertiesP(pXData->GetProperties())->Add( pProp );

    if ( hasMetaSchema ) 
    	FdoClassesP(pSchema->GetClasses())->Add( pXData );

	// More Id'less classes used to test unique table name generation

	FdoPtr<FdoClass> pCoordVal = FdoClass::Create( L"AcDbVertexCoordinateValue", L"" );
	pCoordVal->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pCoordValSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pCoordValSeq->SetDataType( FdoDataType_Int32 );
	pCoordValSeq->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pCoordValSeq  );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pProp  );

    if ( hasMetaSchema ) 
    	FdoClassesP(pSchema->GetClasses())->Add( pCoordVal );

	FdoPtr<FdoClass> pVertex = FdoClass::Create( L"AcDbVertexData", L"" );
	pVertex->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pVertexSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pVertexSeq->SetDataType( FdoDataType_Int32 );
	pVertexSeq->SetNullable(false);
	FdoPropertiesP(pVertex->GetProperties())->Add( pVertexSeq  );

	FdoObjectPropertyP  pObjProp = FdoObjectPropertyDefinition::Create( L"point", L"" );
	pObjProp->SetClass( pCoordVal );
	pObjProp->SetIdentityProperty( pCoordValSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pVertex->GetProperties())->Add( pObjProp );

    if ( hasMetaSchema ) 
    	FdoClassesP(pSchema->GetClasses())->Add( pVertex );

	// A non-abstract base class

	FdoPtr<FdoFeatureClass> pEntClass = FdoFeatureClass::Create( L"AcDbEntity", L"AutoCAD entity base class" );
	pEntClass->SetIsAbstract(false);
    // Test unsetting geometry for class with no geometric properties - should have no effect.
    pEntClass->SetGeometryProperty(NULL);

    if ( addSAD ) 
       	FdoSADP(pEntClass->GetAttributes())->Add( L"classbadsad", L"error" );

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Layer", L"Acad layer" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );
    if ( addSAD ) 
       	FdoSADP(pProp->GetAttributes())->Add( L"propbadsad", L"error" );

	pProp = FdoDataPropertyDefinition::Create( L"ColourIndex", L"Acad Colour" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );

    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	    pObjProp->SetClass( pXData );
	    pObjProp->SetIdentityProperty( pXDataSeq );
	    pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	    pObjProp->SetOrderType( FdoOrderType_Ascending );
	    FdoPropertiesP(pEntClass->GetProperties())->Add( pObjProp );
    }

    FdoClassesP(pSchema->GetClasses())->Add( pEntClass );

	FdoPtr<FdoClass> pEntRefClass = FdoClass::Create( L"Entity", L"Embedded entity base class" );
	pEntRefClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntRefClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Layer", L"Acad layer" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ColourIndex", L"Acad Colour" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );

    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	    pObjProp->SetClass( pXData );
	    pObjProp->SetIdentityProperty( pXDataSeq );
	    pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	    pObjProp->SetOrderType( FdoOrderType_Ascending );
	    FdoPropertiesP(pEntRefClass->GetProperties())->Add( pObjProp );
    }

    FdoClassesP(pSchema->GetClasses())->Add( pEntRefClass );

	// A sub-class to test Object and Data property inheritance.

	FdoPtr<FdoFeatureClass> pPlineClass = FdoFeatureClass::Create( L"AcDb3dPolyline", L"AutoCAD 3d polyline" );
	pPlineClass->SetIsAbstract(false);
	pPlineClass->SetBaseClass( pEntClass );
	FdoClassesP(pSchema->GetClasses())->Add( pPlineClass );

	pProp = FdoDataPropertyDefinition::Create( L"Width", L"line width" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetPrecision(10);
	pProp->SetScale(5);
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

    pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(!CanAddNotNullCol());
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	    pObjProp->SetClass( pVertex );
	    pObjProp->SetIdentityProperty( pVertexSeq );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pPlineClass->GetProperties())->Add( pObjProp );
    }

	FdoPtr<FdoClass> pPlineRefClass = FdoClass::Create( L"Polyline", L"Embedded 3d polyline" );
	pPlineRefClass->SetIsAbstract(false);
	pPlineRefClass->SetBaseClass( pEntRefClass );
	FdoClassesP(pSchema->GetClasses())->Add( pPlineRefClass );

	pProp = FdoDataPropertyDefinition::Create( L"Width", L"line width" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetPrecision(10);
	pProp->SetScale(5);
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pProp );

    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	    pObjProp->SetClass( pVertex );
	    pObjProp->SetIdentityProperty( pVertexSeq );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pObjProp );
    }

	// The following tests object property nesting to 3 levels.

	FdoPtr<FdoFeatureClass> pHatchClass = FdoFeatureClass::Create( L"AcDbHatch", L"AutoCAD hatched polygon" );
	pHatchClass->SetIsAbstract(false);
	pHatchClass->SetBaseClass( pEntClass );

    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"edges", L"" );
	    pObjProp->SetClass( pPlineRefClass );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pHatchClass->GetProperties())->Add( pObjProp );
    }

	FdoClassesP(pSchema->GetClasses())->Add( pHatchClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

  	// Insert a row with null colour. Subsequent removal of colour property should succeed.

    InsertObject( connection, false, pSchema->GetName(), L"AcDbEntity", L"Layer", L"default", NULL );
    InsertObject( connection, false, pSchema->GetName(), L"Entity", L"FeatId", L"1", L"Layer", L"default", NULL );
}

void FdoApplySchemaTest::CreateElectricSchema( FdoIConnection* connection, bool hasMetaSchema )
{
    FdoPtr<FdoFeatureSchemaCollection> pAcadSchema;
    FdoPtr<FdoFeatureSchema> pSchema;
    FdoPtr<FdoClassDefinition> pEntClass;

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) {
	    FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	    pDescCmd->SetSchemaName( L"Acad" );
        
	    pAcadSchema = pDescCmd->Execute();

	    pEntClass = FdoClassesP(FdoFeatureSchemaP(pAcadSchema->GetItem( L"Acad" ))->GetClasses())->GetItem( L"Entity" );

    	/* A schema with dictionary */
        pSchema = FdoFeatureSchema::Create( L"Electric'l", L"Electrical '' schema'" );

        if ( hasMetaSchema ) 
        	FdoSADP(pSchema->GetAttributes())->Add( L"'Author", L"Thomas O'Edison" );
    }
    else {
        pSchema = GetDefaultSchema( connection );
	    pEntClass = FdoClassesP(pSchema->GetClasses())->GetItem( L"Entity" );
    }

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* An abstract base class */

	FdoPtr<FdoFeatureClass> pDevClass = FdoFeatureClass::Create( L"ElectricDevice", L"electic base class" );
	pDevClass->SetIsAbstract(true);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDevClass->GetIdentityProperties())->Add( pProp );

    if ( hasMetaSchema ) {
	    // Test nested object properties (ElectricDevice.graphic.xdata) where graphic's class has an id.

	    FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"graphic", L"Acad entity" );
	    pObjProp->SetObjectType( FdoObjectType_Value );
	    pObjProp->SetClass( pEntClass );
	    FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );
    }

	// Test geometry property

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );

    FdoGeometryType geoms[] = {FdoGeometryType_Point, FdoGeometryType_LineString, FdoGeometryType_CurveString};
    pGeomProp->SetSpecificGeometryTypes( geoms, 3 );
	pGeomProp->SetHasElevation(true);
    FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );

	/* Subclass with dictionary */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Transformer", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

    if ( hasMetaSchema ) {
     	FdoSADP(pClass->GetAttributes())->Add( L"Rules' DLL", L"transformer.dll" );
	    FdoSADP(pClass->GetAttributes())->Add( L"Entrypoint", L"start_transformer" );
    }

	// Add data properties of various types

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"voltage" );
    if ( hasMetaSchema ) 
    	FdoSADP(pProp->GetAttributes())->Add( L"Calculable", L"yes" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);

#ifdef RDBI_DEF_ORA
    pProp->SetScale(-2);
#else
	pProp->SetScale(1);
#endif

	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Phase", L"A, B or C" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(1);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastInspectDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastRepairDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PartNum", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	pProp->SetDataType( FdoDataType_Single );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"SerialNum", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::CreateLongStringSchema( FdoIConnection* connection, bool hasMetaSchema )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    /* Test various long schema, class and property names. */

	FdoPtr<FdoFeatureSchema> pSchema;
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = FdoFeatureSchema::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
    else
        pSchema = GetDefaultSchema( connection );

    /* Create some id'less object property classes. */

	FdoPtr<FdoClass> pNestedObjClass = FdoClass::Create( L"oacdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pNestedObjClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pNestedObjClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pNestedObjClass );

    FdoPtr<FdoObjectPropertyDefinition> pObjProp;
    FdoPtr<FdoClass> pObjClass;

    if ( hasMetaSchema ) {
	    pObjClass = FdoClass::Create( L"obcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	    pObjClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
	    pProp->SetLength(50);
	    FdoPropertiesP(pObjClass->GetProperties())->Add( pProp );

        pObjProp = FdoObjectPropertyDefinition::Create( L"cbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pNestedObjClass );
	    FdoPropertiesP(pObjClass->GetProperties())->Add( pObjProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pObjClass );
    }

	FdoPtr<FdoFeatureClass> pBaseClass = FdoFeatureClass::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pBaseClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);

    if ( hasMetaSchema ) {
        // Create a SAD entry with long name and value
        FdoSADP(pProp->GetAttributes())->Add( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", 
            L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
            L"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012the-end" 
        );
    }

	FdoPropertiesP(pBaseClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBaseClass->GetIdentityProperties())->Add( pProp );

    if ( hasMetaSchema ) {
        pObjProp = FdoObjectPropertyDefinition::Create( L"cbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pObjClass );
	    FdoPropertiesP(pBaseClass->GetProperties())->Add( pObjProp );

        // For object properties, f_attributedefinition.columnname is set to 'n/a'. Make sure property 'n/a' doesn't conflict

	    pProp = FdoDataPropertyDefinition::Create( L"n/a", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
	    pProp->SetLength(50);
	    FdoPropertiesP(pBaseClass->GetProperties())->Add( pProp );
    	
        pObjProp = FdoObjectPropertyDefinition::Create( L"cbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456788", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pObjClass );
	    FdoPropertiesP(pBaseClass->GetProperties())->Add( pObjProp );
    }

    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"gbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	FdoPropertiesP(pBaseClass->GetProperties())->Add( pGeomProp );

	pBaseClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pBaseClass );

    // Test creating a sub-class of a class with long name

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"bbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pClass->SetIsAbstract(false);
    pClass->SetBaseClass(pBaseClass);

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::CreateLandSchema( FdoIConnection* connection, bool hasMetaSchema )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
    	pDescCmd->SetSchemaName( L"Acad" );

	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    CreateLandSchema( pSchemas, hasMetaSchema );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema;
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = pSchemas->GetItem(L"Land");
    else
        pSchema = pSchemas->GetItem(0);

    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

    // Test GetFeatureSchema
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        CPPUNIT_ASSERT(wcscmp(FdoFeatureSchemaP(pCmd->GetFeatureSchema())->GetName(), L"Land") == 0);

    InsertObject(connection, false, pSchema->GetName(), L"1-8 School", L"# Rooms", L"20", NULL );
    InsertObject(connection, false, pSchema->GetName(), L"Driveway", L"Pav'd", L"1", NULL );

    if ( hasMetaSchema ) {
#ifdef RDBI_DEF_ORA
        UnitTestUtil::Sql2Db( 
            FdoStringP::Format(
                L"insert into parcel_person ( %ls, %ls, parcel_province, parcel_pin ) values ( 'Fraser', 'Simon', 'Ontario', '1234-5678' )",
                (FdoString*) GetParcelFirstName(),
                (FdoString*) GetParcelLastName()
            ),
            connection 
        );
#else
        UnitTestUtil::Sql2Db( 
            FdoStringP::Format(
                L"insert into \"parcel_person\" ( \"%ls\", \"%ls\", \"parcel_province\", \"parcel_pin\" ) values ( 'Fraser', 'Simon', 'Ontario', '1234-5678' )",
                (FdoString*) GetParcelFirstName(),
                (FdoString*) GetParcelLastName()
            ),
            connection 
        );
#endif
    }
}

void FdoApplySchemaTest::CreateLandSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema )
{
	FdoPtr<FdoFeatureSchema> pAcadSchema;
    FdoPtr<FdoFeatureSchema> pSchema;
    
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) {
	    pAcadSchema = pSchemas->GetItem(L"Acad");

        /* Create a schema to test successful schema deletion */

	    pSchema = FdoFeatureSchema::Create( L"Land", L"Property schema" );
	    pSchemas->Add( pSchema );
    }
    else {
	    pAcadSchema = pSchemas->GetItem(0);
	    pSchema = pAcadSchema;
    }

    FdoPtr<FdoClassDefinition> pEntityClass = FdoClassesP(pAcadSchema->GetClasses())->GetItem(L"AcDbEntity");

    FdoPtr<FdoClass> pPersonClass = FdoClass::Create( L"Person", L"" );
	pPersonClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pPersonClass );

	FdoPtr<FdoFeatureClass> pDrvClass = FdoFeatureClass::Create( L"Driveway", L"" );
	pDrvClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDrvClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Pav'd", L"'''" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry'", L"location's" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    FdoGeometryType geoms[] = {FdoGeometryType_LineString, FdoGeometryType_MultiLineString};
    pGeomProp->SetSpecificGeometryTypes( geoms, 2 );
    pGeomProp->SetHasMeasure(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );
	pDrvClass->SetGeometryProperty(pGeomProp);

    // Test adding second geometric property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"LabelPoint", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point  );
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDrvClass );

	/* Test feature class with id properties not FeatId */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Parcel", L"land parcel" );
	pClass->SetIsAbstract(false);

	FdoPtr<FdoUniqueConstraintCollection> constraints = pClass->GetUniqueConstraints();
	FdoPtr<FdoUniqueConstraint>  constraint = FdoUniqueConstraint::Create();
	constraints->Add( constraint );

	pProp = FdoDataPropertyDefinition::Create( L"Province", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );
	FdoDataPropertiesP(constraint->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(15);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );
	FdoDataPropertiesP(constraint->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetNullable(true);
	pProp->SetPrecision( 8 );
	pProp->SetScale( 0 );
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp;
    if ( hasMetaSchema ) {
	    pObjProp = FdoObjectPropertyDefinition::Create( L"owner", L"" );
	    pObjProp->SetObjectType( FdoObjectType_Value );
	    pObjProp->SetClass( pPersonClass );
	    FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );
    }

    // Test adding geometric property that is not the geometry property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Grading", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Surface  );
    FdoGeometryType geoms2[] = {FdoGeometryType_CurvePolygon};
    pGeomProp->SetSpecificGeometryTypes( geoms2, 1 );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    if ( hasMetaSchema ) {
    	/* Create class with base class in different schema */
	    
        pClass = FdoFeatureClass::Create( L"Cogo Point", L"'Surveyor's point" );
	    pClass->SetIsAbstract(false);
	    pClass->SetBaseClass( pEntityClass );
    
	    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    }

    FdoPtr<FdoFeatureClass> pBldgClass = FdoFeatureClass::Create( L"Build'g", L"'" );
	pBldgClass->SetIsAbstract(true);

	constraints = pBldgClass->GetUniqueConstraints();
	constraint = FdoUniqueConstraint::Create();
	constraints->Add( constraint );

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBldgClass->GetIdentityProperties())->Add( pProp );
	FdoDataPropertiesP(constraint->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"# Rooms", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"% Occupied", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(constraint->GetProperties())->Add( pProp );

	// Add a property with same name as a system property except for capitalization.
	// Since columns are case-insensitve, this property's column should end up being
	// named "CLASSNAME1" so that the column doesn't get accidently matched to the 
	// "ClassName" system property as well.
	pProp = FdoDataPropertyDefinition::Create( L"classname", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(255);
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp  );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    FdoGeometryType geoms3[] = {FdoGeometryType_LineString, FdoGeometryType_MultiLineString, FdoGeometryType_CurveString};
	pGeomProp->SetSpecificGeometryTypes( geoms3, 3 );
    pGeomProp->SetHasElevation(true);
    pGeomProp->SetHasMeasure(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pGeomProp );
	pBldgClass->SetGeometryProperty(pGeomProp);

	FdoClassesP(pSchema->GetClasses())->Add( pBldgClass );

	pClass = FdoFeatureClass::Create( L"1-8 School", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pBldgClass );
    // Test setting geometry property to an inherited geometric property.
    pClass->SetGeometryProperty(pGeomProp);

	pProp = FdoDataPropertyDefinition::Create( L"# Occupied", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    /* Test feature class with a non-FeatId autogenerated id property */

	pClass = FdoFeatureClass::Create( L"Zoning", L"land use zone" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"ByLaw", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PolyNum", L"polygon id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    // Schema Manager automatically puts a unique constraint on an
    // autogenerated property.
    // Make sure constraint not duplicated when explicitly specified.
    FdoPtr<FdoUniqueConstraint> constr = FdoUniqueConstraint::Create();
    FdoDataPropertiesP(constr->GetProperties())->Add( pProp );
    FdoPtr<FdoUniqueConstraintCollection>(pClass->GetUniqueConstraints())->Add( constr );

	pProp = FdoDataPropertyDefinition::Create( L"Authority", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(30);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ZoningType", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pClass = FdoFeatureClass::Create( L"Township", L"" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location's" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
 	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

    // Test adding second geometric property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"LabelPoint", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point  );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pClass = FdoFeatureClass::Create( L"County", L"" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location's" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
 	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

    // Test adding second geometric property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"LabelPoint", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point  );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );
}

void FdoApplySchemaTest::CreateLTSchema( FdoIConnection* connection )
{

    /* Create a schema to test successful schema deletion */

	FdoFeatureSchemaP pSchema = FdoFeatureSchema::Create( L"LT", L"" );

	FdoPtr<FdoClass> pGrip = FdoClass::Create( L"Grip", L"" );
	pGrip->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pGripSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pGripSeq->SetDataType( FdoDataType_Int32 );
	pGripSeq->SetNullable(false);
	FdoPropertiesP(pGrip->GetProperties())->Add( pGripSeq  );

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"Datavalue", L"datavalue" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(4000);
	pProp->SetNullable(false);
	FdoPropertiesP(pGrip->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pGrip );

    FdoFeatureClassP pClass = FdoFeatureClass::Create( L"Circle Lt", L"" );
    pClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"Version", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    	
    FdoPropertiesP pProps = pClass->GetProperties();
    pProps->Add( pProp );
    FdoDataPropertiesP pIdProps = pClass->GetIdentityProperties();
    pIdProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"NextVer", L"" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProps->Add( pProp );

    FdoClassesP pClasses = pSchema->GetClasses();
    pClasses->Add( pClass );

	FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( L"grips", L"" );
	pObjProp->SetClass( pGrip );
	pObjProp->SetIdentityProperty( pGripSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

    pClass = FdoFeatureClass::Create( L"DelStatus", L"" );
    pClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    	
    pProps = pClass->GetProperties();
    pProps->Add( pProp );
    pIdProps = pClass->GetIdentityProperties();
    pIdProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"LtLock", L"" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProps->Add( pProp );

    pClasses->Add( pClass );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}


void FdoApplySchemaTest::CreateErrorSchema( FdoIConnection* connection )
{
	bool succeeded = false;
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* Create schema with errors */

	/* First create a schema with some bad base classes */

	FdoPtr<FdoFeatureSchema> pGhostSchema = FdoFeatureSchema::Create( L"Acad", L"Not in database" );

	// AcDbEntity with different class type than one in datastore.

	FdoPtr<FdoClass> pEntity = FdoClass::Create( L"AcDbEntity", L"" );
	pEntity->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEntity->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntity->GetIdentityProperties())->Add( pProp );

	// Base class that isn't in datastore.

	FdoPtr<FdoClass> pGhostClass = FdoClass::Create( L"Ghost", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pGhostClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pGhostClass->GetIdentityProperties())->Add( pProp );

	FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);
	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Error", L"Schema with errors" );
	pSchemas->Add(pSchema);

	// Abstract class that gets assigned to object property

	FdoPtr<FdoClass> pAbstract = FdoClass::Create( L"Abstract Class", L"" );
	pAbstract->SetIsAbstract(true);

	// Orphan base and object property class

	FdoPtr<FdoClass> pOrphanClass = FdoClass::Create( L"Orphan Class", L"" );
	FdoPtr<FdoClass> pOrphanSubClass = FdoClass::Create( L"Orphan SubClass", L"" );
	pOrphanSubClass->SetBaseClass( pOrphanClass );

	FdoPtr<FdoClass> pOpClass = FdoClass::Create( L"Object Property Class", L"" );
	pOpClass->SetIsAbstract(false);
	FdoPtr<FdoDataPropertyDefinition> pOpSeqProp = FdoDataPropertyDefinition::Create( L"Seq", L"" );
	pOpSeqProp->SetDataType( FdoDataType_Int64 );
	pOpSeqProp->SetNullable(false);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pOpSeqProp );

	// For testing the setting of an object property to a feature class.

	FdoPtr<FdoFeatureClass> pFeatClass = FdoFeatureClass::Create( L"Feature object property class", L"" );
	pOpClass->SetIsAbstract(false);

    // FeatId property without autogeneration.
	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pFeatClass->GetIdentityProperties())->Add( pProp );

	// Circular object property. References its containing class.
	// Schema Manager should probably support this post-R1.

	FdoPtr<FdoClass> pCircularOp = FdoClass::Create( L"Circular Op", L"" );
	pCircularOp->SetIsAbstract(false);
	FdoPtr<FdoObjectPropertyDefinition> pCircObjProp = FdoObjectPropertyDefinition::Create( L"op1", L"" );
	pCircObjProp->SetClass( pCircularOp );
	FdoPropertiesP( pCircularOp->GetProperties() )->Add( pCircObjProp );

	// Create class with circular and  abstract object properties, plus an object
	// property with non-existent class.

	FdoPtr<FdoClass> pErrorOp = FdoClass::Create( L"Error Op", L"" );
	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"abstractOp", L"" );
	pObjProp->SetClass( pAbstract );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );
	pObjProp = FdoObjectPropertyDefinition::Create( L"circularOp", L"" );
	pObjProp->SetClass( pCircularOp );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );
	pObjProp = FdoObjectPropertyDefinition::Create( L"missingOp", L"" );
	pObjProp->SetClass( pGhostClass );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );
	pObjProp = FdoObjectPropertyDefinition::Create( L"ValueWithId", L"" );
	pObjProp->SetClass( pOpClass );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetIdentityProperty( pOpSeqProp );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );
	pObjProp = FdoObjectPropertyDefinition::Create( L"orphanClass", L"" );
	pObjProp->SetClass( pOrphanClass );
	pObjProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );
	pObjProp = FdoObjectPropertyDefinition::Create( L"featureClass", L"" );
	pObjProp->SetClass( pFeatClass );
	pObjProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP( pErrorOp->GetProperties() )->Add( pObjProp );

	// Create class whose class type differs from base class.

	FdoPtr<FdoClass> pBaseConf = FdoClass::Create( L"Base Conflict", L"" );
	pBaseConf->SetBaseClass( pEntity );

	// Create class whose base class will not be in the database.

	FdoPtr<FdoClass> pBaseMissing = FdoClass::Create( L"Base Missing", L"" );
	pBaseMissing->SetBaseClass( pGhostClass );

	// Create some invalid data properties

	// Outside valid string length range

	pProp = FdoDataPropertyDefinition::Create( L"neg length", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 0 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Note: this one won't log an error until defect 539024 is fixed.
	pProp = FdoDataPropertyDefinition::Create( L"length too long", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(15000);
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Outside precision and scale ranges (for Oracle ).

	pProp = FdoDataPropertyDefinition::Create( L"precision too low", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision( 0 );  // NOTE: -1 triggers exception at the FDO level since it's always wrong.
	pProp->SetScale( 0 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"precision too high", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision( 66 );
	pProp->SetScale( 0 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"scale too low", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision( 5 );
	pProp->SetScale( -85 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"scale too high", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision( 5 );
	pProp->SetScale( 128 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"has default", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 5 );
	pProp->SetDefaultValue( L"A" );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"type not autogen supported", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 5 );
	pProp->SetIsAutoGenerated( true );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Blob and Clob ( not supported )

	pProp = FdoDataPropertyDefinition::Create( L"Blob Prop", L"" );
	pProp->SetDataType( FdoDataType_BLOB );
	pProp->SetLength( 20000 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Clob Prop", L"" );
	pProp->SetDataType( FdoDataType_CLOB );
	pProp->SetLength( 20000 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Class with 1 nullable id property

	FdoPtr<FdoFeatureClass> pNullIds1 = FdoFeatureClass::Create( L"NullIds1", L"" );

    // Set Geometry Property to Geometric property that doesn't belong to the class.
   	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
	pNullIds1->SetGeometryProperty(pGeomProp);

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds1->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds1->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds1->GetProperties())->Add( pProp );

	// Class with multiple nullable id properties

	FdoPtr<FdoFeatureClass> pNullIds2 = FdoFeatureClass::Create( L"NullIds2", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Middle Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );

	// Class with multiple nullable id properties

	FdoPtr<FdoFeatureClass> pReadOnlyId = FdoFeatureClass::Create( L"ReadOnlyId", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetReadOnly(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Middle Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetReadOnly(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );

	// A feature class without identity properties.

	FdoPtr<FdoFeatureClass> pFeatNoId = FdoFeatureClass::Create( L"FeatNoId", L"" );
	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pFeatNoId->GetProperties())->Add( pProp );

	// A feature class based on a class without identity properties

	FdoPtr<FdoFeatureClass> pSubFeatNoId = FdoFeatureClass::Create( L"SubFeatNoId", L"" );
	pProp = FdoDataPropertyDefinition::Create( L"Dataw", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pSubFeatNoId->GetProperties())->Add( pProp );

	// classes with names or descriptions that are too long

	FdoPtr<FdoClass> pNameTooLong = FdoClass::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890", L"" );
	pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNameTooLong->GetProperties())->Add( pProp );

	FdoPtr<FdoClass> pDescTooLong = FdoClass::Create( L"desc_too_long", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890" );
	pProp = FdoDataPropertyDefinition::Create( L"desc_too_long", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );

    // SAD name and value too long: the following name has 256 char and the value has 4001
    FdoSADP(pProp->GetAttributes())->Add( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890", 
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" 
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
        L"1"
    );
	
    FdoPropertiesP(pDescTooLong->GetProperties())->Add( pProp );

	FdoPtr<FdoFeatureClass> pBadDefaults = FdoFeatureClass::Create( L"BadDefaults", L"" );

    pProp = FdoDataPropertyDefinition::Create( L"int32", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBadDefaults->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"int64a", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"ABC");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"datetime1", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"15-OCT-2001");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"datetime2", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"string");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"datetime3", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"TIMESTAMP '12345'");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"datetime4", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"TIMESTAMP '2001-05-12 00'");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"datetime5", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(false);
    pProp->SetDefaultValue(L"TIME '11:00:15'");
	FdoPropertiesP(pBadDefaults->GetProperties())->Add( pProp );

    FdoClassesP(pGhostSchema->GetClasses())->Add( pEntity );
	FdoClassesP(pGhostSchema->GetClasses())->Add( pGhostClass);
	FdoClassesP(pSchema->GetClasses())->Add( pFeatClass);
	FdoClassesP(pSchema->GetClasses())->Add( pAbstract );
	FdoClassesP(pSchema->GetClasses())->Add( pOpClass );
	FdoClassesP(pSchema->GetClasses())->Add( pCircularOp );
	FdoClassesP(pSchema->GetClasses())->Add( pErrorOp );
	FdoClassesP(pSchema->GetClasses())->Add( pBaseConf );
	FdoClassesP(pSchema->GetClasses())->Add( pBaseMissing );
	FdoClassesP(pSchema->GetClasses())->Add( pOrphanSubClass );
	FdoClassesP(pSchema->GetClasses())->Add( pNullIds1 );
	FdoClassesP(pSchema->GetClasses())->Add( pNullIds2 );
	FdoClassesP(pSchema->GetClasses())->Add( pReadOnlyId );
	FdoClassesP(pSchema->GetClasses())->Add( pFeatNoId );
	FdoClassesP(pSchema->GetClasses())->Add( pSubFeatNoId );
	FdoClassesP(pSchema->GetClasses())->Add( pNameTooLong );
	FdoClassesP(pSchema->GetClasses())->Add( pDescTooLong );
	FdoClassesP(pSchema->GetClasses())->Add( pBadDefaults );

	pCmd->SetFeatureSchema( pSchema );

	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(3,false), true);
		FDO_SAFE_RELEASE(e);
	}

	CPPUNIT_ASSERT( !succeeded );

	// Todo: Fdo goes into infinite recursion if the object class loop 
	// isn't broken before free'ing.
	pCircObjProp->SetClass(NULL);

	/* Test modifying a non-existent schema */

	pSchemas = FdoFeatureSchemaCollection::Create(NULL);
	pSchema = FdoFeatureSchema::Create( L"Non Existent", L"" );
	pSchemas->Add(pSchema);
	pSchema->AcceptChanges();
	pSchema->SetDescription( L"Modify non-existent schema" );

	pCmd->SetFeatureSchema( pSchema );

	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(4,false), true);
		FDO_SAFE_RELEASE(e);
	}

	CPPUNIT_ASSERT( !succeeded );

}

void FdoApplySchemaTest::CreateNLSSchema( FdoIConnection* connection, StaticConnection* /*staticConn*/, bool hasMetaSchema )
{
	FdoPtr<FdoFeatureSchema> pSchema;

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) {
	    pSchema = FdoFeatureSchema::Create( 
            FdoStringP::Format(
                L"%lc%lc%lc", 
                UnitTestUtil::GetNlsChar(1),
                UnitTestUtil::GetNlsChar(2),
                UnitTestUtil::GetNlsChar(3)
            ),
            L"NLS Schema"
        );
    }
    else {
        pSchema = GetDefaultSchema( connection );
    }

    /* Name completely non-ASCII7 */
	FdoPtr<FdoClass> pPersonClass = FdoClass::Create( 
        FdoStringP::Format(
            L"%lc%lc%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"" 
    );
	pPersonClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"One%lc%lc%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"" 
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPersonClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"Two%lc%lc%lc%lcplus%lc%lc%lc%lcterm", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(1)
        ),
        L""
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPersonClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"Two%lc%lc%lc%lcplus%lc%lc%lc%lcterm", 
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(1)
        ),
        L"" 
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"Two%lc%lc%lc%lcplus_%lc%lc%lc%lc__term", 
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"" 
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

    FdoClassesP(pSchema->GetClasses())->Add( pPersonClass );

	FdoPtr<FdoFeatureClass> pDrvClass = FdoFeatureClass::Create( 
        FdoStringP::Format(
            L"Three%lc%lc%lc%lcplus%lcplus%lc%lc%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"" 
    );
	pDrvClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"Three%lc%lc%lc%lcplus%lc%lc%lc%lcplus%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(3)
        ),
        L"" 
    );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDrvClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"%lc%lc%lc%lc'#term", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"'''" 
    );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( 
        FdoStringP::Format(
            L"%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2)
        ),
        L"location's" 
    );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    FdoGeometryType geoms[] = {FdoGeometryType_LineString, FdoGeometryType_MultiLineString};
    pGeomProp->SetSpecificGeometryTypes( geoms, 2 );
    pGeomProp->SetHasMeasure(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );
	pDrvClass->SetGeometryProperty(pGeomProp);

    // Test adding second geometric property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( 
        FdoStringP::Format(
            L"One%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2)
        ),
        L"secondary geometry" 
    );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point  );
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDrvClass );

    /* Name completely non-ASCII7 */
	pPersonClass = FdoClass::Create( 
        FdoStringP::Format(
            L"%lc%lc%lc%lc", 
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3)
        ),
        L"" 
    );
	pPersonClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( 
        FdoStringP::Format(
            L"One%lc%lc%lc%lc", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4)
        ),
        L"" 
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPersonClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create(
        FdoStringP::Format(
            L"Two%lc%lc%lc%lcplus%lc%lc%lc%lcterm", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(4),
            UnitTestUtil::GetNlsChar(3),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(1)
        ),
        L""
    );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPersonClass->GetIdentityProperties())->Add( pProp );

    FdoClassesP(pSchema->GetClasses())->Add( pPersonClass );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( pSchema );

	pCmd->Execute();
}

void FdoApplySchemaTest::ModElectricSchema( FdoIConnection* connection, bool hasMetaSchema )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pDescCmd->SetSchemaName( L"Electric'l" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

    ModElectricSchema( pSchemas, hasMetaSchema );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema;
    
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = pSchemas->GetItem( L"Electric'l" );
    else
        pSchema = pSchemas->GetItem(0);

	pCmd->SetFeatureSchema( pSchema );
    pCmd->Execute();
}

void FdoApplySchemaTest::ModElectricSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoFeatureSchema> pSchema;
    FdoPtr<FdoClass> pMaintHist;
    FdoPtr<FdoDataPropertyDefinition> pHistId;    
    FdoPtr<FdoDataPropertyDefinition> pProp;
    FdoObjectPropertyP pObjProp;

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = pSchemas->GetItem( L"Electric'l" );
    else
        pSchema = pSchemas->GetItem(0);

	// Modify schema dictionary


    if ( hasMetaSchema ) {
	    FdoSADP(pSchema->GetAttributes())->SetAttributeValue( L"'Author", L"Nikola Tesla" );
	    FdoSADP(pSchema->GetAttributes())->Add( L"Status", L"Draft" );

	    /* The following 3 classes allow testing of adding nested object properties, where both object
	     * property classes have no id properties.
	     */

	    // Set up a base class for testing an inherited object property class.
	    FdoPtr<FdoClass> pWorkItem = FdoClass::Create( L"Work Item", L"Work Item" );
	    pWorkItem->SetIsAbstract(true);

	    FdoPtr<FdoDataPropertyDefinition> pItemId = FdoDataPropertyDefinition::Create( L"It'm #", L"sequence identifier" );
	    pItemId->SetDataType( FdoDataType_Int16 );
	    pItemId->SetNullable(false);
	    FdoPropertiesP(pWorkItem->GetProperties())->Add( pItemId );

	    // Add a property with same name as a system property except for capitalization.
	    // Since columns are case-insensitve, this property's column should end up being
	    // named "CLASSNAME1" so that the column doesn't get accidently matched to the 
	    // "ClassName" system property as well.

	    pProp = FdoDataPropertyDefinition::Create( L"classname", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(255);
	    pProp->SetNullable(true);
	    FdoPropertiesP(pWorkItem->GetProperties())->Add( pProp  );

	    FdoClassesP(pSchema->GetClasses())->Add( pWorkItem );

	    // Set up an inherited object property class.
	    FdoPtr<FdoClass> pMaintHistItem = FdoClass::Create( L"Maint History Item", L"Maintenance History Item" );
	    pMaintHistItem->SetIsAbstract(false);
	    pMaintHistItem->SetBaseClass(pWorkItem);

	    pProp = FdoDataPropertyDefinition::Create( L"Work Description", L"Description of work performed" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(4000);
	    pProp->SetNullable(false);
	    FdoPropertiesP(pMaintHistItem->GetProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"Part #", L"Part used in work item" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(true);
	    FdoPropertiesP(pMaintHistItem->GetProperties())->Add( pProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pMaintHistItem );

	    pMaintHist = FdoClass::Create( L"Maint History", L"Maintenance History" );
	    pMaintHist->SetIsAbstract(false);

	    pHistId = FdoDataPropertyDefinition::Create( L"Date", L"Date of work done" );
	    pHistId ->SetDataType( FdoDataType_DateTime );
	    pHistId ->SetNullable(false);
	    FdoPropertiesP(pMaintHist->GetProperties())->Add( pHistId );

	    pProp = FdoDataPropertyDefinition::Create( L"Maintenance History Description", L"datavalue" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(50);
	    pProp->SetNullable(true);
	    FdoPropertiesP(pMaintHist->GetProperties())->Add( pProp );

	    pObjProp = FdoObjectPropertyDefinition::Create( L"work items", L"tasks performed on this date" );
	    pObjProp->SetClass( pMaintHistItem );
	    pObjProp->SetIdentityProperty( pItemId );
	    pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	    pObjProp->SetOrderType( FdoOrderType_Descending );
	    FdoPropertiesP(pMaintHist->GetProperties())->Add( pObjProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pMaintHist );
    }

	// Update Geometry property

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));

    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pDevClass->GetProperties())->GetItem(L"Geometry");
	pGeomProp->SetDescription( L"updated geometry description" );

    if ( hasMetaSchema ) {
	    // Add the nested object property. Since added to base class, this also tests inheritance.

	    FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	    pObjProp->SetClass( pMaintHist );
	    pObjProp->SetIdentityProperty( pHistId );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );
    }

    // Add properties already defined on subclass (Transformer)

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastInspectDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastRepairDate", L"modified description" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	pProp->SetReadOnly(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));
	
    if ( CanDropCol() ) {
	    // Delete property that is also being added to base class ( property should stay but become inherited )
	    pProp = (FdoDataPropertyDefinition*) FdoPropertiesP( pClass->GetProperties() )->GetItem( L"InstallDate" );
	    pProp->Delete();
    }

	// Modify property that is also being added to base class.
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP( pClass->GetProperties() )->GetItem( L"LastRepairDate" );
	pProp->SetDescription( L"modified description" );

	// Add a new sub-class to the schema

	pClass = FdoFeatureClass::Create( L"Conductor", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

	pProp = FdoDataPropertyDefinition::Create( L"underground", L"" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	// Add a new non-inherited class that will later be removed.

	pClass = FdoFeatureClass::Create( L"Pole", L"" );
	pClass->SetIsAbstract(false);

   	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetIsAutoGenerated(true);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    if ( hasMetaSchema ) {
	    /* The following 3 classes tests adding nested object properties to an FdoClass,
	     * instead of an FdoFeatureClass.
	     */

	    // class for nested object property

	    FdoPtr<FdoClass> pStClass = FdoClass::Create( L"Street", L"" );
	    pStClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength( 30 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"Type", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength( 30 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pStClass );

	    // Class for top object property ( includes nested property )

	    FdoPtr<FdoClass> pAddrClass = FdoClass::Create( L"'Address", L"" );
	    pAddrClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"Number", L"" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pAddrClass->GetProperties())->Add( pProp );

	    pObjProp = FdoObjectPropertyDefinition::Create( L"street", L"" );
	    pObjProp->SetClass( pStClass );
	    pObjProp->SetObjectType( FdoObjectType_Value );
	    FdoPropertiesP(pAddrClass->GetProperties())->Add( pObjProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pAddrClass );

	    // Class that contains top object property

	    FdoPtr<FdoClass> pEmpClass = FdoClass::Create( L"Employee", L"" );
	    pEmpClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength( 30 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pEmpClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pEmpClass->GetIdentityProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength( 30 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pEmpClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pEmpClass->GetIdentityProperties())->Add( pProp );

	    pObjProp = FdoObjectPropertyDefinition::Create( L"home' address", L"" );
	    pObjProp->SetClass( pAddrClass );
	    pObjProp->SetObjectType( FdoObjectType_Value );
	    FdoPropertiesP(pEmpClass->GetProperties())->Add( pObjProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pEmpClass );
    }

    // Create customer plus sub-classes, used later on for id property modification
    // tests

	FdoPtr<FdoClass> pCustClass = FdoClass::Create( L"Customer", L"" );
	pCustClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pCustClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pCustClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Credit Rating", L"" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );

    FdoClassesP(pSchema->GetClasses())->Add( pCustClass );

	FdoPtr<FdoClass> pResClass = FdoClass::Create( L"Customer - Residential", L"" );
	pResClass->SetIsAbstract(false);
    pResClass->SetBaseClass(pCustClass);
	FdoClassesP(pSchema->GetClasses())->Add( pResClass );

	FdoPtr<FdoClass> pBusClass = FdoClass::Create( L"Customer - Business", L"" );
	pBusClass->SetIsAbstract(false);
    pBusClass->SetBaseClass(pCustClass);
	FdoClassesP(pSchema->GetClasses())->Add( pBusClass );
}

void FdoApplySchemaTest::ModLandSchema( FdoIConnection* connection, bool hasMetaSchema )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoPtr<FdoFeatureSchema> pSchema;

	/* 
     * Test deleting a geometric property that is the geometry property of a 
     * subclass
     */

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) {
	    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    pDescCmd->SetSchemaName( L"Land" );
	    FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	    pSchema = pSchemas->GetItem( L"Land" );
    }
    else {
        pSchema = GetDefaultSchema( connection );
    }

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Driveway"));
	pClass->SetGeometryProperty(NULL);

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Township"));
    FdoGeometricPropertyP pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->FindItem(L"LabelPoint");
    pClass->SetGeometryProperty(pGeomProp);

    pCmd->SetFeatureSchema( pSchema );
  	pCmd->Execute();
}

void FdoApplySchemaTest::ModLTSchema( FdoIConnection* connection ) 
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"LT" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"LT" );

    FdoClassDefinitionP pClass = FdoClassesP(pSchema->GetClasses())->GetItem( L"Circle Lt" );
    pClass->Delete();

    pClass = FdoClassesP(pSchema->GetClasses())->GetItem( L"DelStatus" );
    FdoPropertiesP pProps = pClass->GetProperties();

    FdoDataPropertyP pDataProp = FdoDataPropertyDefinition::Create( L"NewNumber", L"" );
    pDataProp->SetDataType( FdoDataType_Int64 );
    pDataProp->SetNullable(false);
    pProps->Add( pDataProp );

    FdoPropertyP pProp = pProps->GetItem( L"LtLock" );
    pProp->Delete();

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

    pCmd->Execute();
}

void FdoApplySchemaTest::RedefineGeometry( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );

	/* Try to redefine inherited geometry property */

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Conduit", L"Pipe Geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(7,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}

void FdoApplySchemaTest::DelPropertyError( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Land" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Build'g" );
	pClass->SetGeometryProperty(NULL);

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Geometry" );
	pGeomProp->Delete();

	FdoPtr<FdoDataPropertyDefinition> pDataProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"# Rooms" );
	pDataProp->Delete();

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Driveway" );
	pClass->SetGeometryProperty(NULL);

	pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Geometry'" );
	pGeomProp->Delete();

	pDataProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Pav'd" );
	pDataProp->Delete();

    // Set geometry property to deleted geometric property.

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Parcel" );

    pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Grading" );
	pGeomProp->Delete();
    pClass->SetGeometryProperty(pGeomProp);

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	// Try to delete an object property that has data.
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Parcel" );
	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"owner" );
	pObjProp->Delete();

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(8,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );

   	DeleteObjects( connection, L"Land", L"1-8 School" );

    // Retest with empty school table. This time we shouldn't get the
    // "school has rows" error message when Build'g.Geometry is deleted.
    // We should just get the message that it is the geometry property
    // for 1-8 Schools.

	succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(9,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );

}

void FdoApplySchemaTest::ModDelSchemas( FdoIConnection* connection, bool hasMetaSchema )
{
	/* Test some more modifications plus deletions. */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema;

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
    	pSchema = pSchemas->GetItem( L"Electric'l" );
    else
    	pSchema = pSchemas->GetItem(0);
    ModDelElectricSchema( pSchemas, hasMetaSchema );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
    	pSchema = pSchemas->GetItem( L"Acad" );

    ModDelAcadSchema( pSchemas, hasMetaSchema );
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::ModDelElectricSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema )
{
	FdoPtr<FdoFeatureSchema> pSchema;
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
	    pSchema = pSchemas->GetItem( L"Electric'l" );
    else
        pSchema = pSchemas->GetItem( 0 );

	// Dictionary elemetn delete

    if ( hasMetaSchema ) 
    	FdoSADP(pSchema->GetAttributes())->Remove( L"'Author" );

	// Class delete

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Pole"));
	pClass->Delete();

	// Geometric and Object Property delete. Also tests rippling to sub-classes of ElectricDevice.

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));
	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem(L"Geometry");
    if ( CanDropCol() ) 
        pGeomProp->Delete();
    pClass->SetGeometryProperty(NULL);

  	FdoPtr<FdoObjectPropertyDefinition> pObjProp;
    if ( hasMetaSchema ) {
    	pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"maintenance history"));
	    pObjProp->Delete();

        // Complex deletions. Delete top class plus class for nested object property while leaving
        // class for top object property
    
        FdoPtr<FdoClass> pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Employee"));
	    pClsClass->Delete();

	    pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"'Address"));

        pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClsClass->GetProperties())->GetItem(L"street"));
	    pObjProp->Delete();

	    pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Street"));
	    pClsClass->Delete();
    }

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));

    FdoPtr<FdoDataPropertyDefinition> pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Volume" );
    if ( CanDropCol() ) 
        pProp->Delete();

	pProp = FdoDataPropertyDefinition::Create( L"Temperature", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
}

void FdoApplySchemaTest::ModDelAcadSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema )
{
	FdoFeatureSchemaP pSchema;
    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = pSchemas->GetItem( L"Acad" );
    else
	    pSchema = pSchemas->GetItem( 0 );

    pSchema->SetDescription( L"A'CAD Entity Schema" );

	FdoFeatureClassP pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbEntity"));
	FdoPtr<FdoClass> pRefClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Entity"));

	// Test delete of property that has values but all values are null.

    FdoDataPropertyP pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"ColourIndex"));
    if ( CanDropCol() ) 
        pProp->Delete();

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"ColourIndex"));
    if ( CanDropCol() ) 
        pProp->Delete();

	// Test adding data property to existing class.

    pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(true);
	FdoPropertiesP(pRefClass->GetProperties())->Add( pProp );

	// Modify data property

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"Layer"));
	pProp->SetDescription( L"Entity's Classification" );

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"Layer"));
	pProp->SetDescription( L"Entity's Classification" );

    if ( hasMetaSchema ) {
	    // Modify object property
	    FdoObjectPropertyP pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"xdata"));
	    pObjProp->SetDescription( L"new xdata description" );

	    pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"xdata"));
	    pObjProp->SetDescription( L"new xdata description" );

	    // Modify class description

	    FdoPtr<FdoClass> pXData = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcXData"));
	    pXData->SetDescription( L"Application's Data" );

	    // The following verifies that the unique table name generator does not generated a table name
	    // for an existing table, or one referenced by the metaschema.

	    FdoPtr<FdoClass> pCoordVal = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbVertexCoordinateValue"));
	    FdoPtr<FdoClass> pVertex = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbVertexData"));
	    FdoPtr<FdoDataPropertyDefinition> pCoordValSeq = (FdoDataPropertyDefinition*) (FdoPropertiesP(pCoordVal->GetProperties())->GetItem(L"Seq"));

	    pObjProp = FdoObjectPropertyDefinition::Create( L"normal", L"" );
	    pObjProp->SetClass( pCoordVal );
	    pObjProp->SetIdentityProperty( pCoordValSeq );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pVertex->GetProperties())->Add( pObjProp );

	    // Add an id-less class with same spelling ( but different case ) than
	    // an existing id-less class. This class should get a different table
	    // name than the other class.

        // TODO: the SqlServer instance on seconds is case insensitive, so skip the 
        // case sensitive class name test since it will fail with duplicate index error
        // on f_classdefinition. 
#ifndef RDBI_DEF_SSQL
        FdoPtr<FdoClass> pCaseClass = FdoClass::Create( L"aCxdATA", L"Xdata" );
        pCaseClass->SetIsAbstract(false);

        pProp = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPropertiesP(pCaseClass->GetProperties())->Add( pProp  );

        FdoClassesP(pSchema->GetClasses())->Add( pCaseClass );
#endif
    }
    else {
        // When no metaschema, adding geometry to nonfeature class is allowed. Class type simply
        // changes to feature class.

        FdoPtr<FdoClass> pHatchClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbHatch"));

        FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );

        FdoPropertiesP(pHatchClass->GetProperties())->Add( pGeomProp );
    }
}

void FdoApplySchemaTest::ReAddElements( FdoIConnection* connection, bool hasMetaSchema )
{
    FdoPtr<FdoFeatureSchema> pSchema;
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    pDescCmd->SetSchemaName( L"Electric'l" );
	    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	    pSchema = pSchemas->GetItem( L"Electric'l" );
    }
    else {
        pSchema = GetDefaultSchema( connection );
    }

	// Re-add a feature class that was previously deleted. This tests that the Pole class delete actually
	// did remove the Pole table.

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Pole", L"" );
	pClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetIsAutoGenerated(true);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(30);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    if ( hasMetaSchema ) {
	    // re-add a non-feature class.

	    FdoPtr<FdoClass> pStClass = FdoClass::Create( L"Street", L"" );
	    pStClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength( 30 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"Type", L"" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pStClass );
    }

    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp;
    FdoClassDefinitionP classDef = FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );
        
    // PostgreSQL provider handles class inheritance when there is no metaschema.
    // This means that deleting ElectricDevice.Geometry also removes Geometry from Transformer,
    // meaning that it will turn into a non-feature class. Skip re-adding geometry in this 
    // case since geometry can't be added to non-feature class.
    if ( classDef->GetClassType() == FdoClassType_FeatureClass ) {
        pClass = (FdoFeatureClass*) FDO_SAFE_ADDREF(classDef.p);

        // Re-add deleted geometry

        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
        // When MetaSchema, delete of ElectricDevice.Geometry causes delete of Transformer.Geometry.
        // When no MetaSchema, inheritance not preserved so Transformer.Geometry not deleted and
        // still exists at this point.
        if ( hasMetaSchema && CanDropCol() ) {
            FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
        }
        else {
            FdoClassDefinitionP baseClass = pClass->GetBaseClass();
            if ( baseClass )
                pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(baseClass->GetProperties())->FindItem(L"Geometry");
            else
                pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->FindItem(L"Geometry");
        }
        pClass->SetGeometryProperty( pGeomProp );
    }

    if ( hasMetaSchema ) {
        // Try geometry with similar name but different case (should get different column.
        // Skip sqlserver since seconds is data case-insensitive.
#ifndef RDBI_DEF_SSQL
        pGeomProp = FdoGeometricPropertyDefinition::Create( L"GEOMETRY", L"location and shape" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
        FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
#endif
    }

	// Re-add deleted data property. Give it different type and nullibility than before. 

    if ( CanDropCol() ) {
	    pProp = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(true);
	    FdoPropertiesP(classDef->GetProperties())->Add( pProp );
    }

    if ( hasMetaSchema ) {
	    // Re-add a nested object property. Since added to base class, this also tests inheritance.

	    FdoPtr<FdoClassDefinition> pMaintHist = FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History" );
	    FdoPtr<FdoDataPropertyDefinition> pHistId = (FdoDataPropertyDefinition*) FdoPropertiesP(pMaintHist->GetProperties())->GetItem( L"Date" );
	    pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );

	    FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	    pObjProp->SetClass( pMaintHist );
	    pObjProp->SetIdentityProperty( pHistId );
	    pObjProp->SetObjectType( FdoObjectType_Collection );
	    FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );
    }

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::ModErrors( FdoIConnection* connection )
{
	/* Test some bad modifications */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );
	FdoPtr<FdoDataPropertyDefinition> pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"FeatId" );
    pProp->SetIsAutoGenerated( false );

    /* Try to change a property type */
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Pole" );
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Height" );
	FdoPropertiesP(pClass->GetProperties())->Remove(pProp);
	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Height", L"" );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pSchema->AcceptChanges();
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );

	/* Various class and property mods. Currently, only descriptions can be changed */

	FdoPtr<FdoFeatureClass> pTransClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );
	pTransClass->SetBaseClass( NULL );
	pTransClass->SetIsAbstract(true);
	
    pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"Voltage" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetPrecision( 45 );
	pProp->SetScale( 8 );
	pProp->SetNullable( true );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"Phase" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 3 );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"PartNum" );
	pProp->SetNullable(false);

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"SerialNum" );
    pProp->SetIsAutoGenerated( true );

    pGeomProp = pTransClass->GetGeometryProperty();
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
    pGeomProp->SetHasElevation(true);
    pGeomProp->SetHasMeasure(true);

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );
	pDevClass->SetIsAbstract(false);

	FdoPtr<FdoClass> pStClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Street" );
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pStClass->GetProperties())->GetItem( L"Name" );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP(pDevClass->GetProperties())->GetItem( L"graphic" );
	pObjProp->SetClass( pStClass );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	pObjProp->SetIdentityProperty( pProp );

	/* Try to add a Geometric property to a non-feature class */

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Centreline", L"Middle of the street" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
	FdoPropertiesP(pStClass->GetProperties())->Add( pGeomProp );

	/* Try to delete a class that has data */

    InsertObject( connection, false, L"Electric'l", L"Conductor", L"underground", L"0", NULL );
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );
	pClass->Delete();

	/* Try to redefine geometry inherited from base class */

	pClass = FdoFeatureClass::Create( L"Fuse", L"redefining geometry" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );
	// Redefine geometry for fuse
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Try to redefine inherited properties */

	pClass = FdoFeatureClass::Create( L"Dry Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// redefine data type

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"redefined" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine nullability

	pProp = FdoDataPropertyDefinition::Create( L"PartNum", L"redefined" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine string length

	pProp = FdoDataPropertyDefinition::Create( L"Phase", L"redefined" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(2);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine number precision

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"redefined" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(20);
	pProp->SetScale(-2);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine the SAD.

	pProp = FdoDataPropertyDefinition::Create( L"Temperature", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoSADP(pProp->GetAttributes())->Add( L"'Redefined", L"redefining SAD" );
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine object property object type.

	FdoPtr<FdoClassDefinition> pMaintHist = FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History" );
	FdoPtr<FdoDataPropertyDefinition> pHistId = (FdoDataPropertyDefinition*) (FdoPropertiesP(pMaintHist->GetProperties())->GetItem( L"Date" ));
	
	pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pClass = FdoFeatureClass::Create( L"Liquid Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// redefine numeric scale

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"redefined" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(-3);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine object property identity property

	pHistId = (FdoDataPropertyDefinition*) (FdoPropertiesP(pMaintHist->GetProperties())->GetItem( L"Maintenance History Description" ));
	
	pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

	// Create a collection object property with no identity property

	pObjProp = FdoObjectPropertyDefinition::Create( L"idless maintenance", L"" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Try to redefine a property's type */

	pClass = FdoFeatureClass::Create( L"Gas Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	pProp = FdoDataPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pProp->SetDataType( FdoDataType_String );
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	FdoPtr<FdoClass> pRepClass = FdoClass::Create( L"Part Replacement", L"redefining properties" );
	pRepClass->SetIsAbstract(false);
	pRepClass->SetBaseClass( pMaintHist );

	// Redefine object property order.

	FdoPtr<FdoClassDefinition> pWorkItem = FdoClassesP(pSchema->GetClasses())->GetItem( L"Work Item" );
	FdoPtr<FdoClassDefinition> pMaintHistItem = FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History Item" );
	FdoPtr<FdoDataPropertyDefinition> pItemId = (FdoDataPropertyDefinition*) (FdoPropertiesP(pWorkItem->GetProperties())->GetItem( L"It'm #" ));
	pObjProp = FdoObjectPropertyDefinition::Create( L"work items", L"tasks performed on this date" );
	pObjProp->SetClass( pMaintHistItem );
	pObjProp->SetIdentityProperty( pItemId );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pRepClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pRepClass );

	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
 		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(5,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}

void FdoApplySchemaTest::ModErrors2( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	/* Try to change the type of a class */

	FdoPtr<FdoClassDefinition> pClassDef = FdoClassesP(pSchema->GetClasses())->GetItem( L"'Address" );
	FdoClassesP(pSchema->GetClasses())->Remove( pClassDef );

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"'Address", L"" );
	pClass->SetIsAbstract(false);
	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pSchema->AcceptChanges();
	pClass->SetDescription( L"class type change" );

	/* Try to delete a property that has non-null values */

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );
	FdoPtr<FdoPropertyDefinition> pProp = FdoPropertiesP(pClass->GetProperties())->GetItem( L"underground" );
	pProp->Delete();

	/* Try to add a not null property to class with objects */

	FdoPtr<FdoDataPropertyDefinition> pDataProp = FdoDataPropertyDefinition::Create( L"Conductivity", L"" );
	pDataProp->SetDataType( FdoDataType_Double );
	pDataProp->SetNullable( false );
	FdoPropertiesP(pClass->GetProperties())->Add(pDataProp);

  	FdoPtr<FdoFeatureClass> pTransClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );

	/* Try to add a not null property to class without objects.
     * Should only fail for SqlServer
     */

	pDataProp = FdoDataPropertyDefinition::Create( L"Altitude", L"" );
	pDataProp->SetDataType( FdoDataType_Double );
	pDataProp->SetNullable( false );
	FdoPropertiesP(pTransClass->GetProperties())->Add(pDataProp);

	/* Try to redefine inherited properties */

    pClass = FdoFeatureClass::Create( L"Dry Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// Redefine object property class 

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"graphic", L"Acad entity" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pTransClass );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

	// Redefine geometry types

	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Try to change object property ordering */

	FdoPtr<FdoClass> pMaintClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History" );
	pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP( pMaintClass->GetProperties() )->GetItem( L"work items" );
	pObjProp->SetOrderType( FdoOrderType_Ascending );

    /* Try to modify identity properties */

    FdoClassP pCustClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Customer" );
    FdoDataPropertiesP idProps = pCustClass->GetIdentityProperties();
    idProps->RemoveAt(0);

    FdoClassP pBusClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Customer - Business" );
    pBusClass->SetDescription( L"Should not generate error message" );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(6,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}
#if 0
void FdoApplySchemaTest::GetJoinTree( FdoRdbmsSchemaManager* sm )
{
	FdoStringsP props = FdoStringCollection::Create();

	// Get an inherited and inherited system property

	props->Add( L"ClassName" );
	props->Add( L"Layer" );

	//FdoPtr<FdoRdbmsLpJoinTreeNode> rootNode = sm->GetLogicalPhysicalSchemas()->GetJoinTree( L"Land", L"Cogo Point", props );
	FdoRdbmsLpJoinTreeNode* rootNode = sm->GetLogicalPhysicalSchemas()->GetJoinTree( L"Land", L"Cogo Point", props );
	const FdoRdbmsLpTable* pLpTable = rootNode->GetTable();

	// Root table must be Cogo Point class table

#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetName(), L"COGO_POINT" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetName(), L"COGO_POINT" ) == 0 );
#endif
	CPPUNIT_ASSERT( pLpTable->GetTargetTable() == NULL );
	CPPUNIT_ASSERT( pLpTable->GetSourceColumns()->GetCount() == 0 );
	CPPUNIT_ASSERT( pLpTable->GetTargetColumns()->GetCount() == 0 );

	CPPUNIT_ASSERT( rootNode->GetSourceNodes()->GetCount() == 1 );

	// F_FEATURE is intermediate between Cogo Point and F_ClassDefinition

	FdoRdbmsLpJoinTreeNode* baseNode = rootNode->GetSourceNodes()->GetItem(0);
	pLpTable = baseNode->GetTable();

#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetName(), L"F_FEATURE" ) == 0 );
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetTargetTable()->GetName(), L"COGO_POINT" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetName(), L"F_FEATURE" ) == 0 );
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetTargetTable()->GetName(), L"COGO_POINT" ) == 0 );
#endif
	CPPUNIT_ASSERT( pLpTable->GetSourceColumns()->GetCount() == 1 );
#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetSourceColumns()->GetItem(0)->GetName(), L"FEATID" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetSourceColumns()->GetItem(0)->GetName(), L"FEATID" ) == 0 );
#endif
	CPPUNIT_ASSERT( pLpTable->GetTargetColumns()->GetCount() == 1 );
#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetTargetColumns()->GetItem(0)->GetName(), L"FEATID" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetTargetColumns()->GetItem(0)->GetName(), L"FEATID" ) == 0 );
#endif

	CPPUNIT_ASSERT( baseNode->GetSourceNodes()->GetCount() == 1 );

	// F_CLASSDEFINITION is the last table to join

	FdoRdbmsLpJoinTreeNode* metaNode = baseNode->GetSourceNodes()->GetItem(0);
	pLpTable = metaNode->GetTable();

#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetName(), L"F_CLASSDEFINITION" ) == 0 );
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetTargetTable()->GetName(), L"F_FEATURE" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetName(), L"F_CLASSDEFINITION" ) == 0 );
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetTargetTable()->GetName(), L"F_FEATURE" ) == 0 );
#endif
	CPPUNIT_ASSERT( pLpTable->GetSourceColumns()->GetCount() == 1 );
#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetSourceColumns()->GetItem(0)->GetName(), L"CLASSID" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetSourceColumns()->GetItem(0)->GetName(), L"CLASSID" ) == 0 );
#endif
	CPPUNIT_ASSERT( pLpTable->GetTargetColumns()->GetCount() == 1 );
#ifdef _WIN32
	CPPUNIT_ASSERT( _wcsicmp( pLpTable->GetTargetColumns()->GetItem(0)->GetName(), L"CLASSID" ) == 0 );
#else
	CPPUNIT_ASSERT( wcscasecmp( pLpTable->GetTargetColumns()->GetItem(0)->GetName(), L"CLASSID" ) == 0 );
#endif

	// Should be at the end at this point.

	CPPUNIT_ASSERT( metaNode->GetSourceNodes()->GetCount() == 0 );
	rootNode->Release();
}
#endif

void FdoApplySchemaTest::GetClassCapabilities( FdoIConnection* connection, FdoString* schema, FdoStringCollection* classes)
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( schema );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
    FdoFeatureSchemaP pSchema = pSchemas->GetItem( schema );

    for ( FdoInt32 i = 0; i < classes->GetCount(); ++i )
    {
        FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( classes->GetString(i) );
        VldClassCapabilities( 0, 0, pClass );
    }
}

void FdoApplySchemaTest::CheckBaseProperties( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
    FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Land" );
    FdoClassesP pClasses = pSchema->GetClasses();
    
    FdoClassP pClass = (FdoClass*) pClasses->GetItem( L"Person" );
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> pBaseProps = pClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 0 );

    FdoFeatureClassP pFeatClass = (FdoFeatureClass*) pClasses->GetItem( L"Driveway" );
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2);
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"ClassId") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"RevisionNumber") );

    // Make sure that system property doesn't reference a 
    // class from F_MetaSchema. F_MetaSchema is not exposed to FDO.

    FdoSchemaElementP systemProp = pBaseProps->GetItem( L"RevisionNumber" );
    CPPUNIT_ASSERT( systemProp->GetParent() == NULL );


    FdoFeatureClassP pParcelClass = (FdoFeatureClass*) pClasses->GetItem( L"Parcel" );
    pBaseProps = pParcelClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"ClassId") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"RevisionNumber") );

    FdoFeatureClassP pBldgClass = (FdoFeatureClass*) pClasses->GetItem( L"Build'g" );
    pBaseProps = pBldgClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );

    pFeatClass = (FdoFeatureClass*) pClasses->GetItem( L"1-8 School" );
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    pFeatClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );
    pFeatClass->SetBaseClass(pBldgClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );

    FdoFeatureClassP pNewClass = FdoFeatureClass::Create( L"NewClass", L"" );
    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"Feature_Id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pNewClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNewClass->GetIdentityProperties())->Add( pProp );
    pClasses->Add(pNewClass);

    pFeatClass->SetBaseClass(pNewClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 1 );
    pFeatClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    pFeatClass->SetBaseClass(pNewClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 1 );

    FdoDataPropertiesP(pNewClass->GetIdentityProperties())->Clear();
    pNewClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    pNewClass->SetBaseClass(pBldgClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"# Rooms"))->GetQualifiedName() == L"Land:Build'g.# Rooms" );
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"RevisionNumber"))->GetQualifiedName() == L"RevisionNumber" );
}

void FdoApplySchemaTest::CheckNonNumericForeignClass( FdoIConnection* connection )
{
    InsertObject( connection, false, L"OverridesA", L"NOFEATID", L"ID", L"1", L"DATA", L"abcd", NULL );
    InsertObject( connection, false, L"OverridesA", L"NOFEATID", L"ID", L"2", L"DATA", L"wxyz", NULL );

    FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ID") ), 
        FdoComparisonOperations_EqualTo, 
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"1") ) ); 

    FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
    updateCommand->SetFeatureClassName(L"OverridesA:NOFEATID");
    updateCommand->SetFilter( filter ); 
    FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();
    FdoPtr<FdoDataValue> dataValue = FdoDataValue::Create(L"efgh");
    FdoPtr<FdoPropertyValue> propertyValue = FdoPropertyValue::Create(L"DATA", dataValue);
    propertyValues->Add(propertyValue);
    updateCommand->Execute();

    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
    selectCommand->SetFeatureClassName( L"OverridesA:NOFEATID" );
    selectCommand->SetFilter( filter ); 

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    CPPUNIT_ASSERT( rdr->ReadNext() );
    CPPUNIT_ASSERT( wcscmp(rdr->GetString(L"DATA"), L"efgh") == 0 );
    CPPUNIT_ASSERT( !rdr->ReadNext() );

    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCommand->SetFeatureClassName(L"OverridesA:NOFEATID");
    deleteCommand->SetFilter( filter ); 
    deleteCommand->Execute();

    selectCommand->SetFilter( (FdoFilter*) NULL ); 

    rdr = selectCommand->Execute();

    CPPUNIT_ASSERT( rdr->ReadNext() );
    CPPUNIT_ASSERT( wcscmp(rdr->GetString(L"DATA"), L"wxyz") == 0 );
    CPPUNIT_ASSERT( !rdr->ReadNext() );

    deleteCommand->SetFilter( (FdoFilter*) NULL ); 
    deleteCommand->Execute( );
}

void FdoApplySchemaTest::CopySchemas(FdoFeatureSchemaCollection* pSchemas, FdoFeatureSchemaCollection* pSchemas2)
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
    StaticConnection* staticConn = NULL;

    /* Test ApplySchema with ignoreStates=true */

    printf( "Initializing Copy Connection ... \n" );
	FdoPtr<FdoIConnection> connection = UnitTestUtil::CreateConnection(
		true,
		true,
        DB_NAME_COPY_SUFFIX,
        0,
        NULL,
        0
	);

    staticConn = UnitTestUtil::NewStaticConnection();
    staticConn->connect();
    staticConn->SetSchema( DB_NAME_COPY_SUFFIX );

    // Test putting schemas through XML to make sure they round trip ok.

    pSchemas->WriteXml( stream );
    stream->Reset();
    schemas->ReadXml( stream );

    // Test1 applies into an empty datastore and then does an apply 
    // into a non-empty datastore (tests the additive merge).

	printf( "Schema Copy Test1 ... \n" );

	FdoPtr<FdoIApplySchema> pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetIgnoreStates(true);
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(schemas->GetItem(L"Acad")) );
    pCmd->Execute();
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(schemas->GetItem(L"Electric'l")) );
    pCmd->Execute();

    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas2->GetItem(L"Acad")) );
    pCmd->Execute();
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas2->GetItem(L"Electric'l")) );
    pCmd->Execute();
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas2->GetItem(L"Land")) );
    pCmd->Execute();

    FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();
    const FdoSmLpSchemaCollection* lp = mgr->RefLogicalPhysicalSchemas();
    lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test4.xml" ) );

    mgr = NULL;
    delete staticConn;

    printf( "Closing Copy Connection ... \n" );
    UnitTestUtil::CloseConnection(
		connection,
		false,
        DB_NAME_COPY_SUFFIX
	);

    printf( "Initializing Copy Connection ... \n" );
	connection = UnitTestUtil::CreateConnection(
		true,
		true,
        DB_NAME_COPY_SUFFIX,
        0,
        NULL,
        0
	);

    staticConn = UnitTestUtil::NewStaticConnection();
    staticConn->connect();
    staticConn->SetSchema( DB_NAME_COPY_SUFFIX );

    // Test2 applies into an empty datastore. The FDO Feature Schema elements have
    // a variety of states, all of which should be ignored.

	printf( "Schema Copy Test2 ... \n" );
	pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas->GetItem(L"Acad")) );

    try {
        pCmd->Execute();
    }
    catch( FdoSchemaException* ex ) {
        ex->Release();
    }

    // Set up a mixture of element states.
    ModElectricSchema( pSchemas );
    ModDelElectricSchema( pSchemas );
    ModDelAcadSchema( pSchemas );

    pCmd->SetIgnoreStates(true);
    pCmd->Execute();

    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas->GetItem(L"Electric'l")) );
    pCmd->Execute();

    mgr = staticConn->CreateSchemaManager();
    lp = mgr->RefLogicalPhysicalSchemas();
    lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test5.xml" ) );

    mgr = NULL;
    delete staticConn;

    printf( "Closing Copy Connection ... \n" );
    UnitTestUtil::CloseConnection(
		connection,
		false,
        DB_NAME_COPY_SUFFIX
	);

}

void FdoApplySchemaTest::CreateOverrideSchema( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides, bool nnull, bool addConstraints, bool hasMetaSchema )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoFeatureSchemaP                   pSchema;
    FdoFeatureClassP                    pFeatClass;
    FdoFeatureClassP                    pBaseClass;
    FdoDataPropertyP                    pProp;
    FdoGeometricPropertyP               pGeomProp;

    FdoInt32                            idx;
    FdoInt32                            classCount = hasMetaSchema ? 10 : 5;

    if ( hasMetaSchema || CanCreateSchemaWithoutMetaSchema() ) 
        pSchema = FdoFeatureSchema::Create( L"OverridesA", L"AutoCAD schema" );
    else
        pSchema = GetDefaultSchema( connection );

    for ( idx = 0; idx < classCount; idx++ ) {
	    pFeatClass = FdoFeatureClass::Create( 
            FdoStringP::Format( L"OvClass%c", 'A' + idx ), 
            L"a class" 
        );
	    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
	    pFeatClass->SetIsAbstract(false);

        if ( idx > 0 ) {
            pFeatClass->SetBaseClass(pBaseClass);
        }
        else {
    	    pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            pProp->SetIsAutoGenerated(true);
	        FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );
	        FdoDataPropertiesP(pFeatClass->GetIdentityProperties())->Add( pProp );
        }

	    pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format( L"Data%c", 'A' + idx ),
            L"a data property" 
        );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(10);
        pProp->SetNullable( ((idx >= 7) && nnull) ? false : true);
	    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

        if ( idx == 7 ) {
            CreateOverrideValueProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                1,
                2 
            ); 
            CreateOverrideValueProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                2,
                1 
            ); 
            CreateOverrideCollectionProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                3,
                1 
            ); 
            CreateOverrideCollectionProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ), 
                4,
                2 
            ); 
        }

        if ( idx == 8 ) {
            CreateOverrideValueProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                5,
                0 
            ); 
            CreateOverrideCollectionProperty( 
                (FdoFeatureClass*) pFeatClass, 
                FdoStringP::Format( L"%c", 'A' + idx ), 
                6,
                0 
            ); 
        }

        // Create a main geometry
        if ( idx == 0 ) {
            pGeomProp = FdoGeometricPropertyDefinition::Create(
                FdoStringP::Format( L"Geom%c", 'A' + idx ),
                L"a geometry property" 
            );
	        FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp );
        }

        // Create a couple of secondary geometries.
        if ( idx == 5 ) {
            FdoGeometricPropertyP pGeomProp2 = FdoGeometricPropertyDefinition::Create(
                FdoStringP::Format( L"Geom%c", 'A' + idx ),
                L"a secondary geometric property" 
            );
	        FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp2 );
        }

        if ( idx == 8 ) {
            FdoGeometricPropertyP pGeomProp3 = FdoGeometricPropertyDefinition::Create(
                FdoStringP::Format( L"Geom%c", 'A' + idx ),
                L"another secondary geometric property" 
            );
	        FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp3 );
        }

        if ( addConstraints && (idx == 9) ) {
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> pBaseProps = pBaseClass->GetBaseProperties();

            FdoPtr<FdoUniqueConstraint> constr = FdoUniqueConstraint::Create();
            FdoDataPropertiesP(constr->GetProperties())->Add( pProp );
            FdoPropertyP pBaseProp = pBaseProps->GetItem( L"DataC" );
            FdoDataPropertiesP(constr->GetProperties())->Add( (FdoDataPropertyDefinition*)(pBaseProp.p) );
            FdoPtr<FdoUniqueConstraintCollection>(pFeatClass->GetUniqueConstraints())->Add( constr );

            constr = FdoUniqueConstraint::Create();
            pBaseProp = pBaseProps->GetItem( L"DataH" );
            FdoDataPropertiesP(constr->GetProperties())->Add( (FdoDataPropertyDefinition*)(pBaseProp.p) );
            FdoPtr<FdoUniqueConstraintCollection>(pFeatClass->GetUniqueConstraints())->Add( constr );
        }

        pFeatClass->SetGeometryProperty( pGeomProp );

        pBaseClass = pFeatClass;
    }

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}

void FdoApplySchemaTest::CreateForeignBasedSchema( FdoIConnection* connection, FdoFeatureSchema* pBaseSchema, FdoRdbmsOvPhysicalSchemaMapping* pOverrides )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoFeatureSchemaP                   pSchema = FdoFeatureSchema::Create( L"ForeignBased", L"Non-Foreign schema based on Foreign Schema" );
    FdoFeatureClassP                    pClass;
    FdoFeatureClassP                    pBaseClass;
    FdoClassP                           pOpClass1;
    FdoClassP                           pOpClass2;
    FdoObjectPropertyP                  pObProp;
    FdoDataPropertyP                    pOpIdProp;

    pBaseClass = (FdoFeatureClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"OvClassC11") );
    pOpClass1 = (FdoClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"OvOpClassHA") );
    pOpClass2 = (FdoClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"OvOpClassHD") );
    pOpIdProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pOpClass2->GetProperties())->GetItem( L"OpId1" );

    pClass = FdoFeatureClass::Create( L"OvClassC111", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pClass->SetBaseClass( pBaseClass );

    pObProp = FdoObjectPropertyDefinition::Create( L"Object B", L"" );
    pObProp->SetClass( pOpClass1 );
    pObProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pObProp = FdoObjectPropertyDefinition::Create( L"Object D", L"" );
    pObProp->SetClass( pOpClass2 );
    pObProp->SetObjectType( FdoObjectType_Collection );
    pObProp->SetIdentityProperty( pOpIdProp );
	FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pBaseClass = (FdoFeatureClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"OvClassH") );
    pClass = FdoFeatureClass::Create( L"OvClassH1", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pClass->SetBaseClass( pBaseClass );

    pBaseClass = (FdoFeatureClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"Storage") );
    pClass = FdoFeatureClass::Create( L"StorageB", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pClass->SetBaseClass( pBaseClass );

    pOpClass1 = (FdoClass*) ( FdoClassesP(pBaseSchema->GetClasses())->GetItem(L"Floor") );
    pObProp = FdoObjectPropertyDefinition::Create( L"FloorB", L"" );
    pObProp->SetClass( pOpClass1 );
    pObProp->SetObjectType( FdoObjectType_Value );
    FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pObProp = FdoObjectPropertyDefinition::Create( L"FloorC", L"" );
    pObProp->SetClass( pOpClass1 );
    pObProp->SetObjectType( FdoObjectType_Value );
    FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pClass = FdoFeatureClass::Create( L"OneForeign", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    FdoPropertiesP pProps = pClass->GetProperties();
    FdoDataPropertiesP pIdProps = pClass->GetIdentityProperties();

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"ID", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    pProps->Add( pProp );
    pIdProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    pProps->Add( pProp );

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}

void FdoApplySchemaTest::ModOverrideSchema1( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"OverridesA" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	FdoFeatureSchemaP  pSchema = pSchemas->GetItem( L"OverridesA" );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    FdoFeatureClassP                    pFeatClass;
    FdoFeatureClassP                    pBaseClass;
    FdoClassP                           pOpClass;
    FdoDataPropertyP                    pProp;
    FdoObjectPropertyP                  pObProp;

    FdoInt32                            idx;

    for ( idx = 2; idx < 6; idx += 2 ) {
	    pBaseClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( 
            FdoStringP::Format( L"OvClass%c", 'A' + idx ) 
        ));

	    pFeatClass = FdoFeatureClass::Create( 
            FdoStringP::Format( L"OvClass%c1", 'A' + idx ), 
            L"a class" 
        );
	    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
	    pFeatClass->SetIsAbstract(false);

        pFeatClass->SetBaseClass(pBaseClass);

        pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format( L"Data%c1", 'A' + idx ),
            L"a data property" 
        );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(10);
	    pProp->SetNullable(true);
	    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

        pBaseClass = pFeatClass;

	    pFeatClass = FdoFeatureClass::Create( 
            FdoStringP::Format( L"OvClass%c11", 'A' + idx ), 
            L"a class" 
        );
	    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
	    pFeatClass->SetIsAbstract(false);

        pFeatClass->SetBaseClass(pBaseClass);

	    pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format( L"Data%c11", 'A' + idx ),
            L"a data property" 
        );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(10);
	    pProp->SetNullable(true);
	    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    }

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassH" ));

    pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"DataH" ));
    if ( CanDropCol() )
        pProp->Delete();

    pProp = FdoDataPropertyDefinition::Create( L"DataNew", L"a data property" );
    pProp->SetDataType( FdoDataType_Double );
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pObProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"ObjectA" ));
    if ( CanDropCol() )
        pObProp->Delete();

    pObProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"ObjectC" ));
    pObProp->Delete();
    
    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassE11" ));

    pOpClass = (FdoClass*) ( FdoClassesP(pSchema->GetClasses())->GetItem(L"OvOpClassHA") );

    pObProp = FdoObjectPropertyDefinition::Create( L"Object A", L"" );
    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pFeatClass->GetProperties())->Add( pObProp );

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassJ" ));
    pFeatClass->Delete();

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}

void FdoApplySchemaTest::ModOverrideSchema2( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"OverridesA" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	FdoFeatureSchemaP  pSchema = pSchemas->GetItem( L"OverridesA" );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    FdoFeatureClassP                    pFeatClass;
    FdoFeatureClassP                    pBaseClass;
    FdoClassP                           pOpClass;
    FdoDataPropertyP                    pProp;
    FdoObjectPropertyP                  pObProp;

    pOpClass = (FdoClass*) ( FdoClassesP(pSchema->GetClasses())->GetItem(L"OvOpClassHA") );

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassH" ));

    if ( CanDropCol() ) {
        pProp = FdoDataPropertyDefinition::Create( L"DataH", L"a data property" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(10);
	    pProp->SetNullable(true);
        FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

        pObProp = FdoObjectPropertyDefinition::Create( L"ObjectA", L"" );
        pObProp->SetClass( pOpClass );
        pObProp->SetObjectType( FdoObjectType_Value );
	    FdoPropertiesP(pFeatClass->GetProperties())->Add( pObProp );
    }

    pBaseClass = pFeatClass;

    pFeatClass = FdoFeatureClass::Create( L"OvClassJ", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
    pFeatClass->SetIsAbstract(false);
    pFeatClass->SetBaseClass(pBaseClass);

    pProp = FdoDataPropertyDefinition::Create( L"DataJ", L"a data property" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(10);
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassC11" ));

    pObProp = FdoObjectPropertyDefinition::Create( L"ObjectA", L"" );
    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pFeatClass->GetProperties())->Add( pObProp );

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassE11" ));

    if ( CanDropCol() ) {
        pObProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"Object A" ));
        pObProp->Delete();

        pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassI" ));
        pFeatClass->Delete();
    }

    // OvClassK tests setting a (main) geometry property to being in the class
    // table, but with same column name as the property name.
    pFeatClass = FdoFeatureClass::Create( L"OvClassK", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
    pFeatClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"FID", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pFeatClass->GetIdentityProperties())->Add( pProp );

    FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"GeomK", L"" );
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp );
    pFeatClass->SetGeometryProperty(pGeomProp);

    FdoClassP pViewOpClass = FdoClass::Create( L"view_op", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pViewOpClass );
    pFeatClass->SetIsAbstract(false);

    FdoDataPropertyP pSeqProp = FdoDataPropertyDefinition::Create( L"seq", L"" );
    pSeqProp->SetDataType( FdoDataType_Int32 );
    pSeqProp->SetNullable(false);
    FdoPropertiesP(pViewOpClass->GetProperties())->Add( pSeqProp );

    pProp = FdoDataPropertyDefinition::Create( L"col1", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    FdoPropertiesP(pViewOpClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col2", L"" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(true);
    FdoPropertiesP(pViewOpClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col3", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pViewOpClass->GetProperties())->Add( pProp );

    pFeatClass = FdoFeatureClass::Create( L"viewbase", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
    pFeatClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pFeatClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col1", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Column2", L"" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp );
    pFeatClass->SetGeometryProperty(pGeomProp);

    pFeatClass = FdoFeatureClass::Create( L"view1", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pFeatClass );
    pFeatClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pFeatClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Column1", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col2", L"" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col3", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pGeomProp = FdoGeometricPropertyDefinition::Create( L"geometry", L"" );
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pGeomProp );
    pFeatClass->SetGeometryProperty(pGeomProp);

    pObProp = FdoObjectPropertyDefinition::Create( L"view_op", L"" );
    pObProp->SetClass( pViewOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pObProp );

    FdoClassP pClass = FdoClass::Create( L"view2", L"a class" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pFeatClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col1", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col2", L"" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"col3", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    pObProp = FdoObjectPropertyDefinition::Create( L"view_op", L"" );
    pObProp->SetClass( pViewOpClass );
    pObProp->SetObjectType( FdoObjectType_Collection );
    pObProp->SetIdentityProperty( pSeqProp );
    FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pObProp = FdoObjectPropertyDefinition::Create( L"view_op_table", L"" );
    pObProp->SetClass( pViewOpClass );
    pObProp->SetObjectType( FdoObjectType_Collection );
    pObProp->SetIdentityProperty( pSeqProp );
    FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}

void FdoApplySchemaTest::ModOverrideSchemaForeign( FdoFeatureSchema* pSchema, FdoRdbmsOvPhysicalSchemaMapping* pOverrides )
{
    FdoClassP pOpClass = FdoClass::Create( L"Floor", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pOpClass );
    FdoPropertiesP pProps = pOpClass->GetProperties();

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"ID", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    pProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Storage", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    pProps->Add( pProp );

    FdoFeatureClassP pClass = FdoFeatureClass::Create( L"Storage", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pProps = pClass->GetProperties();
    FdoDataPropertiesP pIdProps = pClass->GetIdentityProperties();

    pProp = FdoDataPropertyDefinition::Create( L"ID", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    pProps->Add( pProp );
    pIdProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Storage", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    pProps->Add( pProp );

    // this "if" can be removed when defect 772351 is fixed
    FdoGeometricPropertyP pGeomProp;
#ifndef RDBI_DEF_SSQL
    pGeomProp = FdoGeometricPropertyDefinition::Create( L"Floor", L"location and shape" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetHasElevation(true);
    pProps->Add( pGeomProp );
#endif
    FdoObjectPropertyP pObProp = FdoObjectPropertyDefinition::Create( L"FloorA", L"location and shape" );
    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
    pProps->Add( pObProp );

    pClass = FdoFeatureClass::Create( L"NOFEATID", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );
    pProps = pClass->GetProperties();
    pIdProps = pClass->GetIdentityProperties();

    pProp = FdoDataPropertyDefinition::Create( L"ID", L"id" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(false);
    pProp->SetLength(20);
    pProps->Add( pProp );
    pIdProps->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"DATA", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetNullable(true);
    pProp->SetLength(50);
    pProps->Add( pProp );

    pGeomProp = FdoGeometricPropertyDefinition::Create( L"GEOMETRY", L"location and shape" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetHasElevation(true);
    pProps->Add( pGeomProp );
    pClass->SetGeometryProperty( pGeomProp );
}

void FdoApplySchemaTest::ModOverrideSchemaForeign2( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"OverridesA" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	FdoFeatureSchemaP  pSchema = pSchemas->GetItem( L"OverridesA" );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    FdoFeatureClassP                    pFeatClass;
    FdoFeatureClassP                    pBaseClass;
    FdoClassP                           pOpClass;
    FdoDataPropertyP                    pProp;
    FdoObjectPropertyP                  pObProp;

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"Storage" ));

    pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem(L"Storage"));
    if ( CanDropCol() ) 
        pProp->Delete();
        
    pProp = FdoDataPropertyDefinition::Create( L"Extra", L"a new data property" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(!CanAddNotNullCol());
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}

void FdoApplySchemaTest::CreateOverrideValueProperty( 
    FdoClassDefinition* pClass,
    FdoStringP suffix,
    FdoInt32 position,
    FdoInt32 depth
)
{
	FdoClassP pOpClass = FdoClass::Create( 
        FdoStringP::Format( L"OvOpClass%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
        L"an object property class" 
    );
	pOpClass->SetIsAbstract(false);

	FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"OpA", L"first data property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"OpB", L"first data property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pProp );

    FdoFeatureSchemaP containingSchema = pClass->GetFeatureSchema();
	FdoClassesP(containingSchema->GetClasses())->Add( pOpClass );

    FdoObjectPropertyP pObProp = FdoObjectPropertyDefinition::Create( 
        FdoStringP::Format( L"Object%c", 'A' + position - 1 ),
        L"" 
    );

    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    if ( depth > 0 ) {
        if ( position % 2 ) {
            CreateOverrideValueProperty( 
                (FdoClass*) pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
        else {
            CreateOverrideCollectionProperty( 
                (FdoClass*) pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1),
                1,
                depth - 1 
            ); 
        }
    }
}

void FdoApplySchemaTest::CreateOverrideCollectionProperty( 
    FdoClassDefinition* pClass,
    FdoStringP suffix,
    FdoInt32 position,
    FdoInt32 depth
)
{
	FdoClassP pOpClass = FdoClass::Create( 
        FdoStringP::Format( L"OvOpClass%ls%c", (FdoString*) suffix, 'A' + position -1 ),
        L"an object property class" 
    );
	pOpClass->SetIsAbstract(false);

	FdoDataPropertyP pIdProp = FdoDataPropertyDefinition::Create( L"OpId1", L"object property id property" );
	pIdProp->SetDataType( FdoDataType_Int32 );
	pIdProp->SetNullable(false);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pIdProp );
    if ( position % 2 )
        FdoDataPropertiesP (pOpClass->GetIdentityProperties())->Add(pIdProp);

	FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"OpA", L"first data property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
    if ( position > 4 ) 
    	pProp->SetNullable(false);
    else
    	pProp->SetNullable(true);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"OpB", L"first data property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pProp );

    FdoFeatureSchemaP containingSchema = pClass->GetFeatureSchema();
	FdoClassesP(containingSchema->GetClasses())->Add( pOpClass );

    FdoObjectPropertyP pObProp = FdoObjectPropertyDefinition::Create( 
        FdoStringP::Format( L"Object%c", 'A' + position - 1 ),
        L"" 
    );

    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Collection );
    if ( !(position % 2) )
        pObProp->SetIdentityProperty( pIdProp );

	FdoPropertiesP(pClass->GetProperties())->Add( pObProp );

    if ( depth > 0 ) {
        if ( position % 2 ) {
            CreateOverrideValueProperty( 
                (FdoClass*) pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
        else {
            CreateOverrideCollectionProperty( 
                (FdoClass*) pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
    }
}

FdoRdbmsOvPhysicalSchemaMapping* FdoApplySchemaTest::CreateOverrides( FdoIConnection* connection, int passNum )
{ 
    FdoRdbmsOvPhysicalSchemaMapping* pOverrides = (FdoRdbmsOvPhysicalSchemaMapping*)connection->CreateSchemaMapping();

    FdoRdbmsOvClassP                    pClass;
    FdoInt32                            idx;

    for ( idx = 0; idx < 10; idx++ ) {

        if ( idx != 6 ) {
            pClass = CreateOvClassDefinition( 
                FdoStringP::Format( L"OvClass%c", 'A' + idx )
            );
            ClassesOvAdd(pOverrides, pClass);
        }
        else {
            pClass = NULL;
        }

        if ( (idx == 1) || (idx == 2) || (idx == 7) ) 
            pClass->SetTableMapping( FdoSmOvTableMappingType_ConcreteTable );

        if ( (idx == 0) || (idx == 3) || (idx == 4) || (idx == 5) || (idx == 8) ) 
            pClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );

        if ( (idx == 2) || (idx == 4) || (idx == 9) ) { 
            FdoRdbmsOvTableP pTable = CreateOvTable( 
                FdoStringP::Format( L"ov_table_%c", 'a' + idx )
            );
            ClassOvSetTable(pClass, pTable);

            if ( (idx == 2) || (idx == 9) ) 
                pTable->SetPKeyName( FdoStringP::Format( L"ov_primkey_%c", 'a' + idx ) );

            if ( idx == 9 )
                TableOvSetTablespace(pTable, L"SYSTEM" );
		}

        if ( pClass && (idx % 2) ) {
            FdoRdbmsOvDataPropertyP pProp = CreateOvDataPropertyDefinition( 
                FdoStringP::Format( L"Data%c", 'A' + idx )
            );
            PropertiesOvAdd(pClass, pProp);
        
            FdoRdbmsOvColumnP pCol = CreateOvColumn( 
                FdoStringP::Format( L"ov_col_%c", 'A' + idx )
            );

            if ( idx == 1 || idx == 3 ) 
                ColumnOvSetSequence( pCol, L"ov_seq_1" );

            if ( idx == 7  ) 
                ColumnOvSetSequence( pCol, L"ov_seq_2" );

            DataPropOvSetColumn(pProp, pCol);
        }

        if ( idx == 7 ) {
            FdoRdbmsOvDataPropertyP pProp = CreateOvDataPropertyDefinition( 
                L"DataNew"
            );
            PropertiesOvAdd(pClass, pProp);
        
            FdoRdbmsOvColumnP pCol = CreateOvColumn( 
                L"ov_data_new"
            );
            DataPropOvSetColumn(pProp, pCol);

            CreateOverrideValueProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                1,
                2 
            ); 
            CreateOverrideValueProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                2,
                1 
            ); 
            CreateOverrideCollectionProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                3,
                1 
            ); 
            CreateOverrideCollectionProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ), 
                4,
                2 
            ); 

        }

        if ( idx == 8 ) {
            CreateOverrideValueProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                5,
                0 
            ); 
            CreateOverrideCollectionProperty( 
                pOverrides,
                pClass, 
                FdoStringP::Format( L"%c", 'A' + idx ),
                6,
                0 
            ); 
        }

        // Test overriding a main geometry column.
        if ( idx == 0 ) {
            FdoRdbmsOvGeometricPropertyP pGeomProp = CreateOvGeometricPropertyDefinition(
                FdoStringP::Format( L"Geom%c", 'A' + idx )
            );
            PropertiesOvAdd(pClass, pGeomProp);

            FdoRdbmsOvGeometricColumnP pGeomCol = CreateOvGeometricColumn( 
                FdoStringP::Format( L"ov_geomcol_%c", 'A' + idx )
            );
            GeometricPropOvSetColumn(pGeomProp, pGeomCol);
        }

        // Test overriding a secondary geometry column
        if ( idx == 5 ) {
            FdoRdbmsOvGeometricPropertyP pGeomProp2 = CreateOvGeometricPropertyDefinition(
                FdoStringP::Format( L"Geom%c", 'A' + idx )
            );
            PropertiesOvAdd(pClass, pGeomProp2);

            FdoRdbmsOvGeometricColumnP pGeomCol2 = CreateOvGeometricColumn( 
                FdoStringP::Format( L"ov_geomcol_%c", 'A' + idx )
            );
            GeometricPropOvSetColumn(pGeomProp2, pGeomCol2);
        }

        // Test setting the geometric property column to blank. This causes the 
        // column name to always be the same as the property name (name adjustment
        // is disallowed).
        if ( idx == 8 ) {
            FdoRdbmsOvGeometricPropertyP pGeomProp3 = CreateOvGeometricPropertyDefinition(
                FdoStringP::Format( L"Geom%c", 'A' + idx )
            );
            PropertiesOvAdd(pClass, pGeomProp3);

            FdoRdbmsOvGeometricColumnP pGeomCol3 = CreateOvGeometricColumn( 
                L""
            );
            GeometricPropOvSetColumn(pGeomProp3, pGeomCol3);
        }
/* 
        // Test indexes and unique keys
        if ( createIndexes && (idx == 7) ) {
            if ( passNum == 1 || passNum == 2 ) {
                FdoRdbmsOvIndexP index = FdoRdbmsOvIndexDefinition::Create( L"custom_index_1" );

                FdoStringsP propNames = index->GetProperties();
                propNames->Add( L"DataB" );
                propNames->Add( L"DataA" );

                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( index );
            }

            if ( passNum == 1 || passNum == 2 ) {
                FdoRdbmsOvIndexP index = FdoRdbmsOvIndexDefinition::Create( L"custom_index_2" );

                if ( passNum == 1 ) {
                    FdoStringsP propNames = index->GetProperties();
                    propNames->Add( L"DataH" );
                    propNames->Add( L"DataC" );
                    propNames->Add( L"DataE" );
                }

                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( index );
            }

            if ( passNum == 1 || passNum == 3 ) {
// Unique keys are for the future. OWM doesn't currently handle them.
//                FdoRdbmsOvUniqueKeyP key = FdoRdbmsOvUniqueKeyDefinition::Create( L"alt_key_1" );
                FdoRdbmsOvIndexP key = FdoRdbmsOvIndexDefinition::Create( L"alt_key_1" );

                if ( passNum == 1 ) {
                    FdoStringsP propNames = key->GetProperties();
                    propNames->Add( L"DataD" );
                }

//                FdoRdbmsOvUniqueKeysP(pClass->GetUniqueKeys())->Add( key );
                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( key );
            }

            if ( passNum == 2 ) {
//                FdoRdbmsOvUniqueKeyP key = FdoRdbmsOvUniqueKeyDefinition::Create( L"alt_key_2" );
                FdoRdbmsOvIndexP key = FdoRdbmsOvIndexDefinition::Create( L"alt_key_2" );

                FdoStringsP propNames = key->GetProperties();
                propNames->Add( L"DataG" );

//                FdoRdbmsOvUniqueKeysP(pClass->GetUniqueKeys())->Add( key );
                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( key );
            }
        }

        if ( createIndexes && (idx == 5) ) {
            if ( passNum == 2 ) {
                FdoRdbmsOvIndexP index = FdoRdbmsOvIndexDefinition::Create( L"custom_index_3" );

                FdoStringsP propNames = index->GetProperties();
                propNames->Add( L"DataF" );

                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( index );
            }
        }
        if ( createIndexes && (idx == 9) ) {
            if ( passNum == 1 || passNum == 3 ) {
                FdoRdbmsOvIndexP index = FdoRdbmsOvIndexDefinition::Create( L"custom_index_J" );

                FdoStringsP propNames = index->GetProperties();
                propNames->Add( L"DataJ" );

                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( index );
            }

            if ( passNum == 1 ) {
//                FdoRdbmsOvUniqueKeyP key = FdoRdbmsOvUniqueKeyDefinition::Create( L"alt_key_J" );
                FdoRdbmsOvIndexP key = FdoRdbmsOvIndexDefinition::Create( L"alt_key_J" );

                FdoStringsP propNames = key->GetProperties();
                propNames->Add( L"DataI" );

//                FdoRdbmsOvUniqueKeysP(pClass->GetUniqueKeys())->Add( key );
                FdoRdbmsOvIndexesP(pClass->GetIndexes())->Add( key );
            }
        }
*/
    }

    pClass = CreateOvClassDefinition( L"OvClassC1" );
    ClassesOvAdd(pOverrides, pClass);
    FdoRdbmsOvTableP pTable = CreateOvTable( L"ov_table_c1" );
    ClassOvSetTable(pClass, pTable);

    pClass = CreateOvClassDefinition( L"OvClassC11" );
    ClassesOvAdd(pOverrides, pClass);
    pClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );
    pTable = CreateOvTable( L"ov_table_c2" );

    ClassOvSetTable(pClass, pTable);

    FdoRdbmsOvObjectPropertyP pObProp = CreateOvObjectPropertyDefinition( L"ObjectA" );
    PropertiesOvAdd(pClass, pObProp);
    FdoRdbmsOvPropertyMappingSingleP mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    mapping->SetPrefix( L"TestVeryLongPrefix012345" );

    pClass = CreateOvClassDefinition( L"OvClassE1" );
    ClassesOvAdd(pOverrides, pClass);
    pClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );

    FdoRdbmsOvDataPropertyP pProp = CreateOvDataPropertyDefinition( 
        L"DataE1"
    );
    PropertiesOvAdd(pClass, pProp);
        
    FdoRdbmsOvColumnP pCol = CreateOvColumn( 
        L"ov_col_e1"
    );
    DataPropOvSetColumn(pProp, pCol);

    pClass = CreateOvClassDefinition( L"OvClassE11" );
    ClassesOvAdd(pOverrides, pClass);

    pProp = CreateOvDataPropertyDefinition( 
        L"DataE11"
    );
    PropertiesOvAdd(pClass, pProp);
        
    pCol = CreateOvColumn( 
        L"ov_col_e11"
    );
    DataPropOvSetColumn(pProp, pCol);

    pObProp = CreateOvObjectPropertyDefinition( 
        L"Object A"
    );
    mapping = CreateOvPropertyMappingSingle();
    mapping->SetPrefix( L"objecta" );
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);

    PropertiesOvAdd(pClass, pObProp);

    // Test putting main geometry in class table.
    pClass = CreateOvClassDefinition( L"OvClassK" );
    ClassesOvAdd(pOverrides, pClass);

    pProp = CreateOvDataPropertyDefinition( 
        L"FID"
    );
    PropertiesOvAdd(pClass, pProp);
        
    pCol = CreateOvColumn( 
        L"featid"
    );
    DataPropOvSetColumn(pProp, pCol);

	FdoRdbmsOvGeometricPropertyP pGeomProp4 = CreateOvGeometricPropertyDefinition(
        L"GeomK"
    );
    PropertiesOvAdd(pClass, pGeomProp4);

    // blank column name causes column name to default to property name.
    FdoRdbmsOvGeometricColumnP pGeomCol4 = CreateOvGeometricColumn( 
        L""
    );
    GeometricPropOvSetColumn(pGeomProp4, pGeomCol4);

    pClass = CreateOvClassDefinition( L"viewbase" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"viewbase" );
    ClassOvSetTable(pClass, pTable);
    pProp = CreateOvDataPropertyDefinition( 
        L"id"
    );
    pCol = CreateOvColumn( 
        L"id"
    );
    DataPropOvSetColumn(pProp, pCol);
    PropertiesOvAdd(pClass, pProp);
    pProp = CreateOvDataPropertyDefinition( 
        L"col1"
    );
    pCol = CreateOvColumn( 
        L"col1"
    );
    DataPropOvSetColumn(pProp, pCol);
    PropertiesOvAdd(pClass, pProp);
    pProp = CreateOvDataPropertyDefinition( 
        L"Column2"
    );
    pCol = CreateOvColumn( 
        L"col2"
    );
    DataPropOvSetColumn(pProp, pCol);
    PropertiesOvAdd(pClass, pProp);
    FdoRdbmsOvGeometricPropertyP pGeomProp = CreateOvGeometricPropertyDefinition(
        L"Geometry"
    );
    PropertiesOvAdd(pClass, pGeomProp);
    FdoRdbmsOvGeometricColumnP pGeomCol = CreateOvGeometricColumn( 
        L"geometry"
    );
    GeometricPropOvSetColumn(pGeomProp, pGeomCol);
    pClass = CreateOvClassDefinition( L"view1" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"view1" );
    ClassOvSetTable(pClass, pTable);
    pProp = CreateOvDataPropertyDefinition( 
        L"Column1"
    );
    pCol = CreateOvColumn( 
        L"col1"
    );
    DataPropOvSetColumn(pProp, pCol);
    PropertiesOvAdd(pClass, pProp);
    pObProp = CreateOvObjectPropertyDefinition( 
        L"view_op"
    );
    mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    PropertiesOvAdd(pClass, pObProp);

    pClass = CreateOvClassDefinition( L"view2" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"view2" );
    ClassOvSetTable(pClass, pTable);
    pProp = CreateOvDataPropertyDefinition( 
        L"col3"
    );
    pCol = CreateOvColumn( 
        L"column3"
    );
    DataPropOvSetColumn(pProp, pCol);
    PropertiesOvAdd(pClass, pProp);
    pObProp = CreateOvObjectPropertyDefinition( 
        L"view_op"
    );
    FdoPtr<FdoRdbmsOvPropertyMappingConcrete> cMapping = CreateOvPropertyMappingConcrete();
    FdoRdbmsOvClassP obClass = CreateOvClassDefinition( L"view_op" );
    pTable = CreateOvTable( L"view_op" );
    ClassOvSetTable(obClass, pTable);
    PropertyMappingOvSetInternalClass( cMapping, obClass );
    ObjectPropertyOvSetMappingDefinition(pObProp, cMapping);
    PropertiesOvAdd(pClass, pObProp);

    return pOverrides;
}

FdoRdbmsOvPhysicalSchemaMapping* FdoApplySchemaTest::CreateOverrideDefaults( FdoIConnection* connection, int passNum )
{ 
    FdoRdbmsOvPhysicalSchemaMapping* pOverrides = 
        CreateOverrides( connection, passNum );

    pOverrides->SetTableMapping( FdoSmOvTableMappingType_BaseTable );

    return pOverrides;
}

FdoRdbmsOvPhysicalSchemaMapping* FdoApplySchemaTest::CreateForeignBasedOverrides( FdoIConnection* connection )
{ 
    FdoRdbmsOvPhysicalSchemaMapping* pOverrides = 
        (FdoRdbmsOvPhysicalSchemaMapping*) connection->CreateSchemaMapping();

    FdoRdbmsOvClassP pClass = CreateOvClassDefinition( L"StorageB" );
    ClassesOvAdd(pOverrides, pClass);

    FdoRdbmsOvObjectPropertyP pObProp = 
        CreateOvObjectPropertyDefinition( L"FloorC" );
    PropertiesOvAdd(pClass, pObProp);

    FdoRdbmsOvPropertyMappingSingleP mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    mapping->SetPrefix( L"FLRC" );

    pClass = CreateOvClassDefinition( L"OneForeign" );
    ClassOvSetTable( pClass, FdoRdbmsOvTableP(CreateOvTable(L"oneforeign")) );
    ClassesOvAdd(pOverrides, pClass);

    return pOverrides;
}

void FdoApplySchemaTest::CreateOverrideValueProperty( 
    FdoRdbmsOvPhysicalSchemaMapping* pOverrides,
    FdoRdbmsOvClassDefinition* pClass,
    FdoStringP suffix,
    FdoInt32 position,
    FdoInt32 depth
)
{

    FdoRdbmsOvObjectPropertyP pObProp = CreateOvObjectPropertyDefinition( 
            FdoStringP::Format( L"Object%c", 'A' + position - 1 )
    );
    PropertiesOvAdd(pClass, pObProp);

    FdoRdbmsOvPropertyMappingSingleP mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    mapping->SetPrefix( 
        FdoStringP::Format( L"prefix%c", 'A' + position - 1 )
    );

   	FdoRdbmsOvClassP pOpClass = CreateOvClassDefinition( 
        FdoStringP::Format( L"OvOpClass%ls%c", (FdoString*) suffix, 'A' + position - 1 )
    );
    ClassesOvAdd(pOverrides, pOpClass);

    FdoRdbmsOvTableP pTable = CreateOvTable( 
        FdoStringP::Format( L"ops_table_%ls%c", (FdoString*) suffix.Lower(), 'a' + position - 1 )
    );
    ClassOvSetTable(pOpClass, pTable );

    if ( depth > 0 ) {
        if ( position % 2 ) {
    	
            CreateOverrideValueProperty( 
                pOverrides,
                pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
        else {
            CreateOverrideCollectionProperty( 
                pOverrides,
                pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
    }
}

void FdoApplySchemaTest::CreateOverrideCollectionProperty( 
    FdoRdbmsOvPhysicalSchemaMapping* pOverrides,
    FdoRdbmsOvClassDefinition* pClass,
    FdoStringP suffix,
    FdoInt32 position,
    FdoInt32 depth
)
{

    FdoRdbmsOvObjectPropertyP pObProp = CreateOvObjectPropertyDefinition( 
        FdoStringP::Format( L"Object%c", 'A' + position - 1 )
    );
	PropertiesOvAdd(pClass, pObProp );

    FdoRdbmsOvPropertyMappingConcreteP pMapping = CreateOvPropertyMappingConcrete();
    ObjectPropertyOvSetMappingDefinition(pObProp,  pMapping );

    FdoRdbmsOvClassP pInternalClass = CreateOvClassDefinition(
    );
    PropertyMappingOvSetInternalClass(pMapping, pInternalClass );

    FdoRdbmsOvTableP pTable = CreateOvTable(
        FdoStringP::Format( L"%ls_%ls%c", pClass->GetName(), L"ftable", 'a' + position - 1)
    );
    ClassOvSetTable(pInternalClass, pTable );

    if ( position == 3 || position == 6 ) {
        FdoRdbmsOvDataPropertyP pDataProp = CreateOvDataPropertyDefinition( 
            FdoStringP::Format( L"%ls_%ls", pClass->GetName(), L"FeatId" )
        );
       	PropertiesOvAdd(pInternalClass, pDataProp );
    
        FdoRdbmsOvColumnP pColumn = CreateOvColumn( L"ParentId" );
        DataPropOvSetColumn(pDataProp, pColumn );

        if ( position == 6 ) {
            pDataProp = CreateOvDataPropertyDefinition( L"OpA" );
       	    PropertiesOvAdd(pInternalClass, pDataProp );
        
            pColumn = CreateOvColumn( L"OP_PROP_A" );
            DataPropOvSetColumn(pDataProp, pColumn );
        }

    }

   	FdoRdbmsOvClassP pOpClass = CreateOvClassDefinition( 
        FdoStringP::Format( L"OvOpClass%ls%c", (FdoString*) suffix, 'A' + position - 1 )
    );
    ClassesOvAdd(pOverrides, pOpClass);

    pTable = CreateOvTable( 
        FdoStringP::Format( L"opc_table_%ls%c", (FdoString*) suffix.Lower(), 'a' + position - 1 )
    );
    ClassOvSetTable(pOpClass, pTable );

    if ( depth > 0 ) {
        if ( position == 4 ) {
            pObProp = CreateOvObjectPropertyDefinition( L"ObjectA" );
        	PropertiesOvAdd(pInternalClass, pObProp );

            pMapping = CreateOvPropertyMappingConcrete();
            ObjectPropertyOvSetMappingDefinition(pObProp, pMapping );

            pInternalClass = CreateOvClassDefinition();
            PropertyMappingOvSetInternalClass(pMapping, pInternalClass );

            pTable = CreateOvTable( L"fnested_da" );
            ClassOvSetTable(pInternalClass, pTable );

            FdoRdbmsOvDataPropertyP pDataProp = CreateOvDataPropertyDefinition( L"OpA" );
            PropertiesOvAdd(pInternalClass, pDataProp );
    
            FdoRdbmsOvColumnP pColumn = CreateOvColumn( L"NESTED_COL_A" );
            DataPropOvSetColumn(pDataProp, pColumn );
        }

        FdoRdbmsOvDataPropertyP pDataProp = CreateOvDataPropertyDefinition( L"OpB" );
       	PropertiesOvAdd(pOpClass, pDataProp );
    
        FdoRdbmsOvColumnP pColumn = CreateOvColumn( L"op_col_b" );
        DataPropOvSetColumn(pDataProp, pColumn );


        if ( position % 2 ) {
            CreateOverrideValueProperty( 
                pOverrides,
                pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
        else {
            CreateOverrideCollectionProperty( 
                pOverrides,
                pOpClass, 
                FdoStringP::Format( L"%ls%c", (FdoString*) suffix, 'A' + position - 1 ),
                1,
                depth - 1 
            ); 
        }
    }
}

FdoRdbmsOvPhysicalSchemaMapping* FdoApplySchemaTest::CreateErrorOverrides( FdoIConnection* connection )
{
    // Tests various bad settings for table and column names
    //  - invalid character in name
    //  - name too long
    //  - name is reserved word
    //  - name already used by another col umn.

    FdoRdbmsOvPhysicalSchemaMapping* pOverrides = 
        (FdoRdbmsOvPhysicalSchemaMapping*) connection->CreateSchemaMapping();

    FdoRdbmsOvClassP                   pClass;

    pClass = CreateOvClassDefinition( L"OvClassA" );
    ClassesOvAdd(pOverrides, pClass);
    FdoRdbmsOvGeometricPropertyP pGeomProp = CreateOvGeometricPropertyDefinition(
        L"GeomA"
    );
    PropertiesOvAdd(pClass, pGeomProp);
    FdoRdbmsOvGeometricColumnP pGeomCol = CreateOvGeometricColumn( 
        L"Bad Geom Col Name"
    );
    GeometricPropOvSetColumn(pGeomProp, pGeomCol);

    pClass = CreateOvClassDefinition( L"OvClassB" );
    ClassesOvAdd(pOverrides, pClass);
    FdoRdbmsOvTableP pTable = CreateOvTable( L"Bad Table Name" );
    ClassOvSetTable(pClass, pTable);

    pClass = CreateOvClassDefinition( L"OvClassC" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"toolong890123456789012345678901" );
    ClassOvSetTable(pClass, pTable);
    FdoRdbmsOvDataPropertyP pProp = CreateOvDataPropertyDefinition( L"DataC" );
    PropertiesOvAdd(pClass, pProp);
    FdoRdbmsOvColumnP pCol = CreateOvColumn( L"Germ\x00e4nColumnName" );
    DataPropOvSetColumn(pProp, pCol);

    pClass = CreateOvClassDefinition( L"OvClassD" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"exclusive" );
    ClassOvSetTable(pClass, pTable);
    pProp = CreateOvDataPropertyDefinition( L"DataD" );
    PropertiesOvAdd(pClass, pProp);
    pCol = CreateOvColumn( L"toolong890123456789012345678901" );
    DataPropOvSetColumn(pProp, pCol);

    pClass = CreateOvClassDefinition( L"OvClassE" );
    ClassesOvAdd(pOverrides, pClass);
    pProp = CreateOvDataPropertyDefinition( L"DataE" );
    PropertiesOvAdd(pClass, pProp);
    pCol = CreateOvColumn( L"initial" );
    DataPropOvSetColumn(pProp, pCol);

    pClass = CreateOvClassDefinition( L"OvClassF" );
    ClassesOvAdd(pOverrides, pClass);
    pProp = CreateOvDataPropertyDefinition( L"DataF" );
    PropertiesOvAdd(pClass, pProp);
    pCol = CreateOvColumn( L"dataa" );
    DataPropOvSetColumn(pProp, pCol);

    pGeomProp = CreateOvGeometricPropertyDefinition(
        L"GeomF"
    );
    PropertiesOvAdd(pClass, pGeomProp);
    pGeomCol = CreateOvGeometricColumn( 
        L"geomtoolong234567890123456789"
    );
    GeometricPropOvSetColumn(pGeomProp, pGeomCol);

    pClass = CreateOvClassDefinition( L"OvClassH" );
    ClassesOvAdd(pOverrides, pClass);
    pTable = CreateOvTable( L"Germ\x00e4nTableName" );
    ClassOvSetTable(pClass, pTable);

    FdoRdbmsOvObjectPropertyP pObProp = CreateOvObjectPropertyDefinition( L"ObjectA" );
    PropertiesOvAdd(pClass, pObProp);
    FdoRdbmsOvPropertyMappingSingleP mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    mapping->SetPrefix( L"PrefixTooLong456789012345678901" );

    pObProp = CreateOvObjectPropertyDefinition( L"ObjectB" );
    PropertiesOvAdd(pClass, pObProp);
    mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
    mapping->SetPrefix( L"Bad Prefix Name" );

    pClass = CreateOvClassDefinition( L"OvClassI" );
    ClassesOvAdd(pOverrides, pClass);
    pClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );
    pProp = CreateOvDataPropertyDefinition( L"DataI" );
    PropertiesOvAdd(pClass, pProp);
    pCol = CreateOvColumn( L"Bad Column Name" );
    DataPropOvSetColumn(pProp, pCol);

    pObProp = CreateOvObjectPropertyDefinition( L"ObjectF" );
    PropertiesOvAdd(pClass, pObProp);
    mapping = CreateOvPropertyMappingSingle();
    ObjectPropertyOvSetMappingDefinition(pObProp, mapping);

    pGeomProp = CreateOvGeometricPropertyDefinition(
        L"GeomI"
    );
    PropertiesOvAdd(pClass, pGeomProp);
    pGeomCol = CreateOvGeometricColumn( 
        L"datab"
    );
    GeometricPropOvSetColumn(pGeomProp, pGeomCol);

    pClass = CreateOvClassDefinition( L"OvClassJ" );
    ClassesOvAdd(pOverrides, pClass);
    pClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );

    return pOverrides;
}

void FdoApplySchemaTest::VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass )
{
    FdoInt32 expLockCount;
    
    switch ( lckMode ) {
        case 2:
            expLockCount = 5;
            break;
        case 1:
            expLockCount = 3;
            break;
        default:
            expLockCount = 1;
            break;
    }

    if ( FdoDataPropertiesP(pClass->GetIdentityProperties())->GetCount() == 0 &&
         FdoPtr<FdoReadOnlyDataPropertyDefinitionCollection>(pClass->GetBaseIdentityProperties())->GetCount() == 0)
         expLockCount = 0;

    // The following verifies that the AcDbEntity class capabilities are valid.
    FdoClassCapabilitiesP cc = pClass->GetCapabilities();
    CPPUNIT_ASSERT( cc != NULL );
    CPPUNIT_ASSERT( (lckMode != 0) == (cc->SupportsLocking()) );
    CPPUNIT_ASSERT( (ltMode != 0) == (cc->SupportsLongTransactions()) );

    FdoInt32 lockTypeCount;
    bool lockArray[5];
    FdoInt32 i;
    FdoLockType* lockTypes = cc->GetLockTypes( lockTypeCount );

    CPPUNIT_ASSERT( lockTypeCount == expLockCount );

    for ( i = 0; i < 5; i++ ) 
        lockArray[i] = false;

    for ( i = 0; i < lockTypeCount; i++ ) {
        switch ( lockTypes[i] ) {
        case FdoLockType_Transaction:
            lockArray[0] = true;
            break;
        case FdoLockType_Exclusive:
            lockArray[1] = true;
            break;
        case FdoLockType_LongTransactionExclusive:
            lockArray[2] = true;
            break;
        case FdoLockType_Shared:
            lockArray[3] = true;
            break;
        case FdoLockType_AllLongTransactionExclusive:
            lockArray[4] = true;
            break;
        default:
			CPPUNIT_FAIL( "Unexpected lock type" );
            break;
        }
    }

    for ( i = 0; i < expLockCount; i++ )
        CPPUNIT_ASSERT( lockArray[i] );


    FdoPtr<FdoPropertyDefinitionCollection> props = pClass->GetProperties();
    for ( i = 0; i < props->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
        if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
        {
            CPPUNIT_ASSERT( cc->GetPolygonVertexOrderRule(prop->GetName()) == FdoPolygonVertexOrderRule_CCW );
            CPPUNIT_ASSERT( !cc->GetPolygonVertexOrderStrictness(prop->GetName()) );
        }
    }
}

void FdoApplySchemaTest::WriteXmlOverrides(
    FdoIConnection* connection,
    bool includeDefaults,
    FdoString* schemaName,
    FdoString* fileName,
    FdoString* ownerSuffix
)
{

// This stylesheet removes the user-dependent prefix (e.g. fdo_br) from any 
// SchemaMapping/@owner attibutes, to prevent master file comparison failures
// when tests are run by different users.
static char* pFixSheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<stylesheet version=\"1.0\" \
xmlns=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:gml=\"http://www.opengis.net/gml\" \
xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:fdo=\"http://fdo.osgeo.org/schemas\" \
xmlns:ora=\"http://www.autodesk.com/isd/fdo/OracleProvider\" \
xmlns:rdb=\"http://fdordbms.osgeo.org/schemas\">\
<xsl:param name=\"ownerSuffix\" />\
<xsl:template match=\"ora:SchemaMapping|ora:Table\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*[not(name()='owner')]\"/>\
        <xsl:if test=\"@owner\">\
            <xsl:attribute name=\"owner\">\
                <xsl:call-template name=\"tolower\">\
                    <xsl:with-param name=\"inString\" select=\"concat('(fdo_user_prefix)',substring-after(@owner,substring-before(@owner,$ownerSuffix)))\"/>\
                </xsl:call-template>\
            </xsl:attribute>\
        </xsl:if>\
        <xsl:apply-templates select=\"node()[not(name()='owner')]\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
</xsl:template>\
<xsl:template name=\"tolower\">\
  <xsl:param name=\"inString\"/>\
  <xsl:value-of select=\"translate($inString,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')\"/>\
</xsl:template>\
</stylesheet>";

  	FdoPtr<FdoIDescribeSchemaMapping>  pDescCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);

    if ( includeDefaults )
        pDescCmd->SetIncludeDefaults(includeDefaults);

    if ( wcslen(schemaName) > 0 ) 
        pDescCmd->SetSchemaName( schemaName );

    FdoSchemaMappingsP mappings = pDescCmd->Execute();

    FdoIoMemoryStreamP outStream = FdoIoMemoryStream::Create();
    FdoIoMemoryStreamP outStream2;
    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fileName, L"wt+" );
        
    mappings->WriteXml( outStream );
    outStream->Reset();

    // remove any user-specific part of the SchemaMapping set owner attribute.
    if ( ownerSuffix ) {
        outStream2 = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP stylesheetStream = FdoIoMemoryStream::Create();

        stylesheetStream->Write( (FdoByte*) pFixSheet, strlen(pFixSheet) );
        stylesheetStream->Reset();

        FdoXslTransformerP tfmr = FdoXslTransformer::Create(
            FdoXmlReaderP( FdoXmlReader::Create(outStream) ),
            FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
            FdoXmlWriterP( FdoXmlWriter::Create(outStream2, false) )
        );

        FdoDictionaryP params = tfmr->GetParameters();

        params->Add( FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"ownerSuffix", 
                FdoStringP::Format( L"'%ls'", ownerSuffix ) 
            ) 
        ));

        tfmr->Transform();
        outStream2->Reset();
    }
    else {
        outStream2 = outStream;
    }

    XmlFormatter formatter( outStream2, fileStream );
    formatter.Format();
}

FdoStringP FdoApplySchemaTest::GenLtName( FdoStringP transName )
{
    return FdoStringP::Format( 
        L"%ls_%hs%ls",
        (FdoString*) UnitTestUtil::GetEnviron( "username" ),
        LT_NAME,
        (FdoString*) transName 
    );
}

void FdoApplySchemaTest::StartLongTransaction( FdoIConnection* connection, FdoStringP transName )
{
    FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
    gettrans->SetName( GenLtName(transName) );
    FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

    if ( rdr->ReadNext() == 0 ) {
        FdoPtr<FdoICreateLongTransaction> crtrans = (FdoICreateLongTransaction *) connection->CreateCommand(FdoCommandType_CreateLongTransaction);
        crtrans->SetName( GenLtName(transName) );
        crtrans->Execute();
    }

    rdr = NULL;

    FdoPtr<FdoIActivateLongTransaction> acttrans = (FdoIActivateLongTransaction *) connection->CreateCommand(FdoCommandType_ActivateLongTransaction);
    acttrans->SetName( GenLtName(transName) );
    acttrans->Execute();
}

void FdoApplySchemaTest::EndLongTransaction( FdoIConnection* connection )
{
    FdoPtr<FdoIDeactivateLongTransaction> deacttrans = (FdoIDeactivateLongTransaction *) connection->CreateCommand(FdoCommandType_DeactivateLongTransaction);
    deacttrans->Execute();
}

void FdoApplySchemaTest::RollbackLongTransaction( FdoIConnection* connection, FdoStringP transName )
{
    FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
    gettrans->SetName( GenLtName(transName) );
    FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

    if ( rdr->ReadNext() != 0 ) {
        rdr = NULL;
        FdoPtr<FdoIRollbackLongTransaction> rbtrans = (FdoIRollbackLongTransaction *) connection->CreateCommand(FdoCommandType_RollbackLongTransaction);
        rbtrans->SetName( GenLtName(transName) );
        rbtrans->Execute();
    }
}

FdoStringP FdoApplySchemaTest::GetActiveLongTransaction( FdoIConnection* connection )
{
    FdoStringP activeLT;

    FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
    FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

    while ( rdr->ReadNext() != 0 ) {
        if ( rdr->IsActive() ) {
            activeLT = rdr->GetName();
            break;
        }
    }

    return activeLT;
}

void FdoApplySchemaTest::InsertObject( FdoIConnection* connection, bool conditional, FdoStringP schemaName, FdoString* className, ... )
{
    va_list arguments;
    FdoString* arg;

    va_start(arguments, className);

    arg = va_arg(arguments,FdoString*);

    if ( conditional ) 
        StartLongTransaction( connection, schemaName );

    FdoITransaction* trans = (FdoITransaction *) connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    insertCommand->SetFeatureClassName(schemaName + L":" + FdoStringP(className));
    propertyValues = insertCommand->GetPropertyValues();

    while ( arg != NULL ) {

        propertyValue = FdoApplySchemaTest::AddNewProperty( propertyValues, arg);
        arg = va_arg(arguments,FdoString*);
        
        if ( arg != NULL ) {
            dataValue = FdoDataValue::Create(arg);
            propertyValue->SetValue(dataValue);
            arg = va_arg(arguments,FdoString*);
        }
    }
        
	FdoPtr<FdoIFeatureReader> rdr = insertCommand->Execute();

    trans->Commit();
    FDO_SAFE_RELEASE(trans);

    if ( conditional ) 
        EndLongTransaction( connection );

    va_end(arguments);
}

void FdoApplySchemaTest::DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoStringP className )
{
    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCommand->SetFeatureClassName(schemaName + L":" + className);
	deleteCommand->Execute();
}

void FdoApplySchemaTest::_logicalPhysicalBend( FdoString* inFile, FdoString* outFile, FdoStringP providerName )
{
    FdoIoFileStreamP stream1 = FdoIoFileStream::Create( inFile, L"rt" );
    FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::LogicalPhysicalBend( 
        stream1, 
        stream2, 
        providerName
    );

    stream2->Reset();
    UnitTestUtil::Stream2File( stream2, outFile );
}

void FdoApplySchemaTest::_logicalPhysicalFormat( FdoString* inFile, FdoString* outFile, FdoStringP providerName )
{
    FdoIoFileStreamP stream1 = FdoIoFileStream::Create( inFile, L"rt" );
    FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::LogicalPhysicalFormat( 
        stream1, 
        stream2,
        providerName
    );

    stream2->Reset();
    UnitTestUtil::Stream2File( stream2, outFile );
}

FdoStringP FdoApplySchemaTest::LogicalPhysicalBend( FdoString* inFile )
{
    return inFile;
}

FdoStringP FdoApplySchemaTest::LogicalPhysicalFormat( FdoString* inFile )
{
    return inFile;
}

bool FdoApplySchemaTest::CanApplyWithoutMetaSchema()
{
    return false;
}

bool FdoApplySchemaTest::CanCreateSchemaWithoutMetaSchema()
{
    return false;
}

bool FdoApplySchemaTest::CanAddNotNullCol()
{
    return true;
}

bool FdoApplySchemaTest::CanDropCol()
{
    return true;
}

FdoStringP FdoApplySchemaTest::SchemaTestErrFile( int fileNum, bool isMaster )
{
	if (isMaster)
		return FdoStringP::Format( L"apply_schema_err%d%ls.txt", fileNum, L"_master");
	else
		return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_err%d.txt", fileNum) );
}

FdoStringP FdoApplySchemaTest::SchemaNoMetaErrFile( int fileNum, bool isMaster )
{
    FdoStringP providerName;
    if ( (fileNum == 3) || (fileNum == 5) )
        providerName = FdoStringP(L"_") + UnitTestUtil::GetEnviron("provider");

    if (isMaster)
		return FdoStringP::Format( L"apply_no_meta_err%d%ls%ls.txt", fileNum, (FdoString*) providerName, L"_master");
	else
		return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_no_meta_err%d.txt", fileNum) );
}

FdoStringP FdoApplySchemaTest::SchemaOvErrFile( int fileNum, bool isMaster )
{
	if (isMaster)
		return FdoStringP::Format( L"apply_schema_overrides_err%d%ls.txt", fileNum, L"_master");
	else
		return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_overrides_err%d.txt", fileNum) );
}

FdoRdbmsOvClassDefinition* FdoApplySchemaTest::CreateOvClassDefinition(FdoString *name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvClassDefinition(name);
}

void FdoApplySchemaTest::ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass)
{
    UnitTestUtil::NewSchemaOverrideUtil()->ClassesOvAdd(pSchema, pClass);
}

FdoRdbmsOvTable* FdoApplySchemaTest::CreateOvTable(FdoString* name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvTable(name);
}

FdoRdbmsOvColumn* FdoApplySchemaTest::CreateOvColumn(FdoString* name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvColumn(name);
}

FdoRdbmsOvGeometricColumn* FdoApplySchemaTest::CreateOvGeometricColumn(FdoString* name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvGeometricColumn(name);
}

FdoRdbmsOvDataPropertyDefinition* FdoApplySchemaTest::CreateOvDataPropertyDefinition(FdoString *name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvDataPropertyDefinition(name);
}

FdoRdbmsOvGeometricPropertyDefinition* FdoApplySchemaTest::CreateOvGeometricPropertyDefinition(FdoString *name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvGeometricPropertyDefinition(name);
}

void FdoApplySchemaTest::PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp)
{
    UnitTestUtil::NewSchemaOverrideUtil()->PropertiesOvAdd(pClass, pProp);
}

void FdoApplySchemaTest::ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable)
{
    UnitTestUtil::NewSchemaOverrideUtil()->ClassOvSetTable(pClass, pTable);
}

void FdoApplySchemaTest::TableOvSetTablespace(FdoRdbmsOvTable* pTable, FdoString *tablespace)
{
    UnitTestUtil::NewSchemaOverrideUtil()->TableOvSetTablespace(pTable, tablespace);
}

void FdoApplySchemaTest::DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn)
{
    UnitTestUtil::NewSchemaOverrideUtil()->DataPropOvSetColumn(pDataProp, pDataColumn);
}

void FdoApplySchemaTest::GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn)
{
    UnitTestUtil::NewSchemaOverrideUtil()->GeometricPropOvSetColumn(pGeomProp, pGeometricColumn);
}

void FdoApplySchemaTest::ColumnOvSetSequence(FdoRdbmsOvColumn* pColumn, FdoString *sequence)
{
    UnitTestUtil::NewSchemaOverrideUtil()->ColumnOvSetSequence(pColumn, sequence);
}

FdoRdbmsOvObjectPropertyDefinition* FdoApplySchemaTest::CreateOvObjectPropertyDefinition(FdoString *name)
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvObjectPropertyDefinition(name);
}

FdoRdbmsOvPropertyMappingSingle* FdoApplySchemaTest::CreateOvPropertyMappingSingle()
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvPropertyMappingSingle();
}

FdoRdbmsOvPropertyMappingConcrete* FdoApplySchemaTest::CreateOvPropertyMappingConcrete()
{
    return UnitTestUtil::NewSchemaOverrideUtil()->CreateOvPropertyMappingConcrete();
}

void FdoApplySchemaTest::ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping)
{
    UnitTestUtil::NewSchemaOverrideUtil()->ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
}

void FdoApplySchemaTest::PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass)
{
    UnitTestUtil::NewSchemaOverrideUtil()->PropertyMappingOvSetInternalClass(pMapping, pClass);
}

void FdoApplySchemaTest::SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner)
{
    UnitTestUtil::NewSchemaOverrideUtil()->SchemaOvSetOwner(mapping, owner);
}

FdoFeatureSchemaP FdoApplySchemaTest::GetDefaultSchema( FdoIConnection* connection )
{
    FdoFeatureSchemaP defSchema;

	FdoPtr<FdoIDescribeSchema> cmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoFeatureSchemasP schemas = cmd->Execute();

    CPPUNIT_ASSERT( schemas->GetCount() > 0 );

    defSchema = schemas->FindItem(GetDefaultSchemaName());
    if ( !defSchema ) 
        defSchema = schemas->GetItem(0);

    return defSchema;
}

bool FdoApplySchemaTest::CompareGeometrySI()
{
	return false;
}


bool FdoApplySchemaTest::CreateGeometrySICol()
{
	return false;
}


FdoStringP FdoApplySchemaTest::GetValueColumnName()
{
	return L"Value1";
}

FdoStringP FdoApplySchemaTest::GetParcelFirstName()
{
	return L"first name";
}

FdoStringP FdoApplySchemaTest::GetParcelLastName()
{
	return L"last name";
}

FdoStringP FdoApplySchemaTest::GetDefaultSchemaName()
{
	return L"";
}

FdoPtr<FdoIConnection> FdoApplySchemaTest::GetDirectConnection (FdoIConnection *currentConnection)
{
    FdoPtr<FdoIConnection> directConnection;

    FdoStringP dataStoreUser = UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX);
    dataStoreUser = dataStoreUser.Replace(L"_foreign", L"_override");
    FdoPtr<FdoIConnectionInfo> connectionInfo = currentConnection->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> connectionInfoProperties = connectionInfo->GetConnectionProperties();
    FdoStringP serviceProperty = connectionInfoProperties->GetProperty(L"service");
    FdoStringP providerName = connectionInfo->GetProviderName();
    FdoStringP connectionString = FdoStringP::Format(
                                        L"service=%ls;username=%ls;password=%ls;datastore=%ls",
                                        (FdoString *)serviceProperty,
                                        (FdoString *)dataStoreUser,
                                        L"test",
                                        (FdoString *)dataStoreUser);

    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager();
    directConnection = manager->CreateConnection(providerName);
    directConnection->SetConnectionString(connectionString);

    return directConnection;
}



