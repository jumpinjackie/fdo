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
#include "SqlServerFdoSelectTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoSelectTest, "FdoSelectTest");

void SqlServerFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoSelectTest::TestDateFilter()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoIInsert> insertCmd;
            FdoPtr<FdoIFeatureReader> featReader;
            FdoPtr<FdoPropertyValueCollection> propVals;
            FdoPtr<FdoPropertyValue> propVal;
            FdoPtr<FdoValueExpression> expr;

            FdoString* CLASSNAME = L"Acad:AcDbEntity";
            FdoString* DATEPROPNAME = L"datetime";
            FdoString* IDPROPNAME = L"FeatId";

            // Delete leftover data from previous tests:
            FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)mConnection->CreateCommand(FdoCommandType_Delete);
            deleteCmd->SetFeatureClassName(CLASSNAME);
            deleteCmd->SetFilter(L"layer = 'TESTDATES'");
            FdoInt32 numDeleted = deleteCmd->Execute();

            // Insert some data:
            insertCmd = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            insertCmd->SetFeatureClassName(CLASSNAME);
            propVals = insertCmd->GetPropertyValues();
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"TIMESTAMP '2004-05-06 00:00:00'");
            propVal = FdoPropertyValue::Create(L"datetime", expr);
            propVals->Add(propVal);
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"'TESTDATES'");
            propVal = FdoPropertyValue::Create(L"layer", expr);
            propVals->Add(propVal);
            featReader = insertCmd->Execute();
            featReader->ReadNext();
            FdoInt64 featid1 = featReader->GetInt64(IDPROPNAME);

            insertCmd = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            insertCmd->SetFeatureClassName(CLASSNAME);
            propVals = insertCmd->GetPropertyValues();
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"TIMESTAMP '2004-03-06 00:00:00'");
            propVal = FdoPropertyValue::Create(L"datetime", expr);
            propVals->Add(propVal);
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"'TESTDATES'");
            propVal = FdoPropertyValue::Create(L"layer", expr);
            propVals->Add(propVal);
            featReader = insertCmd->Execute();
            featReader->ReadNext();
            FdoInt64 featid2 = featReader->GetInt64(IDPROPNAME);

            // Select the data to verify the date filter worked:
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            selectCmd->SetFeatureClassName(CLASSNAME);
            FdoPtr<FdoIdentifier> dataPropID = FdoIdentifier::Create(DATEPROPNAME);
            FdoPtr<FdoDateTimeValue> dataValue = FdoDateTimeValue::Create(FdoDateTime(2004, 5, (FdoInt8)6));
            FdoPtr<FdoComparisonCondition> compCond   = FdoComparisonCondition::Create(
                       dataPropID,
                        FdoComparisonOperations_EqualTo,
                        dataValue);
            selectCmd->SetFilter(compCond);
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
            FdoInt64 idFound = -1;
            long countFound = 0;
            while (reader->ReadNext())
            {
                idFound = reader->GetInt64(IDPROPNAME);
                countFound++;
            }
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", countFound==1);
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", idFound==featid1);
            reader->Close();
        }
        catch (FdoException* e)
        {
            TestCommonFail (e);
        }
    }
}


void SqlServerFdoSelectTest::spatial_query_defect813611()
{

}