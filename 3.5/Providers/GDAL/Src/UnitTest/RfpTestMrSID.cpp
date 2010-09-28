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
#include "RfpTestMrSID.h"

RfpTestMrSID::RfpTestMrSID()
{
}

RfpTestMrSID::~RfpTestMrSID()
{
}

void RfpTestMrSID::_setUp()
{
}

void RfpTestMrSID::_tearDown()
{
}

void RfpTestMrSID::testRGB()
{
    //FdoPtr<FdoIConnection> connection = CreateConnection();
    //FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/MrSID/Airport.xml", L"r");
    //connection->SetConfiguration(stream);
    //connection->Open();

    //FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    //FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
    //cmdSelect->SetFeatureClassName(L"Photo");
    //FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
    //CPPUNIT_ASSERT(featureReader->ReadNext());

    //FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    //CPPUNIT_ASSERT(raster->GetImageXSize() == 1895);
    //CPPUNIT_ASSERT(raster->GetImageYSize() == 1068);

    //FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    //CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 32);
    //CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_RGBA);

    //CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
    //CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());

    //// retiling
    //const FdoInt32 tileSizeX = 1024, tileSizeY = 1024;
    //dataModel->SetTileSizeX(tileSizeX);
    //dataModel->SetTileSizeY(tileSizeY);
    //raster->SetDataModel(dataModel);

    //FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());    
    //int numTileRows = (1068 - 1) / tileSizeX + 1;
    //int numTileCols = (1895 - 1) / tileSizeY + 1;
    //const int bytesTile = tileSizeX * tileSizeY * 4;

    //FdoByte* buffer = new FdoByte[bytesTile];
    //for (int i = 0; i < numTileRows; i++)
    //{
    //    // read the first tile of the tile row
    //    FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
    //    CPPUNIT_ASSERT(numRead == bytesTile);
    //    // skip the rest tiles
    //    reader->Skip(bytesTile * (numTileCols - 1));
    //}
    //
    //// no data
    //CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
    //delete buffer;

    //// scale down to 1/4 of original size and read it row by row
    //raster->SetImageXSize(473);
    //raster->SetImageYSize(266);
    //dataModel->SetTileSizeX(raster->GetImageXSize());
    //dataModel->SetTileSizeY(raster->GetImageYSize());
    //raster->SetDataModel(dataModel);
    //reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());    

    //// iterate all rows
    //FdoByte buff[1896];
    //for (int row = 0; row < 266; row++)
    //{
    //    FdoInt32 numRead = reader->ReadNext(buff, 0, 1896);
    //    CPPUNIT_ASSERT(numRead == 1896);
    //}
    //CPPUNIT_ASSERT(reader->ReadNext(buff, 0, 1) == 0);
    //connection->Close();
}

void RfpTestMrSID::testGray()
{
    //FdoPtr<FdoIConnection> connection = CreateConnection();
    //connection->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/MrSID/Grayscale");
    //connection->Open();

    //FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    //FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
    //cmdSelect->SetFeatureClassName(L"default");
    //FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
    //CPPUNIT_ASSERT(featureReader->ReadNext());

    //FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Raster");
    //FdoInt32 xsize = raster->GetImageXSize();
    //FdoInt32 ysize = raster->GetImageYSize();
    //CPPUNIT_ASSERT(raster->GetImageXSize() == 6464);
    //CPPUNIT_ASSERT(raster->GetImageYSize() == 7681);

    //FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    //CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 8);
    //CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_Gray);

    //CPPUNIT_ASSERT(dataModel->GetTileSizeX() == 6464);
    //CPPUNIT_ASSERT(dataModel->GetTileSizeY() == 7681);
    //// retiling
    //dataModel->SetTileSizeX(1024);
    //dataModel->SetTileSizeY(1024);
    //raster->SetDataModel(dataModel);

    //FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());    
    //int numTileRows = (7681 - 1) / 1024 + 1;
    //int numTileCols = (6464 - 1) / 1024 + 1;
    //const int bytesTile = 1024 * 1024 * 1;

    //FdoByte* buffer = new FdoByte[bytesTile];
    //for (int i = 0; i < numTileRows; i++)
    //{
    //    // read the first tile of the tile row
    //    FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
    //    CPPUNIT_ASSERT(numRead == bytesTile);
    //    // skip the rest tiles
    //    reader->Skip(bytesTile * (numTileCols - 1));
    //}
    //
    //// no data
    //CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
    //delete buffer;

    //connection->Close();
}

