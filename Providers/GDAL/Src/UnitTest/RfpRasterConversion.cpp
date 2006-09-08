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
#include "RfpTestRasterConversion.h"
#include <math.h>

RfpTestRasterConversion::RfpTestRasterConversion()
{
}

RfpTestRasterConversion::~RfpTestRasterConversion()
{
}

void RfpTestRasterConversion::_setUp()
{
	m_connection = CreateConnection();
	CPPUNIT_ASSERT(m_connection != NULL);
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/RGB/RGB.xml", L"r");
	m_connection->SetConfiguration(stream);
	m_connection->Open();
	
	// execute the query
	FdoICommand * cmd = m_connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);

	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIFeatureReader> reader = cmdSelect->Execute();
	reader->ReadNext();

	// get the raster
	m_raster = reader->GetRaster(L"Image");

	reader->Close();


}

void RfpTestRasterConversion::_tearDown()
{
	m_raster = NULL;
	m_connection->Close();	
}

void RfpTestRasterConversion::testRgbToBitonal()
{
	// change the data model and tile size
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	dataModel->SetDataModelType(FdoRasterDataModelType_Bitonal);
	dataModel->SetBitsPerPixel(1);
	dataModel->SetTileSizeX(512);
	dataModel->SetTileSizeY(512);

        try 
        { 
            m_raster->SetDataModel(dataModel);
        }
        catch(FdoException*){/* Ok, expected exception was caught.*/}
        catch(...) { CPPUNIT_ASSERT_MESSAGE ("FdoException expected but got none.", false); }
        
#ifdef notdef
// this might become useful if we enable bitonal support.

	// get the data
	FdoIStreamReader * streamReader = m_raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	const FdoInt32 pixelsPerStrip = 512 * 40;
	const FdoInt32 bytesPerStrip = pixelsPerStrip / 8;
	FdoByte buffer[bytesPerStrip];
	// read the strips (black, red, green, blue, white)
	for (int strip = 0; strip < 5; strip++)
	{
		reader->ReadNext(buffer, 0, bytesPerStrip);
	}
	// skip the pads
	reader->Skip((512 - 200) * 512 / 8);

	// there is no data left
	CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
#endif
}

#define GRAY_COLOR_DIFF(c1, c2) ((int)(c1) > (int)(c2) ? ((int)(c1) - (int)(c2)) : ((int)(c2) - (int)(c1)))

void RfpTestRasterConversion::testRgbToGray()
{
	// change the data model and tile size
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	dataModel->SetDataModelType(FdoRasterDataModelType_Gray);
	dataModel->SetBitsPerPixel(8);
	dataModel->SetTileSizeX(256);
	dataModel->SetTileSizeY(256);
	m_raster->SetDataModel(dataModel);

	// get the data
	FdoIStreamReader * streamReader = m_raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	// read the strips (black, red, green, blue, white)
	FdoByte pixelInStrip[] = {0x00, 0x4c, 0x96, 0x1d, 0xff};
	for (int strip = 0; strip < 5; strip++)
	{
		// read the data pixel by pixel
		for (int row = 0; row < 40; row++)
		{
			for (int i = 0; i < 200; i++)
			{
				FdoByte pixel;
				reader->ReadNext(&pixel, 0, 1);
				FdoInt32 colorDiff = (FdoInt32)GRAY_COLOR_DIFF(pixel, pixelInStrip[strip]);
				CPPUNIT_ASSERT(colorDiff < 227);
			}
			reader->Skip(56);
		}
	}
	reader->Skip(256 * 56);

	// there is no data left
	FdoByte pixel;
	CPPUNIT_ASSERT(reader->ReadNext(&pixel, 0, 1) == 0);


}

#define RGB_COLOR_DIFF(c1, c2) sqrt((((c1 & 0x000000ff) - (c2 & 0x000000ff)) * ((c1 & 0x000000ff) - (c2 & 0x000000ff)) + \
					(((c1 >> 8) & 0x000000ff) - ((c2 >> 8) & 0x000000ff)) * (((c1 >> 8) & 0x000000ff) - ((c2 >> 8) & 0x000000ff)) + \
					(((c1 >> 16) & 0x000000ff) - ((c2 >> 16) & 0x000000ff)) * (((c1 >> 16) & 0x000000ff) - ((c2 >> 16) & 0x000000ff))\
					) / 3.0)

void RfpTestRasterConversion::testScaling()
{
	// scale to half size of the original image
	m_raster->SetImageXSize(100);
	m_raster->SetImageYSize(100);
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	dataModel->SetTileSizeX(128);
	dataModel->SetTileSizeY(128);
	dataModel->SetBitsPerPixel(32);
	dataModel->SetDataModelType(FdoRasterDataModelType_RGBA);
	m_raster->SetDataModel(dataModel);

	// read the raster data
	FdoIStreamReader * streamReader = m_raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	
	// read the strips (black, red, green, blue, white)
	FdoInt32 pixelInStrip[] = {0x00000000, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00ffffff};
	for (int strip = 0; strip < 5; strip++)
	{
		// read the data pixel by pixel
		for (int row = 0; row < 19; row++)
		{
			for (int i = 0; i < 100; i++)
			{
				FdoInt32 pixel;
				reader->ReadNext((FdoByte*)&pixel, 0, 4);
				FdoInt32 colorDiff = (FdoInt32)RGB_COLOR_DIFF(pixel, pixelInStrip[strip]);
				CPPUNIT_ASSERT(colorDiff < 0x10);
			}
			reader->Skip(28*4);
		}
                reader->Skip(128 * 4); // last line of first strip mix of red and black.

	}
	reader->Skip(28 * 128 * 4);

	// there is no data left
	FdoByte dummy;
	CPPUNIT_ASSERT(reader->ReadNext(&dummy, 0, 1) == 0);

}

void RfpTestRasterConversion::testScalingAndToGray()
{
	// scale to half size of the original image
	m_raster->SetImageXSize(100);
	m_raster->SetImageYSize(100);
	// change the data model and tile size
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	dataModel->SetDataModelType(FdoRasterDataModelType_Gray);
	dataModel->SetBitsPerPixel(8);
	dataModel->SetTileSizeX(256);
	dataModel->SetTileSizeY(256);
	m_raster->SetDataModel(dataModel);

	// get the data
	FdoIStreamReader * streamReader = m_raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	// read the strips (black, red, green, blue, white)
        // current logic just takes the first band for greyscale.
	//FdoByte pixelInStrip[] = {0x00, 0x4c, 0x96, 0x1d, 0xff};
	FdoByte pixelInStrip[] = {0, 254, 0, 0, 255 };
	for (int strip = 0; strip < 5; strip++)
	{
		// read the data pixel by pixel
		for (int row = 0; row < 19; row++)
		{
			for (int i = 0; i < 100; i++)
			{
				FdoByte pixel;
				reader->ReadNext(&pixel, 0, 1);
				FdoInt32 colorDiff = (FdoInt32)GRAY_COLOR_DIFF(pixel, pixelInStrip[strip]);
				CPPUNIT_ASSERT(colorDiff < 0x15);
			}
			reader->Skip(156);
		}
                reader->Skip(256); // last line of first strip mix of red and black.

	}

	reader->Skip(156 * 256);

	// there is no data left
	FdoByte dummy;
	CPPUNIT_ASSERT(reader->ReadNext(&dummy, 0, 1) == 0);

}

void RfpTestRasterConversion::testRgbToPalette()
{
	// change the data model and tile size
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	dataModel->SetDataModelType(FdoRasterDataModelType_Palette);
	dataModel->SetBitsPerPixel(8);
	dataModel->SetTileSizeX(256);
	dataModel->SetTileSizeY(256);
        try 
        { 
            m_raster->SetDataModel(dataModel);
        }
        catch(FdoException*){/* Ok, expected exception was caught.*/}
        catch(...) { CPPUNIT_ASSERT_MESSAGE ("FdoException expected but got none.", false); }

#ifdef notdef
// this might be useful later when palette support is implemented.
	// get the palette
	FdoPtr<FdoIRasterPropertyDictionary> propDict = m_raster->GetAuxiliaryProperties();
	FdoPtr<FdoDataValue> pal = propDict->GetProperty(L"Palette");
	CPPUNIT_ASSERT(pal->GetDataType() == FdoDataType_BLOB);
	FdoLOBValue* palLOB = static_cast<FdoLOBValue*>(pal.p);
	struct RgbColor
	{
		union {
			struct { FdoByte red; FdoByte green; FdoByte blue; FdoByte alpha; } rgba;
			FdoInt32 packed;
		};
	};
	
	FdoPtr<FdoByteArray> palArray = palLOB->GetData();
	RgbColor* palette = reinterpret_cast<RgbColor*>(palArray->GetData());
	FdoPtr<FdoDataValue> num = propDict->GetProperty(L"NumOfPaletteEntries");
	FdoInt32Value* numInt32 = static_cast<FdoInt32Value*>(num.p);
	FdoInt32 numEntries = numInt32->GetInt32();

    CPPUNIT_ASSERT(numEntries == 61);
    
	// get the data
	FdoIStreamReader * streamReader = m_raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	// read the strips (black, red, green, blue, white)
	FdoInt32 pixelInStrip[] = {0x00000000, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00ffffff};
	for (int strip = 0; strip < 5; strip++)
	{
		// read the data pixel by pixel
		for (int row = 0; row < 40; row++)
		{
			for (int i = 0; i < 200; i++)
			{
				FdoByte pal;
				reader->ReadNext(&pal, 0, 1);
				FdoInt32 pixel = palette[pal].packed;
				FdoInt32 colorDiff = (FdoInt32)RGB_COLOR_DIFF(pixel, pixelInStrip[strip]);
				CPPUNIT_ASSERT(colorDiff < 209);
			}
			reader->Skip(56);
		}
	}

	reader->Skip(56 * 256);

	// there is no data left
	FdoByte dummy;
	CPPUNIT_ASSERT(reader->ReadNext(&dummy, 0, 1) == 0);
#endif
}
