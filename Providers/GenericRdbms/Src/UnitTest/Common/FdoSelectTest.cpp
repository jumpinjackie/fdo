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
#include "FdoSelectTest.h"
#include "UnitTestUtil.h"
//#include <Geometry/GeometryStd.h>
//#include <Geometry/Fgf/Factory.h>
//#include "FdoGeometry.h"
#ifdef _DEBUG
#define  DBG(X)   // X
#else
#define  DBG(X) 
#endif

FdoIConnection* FdoSelectTest::mConnection = NULL;

FdoSelectTest::FdoSelectTest(void)
{
    mSuffix[0] = '\0';
}

FdoSelectTest::FdoSelectTest(char *suffix)
{
    strncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

FdoSelectTest::~FdoSelectTest(void)
{
    if( mConnection != NULL )
    {
        mConnection->Close();
        mConnection->Release();
        mConnection = NULL;
    }
}

void FdoSelectTest::setUp ()
{
    set_provider();
    connect();
}

void FdoSelectTest::tearDown ()
{
    if( mConnection != NULL )
    {
        mConnection->Close();
        mConnection->Release();
        mConnection = NULL;
    }
}

void FdoSelectTest::connect ()
{
    try
    {
        mConnection = (FdoIConnection *) UnitTestUtil::GetConnection(mSuffix, true);

    }
    catch (FdoException *ex)
    {
        ( printf("FDO error: %ls\n", ex->GetExceptionMessage()) );
        if( mConnection != NULL )
        {
            mConnection->Close();
            mConnection->Release();
            mConnection= NULL;
        }
    ex->Release();
        throw;
    }
}

void FdoSelectTest::read_feature_data( FdoIFeatureReader *myReader, bool subset )
{
    try
    {
        FdoPtr<FdoClassDefinition>classDefinition = myReader->GetClassDefinition();
        DBG( printf("New feature: class name( %ls ) Feature id ( %d ) \n", classDefinition->GetName(), myReader->GetInt64(L"FeatId") ));
        DBG( printf("New feature: Feature id ( %d ) - Reader Depth=%d\n", myReader->GetInt64(L"FeatId"), myReader->GetDepth() ) );

        myReader->GetInt64(L"FeatId");
        myReader->GetDepth();
        FdoClassDefinition *classDef = myReader->GetClassDefinition();
        classDef->Release();
        classDef = myReader->GetClassDefinition();
        if( classDef )
        {
            DBG( printf(" \tClassName: %ls\n\n Id Properties: \n",classDef->GetName() ));
            classDef->GetName();
            FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = classDef->GetIdentityProperties();
            for (int i=0; i<idProperties->GetCount(); i++)
            {
                FdoPtr<FdoDataPropertyDefinition> idProperty = idProperties->GetItem(i);
                DBG( printf("\t%d) %ls\n", i+1, idProperty->GetName()) );
                idProperty->GetName();
            }
            DBG( printf("\n Base Properties: \n") );
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = classDef->GetBaseProperties();
            for (int i=0; i<baseProperties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> baseProperty = static_cast<FdoPropertyDefinition *> (baseProperties->GetItem(i));
                DBG( printf("\t%d) %ls\n", i+1, baseProperty->GetName()) );
                baseProperty->GetName();
            }
            DBG( printf("\n Properties: \n") );
            FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
            for (int i=0; i<properties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> property = properties->GetItem(i);
                DBG( printf("\t%d) %ls\n\n", i+1, property->GetName()) );
                property->GetName();
            }

            classDef->Release();
        }
        if( ! myReader->IsNull(L"RevisionNumber") )
        {
            DBG( printf(" \tRevisionNumber: %d\n", myReader->GetInt64(L"RevisionNumber") ) );
            myReader->GetInt64(L"RevisionNumber");
        }


        if( ! myReader->IsNull(L"color") )
        {
            DBG( printf(" \tColor: %ls\n", myReader->GetString(L"color") ) );
            myReader->GetString(L"color");
        }
        if( !subset && ! myReader->IsNull(L"Geometry") )
        {
            FdoByteArray* byteArray = myReader->GetGeometry(L"Geometry");
            DBG( printf(" \tGemetry byte array size: %d\n", byteArray->GetCount()) );
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(byteArray);
            //printf(" \tGeometry Text: <<%ls>>\n", geom->GetText());
            byteArray->GetCount();
            byteArray->Release();
        }

        if (subset == false)
            if( ! myReader->IsNull(L"layer") )
            {
                DBG( printf(" \tLayer: %ls\n", myReader->GetString(L"layer") ) );
                myReader->GetString(L"layer");
            }

        if (subset == false)
            if( ! myReader->IsNull(L"segcount") )
            {
                DBG( printf(" \tSegCount: %d\n", myReader->GetInt32(L"segcount")) );
                myReader->GetInt32(L"segcount");
            }
       if( ! myReader->IsNull(L"xdata") )
        {

            FdoIFeatureReader   *objReader = myReader->GetFeatureObject(L"xdata");
            if( objReader )
            {
                DBG( printf(" \txdata - depth(%d): \n",  objReader->GetDepth() ) );
                objReader->GetDepth();
                classDef = objReader->GetClassDefinition();
                if( classDef )
                {
                    DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                    classDef->GetName();
                    classDef->Release();
                }
                while ( objReader->ReadNext() )
                {
                    DBG( printf(" \t\tDataValue: %ls\n", objReader->GetString(L"DataValue")) );
                    objReader->GetString(L"DataValue");
                }
                objReader->Release();
            }
        }

        if (subset == false)
        {
            if( ! myReader->IsNull(L"xdata2") )
                {

                    FdoIFeatureReader   *objReader2 = myReader->GetFeatureObject(L"xdata2");
                    if( objReader2 )
                    {
                        DBG( printf(" \txdata: \n" ) );
                        classDef = objReader2->GetClassDefinition();
                        if( classDef )
                        {
                            DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                            classDef->GetName();
                            DBG( printf(" \t\tQualified ClassName: %ls\n", (const wchar_t*) classDef->GetQualifiedName() ) );
                            classDef->GetQualifiedName();
                            classDef->Release();
                        }
                        while ( objReader2->ReadNext() )
                        {
                            DBG( printf(" \t\tDataValue: %ls\n", objReader2->GetString(L"DataValue")) );
                            objReader2->GetString(L"DataValue");
                        }
                    }
                    objReader2->Release();
                }
        }

        DBG( printf("\n") );
    }
    catch( FdoException *ex )
    {
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
        throw;
    }
}

FdoFilter* FdoSelectTest::CreateFilter()
{

    FdoComparisonCondition* filter = FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"segcount")),
                        FdoComparisonOperations_GreaterThan,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)0)));

    return filter;
}

void FdoSelectTest::feature_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;
	clock_t start, finish;

    start = clock();
    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoFilter* filterPtr = FdoSelectTest::CreateFilter();
            selCmd->SetFilter(filterPtr);

            filterPtr->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data( myReader );

                    FdoClassDefinitionP classDef = myReader->GetClassDefinition();
                    FdoPropertiesP props = classDef->GetProperties();
                    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

                    CPPUNIT_ASSERT( props->GetCount() == 1 );
                    CPPUNIT_ASSERT( baseProps->GetCount() == 17 );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }
	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}

void FdoSelectTest::feature_object_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDbEntity.xdata2");
            FdoComparisonCondition* filterPtr = FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"seq")),
                        FdoComparisonOperations_GreaterThan,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)0)));
            //FdoFilter* filterPtr = FdoSelectTest::CreateFilter();
            selCmd->SetFilter(filterPtr);
            filterPtr->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    DBG( printf(" AcDbEntity_FeatId: %d\n", myReader->GetInt64(L"AcDbEntity_FeatId")) );
                    myReader->GetInt64(L"AcDbEntity_FeatId");
                    DBG( printf(" \tseq: %d\n", myReader->GetInt32(L"seq")) );
                    myReader->GetInt32(L"seq");
                    DBG( printf(" \tDataValue: %ls\n", myReader->GetString(L"DataValue")) );
                    myReader->GetString(L"DataValue");
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }
}



void FdoSelectTest::read_non_feature_data( FdoIFeatureReader *myReader, bool subset )
{
    try
    {
        FdoClassDefinition *classDef = myReader->GetClassDefinition();
        if( classDef )
        {
            DBG( printf(" \tClassName: %ls\n",classDef->GetName() ) );
            classDef->GetName();
            DBG( printf("\n Id Properties: \n") );
            FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = classDef->GetIdentityProperties();
            for (int i=0; i<idProperties->GetCount(); i++)
            {
                FdoPtr<FdoDataPropertyDefinition> idProperty = idProperties->GetItem(i);
                DBG( printf("\t%d) %ls\n", i+1, idProperty->GetName()) );
                idProperty->GetName();
            }
            DBG( printf("\n Base Properties: \n") );
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = classDef->GetBaseProperties();
            for (int i=0; i<baseProperties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> baseProperty = static_cast<FdoPropertyDefinition *> (baseProperties->GetItem(i));
                DBG( printf("\t%d) %ls\n", i+1, baseProperty->GetName()) );
                baseProperty->GetName();
            }
            DBG( printf("\n Properties: \n") );
            FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
            for (int i=0; i<properties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> property = properties->GetItem(i);
                DBG( printf("\t%d) %ls\n", i+1, property->GetName()) );
                property->GetName();
            }
            DBG( printf("\n") );

            classDef->Release();
        }
        if (subset == false)
        {
            if( ! myReader->IsNull(L"FirstName") )
            {
                DBG( printf(" \tFirst Name: %ls\n", myReader->GetString(L"FirstName") ) );
                myReader->GetString(L"FirstName");
            }
            if( ! myReader->IsNull(L"LastName") )
            {
                DBG( printf(" \tLast Name: %ls\n",  myReader->GetString(L"LastName") ) );
                myReader->GetString(L"LastName");
            }
        }
        if( ! myReader->IsNull(L"Age") )
        {
            DBG( printf(" \tAge: %d\n", myReader->GetInt32(L"Age")) );
            myReader->GetInt32(L"Age");
        }

        if( ! myReader->IsNull(L"Object") )
        {

            FdoIFeatureReader   *objReader = myReader->GetFeatureObject(L"Object");
            if( objReader )
            {
                DBG( printf(" \txdata: \n" ) );
                classDef = objReader->GetClassDefinition();
                if( classDef )
                {
                    DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                    classDef->GetName();
                    classDef->Release();
                }
                while ( objReader->ReadNext() )
                {
                    DBG( printf(" \t\tWeight: %d\n", objReader->GetInt32(L"ObjectWeight")));
                    objReader->GetInt32(L"ObjectWeight");
                }
            }
            objReader->Release();
        }

        DBG( printf("\n") );
    }
    catch( FdoException *ex )
    {
        DBG( printf("FDO exception: %ls \n", ex->GetExceptionMessage() ) );
        throw ex;
    }
}

void FdoSelectTest::non_feature_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:testClass");

            FdoFilter* filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((int)200 ) ) );

            selCmd->SetFilter(filter);
            filter->Release();

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_non_feature_data( myReader );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            throw ex;
        }
    }
}

void FdoSelectTest::distance_query()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoFilter* filterPtr = FdoSelectTest::CreateFilter();
            double coords[6];
            double      distance = 300.0;
            coords[0] = 1410804.854546;
            coords[1] = 553505.994735;
            coords[2] = 0.0;
            coords[3] = 1412350.770162;
            coords[4] = 554980.139631;
            coords[5] = 0.0;
            FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();
            FdoILineString* line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coords);
            FdoByteArray *byteArray = gf->GetFgf(line1);
            gf->Release();
            line1->Release();

            FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>( FdoGeometryValue::Create(byteArray));
            byteArray->Release();
            FdoPtr<FdoDistanceCondition> pSpatialFilter = FdoPtr<FdoDistanceCondition>( FdoDistanceCondition::Create(L"Geometry",
                                                                                FdoDistanceOperations_Within,
                                                                                geomValue,
                                                                                distance ));
            FdoFilter *filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
            selCmd->SetFilter(filter);

            filterPtr->Release();
            filter->Release();

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data( myReader );
                }
                myReader->Release();
            }
            selCmd->Release();

        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            throw ex;
        }
    }
}
void FdoSelectTest::spatial_query()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoFilter* filterPtr = FdoSelectTest::CreateFilter();
            double ordsXYExt[15];
            ordsXYExt[0] = 1412249.54; ordsXYExt[1] = 554576.84; ordsXYExt[2] = 0.0;
            ordsXYExt[3] = 1412249.54; ordsXYExt[4] = 554516.33;ordsXYExt[5] = 0.0;
            ordsXYExt[6] = 1412146.90; ordsXYExt[7] = 554516.33;ordsXYExt[8] = 0.0;
            ordsXYExt[9] = 1412146.90; ordsXYExt[10] = 554576.84;ordsXYExt[11] = 0.0;
            ordsXYExt[12] = 1412249.54; ordsXYExt[13] = 554576.84;  ordsXYExt[14] = 0.0;
            FdoILinearRing* extRing = gf->CreateLinearRing(FdoDimensionality_XY|FdoDimensionality_Z, 15, ordsXYExt);
            FdoIPolygon* poly = gf->CreatePolygon(extRing, NULL );
            FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(poly))));
            FdoPtr<FdoSpatialCondition> pSpatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(L"Geometry",
                                                                      FdoSpatialOperations_Intersects,
                                                                      geomValue));

            FdoFilter *filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
            selCmd->SetFilter(filter);

            poly->Release();
            filterPtr->Release();
            filter->Release();
            extRing->Release();
            gf->Release();

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data( myReader );
                }
                myReader->Release();
            }
            selCmd->Release();

        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            throw ex;
        }
    }
}

void FdoSelectTest::feature_subset_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> name;

            name = FdoIdentifier::Create(L"xdata.DataValue");
            names->Add(name);
            name = FdoIdentifier::Create(L"xdata.seq");
            names->Add(name);

            name = FdoIdentifier::Create(L"color");
            names->Add(name);
            name = FdoIdentifier::Create(L"RevisionNumber");
            names->Add(name);
//            name = FdoIdentifier::Create(L"FeatId");
//            names->Add(name);

            FdoFilter* filterPtr = FdoSelectTest::CreateFilter();

            selCmd->SetFilter(filterPtr);
            filterPtr->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {

                    read_feature_data( myReader, true );

                    FdoClassDefinitionP classDef = myReader->GetClassDefinition();

                    FdoPropertiesP props = classDef->GetProperties();
                    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

                    CPPUNIT_ASSERT( props->GetCount() == 0 );
                    CPPUNIT_ASSERT( baseProps->GetCount() == 3 );
                    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(baseProps,L"color") );
                    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(baseProps,L"xdata") );
                    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(baseProps,L"RevisionNumber") );
                    CPPUNIT_ASSERT( !UnitTestUtil::ContainsRdOnlyProp(baseProps,L"layer") );
                    CPPUNIT_ASSERT( !UnitTestUtil::ContainsRdOnlyProp(baseProps,L"ClassId") );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            throw ex;
        }
    }
}

void FdoSelectTest::feature_subset_query2 ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:NotFlat");
            FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> name;
            name = FdoIdentifier::Create(L"Entity.Text.Style");
            names->Add(name);
            name = FdoIdentifier::Create(L"FeatureId");
            names->Add(name);
            name = FdoIdentifier::Create(L"Entity.Color");
            names->Add(name);

            FdoComparisonCondition* filterPtr = FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"FeatureId")),
                        FdoComparisonOperations_EqualTo,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)311)));

            selCmd->SetFilter(filterPtr);
            filterPtr->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data2( myReader );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            throw ex;
        }
    }
}


void FdoSelectTest::read_feature_data2( FdoIFeatureReader *myReader )
{
    try
    {
        FdoPtr<FdoClassDefinition>classDefinition = myReader->GetClassDefinition();
        DBG( printf("New feature: class name( %ls ) Feature id ( %d ) \n", classDefinition->GetName(), myReader->GetInt64(L"FeatId") ) );

        if( ! myReader->IsNull(L"Entity") )
        {
            FdoPtr<FdoIFeatureReader> objReader1 = myReader->GetFeatureObject(L"Entity");
            if (objReader1->ReadNext())
            {
                if (!objReader1->IsNull(L"Text"))
                {
                    FdoPtr<FdoIFeatureReader> objReader2 = objReader1->GetFeatureObject(L"Text");
                    if (objReader2->ReadNext())
                    {
                        DBG( printf(" \t\tStyle\t: %ls\n", objReader2->GetString(L"Style")) );
                        objReader2->GetString(L"Style");
                    }
                }
            }
        }

        DBG( printf("\n") );
    }
    catch( FdoException *ex )
    {
        DBG( printf("FDO exception: %ls \n", ex->GetExceptionMessage() ) );
        throw ex;
    }
}

void FdoSelectTest::non_feature_subset_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:testClass");

            FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> name;

            name = FdoIdentifier::Create(L"Object.ObjectWeight");
            names->Add(name);
            name = FdoIdentifier::Create(L"Age");
            names->Add(name);
            name = FdoIdentifier::Create(L"LastName");
            names->Add(name);
            name = FdoIdentifier::Create(L"LastName");
            names->Add(name);
            name = FdoIdentifier::Create(L"Object.ObjectWeight");
            names->Add(name);
 //           name = FdoIdentifier::Create(L"FirstName");
//            names->Add(name);

            FdoFilter* filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)200 ) ) );

            selCmd->SetFilter(filter);
            filter->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_non_feature_data( myReader, true );

                    FdoClassDefinitionP classDef = myReader->GetClassDefinition();
                    FdoPropertiesP props = classDef->GetProperties();
                    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

                    CPPUNIT_ASSERT( props->GetCount() == 3 );
                    CPPUNIT_ASSERT( baseProps->GetCount() == 0 );
                    CPPUNIT_ASSERT( props->Contains(L"Object") );
                    CPPUNIT_ASSERT( props->Contains(L"Age") );
                    CPPUNIT_ASSERT( props->Contains(L"LastName") );
                    CPPUNIT_ASSERT( !props->Contains(L"FirstName") );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
        }
    }
}

void FdoSelectTest::feature_select_obj_distinct()
{
    FdoIDataReader *myReader = NULL;
    FdoISelectAggregates *selCmd = NULL;

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

            FdoFilter* filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)200 ) ) );

            selCmd->SetFilter(filter);
            filter->Release();
            selCmd->SetDistinct( true );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    DBG( printf(" \tObj Weight: %ls\n", myReader->GetString(L"ObjectWeight") ) );
                    myReader->GetString(L"ObjectWeight");
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
			throw;
        }
    }
}
void FdoSelectTest::feature_geom_query ()
{
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> name;
            name = FdoIdentifier::Create(L"Geometry");
            names->Add(name);
            name = FdoIdentifier::Create(L"RevisionNumber");
            names->Add(name);
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    FdoPtr<FdoClassDefinition>classDef = myReader->GetClassDefinition();
                    if( classDef )
                    {
                    FdoPropertiesP props = classDef->GetProperties();
                    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

                    CPPUNIT_ASSERT( props->GetCount() == 1 );
                        DBG( printf("ClassName: %ls\n",classDef->GetName() ) );
                        classDef->GetName();
                        DBG( printf("\tId Properties: \n") );
                        FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = classDef->GetIdentityProperties();
                        for (int i=0; i<idProperties->GetCount(); i++)
                        {
                            FdoPtr<FdoDataPropertyDefinition> idProperty = idProperties->GetItem(i);
                            DBG( printf("\t\t%d) %ls\n", i+1, idProperty->GetName()) );
                            idProperty->GetName();
                        }
                        DBG( printf("\tBase Properties: \n") );
                        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = classDef->GetBaseProperties();
                        CPPUNIT_ASSERT( baseProperties->GetCount() == 1 );
                        CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(baseProperties,L"RevisionNumber") );
                        for (int i=0; i<baseProperties->GetCount(); i++)
                        {
                            FdoPtr<FdoPropertyDefinition> baseProperty = static_cast<FdoPropertyDefinition *> (baseProperties->GetItem(i));
                            DBG( printf("\t\t%d) %ls\n", i+1, baseProperty->GetName()) );
                            baseProperty->GetName();
                        }
                        DBG( printf("\tProperties: \n") );
                        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
                        CPPUNIT_ASSERT( properties->GetCount() == 1 );
                        CPPUNIT_ASSERT( properties->Contains(L"Geometry") );
                        for (int i=0; i<properties->GetCount(); i++)
                        {
                            FdoPtr<FdoPropertyDefinition> property = properties->GetItem(i);
                            DBG( printf("\t\t%d) %ls\n", i+1, property->GetName()) );
                            property->GetName();
                        }
                    }

                    if( ! myReader->IsNull(L"RevisionNumber") )
                    {
                        int revId = myReader->GetInt32(L"RevisionNumber");
                        DBG( printf(" \t\tRevision Number: %d\n", revId) );
                    }

                    if( ! myReader->IsNull(L"Geometry") )
                    {
                        FdoByteArray* byteArray = myReader->GetGeometry(L"Geometry");
                        DBG( printf(" \t\tGemetry byte array size: %d\n", byteArray->GetCount()) );
                        byteArray->GetCount();
                        byteArray->Release();
                    }
                    DBG( printf("\n") );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }
}

void FdoSelectTest::TestDateFilter()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            selectCmd->SetFeatureClassName(L"SNDG");

            FdoPtr<FdoIdentifier> dataPropID = FdoIdentifier::Create(L"SNDG_DATE");
            FdoPtr<FdoDateTimeValue> dataValue = FdoDateTimeValue::Create(FdoDateTime(2004, 5, (FdoInt8)6));
            FdoPtr<FdoComparisonCondition> compCond   = FdoComparisonCondition::Create(
                    dataPropID,
                        FdoComparisonOperations_EqualTo,
                        dataValue);
            selectCmd->SetFilter(compCond);
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
            FdoInt32 idFound = -1;
            long countFound = 0;
            while (reader->ReadNext())
            {
                idFound = reader->GetInt32(L"FEATID1");
                countFound++;
            }
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", countFound==1);
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", idFound==21);
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

// Open multiple select statements concurrently :
void FdoSelectTest::concurrent_select()
{
    try
    {
        // determine class name:
		FdoStringP className;
		className = GetClassName();

        // Start query 1:
        FdoPtr<FdoISelect> selectCmd1 = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
        selectCmd1->SetFeatureClassName(className);
        FdoPtr<FdoIFeatureReader> reader1 = selectCmd1->Execute();

        // Start query 2:
        FdoPtr<FdoISelect> selectCmd2 = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
        selectCmd2->SetFeatureClassName(className);
        FdoPtr<FdoIFeatureReader> reader2 = selectCmd2->Execute();

        // read through all the features of both readers:
        while (reader1->ReadNext() && reader2->ReadNext())
        {
            UnitTestUtil::ProcessFeature(reader1);
            UnitTestUtil::ProcessFeature(reader2);
        }
        reader1->Close();
        reader2->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}
