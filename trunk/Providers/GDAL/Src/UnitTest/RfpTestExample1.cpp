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
#include "RfpTestExample1.h"
#include "FdoRfpRect.h"

RfpTestExample1::RfpTestExample1(void)
{
}

RfpTestExample1::~RfpTestExample1(void)
{
}

void RfpTestExample1::_setUp()
{
	m_connection = CreateConnection();
	CPPUNIT_ASSERT(m_connection != NULL);
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"example1.xml", L"r");
	m_connection->SetConfiguration(stream);
	m_connection->Open();
}

void RfpTestExample1::_tearDown()
{
	m_connection->Close();	
}

void RfpTestExample1::testDescribeSchema()
{
	FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_DescribeSchema);
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

	// first property is 'Id', type is string.
	FdoPtr<FdoPropertyDefinition> prop = props->GetItem(0);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_DataProperty);
	FdoPtr<FdoDataPropertyDefinition> propdata = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(prop.p));
	CPPUNIT_ASSERT(propdata->GetDataType() == FdoDataType_String);
	CPPUNIT_ASSERT(STREQUAL(propdata->GetName(), L"Id"));
	CPPUNIT_ASSERT(256 == propdata->GetLength());

	// second property is 'Image', type is Raster.
	prop = props->GetItem(1);
	CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_RasterProperty);
	FdoPtr<FdoRasterPropertyDefinition> propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));	
	CPPUNIT_ASSERT(STREQUAL(propraster->GetName(), L"Image"));
	// default image size (800, 600)
	CPPUNIT_ASSERT(propraster->GetDefaultImageXSize() == 800);
	CPPUNIT_ASSERT(propraster->GetDefaultImageYSize() == 600);

	FdoPtr<FdoRasterDataModel> datamodel = propraster->GetDefaultDataModel();	
	CPPUNIT_ASSERT(32 == datamodel->GetBitsPerPixel()); // 32bpp.
	CPPUNIT_ASSERT(FdoRasterDataModelType_Bitonal == datamodel->GetDataModelType());
	
	CPPUNIT_ASSERT(FdoRasterDataOrganization_Row == datamodel->GetOrganization()); // "Row"
	CPPUNIT_ASSERT(100 == datamodel->GetTileSizeX()); // "100"
	CPPUNIT_ASSERT( 50 == datamodel->GetTileSizeY()); // "50"
}

void RfpTestExample1::testSpatialContext()
{
	// test GetSpacialContexts.
	FdoICommand* cmd = m_connection->CreateCommand(FdoCommandType_GetSpatialContexts);
	FdoPtr<FdoIGetSpatialContexts> getSC = static_cast<FdoIGetSpatialContexts*>(cmd);
	FdoPtr<FdoISpatialContextReader> scReader = getSC->Execute();
	CPPUNIT_ASSERT(true == scReader->ReadNext());
	CPPUNIT_ASSERT(true == scReader->IsActive());
	CPPUNIT_ASSERT(FdoSpatialContextExtentType_Static == scReader->GetExtentType());
	
    //Only one spatial context
	//FDO_CPPUNIT_ASSERT(false == scReader->ReadNext());
}

#define orgX 200000.0
#define orgY 1000000.0

void RfpTestExample1::testSelectCommand()
{

	FdoICommand * cmd = m_connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);

	cmdSelect->SetFeatureClassName(L"Photo");
	FdoPtr<FdoIFeatureReader> reader = cmdSelect->Execute();		

	double sizeX = 800, sizeY = 600;
	int tileX = 16, tileY = 16;

	double tempX = orgX;
	int current = 0;
	for (int i = 1; i <= tileX; i++)
	{
		double tempY = orgY;
		for (int j = 1; j <= tileY; j++)
		{
			CPPUNIT_ASSERT(true == reader->ReadNext());
			CPPUNIT_ASSERT(STREQUAL(reader->GetString(L"Id"), FdoStringP::Format(L"photo%d_%d", i, j)));

			tempY += sizeY;
		}
		tempX += sizeX;
	}
	CPPUNIT_ASSERT(false == reader->ReadNext());

}

void RfpTestExample1::testSelectWithFilter(const FdoPtr<FdoFilter>& filter, StringVector& result, FdoString* className, FdoString* propertyName, FdoString* propertyToSelect)
{
	//cout << "Executing query: select Id from Photo where " << (const char*)FdoStringP(filter->ToString()) << endl;
	FdoICommand * cmd = m_connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);

	cmdSelect->SetFeatureClassName(className);
	cmdSelect->SetFilter(filter);
	FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
	FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create(propertyToSelect);
	propsToSelect->Add(identifier);
	FdoPtr<FdoIFeatureReader> reader = cmdSelect->Execute();
	//Verify the result
	StringVector::size_type count = result.size();
	for (size_t i = 0; i < count; i++)
	{
		CPPUNIT_ASSERT(true == reader->ReadNext());
		FdoString* id = reader->GetString(propertyName);
		bool bFind = false;
		StringVector::iterator it;
		for (it = result.begin(); it != result.end(); it++) 
			if (*it == id)
			{
				bFind = true;
				break;
			}
        CPPUNIT_ASSERT(bFind);
		result.erase(it);
	}
	CPPUNIT_ASSERT(false == reader->ReadNext());
	CPPUNIT_ASSERT(result.size() == 0);	
}


//test intersect
void RfpTestExample1::testSelectWithFilter1()
{
	FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Intersects, 
		FdoRfpRect(orgX + 400, orgY + 300, orgX + 1200, orgY + 900));
	
	StringVector result;
	result.push_back(L"photo1_1");
	result.push_back(L"photo1_2");
	result.push_back(L"photo2_1");
	result.push_back(L"photo2_2");
	testSelectWithFilter(filter, result);

}

//test within
void RfpTestExample1::testSelectWithFilter2()
{
	FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Within, 
		FdoRfpRect(orgX, orgY, orgX + 2400, orgY + 1800));
	
	StringVector result;
	result.push_back(L"photo1_1");
	result.push_back(L"photo1_2");
	result.push_back(L"photo1_3");
	result.push_back(L"photo2_1");
	result.push_back(L"photo2_2");
	result.push_back(L"photo2_3");
	result.push_back(L"photo3_1");
	result.push_back(L"photo3_2");
	result.push_back(L"photo3_3");

	// full qualified name
	testSelectWithFilter(filter, result, L"photos:Photo", L"photos:Photo.Id", L"photos:Photo.Id");

}

//test inside
void RfpTestExample1::testSelectWithFilter3()
{
	FdoPtr<FdoFilter> filter = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Inside, 
		FdoRfpRect(orgX, orgY, orgX + 2400, orgY + 1800));
	
	StringVector result;
	result.push_back(L"photo2_2");

	// partial qualified name
	testSelectWithFilter(filter, result, L"Photo", L"photos:Photo.Id", L"Photo.Id");

}

//test in
void RfpTestExample1::testSelectWithFilter4()
{
	FdoString* ids[] = {L"photo1_1",
						L"photo2_2",
						L"photo16_3"};
	FdoPtr<FdoFilter> filter = FdoInCondition::Create(L"Id", ids, 3);
	StringVector result;
	result.push_back(L"photo1_1");
	result.push_back(L"photo2_2");
	result.push_back(L"photo16_3");
	
	// partial qualified name
	testSelectWithFilter(filter, result, L"photos:Photo", L"Id", L"photos:Photo.Id");

}

void RfpTestExample1::testSelectWithFilter5()
{
	FdoPtr<FdoFilter> lhs = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Intersects, 
		FdoRfpRect(orgX + 400, orgY + 300, orgX + 1200, orgY + 900));

	FdoPtr<FdoFilter> rhs = FdoRfpUtil::CreateSpatialCondition(L"Image", 
		FdoSpatialOperations_Intersects, 
		FdoRfpRect(orgX + 1200, orgY + 300, orgX + 2000, orgY + 900));

	FdoPtr<FdoFilter> filter = FdoFilter::Combine(lhs, FdoBinaryLogicalOperations_And, rhs);
	
	StringVector result;
	result.push_back(L"photo2_1");
	result.push_back(L"photo2_2");

	// partial qualified name
	testSelectWithFilter(filter, result, L"Photo", L"Photo.Id", L"Id");
}

