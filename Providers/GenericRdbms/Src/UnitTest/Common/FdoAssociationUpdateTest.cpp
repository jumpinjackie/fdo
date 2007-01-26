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
#include "FdoAssociationUpdateTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX	L"_assoc"

FdoAssociationUpdateTest::FdoAssociationUpdateTest(void)
{
}

FdoAssociationUpdateTest::~FdoAssociationUpdateTest(void)
{
}

void FdoAssociationUpdateTest::masterTestNoObj( AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat, bool ownerIsFeat  )
{
    try
    {
        // Setup the schema
       if( assocIsFeat )
             mSchemaUtil->TestCreate_NoIdentAssocFeatClass ();
        else
        {
            
            if( type == Update_NoIdentity )
                mSchemaUtil->TestCreate_NoIdent();
            else
               mSchemaUtil->TestCreate_WithIdent();
        }
        // Get a connection object
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

        // Modify the meta data
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        sqlCmd->SetSQLStatement(L"update f_associationdefinition set REVERSEMULTIPLICITY = '0_1' where REVERSEMULTIPLICITY = '1'");
		sqlCmd->ExecuteNonQuery();

        
        FdoPtr<FdoIDelete>deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        if( type == Update_WithIdentityParent )
            deleteCommand->SetFeatureClassName(L"TestSubFeatureClass");
        else
            deleteCommand->SetFeatureClassName(L"TestFeatureClass");
        deleteCommand->Execute();

        deleteCommand->SetFeatureClassName(L"TestClass");
        deleteCommand->Execute();

        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);

        // Add an instance of the TestClass
        insertCommand->SetFeatureClassName(L"TestClass");
	    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        // Add the id property
        FdoPtr<FdoDataValue>dataValue;
        FdoPtr<FdoPropertyValue>propertyValue;
        if( ! assocIsFeat )
        {
            dataValue = FdoDataValue::Create( id );
			propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Id");
	        propertyValue->SetValue(dataValue);
        }
        // Add the name one property
        dataValue = FdoDataValue::Create( L"Olfa" );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        if( assocIsFeat && reader->ReadNext() )
        {
            id = (long)reader->GetInt64(L"FeatId");
        }

        // Add an instance of the TestFeatureClass
        if( type == Update_WithIdentityParent )
            insertCommand->SetFeatureClassName(L"TestSubFeatureClass");
        else
            insertCommand->SetFeatureClassName(L"TestFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();
        
     
        if( type == Update_WithIdentityParent || 
            type == Update_WithIdentityBothSet || 
            type == Update_NoIdentity ||
            type == Update_WithIdentityError )
        {
            dataValue = FdoDataValue::Create( name1 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"First Name");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Last Name");
	        propertyValue->SetValue(dataValue);
        }

        FdoPtr<FdoFilter>pFilter;

        if( ! ownerIsFeat )
        {
            dataValue = FdoDataValue::Create( 19 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Id");
	        propertyValue->SetValue(dataValue);
            reader = insertCommand->Execute();
            pFilter = FdoComparisonCondition::Create(
		            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Id") ), 
		            FdoComparisonOperations_EqualTo, 
		            FdoPtr<FdoDataValue>(FdoDataValue::Create(19) ) ); 
        }
        else
        {
            FdoInt64 featId = -1;
            reader = insertCommand->Execute();
            if( reader->ReadNext() )
            {
                featId = reader->GetInt64(L"FeatId");
            }

            pFilter = FdoComparisonCondition::Create(
		            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ), 
		            FdoComparisonOperations_EqualTo, 
		            FdoPtr<FdoDataValue>(FdoDataValue::Create(featId) ) ); 
        }
        // Create and initialize the update command
        FdoPtr<FdoIUpdate>updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        if( type == Update_WithIdentityParent )
            updateCommand->SetFeatureClassName(L"TestSubFeatureClass");
        else
            updateCommand->SetFeatureClassName(L"TestFeatureClass");
        updateCommand->SetFilter(pFilter);
	    propertyValues = updateCommand->GetPropertyValues();
        if( type == Update_NoIdentity )
        {
            // Initialize the association property
            dataValue = FdoDataValue::Create( id );
            if( assocIsFeat )
                propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Association Prop1.FeatId");
            else
	            propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Association Prop1.Id");
	        propertyValue->SetValue(dataValue);
        }
        if( type == Update_WithIdentityParent || 
            type == Update_WithIdentityAssociated || 
            type == Update_WithIdentityBothSet ||
            type == Update_WithIdentityError )
        {
            // Add the name one property
            if( type == Update_WithIdentityError )
                dataValue = FdoDataValue::Create( L"BOGUS" );
            else
                dataValue = FdoDataValue::Create( L"Olfa" );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Association Prop1.Name One");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	        propertyValue->SetValue(dataValue);
            if( type == Update_WithIdentityBothSet ||
                type == Update_WithIdentityError )
            {
                dataValue = FdoDataValue::Create( L"Olfa" );
	            propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"First Name");
	            propertyValue->SetValue(dataValue);
                // Add the name two property
                dataValue = FdoDataValue::Create( name2 );
	            propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Last Name");
	            propertyValue->SetValue(dataValue);
            }
        }
    
        updateCommand->Execute();
    }
    catch(FdoException *exp )
    {
        if( type != Update_WithIdentityError )
        {
            printf("Update Master Test(NO OBJ) Error: %ls\n", exp->GetExceptionMessage() );
		    UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
            exp->Release();
		    CPPUNIT_FAIL ( "Update Master Test(NO OBJ) exception" );
        }
        else
        {
            printf("Update Master Test(NO OBJ) Expected Error: %ls\n", exp->GetExceptionMessage() );
            exp->Release();
            return;
        }
	}
    if( type == Update_WithIdentityError )
    {
        printf("Update Master Test(NO OBJ) SHOULD Fail");
        CPPUNIT_FAIL ( "Update Master Test(NO OBJ) SHOULD Fail" );
    }
}

void FdoAssociationUpdateTest::update_NoIdent(  )
{
    masterTestNoObj( Update_NoIdentity, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationUpdateTest::update_NoIdentAssocFeatClass () 
{ 
    masterTestNoObj( Update_NoIdentity, L"Firstname", L"Lastname", 10, true );
}

void FdoAssociationUpdateTest::update_WithIdent()
{
    masterTestNoObj( Update_WithIdentityBothSet, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationUpdateTest::update_WithIdentNoFeatClass()
{
    masterTestNoObj( Update_WithIdentityBothSet, L"Firstname", L"Lastname", 10, false, true );
}

void FdoAssociationUpdateTest::update_WithIdentParent() 
{ 
    masterTestNoObj( Update_WithIdentityParent, L"Firstname", L"Lastname", 10 ); 
}

void FdoAssociationUpdateTest::update_WithIdentError() 
{ 
    masterTestNoObj( Update_WithIdentityError, L"Firstname", L"Lastname", 10 );
}


void FdoAssociationUpdateTest::masterTestWithObj(AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id )
{
    try
    {
        // Get a connection object
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

        // Modify the meta data
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        sqlCmd->SetSQLStatement(L"update f_associationdefinition set REVERSEMULTIPLICITY = '0_1' where REVERSEMULTIPLICITY = '1'");
		sqlCmd->ExecuteNonQuery();

        
        // Cleanup the existing data
        FdoPtr<FdoIDelete>deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"TestSubFeatureClass");
        deleteCommand->Execute();

        deleteCommand->SetFeatureClassName(L"TestClass");
        deleteCommand->Execute();


        FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
        
        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);

        // Add an instance of the TestClass
        insertCommand->SetFeatureClassName(L"TestClass");
	    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        // Add the id property
        FdoPtr<FdoDataValue>dataValue = FdoDataValue::Create( 11 );
	    FdoPtr<FdoPropertyValue>propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Id");
	    propertyValue->SetValue(dataValue);
        // Add the name one property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
        insertCommand->Execute();

        // Add an instance of the TestFeatureClass with no association property initialized
        insertCommand->SetFeatureClassName(L"TestSubFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();

        // Add the id property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"First Name");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Last Name");
	    propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create( 10 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Id");
	    propertyValue->SetValue(dataValue);


        // Add the id property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.First Name");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.Last Name");
	    propertyValue->SetValue(dataValue);
        dataValue = FdoDataValue::Create( 10 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.Id");
	    propertyValue->SetValue(dataValue);
        FdoInt64 featId = -1;
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        if( reader->ReadNext() )
        {
            featId = reader->GetInt64(L"FeatId");
        }
        featureTransaction->Commit();

        // Create and update command and use it to update set the association link.
        FdoPtr<FdoIUpdate>updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"TestSubFeatureClass");
        FdoPtr<FdoFilter>pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(featId) ) ); 
        updateCommand->SetFilter(pFilter);
	    propertyValues = updateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create( 10 );
	    propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.Id");
	    propertyValue->SetValue(dataValue);
        if( type == Update_NoIdentity )
        {
            dataValue = FdoDataValue::Create( 11 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.Association Prop1.Id");
	        propertyValue->SetValue(dataValue);
        }
        else if( type == Update_NoIdentityObjNested )
        {
            dataValue = FdoDataValue::Create( name1 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.LeafObject.First Name");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.LeafObject.Last Name");
	        propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create( 10 );
	        propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.LeafObject.Id");
	        propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create( 11 );
            propertyValue = FdoAssociationUpdateTest::AddNewProperty( propertyValues, L"Object.LeafObject.Association Prop1.Id");
            propertyValue->SetValue(dataValue);
        }

        updateCommand->Execute();
    }
    catch(FdoException *exp )
    {
        printf("Update Master Test Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        exp->Release();
		CPPUNIT_FAIL ( "Update Master Test(WITH OBJ) exception" );
    }

}

void FdoAssociationUpdateTest::update_NoIdentObj()
{
    
   mSchemaUtil->TestCreate_NoIdentObj();

   masterTestWithObj( Update_NoIdentity, L"Firstname", L"Lastname", 10 );
}



void FdoAssociationUpdateTest::update_WithIdentObj()
{
    mSchemaUtil->TestCreate_WithIdentObj();

    masterTestWithObj( Update_WithIdentityBothSet, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationUpdateTest::update_NoIdentObjNested()
{

    mSchemaUtil->TestCreate_NoIdentObjNested();
    masterTestWithObj( Update_NoIdentityObjNested, L"Firstname", L"Lastname", 10 );
}

FdoPropertyValue* FdoAssociationUpdateTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;
    try
    {
        propertyValue = propertyValues->GetItem( name );
    }
    catch( FdoException *exp )
    {
        exp->Release();
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}
