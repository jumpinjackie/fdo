// Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "Pch.h"
#include "GmlTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"

#include <Fdo/Xml/FeatureReader.h>
#include <Fdo/Xml/FeatureFlags.h>

#ifdef _DEBUG
//#define DEBUG_DETAIL  1
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (GmlTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (GmlTest, "GmlTest");

GmlTest::GmlTest(void)
{
}

GmlTest::~GmlTest(void)
{
}

void GmlTest::setUp ()
{
}


void GmlTest::testInstantiateFeatureReader()
{
	FdoXmlReader* fileReader = NULL;
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);

	CPPUNIT_ASSERT(featureReader != NULL);

	//fileReader = FdoXmlReader::Create(L"gml_bsc_IBA2.xml");
	//CPPUNIT_ASSERT(fileReader != NULL);
	//	
	//featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	//CPPUNIT_ASSERT(featureReader != NULL);

	//int count = 0;
	//while(featureReader->ReadNext())
	//{
	//	count ++;

	//	FdoStringP name = featureReader->GetString(L"NAME");
	//	CPPUNIT_ASSERT(name != NULL);
	//}

	//CPPUNIT_ASSERT(count == 1);



}

void GmlTest::testInheritedClass()
{
    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_feature.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_inherited_feature_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP value = featureReader->GetString(L"SITEID");
		CPPUNIT_ASSERT(value != NULL);
        value = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(value != NULL);
        value = featureReader->GetString(L"LINK");
		CPPUNIT_ASSERT(value != NULL);
	}

	CPPUNIT_ASSERT(count == 3);
    }
}

void GmlTest::testSimpleFeature()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_feature.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);
		FdoDateTime dateVal = featureReader->GetDateTime(L"DATE.CREATED");
		if (count == 1) 
			CPPUNIT_ASSERT(dateVal.IsDate() && dateVal.year == 2002 && dateVal.month == 12 && dateVal.day == 12);
		else if (count == 2) 
			CPPUNIT_ASSERT(dateVal.IsDateTime() && 
			dateVal.year == 2002 && dateVal.month == 12 && dateVal.day == 12 &&
			dateVal.hour == 19 && dateVal.minute == 19 && dateVal.minute == 19);
		else if (count == 3)
			CPPUNIT_ASSERT(dateVal.IsTime() && 
			dateVal.hour == 19 && dateVal.minute == 19 && dateVal.minute == 19);

	}

	CPPUNIT_ASSERT(count == 3);
    }

    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_feature.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_simple_feature_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);
		FdoDateTime dateVal = featureReader->GetDateTime(L"DATE.CREATED");
		if (count == 1) 
			CPPUNIT_ASSERT(dateVal.IsDate() && dateVal.year == 2002 && dateVal.month == 12 && dateVal.day == 12);
		else if (count == 2) 
			CPPUNIT_ASSERT(dateVal.IsDateTime() && 
			dateVal.year == 2002 && dateVal.month == 12 && dateVal.day == 12 &&
			dateVal.hour == 19 && dateVal.minute == 19 && dateVal.minute == 19);
		else if (count == 3)
			CPPUNIT_ASSERT(dateVal.IsTime() && 
			dateVal.hour == 19 && dateVal.minute == 19 && dateVal.minute == 19);

	}

	CPPUNIT_ASSERT(count == 3);
    }
}

void GmlTest::testNestedFeature()
{
#if 0
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_feature.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		//nested feature
		FdoPtr<FdoIFeatureReader> nestedFeature = featureReader->GetFeatureObject(L"featureMember");
		CPPUNIT_ASSERT(nestedFeature != NULL);

		nestedFeature->ReadNext();
		name = nestedFeature->GetString(L"name");
		CPPUNIT_ASSERT(name != NULL);
		
	}

	CPPUNIT_ASSERT(count == 2);
#endif
}

void GmlTest::testNestedCollection()
{
#if 0
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_collection.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		//nested collection
		FdoPtr<FdoIFeatureReader> nestedCollection = featureReader->GetFeatureObject(L"nestedCollectionProperty");
		CPPUNIT_ASSERT(nestedCollection != NULL);
		
		int nestedCount = 0;
		while(nestedCollection->ReadNext()){
			nestedCount++;
			FdoStringP name = nestedCollection->GetString(L"name");
			CPPUNIT_ASSERT(name != NULL);
		}

		CPPUNIT_ASSERT(nestedCount == 2);
		
	}

	CPPUNIT_ASSERT(count == 2);
#endif
}



void GmlTest::testSimpleGeometry()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"Bounds");
		testGeometryProperty(featureReader, L"gml/pointProperty");
		testGeometryProperty(featureReader, L"gml/lineStringProperty");

		// testGeometryProperty(featureReader, L"gml/linearRingProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }
    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_simple_geometry_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);


	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"Bounds");
		testGeometryProperty(featureReader, L"gml/pointProperty");
		testGeometryProperty(featureReader, L"gml/lineStringProperty");

		// testGeometryProperty(featureReader, L"gml/linearRingProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }
}

void GmlTest::testNestedGeometry()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"Bounds");
		testGeometryProperty(featureReader, L"gml/polygonProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }

    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_nested_geometry_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);


	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"Bounds");
		testGeometryProperty(featureReader, L"gml/polygonProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }
}

void GmlTest::testMultiGeometry()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_multi_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);
    
	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"gml/multiPointProperty");
		testGeometryProperty(featureReader, L"gml/multiLineStringProperty");
		testGeometryProperty(featureReader, L"gml/multiPolygonProperty");
		testGeometryProperty(featureReader, L"gml/multiGeometryProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }

    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_multi_geometry.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_multi_geometry_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);
    
	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		testGeometryProperty(featureReader, L"gml/multiPointProperty");
		testGeometryProperty(featureReader, L"gml/multiLineStringProperty");
		testGeometryProperty(featureReader, L"gml/multiPolygonProperty");
		testGeometryProperty(featureReader, L"gml/multiGeometryProperty");
	}

	CPPUNIT_ASSERT(count == 2);
    }
}


void GmlTest::testSimpleObject()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_object.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		FdoPtr<FdoIFeatureReader> object = featureReader->GetFeatureObject(L"genericObject");
		CPPUNIT_ASSERT(object != NULL);

		int objCount = 0;
		while(object->ReadNext()){
			objCount++;

			FdoStringP objName = object->GetString(L"name");
			CPPUNIT_ASSERT(objName != NULL);
		}

		CPPUNIT_ASSERT(objCount == 2);
	}

	CPPUNIT_ASSERT(count == 2);
    }

    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_simple_object.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_simple_object_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		FdoPtr<FdoIFeatureReader> object = featureReader->GetFeatureObject(L"genericObject");
		CPPUNIT_ASSERT(object != NULL);

		int objCount = 0;
		while(object->ReadNext()){
			objCount++;

			FdoStringP objName = object->GetString(L"name");
			CPPUNIT_ASSERT(objName != NULL);
		}

		CPPUNIT_ASSERT(objCount == 2);
	}

	CPPUNIT_ASSERT(count == 2);
    }
}


void GmlTest::testNestedObject()
{
    // without schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_object.xml");
	CPPUNIT_ASSERT(fileReader != NULL);
		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, NULL);
	CPPUNIT_ASSERT(featureReader != NULL);

	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		//top level
		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		//nested level
		FdoPtr<FdoIFeatureReader> nestedObject = featureReader->GetFeatureObject(L"genericObject");
		CPPUNIT_ASSERT(nestedObject != NULL);

		while(nestedObject->ReadNext()){
			FdoStringP nestedName = nestedObject->GetString(L"name");
			CPPUNIT_ASSERT(nestedName != NULL);

			//inner level
			FdoPtr<FdoIFeatureReader> innerObject = nestedObject->GetFeatureObject(L"genericObject");
			CPPUNIT_ASSERT(innerObject != NULL);

			while(innerObject->ReadNext()){
				FdoStringP innerName = innerObject->GetString(L"name");
				CPPUNIT_ASSERT(innerName != NULL);
			}
		}
	}

	CPPUNIT_ASSERT(count == 2);
    }

    // with schemas
    {
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_nested_object.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_nested_object_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);


	int count = 0;
	while(featureReader->ReadNext())
	{
		count ++;

		//top level
		FdoStringP name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

		//nested level
		FdoPtr<FdoIFeatureReader> nestedObject = featureReader->GetFeatureObject(L"genericObject");
		CPPUNIT_ASSERT(nestedObject != NULL);

		while(nestedObject->ReadNext()){
			FdoStringP nestedName = nestedObject->GetString(L"name");
			CPPUNIT_ASSERT(nestedName != NULL);

			//inner level
			FdoPtr<FdoIFeatureReader> innerObject = nestedObject->GetFeatureObject(L"genericObject");
			CPPUNIT_ASSERT(innerObject != NULL);

			while(innerObject->ReadNext()){
				FdoStringP innerName = innerObject->GetString(L"name");
				CPPUNIT_ASSERT(innerName != NULL);
			}
		}
	}

	CPPUNIT_ASSERT(count == 2);
    }
}
void GmlTest::testBLOBBase64()
{
    const char* orgText[] = {"An infestation of locusts that decimated crops, the worst drought in 15 years, and Niger's government keeping grain prices too high for too long have now caused a region in the brink of a crisis.",
    "It was a crisis-in-the-making that should have been averted, says Mark Malloch Brown, chief of staff to the U.N. secretary-general. \"What is happening was largely foreseeable as early as November,\" he says.",
    };
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_blob.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_blob_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);


	int count = 0;
	while(featureReader->ReadNext())
	{

		FdoString* name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

        FdoPtr<FdoBLOBValue> binData = static_cast<FdoBLOBValue*>(featureReader->GetLOB(L"binData"));
        FdoPtr<FdoByteArray> binData1 = binData->GetData();
        char* src = (char*)binData1->GetData();
        CPPUNIT_ASSERT(strncmp(src, orgText[count], strlen(orgText[count])) == 0);


		count ++;

	}

	CPPUNIT_ASSERT(count == 2);
}

void GmlTest::testBLOBHex()
{
    const char* orgText[] = {"infestation",
    };
	FdoXmlReaderP fileReader = FdoXmlReader::Create(L"gml_blob_hex.xml");
	CPPUNIT_ASSERT(fileReader != NULL);

    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
    schemas->ReadXml(L"gml_blob_hex_schema.xml", flags);
	FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = schemas->GetXmlSchemaMappings();
	FdoPtr<FdoXmlFeatureFlags> featureFlags = FdoXmlFeatureFlags::Create(L"fdo.osgeo.org/schemas/feature", FdoXmlFlags::ErrorLevel_VeryLow);
	featureFlags->SetSchemaMappings(schemaMappings);

		
	FdoXmlFeatureReaderP featureReader = FdoXmlFeatureReader::Create(fileReader, featureFlags);
	CPPUNIT_ASSERT(featureReader != NULL);
    featureReader->SetFeatureSchemas(schemas);


	int count = 0;
	while(featureReader->ReadNext())
	{

		FdoString* name = featureReader->GetString(L"NAME");
		CPPUNIT_ASSERT(name != NULL);

        FdoPtr<FdoBLOBValue> binData = static_cast<FdoBLOBValue*>(featureReader->GetLOB(L"binData"));
        FdoPtr<FdoByteArray> binData1 = binData->GetData();
        char* src = (char*)binData1->GetData();
        CPPUNIT_ASSERT(strncmp(src, orgText[count], strlen(orgText[count])) == 0);


		count ++;

	}

	CPPUNIT_ASSERT(count == 1);
}

void GmlTest::testGeometryProperty(FdoXmlFeatureReader* featureReader, FdoString* propName)
{
    FdoInt32 baCount = 0;
    const FdoByte* baPtr = featureReader->GetGeometry(propName, &baCount);

    FdoPtr<FdoByteArray> geom = featureReader->GetGeometry(propName);
	CPPUNIT_ASSERT(geom != NULL);
    FdoInt32 arrayCount = geom->GetCount();
    FdoByte* arrayPtr = geom->GetData();
    FDO_SAFE_RELEASE(geom.p);

    CPPUNIT_ASSERT(baCount = arrayCount);
    CPPUNIT_ASSERT(baPtr = arrayPtr);
}






