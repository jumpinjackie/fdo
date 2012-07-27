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
#include "RfpTestGetRaster.h"
#include <iostream>
using namespace std;


RfpTestGetRaster::RfpTestGetRaster(void)
{
}

RfpTestGetRaster::~RfpTestGetRaster(void)
{
}

void RfpTestGetRaster::_setUp()
{
    m_connection = CreateConnection();
    CPPUNIT_ASSERT(m_connection != NULL);
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"4_4.xml", L"r");
    m_connection->SetConfiguration(stream);
    m_connection->Open();
}

void RfpTestGetRaster::_tearDown()
{
	m_connection->Close();
}

void RfpTestGetRaster::testGetRasterTileByTile()
{
    FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    while (featureReader->ReadNext())
    {
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
        FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
        dataModel->SetTileSizeX(128);
        dataModel->SetTileSizeY(128);
        raster->SetDataModel(dataModel);
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	
        for (int j = 0; j < 5; j++)
            for (int i = 0; i < 7; i++)
            {
                FdoInt32 tileWidth = 128;
                FdoInt32 tileHeight = 128;

                FdoInt32 countToRead = tileWidth * tileHeight * 3;
                FdoByte* data = new FdoByte[countToRead];
                CPPUNIT_ASSERT(reader->ReadNext(data, 0, countToRead) == countToRead);
                delete data;
            }

    }

}

void RfpTestGetRaster::testGetRasterRGB24()
{
    FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // Now try to read all data of the raster.
    FdoInt32 nRead = 0;
    // count for the rasters we have read. It must be 16 here.(4 x4)
    FdoInt32 nRasterCount = 0; 

    while (featureReader->ReadNext())
    {
        nRasterCount++;
        FdoIStreamReader* readers[2];
        // get the reader from two ways, one the GetRaster and the other is GetBLOB
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
        FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
        dataModel->SetDataModelType(FdoRasterDataModelType_RGBA);
        dataModel->SetBitsPerPixel(32);
        raster->SetDataModel(dataModel);
        readers[1] = raster->GetStreamReader();

        dataModel = raster->GetDataModel();
        dataModel->SetDataModelType(FdoRasterDataModelType_RGB);
        dataModel->SetBitsPerPixel(24);
        raster->SetDataModel(dataModel);
        readers[0] = raster->GetStreamReader();
		
        FdoPtr<FdoBLOBStreamReader> readerRGB = static_cast<FdoBLOBStreamReader*>(readers[0]);		
        FdoPtr<FdoBLOBStreamReader> readerRGBA = static_cast<FdoBLOBStreamReader*>(readers[1]);		
        FdoInt32 nReadSumRGB = 0, nReadSumRGBA = 0;		
        do 
        {
            FdoInt32 pixelRGB, pixelRGBA;
			
            nRead = readerRGB->ReadNext((FdoByte*)&pixelRGB, 0, 3);
            nReadSumRGB += nRead;
            readerRGB->Skip(2);
            readerRGB->Skip(2);
            readerRGB->Skip(2);

            nRead = readerRGBA->ReadNext((FdoByte*)&pixelRGBA, 0, 4);
            nReadSumRGBA += nRead;
            readerRGBA->Skip(2);
            readerRGBA->Skip(2);
            readerRGBA->Skip(4);



            CPPUNIT_ASSERT((pixelRGB & 0x00FFFFFF) == (pixelRGBA & 0x00FFFFFF));
        }
        while(nRead);

        break; //only test one raster because it is very time-consuming
				
    }
}


void RfpTestGetRaster::testGetRaster()
{
    FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // Now try to read all data of the raster.
    FdoByte buff[4096 * 2];	
    FdoInt32 nRead = 0;
    // count for the rasters we have read. It must be 16 here.(4 x4)
    FdoInt32 nRasterCount = 0; 

    while (featureReader->ReadNext())
    {
        nRasterCount++;
        FdoIStreamReader* readers[2];
        // get the reader from two ways, one the GetRaster and the other is GetBLOB
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
        readers[0] = raster->GetStreamReader();
        readers[1] = featureReader->GetLOBStreamReader(L"Image");
		
        for (int k = 0; k < 2; k++)
        {
            FdoInt32 nReadSum = 0;		
            FdoIStreamReader* streamReader = readers[k];
            FdoPtr<FdoBLOBStreamReader> reader = static_cast<FdoBLOBStreamReader*>(streamReader);		
            int count = 0;
            do 
            {
                nRead = reader->ReadNext(buff, 0, sizeof(buff)/sizeof(buff[0]));
                nReadSum += nRead;
                count++;
            }
            while(nRead);
					
            CPPUNIT_ASSERT(nReadSum == 800 * 600 * 3);
        }
    }

    CPPUNIT_ASSERT(nRasterCount == 4 * 4);
}

void RfpTestGetRaster::testGetRasterAllInOnce()
{
    FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // Now try to read all data of the raster.
    // read by tile
    const long SIZE = 5*7*128*128*3;
    FdoByte* buff = new FdoByte[SIZE];
    // read by row
    const long SIZE1 = 800 * 600 * 3;
    FdoInt32 nRead = 0;
    // count for the rasters we have read. It must be 16 here.(4x4)
    FdoInt32 nRasterCount = 0; 
	
    while (featureReader->ReadNext())
    {
        nRasterCount++;
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
        FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
        // read by row
        CPPUNIT_ASSERT(dataModel->GetTileSizeX() == 800);
        CPPUNIT_ASSERT(dataModel->GetTileSizeY() == 600);
        {
            FdoIStreamReader * streamReader = raster->GetStreamReader();
            FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);		
			
            nRead = reader->ReadNext(buff, 0, SIZE1);
            CPPUNIT_ASSERT(nRead == SIZE1);
            nRead = reader->ReadNext(buff, 0, 1);
            CPPUNIT_ASSERT(nRead == 0);
        }


        // read by tile
        // no tiling by default so we must retile by force
        dataModel->SetTileSizeX(128);
        dataModel->SetTileSizeY(128);
        raster->SetDataModel(dataModel);
        {
            FdoIStreamReader * streamReader = raster->GetStreamReader();
            FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);		
			
            nRead = reader->ReadNext(buff, 0, SIZE);
            CPPUNIT_ASSERT(nRead == SIZE);
            nRead = reader->ReadNext(buff, 0, 1);
            CPPUNIT_ASSERT(nRead == 0);
        }
    }

    CPPUNIT_ASSERT(nRasterCount == 4 * 4);
    delete[] buff;
}

#define orgX 200000.0
#define orgY 1000000.0


void RfpTestGetRaster::testGetClippedAndScaledRaster()
{
    FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
                                                                  FdoSpatialOperations_Intersects, 
                                                                  FdoRfpRect(orgX + 100, orgY + 100, orgX + 700.0, orgY + 500.0));
    cmdSelect->SetFilter(filter);
    // set up clip function: CLIP(Image, orgX + 100, orgY + 100, orgX + 700.0, orgY + 500.0)
    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
    funcParams->Add(rasterProp);
    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(orgX + 100, FdoDataType_Double);
    funcParams->Add(minX);
    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(orgY + 100, FdoDataType_Double);
    funcParams->Add(minY);
    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create((FdoInt32(orgX + 700)));
    funcParams->Add(maxX);
    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(orgY + 500, FdoDataType_Double);
    funcParams->Add(maxY);
    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", clipFunc );
    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    // add it to the properties to select
    propsToSelect->Add(clipIdentifier);

    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    // verify the computed class definition
    FdoPtr<FdoClassDefinition> classDef = featureReader->GetClassDefinition();
    CPPUNIT_ASSERT(classDef->GetIsComputed());
    FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
    CPPUNIT_ASSERT(props->GetCount() == 2); // Id and clippedRaster

    FdoPtr<FdoPropertyDefinition> idProp = props->GetItem(0);
    CPPUNIT_ASSERT(STRCASEEQ(idProp->GetName(), L"Id"));
    FdoPtr<FdoPropertyDefinition> computedRasterProp = props->GetItem(1);
    CPPUNIT_ASSERT(STRCASEEQ(computedRasterProp->GetName(), L"clippedRaster"));
    CPPUNIT_ASSERT(computedRasterProp->GetPropertyType() == FdoPropertyType_RasterProperty);


    CPPUNIT_ASSERT(featureReader->ReadNext() == true); 
    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"clippedRaster");
    CPPUNIT_ASSERT(raster->GetImageXSize() == 600);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 400);

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    // read by row
    CPPUNIT_ASSERT(dataModel->GetTileSizeX() == 600);
    CPPUNIT_ASSERT(dataModel->GetTileSizeY() == 400);
    FdoIStreamReader * streamReader = raster->GetStreamReader();
    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);		

    long SIZE = 600 * 400 * 3;
    FdoByte* buff = new FdoByte[SIZE];

    FdoInt32 nRead = reader->ReadNext(buff, 0, SIZE);
    CPPUNIT_ASSERT(nRead == SIZE);
    nRead = reader->ReadNext(buff, 0, 1);
    CPPUNIT_ASSERT(nRead == 0);

    // scale it to half of original one
    raster->SetImageXSize(300);
    raster->SetImageYSize(200);
    dataModel = raster->GetDataModel();
    // read by row
    CPPUNIT_ASSERT(dataModel->GetTileSizeX() == 300);
    CPPUNIT_ASSERT(dataModel->GetTileSizeY() == 200);
    streamReader = raster->GetStreamReader();
    reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);		

    SIZE = 300 * 200 * 3;

    nRead = reader->ReadNext(buff, 0, SIZE);
    CPPUNIT_ASSERT(nRead == SIZE);
    nRead = reader->ReadNext(buff, 0, 1);
    CPPUNIT_ASSERT(nRead == 0);

    delete buff;

    // only contains one record
    CPPUNIT_ASSERT(featureReader->ReadNext() == false);

}

void RfpTestGetRaster::testGetRotatedRaster()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    connection->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/rotated");
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"default:default");

    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
    int count = 0;
    while (featureReader->ReadNext())
    {
        FdoString* id = featureReader->GetString(L"default.FeatId");
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"default:default.Raster");

        FdoInt32 sizeX = raster->GetImageXSize();
        FdoInt32 sizeY = raster->GetImageYSize();

        CPPUNIT_ASSERT(sizeX == sizeY); // because the raster is rotated by 45 degree, so...

        FdoIStreamReader* reader = raster->GetStreamReader();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader1 = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(reader);	
		
        FdoInt32 length = (FdoInt32)reader1->GetLength();
        FdoByte* data = new FdoByte[(size_t)length];
        CPPUNIT_ASSERT(reader1->ReadNext(data, 0, length) == length);
        CPPUNIT_ASSERT(reader1->ReadNext(data, 0, 1) == 0); //no data left
        delete[] data;

        count++;

    }
    CPPUNIT_ASSERT(count == 4);
    featureReader->Close();


}
