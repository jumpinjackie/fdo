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
 */

#include "UnitTest.h"
#include "RfpOverridesSerializeTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"

RfpOverridesSerializeTest::RfpOverridesSerializeTest()
{
}

RfpOverridesSerializeTest::~RfpOverridesSerializeTest()
{
}

void RfpOverridesSerializeTest::_setUp()
{
}

void RfpOverridesSerializeTest::_tearDown()
{
}

void RfpOverridesSerializeTest::testSerialize1()
{
	//First create the schema mapping
	FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = FdoPhysicalSchemaMappingCollection::Create();

	FdoGrfpPhysicalSchemaMappingP schemaMapping = FdoGrfpPhysicalSchemaMapping::Create();
	schemaMapping->SetName(L"testSchemaMapping1");
	mappings->Add(schemaMapping);
	FdoGrfpClassesP classes = schemaMapping->GetClasses();

	FdoGrfpClassDefinitionP classDef = FdoGrfpClassDefinition::Create();
	classDef->SetName(L"testClass1");
	classes->Add(classDef);

	FdoGrfpRasterDefinitionP rasterDef = FdoGrfpRasterDefinition::Create();
	rasterDef->SetName(L"testRaster1");
	classDef->SetRasterDefinition(rasterDef);
	FdoGrfpRasterLocationsP locations = rasterDef->GetLocations();

	FdoGrfpRasterLocationP location = FdoGrfpRasterLocation::Create();
	location->SetName(L"../../TestData/ISD_Shanghai_Office");
	locations->Add(location);
	FdoGrfpRasterFeaturesP rasterFeatures = location->GetFeatureCatalogue();

	double orgX = 200000.0, orgY = 1000000.0;
	double sizeX = 200.0, sizeY = 200.0;
	int tileX = 33, tileY = 9;

	int counter = 0;
	double tempY = orgY;
	for (int j = 1; j <= tileY; j++)
	{
		double tempX = orgX;
		for (int i = 1; i <= tileX; i++)
		{
			FdoGrfpRasterFeatureDefinitionP rasterFeature = FdoGrfpRasterFeatureDefinition::Create();
			rasterFeature->SetName(FdoStringP::Format(L"NewOffice%.2d", counter));
			rasterFeatures->Add(rasterFeature);

			FdoGrfpRasterBandsP rasterBands = rasterFeature->GetBands();

			FdoGrfpRasterBandDefinitionP rasterBand = FdoGrfpRasterBandDefinition::Create();
			rasterBands->Add(rasterBand);
			rasterBand->SetName(L"Band1");
			rasterBand->SetBandNumber(1);
			
			FdoGrfpRasterImageDefinitionP rasterImage = FdoGrfpRasterImageDefinition::Create();
			rasterBand->SetImage(rasterImage);

			rasterImage->SetName(FdoStringP::Format(L"NewOffice.jpg_%.2d.gif", counter));

			FdoGrfpRasterGeoreferenceLocationP geoRefLocation = FdoGrfpRasterGeoreferenceLocation::Create();
			geoRefLocation->SetXInsertionPoint(tempX);
			geoRefLocation->SetYInsertionPoint(tempY);
			
			rasterImage->SetGeoreferencedLocation(geoRefLocation);	

			tempX += sizeX;

			counter++;
		}
		tempY += sizeY;
	}

	//Serialize it to the XML
	FdoPtr<FdoIoStream> output = FdoIoFileStream::Create(L"RfpOverridesSerializeTest_testSerialize1.xml", L"w");
	mappings->WriteXml(output);
	//output = NULL;

	//And now read it back from the XML
	mappings = FdoPhysicalSchemaMappingCollection::Create();
	mappings->ReadXml(L"RfpOverridesSerializeTest_testSerialize1.xml");

    //Verify the mappings from the XML is just the same as the original one
	schemaMapping = static_cast<FdoGrfpPhysicalSchemaMapping*>(mappings->GetItem(0));
	FDO_CPPUNIT_ASSERT(schemaMapping != NULL && wcscmp(schemaMapping->GetName(), L"testSchemaMapping1") == 0);
	classes = schemaMapping->GetClasses();
	FDO_CPPUNIT_ASSERT(classes != NULL);
	classDef = classes->GetItem(0);
	FDO_CPPUNIT_ASSERT(classDef != NULL && STREQUAL(classDef->GetName(), L"testClass1"));
	rasterDef = classDef->GetRasterDefinition();
	FDO_CPPUNIT_ASSERT(rasterDef != NULL && STREQUAL(rasterDef->GetName(), L"testRaster1"));
	locations = rasterDef->GetLocations();
	FDO_CPPUNIT_ASSERT(locations != NULL);
	location = locations->GetItem(0);
	FDO_CPPUNIT_ASSERT(location != NULL && STREQUAL(location->GetName(), L"../../TestData/ISD_Shanghai_Office"));
	rasterFeatures = location->GetFeatureCatalogue();
	FDO_CPPUNIT_ASSERT(rasterFeatures != NULL);

	int current = 0;
	tempY = orgY;
	for (int j = 1; j <= tileY; j++)
	{
		double tempX = orgX;
		for (int i = 1; i <= tileX; i++)
		{
			FdoGrfpRasterFeatureDefinitionP rasterFeature = rasterFeatures->GetItem(current);
			FDO_CPPUNIT_ASSERT(rasterFeature != NULL && STREQUAL(rasterFeature->GetName(), FdoStringP::Format(L"NewOffice%.2d", current)));

			FdoGrfpRasterBandsP rasterBands = rasterFeature->GetBands();

			FdoGrfpRasterBandDefinitionP rasterBand = rasterBands->GetItem(0);
			FDO_CPPUNIT_ASSERT(rasterBand != NULL && STREQUAL(rasterBand->GetName(), L"Band1") && rasterBand->GetBandNumber() == 1);

			FdoGrfpRasterImageDefinitionP rasterImage = rasterBand->GetImage();

			FDO_CPPUNIT_ASSERT(rasterImage != NULL && STREQUAL(rasterImage->GetName(), FdoStringP::Format(L"NewOffice.jpg_%.2d.gif", current)));

			FdoGrfpRasterGeoreferenceLocationP geoRefLocation = rasterImage->GetGeoreferencedLocation();
			FDO_CPPUNIT_ASSERT(geoRefLocation != NULL);
			FDO_CPPUNIT_ASSERT(geoRefLocation->GetXInsertionPoint() == tempX);
			FDO_CPPUNIT_ASSERT(geoRefLocation->GetYInsertionPoint() == tempY);
			//FDO_CPPUNIT_ASSERT(extents->GetMaxX() == tempX + sizeX);
			//FDO_CPPUNIT_ASSERT(extents->GetMaxY() == tempY + sizeY);
			tempX += sizeX;

			current++;
		}
		tempY += sizeY;
	}

	// Oops
}

void RfpOverridesSerializeTest::TestConfiguration(FdoGrfpPhysicalSchemaMapping* schemaMapping)
{
    try {
        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        
        FdoGrfpClassesP classes = schemaMapping->GetClasses();
        for (FdoInt32 i=0; i<classes->GetCount(); i++) {
            FdoGrfpClassDefinitionP classDefn = classes->GetItem(i);
            TestBaseProperties(classDefn);
            
            FdoGrfpRasterDefinitionP rasterDefn = classDefn->GetRasterDefinition();
            TestBaseProperties(rasterDefn);

            FdoGrfpRasterLocationsP locations = rasterDefn->GetLocations();
            for (FdoInt32 j=0; j<locations->GetCount(); j++) {
                FdoGrfpRasterLocationP location = locations->GetItem(j);
                TestBaseProperties(location);
                
                FdoGrfpRasterFeaturesP features = location->GetFeatureCatalogue();
                for (FdoInt32 k=0; k<features->GetCount(); k++) {
                    FdoGrfpRasterFeatureDefinitionP feature = features->GetItem(k);
                    TestBaseProperties(feature);

                    FdoGrfpRasterBandsP bands = feature->GetBands();
                    for (FdoInt32 m=0; m<bands->GetCount(); m++) {
                        FdoGrfpRasterBandDefinitionP band = bands->GetItem(m);
                        TestBaseProperties(band);

                        FdoInt32 bandNumber = band->GetBandNumber();
                        FdoGrfpRasterImageDefinitionP image = band->GetImage();

                        FdoInt32 frameNumber = image->GetFrameNumber();
                        FdoGrfpRasterGeoreferenceLocationP location = image->GetGeoreferencedLocation();

                        FdoDouble x_ins = location->GetXInsertionPoint();
                        FdoDouble y_ins = location->GetYInsertionPoint();
                        FdoDouble x_res = location->GetXResolution();
                        FdoDouble y_res = location->GetYResolution();
                        FdoDouble x_rot = location->GetXRotation();
                        FdoDouble y_rot = location->GetYRotation();
                    }
                }
            }
        }
    }
	catch (FdoException* e) 
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
	}
}

void RfpOverridesSerializeTest::TestBaseProperties(FdoPhysicalElementMapping* elementMapping)
{
    try {
        FdoString * name = elementMapping->GetName();
        FdoStringP qualifiedName = elementMapping->GetQualifiedName();
        FdoPtr<FdoPhysicalElementMapping> parentMapping = elementMapping->GetParent();
        FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = elementMapping->GetSchemaMapping();
    }
	catch (FdoException* e) 
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
	}
}

// TestSimple() reads in a simple Schema override into a memory stream and 
// writes that stream directly back to file. During this process it removes all 
// of the line feed and tab characters ensuring the file is contained on a single 
// line for easy comparison. This file becomes the GOLD data for the test. Secondly, 
// this test uses the Overrides API to write the file to a second stream and 
// then writes the stream to disk, once again removing the new line and tab chars. 
// This test finally compared the two documents to see if they are identical.
void RfpOverridesSerializeTest::testSimple () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"RfpOverridesConfig.xml");
        mappings->ReadXml(configReader);

        FdoGrfpPhysicalSchemaMappingP config = static_cast<FdoGrfpPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestConfiguration(config);

        FdoIoStreamP baseConfigStream = configReader->GetStream();
        FdoStringP baseConfigFileName = FdoStringP::Format(L"RfpOverridesConfig_w%3d.xml", rand());
        Stream2File(baseConfigStream, baseConfigFileName);

        FdoIoMemoryStreamP configStream = FdoIoMemoryStream::Create();
        config->WriteXml(configStream);
        FdoStringP configFileName = FdoStringP::Format(L"RfpOverridesConfig_w%3d.xml", rand());
        Stream2File(configStream, configFileName);

#ifdef _WIN32
        UnitTestUtil::CheckOutput(baseConfigFileName, configFileName);
#endif
    }
	catch (FdoException* e) 
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
	}
	catch (char* error) 
    {
        CPPUNIT_FAIL(error);
	}
}

void RfpOverridesSerializeTest::Stream2File( FdoIoStream* stream, FdoString* fileName )
{
    stream->Reset();
    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fileName, L"w+t" );
    XmlFormatter formatter(stream, fileStream);
    formatter.Format();
}

void RfpOverridesSerializeTest::testSetConfiguration()
{
    try
    {
        FdoPtr<FdoIConnection> connection = CreateConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"4_4.xml", L"r");

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
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
	}
}

