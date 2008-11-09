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
#include "RfpTestSpatialContext.h"
#include <math.h>

RfpTestSpatialContext::RfpTestSpatialContext()
{
}

RfpTestSpatialContext::~RfpTestSpatialContext()
{
}

void RfpTestSpatialContext::_setUp()
{
}

void RfpTestSpatialContext::_tearDown()
{
}

void RfpTestSpatialContext::testSpatialContext1()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_right_1.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoIDescribeSchema> describeSchema = static_cast<FdoIDescribeSchema*>(cmd);
	FdoPtr<FdoFeatureSchemaCollection> featureSchemas = describeSchema->Execute();
	CPPUNIT_ASSERT(featureSchemas->GetCount() == 1);

	FdoPtr<FdoFeatureSchema> schema = featureSchemas->GetItem(0);
	FdoPtr<FdoClassCollection> classes = schema->GetClasses();
	CPPUNIT_ASSERT(classes->GetCount() == 1);

	FdoPtr<FdoClassDefinition> clsDefinition = classes->GetItem(0);
	CPPUNIT_ASSERT( STREQUAL(clsDefinition->GetName(), L"Photo"));

	FdoPtr<FdoPropertyDefinitionCollection> props = clsDefinition->GetProperties();
	CPPUNIT_ASSERT(props->GetCount() == 2);

	// second property is 'Image', type is Raster.
	FdoPtr<FdoPropertyDefinition> prop  = props->GetItem(1);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	FdoPtr<FdoRasterPropertyDefinition> propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));	
	CPPUNIT_ASSERT(STREQUAL(propraster->GetName(), L"Image"));

	FdoStringP scName = propraster->GetSpatialContextAssociation();
	CPPUNIT_ASSERT(STREQUAL(propraster->GetSpatialContextAssociation(), L"area1"));

	connection->Close();
}

void RfpTestSpatialContext::testSpatialContext2()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_right_2.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoIDescribeSchema> describeSchema = static_cast<FdoIDescribeSchema*>(cmd);
	FdoPtr<FdoFeatureSchemaCollection> featureSchemas = describeSchema->Execute();
	CPPUNIT_ASSERT(featureSchemas->GetCount() == 1);

	FdoPtr<FdoFeatureSchema> schema = featureSchemas->GetItem(0);
	FdoPtr<FdoClassCollection> classes = schema->GetClasses();
	CPPUNIT_ASSERT(classes->GetCount() == 1);

	FdoPtr<FdoClassDefinition> clsDefinition = classes->GetItem(0);
	CPPUNIT_ASSERT( STREQUAL(clsDefinition->GetName(), L"Photo"));

	FdoPtr<FdoPropertyDefinitionCollection> props = clsDefinition->GetProperties();
	CPPUNIT_ASSERT(props->GetCount() == 2);

	// second property is 'Image', type is Raster.
	FdoPtr<FdoPropertyDefinition> prop  = props->GetItem(1);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	FdoPtr<FdoRasterPropertyDefinition> propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));	
	CPPUNIT_ASSERT(STREQUAL(propraster->GetName(), L"Image"));

	FdoStringP scName = propraster->GetSpatialContextAssociation();
	CPPUNIT_ASSERT(STREQUAL(propraster->GetSpatialContextAssociation(), L"area1"));

	connection->Close();
}


void RfpTestSpatialContext::testSpatialContext3()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_error_1.xml", L"r");
	connection->SetConfiguration(stream);

	// it must fail for there are different coordinate systems for the two rasters
	bool bError = false;
	try
	{
		connection->Open();
	}
	catch(...)
	{
		bError = true;
		connection->Close();
	}
}


void RfpTestSpatialContext::testSpatialContext4()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_error_2.xml", L"r");
	connection->SetConfiguration(stream);

	// it must fail for there are different coordinate systems for the two rasters
	bool bError = false;
	try
	{
		connection->Open();
	}
	catch(...)
	{
		bError = true;
		connection->Close();
	}
}

void RfpTestSpatialContext::testSpatialContext5()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_right_3.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoIDescribeSchema> describeSchema = static_cast<FdoIDescribeSchema*>(cmd);
	FdoPtr<FdoFeatureSchemaCollection> featureSchemas = describeSchema->Execute();
	CPPUNIT_ASSERT(featureSchemas->GetCount() == 1);

	//test the first feature class
	FdoPtr<FdoFeatureSchema> schema = featureSchemas->GetItem(0);
	FdoPtr<FdoClassCollection> classes = schema->GetClasses();
	CPPUNIT_ASSERT(classes->GetCount() == 2);

	FdoPtr<FdoClassDefinition> clsDefinition = classes->GetItem(0);
	CPPUNIT_ASSERT( STREQUAL(clsDefinition->GetName(), L"Photo"));

	FdoPtr<FdoPropertyDefinitionCollection> props = clsDefinition->GetProperties();
	CPPUNIT_ASSERT(props->GetCount() == 2);

	// second property is 'Image', type is Raster.
	FdoPtr<FdoPropertyDefinition> prop  = props->GetItem(1);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	FdoPtr<FdoRasterPropertyDefinition> propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));	
	CPPUNIT_ASSERT(STREQUAL(propraster->GetName(), L"Image"));

	//
	FdoStringP scName = propraster->GetSpatialContextAssociation();
	CPPUNIT_ASSERT(STREQUAL(propraster->GetSpatialContextAssociation(), L"area1"));

	//test the second feature class
	clsDefinition = classes->GetItem(1);
	CPPUNIT_ASSERT( STREQUAL(clsDefinition->GetName(), L"Photo2"));

	props = clsDefinition->GetProperties();
	CPPUNIT_ASSERT(props->GetCount() == 2);

	// second property is 'Image', type is Raster.
	prop  = props->GetItem(1);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));	
	CPPUNIT_ASSERT(STREQUAL(propraster->GetName(), L"Image"));

	//association spatial context
	scName = propraster->GetSpatialContextAssociation();
	CPPUNIT_ASSERT(STREQUAL(propraster->GetSpatialContextAssociation(), L"area1"));

	connection->Close();
}

void RfpTestSpatialContext::testSpatialContext6()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/band/spatialcontext_right_4.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* cmd = connection->CreateCommand(FdoCommandType_GetSpatialContexts);
	FdoPtr<FdoIGetSpatialContexts> getSpatialContexts = static_cast<FdoIGetSpatialContexts*>(cmd);
	FdoPtr<FdoISpatialContextReader> spatialContextsReader = getSpatialContexts->Execute();
	
	//test the first spatial context
	CPPUNIT_ASSERT(spatialContextsReader->ReadNext());
	
	//FdoString *sName = spatialContextsReader->GetName() ;
	//FdoString *sDesc = spatialContextsReader->GetDescription() ;
	//FdoString *sCS = spatialContextsReader->GetCoordinateSystem() ;
	//FdoString *sWkt = spatialContextsReader->GetCoordinateSystemWkt() ;
	//FdoSpatialContextExtentType type = spatialContextsReader->GetExtentType() ;
	
	CPPUNIT_ASSERT(STREQUAL(spatialContextsReader->GetName(), L"area1"));
	CPPUNIT_ASSERT(STREQUAL(spatialContextsReader->GetDescription(), L"Spatial context for Area 1"));
	CPPUNIT_ASSERT(STREQUAL(spatialContextsReader->GetCoordinateSystem(), L"utm_zone1"));
	FdoByteArray *byteArray = spatialContextsReader->GetExtent() ;
	FdoRfpRect rect = CreateRectFromGeometryAgf(byteArray) ;
	CPPUNIT_ASSERT(rect.m_minX == 100000.00000000000) ;
	CPPUNIT_ASSERT(rect.m_minY == 100000.0000000000) ;
	CPPUNIT_ASSERT(rect.m_maxX == 300513) ;
	CPPUNIT_ASSERT(rect.m_maxY == 1100000) ;

	CPPUNIT_ASSERT(spatialContextsReader->ReadNext() == 0);
	
	connection->Close();
}
