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
#include "FdoSchemaTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>

#define DB_SUFFIX L"_nometaschema"

FdoSchemaTest::FdoSchemaTest(void) :
    m_bDatabaseCreated(false)
{
}

FdoSchemaTest::~FdoSchemaTest(void)
{
}

void FdoSchemaTest::setUp ()
{
    set_provider();
}

void FdoSchemaTest::DropDatastore()
{
    // Drop the old datastore, if it exists:
    try
    {
        FdoStringP dataStoreName = UnitTestUtil::GetEnviron("datastore", DB_SUFFIX);
        FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, L"");
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConnectionString( userConnectString );
        connection->Open();
        FdoPtr<FdoIDestroyDataStore> pDelCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );
        FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDelCmd->GetDataStoreProperties();
        dictionary->SetProperty( L"DataStore",  dataStoreName);
        pDelCmd->Execute();
        connection->Close();
    }
    catch(...) { }
}

void FdoSchemaTest::CreateFreshDb()
{
    if (!m_bDatabaseCreated)
    {
		DropDatastore();
        // Create a datastore (will create metaschema tables automatically, unfortunately):
        UnitTestUtil::CreateDB(false, false, DB_SUFFIX);

        // Connect:
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_SUFFIX));
        connection->Open();

        // Define one new schema:
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"lowercaseschema", L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        // Define one new class with X/Y/Z lowercase property names (and an identity property and a geometry property):
        FdoPtr<FdoFeatureClass> newClass = FdoFeatureClass::Create(L"lowercaseclass", L"");
        FdoPtr<FdoPropertyDefinitionCollection> newProps = newClass->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> newIds = newClass->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> dataPropID = FdoDataPropertyDefinition::Create(L"id", L"");
        dataPropID->SetDataType(FdoDataType_Int32);
        dataPropID->SetNullable(false);
        newProps->Add(dataPropID);
        newIds->Add(dataPropID);
        FdoPtr<FdoDataPropertyDefinition> dataPropX = FdoDataPropertyDefinition::Create(L"x", L"");
        dataPropX->SetDataType(FdoDataType_Double);
        dataPropX->SetNullable(false);
        newProps->Add(dataPropX);
        FdoPtr<FdoDataPropertyDefinition> dataPropY = FdoDataPropertyDefinition::Create(L"y", L"");
        dataPropY->SetDataType(FdoDataType_Double);
        dataPropY->SetNullable(false);
        newProps->Add(dataPropY);
        FdoPtr<FdoDataPropertyDefinition> dataPropZ = FdoDataPropertyDefinition::Create(L"z", L"");
        dataPropZ->SetDataType(FdoDataType_Double);
        dataPropZ->SetNullable(false);
        newProps->Add(dataPropZ);
        FdoPtr<FdoGeometricPropertyDefinition> dataPropGeom = FdoGeometricPropertyDefinition::Create(L"realgeometry", L"");
        dataPropGeom->SetGeometryTypes(FdoGeometricType_Point);
        newProps->Add(dataPropGeom);
        newClass->SetGeometryProperty(dataPropGeom);
        classes->Add(newClass);

        // Create the new class:
        FdoPtr<FdoIApplySchema> as = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
        as->SetFeatureSchema(schema);
        as->Execute();

        // Insert some data:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)connection->CreateCommand(FdoCommandType_Insert);
        insert->SetFeatureClassName(L"lowercaseclass");
        FdoPtr<FdoPropertyValueCollection> propVals = insert->GetPropertyValues();
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"x", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(5.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"y", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(5.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"z", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(0.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"id", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(1)))));
        FdoPtr<FdoIFeatureReader> reader = insert->Execute();
        reader->Close();

        propVals->Clear();
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"x", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(-5.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"y", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(-5.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"z", FdoPtr<FdoDoubleValue>(FdoDoubleValue::Create(0.0)))));
        propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"id", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(2)))));
        reader = insert->Execute();
        reader->Close();


        // Create two tables with odd ascii characters (like '+' and '='):
        FdoPtr<FdoISQLCommand> sql = (FdoISQLCommand*)connection->CreateCommand(FdoCommandType_SQLCommand);
        try
        {
            sql->SetSQLStatement(L"drop table \"TESTODDNAME1+\"");
            sql->ExecuteNonQuery();
        }
        catch (FdoException *e)
        {
            e->Release();
        }
        sql->SetSQLStatement(L"create table \"TESTODDNAME1+\" (mynum INT)");
        sql->ExecuteNonQuery();
        try
        {
            sql->SetSQLStatement(L"drop table \"TESTODDNAME2=\"");
            sql->ExecuteNonQuery();
        }
        catch (FdoException *e)
        {
            e->Release();
        }
        sql->SetSQLStatement(L"create table \"TESTODDNAME2=\" (mynum INT)");
        sql->ExecuteNonQuery();


        // drop the metaschema tables:
        wchar_t *metaschemaTables[] = {
            L"f_associationdefinition",
            L"f_attributedefinition",
            L"f_attributedependencies",
            L"f_classdefinition",
            L"f_classtype",
            // L"F_DBOPEN",    // required on Connection::Close()
            L"f_lockname",
            L"f_options",
            L"f_sad",
            L"f_schemainfo",
            L"f_schemaoptions",
            L"f_sequence",
            L"f_spatialcontext",
            L"f_spatialcontextgeom",
            L"f_spatialcontextgroup",
            NULL };
        wchar_t *metaschemaTableName = NULL;
        FdoStringP stmt;
        sql = (FdoISQLCommand*)connection->CreateCommand(FdoCommandType_SQLCommand);
        for (int i=0; ; i++)
        {
            metaschemaTableName = metaschemaTables[i];
            if (!metaschemaTableName)
                break;
            stmt = L"drop table ";
            stmt += metaschemaTableName;
            sql->SetSQLStatement(stmt);
            try
            {
                sql->ExecuteNonQuery();
            }
            catch (FdoException *e)
            {
                e->Release();
            }
        }

        // Disconnect:
        connection->Close();
        connection = NULL;

        // Remember that we created the database successfully:
        m_bDatabaseCreated = true;
    }
}

void FdoSchemaTest::schema ()
{
/*
	bool					bCreated = false;
	FdoIConnection*		connection = NULL;
	FdoRdbmsSchemaManager*	sm = NULL;

	try {
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            "_schema",
            NULL,
			&bCreated
		);

		sm = FdoRdbmsSchemaManager::GetInstance(connection);

		if ( bCreated ) {
			printf( "Loading test data ... \n" );
			LoadTestData(connection, sm );
		}

		printf( "Writing to XML file ... \n" );
		const FdoRdbmsLpSchemaCollection* logPhysSchemas = sm->GetLogicalPhysicalSchemas();
		logPhysSchemas->XMLSerialize( UnitTestUtil::GetOutputFileName( L"schema_test.xml" ) );
		FDO_SAFE_RELEASE( sm );

		printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			true,
            "_schema"
		);

		FDO_SAFE_RELEASE( connection );
	}
	catch ( FdoException* e ) 
	{
		try {
			FDO_SAFE_RELEASE( sm );
			FDO_SAFE_RELEASE( connection );
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
   	catch (...)
   	{
		FDO_SAFE_RELEASE( sm );
		connection->Close();
		FDO_SAFE_RELEASE( connection );
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}

	printf( "Done\n" );

#ifdef SCHEMA_DEBUG
	FdoRdbmsSmBase::Report();
#endif
*/
}

void FdoSchemaTest::UnconnectedTest ()
{
/*
	FdoPtr<FdoRdbmsConnection>		connection;
	FdoPtr<FdoRdbmsSchemaManager>	sm;
	const FdoRdbmsLpSchemaCollection* logPhysSchemas1;
	FdoRdbmsPhysicalSchema* physSchemas1;
	const FdoRdbmsLpSchemaCollection* logPhysSchemas2;
	FdoRdbmsPhysicalSchema* physSchemas2;

	try {
		sm = FdoRdbmsSchemaManager::GetInstance((DbiConnection*) NULL);
		logPhysSchemas1 = sm->GetLogicalPhysicalSchemas();
		physSchemas1 = sm->GetPhysicalSchema();

		connection = FdoRdbmsConnection::Create();
		sm = FdoRdbmsSchemaManager::GetInstance(connection);
		logPhysSchemas2 = sm->GetLogicalPhysicalSchemas();
		physSchemas2 = sm->GetPhysicalSchema();
	}
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception* e )
	{
		// line to stop compiler warning.
		CppUnit::Exception* e2 = e;

		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}

	CPPUNIT_ASSERT( logPhysSchemas1 == NULL );
	CPPUNIT_ASSERT( physSchemas1 == NULL );
	CPPUNIT_ASSERT( logPhysSchemas2 == NULL );
	CPPUNIT_ASSERT( physSchemas2 == NULL );

	printf( "Done\n" );
*/
}

void FdoSchemaTest::OverrideXmlAllTest()
{
    try {
        FdoIoMemoryStreamP outStream = FdoIoMemoryStream::Create();

        // Read from XML file that contains all types of Oracle Schema mapping elements.
        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        mappings->ReadXml( 
            L"SchemaOverrideAll.xml",
            FdoXmlFlagsP( FdoXmlFlags::Create(L"www.autodesk.com", FdoXmlFlags::ErrorLevel_High) )
        );

        mappings->WriteXml( outStream );
        outStream->Reset();

        // Write it out to a second file.
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create( UnitTestUtil::GetOutputFileName( L"SchemaOverrideAll2.xml" ), L"w+t" );
        XmlFormatter formatter(outStream, fileStream);
        formatter.Format();

        // Make sure we didn't lose anything.
		UnitTestUtil::CheckOutput( "SchemaOverrideAll2_master.txt", UnitTestUtil::GetOutputFileName( L"SchemaOverrideAll2.xml" ) );

        // Test Navigating through the RDBMS API.
        FdoRdbmsOvSchemaMappingP mapping = (FdoRdbmsOvPhysicalSchemaMapping*) mappings->GetItem( L"Autodesk.Oracle.3.4", L"Acad " );
    	CPPUNIT_ASSERT( mapping != NULL );
        FdoRdbmsOvReadOnlyClassesP classes = mapping->GetClasses();
        CPPUNIT_ASSERT( classes->GetCount() == 2 );
        FdoRdbmsOvClassP ovClass = classes->FindItem( L"AcDb%Polyline" );
    	CPPUNIT_ASSERT( ovClass != NULL );
        FdoRdbmsOvReadOnlyPropertiesP props = ovClass->GetProperties();
        CPPUNIT_ASSERT( props->GetCount() == 2 );
        FdoRdbmsOvDataPropertyP prop = dynamic_cast<FdoRdbmsOvDataPropertyDefinition*>(props->GetItem(0));
    	CPPUNIT_ASSERT( prop != NULL );
    	FdoRdbmsOvColumnP col = prop->GetColumn();
    	CPPUNIT_ASSERT( col != NULL );
        CPPUNIT_ASSERT( wcscmp(col->GetName(), L"AC_LENGTH") == 0 );
        FdoRdbmsOvObjectPropertyP obProp = dynamic_cast<FdoRdbmsOvObjectPropertyDefinition*>(props->GetItem(1));
    	CPPUNIT_ASSERT( obProp != NULL );
        FdoRdbmsOvPropertyMappingConcreteP obMapping = dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(obProp->GetMappingDefinition());
    	CPPUNIT_ASSERT( obMapping != NULL );
        FdoRdbmsOvTableP tbl = ovClass->GetTable();
    	CPPUNIT_ASSERT( tbl != NULL );

    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
}

void FdoSchemaTest::OverrideXmlErrTest()
{
    try {
        // The following reads in a schema override set that has errors.
        // The errors logged depend on the 
        // error level. All 4 error levels are tested.
        eachOvXmlErr( FdoXmlFlags::ErrorLevel_High, "SchemaOverrideErrHigh_master.txt", UnitTestUtil::GetOutputFileName( L"SchemaOverrideErrHigh.txt" ) );
        eachOvXmlErr( FdoXmlFlags::ErrorLevel_Normal, "SchemaOverrideErrNormal_master.txt", UnitTestUtil::GetOutputFileName( L"SchemaOverrideErrNormal.txt" ) );
        eachOvXmlErr( FdoXmlFlags::ErrorLevel_Low, "SchemaOverrideErrLow_master.txt", UnitTestUtil::GetOutputFileName( L"SchemaOverrideErrLow.txt" ) );
        eachOvXmlErr( FdoXmlFlags::ErrorLevel_VeryLow, "SchemaOverrideErrLow_master.txt", UnitTestUtil::GetOutputFileName( L"SchemaOverrideErrVeryLow.txt" ) );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoSchemaTest::eachOvXmlErr( FdoXmlFlags::ErrorLevel level, const char* masterFile, const char* outFile)
{
    FdoBoolean bFailed = false;

    try {
        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        mappings->ReadXml( 
            L"SchemaOverrideErr.xml",
            FdoXmlFlagsP( FdoXmlFlags::Create(L"www.autodesk.com", level) )
        );
    }
    catch ( FdoException* e ) {

        // No errors are logged at very low level so an exception is a failure.
        if ( level == FdoXmlFlags::ErrorLevel_VeryLow )
            throw;

        UnitTestUtil::PrintException( e, outFile, true );
        bFailed = true;
    }

    if ( level != FdoXmlFlags::ErrorLevel_VeryLow ) { 
        if ( !bFailed  ) 
            CPPUNIT_FAIL( "Loading of SchemaOverrideErr.xml should have failed" );

#ifdef _WIN32
        UnitTestUtil::CheckOutput( masterFile, outFile );
#endif
    }
}

/*
void FdoSchemaTest::LoadTestData(FdoRdbmsConnection* connection, FdoRdbmsSchemaManager* sm)
{
	static char* id_cols[] = { "ID" };
	static char* featid_cols[] = { "FEATID" };
	static char* addrid_cols[] = { "NUM", "STREET", "CITY" };
	static char* addraddid_cols[] = { "NUM", "STREET", "CITY", "PROVINCE" };
	static char* addrinhid_cols[] = { "NUM", "CITY" };
	static char* custid_cols[] = { "NAME", "PROVINCE" };

	UnitTestUtil::Sql2Db( (const wchar_t**) mInputData, connection );

	sm->dbi_key_add( "PK_ACDBENTITY", "ACDBENTITY", id_cols, 1 );
	sm->dbi_key_add( "PK_ACDBLINE", "ACDBLINE", id_cols, 1 );
	sm->dbi_key_add( "PK_CONDUCTOR", "CONDUCTOR", id_cols, 1 );
	sm->dbi_key_add( "PK_TRANSFORMER", "TRANSFORMER", featid_cols, 1 );
	sm->dbi_key_add( "PK_ELECTRICDEVICE_ACDBENTITY", "ELECTRICDEVICE_ACDBENTITY", id_cols, 1 );
	sm->dbi_key_add( "PK_ADDRESS", "ADDRESS", addrid_cols, 3 );
	sm->dbi_key_add( "PK_ADDRESSADDID", "ADDRESSADDID", addraddid_cols, 4 );
	sm->dbi_key_add( "PK_ADDRESSBADID", "ADDRESSBADID", addrid_cols, 3 );
	sm->dbi_key_add( "PK_ADDRESSINHID", "ADDRESSINHID", addrinhid_cols, 2 );
	sm->dbi_key_add( "PK_CUSTOMER", "CUSTOMER", custid_cols, 2 );
	sm->dbi_key_add( "PK_CUSTOMER_HQADDRESSES", "CUSTOMER_HQADDRESSES", addrid_cols, 3 );
}
*/

const wchar_t* FdoSchemaTest::mInputData[] = {
	L"create table acdbentity (",
	L"    ID       NUMBER(10) NOT NULL,",
	L"    LAYER    VARCHAR(255)",
	L");",
	L"",
	L"",
	L"create table acdbline (",
	L"    ID       NUMBER(10) NOT NULL,",
	L"    LAYER    VARCHAR(255)",
	L");",
	L"",
	L"create table acdb3dpolyline (",
	L"    ID       NUMBER(10) NOT NULL,",
	L"    LAYER    VARCHAR(255)",
	L");",
	L"",
	L"create table acdbblockattribute (",
	L"    ID       NUMBER(10) NOT NULL",
	L");",
	L"",
	L"create table acdbentity_acxdata (",
	L"    ACDBENTITY_ID       NUMBER(10) NOT NULL,",
	L"    SEQ                 NUMBER(10) NOT NULL,",
	L"    DATAVALUE           VARCHAR(4000) NOT NULL",
	L");",
	L"",
	L"create table acdbhatch (",
	L"    ID       NUMBER(10) NOT NULL",
	L");",
	L"",
	L"create table acdbtext (",
	L"    ID       NUMBER(10) NOT NULL",
	L");",
	L"",
	L"create table aeccdbalignment (",
	L"    ID       NUMBER(10) NOT NULL",
	L");",
	L"",
	L"create table aeccdbsurface (",
	L"    ID       NUMBER(10) NOT NULL",
	L");",
	L"",
	L"create table conductor (",
	L"    ID       NUMBER(10) NOT NULL,",
	L"    COST     NUMBER(10,2) NOT NULL",
	L");",
	L"",
	L"create table transformer (",
	L"    FEATID     NUMBER(20) NOT NULL,",
	L"    PHASE        CHAR(1),",
	L"    VOLTAGE      NUMBER",
	L");",
	L"",
	L"create table electricdevice_acdbentity (",
	L"    ELECTRICDEVICE_ID       NUMBER(10) NOT NULL,",
	L"    ID                      NUMBER(10) NOT NULL,",
	L"    LAYER                   VARCHAR(255)",
	L");",
	L"",
	L"create table address (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100)",
	L");",
	L"",
	L"create table addressaddid (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100),",
	L"    PROVINCE                VARCHAR(50) NOT NULL",
	L");",
	L"",
	L"create table addressbadid (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100)",
	L");",
	L"",
	L"create table addressinhid (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(50)",
	L");",
	L"",
	L"create table customer (",
	L"    NAME                    VARCHAR(100) NOT NULL,",
	L"    PROVINCE                VARCHAR(50) NOT NULL",
	L");",
	L"",
	L"create table customer_hqaddresses (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100),",
	L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
	L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
	L");",
	L"",
	L"create table customer_retailaddresses (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100),",
	L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
	L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
	L");",
	L"",
	L"create table customer_factoryaddresses (",
	L"    NUM                     NUMBER(5) NOT NULL, ",
	L"    STREET                  VARCHAR(50) NOT NULL, ",
	L"    CITY                    VARCHAR(50) NOT NULL,",
	L"    OWNER                   VARCHAR(100),",
	L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
	L"    SEQ                     NUMBER(10) NOT NULL,",
	L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
	L");",
	L"",
	L"delete from f_schemainfo where schemaname = 'Acad';",
	L"",
	L"insert into f_schemainfo ( SCHEMANAME, DESCRIPTION, CREATIONDATE, OWNER, ",
	L"SCHEMAVERSIONID ) values ( 'Acad', null, '03-10-07', 'br', 1 );                          ",
	L"                                                                                ",
	L"insert into f_schemainfo ( SCHEMANAME, DESCRIPTION, CREATIONDATE, OWNER, ",
	L"SCHEMAVERSIONID ) values ( 'Electric', 'Electric Utility Model', '03-10-07', 'br', 1 );   ",
	L"                                                                                                                                                                ",
	L"insert into f_schemainfo ( SCHEMANAME, DESCRIPTION, CREATIONDATE, OWNER, ",
	L"SCHEMAVERSIONID ) values ( 'Vine', 'Vine schema', '03-10-16', 'br', 1 );               ",
	L"                                                                                ",
	L"insert into f_schemainfo ( SCHEMANAME, DESCRIPTION, CREATIONDATE, OWNER, ",
	L"SCHEMAVERSIONID ) values ( 'Error', 'Test schema errors', '03-10-16', 'br', 1 );       ",
	L"                                                                                ",
	L"insert into f_schemainfo ( SCHEMANAME, DESCRIPTION, CREATIONDATE, OWNER, ",
	L"SCHEMAVERSIONID ) values ( 'Postal', 'more error tests', '03-10-17', 'br', 1 ); ",
	L"",
	L"delete from f_classdefinition where schemaname = 'Acad';",
	L"",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (101, 'AcDbEntity', ",
	L"'Acad', 'AcDbEntity', 2, null, 0, null );                                                             ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (102, 'AcDbLine', ",
	L"'Acad', 'AcDbLine', 2, null, 0, 'AcDbEntity' );                                                     ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (103, 'ElectricDevice', ",
	L"'Electric', 'ElectricDevice', 2, null, 1, null );                                                     ",
	L"",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (104, 'Conductor', ",
	L"'Electric', 'Conductor', 2, 'Electrical Conductor', 0, 'ElectricDevice' );                        ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (105, 'Transformer', ",
	L"'Electric', 'Transformer', 2, null, 0, 'ElectricDevice' );                                          ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (106, 'AcXData', ",
	L"'Acad', 'AcXData', 1, null, 0, null );                                                                ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (107, 'AbstractText', ",
	L"'Acad', 'AcDbText', 2, 'text base class', 1, 'AcDbEntity' );                                  ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (108, 'AcDb3dPolyline', ",
	L"'Acad', 'AcDb3dPolyline', 2, '3d polyline', 0, 'Acad:AcDbEntity' );                               ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (109, ",
	L"'AcDbBlockAttribute', 'Acad', 'AcDbBlockAttribute', 2, null, 0, 'AcDbEntity' );                                           ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (110, 'AcDbHatch', ",
	L"'Acad', 'AcDbHatch', 2, 'a hatched polygon', 0, 'AcDbEntity' );                                  ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (111, 'AcDbMText', ",
	L"'Acad', 'AcDbText', 2, 'multiline text', 0, 'AbstractText' );                                   ",
	L"                                                                                ",
	L"",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (112, 'AcDbText', ",
	L"'Acad', 'AcDbText', 2, 'single line text', 0, 'AbstractText' );                                  ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (113, ",
	L"'AeccDbAlignment', 'Vine', 'AeccDbAlignment', 2, null, 0, 'Acad:AcDbEntity' );                                        ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (114, 'AeccDbSurface', ",
	L"'Vine', 'AeccDbSurface', 2, null, 0, 'Acad:AcDbEntity' );                                          ",
	L"",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (115, 'BadTable', ",
	L"'Error', 'BadTable', 1, 'Table does not exist', 0, null );                                         ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (116, 'LoopA', 'Error', ",
	L"'LoopA', 1, null, 0, 'LoopC' );                                                           ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (117, 'LoopB', 'Error', ",
	L"'LoopB', 1, null, 0, 'LoopA' );                                                           ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (118, 'LoopC', ",
	L"'Error', 'LoopC', 1, null, 0, 'LoopB' );                                                           ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (119, 'LoopD', ",
	L"'Error', 'LoopD', 1, null, 0, 'LoopC' );                                                           ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (120, 'BadParent', ",
	L"'Error', 'BadParent', 1, 'parent class does not exist', 0, 'NoClass' );                          ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (122, 'AddressAddId', ",
	L"'Postal', 'AddressAddId', 1, null, 0, 'Address' );                                                 ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (121, 'Address', ",
	L"'Postal', 'Address', 1, null, 1, null );                                                             ",
	L"                                                                                ",
	L"",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (123, 'AddressBadId', ",
	L"'Postal', 'AddressBadId', 1, null, 0, null );                                                        ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (124, 'AddressInhId', ",
	L"'Postal', 'AddressInhId', 1, null, 0, 'Address' );                                                 ",
	L"                                                                                ",
	L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
	L"classtype, description, isabstract, parentclassname ) values (125, 'Customer', ",
	L"'Postal', 'Customer', 1, null, 0, null );                                                            ",
	L"",
	L"delete from f_attributedefinition where classid >= 100;       ",
	L"                                                                         ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
	L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity', 101, 'layer', ",
	L"'layer', 'string', 255, 0, 'string', 1, 0, 0, 0, null, null, 'Autocad layer' );      ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'Transformer', ",
	L"105, 'PHASE', 'phase', 'string', 1, 0, 'string', 1, 0, 0, 0, null, null, ",
	L"'an electrical term' );  ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, ",
	L"isnullable, isfeatid, issystem, isreadonly, sequencename, owner, description ) values ",
	L"( 'Transformer', 105, 'VOLTAGE', 'voltage', 'double', 0, 0, 'double', 1, 0, 0, 0, ",
	L"null, null, 'number of volts' ); ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'Conductor', ",
	L"104, 'Cost', 'Cost', 'NUMBER', 10, 2, 'decimal', 1, 0, 0, 0, null, null, ",
	L"'Installation Cost' );     ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( ",
	L"'ElectricDevice_AcDbEntity', 103, 'graphic', 'graphic', 'Acad:AcDbEntity', 0, 0, ",
	L"'Acad:AcDbEntity', 1, 0, 0, 0, null, null, null );                                                                ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
	L"isreadonly, sequencename, owner, description ) values ( 'AcXData', 106, 'datavalue', ",
	L"'DataValue', 'VARCHAR2', 4000, 0, 'string', 1, 0, 0, 0, null, null, null );           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDb3dPolyline', ",
	L"108, 'layer', 'layer', 'string', 255, 0, 'string', 1, 0, 0, 0, null, null, ",
	L"'Autocad layer' );  ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem,",
	L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', 101, ",
	L"'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', ",
	L"102, 'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( ",
	L"'AcDbEntity_AcXData', 102, 'acdbentity_id', 'xdata.acdbentity_id', 'NUMBER', 10, 0, ",
	L"'int32', 1, 1, 0, 0, null, null, null );                                                                       ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
	L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', 102, ",
	L"'seq', 'xdata.seq', 'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData',",
	L"102, 'datavalue', 'xdata.datavalue', 'VARCHAR2', 4000, 0, 'string', 1, 0, 0, 0, ",
	L"null, null, null );                                                                          ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', ",
	L"101, 'seq', 'xdata.seq', 'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AcDbEntity', 101, ",
	L"'id', 'Id', 'NUMBER', 10, 0, 'int32', 1, 1, 0, 0, null, null, null, 1 );                           ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
	L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
	L"only, sequencename, owner, description ) values ( 'AcDbXData', 106, 'seq', 'seq', ",
	L"'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );                          ",
	L"",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
	L"'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                         ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
	L"'street', 'Street', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );                  ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
	L"'city', 'City', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                      ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId',",
	L" 122, 'city', 'City', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                 ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId', 122, ",
	L"'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                    ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
	L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
	L"only, sequencename, owner, description, idposition ) values ( 'AddressAddId', 122, 'street', ",
	L"'Street', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );             ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
	L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
	L"only, sequencename, owner, description, idposition ) values ( 'AddressBadId', 123, 'city', 'C",
	L"ity', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                 ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressBadId', ",
	L"123, 'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                    ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Customer', 125, ",
	L"'Province', 'Province', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );             ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId', ",
	L"122, 'province', 'Province', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 4 );         ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'AddressBadId', ",
	L"123, 'AddressType', 'AddressType', 'CHAR', 1, 0, 'string', 1, 0, 0, 1, null, null, null );        ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
	L"isreadonly, sequencename, owner, description ) values ( 'Transformer_AcXData', 105, ",
	L"'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );          ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Customer', ",
	L"125, 'Name', 'Name', 'VARCHAR2', 100, 0, 'string', 1, 0, 0, 0, null, null, null, 1 );                    ",
	L"",
	L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
	L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
	L"only, sequencename, owner, description ) values ( 'Customer_HqAddresses', 125, 'H",
	L"qAddresses', 'HqAddresses', 'Address', 0, 0, 'Address', 1, 0, 0, 0, null, null, '",
	L"' );                                                                            ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
	L"isreadonly, sequencename, owner, description ) values ( 'Customer_RetailAddresses', ",
	L"125, 'RetailAddresses', 'RetailAddresses', 'Address', 0, 0, 'Address', 1, 0, ",
	L"0, 0, null, null, null );                                                                ",
	L"",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( ",
	L"'Customer_FactoryAddresses', 125, 'FactoryAddresses', 'FactoryAddresses', 'Address', ",
	L"0, 0, 'Address', 1, 0, 0, 0, null, null, null );                                                             ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( ",
	L"'Customer_StorageAddresses', 125, 'storageAddresses', 'storageAddresses', 'Address', ",
	L"0, 0, 'Address', 1, 0, 0, 0, null, null, null );                                                             ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'Transformer', ",
	L"105, 'EntityId', 'EntityId', 'NUMBER', 38, 8, 'double', 1, 0, 0, 0, null, null, null );             ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0', ",
	L"103, 'RDBMS_GEOM', 'Geometry', 'SDO_GEOMETRY', 0, 0, '2', 1, 0, 0, ",
	L"0, null, null, '' );                                                                            ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, ",
	L"isnullable, isfeatid, issystem, isreadonly, sequencename, owner, description ) values ",
	L"( 'F_GEOMETRY_0', 109, 'RDBMS_GEOM', 'InsertPoint', 'SDO_GEOMETRY', 0, 0, '1', ",
	L"1, 0, 0, 0, null, null, null );   ",
	L"",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Conductor', ' ', 'ElectricDevice_AcDbEntity', ' ', 1, '', 'a');                                                                                                           ",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'AcDbEntity', 'Id', 'AcDbEntity_AcXData', 'AcDbEntity_id', -1, 'seq', 'a');                                                                                                ",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'AcDbLine', 'Id', 'AcDbEntity_AcXData', 'AcDbEntity_id', -1, 'seq', 'a');                                                                                                  ",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'Name, Province', 'Customer_HqAddresses', 'Customer_Name, Customer_Province', -1, '', 'a');                                                                    ",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'NoName', 'Customer_RetailAddresses', 'Customer_Name', -1, 'seq', 'a');                                                                                        ",
	L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'Name', 'Customer_FactoryAddresses', 'Customer_NoName', -1, 'seq', 'a');                                                                                       ",
	L"",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0',",
	L"107, 'RDBMS_GEOM', 'InsertPoint', 'SDO_GEOMETRY', 0, 0, '1', 1, 0, 0, 0, null,",
	L"null, null );   ",
	L"                                                                                ",
	L"insert into f_attributedefinition ( tablename, classid, columnname, ",
	L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
	L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0', ",
	L"107, 'C_1', 'Position', 'SDO_GEOMETRY', 0, 0, '4', 1, 0, 0, 0, null, null, null );",
	L"",
	L"delete from f_sad; ",
	L"                                                                                ",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'Electric', 'schema', 'Author', 'Thomas Edison' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'Electric', 'schema', 'Sharable', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Vine', 'Vine', 'schema', 'Author', 'Tarzan' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad', 'AbstractText', 'class', 'Entity', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad', 'AbstractText', 'class', 'Text', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad', 'AbstractText', 'class', 'Encoding', 'ASCII' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad', 'AcDbEntity', 'class', 'Entity', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'Conductor', 'class', 'Construction Template', 'linear' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'ElectricDevice', 'class', 'Construction Template', 'general' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'ElectricDevice', 'class', 'Maintenance Template', 'standard' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric', 'Transformer', 'class', 'Construction Template', 'point' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad:AcDbEntity', 'layer', 'property', 'Classified', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad:AcDbEntity', 'xdata', 'property', 'LinkList', 'Yes' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Acad:AcDbEntity', 'xdata', 'property', 'Ordered', 'No' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric:Conductor', 'Cost', 'property', 'Copper', '5' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric:Conductor', 'Cost', 'property', 'Aluminum', '3' ); ",
	L"",
	L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
	L" values ( 'Electric:Transformer', 'voltage', 'property', 'Formula', 'Ohms Law' ); ",
	L"",
	NULL
};

void FdoSchemaTest::OverrideTablesWithOddAsciiChars()
{
    try {
        CreateFreshDb();


        // Create overrides objects to match these 2 tables:
        FdoPtr<FdoFeatureSchema> ovSchema = FdoFeatureSchema::Create(L"OvSchema", L"");
        FdoPtr<FdoClassCollection> ovClasses = ovSchema->GetClasses();
        FdoPtr<FdoClass> ovClass = FdoClass::Create(L"TESTODDNAME1+", L"");
        FdoPtr<FdoDataPropertyDefinitionCollection> ovIdProps = ovClass ->GetIdentityProperties();
        FdoPtr<FdoPropertyDefinitionCollection> ovProps = ovClass->GetProperties();
        FdoPtr<FdoDataPropertyDefinition> ovProp = FdoDataPropertyDefinition::Create(L"NUMBER", L"");
        ovProp->SetDataType(FdoDataType_Int32);
        ovProps->Add(ovProp);
        ovIdProps->Add(ovProp);
        ovClasses->Add(ovClass);

        ovClass = FdoClass::Create(L"TESTODDNAME2=", L"");
        ovProps = ovClass->GetProperties();
        ovIdProps = ovClass ->GetIdentityProperties();
        ovProp = FdoDataPropertyDefinition::Create(L"NUMBER", L"");
        ovProp->SetDataType(FdoDataType_Int32);
        ovProps->Add(ovProp);
        ovIdProps->Add(ovProp);
        ovClasses->Add(ovClass);

        FdoPtr<FdoIoMemoryStream> xmlMemStream = FdoIoMemoryStream::Create();
        ovSchema->WriteXml(xmlMemStream);
        xmlMemStream->Reset();

        // Connect with configuration file:
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_SUFFIX));
        connection->SetConfiguration(xmlMemStream);
        connection->Open();

        // Do a DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoSchemaTest::Test_Defect802102()
{
    try
    {
        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        mappings->ReadXml( L"tm_802102.xml" );
        mappings->WriteXml( L"tm_802102_2.xml" );
        mappings->Clear();
        mappings->ReadXml( L"tm_802102_2.xml" );
        mappings->WriteXml( L"tm_802102_3.xml" );
        UnitTestUtil::CheckOutput( "tm_802102_3.xml", "tm_802102_2.xml" );
	}
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
}
