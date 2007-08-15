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

#include "stdafx.h"
#include "AssociationDeleteTest.h"
#include "AssociationSchemaTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AssociationDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AssociationDeleteTest, "AssociationDeleteTest");

AssociationDeleteTest::AssociationDeleteTest(void)
{
}

AssociationDeleteTest::~AssociationDeleteTest(void)
{
}

void AssociationDeleteTest::masterTestNoObj( AssociationDeleteType type, FdoDeleteRule deleteRule, bool assocIsFeat  )
{
    bool shouldFail = false;
    int  deletedCount = 1;
    try
    {
		AssociationSchemaTest::SetDeleteRule( deleteRule );
        // Setup the data
       if( assocIsFeat )
            insertUtil.insert_NoIdentAssocFeatClass();
       else 
       {
           // The rule used to create the schema; this is should be the first call to this method
           // for this schema
            if( type == Delete_NoIdentity )
				insertUtil.insert_NoIdent();
            else
                insertUtil.insert_WithIdent();
       }

	   if( deleteRule == FdoDeleteRule_Prevent )
			shouldFail = true;

	   //reset the delete rule
	   AssociationSchemaTest::SetDeleteRule( FdoDeleteRule_Break );

	   FdoPtr<FdoIConnection> connection = insertUtil.openConnection(false);

        FdoPtr<FdoIDelete>deleteCmd = (FdoIDelete*)connection->CreateCommand( FdoCommandType_Delete );
		deleteCmd->SetFeatureClassName(L"TestFeatureClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Delete_NoIdentity  && ! assocIsFeat )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"10") ) ); 
            //deleteCmd->SetFilter(L"Association Prop1.Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Karim") ) );
            //deleteCmd->SetFilter(L"'Association Prop1'.'Name One'='Karim'");

        deleteCmd->SetFilter( pFilter );
        int count = deleteCmd->Execute();
      
		if( deleteRule == FdoDeleteRule_Cascade )
			deletedCount = 2;
        if( deletedCount != count )
        {
            printf("Number of deleted object should be %d\n", deletedCount );
            CPPUNIT_ASSERT(false);
        }
    }
    catch(FdoException *exp )
    {
        if( ! shouldFail )
        {
            printf("Association delete test unexpected error: %ls\n", exp->GetExceptionMessage() );
			CPPUNIT_ASSERT(false);
        }
        exp->Release();
        return;
	}

    if( shouldFail )
    {
        printf("Delete Master Test should fail\n" );
        CPPUNIT_ASSERT(false);
    }
}

void AssociationDeleteTest::masterTestNoObjReverse( AssociationDeleteType type  )
{
    try
    {
        // Setup the data
       if( type == Delete_NoIdentity )
            insertUtil.insert_NoIdent();
        else
            insertUtil.insert_WithIdent();

        FdoPtr<FdoIConnection> connection = insertUtil.openConnection(false);

        FdoPtr<FdoIDelete>deleteCmd = (FdoIDelete*)connection->CreateCommand( FdoCommandType_Delete );
		deleteCmd->SetFeatureClassName(L"TestClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Delete_NoIdentity )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"10") ) ); 
            //deleteCmd->SetFilter(L"Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Karim") ) );
            //deleteCmd->SetFilter(L"'Name One'='Karim'");

        deleteCmd->SetFilter( pFilter );
        int count = deleteCmd->Execute();
        printf("Deleted %d objects\n", count );       
    }
    catch(FdoException *exp )
    {
        printf("Delete Master Test(NO OBJ REVERSE)Error: %ls\n", exp->GetExceptionMessage() );
        exp->Release();
        return;
	}
}

// The next 3 tests should be run in that order
void AssociationDeleteTest::delete_NoIdent(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Break );
}
void AssociationDeleteTest::delete_NoIdentPrevent(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Prevent );
}
void AssociationDeleteTest::delete_NoIdentCascade(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Cascade );
}

void AssociationDeleteTest::delete_NoIdentCascadeFeatClass(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Cascade, true );
}

// The next 3 tests should be run in that order
void AssociationDeleteTest::delete_WithIdent()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Break );
}
void AssociationDeleteTest::delete_WithIdentPrevent()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Prevent );
}
void AssociationDeleteTest::delete_WithIdentCascade()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Cascade );
}



