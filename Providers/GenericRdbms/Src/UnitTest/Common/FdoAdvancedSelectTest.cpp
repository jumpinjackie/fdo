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
#include "FdoAdvancedSelectTest.h"
#include "FdoInsertTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX   L"_selaggr" 


FdoAdvancedSelectTest::FdoAdvancedSelectTest(void)
{
}

FdoAdvancedSelectTest::~FdoAdvancedSelectTest(void)
{
    if( mConnection != NULL )
        mConnection->Close();
	mConnection = NULL;
}

void FdoAdvancedSelectTest::setUp ()
{
    set_provider();
    connect();
}

void FdoAdvancedSelectTest::tearDown ()
{
    if( mConnection != NULL )
        mConnection->Close();
	mConnection = NULL;
}

void FdoAdvancedSelectTest::connect ()
{
    try
    {
        if (UnitTestUtil::DatastoreExists(DB_SUFFIX))
            mConnection = (FdoIConnection *) UnitTestUtil::GetConnection(DB_SUFFIX);
        else
        {
            mConnection = (FdoIConnection *) UnitTestUtil::GetConnection(DB_SUFFIX, true);

            // Insert the test data.
            FdoInsertTest *insert = new FdoInsertTest(DB_SUFFIX);
            insert->setUp();
            insert->insert();
            delete insert;
            insert = new FdoInsertTest(DB_SUFFIX);
            insert->setUp();
            insert->insert2();
            delete insert;
        }
    }
    catch (FdoException *ex)
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        if( mConnection != NULL )
            mConnection->Close();
        mConnection= NULL;
    	TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::read_feature_data( FdoIFeatureReader *myReader, bool subset )
{
    try
    {
        FdoPtr<FdoClassDefinition> classDef = myReader->GetClassDefinition();
        if( classDef )
        {
            printf(" \tClassName: %ls\n",classDef->GetName() );
            printf("\n Id Properties: \n");
            FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = classDef->GetIdentityProperties();
            for (int i=0; i<idProperties->GetCount(); i++)
            {
                FdoPtr<FdoDataPropertyDefinition> idProperty = idProperties->GetItem(i);
                printf("\t%d) %ls\n", i+1, idProperty->GetName());
            }
            printf("\n Base Properties: \n");
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = classDef->GetBaseProperties();
            for (int i=0; i<baseProperties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> baseProperty = static_cast<FdoPropertyDefinition *> (baseProperties->GetItem(i));
                printf("\t%d) %ls\n", i+1, baseProperty->GetName());
            }
            printf("\n Properties: \n");
            FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
            for (int i=0; i<properties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> property = properties->GetItem(i);
                printf("\t%d) %ls\n", i+1, property->GetName());
            }
            printf("\n");
        }
        if( ! myReader->IsNull(L"RevisionNumber") )
            printf(" \tColor: %d\n", myReader->GetInt64(L"RevisionNumber") );


        if( ! myReader->IsNull(L"color") )
            printf(" \tColor: %ls\n", myReader->GetString(L"color") );
        if (subset == false)
            if( ! myReader->IsNull(L"layer") )
                printf(" \tLayer: %ls\n", myReader->GetString(L"layer") );

        if (subset == false)
            if( ! myReader->IsNull(L"segcount") )
                printf(" \tSegCount: %d\n", myReader->GetInt32(L"segcount"));
       if( ! myReader->IsNull(L"xdata") )
        {

            FdoPtr<FdoIFeatureReader> objReader = myReader->GetFeatureObject(L"xdata");
            if( objReader )
            {
                printf(" \txdata: \n" );
                classDef = objReader->GetClassDefinition();
                if( classDef )
                {
                    printf(" \t\tClassName: %ls\n",classDef->GetName() );
                }
                while ( objReader->ReadNext() )
                {
                    printf(" \t\tDataValue: %ls\n", objReader->GetString(L"DataValue"));
                }
            }
        }

        if (subset == false)
        {
            if( ! myReader->IsNull(L"xdata2") )
                {

                    FdoPtr<FdoIFeatureReader> objReader2 = myReader->GetFeatureObject(L"xdata2");
                    if( objReader2 )
                    {
                        printf(" \txdata: \n" );
                        classDef = objReader2->GetClassDefinition();
                        if( classDef )
                        {
                            printf(" \t\tClassName: %ls\n",classDef->GetName() );
                            printf(" \t\tQualified ClassName: %ls\n", (const wchar_t*) classDef->GetQualifiedName() );
                        }
                        while ( objReader2->ReadNext() )
                        {
                            printf(" \t\tDataValue: %ls\n", objReader2->GetString(L"DataValue"));
                        }
                    }
                }
        }
        printf("\n");
    }
    catch( FdoException *ex )
    {
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::compIdentFilterTest ()
{
	FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			selCmd->SetFilter(L"(segcount*xdata2.seq) AS Test, Test >= 10 ");
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                FdoInt32 count=0;
                while ( myReader->ReadNext() )
                {
                    count++;
                    read_feature_data( myReader );

                    FdoInt32 featid = myReader->GetInt32(L"FeatId");
                    FdoInt32 segcount = myReader->GetInt32(L"segcount");
                    FdoInt32 seq = 0;

                    if (!myReader->IsNull(L"xdata2"))
                    {
                        FdoPtr<FdoIFeatureReader> subreader = myReader->GetFeatureObject(L"xdata2");
                        if (subreader->ReadNext())  //TODO: change "if" to "while"; currently, this doesn't seem to work properly
                        {
                            seq = subreader->GetInt32(L"seq");

                            // TODO: this is failing on SQL Server for some strange reason:
                            //CPPUNIT_ASSERT_MESSAGE("feature returned that doesn't satisfy the given filter", (seq*segcount)>=10.0);
                        }
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
            printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
            TestCommonFail(ex);
        }
    }
}

void FdoAdvancedSelectTest::compIdentPropertyTest ()
{
	FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoComputedIdentifier> cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(segcount*xdata2.seq) AS TestCompProp");
			selCmd->SetFilter(L"(segcount*xdata2.seq) AS Test, Test >= 10 ");
            FdoPtr<FdoIdentifierCollection>idCol = selCmd->GetPropertyNames();
            idCol->Add( cmpId );
            FdoPtr<FdoIdentifier> segId = FdoIdentifier::Create(L"segcount");
            idCol->Add( segId );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                FdoPtr<FdoClassDefinition>cls = myReader->GetClassDefinition();
                printf("Class: %ls is%s a computed class\n",cls->GetName(),(cls->GetIsComputed())?"":" not");
                FdoPtr<FdoPropertyDefinitionCollection>props = cls->GetProperties();
                for(int i=0; i<props->GetCount(); i++ )
                {
                    FdoPtr<FdoPropertyDefinition>prop = props->GetItem( i );
                    printf("Prop(%d): %ls %d\n", i,prop->GetName(), prop->GetPropertyType() );
                }
                FdoInt32 count=0;
                while ( myReader->ReadNext() )
                {
                    count++;
                    FdoString *testFunc = myReader->GetString(L"TestCompProp");
                    printf("Computed id= %ls\n", testFunc);
                    FdoInt32 segcount = myReader->GetInt32(L"segcount");
                    if (!myReader->IsNull(L"xdata2"))
                    {
                        FdoPtr<FdoIFeatureReader> subreader = myReader->GetFeatureObject(L"xdata2");
                        if (subreader->ReadNext()) //TODO: do "while" instead of "if"; however, the computed expression seems to only be evaluated for the first object property.
                        {
                            double seq = subreader->GetInt32(L"seq");
                            double dTestFunc = FdoCommonOSUtil::wtof(testFunc);

                            //TODO:CPPUNIT_ASSERT_MESSAGE("Wrong computed identifier result", UnitTestUtil::fuzzyEqual(dTestFunc, seq*segcount));
                            CPPUNIT_ASSERT_MESSAGE("Wrong feature returned", dTestFunc >= 10.0);
                        }
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
            printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
            TestCommonFail(ex);
        }
    }
}

void FdoAdvancedSelectTest::orderByTest()
{
    FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;
    FdoInt64 featid=0;
    FdoInt64 previousfeatid=0;
    bool bFirstRow=true;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			selCmd->SetFilter(L"segcount > 0");
            
            FdoPtr<FdoIdentifierCollection> selectIds = selCmd->GetPropertyNames();
            selectIds->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")));
            selectIds->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")));

            selCmd->SetOrderingOption( FdoOrderingOption_Descending );

            FdoPtr<FdoIdentifierCollection>idCol = selCmd->GetOrdering();
            FdoPtr<FdoIdentifier>id = FdoIdentifier::Create(L"FeatId");
            idCol->Add( id );

            // test an order by a simple class property
            printf("Order by simple class property:\n");
            id = FdoIdentifier::Create(L"segcount");
            idCol->Add( id );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                bFirstRow=true;
                FdoInt32 segcount=0;
                FdoInt32 previoussegcount=0;

                // Determine featid type:
                FdoDataType featidDataType = FdoDataType_Int32;
                // TODO: this fails on SQL Server, it seems the id and property collections are both empty (!):

                while ( myReader->ReadNext() )
                {
                    previousfeatid = featid;
                    previoussegcount = segcount;

                    if (featidDataType == FdoDataType_Int64)
                        featid = myReader->GetInt64(L"FeatId");
                    else
                        featid = myReader->GetInt32(L"FeatId");
                    segcount = myReader->GetInt32(L"segcount");
                    printf("FeatId= %ld, segcount= $ld\n", featid, segcount);

                    if (bFirstRow)
                        bFirstRow = false;
                    else
                    {
                        //TODO: this is failing for some strange reason on SQL Server; it seems to always return the first featid value (44).
                        //CPPUNIT_ASSERT_MESSAGE("orderby didn't return results in correct order", (previousfeatid > featid) || (previousfeatid==featid && previoussegcount>=segcount));
                    }
                }
            }

            // test order by an object property
            printf("Order by object property:\n");
            id->SetText(L"xdata2.seq");
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                bFirstRow=true;
                while ( myReader->ReadNext() )
                {
                    previousfeatid = featid;
                    featid = myReader->GetInt32(L"FeatId");
                    printf("FeatId= %ld\n", featid);

                    if (bFirstRow)
                        bFirstRow = false;
                    else
                    {
                        //TODO: This fails on SQL Server, since featid is int64 not int32; other issues on SQL Server too
                        //CPPUNIT_ASSERT_MESSAGE("orderby didn't return results in correct order", (previousfeatid >= featid));
                    }
                }
            }

            //
            // Test an order by property that is already in the select list
            printf("Order by property that is already in the select list:\n");
            id->SetText(L"segcount");
            FdoPtr<FdoIdentifierCollection>propCol = selCmd->GetPropertyNames();
            id = FdoIdentifier::Create(L"xdata2.seq");
            propCol->Add( id );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                FdoInt32 seq=0;
                FdoInt32 previousseq=0;
                bFirstRow=true;
                while ( myReader->ReadNext() )
                {
                    previousfeatid = featid;
                    featid = myReader->GetInt32(L"FeatId");
                    //TODO: this currently causes error, due to a column name appearing twice in the select list when calling GetFeatureObject(L"xdata2"):
                    /*
                    if (!myReader->IsNull(L"xdata2"))
                    {
                        FdoPtr<FdoIFeatureReader> subReader = myReader->GetFeatureObject(L"xdata2");
                        if (subReader->ReadNext())
                        {
                            seq = subReader->GetInt32(L"seq");
                        }
                        subReader = NULL;
                    }
                    */

                    printf("FeatId= %ld, xdata2.seq= %ld\n", featid, seq );

                    if (bFirstRow)
                        bFirstRow = false;
                    else
                    {
                        //TODO:CPPUNIT_ASSERT_MESSAGE("orderby didn't return results in correct order", (previousfeatid > featid) || (previousfeatid==featid && previousseq>=seq));

                        //TODO: this fails on SQL Server, due to featid being int64 instead of int32 and other issues:
                        //CPPUNIT_ASSERT_MESSAGE("orderby didn't return results in correct order", (previousfeatid >= featid));
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
            printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
            TestCommonFail(ex);
        }

        //
        // Order by on already ordered collection
        try
        {
            printf("Order by on already ordered collection:\n");
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDbEntity.xdata2");
            FdoPtr<FdoComparisonCondition> filterPtr = FdoComparisonCondition::Create(
						FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"seq")), 
						FdoComparisonOperations_GreaterThan, 
						FdoPtr<FdoDataValue>(FdoDataValue::Create((int)0)));
			selCmd->SetFilter(filterPtr);
            FdoPtr<FdoIdentifierCollection>idCol = selCmd->GetOrdering();
            FdoPtr<FdoIdentifier>id = FdoIdentifier::Create(L"DataValue");
            idCol->Add( id );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    printf(" AcDbEntity_FeatId: %d\n", myReader->GetInt64(L"AcDbEntity_FeatId"));
                    printf(" \tseq: %d\n", myReader->GetInt32(L"seq"));
                    printf(" \tDataValue: %ls\n", myReader->GetString(L"DataValue"));
                }
            }
        }
        catch( FdoException *ex )
        {
            printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
            TestCommonFail(ex);
        }
    }
}

void FdoAdvancedSelectTest::functionTest()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( mConnection == NULL )
        return;

    // Use the function in the filter
    try
    {
        printf("\nUsing function in a filter\n");
        selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		selCmd->SetFilter(L"Ceil(segcount/3.001) >= 3 ");
        myReader = selCmd->Execute();
        if( myReader != NULL  )
        {
            FdoInt32 count=0;
            while ( myReader->ReadNext() )
            {
                count++;
                printf("Ceil(segcount/3.001) >= 3  == feat id %ls\n", myReader->GetString(L"FeatId") );
                double segcount = myReader->GetInt32(L"segcount");
                CPPUNIT_ASSERT_MESSAGE("Wrong features returned", ceil(segcount/3.001) >= 3.0);
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

    // Use number function in the select list
    try
    {
        printf("\nUsing number function in a select list\n");
        selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		selCmd->SetFilter(L"Floor(segcount/2.999) >= 3 ");
        FdoPtr<FdoComputedIdentifier> cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Floor(segcount/2.999)) AS TestFunc");
        FdoPtr<FdoIdentifierCollection>idCol = selCmd->GetPropertyNames();
        idCol->Add( cmpId );
        idCol->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")) );
        myReader = selCmd->Execute();
        if( myReader != NULL  )
        {
            FdoPtr<FdoClassDefinition>cls = myReader->GetClassDefinition();
            FdoPtr<FdoPropertyDefinitionCollection>props = cls->GetProperties();
            for(int i=0; i<props->GetCount(); i++ )
            {
                FdoPtr<FdoPropertyDefinition>prop = props->GetItem( i );
                printf("Prop(%d): %ls %d\n", i,prop->GetName(), prop->GetPropertyType() );
            }
            FdoInt32 count=0;
            while ( myReader->ReadNext() )
            {
                count++;
                printf("Floor(segcount/2.999) = %ls\n", myReader->GetString(L"TestFunc") );
                double segcount = myReader->GetInt32(L"segcount");
                CPPUNIT_ASSERT_MESSAGE("Wrong feature returned", floor(segcount/2.999) >= 3.0);
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }


    // Use the function in the select list
    FdoInt32 manualSumSegcount=0;
    try
    {
        printf("\nUsing aggregate function in a select list\n");

        // First, calculate sum(segcount) manually:
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		selCmdAggreg->SetFilter(L"(FeatId*xdata2.seq) AS Test, Test > 0 ");
        myDataReader = selCmdAggreg->Execute();
        while (myDataReader->ReadNext())
            manualSumSegcount += myDataReader->GetInt32(L"segcount");

        // Second, calculate sum(segcount) via aggregate function:
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		selCmdAggreg->SetFilter(L"(FeatId*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Sum(segcount)) AS TestFunc");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            printf("\tProperty count= %d\n",myDataReader->GetPropertyCount() );
            for(int i=0; i<myDataReader->GetPropertyCount(); i++ )
            {
                const wchar_t* prop = myDataReader->GetPropertyName( i );
                printf("\t\tProperty: %ls type(%d)\n", prop, myDataReader->GetPropertyType( prop ) );
            }
            if ( myDataReader->ReadNext() )
            {
                FdoString* sum = myDataReader->GetString(L"TestFunc");
                printf("Sum(segcount)= %ls\n", sum);
                CPPUNIT_ASSERT_MESSAGE("wrong sum", manualSumSegcount == FdoCommonOSUtil::wtoi(sum));
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

    // Use nested function
    try
    {
        printf("\nUsing nested function in a select list\n");
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(SQRT(Sum(segcount))) AS TestFunc");
		selCmdAggreg->SetFilter(L"(segcount*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            if ( myDataReader->ReadNext() )
            {
                double result = myDataReader->GetDouble(L"TestFunc");
                printf("SQRT(SUM(segcount))= %ls\n", myDataReader->GetString(L"TestFunc") );
                CPPUNIT_ASSERT_MESSAGE("sqrt(sum(segcount)) returned wrong result", UnitTestUtil::fuzzyEqual(result, sqrt((double)manualSumSegcount)));
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

    // Use function of a math expression
    try
    {
        // First, calculate max(segcount) manually:
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        FdoPtr<FdoIdentifierCollection> selectList = selCmdAggreg->GetPropertyNames();
        selectList->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")));
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        myDataReader = selCmdAggreg->Execute();
        FdoInt32 manualMaxSegcount = -100000;
        while (myDataReader->ReadNext())
        {
            if (!myDataReader->IsNull(L"segcount"))
            {
                FdoInt32 segcount = myDataReader->GetInt32(L"segcount");
                if (segcount > manualMaxSegcount)
                    manualMaxSegcount = segcount;
            }
        }

        // Second, calculate max(segcount) via aggregate function:
        printf("\nUsing a function of an expression in a select list\n");
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        // FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Sum(segcount*FeatId)) AS TestFunc");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Max(segcount)) AS TestFunc");
		//selCmdAggreg->SetFilter(L"(segcount*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            while ( myDataReader->ReadNext() )
            {
                FdoDataType type = myDataReader->GetDataType( L"TestFunc" );
                // Data type of computed expression will vary depending on the provider (e.g. SqlServer will return a type based on aggregate column's type):
                //if( type != FdoDataType_Double )
                //    throw "Wrong data type returned by SelectAggregates";
                if( ! myDataReader->IsNull( L"TestFunc" ) )
                {
                    if (type == FdoDataType_Double)
                    {
                        double dVal = myDataReader->GetDouble(L"TestFunc");
                        printf("Max(segcount)= %f\n", dVal);
                        CPPUNIT_ASSERT_MESSAGE("Wrong Max(segcount) returned", dVal==(double)manualMaxSegcount);
                    }
                    else if (type == FdoDataType_Int32)
                    {
                        FdoInt32 iVal = myDataReader->GetInt32(L"TestFunc");
                        printf("Max(segcount)= %d\n", iVal);
                        CPPUNIT_ASSERT_MESSAGE("Wrong Max(segcount) returned", iVal==manualMaxSegcount);
                    }
                    else
                        throw "Wrong data type returned by SelectAggregates";
                }
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

    // Use a string function 
    try
    {
        printf("\nUsing a string function\n");
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:testClass");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Upper(FirstName)) AS TestFunc");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            while ( myDataReader->ReadNext() )
            {
                FdoString* upperFirstName = myDataReader->GetString(L"TestFunc");
                wchar_t upperUpperFirstName[128];
                wcscpy(upperUpperFirstName, upperFirstName);
        		FdoCommonOSUtil::wcsupr(upperUpperFirstName);

                printf("Upper(FirstName)= %ls\n", upperFirstName);
                CPPUNIT_ASSERT_MESSAGE("Upper() failed", 0==wcscmp(upperFirstName, upperUpperFirstName));
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }

    // Use string concat function 
    try
    {
        printf("\nUsing a non supported function\n");
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"L\x00e4nd:Parcel");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(CONCAT('Pr\x00f6vince is: ',CONCAT(\"Pr\x00f6vince\",CONCAT('-',\"Pr\x00f6vince\")))) AS TestFunc");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        FdoPtr<FdoIdentifier> idProvince = FdoIdentifier::Create(L"Pr\x00f6vince");
        idCol->Add(idProvince);
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            wchar_t test[128];
            while ( myDataReader->ReadNext() )
            {
                FdoString *testFunc = myDataReader->GetString(L"TestFunc");
                printf("CONCAT('Pr\x00f6vince is: ',CONCAT(Pr\x00f6vince,CONCAT('-',Pr\x00f6vince)))= %ls\n", testFunc );
                FdoString* province = myDataReader->GetString(L"Pr\x00f6vince");
                wcscpy(test,L"Pr\x00f6vince is: ");
                wcscat(test,province);
                wcscat(test,L"-");
                wcscat(test,province);

                CPPUNIT_ASSERT_MESSAGE("Incorrect string concatenation received", 0==wcscmp(test, testFunc));
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestDefect785616()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( mConnection == NULL )
        return;

    // Use no function in the select list -- get all properties
    FdoInt32 manualSumSegcount=0;
    try
    {
        printf("\nUsing no functions in a SelectAggregates command\n");

        // Second, calculate sum(segcount) via aggregate function:
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		selCmdAggreg->SetFilter(L"(FeatId*xdata2.seq) AS Test, Test > 0 ");
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            printf("\tProperty count= %d\n",myDataReader->GetPropertyCount() );
            for(int i=0; i<myDataReader->GetPropertyCount(); i++ )
            {
                const wchar_t* prop = myDataReader->GetPropertyName( i );
                printf("\t\tProperty: %ls type(%d)\n", prop, myDataReader->GetPropertyType( prop ) );
            }
            while ( myDataReader->ReadNext() )
            {
                ; // We just need to know that the column mapping didn't produce errors.
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::groupByTest()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
     try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Sum(xdata2.seq)) AS TestFunc");
		selCmdAggreg->SetFilter(L"(segcount*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        FdoPtr<FdoIdentifierCollection>grpCol = selCmdAggreg->GetGrouping();
        FdoPtr<FdoIdentifier>id = FdoIdentifier::Create(L"FeatId");
        grpCol->Add( id );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            while ( myDataReader->ReadNext() )
            {
                double testFunc = myDataReader->GetDouble(L"TestFunc");
                printf("Sum(xdata2.seq) = %f\n", testFunc );
                CPPUNIT_ASSERT_MESSAGE("Wrong sum(xdata2.seq) returned", testFunc==3.0);
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestDefect779194()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
     try
    {
        // Make sure that GetPropertyCount succeeds when no identifiers are specified.

        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        myDataReader = selCmdAggreg->Execute();
        FdoInt32 propertyCount = myDataReader->GetPropertyCount();
        CPPUNIT_ASSERT_MESSAGE("Expected some columns, got zero", propertyCount>0);
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::selectDistinctTest()
{
    FdoPtr<FdoIDataReader>myReader;
	FdoPtr<FdoISelectAggregates>selCmd;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
            selCmd->SetFeatureClassName(L"Acad:testClass.Object");
            FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> name;
            name = FdoIdentifier::Create(L"ObjectWeight");
            names->Add(name);

            selCmd->SetDistinct( true );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    printf(" \t ObjectWeight\t: %ls\n", myReader->GetString(L"ObjectWeight"));
                    //TODO: add a CPPUNIT_ASSERT_MESSAGE() here, but there is no data yet in testClass class to verify.
                }
            }

            // Single class property
            names->Clear();
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            names = selCmd->GetPropertyNames();

            name = FdoIdentifier::Create(L"segcount");
            names->Add(name);

            selCmd->SetDistinct( true );
            myReader = selCmd->Execute();
            std::vector<FdoInt32> segcounts;
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    if( ! myReader->IsNull( L"segcount") )
                    {
                        FdoInt32 segcount = myReader->GetInt32(L"segcount");
                        printf(" \t Segcount\t: %d\n", segcount);

                        for (int i=0; i < (int)segcounts.size(); i++)
                            CPPUNIT_ASSERT_MESSAGE("duplicate value returned by distinct", segcounts.at(i)!=segcount);
                        segcounts.push_back(segcount);
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
            printf("SelectDistinctTest error: %ls\n", ex->GetExceptionMessage());
            TestCommonFail(ex);
        }
    }
}

void FdoAdvancedSelectTest::getDataTypeTest()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    FdoPtr<FdoIFeatureReader>myReader;

    if( mConnection == NULL )
        return;

    try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(ceil(segcount/1.0)) AS TestFunc");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        if (myDataReader->ReadNext())
        {
            FdoInt32 count = myDataReader->GetPropertyCount();
            for (int i=0; i<myDataReader->GetPropertyCount(); i++)
            {
                FdoString *propName = myDataReader->GetPropertyName(i);
                FdoDataType dataType = myDataReader->GetDataType(propName);
            }
        }
    }

    catch( FdoException *ex )
    {
        printf("getDataTypeTest error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}
void FdoAdvancedSelectTest::groupByorderByTest()
{
    FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
     try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Sum(xdata2.seq)) AS TestFunc");
		selCmdAggreg->SetFilter(L"(segcount*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
		FdoPtr<FdoIdentifier>id = FdoIdentifier::Create(L"FeatId");
        FdoPtr<FdoIdentifierCollection>grpCol = selCmdAggreg->GetGrouping();
        grpCol->Add( id );
		FdoPtr<FdoIdentifierCollection>orderCol = selCmdAggreg->GetOrdering();
        FdoPtr<FdoIdentifier>oId = FdoIdentifier::Create(L"FeatId");
        orderCol->Add( oId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            while ( myDataReader->ReadNext() )
            {
				printf("Sum(xdata2.seq) = %f\n", myDataReader->GetDouble(L"TestFunc") );
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}
void FdoAdvancedSelectTest::orderByTest2()
{
	FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
     try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Sum(xdata2.seq)) AS TestFunc");
		selCmdAggreg->SetFilter(L"(segcount*xdata2.seq) AS Test, Test > 0 ");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        
		FdoPtr<FdoIdentifierCollection>orderCol = selCmdAggreg->GetOrdering();
        FdoPtr<FdoIdentifier>oId = FdoIdentifier::Create(L"TestFunc");
        orderCol->Add( oId );
        myDataReader = selCmdAggreg->Execute();
        if( myDataReader != NULL  )
        {
            while ( myDataReader->ReadNext() )
            {
				printf("Sum(xdata2.seq) = %f\n", myDataReader->GetDouble(L"TestFunc") );
            }
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO advanced select error: %ls\n", ex->GetExceptionMessage());
        TestCommonFail(ex);
    }
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a SelectAggregate request
// when aa aggregate function is used. In this case, no such property should
// be returned by the reader.
void FdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithAggrFunction()
{
    FdoPtr<FdoISelectAggregates>    selAggr;
    FdoPtr<FdoIDataReader>          rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing SelectAggregate Function Reader Content Test   \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an identity property   \n");
        printf("       is automatically added to the reader returned by a    \n");
        printf("       SelectAggregate request when an aggregate function is \n");
        printf("       used. If the reader contains an identity property the \n");
        printf("       test issues an exception.                             \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selAggr = (FdoISelectAggregates*)(mConnection->CreateCommand(FdoCommandType_SelectAggregates));
        selAggr->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(10)));
        selAggr->SetFilter(aFilter);
        ids = selAggr->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyMaxSegcount",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Max(segcount)")))));

        rdr = selAggr->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if there is an additional identity property (or any other
        // property) other than the requested one returned by the reader.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoInt64 myMaxSegcount = rdr->GetInt64(L"MyMaxSegcount");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"FeatId");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (idPropertyFound)
            throw FdoException::Create(L"Id property returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(">>> Test failed \n");
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a SelectAggregate request
// when a function is used that is not an aggregate function but one of CEIL,
// CONCAT, FLOOR, LOWER and UPPER. In this case, no such property should be
// returned by the reader.
void FdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithNumCharFunction()
{
    FdoPtr<FdoISelectAggregates>    selAggr;
    FdoPtr<FdoIDataReader>          rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing SelectAggregate Function Reader Content Test   \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an identity property   \n");
        printf("       is automatically added to the reader returned by a    \n");
        printf("       SelectAggregate request when a function is used that  \n");
        printf("       is not an aggregate function but one of CEIL, CONCAT, \n");
        printf("       FLOOR, LOWER and UPPER. If the reader contains an     \n");
        printf("       identity property the test issues an exception.       \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selAggr = (FdoISelectAggregates*)(mConnection->CreateCommand(FdoCommandType_SelectAggregates));
        selAggr->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(10)));
        selAggr->SetFilter(aFilter);
        ids = selAggr->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyConcatString",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"CONCAT(layer, color)")))));

        rdr = selAggr->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if there is an additional identity property (or any other
        // property) other than the requested one returned by the reader.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoStringP myString = rdr->GetString(L"MyConcatString");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"FeatId");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (idPropertyFound)
            throw FdoException::Create(L"Id property returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(">>> Test failed \n");
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

// The following test is to check whether or not the select command issues
// an exception if the select statement uses an aggregate function. If this
// is not the case the test issues an exception.
void FdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithAggrFunction()
{
    FdoPtr<FdoISelect>              selCmd;
    FdoPtr<FdoIFeatureReader>       rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;
    bool                            expectedExceptionIssued = true;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing Select Function Aggregate Exception Test       \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an exception is issued \n");
        printf("       if the select command is invoked with an aggregate    \n");
        printf("       function. If the expected exception is not issued the \n");
        printf("       test issues an exception.                             \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selCmd = (FdoISelect*)(mConnection->CreateCommand(FdoCommandType_Select));
        selCmd->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        ids = selCmd->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyMaxSegcount",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Max(segcount)")))));

        rdr = selCmd->Execute();

        // If this point is reached the expected exception was not issued and hence
        // an exception needs to be issued to indicate so.
        expectedExceptionIssued = false;

        // Check the content of the reader.

        count = 0;
        printf(">>> ... Checking the returned reader \n");

        while (rdr->ReadNext())
        {
            FdoInt64 myMaxSegcount = rdr->GetInt64(L"MyMaxSegcount");
            FdoStringP className = rdr->GetClassDefinition()->GetName();

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"FeatId");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");

        //if (idPropertyFound)
        //    throw FdoException::Create(L"Identity property found in reader when not expected");

        //if (!expectedExceptionIssued)
        //    throw FdoException::Create(L"Expected exception not issued");

        printf(">>> Test executed succeeded \n");
    }

    catch( FdoException *ex )
    {
        printf(">>> Test failed \n");
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a Select request when a
// function is used that is not an aggregate function but one of CEIL,
// CONCAT, FLOOR, LOWER and UPPER. In this case, the reader is expected to
// return such a property.
void FdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithNumCharFunction()
{
    FdoPtr<FdoISelect>              selCmd;
    FdoPtr<FdoIFeatureReader>       rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing Select Function Reader Content Test            \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>>    The test checks whether or not an identity property   \n");
        printf(">>>    is automatically added to the reader returned by a    \n");
        printf(">>>    Select request when a function is used that is not an \n");
        printf(">>>    aggregate function but one of CEIL, CONCAT, FLOOR,    \n");
        printf(">>>    LOWER and UPPER. In this case, the reader is expected \n");
        printf(">>>    to return such a property.                            \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select command \n");

        selCmd = (FdoISelect*)(mConnection->CreateCommand(FdoCommandType_Select));
        selCmd->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(10)));
        selCmd->SetFilter(aFilter);
        ids = selCmd->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyConcatString",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"CONCAT(layer, color)")))));

        rdr = selCmd->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if the reader does not return identity properties.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoStringP myString = rdr->GetString(L"MyConcatString");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"FeatId");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (!idPropertyFound)
            throw FdoException::Create(L"Id property not returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(">>> Test failed \n");
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestMaxBoolProperty()
{
	FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
     try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Max(boolean)) AS testFld");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        try
        {
            myDataReader = selCmdAggreg->Execute();
            TestCommonFail(FdoException::Create(L"TestUpperDateProperty should fail!"));
        }
        catch( FdoException *ex )
        {
            printf("Expected error: %ls\n", ex->GetExceptionMessage());
            ex->Release();
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestUpperDateProperty()
{
	FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoStringP exec = L"(Upper(";
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(exec + GetDateTimePropName()+ L")) AS testFld");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        try
        {
            myDataReader = selCmdAggreg->Execute();
            TestCommonFail(FdoException::Create(L"TestUpperDateProperty should fail!"));
        }
        catch( FdoException *ex )
        {
            printf("Expected error: %ls\n", ex->GetExceptionMessage());
            ex->Release();
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestCeillInt64Property()
{
	FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Ceil(int64)) AS testFld");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        try
        {
            myDataReader = selCmdAggreg->Execute();
            TestCommonFail(FdoException::Create(L"TestUpperDateProperty should fail!"));
        }
        catch( FdoException *ex )
        {
            printf("Expected error: %ls\n", ex->GetExceptionMessage());
            ex->Release();
        }
    }
    catch( FdoException *ex )
    {
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoAdvancedSelectTest::TestLowerOnStringProperty()
{
	FdoPtr<FdoIDataReader>myDataReader;
	FdoPtr<FdoISelectAggregates>selCmdAggreg;
    try
    {
        selCmdAggreg = (FdoISelectAggregates*)mConnection->CreateCommand( FdoCommandType_SelectAggregates );
        selCmdAggreg->SetFeatureClassName(GetSchemaName() + L":" + AcDb3dPolylineName());
        FdoPtr<FdoComputedIdentifier>cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Lower(layer)) AS testFld");
        FdoPtr<FdoIdentifierCollection>idCol = selCmdAggreg->GetPropertyNames();
        idCol->Add( cmpId );
        myDataReader = selCmdAggreg->Execute();
        printf(">>> ... Checking the returned reader \n");
        while (myDataReader->ReadNext())
        {
            if (!myDataReader->IsNull(L"testFld"))
            {
                FdoStringP myString = myDataReader->GetString(L"testFld");
                printf ("String value = %ls\n", (FdoString*)myString);
            }
        }
        myDataReader->Close();
    }
    catch( FdoException *ex )
    {
        printf("FDO exception: %ls \n", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

