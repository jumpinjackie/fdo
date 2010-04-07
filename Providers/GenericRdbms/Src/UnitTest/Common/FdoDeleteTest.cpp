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
#include "FdoDeleteTest.h"
#include "UnitTestUtil.h"

FdoDeleteTest::FdoDeleteTest(void)
{
    mSuffix[0] = '\0';
}

FdoDeleteTest::FdoDeleteTest(wchar_t *suffix)
{
    wcsncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

FdoDeleteTest::~FdoDeleteTest(void)
{
}

void FdoDeleteTest::setUp ()
{
    set_provider();
}

void FdoDeleteTest::FeatureDelete ()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;
	clock_t start, finish;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, true);

        // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(L"segcount"); 
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create((int)15); 

        FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

        start = clock();

        featureTransaction = connection->BeginTransaction();
        int count;

	    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
	    deleteCommand->SetFilter( filter );

        count = deleteCommand->Execute();

        // FdoInsertTest keeps adding features to the Parcel class. When # of features
        // hits 5000, delete them all in order to prevent parcel table from growing
        // indefinitely.

		FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"Länd:Parcel");
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();

        int limit = 5000;
        int featCount = 0;

        while ( myReader->ReadNext() && (featCount < limit) )
            featCount++;

        myReader = NULL;
        selCmd = NULL;

        if ( featCount >= limit ) 
        {
    	    deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
            deleteCommand->SetFeatureClassName(L"Länd:Parcel");
            deleteCommand->Execute();
        }
    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
    catch (...)
    {
        if (featureTransaction)
	        featureTransaction->Commit();
        if (connection)
	        connection->Close();
        CPPUNIT_FAIL ("delete failed");
    }
	try
	{
		featureTransaction->Commit();
	}
    catch (...)
    {
        if (connection)
    		connection->Close();
        CPPUNIT_FAIL ("commit failed");
	}
	connection->Close();

	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}



void FdoDeleteTest::ObjectPropDelete ()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;
    
	clock_t start, finish;
    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, true);

	    // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(L"testClass_LastName");
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create(L"AnotherLastname"); 
	
	    FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

        start = clock();

	    featureTransaction = connection->BeginTransaction();
        int count;

	    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(L"Acad:testClass.Object2");
	    deleteCommand->SetFilter( filter );

        count = deleteCommand->Execute();
    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
    catch (...)
    {
        if (featureTransaction)
	        featureTransaction->Commit();
        if (connection)
	        connection->Close();
        CPPUNIT_FAIL ("delete failed");
    }
	try
	{
		featureTransaction->Commit();
	}
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
    catch (...)
    {
        if (connection)
    		connection->Close();
        CPPUNIT_FAIL ("commit failed");
	}
	connection->Close();
	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}
