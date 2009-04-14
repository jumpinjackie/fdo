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
#include "AssociationSelectTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION( AssociationSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AssociationSelectTest, "AssociationSelectTest");

AssociationSelectTest::AssociationSelectTest(void)
{
}

AssociationSelectTest::~AssociationSelectTest(void)
{
}

void AssociationSelectTest::masterTestNoObj( AssociationSelectType type, bool assocIsFeat, bool ownerIsFeat  )
{
    bool        failed = true;
    try
    {
        // Setup the data
       if( type == Select_NoIdentity )
       {
           if( assocIsFeat )
            insertUtil.insert_NoIdentAssocFeatClass();
           else
            insertUtil.insert_NoIdent();
       }
       else
       {
           if( ownerIsFeat )
                insertUtil.insert_WithIdent();
           else
                insertUtil.insert_WithIdentNoFeatClass();
       }

        FdoPtr<FdoIConnection> connection = insertUtil.openConnection(false);


        FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		selCmd->SetFeatureClassName(L"TestFeatureClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Select_NoIdentity && ! assocIsFeat )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)10) ) ); 
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Name One") ), 
						//FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"First Name") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Karim") ) );

        selCmd->SetFilter( pFilter );
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
        if( myReader != NULL  )
        {
            while ( myReader->ReadNext() )
            {
               if( ! myReader->IsNull( L"Association Prop1") )
               {
                   int featid;
                   if( ownerIsFeat )
                        featid = myReader->GetInt32(L"FeatId");
                   else
                        featid = -1;
                    FdoPtr<FdoIFeatureReader>assocReader = myReader->GetFeatureObject( L"Association Prop1" );
                    if( assocReader && assocReader->ReadNext() )
                    {
                        CPPUNIT_ASSERT( wcscmp(assocReader->GetString(L"Name One"),L"Karim") == 0 );
                        failed = false;
                    }
               }
               if( type == Select_NoIdentity && ! myReader->IsNull( L"Association Prop2") )
               {
                   int featid;
                   if( ownerIsFeat )
                        featid = myReader->GetInt32(L"FeatId");
                   else
                        featid = -1;
                    FdoPtr<FdoIFeatureReader>assocReader = myReader->GetFeatureObject( L"Association Prop2" );
                    if( assocReader && assocReader->ReadNext() )
                    {
                        CPPUNIT_ASSERT( wcscmp(assocReader->GetString(L"Name One"),L"Karim") == 0 );
                        failed = false;
                    }
               }
            }
        }
       
    }
    catch(FdoException *exp )
    {
        printf("Select Master Test abnormal Error: %ls\n", exp->GetExceptionMessage() );
        exp->Release();
        CPPUNIT_ASSERT( false );
	}
    CPPUNIT_ASSERT( ! failed );
}

void AssociationSelectTest::masterTestNoObjReverse( AssociationSelectType type  )
{
    bool        failed = true;
    try
    {
        // Setup the data
       if( type == Select_NoIdentity )
            insertUtil.insert_NoIdent();
        else
            insertUtil.insert_WithIdent();

        FdoPtr<FdoIConnection> connection = insertUtil.openConnection(false);


        FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		selCmd->SetFeatureClassName(L"TestClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Select_NoIdentity )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)10) ) ); 
            //selCmd->SetFilter(L"Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Karim") ) );
            //selCmd->SetFilter(L"'Name One'='Karim'");

        selCmd->SetFilter( pFilter );
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
        if( myReader != NULL  )
        {
            while ( myReader->ReadNext() )
            {
               if( ! myReader->IsNull( L"Reverse Name") )
               {
                    FdoPtr<FdoIFeatureReader>assocReader = myReader->GetFeatureObject( L"Reverse Name" );
                    if( assocReader && assocReader->ReadNext() )
                    {
                        //printf("Association Name( REVERSE ) = %ls\n", assocReader->GetString(L"First Name") );
						CPPUNIT_ASSERT( ( wcscmp(assocReader->GetString(L"First Name"),L"Karim") == 0 ) );
                        failed = false;
                    }
               }
            }
        }
       
    }
    catch( FdoException *exp )
    {
        printf("Select Master Test abnormal Error: %ls\n", exp->GetExceptionMessage() );
        exp->Release();
        CPPUNIT_ASSERT( false );
	}
    CPPUNIT_ASSERT( ! failed );
}

void AssociationSelectTest::select_NoIdent(  )
{
    masterTestNoObj( Select_NoIdentity );
}

void AssociationSelectTest::select_NoIdentFeatClass(  )
{
    masterTestNoObj( Select_NoIdentity, true );
}

void AssociationSelectTest::select_WithIdent()
{
    masterTestNoObj( Select_WithIdentityBothSet );
}

void AssociationSelectTest::select_WithIdentNoFeatClass()
{
    masterTestNoObj( Select_WithIdentityBothSet, false, false );
}

void AssociationSelectTest::select_NoIdentReverse(  )
{
    masterTestNoObjReverse( Select_NoIdentity );
}

void AssociationSelectTest::select_WithIdentReverse()
{
    masterTestNoObjReverse( Select_WithIdentityBothSet );
}





