// 
// Copyright (C) 2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 

#include "stdafx.h"


// CreateConnection demonstrates how to create a connection
FdoIConnection* CreateConnection()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
	// You must use the proper provider's name here
    return (manager->CreateConnection (L"OSGeo.Gdal.3.3"));
}

// OpenConfiguration demonstrates how to open the connection using the specified configuration file
void OpenConfiguration(FdoIConnection* connection, FdoString* configFileName)
{
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(configFileName, L"r");
	connection->SetConfiguration(stream);
	connection->Open();
}

// ReadRasterRowByRow demonstrates how to get raster data row by row 
void ReadRasterRowByRow(FdoIRaster* raster)
{
	FdoIStreamReader * streamReader = raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	

	// Read the raster data row by row
	// First figure out the count of bytes in one row
	FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
	FdoInt32 bytesPerRow = (raster->GetImageXSize() * dataModel->GetBitsPerPixel() + 7) / 8;
	/////////////////////////////////////////////////////////////////////
	// NOTE: RasterFile provider actually pads rows to 8 bytes boundaries
	/////////////////////////////////////////////////////////////////////
	bytesPerRow = (bytesPerRow + 7) / 8 * 8;

	FdoByte* buff = new FdoByte[bytesPerRow];
	FdoInt32 height = raster->GetImageYSize();
	for (int j = 0; j < height; j++)
	{
		int numRead = reader->ReadNext(buff, 0, bytesPerRow);
		// assert(numRead == bytesPerRow);
		// You can do further processing using the raster data here
		// ...

	}
	delete[] buff;

}

void ReadRasterTileByTile(FdoIRaster* raster)
{
    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();

    FdoInt32 tileXSize = dataModel->GetTileSizeX();
    FdoInt32 tileYSize = dataModel->GetTileSizeY();

    FdoIStreamReader* streamReader = raster->GetStreamReader();
	FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = static_cast<FdoIStreamReaderTmpl<FdoByte>*>(streamReader);	
	// now read raster data tile by tile
	// first figure out number of tile row and tile column
	FdoInt32 numTileX = (raster->GetImageXSize() + tileXSize - 1) / tileXSize;
	FdoInt32 numTileY = (raster->GetImageYSize() + tileYSize - 1) / tileYSize;

	FdoInt32 bytesPerTile = (tileXSize * dataModel->GetBitsPerPixel() + 7) / 8 * tileYSize;
	FdoByte* buff = new FdoByte[bytesPerTile];
	for (int i = 0; i < numTileX * numTileY; i++)
	{
		FdoInt32 numRead = reader->ReadNext(buff, 0, bytesPerTile);
		// assert(numRead == bytesPerTile)
		// You can do further processing using the raster data here
		// ...

	}
	delete buff;
}

// RetrieveScaledRaster demonstrates how to scale the raster
void RetrieveScaledRaster(FdoIRaster* raster, double scaleFactorX, double scaleFactorY)
{
	raster->SetImageXSize(FdoInt32(raster->GetImageXSize() * scaleFactorX));
	raster->SetImageYSize(FdoInt32(raster->GetImageYSize() * scaleFactorY));
}

// GetArbitraryRaster demonstrates how to request arbitrary from a virtual raster represented by connection, with
// the clipping bounds and the desired X/Y scale factors.
FdoIRaster* GetArbitraryRaster(FdoIConnection* connection, FdoString* className, FdoString* rasterPropertyName,
                        double minX, double minY, double maxX, double maxY, 
                        double scaleFactorX, double scaleFactorY)
{
    // our goal: To execute: 
    // SELECT CLIP(rasterPropertyName, minX, minY, maxX, maxY) as "clippedRaster"
    // FROM className WHERE Image Intersects (minX, minY, maxX, maxY)

    // to return single raster, we have to create Select Aggregate command first
	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_SelectAggregates);
    FdoPtr<FdoISelectAggregates> cmdSelect = static_cast<FdoISelectAggregates*>(cmd);
	cmdSelect->SetFeatureClassName(className);

	// set the spatial filter
	FdoPtr<FdoFgfGeometryFactory> fgfFactory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIEnvelope> envelope = fgfFactory->CreateEnvelopeXY(minX, minY, maxX, maxY);
	FdoPtr<FdoIGeometry> geometry = fgfFactory->CreateGeometry(envelope);
	FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(fgfFactory->GetFgf(geometry));
    // using spatial operator FdoSpatialOperations_Intersects
	FdoPtr<FdoFilter> filter = FdoSpatialCondition::Create(rasterPropertyName, 
        FdoSpatialOperations_Intersects, geoValue);

	cmdSelect->SetFilter(filter);

	// first set up CLIP function: CLIP(Image, minX, minY, maxX, maxY)
	FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(rasterPropertyName);
	funcParams->Add(rasterProp);
	FdoPtr<FdoDataValue> argMinX = FdoDataValue::Create(minX, FdoDataType_Double);
	funcParams->Add(argMinX);
	FdoPtr<FdoDataValue> argMinY = FdoDataValue::Create(minY, FdoDataType_Double);
	funcParams->Add(argMinY);
	FdoPtr<FdoDataValue> argMaxX = FdoDataValue::Create(maxX, FdoDataType_Double);
	funcParams->Add(argMaxX);
	FdoPtr<FdoDataValue> argMaxY = FdoDataValue::Create(maxY, FdoDataType_Double);
	funcParams->Add(argMaxY);
	FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);

	// and then create the computed identifier containing the CLIP function and the alias name
	FdoPtr<FdoComputedIdentifier> clippedIdentifier = FdoComputedIdentifier::Create( L"clippedRaster", clipFunc);
    
    // finally add the computed identifier to the properties to select
    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    propsToSelect->Add(clippedIdentifier);

    // execute the select aggregate command
	FdoPtr<FdoIDataReader> dataReader = cmdSelect->Execute();		

    FdoPtr<FdoIRaster> returnRaster;
    if (dataReader->ReadNext())
    {
        returnRaster = dataReader->GetRaster(L"clippedRaster");
        RetrieveScaledRaster(returnRaster, scaleFactorX, scaleFactorY);
    }

    return FDO_SAFE_ADDREF(returnRaster.p);

}

// This is the main proc of the RasterFile provider sample, which simulates the following scenario:
// We could deem a connection to RasterFile provider as a virtual huge raster. There is a window used
// to view the raster. The window has fixed size and the user can pan the window and also can zoom in and
// zoom out the virtual raster behind the window. And also the client application can display the returned 
// raster in whatever data models (RGB, Grayscale, Palette and Bitonal) he/she likes regardless of the
// original data model. But definitely you can not see colorful picture when the original data model is
// grayscale or bitonal.
//
void MainProc()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    OpenConfiguration(connection, L"airport.xml");

    // set up the original view bounds
    double minX = 0.0, minY = 0.0;
    double width = 100.0, height = 100.0;
    double scale = 1.0;

    // get the first raster
    FdoPtr<FdoIRaster> raster = GetArbitraryRaster(connection, L"Photo", L"Image", 
        minX, minY, minX + width / scale, minY + height / scale, scale, scale);
    // display the raster
    ReadRasterRowByRow(raster);

    // then the user pan rightward the view window a little bit
    minX += 20.0;

    // get the second raster
    raster = GetArbitraryRaster(connection, L"Photo", L"Image", 
        minX, minY, minX + width / scale, minY + height / scale, scale, scale);
    // view the raster in grayscale data model
    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
	dataModel->SetDataModelType(FdoRasterDataModelType_Gray);
	dataModel->SetBitsPerPixel(8);
	raster->SetDataModel(dataModel);
    // display the raster
    ReadRasterRowByRow(raster);

    // and then the user zoom in a little bit
    scale *= 1.2;

    // get the third raster
    raster = GetArbitraryRaster(connection, L"Photo", L"Image", 
        minX, minY, minX + width / scale, minY + height / scale, scale, scale);
    // retile the return raster
    dataModel = raster->GetDataModel();
    dataModel->SetTileSizeX(128);
    dataModel->SetTileSizeY(128);
    raster->SetDataModel(dataModel);
    // display the raster (Read raster tile by tile)
    ReadRasterTileByTile(raster);
    
    // and then the user pan the window again...
    
    // ...

    // and then zoom out a little bit...

    // ...

}


