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

#include "Pch.h"
#include "SqlServerFdoAdvancedSelectTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoAdvancedSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoAdvancedSelectTest, "FdoAdvancedSelectTest");

void SqlServerFdoAdvancedSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoAdvancedSelectTest::isValidTest()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( mConnection == NULL )
        return;

    // Use the function in the filter
    try
    {
        // Clear out any data from previous run.

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) mConnection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Ward");
        deleteCommand->Execute();
        deleteCommand = NULL;

        // Insert the test data
        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"L\x00e4nd:Ward");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        // Feature with valid polygon geometry
        dataValue = FdoDataValue::Create(L"Kanata North");
        propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Name");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoInt32)800000);
        propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Population");
        propertyValue->SetValue(dataValue);

        double       coordsBuffer[10];
        coordsBuffer[0] = 0;
        coordsBuffer[1] = 0;
        coordsBuffer[2] = 0;
        coordsBuffer[3] = 1;
        coordsBuffer[4] = 1;
        coordsBuffer[5] = 1;
        coordsBuffer[6] = 1;
        coordsBuffer[7] = 0;
        coordsBuffer[8] = 0;
        coordsBuffer[9] = 0;

        propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Geometry");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        FdoPtr<FdoILinearRing> ring1 = gf->CreateLinearRing(FdoDimensionality_XY, 10, coordsBuffer);
        FdoPtr<FdoIPolygon> poly1 = gf->CreatePolygon( ring1, NULL );
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(poly1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

        // Feature with invalid polygon geometry (a figure 8)
        dataValue = FdoDataValue::Create(L"Figure8");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        coordsBuffer[0] = 0;
        coordsBuffer[1] = 0;
        coordsBuffer[2] = 0;
        coordsBuffer[3] = 1;
        coordsBuffer[4] = 1;
        coordsBuffer[5] = 0;
        coordsBuffer[6] = 1;
        coordsBuffer[7] = 1;
        coordsBuffer[8] = 0;
        coordsBuffer[9] = 0;

        propertyValue = propertyValues->GetItem(L"Geometry");

        ring1 = gf->CreateLinearRing(FdoDimensionality_XY, 10, coordsBuffer);
        poly1 = gf->CreatePolygon( ring1, NULL );
        byteArray = gf->GetFgf(poly1);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        reader = insertCommand->Execute();
        reader = NULL;
        insertCommand = NULL;

        printf("\nUsing function in a filter\n");
        FdoPtr<FdoISelect> selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"L\x00e4nd:Ward");
		selCmd->SetFilter(L"IsValid(Geometry) = 0");
        myReader = selCmd->Execute();
        CPPUNIT_ASSERT( myReader != NULL );
        FdoClassDefinitionP classDef = myReader->GetClassDefinition(); 
        FdoInt32 count=0;
        while ( myReader->ReadNext() )
        {
            CPPUNIT_ASSERT( myReader->IsNull(L"Name") == false );
            CPPUNIT_ASSERT( wcscmp(myReader->GetString(L"Name"),L"Figure8") == 0 );
            count++;
        }
        CPPUNIT_ASSERT( count == 1 );

        printf("\nUsing function in a calculated property\n");
        selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"L\x00e4nd:Ward");
        FdoPtr<FdoIdentifierCollection> propertyNames = selCmd->GetPropertyNames();
        FdoPropertiesP props = classDef->GetProperties();
        for ( int i = 0; i < props->GetCount(); i++ ) 
        {
            FdoPropertyP prop = props->GetItem(i);
            FdoPtr<FdoIdentifier> id = FdoIdentifier::Create( prop->GetName() );
            propertyNames->Add( id );
        }

        FdoPtr<FdoExpressionCollection> args = FdoExpressionCollection::Create();
        FdoPtr<FdoIdentifier> arg = FdoIdentifier::Create(L"Geometry");
        args->Add(arg);
        FdoPtr<FdoExpression> expr = FdoFunction::Create(L"IsValid", args);
        FdoPtr<FdoComputedIdentifier> compIdent = FdoComputedIdentifier::Create(L"comp1", expr );
        propertyNames->Add( compIdent );

        myReader = selCmd->Execute();
        CPPUNIT_ASSERT( myReader != NULL );
        count = 0;
        while ( myReader->ReadNext() )
        {
            CPPUNIT_ASSERT( myReader->IsNull(L"Name") == false );
            if ( wcscmp(myReader->GetString(L"Name"),L"Kanata North") == 0 )
                CPPUNIT_ASSERT( myReader->GetInt32(L"comp1") == 1 );
            else if ( wcscmp(myReader->GetString(L"Name"),L"Figure8") == 0 )
                CPPUNIT_ASSERT( myReader->GetInt32(L"comp1") == 0 );
            else
                CPPUNIT_FAIL("Unexpected Ward name");
            count++;
        }
        CPPUNIT_ASSERT( count == 2 );
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

}

