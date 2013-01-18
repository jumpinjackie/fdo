/*
 * (C) Copyright 2004 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 * Revision Control Modification History
 *
 *         $Id: //Providers/RasterFile/src/UnitTest/RfpTestAAIGird.cpp#1 $
 *     $Author: miaoya $
 *   $DateTime: 2005/03/17 19:58:08 $
 *     $Change: 6738 $
 *
 */

#include "UnitTest.h"
#include "RfpTestAAIGrid.h"

RfpTestAAIGird::RfpTestAAIGird()
{
}

RfpTestAAIGird::~RfpTestAAIGird()
{
}

void RfpTestAAIGird::_setUp()
{
}

void RfpTestAAIGird::_tearDown()
{
}
void RfpTestAAIGird::testDataStream()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/ESRI/ASCII/aaigrid.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	CPPUNIT_ASSERT(featureReader->ReadNext());

	FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoInt32 xSize = raster->GetImageXSize();
    FdoInt32 ySize = raster->GetImageYSize();
	CPPUNIT_ASSERT(xSize == 216);
	CPPUNIT_ASSERT(ySize == 196);

	FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
	CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 16);
	CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_Gray);

	CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
	CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());
	// retiling
	const FdoInt32 tileSizeX = 128, tileSizeY = 128;
	dataModel->SetTileSizeX(tileSizeX);
	dataModel->SetTileSizeY(tileSizeY);
	raster->SetDataModel(dataModel);

	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	
	int numTileRows = (196 - 1) / tileSizeX + 1;
	int numTileCols = (216 - 1) / tileSizeY + 1;
	const int bytesTile = tileSizeX * tileSizeY * 4;

	FdoByte* buffer = new FdoByte[bytesTile];
	for (int i = 0; i < numTileRows; i++)
	{
		// read the first tile of the tile row
		FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
		CPPUNIT_ASSERT(numRead == bytesTile);
	}
    
	// no data
	CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
	delete[] buffer;

	// scale down to 1/2 of original size and read it row by row
	raster->SetImageXSize(60);
	raster->SetImageYSize(60);
	dataModel->SetTileSizeX(raster->GetImageXSize());
	dataModel->SetTileSizeY(raster->GetImageYSize());
	raster->SetDataModel(dataModel);
	reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	

	// iterate all rows
	FdoByte buff[140]; //60*2
	for (int row = 0; row < 60; row++)
	{
		FdoInt32 numRead = reader->ReadNext(buff, 0, 120);
		CPPUNIT_ASSERT(numRead == 120);
	}
	CPPUNIT_ASSERT(reader->ReadNext(buff, 0, 1) == 0);

	connection->Close();
}

void RfpTestAAIGird::testGetNullValue()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/ESRI/ASCII/aaigrid.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	// the first feature is a jpg file
	CPPUNIT_ASSERT(featureReader->ReadNext());

	FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
	FdoDataValue* nullVal = raster->GetNullPixelValue();
	FdoPtr<FdoInt16Value> val = static_cast<FdoInt16Value*>(nullVal);

	CPPUNIT_ASSERT(nullVal->GetDataType() == FdoDataType_Int16);
	CPPUNIT_ASSERT(val->GetInt16 () == -9999);

	connection->Close();
}

#define orgX 200000.0
#define orgY 1000000.0
void RfpTestAAIGird::testClippedMosaic()
{
	wprintf (L"Testing: %ls\n\n", L"RfpTestAAIGird::testClippedMosaic()");
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/ESRI/ASCII/aaigrid_filters.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_SelectAggregates);
	FdoPtr<FdoISelectAggregates> cmdSelect = static_cast<FdoISelectAggregates*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Intersects, 
		FdoRfpRect(orgX + 50, orgY + 50, orgX + 1000, orgY + 1000));
	cmdSelect->SetFilter(filter);

	FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();

	// set up clip function: CLIP(Image, orgX + 50, orgY + 50, orgX + 1000.0, orgY + 1000.0)
	FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	funcParams->Add(rasterProp);
	FdoPtr<FdoDataValue> minX = FdoDataValue::Create(orgX + 50, FdoDataType_Double);
	funcParams->Add(minX);
	FdoPtr<FdoDataValue> minY = FdoDataValue::Create(orgY + 50, FdoDataType_Double);
	funcParams->Add(minY);
	FdoPtr<FdoDataValue> maxX = FdoDataValue::Create(orgX + 1000, FdoDataType_Double);
	funcParams->Add(maxX);
	FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(orgY + 1000, FdoDataType_Double);
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

	// MUST contains 1 records
	// verify the bounds
	FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoByteArray> ba = raster->GetBounds();
	FdoPtr<FdoIGeometry> geometery = geomFactory->CreateGeometryFromFgf(ba);
	FdoPtr<FdoIEnvelope> bounds = geometery->GetEnvelope();

	CPPUNIT_ASSERT(bounds->GetMinX() == 200050.0);
	CPPUNIT_ASSERT(bounds->GetMinY() == 1000050.0);
	CPPUNIT_ASSERT(bounds->GetMaxX() == 200216.0);
	CPPUNIT_ASSERT(bounds->GetMaxY() == 1000196.0);

	// verify the image size
	CPPUNIT_ASSERT(raster->GetImageXSize() == 166);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 146);

	// read the raster data
	FdoIStreamReader * streamReader = raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	

	FdoByte* buffer = new FdoByte[166*4];

	// read row by row
	FdoInt32 numRead = 0;
	for (int row = 0; row < 146; row++)
	{
		FdoInt32 numRead = reader->ReadNext(buffer, 0, 166*4);
		CPPUNIT_ASSERT(numRead == 166*4);			
	} 
	CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);


	connection->Close();
}
