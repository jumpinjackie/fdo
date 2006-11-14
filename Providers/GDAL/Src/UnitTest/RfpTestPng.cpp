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
 *         $Id: //Providers/RasterFile/src/UnitTest/RfpTestPng.cpp#1 $
 *     $Author: miaoya $
 *   $DateTime: 2005/03/17 19:58:08 $
 *     $Change: 6738 $
 *
 */

#include "UnitTest.h"
#include "RfpTestPng.h"

RfpTestPng::RfpTestPng()
{
}

RfpTestPng::~RfpTestPng()
{
}

void RfpTestPng::_setUp()
{
}

void RfpTestPng::_tearDown()
{
}

void RfpTestPng::testLoad()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	connection->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/PNG");
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"default");
	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    FdoInt32 count=0;
    while (featureReader->ReadNext())
    {
        FdoStringP name = featureReader->GetString(L"FeatId");
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Raster");
        FdoInt32 xsize = raster->GetImageXSize();
        FdoInt32 ysize = raster->GetImageYSize();

	    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
        FdoInt32 ppb = dataModel->GetBitsPerPixel();
	    FdoRasterDataModelType dmType = dataModel->GetDataModelType();

	    FdoInt32 xTileSize = dataModel->GetTileSizeX();
	    FdoInt32 yTileSize = dataModel->GetTileSizeY();

	    dataModel->SetTileSizeX(1024);
	    dataModel->SetTileSizeY(1024);

	    raster->SetDataModel(dataModel);

	    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	
	    int numTileRows = (xsize - 1) / 1024 + 1;
	    int numTileCols = (ysize - 1) / 1024 + 1;
	    const int bytesTile = 1024 * 1024 * 1;

	    FdoByte* buffer = new FdoByte[bytesTile];
	    for (int i = 0; i < numTileRows; i++)
	    {
		    // read the first tile of the tile row
		    FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
		    CPPUNIT_ASSERT(numRead == bytesTile);
		    // skip the rest tiles
		    reader->Skip(bytesTile * (numTileCols - 1));
	    }
        
	    // no data
	    CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
	    delete buffer;
        count++;
    }

    CPPUNIT_ASSERT(count == 11);

	connection->Close();
}

void RfpTestPng::testSpecialCharacters()
{
	try
    {
        FdoPtr<FdoIConnection> connection = CreateConnection();
	    connection->SetConnectionString(L"DefaultRasterFileLocation=\"../../TestData/PNG/COTcvdm.U (.!,@#$%^()_+=-`~[]').png\"");
	    connection->Open();

	    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"default");
	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

        FdoInt32 count=0;
        while (featureReader->ReadNext())
        {
            FdoStringP name = featureReader->GetString(L"FeatId");
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Raster");
            FdoInt32 xsize = raster->GetImageXSize();
            FdoInt32 ysize = raster->GetImageYSize();
	        CPPUNIT_ASSERT(xsize == 2227);
	        CPPUNIT_ASSERT(ysize == 2267);

	        FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
            FdoInt32 ppb = dataModel->GetBitsPerPixel();
	        CPPUNIT_ASSERT(ppb == 8);
	        FdoRasterDataModelType dmType = dataModel->GetDataModelType();
	        CPPUNIT_ASSERT(dmType == FdoRasterDataModelType_Palette);

	        FdoInt32 xTileSize = dataModel->GetTileSizeX();
	        FdoInt32 yTileSize = dataModel->GetTileSizeY();
	        CPPUNIT_ASSERT(xTileSize == 2227);
	        CPPUNIT_ASSERT(yTileSize == 1);

	        dataModel->SetTileSizeX(1024);
	        dataModel->SetTileSizeY(1024);

	        raster->SetDataModel(dataModel);

	        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	
	        int numTileRows = (xsize - 1) / 1024 + 1;
	        int numTileCols = (ysize - 1) / 1024 + 1;
	        const int bytesTile = 1024 * 1024 * 1;

	        FdoByte* buffer = new FdoByte[bytesTile];
	        for (int i = 0; i < numTileRows; i++)
	        {
		        // read the first tile of the tile row
		        FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
		        CPPUNIT_ASSERT(numRead == bytesTile);
		        // skip the rest tiles
		        reader->Skip(bytesTile * (numTileCols - 1));
	        }
            
	        // no data
	        CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
	        delete buffer;
            count++;
        }

        CPPUNIT_ASSERT(count == 1);

	    connection->Close();
    }
	catch (FdoException* e) 
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
	}
}
