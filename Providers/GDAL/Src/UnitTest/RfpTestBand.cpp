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
#include "RfpTestBand.h"

RfpTestBand::RfpTestBand()
{
}

RfpTestBand::~RfpTestBand()
{
}

void RfpTestBand::_setUp()
{
}

void RfpTestBand::_tearDown()
{
}

void RfpTestBand::testSelect()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/band.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	CPPUNIT_ASSERT(featureReader->ReadNext());
	// <Feature name="photo11_12">
	FdoStringP id = featureReader->GetString(L"Id");
	CPPUNIT_ASSERT(wcscmp(id, L"photo11_12") == 0);

	FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
	CPPUNIT_ASSERT(raster->GetNumberOfBands() == 2);

	// <Band name="photo1_1" number="1">
	raster->SetCurrentBand(1); // photo1_2.JPG

	CPPUNIT_ASSERT(raster->GetImageXSize() == 800);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 600);
	FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
	CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 24);
	CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_RGB);
	CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
	CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());


	// <Band name="photo1_2" number="2">
	raster->SetCurrentBand(2); // photo1_2.JPG
	CPPUNIT_ASSERT(raster->GetImageXSize() == 800);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 600);
	dataModel = raster->GetDataModel();
	CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 24);
	CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_RGB);
	CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
	CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());


	CPPUNIT_ASSERT(featureReader->ReadNext());

	// <Feature name="photo1_3">
	id = featureReader->GetString(L"Id");
	CPPUNIT_ASSERT(wcscmp(id, L"photo1_3") == 0);

	raster = featureReader->GetRaster(L"Image");
	CPPUNIT_ASSERT(raster->GetNumberOfBands() == 1);

	//<Band name="photo1_3" number="1">
	raster->SetCurrentBand(1); // photo1_3.JPG

	CPPUNIT_ASSERT(raster->GetImageXSize() == 800);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 600);
	dataModel = raster->GetDataModel();
	CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 24);
	CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_RGB);
	CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
	CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());

	connection->Close();
}


#define orgX 200000.0
#define orgY 1000000.0

void RfpTestBand::testClippedMosaic()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/4_4_band.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_SelectAggregates);
    FdoPtr<FdoISelectAggregates> cmdSelect = static_cast<FdoISelectAggregates*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
                                                                  FdoSpatialOperations_Intersects, 
                                                                  FdoRfpRect(orgX + 1500, orgY + 1100, orgX + 2500, orgY + 1900));
    cmdSelect->SetFilter(filter);

    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();

    // set up clip function: CLIP(Image, orgX + 1500, orgY + 1100, orgX + 2500.0, orgY + 1900.0)
    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
    funcParams->Add(rasterProp);
    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(orgX + 1500, FdoDataType_Double);
    funcParams->Add(minX);
    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(orgY + 1100, FdoDataType_Double);
    funcParams->Add(minY);
    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create(orgX + 2500, FdoDataType_Double);
    funcParams->Add(maxX);
    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(orgY + 1900, FdoDataType_Double);
    funcParams->Add(maxY);
    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", clipFunc );

    // create the nested MOSAIC function that contains CLIP function
    funcParams = FdoExpressionCollection::Create();
    funcParams->Add(clipFunc); // you could add either clipFunc or clipIdentifier here
    FdoPtr<FdoFunction> mosaicFunc = FdoFunction::Create(L"MOSAIC", funcParams);
    FdoPtr<FdoComputedIdentifier> mosaicIdentifier = FdoComputedIdentifier::Create(L"stitchedRaster", mosaicFunc );
    // add it to the properties to select
    propsToSelect->Add(mosaicIdentifier);
    FdoPtr<FdoIDataReader> dataReader = cmdSelect->Execute();

	
    CPPUNIT_ASSERT( dataReader->ReadNext() == true);
		
    CPPUNIT_ASSERT(dataReader->GetPropertyCount() == 1);
    CPPUNIT_ASSERT(STRCASEEQ(dataReader->GetPropertyName(0), L"stitchedRaster"));
    CPPUNIT_ASSERT(dataReader->GetPropertyType(L"stitchedRaster") == FdoPropertyType_RasterProperty);

    // use alias to refer the stitched raster
    FdoPtr<FdoIRaster> raster = dataReader->GetRaster(L"stitchedRaster");
    // MUST contains 3 records
    // MUST contains 3 records
    FdoInt32 band_i;
    for (band_i=1; band_i<=3; band_i++)
    {
        raster->SetCurrentBand(band_i);

        // verify the bounds
        FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoByteArray> ba = raster->GetBounds();
        FdoPtr<FdoIGeometry> geometery = geomFactory->CreateGeometryFromFgf(ba);
        FdoPtr<FdoIEnvelope> bounds = geometery->GetEnvelope();

        CPPUNIT_ASSERT(bounds->GetMinX() == 201500.0);
        CPPUNIT_ASSERT(bounds->GetMinY() == 1001100.0);
        CPPUNIT_ASSERT(bounds->GetMaxX() == 202500.0);
        CPPUNIT_ASSERT(bounds->GetMaxY() == 1001900.0);

        // verify the image size
        CPPUNIT_ASSERT(raster->GetImageXSize() == 1000);
        CPPUNIT_ASSERT(raster->GetImageYSize() == 800);

        // read the raster data
        FdoIStreamReader * streamReader = raster->GetStreamReader();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	

        FdoInt32 bytesPerTileRow = 25*128*128*4;
        FdoByte* buffer = new FdoByte[bytesPerTileRow];
		
        // read row by row
        FdoInt32 numRead = 0;
        do {
            FdoInt32 bytesRead = reader->ReadNext(buffer, 0, bytesPerTileRow);
            numRead += bytesRead;
            if (bytesRead == 0)
                break;
        } while (true);
        CPPUNIT_ASSERT(numRead == 1000 * 800 * 4);
    }

    connection->Close();
}

void RfpTestBand::testMosaic4by4()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/4_4_band_1.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_SelectAggregates);
	FdoPtr<FdoISelectAggregates> cmdSelect = static_cast<FdoISelectAggregates*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
	// create the computed identifier containing the MOSAIC function and the alias name
	FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	funcParams->Add(rasterProp);
	FdoPtr<FdoFunction> mosaicFunc = FdoFunction::Create(L"MOSAIC", funcParams);
	FdoPtr<FdoComputedIdentifier> mosaicIdentifier = FdoComputedIdentifier::Create(L"stitchedRaster", mosaicFunc );
	// add it to the properties to select
	propsToSelect->Add(mosaicIdentifier);
	FdoPtr<FdoIDataReader> dataReader = cmdSelect->Execute();

	// MUST contains one record
	CPPUNIT_ASSERT( dataReader->ReadNext() == true);
	
	CPPUNIT_ASSERT(dataReader->GetPropertyCount() == 1);
	CPPUNIT_ASSERT(STRCASEEQ(dataReader->GetPropertyName(0), L"stitchedRaster"));
	CPPUNIT_ASSERT(dataReader->GetPropertyType(L"stitchedRaster") == FdoPropertyType_RasterProperty);

	// use alias to refer the stitched raster
	FdoPtr<FdoIRaster> raster = dataReader->GetRaster(L"stitchedRaster");

	// MUST contains 3 records
	FdoInt32 band_i;
	for (band_i=1; band_i<=3; band_i++)
	{
		raster->SetCurrentBand(band_i);

		// verify the bounds
        FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
	    FdoPtr<FdoByteArray> ba = raster->GetBounds();
        FdoPtr<FdoIGeometry> geometery = geomFactory->CreateGeometryFromFgf(ba);
        FdoPtr<FdoIEnvelope> bounds = geometery->GetEnvelope();

        CPPUNIT_ASSERT(bounds->GetMinX() == 200000.0);
		CPPUNIT_ASSERT(bounds->GetMinY() == 1000000.0);
		CPPUNIT_ASSERT(bounds->GetMaxX() == 203200.0);
		CPPUNIT_ASSERT(bounds->GetMaxY() == 1002400.0);

		// verify the image size
		CPPUNIT_ASSERT(raster->GetImageXSize() == 3200);
		CPPUNIT_ASSERT(raster->GetImageYSize() == 2400);

		// read the raster data
		FdoIStreamReader * streamReader = raster->GetStreamReader();
		FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	

		FdoInt32 bytesPerTileRow = 25*128*128*4;
		FdoByte* buffer = new FdoByte[bytesPerTileRow];
		
		// read row by row
		FdoInt32 numRead = 0;
		do {
			FdoInt32 bytesRead = reader->ReadNext(buffer, 0, bytesPerTileRow);
			numRead += bytesRead;
			if (bytesRead == 0)
				break;
		} while (true);
		CPPUNIT_ASSERT(numRead == 3200 * 2400 * 4);

		// read tile by tile
		FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
		dataModel->SetTileSizeX(128);
		dataModel->SetTileSizeY(128);
		raster->SetDataModel(dataModel);
		reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());

		for (int i = 0; i < 19; i ++)
		{
			FdoInt32 bytesRead = reader->ReadNext(buffer, 0, bytesPerTileRow);
			CPPUNIT_ASSERT(bytesRead == bytesPerTileRow);
		}
		// reach the end of the stream
		CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
		
		// test skip
		reader->Reset();
		for (int i = 0; i < 25 * 19 / 5; i++)
		{
			FdoInt32 bytesRead = reader->ReadNext(buffer, 0, 128*128*4);
			CPPUNIT_ASSERT(bytesRead == 128*128*4);
			reader->Skip(128*128*4*4);	//skip 4 tiles
		}

		// reach the end of the stream
		CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
		
		delete buffer;

		// CAN ONLY contains one record
		CPPUNIT_ASSERT( dataReader->ReadNext() == false);
	}

	connection->Close();

}
