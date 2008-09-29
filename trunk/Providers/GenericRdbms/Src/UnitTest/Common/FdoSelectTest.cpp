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
#include "FdoSpatial.h"
//#include <Geometry/GeometryStd.h>
//#include <Geometry/Fgf/Factory.h>
//#include "FdoGeometry.h"
#ifdef _DEBUG
#define  DBG(X)   // X
#else
#define  DBG(X) 
#endif

//FdoIConnection* FdoSelectTest::mConnection = NULL;

FdoSelectTest::FdoSelectTest(void)
{
    mSuffix[0] = '\0';
}

FdoSelectTest::FdoSelectTest(wchar_t *suffix)
{
    wcsncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

FdoSelectTest::~FdoSelectTest(void)
{
    if( mConnection != NULL )
        mConnection->Close();
    mConnection = NULL;
}

void FdoSelectTest::setUp ()
{
    set_provider();
    connect();
}

void FdoSelectTest::tearDown ()
{
    if( mConnection != NULL )
        mConnection->Close();
    mConnection = NULL;
}

void FdoSelectTest::connect ()
{
    try
    {
        mConnection = (FdoIConnection *) UnitTestUtil::GetConnection(mSuffix, true);

    }
    catch (FdoException *ex)
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        if( mConnection != NULL )
            mConnection->Close();
        mConnection= NULL;
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
        FdoPtr<FdoClassDefinition> classDef = myReader->GetClassDefinition();
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
            FdoPtr<FdoByteArray> byteArray = myReader->GetGeometry(L"Geometry");
            DBG( printf(" \tGemetry byte array size: %d\n", byteArray->GetCount()) );
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(byteArray);
            //printf(" \tGeometry Text: <<%ls>>\n", geom->GetText());
            byteArray->GetCount();
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

            FdoPtr<FdoIFeatureReader> objReader = myReader->GetFeatureObject(L"xdata");
            if( objReader )
            {
                DBG( printf(" \txdata - depth(%d): \n",  objReader->GetDepth() ) );
                objReader->GetDepth();
                classDef = objReader->GetClassDefinition();
                if( classDef )
                {
                    DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                    classDef->GetName();
                }
                while ( objReader->ReadNext() )
                {
                    DBG( printf(" \t\tDataValue: %ls\n", objReader->GetString(L"DataValue")) );
                    objReader->GetString(L"DataValue");
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
                        DBG( printf(" \txdata: \n" ) );
                        classDef = objReader2->GetClassDefinition();
                        if( classDef )
                        {
                            DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                            classDef->GetName();
                            DBG( printf(" \t\tQualified ClassName: %ls\n", (const wchar_t*) classDef->GetQualifiedName() ) );
                            classDef->GetQualifiedName();
                        }
                        while ( objReader2->ReadNext() )
                        {
                            DBG( printf(" \t\tDataValue: %ls\n", objReader2->GetString(L"DataValue")) );
                            objReader2->GetString(L"DataValue");
                        }
                    }
                }
        }

        DBG( printf("\n") );
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
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
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;
	clock_t start, finish;

    start = clock();
    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoFilter> filterPtr = FdoSelectTest::CreateFilter();
            selCmd->SetFilter(filterPtr);

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
            }
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }
	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}

void FdoSelectTest::feature_object_query ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDbEntity.xdata2");
            FdoPtr<FdoComparisonCondition> filterPtr = FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"seq")),
                        FdoComparisonOperations_GreaterThan,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)0)));
            //FdoFilter* filterPtr = FdoSelectTest::CreateFilter();
            selCmd->SetFilter(filterPtr);
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
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
			TestCommonFail (ex);
        }
    }
}



void FdoSelectTest::read_non_feature_data( FdoIFeatureReader *myReader, bool subset )
{
    try
    {
        FdoPtr<FdoClassDefinition> classDef = myReader->GetClassDefinition();
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

            FdoPtr<FdoIFeatureReader> objReader = myReader->GetFeatureObject(L"Object");
            if( objReader )
            {
                DBG( printf(" \txdata: \n" ) );
                classDef = objReader->GetClassDefinition();
                if( classDef )
                {
                    DBG( printf(" \t\tClassName: %ls\n",classDef->GetName() ) );
                    classDef->GetName();
                }
                while ( objReader->ReadNext() )
                {
                    DBG( printf(" \t\tWeight: %d\n", objReader->GetInt32(L"ObjectWeight")));
                    objReader->GetInt32(L"ObjectWeight");
                }
            }
        }

        DBG( printf("\n") );
    }
    catch( FdoException *ex )
    {
        DBG( printf("FDO exception: %ls \n", ex->GetExceptionMessage() ) );
        TestCommonFail (ex);
    }
}

void FdoSelectTest::non_feature_query ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:testClass");

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((int)200 ) ) );

            selCmd->SetFilter(filter);

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_non_feature_data( myReader );
                }
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
        }
    }
}

void FdoSelectTest::distance_query()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoInt32 size;

            FdoPtr<FdoIFilterCapabilities> filterCapabilities = mConnection->GetFilterCapabilities();
            FdoDistanceOperations *distanceOperations = filterCapabilities->GetDistanceOperations(size);

            // Do test only if provider supports distance filters.
            if ( size > 0 ) 
            {
                selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
                selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
                FdoPtr<FdoFilter> filterPtr = FdoSelectTest::CreateFilter();
                double coords[6];
                double      distance = 300.0;
                coords[0] = 1410804.854546;
                coords[1] = 553505.994735;
                coords[2] = 0.0;
                coords[3] = 1412350.770162;
                coords[4] = 554980.139631;
                coords[5] = 0.0;
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coords);
                FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

                FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>( FdoGeometryValue::Create(byteArray));
                FdoPtr<FdoDistanceCondition> pSpatialFilter = FdoPtr<FdoDistanceCondition>( FdoDistanceCondition::Create(L"Geometry",
                                                                                    FdoDistanceOperations_Within,
                                                                                    geomValue,
                                                                                    distance ));
                FdoPtr<FdoFilter> filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
                selCmd->SetFilter(filter);

                myReader = selCmd->Execute();
                if( myReader != NULL  )
                {
                    while ( myReader->ReadNext() )
                    {
                        read_feature_data( myReader );
                    }
                }

                // Try the same query again, but with a dimensionality mismatch between filter and selected geometries.
                selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
                selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			    filterPtr = CreateFilter();
                double coords2[4];
                coords2[0] = 1410804.854546;
                coords2[1] = 553505.994735;
                coords2[2] = 1412350.770162;
                coords2[3] = 554980.139631;
			    line1 = gf->CreateLineString(FdoDimensionality_XY, 4, coords2);
			    byteArray = gf->GetFgf(line1);
                printf("Testing DistanceCondition with 2D geometry:\n<<%ls>>\n", line1->GetText());
     
			    geomValue = FdoPtr<FdoGeometryValue>( FdoGeometryValue::Create(byteArray));
		        pSpatialFilter = FdoPtr<FdoDistanceCondition>( FdoDistanceCondition::Create(L"Geometry", 
																				    FdoDistanceOperations_Within,
																				    geomValue, 
																				    distance ));
			    filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
			    selCmd->SetFilter(filter);

                FdoInt32 numRows = 0;
                myReader = selCmd->Execute();
                if( myReader != NULL  )
                {
                    while ( myReader->ReadNext() )
                    {
                        numRows++;
                        read_feature_data( myReader );
                    }
                }
                printf("Read %d rows.\n", numRows);
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
        }
    }
}
void FdoSelectTest::spatial_query()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoFilter> filterPtr = FdoSelectTest::CreateFilter();
            double ordsXYExt[15];
            ordsXYExt[0] = 1412249.54; ordsXYExt[1] = 554576.84; ordsXYExt[2] = 0.0;
            ordsXYExt[3] = 1412249.54; ordsXYExt[4] = 554516.33;ordsXYExt[5] = 0.0;
            ordsXYExt[6] = 1412146.90; ordsXYExt[7] = 554516.33;ordsXYExt[8] = 0.0;
            ordsXYExt[9] = 1412146.90; ordsXYExt[10] = 554576.84;ordsXYExt[11] = 0.0;
            ordsXYExt[12] = 1412249.54; ordsXYExt[13] = 554576.84;  ordsXYExt[14] = 0.0;
            FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY|FdoDimensionality_Z, 15, ordsXYExt);
            FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(extRing, NULL );
            FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(poly))));
            FdoPtr<FdoSpatialCondition> pSpatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(L"Geometry",
                                                                      FdoSpatialOperations_Intersects,
                                                                      geomValue));

            // This AND filter is bogus since filterPtr is empty.
            FdoPtr<FdoFilter> filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
            //selCmd->SetFilter(filter);
            
            selCmd->SetFilter(pSpatialFilter);

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data( myReader );
                }
            }

			pSpatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(L"Geometry",
                                                                      FdoSpatialOperations_EnvelopeIntersects,
                                                                      geomValue));

            // This AND filter is bogus since filterPtr is empty.
            filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
            //selCmd->SetFilter(filter);
            
            selCmd->SetFilter(pSpatialFilter);

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data( myReader );
                }
            }

        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
        }
    }
}

void FdoSelectTest::spatial_query_defect792377()
{
    FdoPtr<FdoIFeatureReader> myReader;
	FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			FdoPtr<FdoFilter> filterPtr = CreateFilter();

            // We want a geometry with more than 999 ordinates, which is a limit imposed
            // by the Oracle parser.  This makes sure that we are binding geometries in
            // spatial filters -- binding can be done with any legal geometry.
            // The easiest way to create one here is to linearise a circular arc.

            FdoString * geomText = L"CURVEPOLYGON XYZ ((1410804.854546 553505.994735 0 (CIRCULARARCSEGMENT (1412350.770162 554980.139631 0, 1410804.854546 553505.994735 0))))";
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(geomText);

            FdoPtr<FdoIGeometry> linearizedGeom =
                FdoSpatialUtility::ApproximateGeometryWithLineStrings(geom, 0, 0.001, gf);

            FdoGeometryType geomType = linearizedGeom->GetDerivedType();
            CPPUNIT_ASSERT( geomType == FdoGeometryType_Polygon );
            FdoIPolygon * poly = static_cast<FdoIPolygon *>(linearizedGeom.p);
            FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing();
            FdoInt32 numPositions = ring->GetCount();
            CPPUNIT_ASSERT( numPositions > 333 );

	        FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(linearizedGeom))));
	        FdoPtr<FdoSpatialCondition> pSpatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(L"Geometry", 
																	  FdoSpatialOperations_Intersects, 
																	  geomValue));

            // This AND filter is bogus since filterPtr is empty
			FdoPtr<FdoFilter> filter = FdoFilter::Combine( filterPtr, FdoBinaryLogicalOperations_And, pSpatialFilter);
			//selCmd->SetFilter(filter);
            
            selCmd->SetFilter(pSpatialFilter);
            FdoInt32 numFeatures = 0;

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    numFeatures++;
                    read_feature_data( myReader );
                }
            }
            printf("   %i feature(s) read\n", numFeatures);
        }
        catch( FdoException *ex )
        {
            printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage());
            throw;
        }
    }
}

void FdoSelectTest::spatial_query_defect813611 ()
{
    FdoPtr<FdoIFeatureReader> myReader;
	FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoInt32 size;
            FdoInt32 idx;
            bool found = false;
            FdoPtr<FdoIFilterCapabilities> filterCapabilities = mConnection->GetFilterCapabilities();
            FdoSpatialOperations *spatialOperations = filterCapabilities->GetSpatialOperations(size);

            for ( idx = 0; idx < size; idx++ ) 
            {
                if ( spatialOperations[idx] == FdoSpatialOperations_Inside )
                {
                    found = true;
                    break;
                }
            }

            if ( found ) 
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
                selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");

                // We want a polygon with one circle formed by two arcs.

                FdoString * geomText = L"CURVEPOLYGON XYZ ((1500876 500030 0 (CIRCULARARCSEGMENT (1400856 600050 0, 1300836 500030 0), CIRCULARARCSEGMENT (1400856 400010 0, 1500876 500030 0))))";
                FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(geomText);

	            FdoPtr<FdoGeometryValue> geomValue = FdoPtr<FdoGeometryValue>(FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(geom))));
	            FdoPtr<FdoSpatialCondition> pSpatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(L"Geometry", 
																	      FdoSpatialOperations_Inside, 
																	      geomValue));

			    selCmd->SetFilter(pSpatialFilter);
                
                FdoInt32 numFeatures = 0;

                myReader = selCmd->Execute();
                if( myReader != NULL  )
                {
                    while ( myReader->ReadNext() )
                    {
                        numFeatures++;
                        read_feature_data( myReader );
                    }
                }
                printf("   %i feature(s) read\n", numFeatures);
            }
        }
        catch( FdoException *ex )
        {
            printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage());
            throw;
        }
    }
}

void FdoSelectTest::spatial_query_defect880310 ()
{
    FdoPtr<FdoIFeatureReader> myReader;
	FdoPtr<FdoISelect> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");

            // Use multiple spatial filters (tests binding of multiple geometries).
            selCmd->SetFilter(L"Geometry INTERSECTS geomfromtext('MULTILINESTRING XYZ ((10 48,10 21,10 0), (16 0,16 23,16 48))') "
                              L"or Geometry INTERSECTS geomfromtext('MULTILINESTRING XYZ ((10 48,10 21,10 0), (16 0,16 23,16 48))')");
            
            FdoInt32 numFeatures = 0;

            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    numFeatures++;
                    read_feature_data( myReader );
                }
            }
            printf("   %i feature(s) read\n", numFeatures);
        }
        catch( FdoException *ex )
        {
            printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage());
            throw;
        }
    }
}

void FdoSelectTest::feature_subset_query ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

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

            FdoPtr<FdoFilter> filterPtr = FdoSelectTest::CreateFilter();
            selCmd->SetFilter(filterPtr);

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
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
        }
    }
}

void FdoSelectTest::feature_subset_query2 ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

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

            FdoPtr<FdoComparisonCondition> filterPtr = FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"FeatureId")),
                        FdoComparisonOperations_EqualTo,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)311)));

            selCmd->SetFilter(filterPtr);
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    read_feature_data2( myReader );
                }
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
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
        TestCommonFail (ex);
    }
}

void FdoSelectTest::non_feature_subset_query ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

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
 //           name = FdoIdentifier::Create(L"FirstName");
//            names->Add(name);

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Object.ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)200 ) ) );

            selCmd->SetFilter(filter);
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
            }
        }
        catch( FdoException *ex )
        {
            DBG( printf("FDO Feature query error: %ls\n", ex->GetExceptionMessage()) );
            TestCommonFail (ex);
        }
    }
}

void FdoSelectTest::feature_select_obj_distinct()
{
    FdoPtr<FdoIDataReader> myReader;
    FdoPtr<FdoISelectAggregates> selCmd;

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

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ObjectWeight") ),
                    FdoComparisonOperations_LessThan,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt32)200 ) ) );

            selCmd->SetFilter(filter);
            selCmd->SetDistinct( true );
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    DBG( printf(" \tObj Weight: %ls\n", myReader->GetString(L"ObjectWeight") ) );
                    myReader->GetString(L"ObjectWeight");
                }
            }
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }
}
void FdoSelectTest::feature_geom_query ()
{
    FdoPtr<FdoIFeatureReader> myReader;
    FdoPtr<FdoISelect> selCmd;

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
                        int count1, count2;

                        const FdoByte* bytes = myReader->GetGeometry(L"Geometry", &count1);

                        FdoPtr<FdoByteArray> byteArray = myReader->GetGeometry(L"Geometry");
                        DBG( printf(" \t\tGemetry byte array size: %d\n", byteArray->GetCount()) );
                        count2 = byteArray->GetCount();
                        printf(" \t\tGeometry byte array size: %d\n", count2);
                        if ( count1 != count2 )
                        {
                            CPPUNIT_FAIL((const char*) FdoStringP::Format(L"FDO Feature query error: count1=%d vs. count2=%d\n", count1, count2));
                            throw;
                        }
                    }
                    DBG( printf("\n") );
                }
            }
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
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
            TestCommonFail (e);
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
        TestCommonFail (e);
    }
}
