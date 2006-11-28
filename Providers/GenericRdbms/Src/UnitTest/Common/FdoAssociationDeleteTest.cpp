 //
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
#include "FdoAssociationDeleteTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX	"_assoc"

FdoAssociationDeleteTest::FdoAssociationDeleteTest(void)
{
}

FdoAssociationDeleteTest::~FdoAssociationDeleteTest(void)
{
}

void FdoAssociationDeleteTest::masterTestNoObj( AssociationDeleteType type, FdoDeleteRule deleteRule, bool assocIsFeat  )
{
    bool shouldFail = false;
    int  deletedCount = 1;
    try
    {
        // Setup the data
       if( assocIsFeat )
            mInsertUtil->insert_NoIdentAssocFeatClass();
       else if( deleteRule == FdoDeleteRule_Break || ! SupportsTransaction() )
       {
           // The rule used to create the schema; this is should be the first call to this method
           // for this schema
            if( type == Delete_NoIdentity )
            {
                
                    mInsertUtil->insert_NoIdent();
            }
            else
                mInsertUtil->insert_WithIdent();
       }
       /*case FdoDeleteRule_Cascade: return 1;
        case FdoDeleteRule_Prevent: return 2;
        case FdoDeleteRule_Break:   return 3;*/
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);
        if( deleteRule == FdoDeleteRule_Prevent )
        {       
            // Modify the meta data
            FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
            sqlCmd->SetSQLStatement(L"update f_associationdefinition set DELETERULE = 2");
			sqlCmd->ExecuteNonQuery();
            shouldFail = true;
        }
        else if( deleteRule == FdoDeleteRule_Cascade )
        {
            // Modify the meta data
            FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
            sqlCmd->SetSQLStatement(L"update f_associationdefinition set DELETERULE = 1");
			sqlCmd->ExecuteNonQuery();
            deletedCount = 2;
        }

        FdoPtr<FdoITransaction> trans;
		if( SupportsTransaction() )
			trans = connection->BeginTransaction();

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
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );
            //deleteCmd->SetFilter(L"'Association Prop1'.'Name One'='Firstname'");

        deleteCmd->SetFilter( pFilter );
        int count = deleteCmd->Execute();
        printf("Deleted %d objects\n", count );
		if( trans != NULL )
		{
			trans->Rollback();
			printf("And then rolled back\n"); 
		}
        if( deletedCount != count )
        {
            printf("Number of deleted object should be %d\n", deletedCount );
            CPPUNIT_FAIL ( "Wrong number of deleted objects" );
        }
    }
    catch(FdoException *exp )
    {
        if( ! shouldFail )
        {
            printf("Delete Master Test(NO OBJ)Error: %ls\n", exp->GetExceptionMessage() );
		    UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        }
        exp->Release();
        return;
	}

    if( shouldFail )
    {
        printf("Delete Master Test(NO OBJ) should fail\n" );
        CPPUNIT_FAIL ( "Delete Master Test(NO OBJ) should fail" );
    }
}

void FdoAssociationDeleteTest::masterTestNoObjReverse( AssociationDeleteType type  )
{
    try
    {
        // Setup the data
       if( type == Delete_NoIdentity )
            mInsertUtil->insert_NoIdent();
        else
            mInsertUtil->insert_WithIdent();

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);
        FdoPtr<FdoITransaction> trans;
		if( SupportsTransaction() )
			trans = connection->BeginTransaction();

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
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );
            //deleteCmd->SetFilter(L"'Name One'='Firstname'");

        deleteCmd->SetFilter( pFilter );
        int count = deleteCmd->Execute();
        printf("Deleted %d objects\n", count );
		if( trans != NULL )
		{
			trans->Rollback();
			printf("And then rolled back\n");
		}
    }
    catch(FdoException *exp )
    {
        printf("Delete Master Test(NO OBJ REVERSE)Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ));
        exp->Release();
        return;
	}
}

// The next 3 tests should be run in that order
void FdoAssociationDeleteTest::delete_NoIdent(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Break );
}
void FdoAssociationDeleteTest::delete_NoIdentPrevent(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Prevent );
}
void FdoAssociationDeleteTest::delete_NoIdentCascade(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Cascade );
}

void FdoAssociationDeleteTest::delete_NoIdentCascadeFeatClass(  )
{
    masterTestNoObj( Delete_NoIdentity, FdoDeleteRule_Cascade, true );
}

// The next 3 tests should be run in that order
void FdoAssociationDeleteTest::delete_WithIdent()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Break );
}
void FdoAssociationDeleteTest::delete_WithIdentPrevent()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Prevent );
}
void FdoAssociationDeleteTest::delete_WithIdentCascade()
{
    masterTestNoObj( Delete_WithIdentityBothSet, FdoDeleteRule_Cascade );
}


void FdoAssociationDeleteTest::masterTestWithObj(AssociationDeleteType type )
{
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);
        FdoPtr<FdoITransaction> trans;
		if( SupportsTransaction() )
			trans = connection->BeginTransaction();

        FdoPtr<FdoIDelete>deleteCmd = (FdoIDelete*)connection->CreateCommand( FdoCommandType_Delete );
		deleteCmd->SetFeatureClassName(L"TestSubFeatureClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Delete_NoIdentityObjNested )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.LeafObject.Association Prop1.Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"11") ) ); 
            //deleteCmd->SetFilter(L"Association Prop1.Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.Association Prop1.Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );
            //deleteCmd->SetFilter(L"Association Prop1.Name One='Firstname'");

        deleteCmd->SetFilter( pFilter );
        int count = deleteCmd->Execute();
        printf("Deleted %d objects\n", count );
		if( trans != NULL )
		{
			trans->Rollback();
			printf("And then rolled back\n");
		}
    }
    catch(FdoException *exp )
    {
        printf("Delete Master Test(OBJ) Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        exp->Release();
		CPPUNIT_FAIL ( "Delete Master Test(OBJ) Error");
    }
}

void FdoAssociationDeleteTest::delete_NoIdentObj()
{
    
   mInsertUtil->insert_NoIdentObj();

   masterTestWithObj( Delete_NoIdentity );
}

void FdoAssociationDeleteTest::delete_WithIdentObj()
{
    mInsertUtil->insert_WithIdentObj();

    masterTestWithObj( Delete_WithIdentityBothSet );
}

void FdoAssociationDeleteTest::delete_NoIdentObjNested()
{

    mInsertUtil->insert_NoIdentObjNested();
    masterTestWithObj( Delete_NoIdentityObjNested );
}
