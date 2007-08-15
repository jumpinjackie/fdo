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
#include "AssociationInsertTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AssociationInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AssociationInsertTest, "AssociationInsertTest");

static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*   propertyValue = propertyValues->FindItem( name );
    if( propertyValue == NULL )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}

AssociationInsertTest::AssociationInsertTest(void)
{
}

AssociationInsertTest::~AssociationInsertTest(void)
{
}

void AssociationInsertTest::masterTestNoObj( AssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat, bool ownerIsFeat  )
{
    try
    {
        // Setup the schema
		if( type != NoSchema )
		{
			if( assocIsFeat && ownerIsFeat )
				schemaUtil.TestCreate_NoIdentAssocFeatClass ();
			else if( !ownerIsFeat )
			{
				schemaUtil.TestCreate_WithIdentNoFeatClass();
			}
			else
			{
	            
				if( type == NoIdentity )
					schemaUtil.TestCreate_NoIdent();
				else
					schemaUtil.TestCreate_WithIdent();
			}
		}
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
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        if( assocIsFeat && ownerIsFeat && reader->ReadNext() )
        {
            id = (long)reader->GetInt32(L"FeatId");
			FdoPtr<FdoClassDefinition>cls = reader->GetClassDefinition();
			CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"TestClass",cls->GetName())==0);
        }

        // Add an instance of the TestFeatureClass
        insertCommand->SetFeatureClassName(L"TestFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();
        
        // Initialize the association property
        
        if( !ownerIsFeat )
        {
            dataValue = FdoDataValue::Create(19);
            propertyValue = AddNewProperty( propertyValues, L"Id");
            propertyValue->SetValue(dataValue);
        }
        if( type == NoIdentity )
        {
            // Initialize the association property
            dataValue = FdoDataValue::Create( id );
            if( assocIsFeat )
                propertyValue = AddNewProperty( propertyValues, L"Association Prop1.FeatId");
            else
	            propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Id");
	        propertyValue->SetValue(dataValue);

            // Initialize the second association
            if( assocIsFeat )
                propertyValue = AddNewProperty( propertyValues, L"Association Prop2.FeatId");
            else
	            propertyValue = AddNewProperty( propertyValues, L"Association Prop2.Id");
	        propertyValue->SetValue(dataValue);
        }
        
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
        if( type == WithIdentityAssociated || 
            type == WithIdentityBothSet ||
            type == WithIdentityError ||
			type == NoSchema )
        {
            // Add the name one property
            if( type == WithIdentityError )
                dataValue = FdoDataValue::Create( L"BOGUS" );
            else
                dataValue = FdoDataValue::Create( name1 );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name One");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	        propertyValue->SetValue(dataValue);
        }

		dataValue = FdoDataValue::Create( L"Test data" );
        propertyValue = AddNewProperty( propertyValues, L"PlaceHolder");
        propertyValue->SetValue(dataValue);


        reader = insertCommand->Execute();
		FdoPtr<FdoClassDefinition>cls = reader->GetClassDefinition();
		CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"TestFeatureClass",cls->GetName())==0);
    }
    catch(FdoException *exp )
    {
        if( type != WithIdentityError )
        {
            printf("Insert Master Test Error: %ls\n", exp->GetExceptionMessage() );
            exp->Release();
		    throw "Insert Master Test exception";
        }
        else
        {
            // Insert Master Test Expected Error
			// TODO: should check for the specific exception
            exp->Release();
            return;
        }
	}
	CPPUNIT_ASSERT_MESSAGE("Insert Master Test SHOULD Fail",!(type == WithIdentityError));
}

void AssociationInsertTest::insert_NoIdent(  )
{
    masterTestNoObj( NoIdentity, L"Karim", L"Makram", 10 );
}

void AssociationInsertTest::insert_NoIdentAssocFeatClass () 
{ 
    masterTestNoObj( NoIdentity, L"Karim", L"Makram", 10, true );
}

void AssociationInsertTest::insert_WithIdent()
{
    masterTestNoObj( WithIdentityBothSet, L"Karim", L"Makram", 10 );
}

void AssociationInsertTest::insert_Cardinality_1_1()
{
	schemaUtil.TestCreate_Cardinality_1_1();
    masterTestNoObj( NoSchema, L"Karim", L"Makram", 10 );
}

void AssociationInsertTest::insert_WithIdentNoFeatClass()
{
    masterTestNoObj( WithIdentityBothSet, L"Karim", L"Makram", 10, false, false );
}

// this test depend on insert_WithIdent which creates the valid schema:
// Always run insert_WithIdent before this test.
void AssociationInsertTest::insert_ManyWithIdent()
{
    FdoStringP name1 = L"FirstName";
    FdoStringP name2 = L"LastName";
	
	insert_WithIdent();
    
    FdoPtr<FdoIConnection> connection = schemaUtil.openConnection( false );

    
    FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);

    // Add instances of the TestClass
    
	FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue>dataValue;
    FdoPtr<FdoPropertyValue>propertyValue;
    FdoPtr<FdoIFeatureReader> reader;
    for(int i=90; i<100; i++ )
    {
        char  id[4];
        
        sprintf(id,"%d",i);
        FdoStringP idx(id);
        FdoStringP  val1 = name1 + idx;
        FdoStringP  val2 = name2 + idx;
        insertCommand->SetFeatureClassName(L"TestClass");
        propertyValues = insertCommand->GetPropertyValues();
        dataValue = FdoDataValue::Create( i );
	    propertyValue = AddNewProperty( propertyValues, L"Id");
	    propertyValue->SetValue(dataValue);
        // Add the name one property
        dataValue = FdoDataValue::Create( (FdoString*)val1 );
	    propertyValue = AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( (FdoString*)val2 );
	    propertyValue = AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
        reader = insertCommand->Execute();

        // Add an instance of the TestFeatureClass
        insertCommand->SetFeatureClassName(L"TestFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();
        
        dataValue = FdoDataValue::Create( L"Jim" );
	    propertyValue = AddNewProperty( propertyValues, L"First Name");
	    propertyValue->SetValue(dataValue);
        dataValue = FdoDataValue::Create( L"Slim" );
	    propertyValue = AddNewProperty( propertyValues, L"Last Name");
	    propertyValue->SetValue(dataValue);
        dataValue = FdoDataValue::Create( L"Some placeholder value" );
	    propertyValue = AddNewProperty( propertyValues, L"PlaceHolder");
	    propertyValue->SetValue(dataValue);
        dataValue = FdoDataValue::Create( (FdoString*)val1 );
	    propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( (FdoString*)val2 );
	    propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	    propertyValue->SetValue(dataValue);
        reader = insertCommand->Execute();
    }
}

void AssociationInsertTest::insert_WithIdentParent() 
{ 
    masterTestNoObj( WithIdentityParent, L"Karim", L"Makram", 10 ); 
}

void AssociationInsertTest::insert_WithIdentAssociated() 
{ 
    masterTestNoObj( WithIdentityAssociated, L"Karim", L"Makram", 10 ); 
}

void AssociationInsertTest::insert_WithIdentError() 
{ 
    masterTestNoObj( WithIdentityError, L"Karim", L"Makram", 10 );
}

