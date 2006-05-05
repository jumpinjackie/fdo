/*
 * 
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
#include "OverridesTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#include "FdoCommonFile.h"
#include <Fdo/Xml/SpatialContextSerializer.h>
#include <Fdo/ClientServices/ClientServices.h>
#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ConnectionManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Fdo/ClientServices/ClientServiceException.h>
#include <Fdo/ClientServices/Provider.h>

#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION( OverridesTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OverridesTest, "OverridesTest");

#ifdef _WIN32
#define ROOT_LOCATION L"..\\..\\TestData\\"
#define ONTARIO_LOCATION ROOT_LOCATION L"Ontario\\"
#define NANBOKU_LOCATION ROOT_LOCATION L"Nanboku\\"
#else
#define ROOT_LOCATION L"../../TestData/"
#define ONTARIO_LOCATION ROOT_LOCATION L"Ontario/"
#define NANBOKU_LOCATION ROOT_LOCATION L"Nanboku/"
#endif

// Schema constants:
const FdoString* OverridesTest::schemaName   = L"NewSchema";

// Ontario class constants:
const FdoString* OverridesTest::classNameOntario = L"NewOntario";
const FdoString* OverridesTest::shapefileOntario = ROOT_LOCATION L"OntarioGenerated.shp";
const FdoString* OverridesTest::propOntarioId =        L"NewFeatId";
const FdoString* OverridesTest::propOntarioGeom =      L"NewGeometry";
const FdoString* OverridesTest::propOntarioPerimeter = L"NewPerimeter";
const FdoString* OverridesTest::propOntarioArea =      L"NewArea";
const FdoString* OverridesTest::colOntarioPerimeter = L"PERIMETER";
const FdoString* OverridesTest::colOntarioArea =      L"AREA";

// Nanboku class constants:
const FdoString* OverridesTest::classNameNanboku = L"NewNanboku";
const FdoString* OverridesTest::shapefileNanboku = ROOT_LOCATION L"NanbokuGenerated.shp";
const FdoString* OverridesTest::propNanbokuId =        L"NewFeatId";
const FdoString* OverridesTest::propNanbokuGeom =      L"NewGeometry";
const FdoString* OverridesTest::propNanbokuGL =        L"NewGL";
const FdoString* OverridesTest::colNanbokuGL =        L"GL";
const FdoString* OverridesTest::filterNanboku =        L"(NewGL < 25.0) and (NewFeatId < 100)";

OverridesTest::OverridesTest(void)
{
    schemaNames = FdoStringCollection::Create();
    classNames = FdoStringCollection::Create();
    propertyNames = FdoStringCollection::Create();
    columnNames = FdoStringCollection::Create();
    fileNames = FdoStringCollection::Create();

    schemaNames->Add(L"Water");
    schemaNames->Add(L"Water2");
    
    classNames->Add(L"Lake");
    classNames->Add(L"River");

    columnNames->Add(L"SoundingDate");
    columnNames->Add(L"Surveyor");

    propertyNames->Add(L"SNDG_DATE");
    propertyNames->Add(L"SRVYR");

    fileNames->Add(L"/Foo/Lake.shp");
    fileNames->Add(L"/Foo/River.shp");
}

OverridesTest::~OverridesTest(void)
{
}

void OverridesTest::TestCreateSchemaOverrides()
{
    try {
        for (FdoInt32 i=0; i<schemaNames->GetCount(); i++)
        {
            // Create a new in memory schema override set
            FdoShpOvPhysicalSchemaMappingP config = FdoShpOvPhysicalSchemaMapping::Create();
            config->SetName(schemaNames->GetString(i));
            FdoShpOvClassCollectionP classes = config->GetClasses();

            // Create class overrides
            for (FdoInt32 j=0; j<classNames->GetCount(); j++)
            {
                // Create a new class override
                FdoShpOvClassDefinitionP classDefn = FdoShpOvClassDefinition::Create();
                classDefn->SetName(classNames->GetString(j));
                classDefn->SetShapeFile(fileNames->GetString(j));
                FdoShpOvPropertyDefinitionCollectionP properties = classDefn->GetProperties();

                // Create property overrides
                for (FdoInt32 k=0; k<propertyNames->GetCount(); k++)
                {
                    // Create a new property override
                    FdoShpOvPropertyDefinitionP propDefn = FdoShpOvPropertyDefinition::Create();
                    propDefn->SetName(propertyNames->GetString(k));

                    // Create column override
                    FdoShpOvColumnDefinitionP columnDefn = FdoShpOvColumnDefinition::Create();
                    columnDefn->SetName(columnNames->GetString(k));

                    // Set the associated column
                    propDefn->SetColumn(columnDefn);

                    // Add the property override
                    properties->Add(propDefn);
                }
        
                // Add the class override
                classes->Add(classDefn);
            }

            // Test the data we just created to see if we can read what we wrote
            TestNewConfiguration(config, i);
        }
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

void OverridesTest::TestNewConfiguration(FdoShpOvPhysicalSchemaMapping* schemaMapping, FdoInt32 schemaIndex)
{
    try {
        FdoStringP name = schemaMapping->GetName();
        if (name != schemaNames->GetString(schemaIndex)) {
            CPPUNIT_FAIL("FAILED - FdoShpOvPhysicalSchemaMapping::GetName()");
        }

        FdoShpOvClassCollectionP classes = schemaMapping->GetClasses();
        for (FdoInt32 j=0; j<classNames->GetCount(); j++)
        {
            FdoShpOvClassDefinitionP classDefn = classes->GetItem(j);
            FdoStringP name = classDefn->GetName();
            if (name != classNames->GetString(j)) {
                CPPUNIT_FAIL("FAILED - FdoShpOvClassDefinition::GetName()");
            }

            FdoStringP shapeFileName = classDefn->GetShapeFile();
            if (shapeFileName != fileNames->GetString(j)) {
                CPPUNIT_FAIL("FAILED - FdoShpOvClassDefinition::GetShapeFile()");
            }

            FdoShpOvPropertyDefinitionCollectionP properties = classDefn->GetProperties();
            for (FdoInt32 k=0; k<propertyNames->GetCount(); k++)
            {
                FdoShpOvPropertyDefinitionP propDefn = properties->GetItem(k);
                FdoStringP name = propDefn->GetName();
                if (name != propertyNames->GetString(k)) {
                    CPPUNIT_FAIL("FAILED - FdoShpOvPropertyDefinition::GetName()");
                }

                FdoShpOvColumnDefinitionP columnDefn = propDefn->GetColumn();
                FdoStringP columnName = columnDefn->GetName();
                if (columnName != columnNames->GetString(k)) {
                    CPPUNIT_FAIL("FAILED - FdoShpOvColumnDefinition::GetName()");
                }
            }
        }
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

// TestSimple() reads in a simple ODBC Schema override into a memory stream and 
// writes that stream directly back to file. During this process it removes all 
// of the line feed and tab characters ensuring the file is contained on a single 
// line for easy comparison. This file becomes the GOLD data for the test. Secondly, 
// this test uses the ODBC Overrides API to write the file to a second stream and 
// then writes the stream to disk, once again removing the new line and tab chars. 
// This test finally compared the two documents to see if they are identical.
void OverridesTest::TestSimple () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"ShpOverridesConfig.xml");
        mappings->ReadXml(configReader);

        FdoShpOvPhysicalSchemaMappingP config = static_cast<FdoShpOvPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestConfiguration(config);

        FdoIoStreamP baseConfigStream = configReader->GetStream();
        FdoStringP baseConfigFileName = FdoStringP::Format(L"ShpOverridesConfig_w%3d.xml", rand());
        Stream2File(baseConfigStream, baseConfigFileName);

        FdoIoMemoryStreamP configStream = FdoIoMemoryStream::Create();
        config->WriteXml(configStream);
        FdoStringP configFileName = FdoStringP::Format(L"ShpOverridesConfig_w%3d.xml", rand());
        Stream2File(configStream, configFileName);

#ifdef _WIN32
        UnitTestUtil::CheckOutput(baseConfigFileName, configFileName);
#else
        // throw FdoCommandException::Create(L"Test Not Supported yet on Linux");
#endif
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
	catch (char* error) 
    {
        fail (error);
	}
}

void OverridesTest::Stream2File( FdoIoStream* stream, FdoString* fileName )
{
    stream->Reset();
    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fileName, L"w+t" );
    XmlFormatter formatter(stream, fileStream);
    formatter.Format();
}

void OverridesTest::TestConfiguration(FdoShpOvPhysicalSchemaMapping* schemaMapping)
{
    try {
        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        FdoShpOvClassCollectionP classes = schemaMapping->GetClasses();
        for (FdoInt32 i=0; i<classes->GetCount(); i++) {
            FdoShpOvClassDefinitionP classDefn = classes->GetItem(i);
            TestBaseProperties(classDefn);
            FdoShpOvPropertyDefinitionCollectionP properties = classDefn->GetProperties();
            for (FdoInt32 j=0; j<properties->GetCount(); j++) {
                FdoShpOvPropertyDefinitionP propDefn = properties->GetItem(j);
                TestBaseProperties(propDefn);
                FdoShpOvColumnDefinitionP columnDefn = propDefn->GetColumn();
                TestBaseProperties(columnDefn);
            }
        }
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
	catch (char* error) 
    {
        fail (error);
	}
}

void OverridesTest::TestBaseProperties(FdoPhysicalElementMapping* elementMapping)
{
    try {
        FdoString * name = elementMapping->GetName();
        FdoStringP qualifiedName = elementMapping->GetQualifiedName();
        FdoPtr<FdoPhysicalElementMapping> parentMapping = elementMapping->GetParent();
        FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = elementMapping->GetSchemaMapping();
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
	catch (char* error) 
    {
        fail (error);
	}
}

void OverridesTest::TestSetConfiguration()
{
    try
    {
        FdoPtr<FdoIConnection> connection = ShpTests::GetConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }

        connection->SetConnectionString (L"DefaultFileLocation=" ONTARIO_LOCATION);
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"ShpSchemaConfig.xml", L"r");

        connection->SetConfiguration(fileStream);
        connection->Open();
        connection->Close();

        connection->SetConfiguration(fileStream);
        connection->SetConfiguration(fileStream);

        connection->Open();

        bool bFail = false;
        try
        {
            connection->SetConfiguration(fileStream);
        }
        catch (FdoException *e)
        {
            bFail = true;
            e->Release();
        }

        if (bFail == false) {
            CPPUNIT_FAIL("FAILED - SetConfiguration() while state == Open should not be supported\n");
        }

        connection->Close();
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}


void OverridesTest::TestSetEmptyConfiguration ()
{
    try
    {
        // clean up after prior test
        if (FdoCommonFile::FileExists (ONTARIO_LOCATION L"schema.xml"))
            FdoCommonFile::Delete (ONTARIO_LOCATION L"schema.xml");
        FdoPtr<FdoIConnection> connection = ShpTests::GetConnection();
        connection->SetConnectionString (L"DefaultFileLocation=" ONTARIO_LOCATION);
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"ShpEmptySchemaConfig.xml", L"r");
        connection->SetConfiguration(fileStream);
        connection->Open();

        // ensure there are zero schemas
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)connection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("wrong schema count", 0 == schemas->GetCount ());

        // ensure asking for a nonexistant schema yields the proper exception
        try
        {
            describe->SetSchemaName (L"Bogus");
            FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
            CPPUNIT_FAIL ("no exception asking for a nonexistant schema in an empty configuration");
        }
        catch (FdoException* ge)
        {
            // check for expected message
            CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == wcsncmp (L"Schema 'Bogus' not found.", ge->GetExceptionMessage (), 25));
            ge->Release ();
        }

        connection->Close();
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

/* Test describe/select operations with schema overrides. */
void OverridesTest::TestReadWithOverrides ()
{
    // Ontario class constants:
    FdoString* shapefileOntario = L"ontario.shp";

    // Nanboku class constants:
#ifdef _WIN32
    FdoString* shapefileNanboku = L"..\\Nanboku\\Nanboku_3dLine.shp";
#else
    FdoString* shapefileNanboku = L"../Nanboku/Nanboku_3dLine.shp";
#endif

    try
    {
        // Create schema mappings for config stream:
        /////////////////////////////////////////////////////////////////

        FdoPtr<FdoShpOvPhysicalSchemaMapping> schemaMapping = FdoShpOvPhysicalSchemaMapping::Create();
        schemaMapping->SetName(schemaName);
        FdoPtr<FdoShpOvClassCollection> classMappings = schemaMapping->GetClasses();

        // Create class mappings for Ontario:
        FdoPtr<FdoShpOvClassDefinition> classMapping = FdoShpOvClassDefinition::Create();
        classMapping->SetName(classNameOntario);
        classMapping->SetShapeFile(shapefileOntario);
        FdoPtr<FdoShpOvPropertyDefinitionCollection> propMappings = classMapping->GetProperties();
        FdoPtr<FdoShpOvPropertyDefinition> propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propOntarioPerimeter);
        FdoPtr<FdoShpOvColumnDefinition> colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colOntarioPerimeter);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propOntarioArea);
        colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colOntarioArea);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        classMappings->Add(classMapping);

        // Create class mappings for Nanboku:
        classMapping = FdoShpOvClassDefinition::Create();
        classMapping->SetName(classNameNanboku);
        classMapping->SetShapeFile(shapefileNanboku);
        propMappings = classMapping->GetProperties();
        propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propNanbokuGL);
        colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colNanbokuGL);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        classMappings->Add(classMapping);


        // Create logical schema for config stream:
        ////////////////////////////////////////////////////////////////

        FdoPtr<FdoFeatureSchema> configSchema = FdoFeatureSchema::Create(schemaName, L"");
        FdoPtr<FdoClassCollection> configClasses = configSchema->GetClasses();

        // Create ontario class:
        FdoPtr<FdoFeatureClass> configClassOntario = FdoFeatureClass::Create(classNameOntario, L"");
        FdoPtr<FdoPropertyDefinitionCollection> configPropsOntario = configClassOntario->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> configIdPropsOntario = configClassOntario->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioID = FdoDataPropertyDefinition::Create(propOntarioId, L"");
        configPropOntarioID->SetDataType(FdoDataType_Int32);
        configPropOntarioID->SetIsAutoGenerated(false);
        configPropOntarioID->SetNullable(false);
        configPropOntarioID->SetReadOnly(true);
        configPropsOntario->Add(configPropOntarioID);
        configIdPropsOntario->Add(configPropOntarioID);
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioPerimeter = FdoDataPropertyDefinition::Create(propOntarioPerimeter, L"");
        configPropOntarioPerimeter->SetDataType(FdoDataType_Decimal);
        configPropOntarioPerimeter->SetPrecision(20);
        configPropOntarioPerimeter->SetScale(8);
        configPropOntarioPerimeter->SetNullable(false);
        configPropOntarioPerimeter->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioPerimeter);
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioArea = FdoDataPropertyDefinition::Create(propOntarioArea, L"");
        configPropOntarioArea->SetDataType(FdoDataType_Decimal);
        configPropOntarioArea->SetPrecision(20);
        configPropOntarioArea->SetScale(8);
        configPropOntarioArea->SetNullable(false);
        configPropOntarioArea->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioArea);
        FdoPtr<FdoGeometricPropertyDefinition> configPropOntarioGeom = FdoGeometricPropertyDefinition::Create(propOntarioGeom, L"");
        configPropOntarioGeom->SetGeometryTypes(FdoGeometricType_Surface);
        configPropOntarioGeom->SetHasElevation(false);
        configPropOntarioGeom->SetHasMeasure(false);
        configPropOntarioGeom->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioGeom);
        configClassOntario->SetGeometryProperty(configPropOntarioGeom);
        configClasses->Add(configClassOntario);

        // Create Nanboku class:
        FdoPtr<FdoFeatureClass> configClassNanboku = FdoFeatureClass::Create(classNameNanboku, L"");
        FdoPtr<FdoPropertyDefinitionCollection> configPropsNanboku = configClassNanboku->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> configIdPropsNanboku = configClassNanboku->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> configPropNanbokuID = FdoDataPropertyDefinition::Create(propNanbokuId, L"");
        configPropNanbokuID->SetDataType(FdoDataType_Int32);
        configPropNanbokuID->SetIsAutoGenerated(false);
        configPropNanbokuID->SetNullable(false);
        configPropNanbokuID->SetReadOnly(true);
        configPropsNanboku->Add(configPropNanbokuID);
        configIdPropsNanboku->Add(configPropNanbokuID);
        FdoPtr<FdoDataPropertyDefinition> configPropNanbokuGL = FdoDataPropertyDefinition::Create(propNanbokuGL, L"");
        configPropNanbokuGL->SetDataType(FdoDataType_Decimal);
        configPropNanbokuGL->SetPrecision(20);
        configPropNanbokuGL->SetScale(8);
        configPropNanbokuGL->SetNullable(false);
        configPropNanbokuGL->SetReadOnly(false);
        configPropsNanboku->Add(configPropNanbokuGL);
        FdoPtr<FdoGeometricPropertyDefinition> configPropNanbokuGeom = FdoGeometricPropertyDefinition::Create(propNanbokuGeom, L"");
        configPropNanbokuGeom->SetGeometryTypes(FdoGeometricType_Curve);
        configPropNanbokuGeom->SetHasElevation(false);
        configPropNanbokuGeom->SetHasMeasure(false);
        configPropNanbokuGeom->SetReadOnly(false);
        configPropsNanboku->Add(configPropNanbokuGeom);
        configClassNanboku->SetGeometryProperty(configPropNanbokuGeom);
        configClasses->Add(configClassNanboku);



        // Create connection:
        ////////////////////////////////////////////////////////////////

        // Output to XML stream:
        FdoPtr<FdoIoMemoryStream>  xmlConfigStream = FdoIoMemoryStream::Create();
        FdoPtr<FdoXmlWriter> xmlWriter = FdoXmlWriter::Create(xmlConfigStream);
        FdoPtr<FdoXmlFlags> xmlFlags = FdoXmlFlags::Create();
        configSchema->WriteXml(xmlWriter, xmlFlags);
        schemaMapping->WriteXml(xmlWriter, xmlFlags);
        xmlWriter = NULL;  // required to close the datastore tag
        xmlConfigStream->Reset();

        // Create a new connection, passing in the XML config stream:
        FdoPtr<FdoIConnection> conn = ShpTests::GetConnection ();
        conn->SetConnectionString (L"DefaultFileLocation=" ONTARIO_LOCATION);
        conn->SetConfiguration(xmlConfigStream);
        conn->Open();


        // Verify the schema looks the way we want:
        ////////////////////////////////////////////////////////////////

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        CPPUNIT_ASSERT_MESSAGE("wrong schema count", 1==schemas->GetCount());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        CPPUNIT_ASSERT_MESSAGE("wrong schema name", 0==wcscmp(schemaName, schema->GetName()));
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        CPPUNIT_ASSERT_MESSAGE("wrong class count", 2==classes->GetCount());

        // validate Ontario class:
        FdoPtr<FdoClassDefinition> aClass = classes->GetItem(classNameOntario);
        CPPUNIT_ASSERT_MESSAGE("wrong class type", FdoClassType_FeatureClass==aClass->GetClassType());
        FdoPtr<FdoPropertyDefinitionCollection> properties = aClass->GetProperties();
        CPPUNIT_ASSERT_MESSAGE("wrong property count", 4==properties->GetCount());
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem(propOntarioId);
        FdoDataPropertyDefinition* dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Int32==dataProperty->GetDataType());
        property = properties->GetItem(propOntarioPerimeter);
        dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
        property = properties->GetItem(propOntarioArea);
        dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
        property = properties->GetItem(propOntarioGeom);
        FdoGeometricPropertyDefinition* geomProperty = dynamic_cast<FdoGeometricPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=geomProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong geom types", FdoGeometricType_Surface==geomProperty->GetGeometryTypes());

        // validate Nanboku class:
        aClass = classes->GetItem(classNameNanboku);
        CPPUNIT_ASSERT_MESSAGE("wrong class type", FdoClassType_FeatureClass==aClass->GetClassType());
        properties = aClass->GetProperties();
        CPPUNIT_ASSERT_MESSAGE("wrong property count", 3==properties->GetCount());
        property = properties->GetItem(propNanbokuId);
        dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Int32==dataProperty->GetDataType());
        property = properties->GetItem(propNanbokuGL);
        dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
        property = properties->GetItem(propNanbokuGeom);
        geomProperty = dynamic_cast<FdoGeometricPropertyDefinition*>(property.p);
        CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=geomProperty);
        CPPUNIT_ASSERT_MESSAGE("wrong geom types", FdoGeometricType_Curve==geomProperty->GetGeometryTypes());

        // Select from Ontario class (no filter, no property subset):
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
        select->SetFeatureClassName(classNameOntario);
        FdoPtr<FdoIFeatureReader> reader = select->Execute();
        while (reader->ReadNext())
        {
            FdoInt32 id = reader->GetInt32(propOntarioId);
            FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry(propOntarioGeom);
            double perimeter = reader->GetDouble(propOntarioPerimeter);
            double area = reader->GetDouble(propOntarioArea);

            area=area; // dummy line for debugging purposes
        }

        // Select from Nanboku class (with filter and property subset):
        select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
        select->SetFeatureClassName(classNameNanboku);
        select->SetFilter(filterNanboku);
        FdoPtr<FdoIdentifierCollection> selectedProperties = select->GetPropertyNames();
        FdoPtr<FdoIdentifier> selectedProperty = FdoIdentifier::Create(propNanbokuId);
        selectedProperties->Add(selectedProperty);
        selectedProperty = FdoIdentifier::Create(propNanbokuGL);
        selectedProperties->Add(selectedProperty);
        reader = select->Execute();
        while (reader->ReadNext())
        {
            FdoInt32 id = reader->GetInt32(propNanbokuId);
            double gl = reader->GetDouble(propNanbokuGL);

            CPPUNIT_ASSERT_MESSAGE("NewFeatId not filtered out as expected", id<100);
            CPPUNIT_ASSERT_MESSAGE("NewGL not filtered out as expected", gl<25.0);

            // should throw exception (not selected):
            try
            {
                FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry(propNanbokuGeom);
                CPPUNIT_ASSERT_MESSAGE("Didn't get expected exception when fetching property not in select set", false);
            }
            catch (FdoException *e)
            {
                // We expect to get here, so don't re-throw exception
                e->Release();
            }
        }

    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}





/* Test applyschema/insert operations with schema overrides. */
void OverridesTest::TestWriteWithOverrides ()
{
    try
    {
        // Create schema mappings for config stream:
        /////////////////////////////////////////////////////////////////

        FdoPtr<FdoShpOvPhysicalSchemaMapping> schemaMapping = FdoShpOvPhysicalSchemaMapping::Create();
        schemaMapping->SetName(schemaName);
        FdoPtr<FdoShpOvClassCollection> classMappings = schemaMapping->GetClasses();

        // Create class mappings for Ontario:
        FdoPtr<FdoShpOvClassDefinition> classMapping = FdoShpOvClassDefinition::Create();
        classMapping->SetName(classNameOntario);
        classMapping->SetShapeFile(shapefileOntario);
        FdoPtr<FdoShpOvPropertyDefinitionCollection> propMappings = classMapping->GetProperties();
        FdoPtr<FdoShpOvPropertyDefinition> propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propOntarioPerimeter);
        FdoPtr<FdoShpOvColumnDefinition> colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colOntarioPerimeter);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propOntarioArea);
        colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colOntarioArea);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        classMappings->Add(classMapping);

        // Create class mappings for Nanboku:
        classMapping = FdoShpOvClassDefinition::Create();
        classMapping->SetName(classNameNanboku);
        classMapping->SetShapeFile(shapefileNanboku);
        propMappings = classMapping->GetProperties();
        propMapping = FdoShpOvPropertyDefinition::Create();
        propMapping->SetName(propNanbokuGL);
        colMapping = FdoShpOvColumnDefinition::Create();
        colMapping->SetName(colNanbokuGL);
        propMapping->SetColumn(colMapping);
        propMappings->Add(propMapping);
        classMappings->Add(classMapping);


        // Create logical schema for config stream:
        ////////////////////////////////////////////////////////////////

        FdoPtr<FdoFeatureSchema> configSchema = FdoFeatureSchema::Create(schemaName, L"");
        FdoPtr<FdoClassCollection> configClasses = configSchema->GetClasses();

        // Create ontario class:
        FdoPtr<FdoFeatureClass> configClassOntario = FdoFeatureClass::Create(classNameOntario, L"");
        FdoPtr<FdoPropertyDefinitionCollection> configPropsOntario = configClassOntario->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> configIdPropsOntario = configClassOntario->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioID = FdoDataPropertyDefinition::Create(propOntarioId, L"");
        configPropOntarioID->SetDataType(FdoDataType_Int32);
        configPropOntarioID->SetIsAutoGenerated(false);
        configPropOntarioID->SetNullable(false);
        configPropOntarioID->SetReadOnly(true);
        configPropsOntario->Add(configPropOntarioID);
        configIdPropsOntario->Add(configPropOntarioID);
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioPerimeter = FdoDataPropertyDefinition::Create(propOntarioPerimeter, L"");
        configPropOntarioPerimeter->SetDataType(FdoDataType_Decimal);
        configPropOntarioPerimeter->SetPrecision(20);
        configPropOntarioPerimeter->SetScale(8);
        configPropOntarioPerimeter->SetNullable(false);
        configPropOntarioPerimeter->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioPerimeter);
        FdoPtr<FdoDataPropertyDefinition> configPropOntarioArea = FdoDataPropertyDefinition::Create(propOntarioArea, L"");
        configPropOntarioArea->SetDataType(FdoDataType_Decimal);
        configPropOntarioArea->SetPrecision(20);
        configPropOntarioArea->SetScale(8);
        configPropOntarioArea->SetNullable(false);
        configPropOntarioArea->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioArea);
        FdoPtr<FdoGeometricPropertyDefinition> configPropOntarioGeom = FdoGeometricPropertyDefinition::Create(propOntarioGeom, L"");
        configPropOntarioGeom->SetGeometryTypes(FdoGeometricType_Surface);
        configPropOntarioGeom->SetHasElevation(false);
        configPropOntarioGeom->SetHasMeasure(false);
        configPropOntarioGeom->SetReadOnly(false);
        configPropsOntario->Add(configPropOntarioGeom);
        configClassOntario->SetGeometryProperty(configPropOntarioGeom);
        configClasses->Add(configClassOntario);

        // Create Nanboku class:
        FdoPtr<FdoFeatureClass> configClassNanboku = FdoFeatureClass::Create(classNameNanboku, L"");
        FdoPtr<FdoPropertyDefinitionCollection> configPropsNanboku = configClassNanboku->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> configIdPropsNanboku = configClassNanboku->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> configPropNanbokuID = FdoDataPropertyDefinition::Create(propNanbokuId, L"");
        configPropNanbokuID->SetDataType(FdoDataType_Int32);
        configPropNanbokuID->SetIsAutoGenerated(false);
        configPropNanbokuID->SetNullable(false);
        configPropNanbokuID->SetReadOnly(true);
        configPropsNanboku->Add(configPropNanbokuID);
        configIdPropsNanboku->Add(configPropNanbokuID);
        FdoPtr<FdoDataPropertyDefinition> configPropNanbokuGL = FdoDataPropertyDefinition::Create(propNanbokuGL, L"");
        configPropNanbokuGL->SetDataType(FdoDataType_Decimal);
        configPropNanbokuGL->SetPrecision(20);
        configPropNanbokuGL->SetScale(8);
        configPropNanbokuGL->SetNullable(false);
        configPropNanbokuGL->SetReadOnly(false);
        configPropsNanboku->Add(configPropNanbokuGL);
        FdoPtr<FdoGeometricPropertyDefinition> configPropNanbokuGeom = FdoGeometricPropertyDefinition::Create(propNanbokuGeom, L"");
        configPropNanbokuGeom->SetGeometryTypes(FdoGeometricType_Curve);
        configPropNanbokuGeom->SetHasElevation(false);
        configPropNanbokuGeom->SetHasMeasure(false);
        configPropNanbokuGeom->SetReadOnly(false);
        configPropsNanboku->Add(configPropNanbokuGeom);
        configClassNanboku->SetGeometryProperty(configPropNanbokuGeom);
        configClasses->Add(configClassNanboku);



        // Create connection:
        ////////////////////////////////////////////////////////////////

        // Create a new connection, with NO configuration file:
        FdoPtr<FdoIConnection> conn = ShpTests::GetConnection ();
        conn->SetConnectionString (L"DefaultFileLocation=" ROOT_LOCATION);
        conn->Open();

        // Clean up leftovers from previous tests:
        CleanUpClass(conn, NULL, L"NewOntario");
        CleanUpClass(conn, NULL, L"NewNanboku");


        // Create the new classes using ApplySchema and the overrides:
        ////////////////////////////////////////////////////////////////

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
        apply->SetFeatureSchema(configSchema);
        apply->SetPhysicalMapping(schemaMapping);
        apply->Execute();

		FdoStringP sSchema = ROOT_LOCATION L"schema.xml";
		FdoPtr <FdoXmlWriter> writer = FdoXmlWriter::Create (sSchema, true, FdoXmlWriter::LineFormat_Indent, 128);

		FdoPtr<FdoIDescribeSchema> describe_schema = (FdoIDescribeSchema*)conn->CreateCommand (FdoCommandType_DescribeSchema);
		FdoPtr<FdoFeatureSchemaCollection> schemas = describe_schema->Execute ();
		schemas->WriteXml (writer, NULL);
		FdoPtr<FdoIDescribeSchemaMapping> describe_mapping = (FdoIDescribeSchemaMapping*)conn->CreateCommand (FdoCommandType_DescribeSchemaMapping);
		FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = describe_mapping->Execute ();
		if (0 != mappings->GetCount ())
			mappings->WriteXml (writer, NULL);
		writer = NULL;

        // Verify the schema looks the way we want:
        ////////////////////////////////////////////////////////////////
        VerifySchema (conn);

        // Disconnect and reconnect (to clear schema cache/overrides):
        ////////////////////////////////////////////////////////////////
        conn->Close ();
        conn->Open();
        VerifySchema (conn);
        conn->Close ();

        // Try a completely different connection:
        ////////////////////////////////////////////////////////////////
        FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();
        connection->SetConnectionString (L"DefaultFileLocation=" ROOT_LOCATION);
        connection->Open();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void OverridesTest::VerifySchema (FdoIConnection* conn)
{
    FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand (FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(schemaName);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    CPPUNIT_ASSERT_MESSAGE("wrong class count", 2==classes->GetCount());

    // validate Ontario class:
    FdoPtr<FdoClassDefinition> aClass = classes->GetItem(classNameOntario);
    CPPUNIT_ASSERT_MESSAGE("wrong class type", FdoClassType_FeatureClass==aClass->GetClassType());
    FdoPtr<FdoPropertyDefinitionCollection> properties = aClass->GetProperties();
    CPPUNIT_ASSERT_MESSAGE("wrong property count", 4==properties->GetCount());
    FdoPtr<FdoPropertyDefinition> property = properties->GetItem(propOntarioId);
    FdoDataPropertyDefinition* dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Int32==dataProperty->GetDataType());
    property = properties->GetItem(propOntarioPerimeter);
    dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
    property = properties->GetItem(propOntarioArea);
    dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
    property = properties->GetItem(propOntarioGeom);
    FdoGeometricPropertyDefinition* geomProperty = dynamic_cast<FdoGeometricPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=geomProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong geom types", FdoGeometricType_Surface==geomProperty->GetGeometryTypes());

    // validate Nanboku class:
    aClass = classes->GetItem(classNameNanboku);
    CPPUNIT_ASSERT_MESSAGE("wrong class type", FdoClassType_FeatureClass==aClass->GetClassType());
    properties = aClass->GetProperties();
    CPPUNIT_ASSERT_MESSAGE("wrong property count", 3==properties->GetCount());
    property = properties->GetItem(propNanbokuId);
    dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Int32==dataProperty->GetDataType());
    property = properties->GetItem(propNanbokuGL);
    dataProperty = dynamic_cast<FdoDataPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=dataProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong data type", FdoDataType_Decimal==dataProperty->GetDataType());
    property = properties->GetItem(propNanbokuGeom);
    geomProperty = dynamic_cast<FdoGeometricPropertyDefinition*>(property.p);
    CPPUNIT_ASSERT_MESSAGE("wrong property type", NULL!=geomProperty);
    CPPUNIT_ASSERT_MESSAGE("wrong geom types", FdoGeometricType_Curve==geomProperty->GetGeometryTypes());

    // Select from Ontario class (no filter, no property subset):
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
    select->SetFeatureClassName(classNameOntario);
    FdoPtr<FdoIFeatureReader> reader = select->Execute();
    while (reader->ReadNext())
    {
        FdoInt32 id = reader->GetInt32(propOntarioId);
        FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry(propOntarioGeom);
        double perimeter = reader->GetDouble(propOntarioPerimeter);
        double area = reader->GetDouble(propOntarioArea);

        area=area; // dummy line for debugging purposes
    }

    // Select from Nanboku class (with filter and property subset):
    select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
    select->SetFeatureClassName(classNameNanboku);
    select->SetFilter(filterNanboku);
    FdoPtr<FdoIdentifierCollection> selectedProperties = select->GetPropertyNames();
    FdoPtr<FdoIdentifier> selectedProperty = FdoIdentifier::Create(propNanbokuId);
    selectedProperties->Add(selectedProperty);
    selectedProperty = FdoIdentifier::Create(propNanbokuGL);
    selectedProperties->Add(selectedProperty);
    reader = select->Execute();
    while (reader->ReadNext())
    {
        FdoInt32 id = reader->GetInt32(propNanbokuId);
        double gl = reader->GetDouble(propNanbokuGL);

        CPPUNIT_ASSERT_MESSAGE("NewFeatId not filtered out as expected", id<100);
        CPPUNIT_ASSERT_MESSAGE("NewGL not filtered out as expected", gl<25.0);

        // should throw exception (not selected):
        try
        {
            FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry(propNanbokuGeom);
            CPPUNIT_ASSERT_MESSAGE("Didn't get expected exception when fetching property not in select set", false);
        }
        catch (FdoException *e)
        {
            // We expect to get here, so don't re-throw exception
            e->Release();
        }
    }
}


