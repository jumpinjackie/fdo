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
#include "AssociationUpdateTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AssociationUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AssociationUpdateTest, "AssociationUpdateTest");


static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
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

AssociationUpdateTest::AssociationUpdateTest(void)
{
}

AssociationUpdateTest::~AssociationUpdateTest(void)
{
}


void AssociationUpdateTest::masterTestNoObj( AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat, bool ownerIsFeat  )
{
	static wchar_t* rev_mult = L"0_1";
	static wchar_t* old_rev_mult = L"1";
    try
    {
		if( type != WithIdentityError )
			schemaUtil.SetRevMultiplicity( rev_mult );
        // Setup the schema
       if( assocIsFeat )
             schemaUtil.TestCreate_NoIdentAssocFeatClass ();
        else
        {
            
            if( type == NoIdentity )
                schemaUtil.TestCreate_NoIdent();
            else
               schemaUtil.TestCreate_WithIdent();
        }
		schemaUtil.SetRevMultiplicity( old_rev_mult );
        // Get a connection object
        FdoPtr<FdoIConnection> connection = schemaUtil.openConnection( false );

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
	        propertyValue = AddNewProperty( propertyValues, L"Id");
	        propertyValue->SetValue(dataValue);
        }
        // Add the name one property
        dataValue = FdoDataValue::Create( L"Olfa" );
	    propertyValue = AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
		if( type == NoAssocUpdate )
		{
			dataValue = FdoDataValue::Create( L"Olfa" );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name One");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	        propertyValue->SetValue(dataValue);
		}
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        if( assocIsFeat && reader->ReadNext() )
        {
            id = (long)reader->GetInt32(L"FeatId");
        }
			

        // Add an instance of the TestFeatureClass
        if( type == WithIdentityParent )
            insertCommand->SetFeatureClassName(L"TestSubFeatureClass");
        else
            insertCommand->SetFeatureClassName(L"TestFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();
        
/*     
        if( type == WithIdentityParent || 
            type == WithIdentityBothSet || 
            type == NoIdentity ||
            type == WithIdentityError )
        {
*/
            dataValue = FdoDataValue::Create( name1 );
	        propertyValue = AddNewProperty( propertyValues, L"First Name");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Last Name");
	        propertyValue->SetValue(dataValue);
/*
        }
*/

        FdoPtr<FdoFilter>pFilter;

        if( ! ownerIsFeat )
        {
            dataValue = FdoDataValue::Create( 19 );
	        propertyValue = AddNewProperty( propertyValues, L"Id");
	        propertyValue->SetValue(dataValue);
            reader = insertCommand->Execute();
            pFilter = FdoComparisonCondition::Create(
		            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Id") ), 
		            FdoComparisonOperations_EqualTo, 
		            FdoPtr<FdoDataValue>(FdoDataValue::Create(19) ) ); 
        }
        else
        {
            FdoInt32 featId = -1;
            reader = insertCommand->Execute();
            if( reader->ReadNext() )
            {
                featId = reader->GetInt32(L"FeatId");
            }

            pFilter = FdoComparisonCondition::Create(
		            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ), 
		            FdoComparisonOperations_EqualTo, 
		            FdoPtr<FdoDataValue>(FdoDataValue::Create(featId) ) ); 
        }
        // Create and initialize the update command
        FdoPtr<FdoIUpdate>updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        if( type == WithIdentityParent )
            updateCommand->SetFeatureClassName(L"TestSubFeatureClass");
        else
            updateCommand->SetFeatureClassName(L"TestFeatureClass");
        updateCommand->SetFilter(pFilter);
	    propertyValues = updateCommand->GetPropertyValues();
        if( type == NoIdentity )
        {
            // Initialize the association property
            dataValue = FdoDataValue::Create( id );
            if( assocIsFeat )
                propertyValue = AddNewProperty( propertyValues, L"Association Prop1.FeatId");
            else
	            propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Id");
	        propertyValue->SetValue(dataValue);
        }
        else if( type == WithIdentityParent || 
            type == WithIdentityAssociated || 
            type == WithIdentityBothSet ||
            type == WithIdentityError )
        {
            // Add the name one property
            if( type == WithIdentityError )
                dataValue = FdoDataValue::Create( L"BOGUS" );
            else
                dataValue = FdoDataValue::Create( L"Olfa" );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name One");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	        propertyValue->SetValue(dataValue);
            if( type == WithIdentityBothSet ||
                type == WithIdentityError )
            {
                dataValue = FdoDataValue::Create( L"Olfa" );
	            propertyValue = AddNewProperty( propertyValues, L"First Name");
	            propertyValue->SetValue(dataValue);
                // Add the name two property
                dataValue = FdoDataValue::Create( name2 );
	            propertyValue = AddNewProperty( propertyValues, L"Last Name");
	            propertyValue->SetValue(dataValue);
            }
        }
		else if( type == NoAssocUpdate )
		{
			dataValue = FdoDataValue::Create( L"Test data" );
	        propertyValue = AddNewProperty( propertyValues, L"PlaceHolder");
	        propertyValue->SetValue(dataValue);
		}
    
        updateCommand->Execute();
    }
    catch(FdoException *exp )
    {
        if( type != WithIdentityError )
        {
            printf("Update Master Test(NO OBJ) Error: %ls\n", exp->GetExceptionMessage() );
            exp->Release();
		    throw "Update Master Test(NO OBJ) exception";
        }
        else
        {
            printf("Update Master Test(NO OBJ) Expected Error: %ls\n", exp->GetExceptionMessage() );
            exp->Release();
            return;
        }
	}
    if( type == WithIdentityError )
    {
        printf("Update Master Test(NO OBJ) SHOULD Fail");
        throw "Update Master Test(NO OBJ) SHOULD Fail";
    }
}

void AssociationUpdateTest::update_NoIdent(  )
{
    masterTestNoObj( NoIdentity, L"Karim", L"Makram", 10 );
}

void AssociationUpdateTest::update_NoAssocSet(  )
{
    masterTestNoObj( NoAssocUpdate, L"Karim", L"Makram", 10 );
}

void AssociationUpdateTest::update_NoIdentAssocFeatClass () 
{ 
    masterTestNoObj( NoIdentity, L"Karim", L"Makram", 10, true );
}

void AssociationUpdateTest::update_WithIdent()
{
    masterTestNoObj( WithIdentityBothSet, L"Karim", L"Makram", 10 );
}

void AssociationUpdateTest::update_WithIdentNoFeatClass()
{
    masterTestNoObj( WithIdentityBothSet, L"Karim", L"Makram", 10, false, true );
}

void AssociationUpdateTest::update_WithIdentParent() 
{ 
    masterTestNoObj( WithIdentityParent, L"Karim", L"Makram", 10 ); 
}

void AssociationUpdateTest::update_WithIdentError() 
{ 
    masterTestNoObj( WithIdentityError, L"Karim", L"Makram", 10 );
}




