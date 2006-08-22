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
#include "FdoAssociationInsertTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX	"_assoc"

static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = propertyValues->FindItem( name );

    if ( !propertyValue )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}

FdoAssociationInsertTest::FdoAssociationInsertTest(void)
{
}

FdoAssociationInsertTest::~FdoAssociationInsertTest(void)
{
}

void FdoAssociationInsertTest::masterTestNoObj( FdoAssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat, bool ownerIsFeat  )
{
    try
    {
		
        // Setup the schema
        if( assocIsFeat && ownerIsFeat )
             mSchemaUtil->TestCreate_NoIdentAssocFeatClass ();
        else if( !ownerIsFeat )
        {
            mSchemaUtil->TestCreate_WithIdentNoFeatClass();
        }
        else
        {
            
            if( type == Insert_NoIdentity )
                mSchemaUtil->TestCreate_NoIdent();
            else
                mSchemaUtil->TestCreate_WithIdent();
        }
        // Get a connection object
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

        FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
        
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
            id = (long)reader->GetInt64(L"FeatId");
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
        if( type == Insert_NoIdentity )
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
        
        if( type == Insert_WithIdentityParent || 
            type == Insert_WithIdentityBothSet || 
            type == Insert_NoIdentity ||
            type == Insert_WithIdentityError )
        {
            dataValue = FdoDataValue::Create( name1 );
	        propertyValue = AddNewProperty( propertyValues, L"First Name");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Last Name");
	        propertyValue->SetValue(dataValue);
        }
        if( type == Insert_WithIdentityAssociated || 
            type == Insert_WithIdentityBothSet ||
            type == Insert_WithIdentityError )
        {
            // Add the name one property
            if( type == Insert_WithIdentityError )
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
    
        reader = insertCommand->Execute();

        featureTransaction->Commit();
    }
    catch(FdoException *exp )
    {
        if( type != Insert_WithIdentityError )
        {
            printf("Insert Master Test(NO OBJ) Error: %ls\n", exp->GetExceptionMessage() );
		    UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
            exp->Release();
		    CPPUNIT_FAIL ( "Insert Master Test(NO OBJ) exception" );
        }
        else
        {
            printf("Insert Master Test(NO OBJ) Expected Error: %ls\n", exp->GetExceptionMessage() );
            exp->Release();
            return;
        }
	}
    if( type == Insert_WithIdentityError )
    {
        printf("Insert Master Test(NO OBJ) SHOULD Fail");
        CPPUNIT_FAIL ( "Insert Master Test(NO OBJ) SHOULD Fail" );
    }
}

void FdoAssociationInsertTest::insert_NoIdent(  )
{
    masterTestNoObj( Insert_NoIdentity, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationInsertTest::insert_NoIdentAssocFeatClass () 
{ 
    masterTestNoObj( Insert_NoIdentity, L"Firstname", L"Lastname", 10, true );
}

void FdoAssociationInsertTest::insert_WithIdent()
{
    masterTestNoObj( Insert_WithIdentityBothSet, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationInsertTest::insert_WithIdentNoFeatClass()
{
    masterTestNoObj( Insert_WithIdentityBothSet, L"Firstname", L"Lastname", 10, false, false );
}

// this test depend on insert_WithIdent which creates the valid schema:
// Always run insert_WithIdent before this test.
void FdoAssociationInsertTest::insert_ManyWithIdent()
{
    FdoStringP name1 = L"FirstName";
    FdoStringP name2 = L"LastName";

    
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

    FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
    
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
        
        dataValue = FdoDataValue::Create( (FdoString*)val1 );
	    propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( (FdoString*)val2 );
	    propertyValue = AddNewProperty( propertyValues, L"Association Prop1.Name Two");
	    propertyValue->SetValue(dataValue);
        reader = insertCommand->Execute();
    }
    featureTransaction->Commit();
}

void FdoAssociationInsertTest::insert_WithIdentParent() 
{ 
    masterTestNoObj( Insert_WithIdentityParent, L"Firstname", L"Lastname", 10 ); 
}

void FdoAssociationInsertTest::insert_WithIdentAssociated() 
{ 
    masterTestNoObj( Insert_WithIdentityAssociated, L"Firstname", L"Lastname", 10 ); 
}

void FdoAssociationInsertTest::insert_WithIdentError() 
{ 
    masterTestNoObj( Insert_WithIdentityError, L"Firstname", L"Lastname", 10 );
}


void FdoAssociationInsertTest::masterTestWithObj(FdoAssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id )
{
    try
    {
        // Get a connection object
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

        FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
        
        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);


        // Add an instance of the TestClass
        insertCommand->SetFeatureClassName(L"TestClass");
	    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        // Add the id property
        FdoPtr<FdoDataValue>dataValue = FdoDataValue::Create( 11 );
	    FdoPtr<FdoPropertyValue>propertyValue = AddNewProperty( propertyValues, L"Id");
	    propertyValue->SetValue(dataValue);
        // Add the name one property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = AddNewProperty( propertyValues, L"Name One");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = AddNewProperty( propertyValues, L"Name Two");
	    propertyValue->SetValue(dataValue);
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

        // Add an instance of the TestFeatureClass
        insertCommand->SetFeatureClassName(L"TestSubFeatureClass");
	    propertyValues = insertCommand->GetPropertyValues();

        // Add the id property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = AddNewProperty( propertyValues, L"First Name");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = AddNewProperty( propertyValues, L"Last Name");
	    propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create( 10 );
	    propertyValue = AddNewProperty( propertyValues, L"Id");
	    propertyValue->SetValue(dataValue);


        // Add the id property
        dataValue = FdoDataValue::Create( name1 );
	    propertyValue = AddNewProperty( propertyValues, L"Object.First Name");
	    propertyValue->SetValue(dataValue);
        // Add the name two property
        dataValue = FdoDataValue::Create( name2 );
	    propertyValue = AddNewProperty( propertyValues, L"Object.Last Name");
	    propertyValue->SetValue(dataValue);
        dataValue = FdoDataValue::Create( 10 );
	    propertyValue = AddNewProperty( propertyValues, L"Object.Id");
	    propertyValue->SetValue(dataValue);
        if( type == Insert_NoIdentity )
        {
            dataValue = FdoDataValue::Create( 11 );
	        propertyValue = AddNewProperty( propertyValues, L"Object.Association Prop1.Id");
	        propertyValue->SetValue(dataValue);
        }
        else if( type == Insert_NoIdentityObjNested )
        {
            dataValue = FdoDataValue::Create( name1 );
	        propertyValue = AddNewProperty( propertyValues, L"Object.LeafObject.First Name");
	        propertyValue->SetValue(dataValue);
            // Add the name two property
            dataValue = FdoDataValue::Create( name2 );
	        propertyValue = AddNewProperty( propertyValues, L"Object.LeafObject.Last Name");
	        propertyValue->SetValue(dataValue);
            dataValue = FdoDataValue::Create( 10 );
	        propertyValue = AddNewProperty( propertyValues, L"Object.LeafObject.Id");
	        propertyValue->SetValue(dataValue);
            dataValue = FdoDataValue::Create( 11 );
            propertyValue = AddNewProperty( propertyValues, L"Object.LeafObject.Association Prop1.Id");
            propertyValue->SetValue(dataValue);
        }

        reader = insertCommand->Execute();
        featureTransaction->Commit();
    }
    catch(FdoException *exp )
    {
        printf("Insert Master Test Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        exp->Release();
		CPPUNIT_FAIL ( "Insert Master Test(WITH OBJ) exception" );
    }

}

void FdoAssociationInsertTest::insert_NoIdentObj()
{
    
   mSchemaUtil->TestCreate_NoIdentObj();

   masterTestWithObj( Insert_NoIdentity, L"Firstname", L"Lastname", 10 );
}



void FdoAssociationInsertTest::insert_WithIdentObj()
{
    mSchemaUtil->TestCreate_WithIdentObj();

    masterTestWithObj( Insert_WithIdentityBothSet, L"Firstname", L"Lastname", 10 );
}

void FdoAssociationInsertTest::insert_NoIdentObjNested()
{

    mSchemaUtil->TestCreate_NoIdentObjNested();
    masterTestWithObj( Insert_NoIdentityObjNested, L"Firstname", L"Lastname", 10 );
}
