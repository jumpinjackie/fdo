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
#include "SqlServerFdoDeleteTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoDeleteTest, "FdoDeleteTest");

void SqlServerFdoDeleteTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoDeleteTest::DeleteTestWithParams()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, true);

        FdoPtr<FdoStringValue> sValue = FdoStringValue::Create(L"test"); 
        FdoPtr<FdoPropertyValue> propSIns = FdoPropertyValue::Create(L"ByLaw", sValue);
        
        int startVal = 3;
        for (int idx = 0; idx < 10; idx++)
        {
            FdoPtr<FdoIInsert> insCmd = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insCmd->SetFeatureClassName(L"L\x00e4nd:Zoning");
            FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
            
            sValue->SetString(FdoStringP::Format(L"test_%d", idx+1));
            FdoPtr<FdoDataValue> dtValue = FdoDataValue::Create(startVal + idx); 
            FdoPtr<FdoPropertyValue> propIns = FdoPropertyValue::Create(L"ZoningType", dtValue);
            vals->Add(propIns);
            vals->Add(propSIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
        }

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Zoning");
	    deleteCommand->SetFilter( L"ZoningType = :MyPar" );
        FdoPtr<FdoParameterValueCollection> parVals = deleteCommand->GetParameterValues();
        FdoPtr<FdoInt32Value> dVal = FdoInt32Value::Create(startVal); 
        FdoPtr<FdoParameterValue> pVal = FdoParameterValue::Create(L"MyPar", dVal);
        parVals->Add(pVal);
        
        for (int idx = 0; idx < 8; idx++)
        {
            dVal->SetInt32(startVal + idx);
            int count = deleteCommand->Execute();
            printf ("Delete feature No %d -> %d deleted featues\n", startVal + idx, count);
        }
        parVals->Clear(); // this will force select command to be re-built
        parVals->Add(pVal);
        
        dVal->SetInt32(startVal + 8);
        int count = deleteCommand->Execute();
        printf ("Delete feature No %d -> %d deleted featues\n", startVal + 8, count);
	    
        deleteCommand->SetFilter( L"ZoningType = :MyPar" );
        dVal->SetInt32(startVal + 9);
        count = deleteCommand->Execute();
        printf ("Delete feature No %d -> %d deleted featues\n", startVal + 9, count);
    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
}