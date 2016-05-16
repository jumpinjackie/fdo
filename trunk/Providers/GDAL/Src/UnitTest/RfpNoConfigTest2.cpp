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
 */

#include "UnitTest.h"
#include "RfpNoConfigTest2.h"
#include <cmath>

RfpNoConfigTest2::RfpNoConfigTest2(void)
{
}

RfpNoConfigTest2::~RfpNoConfigTest2(void)
{
}

void RfpNoConfigTest2::_setUp()
{
	
}

void RfpNoConfigTest2::_tearDown()
{

}

void RfpNoConfigTest2::testSpatialContext()
{
    //We want to check spatial context extents from rasters w/o coordinate system info match extents returned by the gdalinfo utility

    //TestData/GeoTiff/AspectTest3
    FdoPtr<FdoIConnection> connTif = CreateConnection();
    FDO_CPPUNIT_ASSERT(connTif != NULL);

    connTif->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/GeoTiff/AspectTest3/AspectTest3.tif");
    connTif->Open();

    FdoPtr<FdoIGetSpatialContexts> getSC = static_cast<FdoIGetSpatialContexts*>(connTif->CreateCommand(FdoCommandType_GetSpatialContexts));
    getSC->SetActiveOnly(false);
    FdoPtr<FdoISpatialContextReader> reader = getSC->Execute();
    FDO_CPPUNIT_ASSERT(reader->ReadNext() == true);

    FdoByteArray* fgf = reader->GetExtent();
	FdoRfpRect rect = FdoRfpUtil::CreateRectFromGeometryAgf(fgf);
	
    printf("LL: %12.7f, %12.7f\n", rect.m_minX, rect.m_minY);
    printf("UR: %12.7f, %12.7f\n", rect.m_maxX, rect.m_maxY);

    //As reported by gdalinfo
	FDO_CPPUNIT_ASSERT (rect.m_minX == 9.5);
	FDO_CPPUNIT_ASSERT (rect.m_minY == -989.5);
    FDO_CPPUNIT_ASSERT (rect.m_maxX == 1009.5);
    FDO_CPPUNIT_ASSERT (rect.m_maxY == 10.5);

    connTif->Close();

    //TestData/PNG/CM13ct
    FdoPtr<FdoIConnection> connPng = CreateConnection();
    FDO_CPPUNIT_ASSERT(connPng != NULL);

    connPng->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/PNG/CM13ct.png");
    connPng->Open();

    getSC = static_cast<FdoIGetSpatialContexts*>(connPng->CreateCommand(FdoCommandType_GetSpatialContexts));
    getSC->SetActiveOnly(false);
    reader = getSC->Execute();
    FDO_CPPUNIT_ASSERT(reader->ReadNext() == true);

    fgf = reader->GetExtent();
	rect = FdoRfpUtil::CreateRectFromGeometryAgf(fgf);
	
    printf("LL: %12.7f, %12.7f\n", rect.m_minX, rect.m_minY);
    printf("UR: %12.7f, %12.7f\n", rect.m_maxX, rect.m_maxY);

    //As reported by gdalinfo, but gdalinfo only displays to 3 decimal places, so compare our numbers to 3 decimal places
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_minX - 674425.295) <= 0.001);
	FDO_CPPUNIT_ASSERT (std::fabs(rect.m_minY - 6431368.631) <= 0.001);
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_maxX - 693935.295) <= 0.001);
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_maxY - 6462458.631) <= 0.001);

    connPng->Close();

    //TestData/JPG/NBenguela.2004357.aqua.250m  (.!,@#$%^()_+=-`~[]').jpg
    FdoPtr<FdoIConnection> connJpg = CreateConnection();
    FDO_CPPUNIT_ASSERT(connJpg != NULL);

    connJpg->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/JPG/NBenguela.2004357.aqua.250m  (.!,@#$%^()_+=-`~[]').jpg");
    connJpg->Open();

    getSC = static_cast<FdoIGetSpatialContexts*>(connJpg->CreateCommand(FdoCommandType_GetSpatialContexts));
    getSC->SetActiveOnly(false);
    reader = getSC->Execute();
    FDO_CPPUNIT_ASSERT(reader->ReadNext() == true);

    fgf = reader->GetExtent();
	rect = FdoRfpUtil::CreateRectFromGeometryAgf(fgf);
	
    printf("LL: %12.7f, %12.7f\n", rect.m_minX, rect.m_minY);
    printf("UR: %12.7f, %12.7f\n", rect.m_maxX, rect.m_maxY);

    //As reported by gdalinfo, but gdalinfo only displays to 7 decimal places, so compare our numbers to 7 decimal places
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_minX - 8.0045000) <= 0.0000001);
	FDO_CPPUNIT_ASSERT (std::fabs(rect.m_minY - (-29.0716280)) <= 0.0000001);
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_maxX - 16.9977059) <= 0.0000001);
    FDO_CPPUNIT_ASSERT (std::fabs(rect.m_maxY - (-16.9308000)) <= 0.0000001);

    connJpg->Close();

    //There's probably others, but this is a sufficient sample set for now
}
