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
#include "RfpTestGeotiff.h"

RfpTestGeotiff::RfpTestGeotiff()
{
}

RfpTestGeotiff::~RfpTestGeotiff()
{
}

void RfpTestGeotiff::_setUp()
{
}

void RfpTestGeotiff::_tearDown()
{
}

void RfpTestGeotiff::testGeotiff()
{
    try {
        FdoPtr<FdoIConnection> connection = CreateConnection();
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/pci_eg/config.xml", L"r");
        connection->SetConfiguration(stream);
        CPPUNIT_ASSERT( connection->Open() == FdoConnectionState_Open );
	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"Photo");

	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
	int count = 0;
	while (featureReader->ReadNext())
	{
            FdoString* id = featureReader->GetString(L"Id");
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");

            FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();

            FdoInt32 bpp = dataModel->GetBitsPerPixel();
            FdoRasterDataModelType type = dataModel->GetDataModelType();
            FdoInt32 tileX = dataModel->GetTileSizeX();
            FdoInt32 tileY = dataModel->GetTileSizeY();

            FdoIStreamReader* reader = raster->GetStreamReader();
            FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader1 = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(reader);	
		
            FdoInt32 length = (FdoInt32)reader1->GetLength();
            FdoByte* data = new FdoByte[(size_t)length];
            CPPUNIT_ASSERT(reader1->ReadNext(data, 0, length) == length);
            CPPUNIT_ASSERT(reader1->ReadNext(data, 0, 1) == 0); //no data left
            delete[] data;

            count++;

	}
	CPPUNIT_ASSERT(count == 1);
	featureReader->Close();
	connection->Close();
    } catch( FdoException * ge) {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
        CPPUNIT_ASSERT( 0 );
    }
}
