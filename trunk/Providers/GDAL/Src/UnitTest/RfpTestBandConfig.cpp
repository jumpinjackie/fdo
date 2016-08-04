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
#include "RfpTestBandConfig.h"
#include <math.h>

RfpTestBandConfig::RfpTestBandConfig()
{
}

RfpTestBandConfig::~RfpTestBandConfig()
{
}

void RfpTestBandConfig::_setUp()
{
}

void RfpTestBandConfig::_tearDown()
{
}

void RfpTestBandConfig::testOverrides()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/band.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
	FdoPtr<FdoIDescribeSchemaMapping> describeMapping = static_cast<FdoIDescribeSchemaMapping*>(cmd);
	FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = describeMapping->Execute();
	FDO_CPPUNIT_ASSERT(mappings != NULL && mappings->GetCount() == 1);

	FdoPtr<FdoGrfpPhysicalSchemaMapping> mapping = static_cast<FdoGrfpPhysicalSchemaMapping*>(mappings->GetItem(0));
	FDO_CPPUNIT_ASSERT(mapping != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(mapping->GetName(), L"photos"));

	FdoPtr<FdoGrfpClassCollection> classes = mapping->GetClasses();
	FDO_CPPUNIT_ASSERT(classes != NULL && classes->GetCount() == 1);

	FdoPtr<FdoGrfpClassDefinition> classDef = classes->GetItem(0);
	FDO_CPPUNIT_ASSERT(classDef != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(classDef->GetName(), L"Photo"));

	FdoPtr<FdoGrfpRasterDefinition> rasterDef = classDef->GetRasterDefinition();
	FDO_CPPUNIT_ASSERT(rasterDef != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(rasterDef->GetName(), L"photos"));

	FdoPtr<FdoGrfpRasterLocationCollection> locations = rasterDef->GetLocations();
	FDO_CPPUNIT_ASSERT(locations != NULL && locations->GetCount() == 1);

	FdoPtr<FdoGrfpRasterLocation> location = locations->GetItem(0);
	FDO_CPPUNIT_ASSERT(location != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(location->GetName(), L"../../TestData/band"));

	FdoPtr<FdoGrfpRasterFeatureCollection> features = location->GetFeatureCatalogue();
	FDO_CPPUNIT_ASSERT(features != NULL && features->GetCount() == 2);

	// feature "photo11_12"
	FdoPtr<FdoGrfpRasterFeatureDefinition> featureDef1 = features->GetItem(0);
	FDO_CPPUNIT_ASSERT(featureDef1 != NULL && STRCASEEQ(featureDef1->GetName(), L"photo11_12"));

	FdoGrfpRasterBandsP bands1 = featureDef1->GetBands();
	FDO_CPPUNIT_ASSERT(bands1 != NULL && bands1->GetCount() == 2);

	FdoPtr<FdoGrfpRasterBandDefinition> bandDef1 = bands1->GetItem(0);
	FDO_CPPUNIT_ASSERT(bandDef1 != NULL && STRCASEEQ(bandDef1->GetName(), L"photo1_1") && bandDef1->GetBandNumber() == 1);
	FdoGrfpRasterImageDefinitionP imageDef1 = bandDef1->GetImage();
	FDO_CPPUNIT_ASSERT(imageDef1 != NULL && STRCASEEQ(imageDef1->GetName(), L"photo1_1.JPG"));
	FdoPtr<FdoGrfpRasterGeoreferenceLocation> geoRef1 = imageDef1->GetGeoreferencedLocation();
	FDO_CPPUNIT_ASSERT(geoRef1 != NULL);
	FDO_CPPUNIT_ASSERT(geoRef1->GetXInsertionPoint() == 100000.000000);
	FDO_CPPUNIT_ASSERT(geoRef1->GetYInsertionPoint() == 100000.000000);
	FDO_CPPUNIT_ASSERT(geoRef1->GetXResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef1->GetYResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef1->GetXRotation() == 0);
	FDO_CPPUNIT_ASSERT(geoRef1->GetYRotation() == 0);

	FdoPtr<FdoGrfpRasterBandDefinition> bandDef2 = bands1->GetItem(1);
	FDO_CPPUNIT_ASSERT(bandDef2 != NULL && STRCASEEQ(bandDef2->GetName(), L"photo1_2") && bandDef2->GetBandNumber() == 2);
	FdoGrfpRasterImageDefinitionP imageDef2 = bandDef2->GetImage();
	FDO_CPPUNIT_ASSERT(imageDef2 != NULL && STRCASEEQ(imageDef2->GetName(), L"photo1_2.JPG"));
	FdoPtr<FdoGrfpRasterGeoreferenceLocation> geoRef2 = imageDef2->GetGeoreferencedLocation();
	FDO_CPPUNIT_ASSERT(geoRef2 != NULL);
	FDO_CPPUNIT_ASSERT(geoRef2->GetXInsertionPoint() == 100000.000000);
	FDO_CPPUNIT_ASSERT(geoRef2->GetYInsertionPoint() == 100000.000000);
	FDO_CPPUNIT_ASSERT(geoRef2->GetXResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef2->GetYResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef2->GetXRotation() == 0);
	FDO_CPPUNIT_ASSERT(geoRef2->GetYRotation() == 0);

	// feature "photo1_3"
	FdoPtr<FdoGrfpRasterFeatureDefinition> featureDef2 = features->GetItem(1);
	FDO_CPPUNIT_ASSERT(featureDef2 != NULL && STRCASEEQ(featureDef2->GetName(), L"photo1_3"));

	FdoGrfpRasterBandsP bands2 = featureDef2->GetBands();
	FDO_CPPUNIT_ASSERT(bands2 != NULL && bands2->GetCount() == 1);

	FdoPtr<FdoGrfpRasterBandDefinition> bandDef3 = bands2->GetItem(0);
	FDO_CPPUNIT_ASSERT(bandDef3 != NULL && STRCASEEQ(bandDef3->GetName(), L"photo1_3") && bandDef3->GetBandNumber() == 1);
	FdoGrfpRasterImageDefinitionP imageDef3 = bandDef3->GetImage();
	FDO_CPPUNIT_ASSERT(imageDef3 != NULL && STRCASEEQ(imageDef3->GetName(), L"photo1_3.JPG"));
	FdoPtr<FdoGrfpRasterGeoreferenceLocation> geoRef3 = imageDef3->GetGeoreferencedLocation();
	FDO_CPPUNIT_ASSERT(geoRef3 != NULL);
	FDO_CPPUNIT_ASSERT(geoRef3->GetXInsertionPoint() == 200000.000000);
	FDO_CPPUNIT_ASSERT(geoRef3->GetYInsertionPoint() == 200000.000000);
	FDO_CPPUNIT_ASSERT(geoRef3->GetXResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef3->GetYResolution() == 1);
	FDO_CPPUNIT_ASSERT(geoRef3->GetXRotation() == 0);
	FDO_CPPUNIT_ASSERT(geoRef3->GetYRotation() == 0);

	connection->Close();
}


// --------------------------------------------------------------
// test if the config file is taken for preference for geo than 
// native geo-reference in raster file. 
// --------------------------------------------------------------
void RfpTestBandConfig::testGeoReference()
{
    try
    {
        FdoPtr<FdoIConnection> connection = CreateConnection();
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/rotated/GeoReference.xml", L"r");
        connection->SetConfiguration(stream);
        connection->Open();

        FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);

        cmdSelect->SetFeatureClassName(L"Photo");
        FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

        // first feature "lefttop", the config file has no georeference tag, so geo-info has to be retrieved from the raster image
        CPPUNIT_ASSERT(featureReader->ReadNext());

        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
        FdoInt32 xSize = raster->GetImageXSize();
        FdoInt32 ySize = raster->GetImageYSize();
        CPPUNIT_ASSERT(xSize == 756);
        CPPUNIT_ASSERT(ySize == 516);

        FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoByteArray> ba = raster->GetBounds();
        FdoPtr<FdoIGeometry> geometery = geomFactory->CreateGeometryFromFgf(ba);
        FdoPtr<FdoIEnvelope> bounds = geometery->GetEnvelope();

        CPPUNIT_ASSERT(fabs(bounds->GetMinX() - 515.000000) < 0.001);
        CPPUNIT_ASSERT(fabs(bounds->GetMinY() - 0.00000000) < 0.001);
        CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 1271.00000) < 0.001) ;
        CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 516.000000) < 0.001);

        // second feature "leftbottom", the config file has georeference info. so they should be taken preference.
        CPPUNIT_ASSERT(featureReader->ReadNext());

        raster = featureReader->GetRaster(L"Image");
        xSize = raster->GetImageXSize();
        ySize = raster->GetImageYSize();
        CPPUNIT_ASSERT(xSize == 756);
        CPPUNIT_ASSERT(ySize == 516);

        ba = raster->GetBounds();
        geometery = geomFactory->CreateGeometryFromFgf(ba);
        bounds = geometery->GetEnvelope();

        CPPUNIT_ASSERT(fabs(bounds->GetMinX() - 100000.000000) < 0.001);
        CPPUNIT_ASSERT(fabs(bounds->GetMinY() - 100000.000000) < 0.001);
        CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 100756.000000) < 0.001);
        CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 100516.000000) < 0.001);

        connection->Close();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

// --------------------------------------------------------------
// it's illegal that a raster image has no any geo-reference
// info associated, 
// --------------------------------------------------------------
void RfpTestBandConfig::testNoGeoReference()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/band_no_geo.xml", L"r");
    connection->SetConfiguration(stream);
    // it must fail for there is no geo-reference associated with the raster image
    bool bError = false;
    try
    {
        connection->Open();
    }
    catch(...)
    {
        bError = true;
        connection->Close();
    }

    CPPUNIT_ASSERT(bError == true);
}

// --------------------------------------------------------------
// dem files can be mixed up with other images as long as mosaic
// won't be operated on them. It's ok with selecting command.
// --------------------------------------------------------------
void RfpTestBandConfig::testMixDataModel1()
{
    // currently it's hard to find two rasters with DEM and RGBA data model while they have the same coordinate system.
    // it is commented out considering the coordinate system 
#ifdef _WIN32
    //FdoPtr<FdoIConnection> connection = CreateConnection();
    //FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/mix_datamodel.xml", L"r");
    //connection->SetConfiguration(stream);
    //connection->Open();

    //FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    //FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
    //cmdSelect->SetFeatureClassName(L"Photo");
    //FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    //// the first feature is a jpg file
    //CPPUNIT_ASSERT(featureReader->ReadNext());

    //FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    //   FdoInt32 xSize = raster->GetImageXSize();
    //   FdoInt32 ySize = raster->GetImageYSize();
    //CPPUNIT_ASSERT(xSize == 800);
    //CPPUNIT_ASSERT(ySize == 600);

    //// the first feature is a dem file
    //CPPUNIT_ASSERT(featureReader->ReadNext());

    //raster = featureReader->GetRaster(L"Image");
    //   xSize = raster->GetImageXSize();
    //   ySize = raster->GetImageYSize();
    //CPPUNIT_ASSERT(xSize == 337);
    //CPPUNIT_ASSERT(ySize == 467);

    //connection->Close();
#endif
}

// --------------------------------------------------------------
// dem files can't be mosaiced with other image files
// it's disallowed with SelectAggregates command.
// --------------------------------------------------------------
void RfpTestBandConfig::testMixDataModel2()
{
	// currently it's hard to find two rasters with DEM and RGBA data model while they have the same coordinate system.
	// it is commented out considering the coordinate system 
#ifdef _WIN32
	//FdoPtr<FdoIConnection> connection = CreateConnection();
	//FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/mix_datamodel.xml", L"r");
	//connection->SetConfiguration(stream);
	//connection->Open();

	//FdoICommand* cmd = connection->CreateCommand(FdoCommandType_SelectAggregates);
	//FdoPtr<FdoISelectAggregates> cmdSelect = static_cast<FdoISelectAggregates*>(cmd);
	//cmdSelect->SetFeatureClassName(L"Photo");

	//FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
	//// create the computed identifier containing the MOSAIC function and the alias name
	//FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	//FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	//funcParams->Add(rasterProp);
	//FdoPtr<FdoFunction> mosaicFunc = FdoFunction::Create(L"MOSAIC", funcParams);
	//FdoPtr<FdoComputedIdentifier> mosaicIdentifier = FdoComputedIdentifier::Create(L"stitchedRaster", mosaicFunc );
	//// add it to the properties to select
	//propsToSelect->Add(mosaicIdentifier);

	//bool bError = false;
	//try
	//{
	//	FdoPtr<FdoIDataReader> dataReader = cmdSelect->Execute();

	//	// MUST contains one record
	//	CPPUNIT_ASSERT( dataReader->ReadNext() == true);
	//	
	//	CPPUNIT_ASSERT(dataReader->GetPropertyCount() == 1);
	//	CPPUNIT_ASSERT(STRCASEEQ(dataReader->GetPropertyName(0), L"stitchedRaster"));
	//	CPPUNIT_ASSERT(dataReader->GetPropertyType(L"stitchedRaster") == FdoPropertyType_RasterProperty);

	//	// use alias to refer the stitched raster
	//	FdoPtr<FdoIRaster> raster = dataReader->GetRaster(L"stitchedRaster");

	//	// read the raster data
	//	FdoIStreamReader * streamReader = raster->GetStreamReader();
	//	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);
	//}
	//catch(...)
	//{	
	//	bError = true;
	//	connection->Close();
	//}	
	//CPPUNIT_ASSERT(bError == true);
#endif
}

// 
void RfpTestBandConfig::testGetNullValue1()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/getnullvalue1.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);

// for static_cast info see http://trac.osgeo.org/fdo/ticket/12#comment:2
#ifndef WIN32
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
#else
    FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
#endif
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // the first feature is a jpg file
    CPPUNIT_ASSERT(featureReader->ReadNext());

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoDataValue* nullVal = raster->GetNullPixelValue();
    FdoPtr<FdoInt32Value> val = static_cast<FdoInt32Value*>(nullVal);

    CPPUNIT_ASSERT(nullVal == NULL);

    connection->Close();
}

// 
void RfpTestBandConfig::testGetNullValue2()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/getnullvalue2.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
#ifndef WIN32
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
#else
    FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
#endif
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // the first feature is a dem file
    CPPUNIT_ASSERT(featureReader->ReadNext());

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoDataValue* nullVal = raster->GetNullPixelValue();
    FdoPtr<FdoInt16Value> val = static_cast<FdoInt16Value*>(nullVal);

    CPPUNIT_ASSERT(nullVal->GetDataType() == FdoDataType_Int16);	
    CPPUNIT_ASSERT(val->GetInt16() == -32767);

    connection->Close();
}

// --------------------------------------------------------------
// test if the resolution and rotation info set in the config file
// are correct
// --------------------------------------------------------------
void RfpTestBandConfig::testResolution()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/geo_resolution_rot.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
#ifndef WIN32
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
#else
    FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
#endif
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // feature "photo11_1", rotation = 0, resolution = 1;
    CPPUNIT_ASSERT(featureReader->ReadNext());

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    CPPUNIT_ASSERT(raster->GetImageXSize() == 800);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 600);

    FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> ba = raster->GetBounds();
    FdoPtr<FdoIGeometry> geometery = geomFactory->CreateGeometryFromFgf(ba);
    FdoPtr<FdoIEnvelope> bounds = geometery->GetEnvelope();

    CPPUNIT_ASSERT(fabs(bounds->GetMinX() - 100000.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMinY() - 100000.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 100800.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 100600.000000) < 0.001);

    // feature "photo11_2", rotation = 0, resolution = 2;
    CPPUNIT_ASSERT(featureReader->ReadNext());

    raster = featureReader->GetRaster(L"Image");
    CPPUNIT_ASSERT(raster->GetImageXSize() == 800);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 600);

    ba = raster->GetBounds();
    geometery = geomFactory->CreateGeometryFromFgf(ba);
    bounds = geometery->GetEnvelope();

    CPPUNIT_ASSERT(fabs(bounds->GetMinX() - 100000.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMinY() - 100000.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 101600.000000) < 0.001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 101200.000000) < 0.001);

    // feature "photo11_3", rotation = 90, resolution = 1;
    CPPUNIT_ASSERT(featureReader->ReadNext());

    raster = featureReader->GetRaster(L"Image");
    CPPUNIT_ASSERT(raster->GetImageXSize() == 600);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 800);

    ba = raster->GetBounds();
    geometery = geomFactory->CreateGeometryFromFgf(ba);
    bounds = geometery->GetEnvelope();

    CPPUNIT_ASSERT(bounds->GetMinX() == 100000.000000 - 600);
    CPPUNIT_ASSERT(bounds->GetMinY() == 100000.000000);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 100000.000000) < 0.0001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 100800.000000) < 0.0001);

    // feature "photo11_4", rotation = 90, resolution = 2;
    CPPUNIT_ASSERT(featureReader->ReadNext());

    raster = featureReader->GetRaster(L"Image");
    CPPUNIT_ASSERT(raster->GetImageXSize() == 600);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 800);

    ba = raster->GetBounds();
    geometery = geomFactory->CreateGeometryFromFgf(ba);
    bounds = geometery->GetEnvelope();

    CPPUNIT_ASSERT(bounds->GetMinX() == 100000.000000 - 1200);
    CPPUNIT_ASSERT(bounds->GetMinY() == 100000.000000);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxX() - 100000.000000) < 0.0001);
    CPPUNIT_ASSERT(fabs(bounds->GetMaxY() - 101600.000000) < 0.0001);

    connection->Close();
}

void RfpTestBandConfig::testMissingXmlTag()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
	
    bool bError = false;

    // missing <Location>
    try
    {
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/missing_location.xml", L"r");
        connection->SetConfiguration(stream);
        connection->Open();
    }
    catch(...)
    {
        bError = true;
        connection->Close();
    }
    CPPUNIT_ASSERT(bError == true);

    // missing <Feature>
    bError = false;
    try
    {
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/missing_feature.xml", L"r");
        connection->SetConfiguration(stream);
        connection->Open();
    }
    catch(...)
    {
        bError = true;
        connection->Close();
    }
    CPPUNIT_ASSERT(bError == true);

    // missing <Band>
    bError = false;
    try
    {
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/missing_band.xml", L"r");
        connection->SetConfiguration(stream);
        connection->Open();
    }
    catch(...)
    {
        bError = true;
        connection->Close();
    }
    CPPUNIT_ASSERT(bError == true);

    // missing <Image>
    bError = false;
    try
    {
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/missing_image.xml", L"r");
        connection->SetConfiguration(stream);
        connection->Open();
    }
    catch(...)
    {
        bError = true;
        connection->Close();
    }
    CPPUNIT_ASSERT(bError == true);
}
