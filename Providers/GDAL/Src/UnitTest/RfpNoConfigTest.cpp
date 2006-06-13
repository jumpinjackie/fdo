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
#include "RfpNoConfigTest.h"

RfpNoConfigTest::RfpNoConfigTest(void)
{
}

RfpNoConfigTest::~RfpNoConfigTest(void)
{
}
void RfpNoConfigTest::_setUp()
{
	m_connection = CreateConnection();
	FDO_CPPUNIT_ASSERT(m_connection != NULL);
	m_connection->SetConnectionString(L"DefaultRasterFileLocation=../../TestData/pci_eg");
	m_connection->Open();
}
void RfpNoConfigTest::_tearDown()
{
	m_connection->Close();
	m_connection = NULL;
}

void RfpNoConfigTest::testSchemaOverrides()
{
	FdoPtr<FdoIDescribeSchemaMapping> describeMapping = static_cast<FdoIDescribeSchemaMapping*>(m_connection->CreateCommand(FdoCommandType_DescribeSchemaMapping));
	FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = describeMapping->Execute();
	FDO_CPPUNIT_ASSERT(mappings != NULL && mappings->GetCount() == 1);

	FdoPtr<FdoGrfpPhysicalSchemaMapping> mapping = static_cast<FdoGrfpPhysicalSchemaMapping*>(mappings->GetItem(0));
	FDO_CPPUNIT_ASSERT(mapping != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(mapping->GetName(), L"default"));

	FdoPtr<FdoGrfpClassCollection> classes = mapping->GetClasses();
	FDO_CPPUNIT_ASSERT(classes != NULL && classes->GetCount() == 1);

	FdoPtr<FdoGrfpClassDefinition> classDef = classes->GetItem(0);
	FDO_CPPUNIT_ASSERT(classDef != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(classDef->GetName(), L"default"));

	FdoPtr<FdoGrfpRasterDefinition> rasterDef = classDef->GetRasterDefinition();
	FDO_CPPUNIT_ASSERT(rasterDef != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(rasterDef->GetName(), L"default"));

	FdoPtr<FdoGrfpRasterLocationCollection> locations = rasterDef->GetLocations();
	FDO_CPPUNIT_ASSERT(locations != NULL && locations->GetCount() == 1);

	FdoPtr<FdoGrfpRasterLocation> location = locations->GetItem(0);
	FDO_CPPUNIT_ASSERT(location != NULL);
	FDO_CPPUNIT_ASSERT(STRCASEEQ(location->GetName(), L"../../TestData/pci_eg"));

	FdoPtr<FdoGrfpRasterFeatureCollection> catalogue = location->GetFeatureCatalogue();
	FDO_CPPUNIT_ASSERT(catalogue != NULL && catalogue->GetCount() == 0);
}

void RfpNoConfigTest::testSelect()
{

	FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	cmdSelect->SetFeatureClassName(L"default:default");

	FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
	int count = 0;
	while (featureReader->ReadNext())
	{
		FdoString* id = featureReader->GetString(L"default.FeatId");
		FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"default:default.Raster");

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
	CPPUNIT_ASSERT(count == 18);
	featureReader->Close();
}


void RfpNoConfigTest::testDescribeSchema()
{
	FdoICommand* command = m_connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoIDescribeSchema> describeSchema = static_cast<FdoIDescribeSchema*>(command);
	FdoPtr<FdoFeatureSchemaCollection> featureSchemas = describeSchema->Execute();
	FDO_CPPUNIT_ASSERT(featureSchemas->GetCount() == 1);
	
	FdoPtr<FdoFeatureSchema> featureSchema = featureSchemas->GetItem(0);
	FDO_CPPUNIT_ASSERT(wcscmp(featureSchema->GetName(), L"default") == 0);
	FdoPtr<FdoClassCollection> classes = featureSchema->GetClasses();
	FDO_CPPUNIT_ASSERT(classes->GetCount() == 1);
	
	FdoPtr<FdoClassDefinition> classDef = classes->GetItem(0);
	FDO_CPPUNIT_ASSERT(classDef->GetClassType() == FdoClassType_FeatureClass);
	FDO_CPPUNIT_ASSERT(wcscmp(classDef->GetName(), L"default") == 0);

	FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
	FDO_CPPUNIT_ASSERT(props->GetCount() == 2);
	
	//FeatId : String
	FdoPtr<FdoPropertyDefinition> prop = props->GetItem(0);
	FDO_CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_DataProperty);
	FdoPtr<FdoDataPropertyDefinition> propData = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(prop.p));
	FDO_CPPUNIT_ASSERT(propData->GetDataType() == FdoDataType_String);
	FDO_CPPUNIT_ASSERT(wcscmp(propData->GetName(), L"FeatId") == 0);
	
	//Raster : RasterProperty
	prop = props->GetItem(1);
	FDO_CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	FdoPtr<FdoRasterPropertyDefinition> propRaster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));
	FDO_CPPUNIT_ASSERT(wcscmp(propRaster->GetName(), L"Raster") == 0);
	//Some other test here such as default image size and default data model, etc.
	//...


}

void RfpNoConfigTest::testSpatialContext()
{
	//Get Spatial Context
	FdoPtr<FdoIGetSpatialContexts> getSC = static_cast<FdoIGetSpatialContexts*>(m_connection->CreateCommand(FdoCommandType_GetSpatialContexts));
    getSC->SetActiveOnly(false);
	FdoPtr<FdoISpatialContextReader> reader = getSC->Execute();
	FDO_CPPUNIT_ASSERT(reader->ReadNext() == true);

	FDO_CPPUNIT_ASSERT(reader->GetExtentType() == FdoSpatialContextExtentType_Static);
	FDO_CPPUNIT_ASSERT(reader->IsActive() == true);
	
    //Only one spatial context
	//FDO_CPPUNIT_ASSERT(reader->ReadNext() == false);

	//Get Spatial Context
	FdoPtr<FdoIGetSpatialContexts> getSC2 = static_cast<FdoIGetSpatialContexts*>(m_connection->CreateCommand(FdoCommandType_GetSpatialContexts));
    getSC2->SetActiveOnly(true);
	FdoPtr<FdoISpatialContextReader> reader2 = getSC2->Execute();
	FDO_CPPUNIT_ASSERT(reader2->ReadNext() == true);

	FDO_CPPUNIT_ASSERT(reader2->GetExtentType() == FdoSpatialContextExtentType_Static);
	FDO_CPPUNIT_ASSERT(reader2->IsActive() == true);
	
    //Only one spatial context
	//FDO_CPPUNIT_ASSERT(reader2->ReadNext() == false);
}




