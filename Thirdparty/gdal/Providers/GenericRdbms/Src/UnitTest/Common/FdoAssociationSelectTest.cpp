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
#include "FdoAssociationSelectTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX	L"_assoc"

FdoAssociationSelectTest::FdoAssociationSelectTest(void)
{
}

FdoAssociationSelectTest::~FdoAssociationSelectTest(void)
{
}

void FdoAssociationSelectTest::masterTestNoObj( AssociationSelectType type, bool assocIsFeat, bool ownerIsFeat, int circularType  )
{
    bool        failed = true;
    try
    {
        // Setup the data
       if( type == Select_NoIdentity )
       {
           if( assocIsFeat )
           {
               if ( circularType > 0 )
                   mInsertUtil->insert_NoIdentAssocFeatClassCirc(circularType);
               else
                   mInsertUtil->insert_NoIdentAssocFeatClass();
           }
           else
           {
               if ( circularType > 0 )
                   mInsertUtil->insert_NoIdentCirc(circularType);
               else
                   mInsertUtil->insert_NoIdent();
           }
       }
       else
       {
           if( ownerIsFeat )
           {
               if ( circularType > 0 )
                   mInsertUtil->insert_WithIdentCirc(circularType);
               else
                   mInsertUtil->insert_WithIdent();
           }
           else
           {
               if ( circularType > 0 )
                   mInsertUtil->insert_WithIdentNoFeatClassCirc(circularType);
               else
                   mInsertUtil->insert_WithIdentNoFeatClass();
           }
       }

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);


        FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		selCmd->SetFeatureClassName(L"TestFeatureClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Select_NoIdentity && ! assocIsFeat )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"10") ) ); 
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Association Prop1.Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );

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
                        FdoString  *name = assocReader->GetString(L"Name One");
                        printf("Feat(%d): Association Name = %ls\n",featid, name );
                        if( wcscmp(name,L"Firstname") == 0 )
                            failed = false;
                    }
               }
               if( circularType == 0 && type == Select_NoIdentity && ! myReader->IsNull( L"Association Prop2") )
               {
                   int featid;
                   if( ownerIsFeat )
                        featid = myReader->GetInt32(L"FeatId");
                   else
                        featid = -1;
                    FdoPtr<FdoIFeatureReader>assocReader = myReader->GetFeatureObject( L"Association Prop2" );
                    if( assocReader && assocReader->ReadNext() )
                    {
                        FdoString  *name = assocReader->GetString(L"Name One");
                        printf("Feat(%d): Association Name = %ls\n",featid, name );
                        if( wcscmp(name,L"Firstname") == 0 )
                            failed = false;
                    }
               }
            }
        }
       
    }
    catch(FdoException *exp )
    {
        printf("Select Master Test(NO OBJ)Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        exp->Release();
        CPPUNIT_FAIL ( "Select Master Test(OBJ) exception" );
	}
    if( failed )
    {
        printf("Select Master Test(OBJ) failed\n");
        CPPUNIT_FAIL ( "Select Master Test(OBJ) failed" );
    }
}

void FdoAssociationSelectTest::masterTestNoObjReverse( AssociationSelectType type  )
{
    bool        failed = true;
    try
    {
        // Setup the data
       if( type == Select_NoIdentity )
            mInsertUtil->insert_NoIdent();
        else
            mInsertUtil->insert_WithIdent();

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);


        FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		selCmd->SetFeatureClassName(L"TestClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Select_NoIdentity )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"10") ) ); 
            //selCmd->SetFilter(L"Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );
            //selCmd->SetFilter(L"'Name One'='Firstname'");

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
                        printf("Association Name( REVERSE ) = %ls\n", assocReader->GetString(L"First Name") );
                        failed = false;
                    }
               }
            }
        }
       
    }
    catch(FdoException *exp )
    {
        printf("Select Master Test(NO OBJ REVERSE)Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        exp->Release();
        CPPUNIT_FAIL ( "Select Master Test(OBJ REVERSE) exception" );
	}
    if( failed )
    {
        printf("Select Master Test(OBJ REVERSE) failed\n");
        CPPUNIT_FAIL ( "Select Master Test(OBJ REVERSE) failed" );
    }
}

void FdoAssociationSelectTest::select_NoIdent(  )
{
    masterTestNoObj( Select_NoIdentity );
}

void FdoAssociationSelectTest::select_NoIdentFeatClass(  )
{
    masterTestNoObj( Select_NoIdentity, true );
}

void FdoAssociationSelectTest::select_WithIdent()
{
    masterTestNoObj( Select_WithIdentityBothSet );
}

void FdoAssociationSelectTest::select_WithIdentNoFeatClass()
{
    masterTestNoObj( Select_WithIdentityBothSet, false, false );
}

void FdoAssociationSelectTest::select_NoIdentReverse(  )
{
    masterTestNoObjReverse( Select_NoIdentity );
}

void FdoAssociationSelectTest::select_WithIdentReverse()
{
    masterTestNoObjReverse( Select_WithIdentityBothSet );
}

void FdoAssociationSelectTest::masterTestWithObj(AssociationSelectType type )
{
    bool        failed = true;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX);

        FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		selCmd->SetFeatureClassName(L"TestSubFeatureClass");
        FdoPtr<FdoComparisonCondition>pFilter;
        if( type == Select_NoIdentityObjNested )
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.LeafObject.Association Prop1.Id") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"11") ) ); 
            //selCmd->SetFilter(L"Association Prop1.Id=10");
        else
            pFilter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.Association Prop1.Name One") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Firstname") ) );
            //selCmd->SetFilter(L"Association Prop1.Name One='Firstname'");

        selCmd->SetFilter( pFilter );
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
        if( myReader != NULL  )
        {
             while ( myReader->ReadNext() )
            {
               FdoPtr<FdoIFeatureReader>objReader;
               if( type == Select_NoIdentityObjNested )
               {
                    FdoPtr<FdoIFeatureReader>obj1Reader=myReader->GetFeatureObject(L"Object");
                    if(obj1Reader && obj1Reader->ReadNext() )
                        objReader = obj1Reader->GetFeatureObject(L"LeafObject");
               }
               else
                    objReader = myReader->GetFeatureObject(L"Object");
               if( objReader && objReader->ReadNext() && ! objReader->IsNull( L"Association Prop1") )
               {
                    FdoPtr<FdoIFeatureReader>assocReader = objReader->GetFeatureObject( L"Association Prop1" );
                    if( assocReader && assocReader->ReadNext() )
                    {
                        printf("Association Name = %ls\n", assocReader->GetString(L"Name One") );
                        failed = false;
                    }
               }
            }
        }
    }
    catch(FdoException *exp )
    {
        printf("Select Master Test(OBJ) Error: %ls\n", exp->GetExceptionMessage() );
		UnitTestUtil::PrintException(exp, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
		CPPUNIT_FAIL ( "Select Master Test(OBJ) exception" );
    }

    if( failed )
    {
        printf("Select Master Test(OBJ) failed\n");
        CPPUNIT_FAIL ( "Select Master Test(OBJ) failed" );
    }
}

void FdoAssociationSelectTest::select_NoIdentObj()
{
    
   mInsertUtil->insert_NoIdentObj();

   masterTestWithObj( Select_NoIdentity );
}

void FdoAssociationSelectTest::select_WithIdentObj()
{
    mInsertUtil->insert_WithIdentObj();

    masterTestWithObj( Select_WithIdentityBothSet );
}

void FdoAssociationSelectTest::select_NoIdentObjNested()
{

    mInsertUtil->insert_NoIdentObjNested();
    masterTestWithObj( Select_NoIdentityObjNested );
}

void FdoAssociationSelectTest::select_NoIdentFeatClassCirc()
{
    masterTestNoObj( Select_WithIdentityAssociated, false, false, 1 );
}

void FdoAssociationSelectTest::select_NoIdentNoFeatClassCirc()
{
    masterTestNoObj( Select_WithIdentityAssociated, false, false, 2 );
}

void FdoAssociationSelectTest::select_NoIdentBothClassCirc()
{
    masterTestNoObj( Select_WithIdentityAssociated, false, false, 3 );
}





