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
#include "RfpTestMosaic.h"

RfpTestMosaic::RfpTestMosaic()
{
}

RfpTestMosaic::~RfpTestMosaic()
{
}

void RfpTestMosaic::_setUp()
{
}

void RfpTestMosaic::_tearDown()
{
}

#define orgX 200000.0
#define orgY 1000000.0

void RfpTestMosaic::testClippedMosaic()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"4_4.xml", L"r");
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

	// MUST contains one record
	CPPUNIT_ASSERT( dataReader->ReadNext() == true);
	
	CPPUNIT_ASSERT(dataReader->GetPropertyCount() == 1);
	CPPUNIT_ASSERT(STRCASEEQ(dataReader->GetPropertyName(0), L"stitchedRaster"));
	CPPUNIT_ASSERT(dataReader->GetPropertyType(L"stitchedRaster") == FdoPropertyType_RasterProperty);

	// use alias to refer the stitched raster
	FdoPtr<FdoIRaster> raster = dataReader->GetRaster(L"stitchedRaster");

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

void RfpTestMosaic::testMosaic4by4()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"4_4.xml", L"r");
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


	connection->Close();

}

void RfpTestMosaic::testResample ()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"4_4.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
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
	FdoPtr<FdoDataValue> height = FdoDataValue::Create(500);
	funcParams->Add(height);
	FdoPtr<FdoDataValue> width = FdoDataValue::Create(500);
	funcParams->Add(width);
	FdoPtr<FdoFunction> resampleFunc = FdoFunction::Create(L"RESAMPLE", funcParams);
	FdoPtr<FdoComputedIdentifier> resampleIdentifier = FdoComputedIdentifier::Create(L"resampledRaster", resampleFunc );

	// add it to the properties to select
	propsToSelect->Add(resampleIdentifier);
	FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute();

	// MUST contains one record
	CPPUNIT_ASSERT( featReader->ReadNext() == true);

	FdoPtr<FdoClassDefinition> clsDef = featReader->GetClassDefinition ();
	FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
	FdoInt32 cntProps = props->GetCount ();
	CPPUNIT_ASSERT (cntProps == 2);

	// use alias to refer the stitched raster
	FdoPtr<FdoIRaster> raster = featReader->GetRaster(L"resampledRaster");

	// verify the bounds
    FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> ba = raster->GetBounds();
    FdoPtr<FdoIGeometry> geometry = geomFactory->CreateGeometryFromFgf(ba);
    FdoPtr<FdoIEnvelope> bounds = geometry->GetEnvelope();

    CPPUNIT_ASSERT(bounds->GetMinX() == 201500.0);
	CPPUNIT_ASSERT(bounds->GetMinY() == 1001100.0);
	CPPUNIT_ASSERT(bounds->GetMaxX() == 201600.0);
	CPPUNIT_ASSERT(bounds->GetMaxY() == 1001200.0);

	// verify the image size
	CPPUNIT_ASSERT(raster->GetImageXSize() == 500);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 500);

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
	CPPUNIT_ASSERT(numRead == 500 * 500 * 4);

	// use RESAMPLE with aggregates select
	FdoICommand* cmdAggregates = connection->CreateCommand(FdoCommandType_SelectAggregates);
	FdoPtr<FdoISelectAggregates> cmdSelectAggregates = static_cast<FdoISelectAggregates*>(cmdAggregates);

	cmdSelectAggregates->SetFeatureClassName(L"Photo");

	cmdSelectAggregates->SetFilter(filter);

	FdoPtr<FdoIdentifierCollection> propsToSelectAggregates = cmdSelectAggregates->GetPropertyNames();

	// create the nested MOSAIC function that contains RESAMPLE function
	funcParams = FdoExpressionCollection::Create();
	funcParams->Add(resampleFunc);
	FdoPtr<FdoFunction> mosaicFunc = FdoFunction::Create(L"MOSAIC", funcParams);
	FdoPtr<FdoComputedIdentifier> mosaicIdentifier = FdoComputedIdentifier::Create(L"resampledRaster", mosaicFunc );
	
	// add it to the properties to select
	propsToSelectAggregates->Add(mosaicIdentifier);
	FdoPtr<FdoIDataReader> dataReader = cmdSelectAggregates->Execute();
	CPPUNIT_ASSERT (dataReader->ReadNext() == true);
	raster = dataReader->GetRaster (L"resampledRaster");

	ba = raster->GetBounds ();
	geometry = geomFactory->CreateGeometryFromFgf (ba);
	bounds = geometry->GetEnvelope ();

	// verify the bounds and image size
	CPPUNIT_ASSERT(bounds->GetMinX() == 201500.0);
	CPPUNIT_ASSERT(bounds->GetMinY() == 1001100.0);
	CPPUNIT_ASSERT(bounds->GetMaxX() == 202500.0);
	CPPUNIT_ASSERT(bounds->GetMaxY() == 1001900.0);

	CPPUNIT_ASSERT(raster->GetImageXSize() == 500);
	CPPUNIT_ASSERT(raster->GetImageYSize() == 500);

	// read the raster data
	streamReader = raster->GetStreamReader();
	reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	
	numRead = 0;
	do {
		FdoInt32 bytesRead = reader->ReadNext(buffer, 0, bytesPerTileRow);
		numRead += bytesRead;
		if (bytesRead == 0)
			break;
	} while (true);
	CPPUNIT_ASSERT(numRead == 500 * 500 * 4);

	delete [] buffer;
}

