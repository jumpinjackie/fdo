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

#define   DB_NAME_SUFFIX                    "_apply_schema"
#define   DB_NAME_COPY_SUFFIX               "_apply_copy"
#define   DB_NAME_OVERRIDE_SUFFIX           "_apply_override"
#define   DB_NAME_OVERRIDE_DEFAULT_SUFFIX   "_apply_overridedef"
#define   DB_NAME_FOREIGN_SUFFIX            "_apply_foreign"
#define   DB_NAME_CONFIG_SUFFIX             "_apply_config"

#define   LT_NAME                           L"ApplyTest"
#define   DB_NAME_LT_SUFFIX                 "_apply_lt"


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
    mCanAddNotNullCol(true), mIsLowerDatastoreName(false)
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
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;

    // SqlServer does not allow not null columns to be added to existing tables,
    // even if they are empty.
#ifdef RDBI_DEF_SSQL
    mCanAddNotNullCol = false;
#endif
/*
    if ( strcmp(UnitTestUtil::GetEnv("ltmethod", "-1"), "0") == 0 )
        mIsOWM = false;
    else
        mIsOWM = true;
*/
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

        printf( "Writing 3rd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_test3.xml" ) );

/*
		printf( "Testing Join Tree retrieval ... \n" );
		GetJoinTree( sm );
*/

		printf( "Testing Class Capabilities ... \n" );
		GetClassCapabilities( connection );

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

#ifndef RDBI_DEF_SSQL
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_test1_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test1.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_test2_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test2.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_test3_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test3.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_test4_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test4.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_test5_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_test5.xml" ) )
        );
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
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;
	
#ifdef RDBI_DEF_SSQL
    mCanAddNotNullCol = false;
#endif
    // Oracle-specific settings:
    if ( strcmp(UnitTestUtil::GetEnv("ltmethod", "-1"), "0") == 0 )
        mIsOWM = false;
    else
        mIsOWM = true;

    mDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX);

    try {

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

        // Create RDBMS-specific elements (such as filegroups on SQL Server, tablespaces on Oracle, etc):
        wchar_t *wDatastore = NULL;
        multibyte_to_wide(wDatastore, UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX));
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
        FdoSmPhMgrP ph = mgr->GetPhysicalSchema();

        FdoSmLpQClassesP classes;
#ifdef RDBI_DEF_SSQL
        classes = lp->TableToClasses( L"dbo.ovclassa" );
#else
        classes = lp->TableToClasses( L"ovclassa" );
#endif
        CPPUNIT_ASSERT( classes->GetCount() == 1 );
        CPPUNIT_ASSERT( wcscmp(classes->RefClassDefinition(0)->GetName(), L"OvClassA") == 0 );
#ifdef RDBI_DEF_SSQL
        classes = lp->TableToClasses( L"dbo.ov_table_c" );
#else
        classes = lp->TableToClasses( L"ov_table_c" );
#endif
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

#ifdef RDBI_DEF_ORA
		// grant access to foreign datastore.
		UnitTestUtil::GrantDatastore( connection, mgr, L"select", UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX) );
#endif

        FdoSmPhOwnerP owner = ph->GetOwner();
        FdoSmPhTableP table = owner->CreateTable( ph->GetDcDbObjectName(L"storage") );
        FdoSmPhColumnP column = table->CreateColumnInt64( ph->GetDcColumnName(L"id"), false );
        column = table->CreateColumnChar( ph->GetDcColumnName(L"storage"), true, 50 );
        column = table->CreateColumnGeom( ph->GetDcColumnName(L"floor"), (FdoSmPhScInfo*) NULL );
        column = table->CreateColumnInt16( ph->GetDcColumnName(L"extra"), !mCanAddNotNullCol );
        table->Commit();

        UnitTestUtil::Sql2Db( 
            L"insert into storage values ( 1, 'a string', NULL, 2)",
            connection
        );

#ifdef RDBI_DEF_ORA
        UnitTestUtil::Sql2Db( 
            FdoStringP::Format( 
                        L"grant select on %ls.storage to %ls",
                        (FdoString*) FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX)),
                        (FdoString*) FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX))
                    ),
                    connection
                );
#endif

        table = owner->CreateTable( ph->GetDcDbObjectName(L"storage_floor") );
        column = table->CreateColumnInt64( ph->GetDcColumnName(L"id"), false );
        column = table->CreateColumnInt64( ph->GetDcColumnName(L"storage_id"), false );
        column = table->CreateColumnChar( ph->GetDcColumnName(L"storage"), true, 50 );
        table->Commit();

#ifdef RDBI_DEF_ORA
        UnitTestUtil::Sql2Db( 
            FdoStringP::Format( 
                        L"grant select on %ls.storage_floor to %ls",
                        (FdoString*) FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX)),
                        (FdoString*) FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_FOREIGN_SUFFIX))
            ),
            connection
        );
#endif

        // Grab schemas and overrides to apply to foreign datastore

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    pDescCmd->SetSchemaName( L"OverridesA" );
	    FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	    FdoFeatureSchemaP  pSchema = pSchemas->GetItem( L"OverridesA" );

      	pDescMappingCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
        pDescMappingCmd->SetIncludeDefaults(true);
        mappings = pDescMappingCmd->Execute();

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_OVERRIDE_SUFFIX
		);

        // Compare output files with expected results.

#ifndef RDBI_DEF_SSQL
	    // First do xml dumps of LogicalPhysical schema
        // TODO: get this to work on MySQL and SQL Server
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_overrides1_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides1.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_overrides2_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides2.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_overrides3_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_overrides3.xml" ) )
        );
#endif

        // Next, compare described schema mappings.
        // TODO: get this to work on MySQL and SQL Server
#ifdef RDBI_DEF_ORA
        UnitTestUtil::CheckOutput( "apply_schema_overrides_out1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out1.xml" ) );
        UnitTestUtil::CheckOutput( "apply_schema_overrides_out2_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_out2.xml" ) );
#endif

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

        FdoRdbmsOvSchemaMappingP mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem( connection, pSchema->GetName());
        ShemaOvSetOwner(mapping, FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX)) );

        // Apply the foreign schema
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema( pSchema );
        pApplyCmd->SetPhysicalMapping( mapping );
        pApplyCmd->SetIgnoreStates(true);
        pApplyCmd->Execute();

        CreateForeignBasedSchema( 
            connection, 
            pSchema, 
            CreateForeignBasedOverrides(connection)
        );

        printf( "Writing 1st LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_foreign1.xml" ) );

        WriteXmlOverrides( 
            connection, 
            false, 
            L"", 
            UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out1.xml" ),
            FdoStringP(DB_NAME_OVERRIDE_SUFFIX)
        );

        WriteXmlOverrides( 
            connection, 
            true, 
            L"", 
            UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out2.xml" ),
            FdoStringP(DB_NAME_OVERRIDE_SUFFIX)
        );


        printf( "Modifying foreign schema ... \n" );
        ModOverrideSchemaForeign2( connection, mapping );

		printf( "Writing 2nd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"apply_schema_foreign2.xml" ) );

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

#ifdef RDBI_DEF_ORA
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_foreign1_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign1.xml" ) )
        );
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_foreign2_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign2.xml" ) )
        );
#endif
        UnitTestUtil::CheckOutput( 
            (const char*)LogicalPhysicalBend(L"apply_schema_foreign3_master.txt"), 
            (const char*)LogicalPhysicalFormat(UnitTestUtil::GetOutputFileName( L"apply_schema_foreign3.xml" ) )
        );

        //TODO: handle master comparison for other providers
#ifdef RDBI_DEF_ORA
        UnitTestUtil::CheckOutput( "apply_schema_foreign_out1_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out1.xml" ) );
	    UnitTestUtil::CheckOutput( "apply_schema_foreign_out2_master.txt", UnitTestUtil::GetOutputFileName( L"apply_schema_foreign_out2.xml" ) );
#endif

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_FOREIGN_SUFFIX
		);
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

void FdoApplySchemaTest::TestOverrideDefaults ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;

    if ( strcmp(UnitTestUtil::GetEnv("ltmethod", "-1"), "0") == 0 )
        mIsOWM = false;
    else
        mIsOWM = true;

    mDatastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_DEFAULT_SUFFIX);

	try {
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
        wchar_t *wDatastore = NULL;
        multibyte_to_wide(wDatastore, UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_DEFAULT_SUFFIX));
        CreateRdbmsSpecificElements(connection, wDatastore);

		// The following tests must be run in the following order to get the expected results.

		printf( "Creating Override Schema ... \n" );
		CreateOverrideSchema( 
            connection, 
            FdoRdbmsOvSchemaMappingP( CreateOverrideDefaults(connection, 1) )
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

    if ( strcmp(UnitTestUtil::GetEnv("ltmethod", "-1"), "0") == 0 )
        mIsOWM = false;
    else
        mIsOWM = true;

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
                UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_err1.txt" ), 
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
/*
		// Compare output files with expected results.
	    FdoStringP provider = UnitTestUtil::GetEnviron("provider","");
   		UnitTestUtil::CheckOutput( 
            (const char*) FdoStringP::Format( L"apply_schema_overrides_err1%ls_master.txt", (FdoString*) provider ), 
            UnitTestUtil::GetOutputFileName( L"apply_schema_overrides_err1.txt" )
        );
*/
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
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;
    FdoSmPhMgrP ph;

    int LtLckMethod = GetLtLockMethod();

	try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_LT_SUFFIX,
            0,
            NULL,
            LtLckMethod,
            true
		);

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_LT_SUFFIX );

		// The following tests must be run in the following order to get the expected results.

	    printf( "Creating LT Schema ... \n" );
	    CreateLTSchema( connection );

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

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"NEXTLTID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"LOCKID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = table->RefColumns()->RefItem( ph->GetDcColumnName(L"LOCKTYPE") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        if ( LtLckMethod == 1  ) 
            CPPUNIT_ASSERT( table->RefPkeyColumns()->GetCount() == 2 );
        else
            CPPUNIT_ASSERT( table->RefPkeyColumns()->GetCount() == 1 );

        column = table->RefPkeyColumns()->RefItem( ph->GetDcColumnName(L"FEATID") );
        CPPUNIT_ASSERT( column != NULL );
        column = table->RefPkeyColumns()->RefItem( 0 );
        CPPUNIT_ASSERT( FdoStringP(column->GetName()).ICompare(L"FEATID") == 0 );

        column = table->RefPkeyColumns()->RefItem( ph->GetDcColumnName(L"LTID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        ph = mgr->GetPhysicalSchema();
        FdoSmPhDbObjectP pTable;
        
#ifdef RDBI_DEF_ORA
        pTable = ph->FindDbObject(L"CIRCLE_LT1_GRIP",L"",L"",false);
#else
        pTable = ph->FindDbObject(L"CIRCLE_LT_GRIP",L"",L"",false);
#endif
        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"LTID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"NEXTLTID") );
        CPPUNIT_ASSERT( (LtLckMethod == 1) == (column != NULL) );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"LOCKID") );
        CPPUNIT_ASSERT( column == NULL );

        column = pTable->RefColumns()->RefItem( ph->GetDcColumnName(L"LOCKTYPE") );
        CPPUNIT_ASSERT( column == NULL );

	    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
        FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"LT" );

        FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"DelStatus" );
        VldClassCapabilities( LtLckMethod, LtLckMethod, pClass );
/*
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
		        UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( "apply_schema_lt_err1.txt" ) );
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
		        UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( "apply_schema_lt_err2.txt" ) );
		        FDO_SAFE_RELEASE(e);
	        }

	        if ( succeeded ) 
		        CPPUNIT_FAIL( "2nd Modification was supposed to fail." );

            CPPUNIT_ASSERT( GetActiveLongTransaction(connection) == (FdoStringP(LT_NAME) + L"LT" ) );
            EndLongTransaction( connection );

            RollbackLongTransaction(connection, L"LT" );

            printf( "Last Modification, should succeed ... \n" );
	        ModLTSchema( connection ); 
        }
*/
        printf( "Closing Connection ... \n" );
	    UnitTestUtil::CloseConnection(
	        connection,
		    false,
            DB_NAME_SUFFIX
		);
/*
		// Compare output files with expected results.
        if ( LtLckMethod > 0 ) {
    		UnitTestUtil::CheckOutput( "apply_schema_lt_err1_master.txt", UnitTestUtil::GetOutputFileName( "apply_schema_lt_err1.txt" ) );
	    	UnitTestUtil::CheckOutput( "apply_schema_lt_err1_master.txt", UnitTestUtil::GetOutputFileName( "apply_schema_lt_err2.txt" ) );
        }
*/
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
/*
// Test that various OWM reserved names are being recognized as such 
void FdoApplySchemaTest::TestLTReserved ()
{
	FdoPtr<FdoIConnection> connection;

    mIsOWM = true;

	try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            ""
		);

        DbiConnection* dbiConn = connection->GetDbiConnection();
*/
        /* Check some names with OWM reserved suffixes */
/*
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_aux") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_base") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_bpkc") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_conf") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_cons") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_diff") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_hist") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_lock") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_lt") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_lts") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_mw") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_pkc") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_pkd") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_pkdb") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_pkdc") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_arc_pkdc") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_arc_ltb") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_arc_ri$b") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_arc_ri$t") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_arc_vtb") ) );
*/       
        /* Check some OWM reserved names */
/*
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("childstate") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("parentstate") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("wm_retiretime") ) );
        CPPUNIT_ASSERT( dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("wm$otherchildsourcever") ) );
*/       
        /* Check some non-reserved names */
/*
        CPPUNIT_ASSERT( !dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_aux1") ) );
        CPPUNIT_ASSERT( !dbiConn->dbi_is_res_object( (char*) (const char*) FdoStringP("circle_aux_ox") ) );
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
*/

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
    <xsl:if test=\"not(@name = 'F_CLASSDEFINITION') and not(@name = 'f_classdefinition') and not(@name = 'dbo.f_classdefinition')\">\
        <xsl:variable name=\"tableName\" select=\"@name\"/>\
        <xsl:copy>\
            <xsl:apply-templates select=\"@*\"/>\
            <xsl:apply-templates select=\"lp:property[not(@name = 'FeatId' and ($tableName = 'PARCEL' or $tableName = 'ZONING' or $tableName = 'parcel' or $tableName = 'zoning' or $tableName = 'dbo.parcel' or $tableName = 'dbo.zoning')) and not(@name = 'ClassId' or @name = 'RevisionNumber' or @name = 'ClassName' or @name = 'SchemaName' or @name='Bounds')]\">\
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
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;
    FdoSmPhMgrP ph;
    FdoIoMemoryStreamP configStream1 = FdoIoMemoryStream::Create();
    FdoIoMemoryStreamP configStream2 = FdoIoMemoryStream::Create();

    FdoStringP RmvMetaSheet = FdoStringP(pRmvLpMetaSchema) + FdoStringP(pRmvLpMetaSchema2);

    FdoXmlSpatialContextFlagsP scFlags = FdoXmlSpatialContextFlags::Create();
    scFlags->SetIncludeDefault( true );

    // SqlServer does not allow not null columns to be added to existing tables,
    // even if they are empty.
#ifdef RDBI_DEF_SSQL
    mCanAddNotNullCol = false;
#endif
	try {
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
                 (objName.Mid( 0, 6 ) == L"dbo.f_")
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
        CPPUNIT_ASSERT( ph->GetDcDbObjectName(L"acdb3dpolyline") == lpTable->GetName() );

        const FdoSmLpDataPropertyDefinition* lpProp = 
            FdoSmLpDataPropertyDefinition::Cast(lpClass->RefProperties()->RefItem(L"Closed"));
        CPPUNIT_ASSERT(lpProp);

        const FdoSmPhColumn* lpColumn = lpProp->RefColumn();
        CPPUNIT_ASSERT(lpColumn);
        CPPUNIT_ASSERT( ph->GetDcColumnName(L"closed") == lpColumn->GetName() );

        lpClass = lp->FindClass( L"Electric'l", L"Transformer" );
        CPPUNIT_ASSERT( lpClass );

        const FdoSmLpGeometricPropertyDefinition* lpGeomProp = 
            FdoSmLpGeometricPropertyDefinition::Cast(lpClass->RefProperties()->RefItem(L"Geometry"));
        CPPUNIT_ASSERT(lpProp);

        lpColumn = lpGeomProp->RefColumn();
        CPPUNIT_ASSERT(lpColumn);
        CPPUNIT_ASSERT( ph->GetDcColumnName(L"geometry") == lpColumn->GetName() );

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
            table = owner->GetDbObject( ph->GetDcDbObjectName(L"parcel") )->SmartCast<FdoSmPhTable>();
            constraint = new FdoSmPhCheckConstraint( L"value_check", ph->GetDcColumnName(L"value"), L"value < 10000000" );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( L"alter table parcel add constraint value_check check ( value1 < 10000000 )" );
        }

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"zoning") )->SmartCast<FdoSmPhTable>();

        FdoSmPhColumnsP	ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		int numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"authority") );
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"zoningtype") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);
        grdOwner->ActivateAndExecute( L"alter table zoning add constraint z1 unique ( authority, zoningtype )" );

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"transformer") )->SmartCast<FdoSmPhTable>();

        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"phase") );
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"partnum") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);
        grdOwner->ActivateAndExecute( L"alter table transformer add constraint z2 unique ( phase, partnum )" );

        if ( supportsRange ) {
            constraint = new FdoSmPhCheckConstraint( L"partnum_check", ph->GetDcColumnName(L"partnum"), L"partnum > 0" );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( L"alter table transformer add constraint partnum_check check ( partnum > 0 )" );
        }

        table = owner->GetDbObject( ph->GetDcDbObjectName(L"pole") )->SmartCast<FdoSmPhTable>();

        ukeyColumns = new FdoSmPhColumnCollection();
	    table->GetUkeyColumns()->Add( ukeyColumns );
		numUkeys = table->GetUkeyColumns()->GetCount();
        table->AddUkeyCol( numUkeys - 1, ph->GetDcColumnName(L"height") );
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);
        grdOwner->ActivateAndExecute( L"alter table pole add constraint z3 unique ( height )" );

        if ( supportsRange ) {
            constraint = new FdoSmPhCheckConstraint( L"height_check", ph->GetDcColumnName(L"height"), L"height > 5" );
            table->AddCkeyCol( constraint );
            grdOwner->ActivateAndExecute( L"alter table pole add constraint height_check check ( height > 5.1 )" );
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
        FdoPtr<FdoInt32Value> intVal;
        FdoInt32 ix;

        if ( supportsRange ) {
            classDef = classes->GetItem( L"Parcel" );
            properties = classDef->GetProperties();
            dataProp = (FdoDataPropertyDefinition*)(properties->GetItem(L"Value"));
            valCons = (FdoPropertyValueConstraintRange*)(dataProp->GetValueConstraint());
            if ( !valCons ) 
                CPPUNIT_FAIL( "Parcel check constraint missing; if on SqlServer 2005 then this is expected" );
            intVal = (FdoInt32Value*)(valCons->GetMaxValue());
            CPPUNIT_ASSERT( intVal->GetInt32() == 10000000 );
        }

        classDef = classes->GetItem( L"Zoning" );
        FdoPtr<FdoUniqueConstraintCollection> constraints = classDef->GetUniqueConstraints();
        FdoPtr<FdoUniqueConstraint> ucons;
        FdoDataPropertiesP dataProps;

		// SchemaMgr is adding an unique constraint on auto-incremented columns
		CPPUNIT_ASSERT( constraints->GetCount() == 4);

        bool constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 2 )
                constraintFound = ( dataProps->FindItem(L"Authority") && dataProps->FindItem(L"ZoningType") );
        }

        CPPUNIT_ASSERT( constraintFound );

        schema = schemas->GetItem( L"Electric'l" );
        classes = schema->GetClasses();

        classDef = classes->GetItem( L"Transformer" );
        constraints = classDef->GetUniqueConstraints();

		// SchemaMgr is adding an unique constraint on auto-incremented columns
		CPPUNIT_ASSERT( constraints->GetCount() == 2);

        constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 2 )
                constraintFound = ( dataProps->FindItem(L"Phase") && dataProps->FindItem(L"PartNum") );
        }

        CPPUNIT_ASSERT( constraintFound );

        if ( supportsRange ) {
            properties = classDef->GetProperties();
            dataProp = (FdoDataPropertyDefinition*)(properties->GetItem(L"PartNum"));
            valCons = (FdoPropertyValueConstraintRange*)(dataProp->GetValueConstraint());
            intVal = (FdoInt32Value*)(valCons->GetMinValue());
            CPPUNIT_ASSERT( intVal->GetInt32() == 0 );
        }

        classDef = classes->GetItem( L"Pole" );
        constraints = classDef->GetUniqueConstraints();

		// SchemaMgr is adding an unique constraint on auto-incremented columns
		CPPUNIT_ASSERT( constraints->GetCount() == 2);

        constraintFound = false;

        for ( ix = 0, constraintFound = false; (ix < constraints->GetCount()) && !constraintFound; ix++ ) {
            ucons = constraints->GetItem(ix);
            dataProps = ucons->GetProperties();
            if ( dataProps->GetCount() == 1 )
                constraintFound = ( dataProps->FindItem(L"Height") != NULL  );
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
#ifdef _WIN32
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' );
        if (pMessage) pMessage += 2;
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

void FdoApplySchemaTest::DeleteLandSchema( FdoIConnection* connection )
{
	UnitTestUtil::Sql2Db( L"delete from a1_8_school", connection );
	UnitTestUtil::Sql2Db( L"delete from driveway", connection );
	UnitTestUtil::Sql2Db( L"delete from parcel_person", connection );

	FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);

	pCmd->SetSchemaName( L"Acad" );
	pCmd->SetSchemaName( L"Land" );

	pCmd->Execute();
}

void FdoApplySchemaTest::CreateSystemSchema( FdoIConnection* connection )
{
	FdoStringP datastoreName;

	/* Try to create schema with same name as the datastore.
	   This should fail since each datastore has a system schema
	   with this name.
     */

    datastoreName = FdoStringP::Format( L"%hs", UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX ) ).Upper();
    if ( mIsLowerDatastoreName ) 
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
		UnitTestUtil::PrintException(e, SchemaTestErrFile(100,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "System schema create was supposed to fail" );
}

void FdoApplySchemaTest::CreateAcadSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );
	pSchemas->Add( pSchema );

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

	FdoClassesP(pSchema->GetClasses())->Add( pVertex );

	// A non-abstract base class

	FdoPtr<FdoFeatureClass> pEntClass = FdoFeatureClass::Create( L"AcDbEntity", L"AutoCAD entity base class" );
	pEntClass->SetIsAbstract(false);
    // Test unsetting geometry for class with no geometric properties - should have no effect.
    pEntClass->SetGeometryProperty(NULL);

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

	pProp = FdoDataPropertyDefinition::Create( L"ColourIndex", L"Acad Colour" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	pObjProp->SetClass( pXData );
	pObjProp->SetIdentityProperty( pXDataSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pEntClass->GetProperties())->Add( pObjProp );

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

	pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	pObjProp->SetClass( pXData );
	pObjProp->SetIdentityProperty( pXDataSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pObjProp );

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
	pProp->SetNullable(!mCanAddNotNullCol);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	pObjProp->SetClass( pVertex );
	pObjProp->SetIdentityProperty( pVertexSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pObjProp );

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

	pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	pObjProp->SetClass( pVertex );
	pObjProp->SetIdentityProperty( pVertexSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pObjProp );

	// The following tests object property nesting to 3 levels.

	FdoPtr<FdoFeatureClass> pHatchClass = FdoFeatureClass::Create( L"AcDbHatch", L"AutoCAD hatched polygon" );
	pHatchClass->SetIsAbstract(false);
	pHatchClass->SetBaseClass( pEntClass );

	pObjProp = FdoObjectPropertyDefinition::Create( L"edges", L"" );
	pObjProp->SetClass( pPlineRefClass );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pHatchClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pHatchClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

  	// Insert a row with null colour. Subsequent removal of colour property should succeed.

    InsertObject( connection, false, L"Acad", L"AcDbEntity", L"Layer", L"default", NULL );
    InsertObject( connection, false, L"Acad", L"Entity", L"FeatId", L"1", L"Layer", L"default", NULL );
}

void FdoApplySchemaTest::CreateElectricSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	pDescCmd->SetSchemaName( L"Acad" );
    
	FdoPtr<FdoFeatureSchemaCollection> pAcadSchema = pDescCmd->Execute();

	FdoPtr<FdoClassDefinition> pEntClass = FdoClassesP(FdoFeatureSchemaP(pAcadSchema->GetItem( L"Acad" ))->GetClasses())->GetItem( L"Entity" );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* A schema with dictionary */

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Electric'l", L"Electrical '' schema'" );
	pAcadSchema->Add( pSchema );

	FdoSADP(pSchema->GetAttributes())->Add( L"'Author", L"Thomas O'Edison" );

	/* An abstract base class */

	FdoPtr<FdoFeatureClass> pDevClass = FdoFeatureClass::Create( L"ElectricDevice", L"electic base class" );
	pDevClass->SetIsAbstract(true);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDevClass->GetIdentityProperties())->Add( pProp );

	// Test nested object properties (ElectricDevice.graphic.xdata) where graphic's class has an id.

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"graphic", L"Acad entity" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pEntClass );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );

	// Test geometry property

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	pGeomProp->SetHasElevation(true);
    FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );

	/* Subclass with dictionary */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Transformer", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

 	FdoSADP(pClass->GetAttributes())->Add( L"Rules' DLL", L"transformer.dll" );
	FdoSADP(pClass->GetAttributes())->Add( L"Entrypoint", L"start_transformer" );

	// Add data properties of various types

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"voltage" );
	FdoSADP(pProp->GetAttributes())->Add( L"Calculable", L"yes" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
/* TODO: -2 on MySql?
	pProp->SetScale(-2);
*/
	pProp->SetScale(1);
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

void FdoApplySchemaTest::CreateLongStringSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    /* Test various long schema, class and property names. */

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );

    /* Create some id'less object property classes. */

	FdoPtr<FdoClass> pNestedObjClass = FdoClass::Create( L"oacdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pNestedObjClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pNestedObjClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pNestedObjClass );

	FdoPtr<FdoClass> pObjClass = FdoClass::Create( L"obcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pObjClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pObjClass->GetProperties())->Add( pProp );

    FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"cbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
    pObjProp->SetObjectType( FdoObjectType_Value );
    pObjProp->SetClass( pNestedObjClass );
	FdoPropertiesP(pObjClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pObjClass );

	FdoPtr<FdoFeatureClass> pBaseClass = FdoFeatureClass::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pBaseClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);

    // Create a SAD entry with long name and value
    FdoSADP(pProp->GetAttributes())->Add( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", 
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
        L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" 
        L"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012the-end" 
    );

	FdoPropertiesP(pBaseClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBaseClass->GetIdentityProperties())->Add( pProp );

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

void FdoApplySchemaTest::CreateLandSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	pDescCmd->SetSchemaName( L"Acad" );

	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    CreateLandSchema( pSchemas );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem(L"Land");

    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	// Test GetFeatureSchema
    CPPUNIT_ASSERT(wcscmp(FdoFeatureSchemaP(pCmd->GetFeatureSchema())->GetName(), L"Land") == 0);

    InsertObject(connection, false, L"Land", L"1-8 School", L"# Rooms", L"20", NULL );
    InsertObject(connection, false, L"Land", L"Driveway", L"Pav'd", L"1", NULL );
	UnitTestUtil::Sql2Db( L"insert into parcel_person ( first_name, last_name, parcel_province, parcel_pin ) values ( 'Fraser', 'Simon', 'Ontario', '1234-5678' )", connection );
}

void FdoApplySchemaTest::CreateLandSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoPtr<FdoFeatureSchema> pAcadSchema = pSchemas->GetItem(L"Acad");
	FdoPtr<FdoClassDefinition> pEntityClass = FdoClassesP(pAcadSchema->GetClasses())->GetItem(L"AcDbEntity");

    /* Create a schema to test successful schema deletion */

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Land", L"Property schema" );
	pSchemas->Add( pSchema );

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

	pProp = FdoDataPropertyDefinition::Create( L"Province", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(15);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetNullable(true);
	pProp->SetPrecision( 8 );
	pProp->SetScale( 0 );
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"owner", L"" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pPersonClass );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

    // Test adding geometric property that is not the geometry property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Grading", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Surface  );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Create class with base class in different schema */

	pClass = FdoFeatureClass::Create( L"Cogo Point", L"'Surveyor's point" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pEntityClass );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    FdoPtr<FdoFeatureClass> pBldgClass = FdoFeatureClass::Create( L"Build'g", L"'" );
	pBldgClass->SetIsAbstract(true);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBldgClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"# Rooms", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"% Occupied", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

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
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
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

void FdoApplySchemaTest::ModElectricSchema( FdoIConnection* connection )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

    ModElectricSchema( pSchemas );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::ModElectricSchema( FdoFeatureSchemaCollection* pSchemas )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	// Modify schema dictionary

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

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"classname", L"" );
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

	FdoPtr<FdoClass> pMaintHist = FdoClass::Create( L"Maint History", L"Maintenance History" );
	pMaintHist->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pHistId = FdoDataPropertyDefinition::Create( L"Date", L"Date of work done" );
	pHistId ->SetDataType( FdoDataType_DateTime );
	pHistId ->SetNullable(false);
	FdoPropertiesP(pMaintHist->GetProperties())->Add( pHistId );

	pProp = FdoDataPropertyDefinition::Create( L"Maintenance History Description", L"datavalue" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(50);
	pProp->SetNullable(true);
	FdoPropertiesP(pMaintHist->GetProperties())->Add( pProp );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"work items", L"tasks performed on this date" );
	pObjProp->SetClass( pMaintHistItem );
	pObjProp->SetIdentityProperty( pItemId );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Descending );
	FdoPropertiesP(pMaintHist->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pMaintHist );

	// Update Geometry property

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));

    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pDevClass->GetProperties())->GetItem(L"Geometry");
	pGeomProp->SetDescription( L"updated geometry description" );

	// Add the nested object property. Since added to base class, this also tests inheritance.

	pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );

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
	
	// Delete property that is also being added to base class ( property should stay but become inherited )
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP( pClass->GetProperties() )->GetItem( L"InstallDate" );
	pProp->Delete();

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

void FdoApplySchemaTest::ModLandSchema( FdoIConnection* connection )
{
	/* 
     * Test deleting a geometric property that is the geometry property of a 
     * subclass
     */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Land" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Driveway"));
	pClass->SetGeometryProperty(NULL);

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

   	UnitTestUtil::Sql2Db( L"delete from a1_8_school", connection );

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

void FdoApplySchemaTest::ModDelSchemas( FdoIConnection* connection )
{
	/* Test some more modifications plus deletions. */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
    ModDelElectricSchema( pSchemas );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	pSchema = pSchemas->GetItem( L"Acad" );

    ModDelAcadSchema( pSchemas );
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoApplySchemaTest::ModDelElectricSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	// Dictionary elemetn delete

	FdoSADP(pSchema->GetAttributes())->Remove( L"'Author" );

	// Class delete

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Pole"));
	pClass->Delete();

	// Geometric and Object Property delete. Also tests rippling to sub-classes of ElectricDevice.

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));
	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem(L"Geometry");
	pGeomProp->Delete();
    pClass->SetGeometryProperty(NULL);

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"maintenance history"));
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

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));

    FdoPtr<FdoDataPropertyDefinition> pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Volume" );
	pProp->Delete();

	pProp = FdoDataPropertyDefinition::Create( L"Temperature", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
}

void FdoApplySchemaTest::ModDelAcadSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Acad" );
	pSchema->SetDescription( L"A'CAD Entity Schema" );

	FdoFeatureClassP pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbEntity"));
	FdoPtr<FdoClass> pRefClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Entity"));

	// Test delete of property that has values but all values are null.

    FdoDataPropertyP pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"ColourIndex"));
	pProp->Delete();

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"ColourIndex"));
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

void FdoApplySchemaTest::ReAddElements( FdoIConnection* connection )
{
    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

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

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );

	// Re-add deleted geometry

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

    // Try geometry with similar name but different case (should get different column.
    // Skip sqlserver since seconds is data case-insensitive.
#ifndef RDBI_DEF_SSQL
    pGeomProp = FdoGeometricPropertyDefinition::Create( L"GEOMETRY", L"location and shape" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
#endif

	// Re-add deleted data property. Give it different type and nullibility than before. 
	// Note that "volume" column is still around ( the provider doesn't yet support column deletes )
	// so the following creates a "volume1" column.
/*
	pProp = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
*/
	// Re-add a nested object property. Since added to base class, this also tests inheritance.

	FdoPtr<FdoClassDefinition> pMaintHist = FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History" );
	FdoPtr<FdoDataPropertyDefinition> pHistId = (FdoDataPropertyDefinition*) FdoPropertiesP(pMaintHist->GetProperties())->GetItem( L"Date" );
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

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
	CPPUNIT_ASSERT( wcsicmp( pLpTable->GetName(), L"COGO_POINT" ) == 0 );
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

	CPPUNIT_ASSERT( FdoStringP(pLpTable->GetName()).ICompare(L"F_CLASSDEFINITION") == 0 );
	CPPUNIT_ASSERT( FdoStringP(pLpTable->GetTargetTable()->GetName()).ICompare(L"COGO_POINT") == 0 );

    CPPUNIT_ASSERT( pLpTable->GetSourceColumns()->GetCount() == 1 );
	CPPUNIT_ASSERT( FdoStringP(pLpTable->GetSourceColumns()->GetItem(0)->GetName()).ICompare(L"CLASSID") == 0 );
	CPPUNIT_ASSERT( pLpTable->GetTargetColumns()->GetCount() == 1 );
	CPPUNIT_ASSERT( FdoStringP(pLpTable->GetTargetColumns()->GetItem(0)->GetName()).ICompare(L"CLASSID") == 0 );

	CPPUNIT_ASSERT( baseNode->GetSourceNodes()->GetCount() == 0 );

	rootNode->Release();
}
#endif

void FdoApplySchemaTest::GetClassCapabilities( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Acad" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
    FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Acad" );

    FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"AcDbEntity" );
    VldClassCapabilities( 0, 0, pClass );

    pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"AcXData" );
    VldClassCapabilities( 0, 0, pClass );

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
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 3 );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"ClassId") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"RevisionNumber") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"FeatId") );

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
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
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
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 3 );
    pFeatClass->SetBaseClass(pNewClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 1 );

    FdoDataPropertiesP(pNewClass->GetIdentityProperties())->Clear();
    pNewClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 9 );
    pNewClass->SetBaseClass(pBldgClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"# Rooms"))->GetQualifiedName() == L"Land:Build'g.# Rooms" );
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"RevisionNumber"))->GetQualifiedName() == L"RevisionNumber" );
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

void FdoApplySchemaTest::CreateOverrideSchema( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides, bool nnull )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoFeatureSchemaP                   pSchema = FdoFeatureSchema::Create( L"OverridesA", L"AutoCAD schema" );
    FdoFeatureClassP                    pFeatClass;
    FdoFeatureClassP                    pBaseClass;
    FdoDataPropertyP                    pProp;
    FdoGeometricPropertyP               pGeomProp;

    FdoInt32                            idx;


    for ( idx = 0; idx < 10; idx++ ) {
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

    pCmd->SetFeatureSchema( pSchema );

    if ( pOverrides ) 
        pCmd->SetPhysicalMapping( pOverrides );

	pCmd->Execute();
}
#if 0
void FdoApplySchemaTest::CreateForeignErrorSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	FdoFeatureSchemaP                   pSchema = FdoFeatureSchema::Create( L"ForeignError", L"Foreign Schema with errors" );
    FdoFeatureClassP                    pClass;
    FdoDataPropertyP                    pProp;

    pClass = FdoFeatureClass::Create( L"ErrClassA", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );

    pProp = FdoDataPropertyDefinition::Create( L"Id", L"id" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(false);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength( 50 );
    pProp->SetNullable(false);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    pClass = FdoFeatureClass::Create( L"ErrClassB", L"" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );

    pProp = FdoDataPropertyDefinition::Create( L"Id", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength( 50 );
    pProp->SetNullable(false);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pClass = FdoFeatureClass::Create( L"ClassC", L"no errors" );
    FdoClassesP(pSchema->GetClasses())->Add( pClass );

    pProp = FdoDataPropertyDefinition::Create( L"Id", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength( 50 );
    pProp->SetNullable(false);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    FdoOracleOvSchemaMappingP mapping = FdoOracleOvPhysicalSchemaMapping::Create( L"ForeignError");
    mapping->SetOwner( FdoStringP(UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX)) );

    pCmd->SetFeatureSchema( pSchema );
    pCmd->SetPhysicalMapping( mapping );
	pCmd->Execute();
}
#endif
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
    pProp->Delete();

    pProp = FdoDataPropertyDefinition::Create( L"DataNew", L"a data property" );
    pProp->SetDataType( FdoDataType_Double );
    pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pObProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"ObjectA" ));
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

    pProp = FdoDataPropertyDefinition::Create( L"DataH", L"a data property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
    FdoPropertiesP(pFeatClass->GetProperties())->Add( pProp );

    pObProp = FdoObjectPropertyDefinition::Create( L"ObjectA", L"" );
    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pFeatClass->GetProperties())->Add( pObProp );

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

    pObProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pFeatClass->GetProperties())->GetItem( L"Object A" ));
    pObProp->Delete();

    pFeatClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem( L"OvClassI" ));
    pFeatClass->Delete();

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
#ifndef RDBI_DEF_SSQL
    FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Floor", L"location and shape" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetHasElevation(true);
    pProps->Add( pGeomProp );
#endif
    FdoObjectPropertyP pObProp = FdoObjectPropertyDefinition::Create( L"FloorA", L"location and shape" );
    pObProp->SetClass( pOpClass );
    pObProp->SetObjectType( FdoObjectType_Value );
    pProps->Add( pObProp );
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
    pProp->Delete();
        
    pProp = FdoDataPropertyDefinition::Create( L"Extra", L"a new data property" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(!mCanAddNotNullCol);
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
        L"geomtoolong23456789012345678901"
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
<xsl:template match=\"ora:SchemaMapping\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*[not(name()='owner')]\"/>\
        <xsl:if test=\"@owner\">\
            <xsl:attribute name=\"owner\">\
                <xsl:value-of select=\"concat('(fdo_user_prefix)',substring-after(@owner,substring-before(@owner,$ownerSuffix)))\"/>\
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

void FdoApplySchemaTest::StartLongTransaction( FdoIConnection* connection, FdoStringP transName )
{
    if ( mIsOWM ) {
        FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
        gettrans->SetName( FdoStringP(LT_NAME) + transName );
        FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

        if ( rdr->ReadNext() == 0 ) {
            FdoPtr<FdoICreateLongTransaction> crtrans = (FdoICreateLongTransaction *) connection->CreateCommand(FdoCommandType_CreateLongTransaction);
            crtrans->SetName( FdoStringP(LT_NAME) + transName );
            crtrans->Execute();
        }

        rdr = NULL;

        FdoPtr<FdoIActivateLongTransaction> acttrans = (FdoIActivateLongTransaction *) connection->CreateCommand(FdoCommandType_ActivateLongTransaction);
        acttrans->SetName( FdoStringP(LT_NAME) + transName );
        acttrans->Execute();
    }
}

void FdoApplySchemaTest::EndLongTransaction( FdoIConnection* connection )
{
    if ( mIsOWM ) {
        FdoPtr<FdoIDeactivateLongTransaction> deacttrans = (FdoIDeactivateLongTransaction *) connection->CreateCommand(FdoCommandType_DeactivateLongTransaction);
        deacttrans->Execute();
    }
}

void FdoApplySchemaTest::RollbackLongTransaction( FdoIConnection* connection, FdoStringP transName )
{
    if ( mIsOWM ) {
        FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
        gettrans->SetName( FdoStringP(LT_NAME) + transName );
        FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

        if ( rdr->ReadNext() != 0 ) {
            rdr = NULL;
            FdoPtr<FdoIRollbackLongTransaction> rbtrans = (FdoIRollbackLongTransaction *) connection->CreateCommand(FdoCommandType_RollbackLongTransaction);
            rbtrans->SetName( FdoStringP(LT_NAME) + transName );
            rbtrans->Execute();
        }
    }
}

FdoStringP FdoApplySchemaTest::GetActiveLongTransaction( FdoIConnection* connection )
{
    FdoStringP activeLT;

    if ( mIsOWM ) {
        FdoPtr<FdoIGetLongTransactions> gettrans = (FdoIGetLongTransactions *) connection->CreateCommand(FdoCommandType_GetLongTransactions);
        FdoPtr<FdoILongTransactionReader> rdr = gettrans->Execute();

        while ( rdr->ReadNext() != 0 ) {
            if ( rdr->IsActive() ) {
                activeLT = rdr->GetName();
                break;
            }
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
/*
    if ( conditional ) 
        StartLongTransaction( connection, schemaName );
*/
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
        
	insertCommand->Execute();

    trans->Commit();
    FDO_SAFE_RELEASE(trans);
/*
    if ( conditional ) 
        EndLongTransaction( connection );
*/
    va_end(arguments);
}

void FdoApplySchemaTest::DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoStringP className )
{
/*
    StartLongTransaction( connection, schemaName );
*/
    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCommand->SetFeatureClassName(schemaName + L":" + className);
	deleteCommand->Execute();
/*
    EndLongTransaction( connection );
*/
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

void FdoApplySchemaTest::_logicalPhysicalFormat( FdoString* inFile, FdoString* outFile )
{
    FdoIoFileStreamP stream1 = FdoIoFileStream::Create( inFile, L"rt" );
    FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::LogicalPhysicalFormat( 
        stream1, 
        stream2
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

FdoStringP FdoApplySchemaTest::SchemaTestErrFile( int fileNum, bool isMaster )
{
	if (isMaster)
		return FdoStringP::Format( L"apply_schema_err%d%ls.txt", fileNum, L"_master");
	else
		return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_err%d%ls.txt", fileNum, L"_master") );
}
