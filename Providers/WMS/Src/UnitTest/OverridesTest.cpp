/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION( OverridesTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OverridesTest, "OverridesTest");

OverridesTest::OverridesTest(void)
{
    schemaNames = FdoStringCollection::Create();
    classNames = FdoStringCollection::Create();
    rasterPropertyNames = FdoStringCollection::Create();
    layerNames = FdoStringCollection::Create();
    styleNames = FdoStringCollection::Create();

    schemaNames->Add(L"Schema1");
    schemaNames->Add(L"Schema2");
    
    classNames->Add(L"Class1");
    classNames->Add(L"Class2");
    
    rasterPropertyNames->Add(L"Raster1");
    rasterPropertyNames->Add(L"Raster2");

    layerNames->Add(L"Layer1");
    layerNames->Add(L"Layer2");

    styleNames->Add(L"Style1");
    styleNames->Add(L"Style2");
}

OverridesTest::~OverridesTest(void)
{
}

void OverridesTest::TestSetConfiguration2 ()
{
	FdoPtr<FdoIConnection> connection = WmsTests::GetConnection();
	if (connection == NULL) {
		CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
	}
	try
	{
		connection->SetConnectionString(L"FeatureServer=http://cadc-isd-jake.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
		FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"WmsSchemaConfig_Jake_MapServer.xml", L"r");

		connection->SetConfiguration(fileStream);
		connection->Open();

		FdoPtr<FdoIDescribeSchema> cmd = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
		FdoPtr<FdoFeatureSchemaCollection> schemas = cmd->Execute ();
		CPPUNIT_ASSERT (schemas->GetCount () == 1);
		FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
		CPPUNIT_ASSERT (wcscmp (schema->GetName (), L"Itasca_Demo") == 0);

		FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
		CPPUNIT_ASSERT (classes->GetCount () == 1);
		FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (0);
		CPPUNIT_ASSERT (wcscmp (clsDef->GetName (), L"Cities") == 0);
		FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();

		FdoInt32 cntProps = props->GetCount ();
		CPPUNIT_ASSERT (cntProps == 2);

#ifdef _DEBUG
		wprintf (L"Class: %ls\n", clsDef->GetName ());
		for (FdoInt32 i=0; i<cntProps; i++)
		{
			FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
			wprintf (L"Property: %ls\n", prop->GetName ());
		}
#endif

		FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect *> (connection->CreateCommand (FdoCommandType_Select));
		cmdSelect->SetFeatureClassName (clsDef->GetName ());
		FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();

		while (featReader->ReadNext ())
		{
#ifdef _DEBUG
			wprintf (L"Id: %ls\n", featReader->GetString (L"Id"));
#endif
			FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Image");
			FdoInt32 xSize = raster->GetImageXSize ();
			FdoInt32 ySize = raster->GetImageYSize ();		
			FdoPtr<FdoIStreamReader> streamReader = raster->GetStreamReader ();
			CPPUNIT_ASSERT (streamReader != NULL);
			FdoIStreamReaderTmpl<FdoByte> * byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader.p);

			FdoByte buff[4096];
			FdoInt64 cntTotal = 0;
			FdoInt32 cntRead = 0;
			do
			{
				cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
				cntTotal += cntRead;
			}
			while (cntRead);
		}

		connection->Close();
	}
	catch (FdoException* e) 
    {
        fail (e);
	}
}

void OverridesTest::TestCreateSchemaOverrides()
{
    try {
        for (FdoInt32 i=0; i<schemaNames->GetCount(); i++)
        {
            FdoWmsOvPhysicalSchemaMappingP config = FdoWmsOvPhysicalSchemaMapping::Create();
            config->SetName(schemaNames->GetString(i));

            FdoWmsOvClassesP classes = config->GetClasses();
            for (FdoInt32 j=0; j<classNames->GetCount(); j++)
            {
                FdoWmsOvClassDefinitionP classDefn = FdoWmsOvClassDefinition::Create();
                classDefn->SetName(classNames->GetString(j));

                FdoWmsOvRasterDefinitionP rasterDefn = FdoWmsOvRasterDefinition::Create();
                rasterDefn->SetName(rasterPropertyNames->GetString(j));
                rasterDefn->SetImageFormat(L"image/tiff");
                rasterDefn->SetTransparent(true);
                rasterDefn->SetBackgroundColor(L"0xFFFFFF");
                rasterDefn->SetTimeDimension(L"TIME=current");
                rasterDefn->SetElevationDimension(L"ELEVATION=0");
                rasterDefn->SetSpatialContextName(L"EPSG:4326");

                FdoWmsOvLayersP layers = rasterDefn->GetLayers();
                for (FdoInt32 k=0; k<layerNames->GetCount(); k++)
                {
                    FdoWmsOvLayerDefinitionP layerDefn = FdoWmsOvLayerDefinition::Create();
                    layerDefn->SetName(layerNames->GetString(k));

                    FdoWmsOvStyleDefinitionP styleDefn = FdoWmsOvStyleDefinition::Create();
                    styleDefn->SetName(styleNames->GetString(k));

                    layerDefn->SetStyle(styleDefn);

                    layers->Add(layerDefn);
                }
        
                classDefn->SetRasterDefinition(rasterDefn);
                classes->Add(classDefn);
            }

            TestNewConfiguration(config, i);
        }
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

void OverridesTest::TestNewConfiguration(FdoWmsOvPhysicalSchemaMapping* schemaMapping, FdoInt32 schemaIndex)
{
    try {
        FdoStringP name = schemaMapping->GetName();
        if (name != schemaNames->GetString(schemaIndex)) {
            CPPUNIT_FAIL("FAILED - FdoWmsOvPhysicalSchemaMapping::GetName()");
        }

        FdoWmsOvClassesP classes = schemaMapping->GetClasses();
        for (FdoInt32 j=0; j<classNames->GetCount(); j++)
        {
            FdoWmsOvClassDefinitionP classDefn = classes->GetItem(j);
            FdoStringP name = classDefn->GetName();
            if (name != classNames->GetString(j)) {
                CPPUNIT_FAIL("FAILED - FdoWmsOvClassDefinition::GetName()");
            }

            FdoWmsOvRasterDefinitionP rasterDefinition = classDefn->GetRasterDefinition();
            FdoStringP rasterPropName = rasterDefinition->GetName();
            if (rasterPropName != rasterPropertyNames->GetString(j)) {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetName()");
            }
            FdoStringP format = rasterDefinition->GetImageFormat();
            if (format != L"image/tiff")
            {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoBoolean bTransparent = rasterDefinition->GetTransparent();
            if (bTransparent != true) {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoStringP bgcolor = rasterDefinition->GetBackgroundColor();
            if (bgcolor != L"0xFFFFFF") {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoStringP timeDimension = rasterDefinition->GetTimeDimension();
            if (timeDimension != L"TIME=current") {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoStringP elevationDimension = rasterDefinition->GetElevationDimension();
            if (elevationDimension != L"ELEVATION=0") {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoStringP sc = rasterDefinition->GetSpatialContextName();
            if (sc != L"EPSG:4326") {
                CPPUNIT_FAIL("FAILED - FdoWmsOvRasterDefinition::GetImageFormat()");
            }

            FdoWmsOvLayersP layers = rasterDefinition->GetLayers();
            for (FdoInt32 k=0; k<layers->GetCount(); k++)
            {
                FdoWmsOvLayerDefinitionP layerDefn = layers->GetItem(k);
                FdoStringP name = layerDefn->GetName();
                if (name != layerNames->GetString(k)) {
                    CPPUNIT_FAIL("FAILED - FdoWmsOvLayerDefinition::GetName()");
                }

                FdoWmsOvStyleDefinitionP styleDefn = layerDefn->GetStyle();
                FdoStringP styleName = styleDefn->GetName();
                if (styleName != styleNames->GetString(k)) {
                    CPPUNIT_FAIL("FAILED - FdoWmsOvStyleDefinition::GetName()");
                }
            }
        }
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

// TestSimple() reads in a simple Schema override into a memory stream and 
// writes that stream directly back to file. During this process it removes all 
// of the line feed and tab characters ensuring the file is contained on a single 
// line for easy comparison. This file becomes the GOLD data for the test. Secondly, 
// this test uses the Overrides API to write the file to a second stream and 
// then writes the stream to disk, once again removing the new line and tab chars. 
// This test finally compared the two documents to see if they are identical.
void OverridesTest::TestSimple () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"WmsOverridesConfig.xml"); // this configuration is from WMS 3.3
        mappings->ReadXml(configReader);

        FdoWmsOvPhysicalSchemaMappingP config = static_cast<FdoWmsOvPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestConfiguration(config);

        FdoIoStreamP baseConfigStream = configReader->GetStream();
        FdoStringP baseConfigFileName = FdoStringP::Format(L"WmsOverridesConfig_w%3d.xml", rand());
        Stream2File(baseConfigStream, baseConfigFileName);

        FdoIoMemoryStreamP configStream = FdoIoMemoryStream::Create();
        config->WriteXml(configStream);
        FdoStringP configFileName = FdoStringP::Format(L"WmsOverridesConfig_w%3d.xml", rand());
        Stream2File(configStream, configFileName);

#ifdef _WIN32
        UnitTestUtil::CheckOutput(baseConfigFileName, configFileName);
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

void OverridesTest::TestImageFormatBackwardsComp () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"Wms322OverridesConfig.xml"); // this configuration is from WMS 3.2
        mappings->ReadXml(configReader);

        FdoWmsOvPhysicalSchemaMappingP schemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        
        FdoWmsOvClassesP classes = schemaMapping->GetClasses();
        CPPUNIT_ASSERT(classes->GetCount() == 1);

        FdoWmsOvClassDefinitionP classDefn = classes->GetItem(0);
        TestBaseProperties(classDefn);
        
        FdoWmsOvRasterDefinitionP rasterDefn = classDefn->GetRasterDefinition();
        TestBaseProperties(rasterDefn);
        FdoStringP format = rasterDefn->GetImageFormat();
        CPPUNIT_ASSERT(format == L"image/tiff");
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

void OverridesTest::TestConflictImageFormatConfig () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"WmsOverridesConflictImageFormat.xml"); // this configuration is from WMS 3.3
        mappings->ReadXml(configReader);

        FdoWmsOvPhysicalSchemaMappingP schemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        
        FdoWmsOvClassesP classes = schemaMapping->GetClasses();
        CPPUNIT_ASSERT(classes->GetCount() == 1);

        FdoWmsOvClassDefinitionP classDefn = classes->GetItem(0);
        TestBaseProperties(classDefn);
        
        FdoWmsOvRasterDefinitionP rasterDefn = classDefn->GetRasterDefinition();
        TestBaseProperties(rasterDefn);
        FdoStringP format = rasterDefn->GetImageFormat();
        CPPUNIT_ASSERT(format == L"image/tiff; PhotometricInterpretation=RGB");
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

void OverridesTest::TestDefaultImageFormat () 
{
    try {
        srand((unsigned)time(NULL));

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoXmlReaderP configReader = FdoXmlReader::Create(L"WmsOverridesConfigNoneImageFormat.xml"); // this configuration is from WMS 3.2
        mappings->ReadXml(configReader);

        FdoWmsOvPhysicalSchemaMappingP schemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping*>(mappings->GetItem(0));

        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        
        FdoWmsOvClassesP classes = schemaMapping->GetClasses();
        CPPUNIT_ASSERT(classes->GetCount() == 1);

        FdoWmsOvClassDefinitionP classDefn = classes->GetItem(0);
        TestBaseProperties(classDefn);
        
        FdoWmsOvRasterDefinitionP rasterDefn = classDefn->GetRasterDefinition();
        TestBaseProperties(rasterDefn);
        FdoStringP format = rasterDefn->GetImageFormat();
        CPPUNIT_ASSERT(format == L"image/png");

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

void OverridesTest::TestConfiguration(FdoWmsOvPhysicalSchemaMapping* schemaMapping)
{
    try {
        TestBaseProperties(schemaMapping);
        FdoString * providerName = schemaMapping->GetProvider();
        
        FdoWmsOvClassesP classes = schemaMapping->GetClasses();
        for (FdoInt32 i=0; i<classes->GetCount(); i++) {
            FdoWmsOvClassDefinitionP classDefn = classes->GetItem(i);
            TestBaseProperties(classDefn);
            
            FdoWmsOvRasterDefinitionP rasterDefn = classDefn->GetRasterDefinition();
            TestBaseProperties(rasterDefn);
            FdoStringP format = rasterDefn->GetImageFormat();

            FdoBoolean bTransparent = rasterDefn->GetTransparent();
            FdoStringP bgcolor = rasterDefn->GetBackgroundColor();
            FdoStringP timeDimension = rasterDefn->GetTimeDimension();
            FdoStringP elevationDimension = rasterDefn->GetElevationDimension();
            FdoStringP sc = rasterDefn->GetSpatialContextName();

            FdoWmsOvLayersP layers = rasterDefn->GetLayers();
            for (FdoInt32 j=0; j<layers->GetCount(); j++) {
                FdoWmsOvLayerDefinitionP layerDefn = layers->GetItem(j);
                TestBaseProperties(layerDefn);
                
                FdoWmsOvStyleDefinitionP styleDefn = layerDefn->GetStyle();
                TestBaseProperties(styleDefn);
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
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }

        connection->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"WmsSchemaConfig.xml", L"r"); // config is from WMS 3.2.2

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

void OverridesTest::TestSetConfiguration3()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }

        connection->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"WMSOverrideClass2.xml", L"r"); //WMS 3.2.2

        connection->SetConfiguration(fileStream);
        connection->Open();

        FdoPtr<FdoIDescribeSchema> cmd = 
            static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmd->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();

        for (FdoInt32 i=0; i<cntSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (i);
            FdoStringP schemaName = schema->GetName();
#ifdef _DEBUG
            wprintf (L"    Schema: %ls\n", (FdoString *) schemaName);
#endif

            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
            FdoInt32 cntClasses = classes->GetCount();

            for (FdoInt32 j=0; j<cntSchemas; j++)
            {
                FdoPtr<FdoClassDefinition> clsDef = classes->GetItem(j);
                FdoStringP className = clsDef->GetName();
#ifdef _DEBUG
                wprintf (L"    Class: %ls\n", (FdoString *) className);
#endif

                FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
                FdoInt32 cntProps = props->GetCount();

                for (FdoInt32 i=0; i<cntProps; i++)
                {
                    FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
                    FdoStringP propName = prop->GetName ();
#ifdef _DEBUG
                    wprintf (L"        Property: %ls\n", (FdoString *) propName);
#endif
                }

                FdoPtr<FdoISelect> cmdSelect = 
                    static_cast<FdoISelect *> (connection->CreateCommand (FdoCommandType_Select));
                cmdSelect->SetFeatureClassName (clsDef->GetName ());
                FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();

                while (featReader->ReadNext ())
                {
#ifdef _DEBUG
                    wprintf (L"            Id: %ls\n", featReader->GetString (L"Id"));
#endif
                    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Image");
                    FdoInt32 xSize = raster->GetImageXSize ();
                    FdoInt32 ySize = raster->GetImageYSize ();		
                    FdoPtr<FdoIStreamReader> streamReader = raster->GetStreamReader ();
                    CPPUNIT_ASSERT (streamReader != NULL);
                    FdoIStreamReaderTmpl<FdoByte> * byteStreamReader = 
                        static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader.p);

                    FdoByte buff[4096];
                    FdoInt64 cntTotal = 0;
                    FdoInt32 cntRead = 0;
                    do
                    {
                        cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                        cntTotal += cntRead;
                    }
                    while (cntRead);
                }
            }
        }


        connection->Close();
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}
// Test configuration, which contains the new xml image format, like "image/tiff; PhotometricInterpretation=RGB"
void OverridesTest::TestRequestSpcialImageFormat()
{
    try
    {   
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }
 
        connection->SetConnectionString(L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?version=1.1.0"); 
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"newconfig1.xml", L"r"); // config for WMS 3.3
  
        connection->SetConfiguration(fileStream);
        connection->Open();
      
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"WMSLayers:Foundation BARRIERL_1M"); // layer name is Foundation BARRIERL_1M
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Image");
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close();
        
    }
	catch (FdoException* e) 
    {
        fail (e);
	}
}

// Test configuration, which contains the new xml image format, like "image/tiff; PhotometricInterpretation=RGB"
void OverridesTest::TestRequestUnsupportedImage()
{
    try
    {   
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection();
        if (connection == NULL) {
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        }

        connection->SetConnectionString(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap?version=1.3.0"); 
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"newconfig.xml", L"r"); // WMS 3.3
        connection->SetConfiguration(fileStream);
        connection->Open();
      
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"WMSLayers:rail"); 
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Image");
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close();

        fail("<Unsupported Image Format> Exception is not thrown out.");
    }
	catch (FdoException* e) 
    {
        e->Release();
	}

}
void OverridesTest::TestNoDefaultDataModel()
{
    try
    {  
        FdoIoMemoryStreamP configStream = FdoIoMemoryStream::Create();
        FdoXmlWriterP writer = FdoXmlWriter::Create(configStream);

        ////////////

        FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create( NULL );
        FdoFeatureSchemaP pSchema = FdoFeatureSchema::Create( L"TestSchema", L"" );
        FdoClassesP pClasses = pSchema->GetClasses();

        FdoFeatureClassP pClass = FdoFeatureClass::Create( L"TestClass", L"" );
        pClass->SetIsAbstract(false);
        FdoPropertiesP pProperties = pClass->GetProperties();
        FdoDataPropertiesP pIDProperties = pClass->GetIdentityProperties();

        FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(255);
        pProp->SetNullable(false);
        pProp->SetReadOnly(false);

        pProperties->Add( pProp );
        pIDProperties->Add( pProp );

        FdoRasterPropertyP pRasterProp = FdoRasterPropertyDefinition::Create( L"Picture", L"" );
        pRasterProp->SetNullable(true);
        pRasterProp->SetReadOnly(false);
        pRasterProp->SetDefaultImageXSize(0);
        pRasterProp->SetDefaultImageYSize(0);
        pRasterProp->SetSpatialContextAssociation(L"");
        pRasterProp->SetDefaultDataModel(NULL);

        pProperties->Add( pRasterProp );

        pClasses->Add( pClass );
        pSchema->AcceptChanges();
        pSchemas->Add( pSchema );
        pSchemas->WriteXml(writer);

        //////////////

        FdoSchemaMappingsP mappings = FdoPhysicalSchemaMappingCollection::Create();
        FdoWmsOvPhysicalSchemaMappingP config = FdoWmsOvPhysicalSchemaMapping::Create();
        config->SetName(L"TestSchema");

        FdoWmsOvClassesP ovClasses = config->GetClasses();
        FdoWmsOvClassDefinitionP classDefn = FdoWmsOvClassDefinition::Create();
        classDefn->SetName(L"TestClass");

        FdoWmsOvRasterDefinitionP rasterDefn = FdoWmsOvRasterDefinition::Create();
        rasterDefn->SetName(L"Picture");
        rasterDefn->SetImageFormat(L"image/png");

        FdoWmsOvLayersP layers = rasterDefn->GetLayers();
        FdoWmsOvLayerDefinitionP layerDefn = FdoWmsOvLayerDefinition::Create();
        layerDefn->SetName(L"worldwind_dem");
        layers->Add(layerDefn);

        classDefn->SetRasterDefinition(rasterDefn);
        ovClasses->Add(classDefn);

        mappings->Add(config);
        mappings->WriteXml(writer);

        writer->Close();
        configStream->Reset();

        ///////////////

        FdoPtr<FdoIConnection> conn = WmsTests::GetConnection();
        conn->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
        conn->SetConfiguration(configStream);
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        //////////////

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
#ifdef _DEBUG
        wprintf (L"Schema: %ls\n", schema->GetName ());
#endif

        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoInt32 cntClasses = classes->GetCount ();
        CPPUNIT_ASSERT (cntClasses == 1);

        // test the inheritance realationship
        for (FdoInt32 j=0; j<cntClasses; j++)
        {
            FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (j);
            FdoStringP clsName = clsDef->GetName ();
            FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);

            FdoFeatureClass* base = static_cast<FdoFeatureClass *> (featClsDef->GetBaseClass ());		

#ifdef _DEBUG
            wprintf (L"  Class: %ls\n", (FdoString*)clsName);
            if (base != NULL)
                wprintf (L"  Base:  %ls\n", base->GetName ());
            else
	            wprintf (L"  Base:  (null)\n");
#endif

            FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
            FdoInt32 cntProps = props->GetCount ();
            for (FdoInt32 k=0; k<cntProps; k++)
            {
	            FdoPtr<FdoPropertyDefinition> prop = props->GetItem (k);
	            FdoStringP propName = prop->GetName ();
	            FdoPropertyType propType = prop->GetPropertyType ();
#ifdef _DEBUG
	            wprintf (L"    Property: %ls\t", (FdoString*)propName);
#endif
            }		
        }

        /////////////////

        FdoPtr<FdoIDescribeSchemaMapping> cmd = static_cast<FdoIDescribeSchemaMapping *> (conn->CreateCommand (FdoCommandType_DescribeSchemaMapping));
        FdoPtr<FdoPhysicalSchemaMappingCollection> physicalMappings = cmd->Execute ();

        for (FdoInt32 i=0; i<physicalMappings->GetCount(); i++)
        {
            FdoPtr<FdoPhysicalSchemaMapping> mapping = physicalMappings->GetItem (i);
            FdoString* mappingName = mapping->GetName ();
            FdoString* providerName = mapping->GetProvider ();
#ifdef _DEBUG
            wprintf (L"\nProvider: %ls\nSchemaMapping: %ls\n", providerName, mappingName);
#endif
        }
    }
	catch (FdoException* e) 
    {
        fail(e);
	}
}

void OverridesTest::TestQualifiedName()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"NASA_WMS_Config.xml", L"r"); // configuration from WMS 3.2.2
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchemaMapping> cmd = static_cast<FdoIDescribeSchemaMapping *>(conn->CreateCommand(FdoCommandType_DescribeSchemaMapping));
        FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = cmd->Execute ();
        int cntSchema = mappings->GetCount ();
        for (int i=0; i<cntSchema; i++)
        {
            FdoPtr<FdoPhysicalSchemaMapping> mapping = mappings->GetItem (i);
            FdoWmsOvPhysicalSchemaMapping* wmsSchemaMapping = dynamic_cast<FdoWmsOvPhysicalSchemaMapping *>(mapping.p);
            FdoStringP schemaQName = wmsSchemaMapping->GetQualifiedName ();
            CPPUNIT_ASSERT (wcscmp (L"Itasca_Demo", (FdoString*)schemaQName) == 0 );
#ifdef _DEBUG
            wprintf (L"Schema: %ls\n", (FdoString*)schemaQName);
#endif
            FdoPtr<FdoPhysicalElementMapping> parent = wmsSchemaMapping->GetParent ();
            if (parent)
            {
                FdoStringP qName = parent->GetQualifiedName ();
            }
                
            FdoPtr<FdoWmsOvClassCollection> wmsClassMappings = wmsSchemaMapping->GetClasses ();
            int cntClass = wmsClassMappings->GetCount ();
            for (int j=0; j<cntClass; j++)
            {
                FdoPtr<FdoWmsOvClassDefinition> wmsClassMapping = wmsClassMappings->GetItem (j);
                FdoStringP classQName = wmsClassMapping->GetQualifiedName ();
                CPPUNIT_ASSERT (wcscmp (L"Itasca_Demo:global_mosaic", (FdoString*)classQName) == 0);
#ifdef _DEBUG
                wprintf (L"Class: %ls\n", (FdoString*)classQName);
#endif
                FdoPtr<FdoWmsOvRasterDefinition> rasterDef = wmsClassMapping->GetRasterDefinition ();
                FdoStringP rasterDefQName = rasterDef->GetQualifiedName ();
                CPPUNIT_ASSERT (wcscmp (L"Itasca_Demo:global_mosaic.Image", (FdoString*)rasterDefQName) == 0);
#ifdef _DEBUG
                wprintf (L"Property: %ls\n", (FdoString*)rasterDefQName);
#endif
            }
        }
	}
	catch(FdoException* ex)
	{
        fail(ex);
	}
}
