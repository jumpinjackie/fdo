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
#include "FdoImportExportTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"

#define   DB_NAME_SRC_SUFFIX        "_impexp_src"
#define   DB_NAME_DEST_SUFFIX       "_impexp_dest"

FdoImportExportTest::FdoImportExportTest(void)
{
}

FdoImportExportTest::~FdoImportExportTest(void)
{
}

void FdoImportExportTest::setUp ()
{
    set_provider();
}

void FdoImportExportTest::tearDown ()
{
}

// The following stylesheet sorts the output XML files so that the Spatial 
// Contexts, Schemas and Schema Mappings are always in the same order. This
// allows the XML files to be compared against corresponding masters, without
// element ordering differences triggering false test failures. 

char* FdoImportExportTest::pSortSheet = 
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
<xsl:template match=\"fdo:DataStore\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"gml:DerivedCRS\">\
            <xsl:sort select=\"@gml:id\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"xs:schema\">\
            <xsl:sort select=\"@targetNamespace\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"node()[local-name()='SchemaMapping']\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"node()[local-name()='FeatureCollection']\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"ora:SchemaMapping|mql:SchemaMapping|sqs:SchemaMapping\">\
    <xsl:element name=\"SchemaMapping\" namespace=\"{namespace::node()[name()='']}\">\
		<xsl:for-each select=\"namespace::node()[not(name()='')]\">\
			<xsl:copy/>\
		</xsl:for-each>\
        <xsl:apply-templates select=\"@*|node()\"/>\
    </xsl:element>\
</xsl:template>\
<xsl:template match=\"gml:FeatureColection\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"gml:featureMember\">\
            <xsl:sort select=\"ClassB1/Prop1\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"gml:coordinates\">\
    <xsl:element name=\"gml:coordinates\" >\
        <xsl:variable name=\"coords\" select=\"text()\"/>\
        <xsl:value-of select=\"$coords\"/>\
        <xsl:if test=\"not(contains(substring-after($coords,','),','))\" >,0.000000</xsl:if>\
    </xsl:element>\
</xsl:template>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
  </xsl:template>\
</stylesheet>";

FdoPropertyValue* FdoImportExportTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;
    propertyValue = propertyValues->FindItem( name );

    if ( propertyValue == NULL )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }

    return propertyValue;
}

void FdoImportExportTest::Test1 ()
{
    try {
        FdoIoStreamP configStream = FdoIoMemoryStream::Create();
        FdoIoStreamP streamA = FdoIoMemoryStream::Create();
        FdoIoStreamP streamB = FdoIoMemoryStream::Create();

        // Create the configuration document
        CreateConfig1( configStream );

        // Test doing an add-only read from the document. This will fail since the 
        // document contains spatial context Default, which is already in the DataStore/
        printf( "Test1 (Add only, include default) Should fail importing Default ... \n" );

        FdoXmlSpatialContextFlagsP flags = 
            FdoXmlSpatialContextFlags::Create(
                L"www.failure.com",
                FdoXmlFlags::ErrorLevel_High,
                true,
                FdoXmlSpatialContextFlags::ConflictOption_Add,
                true
            );

        FdoBoolean succeeded = false;
	    try {
            DoTest( configStream, streamA, streamB, flags, DB_NAME_SRC_SUFFIX );
		    succeeded = true;
	    }
	    catch ( FdoException* e )
	    {
		    UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"impexp_err1.txt" ), true);
		    FDO_SAFE_RELEASE(e);
	    }

	    CPPUNIT_ASSERT( !succeeded );

#ifdef _WIN32
		UnitTestUtil::CheckOutput( "impexp_err1_master.txt", UnitTestUtil::GetOutputFileName( L"impexp_err1.txt" ) );
#endif

    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
		
	printf( "Done\n" );

}

void FdoImportExportTest::Test2_3_4 ()
{
    try {
        FdoIoStreamP configStream1 = FdoIoMemoryStream::Create();
        FdoIoStreamP configStream2 = FdoIoMemoryStream::Create();
        FdoIoStreamP stream2A = FdoIoMemoryStream::Create();
        FdoIoStreamP stream2B = FdoIoMemoryStream::Create();
        FdoIoStreamP stream3A = FdoIoMemoryStream::Create();
        FdoIoStreamP stream3B = FdoIoMemoryStream::Create();
        FdoIoStreamP stream4A = FdoIoMemoryStream::Create();
        FdoIoStreamP stream4B = FdoIoMemoryStream::Create();

        CreateConfig1( configStream1 );
        CreateConfig2( configStream2 );

        // Test 2 is the same as Test 1 except that the flags specify to ignore
        // the default spatial context (Default). Therefore Test 2 should succeed
        // since it's other spatial contexts are not in the DataStore.
        printf( "Test2 (exclude default SC) Should succeed ... \n" );

        FdoXmlSpatialContextFlagsP flags = 
            FdoXmlSpatialContextFlags::Create(
                L"www.failure.com",
                FdoXmlFlags::ErrorLevel_High,
                true,
                FdoXmlSpatialContextFlags::ConflictOption_Add,
                false
            );

        DoTest( configStream1, stream2A, stream2B, flags, DB_NAME_SRC_SUFFIX );
        Stream2SortedFile( stream2A, UnitTestUtil::GetOutputFileName( L"impexp2a.xml" ) );
        Stream2SortedFile( stream2B, UnitTestUtil::GetOutputFileName( L"impexp2b.xml" ) );

/* TODO: activate test 3 when update spatial context supported  
        // Test 3 is also the same as Test 1 except that the flags allow spatial
        // contexts to be updated. Therefore the test succeeds even though Default
        // is already in the datastore.
        printf( "Test3 (include default SC, update SC's) Should succeed ... \n" );

        flags->SetConflictOption( FdoXmlSpatialContextFlags::ConflictOption_Update );
        flags->SetIncludeDefault( true );

        DoTest( configStream2, stream3A, stream3B, flags, DB_NAME_SRC_SUFFIX, false, true, false  );

        Stream2SortedFile( stream3A, UnitTestUtil::GetOutputFileName( L"impexp3a.xml" ) );
        Stream2SortedFile( stream3B, UnitTestUtil::GetOutputFileName( L"impexp3b.xml" ) );
*/
        // Test 4 verifies that we can copy spatial contexts and feature schemas from
        // one datastore to another. It copies everything except the default spatial
        // context.
        printf( "Test4 (export from one DataSet; import into another) Should succeed ... \n" );
// Test 4 depends on Test3, for now feed in test 2a result.
//        DoTest( stream3A, stream4A, stream4B, NULL, DB_NAME_DEST_SUFFIX );
        DoTest( stream2A, stream4A, stream4B, NULL, DB_NAME_DEST_SUFFIX );
/*
        Stream2SortedFile( stream4A, UnitTestUtil::GetOutputFileName( L"impexp4a.xml" ) );
        Stream2SortedFile( stream4B, UnitTestUtil::GetOutputFileName( L"impexp4b.xml" ) );
*/
        // Verify all the generated config files.
#ifdef _WIN32
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2a_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp2a.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2b_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp2b.xml" ) );
/* TODO: activate test 3 when update spatial context supported
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp3a_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp3a.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp3b_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp3b.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp3a_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp4a.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp4b_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp4b.xml" ) );
*/
#endif

    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
		
	printf( "Done\n" );

}

void FdoImportExportTest::Test5_6_7 ()
{
    try {
        FdoIoStreamP configStream = FdoIoMemoryStream::Create();
        FdoIoStreamP streamA = FdoIoMemoryStream::Create();
        FdoIoStreamP streamB = FdoIoMemoryStream::Create();

        CreateConfig1( configStream );

        // Test5 is also similar to Test1. It tests the skipping of spatial contexts
        // already in the datastore. Therefore it succeeds, since Default will be skipped
        // without error.
        printf( "Test5 (include default SC: skip updates) Should succeed ... \n" );

        FdoXmlSpatialContextFlagsP flags = 
            FdoXmlSpatialContextFlags::Create(
                L"www.fdo.rdbms.com",
                FdoXmlFlags::ErrorLevel_High,
                true,
                FdoXmlSpatialContextFlags::ConflictOption_Skip,
                true
            );

        DoTest( configStream, streamA, streamB, flags, DB_NAME_SRC_SUFFIX );

        Stream2SortedFile( streamA, UnitTestUtil::GetOutputFileName( L"impexp5a.xml" ) );
        Stream2SortedFile( streamB, UnitTestUtil::GetOutputFileName( L"impexp5b.xml" ) );

        // Unlike Tests 1-5, Test6 does not re-create the datastore. It tests the 
        // skipping of non-default spatial contexts that are already in the database.
        // Note that this test skips the feature schemas, since the Oracle Provider
        // does not fully support merging pre-existing schemas yet.
        printf( "Test6 (retry skip updates to test skipping non-default SC.) Should succeed ... \n" );

        DoTest( configStream, streamA, streamB, flags, DB_NAME_SRC_SUFFIX, false, true, false );

        Stream2SortedFile( streamA, UnitTestUtil::GetOutputFileName( L"impexp6a.xml" ) );
        Stream2SortedFile( streamB, UnitTestUtil::GetOutputFileName( L"impexp6b.xml" ) );

        // Test 7 is similar to Test 8, except that attempts to update spatial contexts
        // generate an error. It tests generating an error for a pre-existing 
        // non-default spatial context.
        printf( "Test7 (retry with Add-Only flag) Should fail ... \n" );

        flags->SetConflictOption( FdoXmlSpatialContextFlags::ConflictOption_Add );
        // Skip the default spatial context, so that we guarantee failure on an 
        // non-default spatial context.
        flags->SetIncludeDefault( false );

        FdoBoolean succeeded = false;
	    try {
            DoTest( configStream, streamA, streamB, flags, DB_NAME_SRC_SUFFIX, false, true, false );
		    succeeded = true;
	    }
	    catch ( FdoException* e )
	    {
		    UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"impexp_err7.txt" ), true);
		    FDO_SAFE_RELEASE(e);
	    }

	    CPPUNIT_ASSERT( !succeeded );

        // Verify that we got the right config files and errors.
#ifdef _WIN32
        UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2a_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp5a.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2b_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp5b.xml" ) );
        UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2a_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp6a.xml" ) );
		UnitTestUtil::CheckOutput( (const char*)OverrideBend(L"impexp2b_master.txt"), UnitTestUtil::GetOutputFileName( L"impexp6b.xml" ) );
		UnitTestUtil::CheckOutput( "impexp_err7_master.txt", UnitTestUtil::GetOutputFileName( L"impexp_err7.txt" ) );
#endif

    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
		
	printf( "Done\n" );

}

void FdoImportExportTest::Test8 ()
{
    try {
        FdoIoStreamP configStream = FdoIoMemoryStream::Create();

        CreateConfigBadWkt( configStream );

        // Test 8 tries adding a spatial context whose WKT differs
        // from what Oracle defines for its coordinate system.
        // This test should fail for Oracle and pass for the others.
        printf( "Test8 (junk WKT) ... \n" );

        printf( "\tOpening Source Connection ... \n" );
        FdoPtr<FdoIConnection> connection = UnitTestUtil::CreateConnection(
		    true,
		    true,
            DB_NAME_SRC_SUFFIX,
            NULL,
            NULL,
            0
	    );

        FdoPtr<FdoIConnectionCapabilities> cap = connection->GetConnectionCapabilities();

        // Import from the input XML document
        printf( "\tImporting ...\n" );

        Import(
            connection, 
            configStream,
            NULL,
            true,
            false
        );

        FdoPtr<FdoIGetSpatialContexts> cmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
        cmd->SetActiveOnly(false);

        FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();

        while ( reader->ReadNext() ) {
            // Skip default spatial context if not writing it.
            if ( wcscmp(reader->GetName(), L"Manhole1") == 0 ) {
                if ( cap->SupportsCSysWKTFromCSysName() )
                    CPPUNIT_ASSERT( wcscmp(reader->GetCoordinateSystemWkt(), L"LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]") == 0 );
                else
                    CPPUNIT_ASSERT( wcscmp(reader->GetCoordinateSystemWkt(), L"nonsense wkt") == 0 );
            }
        }
    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
    }

	printf( "Done\n" );
}

// The following runs a single test.
void FdoImportExportTest::DoTest( 
    FdoIoStream* inStream,              // import from this XML document
    FdoIoStream* outStream1,            // export to this XML document (excludes default spatial context) 
    FdoIoStream* outStream2,            // export to this XML document (excludes default spatial context)
    FdoXmlSpatialContextFlags* flags,   // spatial context serialization flags
    char* dbName,                       // Datastore to import into and export from
    FdoBoolean createDB,                // true - re-create database before running test
    FdoBoolean importSC,                // true - import spatial contexts
    FdoBoolean importSchemas            // true - import feature schemas.
)
{
    printf( "\tOpening Source Connection ... \n" );
    FdoPtr<FdoIConnection> connection = UnitTestUtil::CreateConnection(
		createDB,
		createDB,
        dbName,
        NULL,
        NULL,
        0
	);

    // Import from the input XML document
    printf( "\tImporting ...\n" );

    Import(
        connection, 
        inStream,
        flags,
        importSC,
        importSchemas
    );

    // Test export that excludes default spatial context
    printf( "\tExporting without default context ...\n" );
    Export(
        connection,
        outStream1,
        NULL
    );

    FdoXmlSpatialContextFlagsP expFlags = 
        FdoXmlSpatialContextFlags::Create(
            L"www.fdo.rdbms.com",
            FdoXmlFlags::ErrorLevel_High,
            true,
            FdoXmlSpatialContextFlags::ConflictOption_Skip,
            true
        );

    expFlags->SetIncludeDefault(true);

    // Test export that includes default spatial context
    printf( "\tExporting with default context ...\n" );
try {
    Export(
        connection,
        outStream2,
        expFlags
    );
}
catch ( ... ) {}

    UnitTestUtil::CloseConnection(
	    connection,
		false,
        dbName
    );
}

// Exports spatial contexts and feature schemas from datastore to XML.
void FdoImportExportTest::Export( FdoIConnection* connection, FdoIoStream* stream, FdoXmlSpatialContextFlags* flags )
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream);

    // Serialize the spatial contexts
    FdoXmlSpatialContextSerializer::XmlSerialize( 
        connection,
        FdoXmlSpatialContextWriterP(
            FdoXmlSpatialContextWriter::Create(writer,flags)
        ),
        flags
    );

    // Get the feature schemas from the Datastore
    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoFeatureSchemasP schemas = pDescCmd->Execute();

    // Serialize the feature schemas.
    schemas->WriteXml( writer );

    // Get the feature schema overrides from the Datastore
    FdoPtr<FdoIDescribeSchemaMapping>  pDescMappingsCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
    FdoSchemaMappingsP mappings = pDescMappingsCmd->Execute();

    // Serialize the feature schemas.
    mappings->WriteXml( writer );
#if 1
    FdoFeatureSchemaP selSchema = schemas->FindItem( L"Schema1" );

    if ( selSchema ) {
        FdoClassDefinitionP selClass = FdoClassesP(selSchema->GetClasses())->FindItem( L"ClassB1" );

        if ( selClass ) {
            FdoPtr<FdoISelect> selCmd = (FdoISelect*) connection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(selClass->GetQualifiedName());
            FdoPtr<FdoIFeatureReader> rdr = selCmd->Execute();

            FdoXmlFeatureWriterP featureWriter = FdoXmlFeatureWriter::Create(writer);
            FdoXmlFeatureFlagsP featureFlags = FdoXmlFeatureFlags::Create();
            FdoXmlFeatureSerializer::XmlSerialize( rdr, featureWriter, featureFlags );
        }
    }
#endif
}

// Imports spatial contexts and feature schemas from XML to datastore.
void FdoImportExportTest::Import( 
    FdoIConnection* connection, 
    FdoIoStream* stream, 
    FdoXmlSpatialContextFlags* flags,
    FdoBoolean importSC,
    FdoBoolean importSchemas
)
{
    FdoPtr<FdoITransaction> featureTransaction;

	stream->Reset();

    // Import the Spatial Contexts
    if ( importSC ) {
        FdoXmlSpatialContextSerializer::XmlDeserialize( 
            connection,
            FdoXmlSpatialContextReaderP(
                FdoXmlSpatialContextReader::Create(
                    FdoXmlReaderP(
                        FdoXmlReader::Create(stream)
                    ),
                    flags
                )
            ),
            flags
        );

        stream->Reset();
    }

    // Import the Schema Override sets.

    FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
    stream->Reset();
    mappings->ReadXml( stream );

    // Import the Feature Schemas
    if ( importSchemas ) {
        FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
        // Deserialize the feature schemas
        stream->Reset();
        schemas->ReadXml( stream );

        // Add each feature schema to the datastore.
  	    FdoPtr<FdoIApplySchema>  applyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        for ( int idx = 0; idx < schemas->GetCount(); idx++ ) {
            FdoFeatureSchemaP schema = schemas->GetItem(idx);
            applyCmd->SetFeatureSchema( schema );

            FdoPhysicalSchemaMappingP overrides = 
                (FdoPhysicalSchemaMapping*) mappings->GetItem( connection, schema->GetName() );
            if ( overrides )
                applyCmd->SetPhysicalMapping( overrides );

            applyCmd->Execute();
        }
        FdoFeatureSchemaP insSchema = schemas->FindItem( L"Schema1" );

        if ( insSchema ) {
            FdoClassDefinitionP insClass = FdoClassesP(insSchema->GetClasses())->FindItem( L"ClassB1" );

            if ( insClass ) {

                featureTransaction = connection->BeginTransaction();

                FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
                insertCommand->SetFeatureClassName(insClass->GetQualifiedName());

                AddFeature( insertCommand, 1 );
                AddFeature( insertCommand, 2 );
                AddFeature( insertCommand, 3 );
                featureTransaction->Commit();
            }
        }
    }
}

// Creates a config file with 2 spatial contexts ( 1 default and 1 non-default )
// and 2 feature schemas.
// The spatial contexts and feature schemas are interleaved to ensure that they can 
// be deserialized successfully regardless of their order.
void FdoImportExportTest::CreateConfig1( FdoIoStream* stream )
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream);
    AddSC( writer, L"Default", L"default context", L"Non-Earth (Meter)", GetWKT(L"Non-Earth (Meter)"), 1000 );
    AddSchema( writer, 1 );
    AddSC( writer, L"Manhole1", L"diagram", L"Non-Earth (Meter)", GetWKT(L"Non-Earth (Meter)"), 1 );
    AddSchema( writer, 2 );
    AddMapping( writer, L"Schema1", 3 );
//    AddMapping( writer, L"Schema2", 1 );
    AddMapping( writer, L"Schema3", 2 );
    writer = NULL;

    Stream2File( stream, UnitTestUtil::GetOutputFileName( L"impexp_config1.xml" ) );
}

// Creates a config file similar to the previous one, but with an extra spatial
// context and feature schema. 
void FdoImportExportTest::CreateConfig2( FdoIoStream* stream )
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream);
    AddSC( writer, L"Default", L"default context", L"Non-Earth (Meter)", GetWKT(L"Non-Earth (Meter)"), 2000 );
    AddSchema( writer, 1 );
    AddSC( writer, L"Manhole1", L"diagram", L"Non-Earth (Meter)", GetWKT(L"Non-Earth (Meter)"), 3 );
    AddSchema( writer, 2 );
    AddSC( writer, L"Manhole2", L"diagram", L"Non-Earth (Meter)", GetWKT(L"Non-Earth (Meter)"), 2 );
    AddSchema( writer, 3 );
    writer = NULL;

    Stream2File( stream, UnitTestUtil::GetOutputFileName( L"impexp_config2.xml" ) );
}

// Creates a config file similar to the previous one, but with an extra spatial
// context and feature schema. 
void FdoImportExportTest::CreateConfigBadWkt( FdoIoStream* stream )
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream);
    AddSC( writer, L"Manhole1", L"diagram", L"Non-Earth (Meter)", L"nonsense wkt", 3 );
    writer = NULL;

    Stream2File( stream, UnitTestUtil::GetOutputFileName( L"impexp_configBadWkt.xml" ) );
}

// Create a Feature Schema and add it to an XML document
void FdoImportExportTest::AddSchema( FdoXmlWriter* writer, FdoInt32 idx )
{
   
	FdoFeatureSchemaP pSchema = FdoFeatureSchema::Create( 
        (FdoString*) FdoStringP::Format( L"Schema%d", idx ), 
        L"" 
    );

	FdoFeatureClassP pClass = FdoFeatureClass::Create( 
        (FdoString*) FdoStringP::Format( L"ClassA%d", idx ), 
        L"" 
     );
	pClass->SetIsAbstract(false);

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"Prop1", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    if ( !(idx % 2) ) 
        pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
   	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Prop2", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    if ( idx == 2 ) {
        pProp = FdoDataPropertyDefinition::Create( L"Prop3", L"" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
	    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
    }

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetHasElevation((idx % 2) ? true : false);
    pGeomProp->SetHasMeasure((idx % 2) ? false : true);
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

    FdoClassesP(pSchema->GetClasses())->Add( pClass );

	FdoFeatureClassP pSubClass = FdoFeatureClass::Create( 
        (FdoString*) FdoStringP::Format( L"ClassB%d", idx ), 
        L"" 
     );
	pSubClass->SetIsAbstract(false);
    pSubClass->SetBaseClass(pClass);
    pSubClass->SetGeometryProperty(pGeomProp);
    FdoClassesP(pSchema->GetClasses())->Add( pSubClass );
#if 1
    pProp = FdoDataPropertyDefinition::Create( L"SubProp1", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
	pProp->SetNullable(true);
	FdoPropertiesP(pSubClass->GetProperties())->Add( pProp );
#endif
    pSchema->WriteXml(writer);
}

// Create a Spatial Context and add it to an XML document
void FdoImportExportTest::AddSC( FdoXmlWriter* writer, FdoString* name, FdoString* description, FdoString* coordSys, FdoString* wkt, FdoInt32 idx  )
{
    FdoXmlSpatialContextWriterP SCWriter = 
        FdoXmlSpatialContextWriter::Create( writer );

    SCWriter->SetName( name );
    SCWriter->SetDescription( description );
    SCWriter->SetCoordinateSystem( coordSys );

    if ( wcslen(wkt) > 0 ) 
        SCWriter->SetCoordinateSystemWkt( wkt );

    SCWriter->SetExtentType( 
        idx % 2 ?
            FdoSpatialContextExtentType_Dynamic :
            FdoSpatialContextExtentType_Static
    );

    FdoByteArray* extent = SerializeExtent( 0, 0, 150 * idx, 100 * idx );
    SCWriter->SetExtent( extent );
    extent->Release();
    SCWriter->SetXYTolerance( 0.001 );
    SCWriter->SetZTolerance( 0.01 );

    SCWriter->WriteSpatialContext();
}

void FdoImportExportTest::AddFeature( FdoIInsert* insertCommand, FdoInt32 idx )
{
    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    dataValue = FdoDataValue::Create((FdoInt64) idx);
    propertyValue = AddNewProperty( propertyValues, L"Prop1");
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create((FdoString*)FdoStringP::Format(L"Prop2_Val%d",idx));
    propertyValue = AddNewProperty( propertyValues, L"Prop2");
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create((FdoString*)FdoStringP::Format(L"SubProp1_Val%d",idx));
    propertyValue = AddNewProperty( propertyValues, L"SubProp1");
    propertyValue->SetValue(dataValue);

    double       coordsBuffer[3];
    int          segCount = 1;

	// Use 2D points to accomodate MySql
    coordsBuffer[0] = 5 * idx;
    coordsBuffer[1] = 10 * idx;

    propertyValue = AddNewProperty( propertyValues, L"Geometry");
    FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();
    FdoILineString* line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    FdoByteArray *byteArray = gf->GetFgf(line1);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
    propertyValue->SetValue(geometryValue);

    line1->Release();
    byteArray->Release();

    FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
}

void FdoImportExportTest::_addMapping( FdoXmlWriter* writer, FdoString* name, FdoString* provider, FdoString* xmlns )
{
    writer->WriteStartElement( L"SchemaMapping" );
    writer->WriteAttribute( L"name", name );
    writer->WriteAttribute( L"tablespace", L"USERS" );
    writer->WriteAttribute( L"tableMapping", L"Base" );

    writer->WriteAttribute( L"xmlns", xmlns );
    writer->WriteAttribute( L"provider", provider );
    writer->WriteEndElement();
}

// Convert extent from doubles to byte array
FdoByteArray* FdoImportExportTest::SerializeExtent( double minX, double minY, double maxX, double maxY )
{
    // Create a byte array 
  	FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope>            env = gf->CreateEnvelopeXY(minX, minY, maxX, maxY );  
    FdoPtr<FdoIGeometry>            geom = gf->CreateGeometry(env); 

    return( gf->GetFgf(geom) );
}

// Write a stream out to a file. Applies an XSL transformation to sort the 
// elements in the file.
void FdoImportExportTest::Stream2SortedFile( FdoIoStream* stream, FdoString* fileName )
{
    FdoIoMemoryStreamP stylesheetStream = FdoIoMemoryStream::Create();
    FdoIoMemoryStreamP sortedStream = FdoIoMemoryStream::Create();

    stylesheetStream->Write( (FdoByte*) pSortSheet, strlen(pSortSheet) );
    stylesheetStream->Reset();

    stream->Reset();

    FdoXslTransformerP tfmr = FdoXslTransformer::Create(
        FdoXmlReaderP( FdoXmlReader::Create(stream) ),
        FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
        FdoXmlWriterP( FdoXmlWriter::Create(sortedStream, false) )
    );

    tfmr->Transform();

    sortedStream->Reset();

    Stream2File( sortedStream, fileName );
}


// Write a stream out to a file.
void FdoImportExportTest::Stream2File( FdoIoStream* stream, FdoString* fileName )
{
    stream->Reset();

    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fileName, L"w+t" );
    XmlFormatter formatter(stream, fileStream);
    formatter.Format();
}

void FdoImportExportTest::AddMapping( FdoXmlWriter* writer, FdoString* name, FdoInt32 providerVersion )
{
}

FdoStringP FdoImportExportTest::GetWKT( FdoString* coordSys )
{
    if ( wcscmp(coordSys, L"Non-Earth (Meter)") == 0 ) 
        return( L"LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]" );

    return ( L"" );
}

void FdoImportExportTest::_overrideBend( FdoString* inFile, FdoString* outFile, FdoStringP providerName, FdoStringP xmlns )
{
    FdoIoFileStreamP stream1 = FdoIoFileStream::Create( inFile, L"rt" );
    FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
    UnitTestUtil::OverrideBend( 
        stream1, 
        stream2, 
        providerName, 
        xmlns,
        L"",
        L""
    );

    stream2->Reset();
    Stream2File( stream2, outFile );
}

FdoStringP FdoImportExportTest::OverrideBend( FdoString* inFile )
{
    return inFile;
}
