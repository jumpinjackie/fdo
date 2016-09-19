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
 *
 * --------------------------------------------------------------------------
 *
 * This file contains tests of the spatial query ability - in particular it
 * tests this with and without <Bounds> specified in the config file.
 */

#include "UnitTest.h"
#include "RfpTestSpatialQuery.h"

RfpTestSpatialQuery::RfpTestSpatialQuery()
{
}

RfpTestSpatialQuery::~RfpTestSpatialQuery()
{
}

void RfpTestSpatialQuery::_setUp()
{
}

void RfpTestSpatialQuery::_tearDown()
{
}

// Test a spatial query against a config file with bounds specified.
//
// The config file contains two photos, one within the bounds (MtJefferson) 
// and one outside the bounds (cupertino_missing).  We verify we only get
// MtJefferson. 
//
// We also verify that the other features isn't actually accessed on disk,
// since the file doesn't exist.  If it were accessed at all, an exception
// would occur during processing. 

void RfpTestSpatialQuery::testConfigWithBounds()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/DEM/dem_bounds.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");

    FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
                                                                  FdoSpatialOperations_Intersects, 
                                                                  FdoRfpRect(0, 0, 250000, 2000000));
    cmdSelect->SetFilter(filter);


    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    CPPUNIT_ASSERT(featureReader->ReadNext());

    // Confirm we get the expected result.

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoInt32 xSize = raster->GetImageXSize();
    FdoInt32 ySize = raster->GetImageYSize();
    CPPUNIT_ASSERT(raster->GetImageXSize() == 337);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 469);

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 16);

    // Confirm we only get the one hit.

    CPPUNIT_ASSERT(featureReader->ReadNext() == 0);

    connection->Close();
}

// Test a spatial query against a config file without bounds specified.
//
// Similar to last test, but we use a config file without bounds, and
// we have to reference files that actually exist since they will both 
// be opened and accessed.

void RfpTestSpatialQuery::testConfigWithoutBounds()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/DEM/dem_nobounds.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");

    FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
                                                                  FdoSpatialOperations_Intersects, 
                                                                  FdoRfpRect(0, 0, 250000, 2000000));
    cmdSelect->SetFilter(filter);


    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    CPPUNIT_ASSERT(featureReader->ReadNext());

    // Confirm we get the expected result.

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoInt32 xSize = raster->GetImageXSize();
    FdoInt32 ySize = raster->GetImageYSize();
    CPPUNIT_ASSERT(raster->GetImageXSize() == 337);
    CPPUNIT_ASSERT(raster->GetImageYSize() == 469);

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 16);

    // Confirm we only get the one hit.

    CPPUNIT_ASSERT(featureReader->ReadNext() == 0);

    connection->Close();
}

// Test a spatial query against a directory with no config file.
//
// We use the TestData/rotated files since they are spatial distributed with inherent 
// georeferencing. 

void RfpTestSpatialQuery::testWithoutConfig()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    connection->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/rotated");
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
    cmdSelect->SetFeatureClassName(L"default");

    FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Raster", 
                                                                  FdoSpatialOperations_Intersects, 
                                                                  FdoRfpRect(-10,-10,10,10));
    cmdSelect->SetFilter(filter);


    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    CPPUNIT_ASSERT(featureReader->ReadNext());

    // Confirm we get the expected result.

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Raster");

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 24);

    // Confirm we only get the one hit.

    CPPUNIT_ASSERT(featureReader->ReadNext() == 0);

    connection->Close();
}

