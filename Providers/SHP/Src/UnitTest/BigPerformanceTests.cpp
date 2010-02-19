/*
 * 
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
#include "BigPerformanceTests.h"

//#include <vld.h>

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#include <Spatial/SpatialUtility.h>

#include <stdlib.h>
#include <crtdbg.h>

//#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW

#ifdef _WIN32
//#define LOCATION L"..\\..\\TestData\\Sheboygan"
//#define LOCATION L"C:\\bugs\\SHP_umlauts"
//#define LOCATION L"C:\\bugs\\1204602.01_SHP"
//#define LOCATION L"E:\\DATA\\SHP_complex_poly"
//#define LOCATION L"C:\\bugs\\Australia_SHP"
//#define LOCATION L"C:\\bugs\\SHP_multipoly"
//#define LOCATION L"E:\\Bugs\\1251835_shp_query"
#define LOCATION L"E:\\Bugs\\SHP_slow\\1304306" 
#else
#define LOCATION L"../../TestData/Sheboygan"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (BigPerformanceTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BigPerformanceTests, "BigPerformanceTests");

FdoPtr<FdoIConnection> BigPerformanceTests::mConnection;

BigPerformanceTests::BigPerformanceTests (void)
{
 //_CrtSetDbgFlag ( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | /*_CRTDBG_CHECK_ALWAYS_DF */_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}

BigPerformanceTests::~BigPerformanceTests (void)
{
    //_CrtDumpMemoryLeaks();

}

void BigPerformanceTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
    mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void BigPerformanceTests::tearDown ()
{
    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
}

void BigPerformanceTests::select ()
{
    clock_t start;
    clock_t restart;
    clock_t finish;
    int count;
    int countMultiPoly;

    double maxTime = 0;
    int maxFeatId = 0;
    long featId = 0;

    double x = 55445.276195817059 ;//55445.276282502949;
    float y = (float)x;
    double z = (double)y;
VERBOSE = true;

    try
    {
        start = clock ();
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        for (int i = 0; i < schemas->GetCount (); i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (i);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
            FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
            FdoFgfGeometryFactory       *factory = FdoFgfGeometryFactory::GetInstance ();
//EB
//        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"Geometry INTERSECTS GeomFromText('POLYGON ((1380146.374386 730212.170323655, 1380121.04874493 728498.873410508, 1381447.8204792 728528.276707783, 1381434.7653856 727562.15479815, 1377949.00985147 726215.684194416, 1377957.39594561 726662.815283045, 1376647.19034448 726550.033842042, 1376716.41746077 730241.101160973, 1380146.374386 730212.170323655))')  OR Geometry INTERSECTS GeomFromText('POLYGON ((1545925.39087765 621858.338925794, 1545740.26201722 621719.868627921, 1545525.92501816 621520.773281604, 1545357.60103542 621388.154390335, 1545128.06848407 621270.837641224, 1544888.33431837 621214.729740113, 1544674.10390918 621224.931075081, 1544495.57845177 621270.837641224, 1544368.06036769 621347.348491684, 1544245.6429511 621377.952915683, 1544026.31173472 621347.348491684, 1543887.13512181 621283.254033819, 1543883.79255852 621872.330697075, 1545925.39087765 621858.338925794))')")));

//           select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON ((1376629.34820628 616651.142761081, 1545925.39087765 616651.142761081, 1545925.39087765 730241.101160973, 1376629.34820628 730241.101160973, 1376629.34820628 616651.142761081))') AND (Geometry INTERSECTS GeomFromText('POLYGON ((1380146.374386 730212.170323655, 1380121.04874493 728498.873410508, 1381447.8204792 728528.276707783, 1381434.7653856 727562.15479815, 1377949.00985147 726215.684194416, 1377957.39594561 726662.815283045, 1376647.19034448 726550.033842042, 1376716.41746077 730241.101160973, 1380146.374386 730212.170323655))') OR Geometry INTERSECTS GeomFromText('POLYGON ((1545925.39087765 621858.338925794, 1545740.26201722 621719.868627921, 1545525.92501816 621520.773281604, 1545357.60103542 621388.154390335, 1545128.06848407 621270.837641224, 1544888.33431837 621214.729740113, 1544674.10390918 621224.931075081, 1544495.57845177 621270.837641224, 1544368.06036769 621347.348491684, 1544245.6429511 621377.952915683, 1544026.31173472 621347.348491684, 1543887.13512181 621283.254033819, 1543883.79255852 621872.330697075, 1545925.39087765 621858.338925794))'))")));
//          select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId = 567381")));

//            select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON ((1734799.60608619 526242.787559716, 1736490.0581299 526242.787559716, 1736490.0581299 527932.483472178, 1734799.60608619 527932.483472178, 1734799.60608619 526242.787559716))')")));

//            Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON ((648357.967150827 850886.292392258, 648639.378315462 850886.292392258, 648639.378315462 851167.589263652, 648357.967150827 851167.589263652, 648357.967150827 850886.292392258))') AND Geometry INSIDE GeomFromText('POLYGON XYZ ((642031.712820815 853898.458083589 0, 642025.790439546 844189.849981099 0, 656125.792570331 844181.352320515 0, 656131.589821817 853889.953919586 0, 642031.712820815 853898.458083589 0))')

            for (int j = 1 /* !!!!*/; j < classes->GetCount (); j++)
            {
                FdoPtr<FdoClassDefinition> cls = classes->GetItem (j);
                restart = clock ();
                select->SetFeatureClassName (cls->GetName ());
                
                printf("------------- %ls\n ---------", cls->GetName ());

                FdoPtr<FdoIFeatureReader> reader = select->Execute ();
                count = 0;
                countMultiPoly = 0;
                while (reader->ReadNext ())
                {
                    clock_t start1  = clock();
                    FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
                    clock_t finish1 = clock ();

                    double elapsed = (double)(finish1 - start1) / CLOCKS_PER_SEC;

                    featId = reader->GetInt32(L"FeatId");
                    if (elapsed > maxTime)
                    {
                        maxTime = elapsed;
                        maxFeatId = featId;
                    }
bool DETAILS = true;
if (DETAILS)
{
                    FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf(geometry);
                    
                    if (geom->GetDerivedType() == FdoGeometryType_Polygon)
                    {

                        FdoIPolygon *poly = (FdoIPolygon *)(geom.p);
                        FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing();

//                        printf("%d.  Polygon - %d holes\n", featId, poly->GetInteriorRingCount());
//                        printf("    %s\n", FdoSpatialUtility::LinearRingIsClockwise(ring)? "CW" : "CCW");
                        
                        for (int k = 0; k < poly->GetInteriorRingCount(); k++)
                        {
                             FdoPtr<FdoILinearRing> ring = poly->GetInteriorRing(k);
//                             printf("    %s\n", FdoSpatialUtility::LinearRingIsClockwise(ring)? "CW" : "CCW");
                        }
                    }
                    else if (geom->GetDerivedType() == FdoGeometryType_MultiPolygon)
                    {
                        countMultiPoly++;
                        FdoIMultiPolygon *mpoly = (FdoIMultiPolygon *)(geom.p);
                        printf("%d.  MultiPolygon - %d polypons (%2.3f  sec)\n", featId, mpoly->GetCount(), (double)(finish1 - start1) / CLOCKS_PER_SEC);

                        for (int n = 0; n < mpoly->GetCount(); n++)
                        {
                            FdoPtr<FdoIPolygon> poly = mpoly->GetItem(n);
                            FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing();
//                            printf("  %d.  Polygon - %d holes\n", n, poly->GetInteriorRingCount());


 //                          printf("    %s\n", FdoSpatialUtility::LinearRingIsClockwise(ring)? "CW" : "CCW");
                        
                            for (int k = 0; k < poly->GetInteriorRingCount(); k++)
                            {
                                 FdoPtr<FdoILinearRing> ring = poly->GetInteriorRing(k);
 //                                printf("    %s\n", FdoSpatialUtility::LinearRingIsClockwise(ring)? "CW" : "CCW");
                            }
                        } 
                    }
}
                    

                    count++;

                    if (count==400000)
                        break;

                }
                finish = clock ();
                if (VERBOSE)
                    printf ("%d features from class %ls in %2.3f seconds\n", count, cls->GetName (), (double)(finish - restart) / CLOCKS_PER_SEC);
                
                printf("    %d multipolygons\n", countMultiPoly);
            }
        }
        finish = clock ();
        //if (VERBOSE)
            printf ("%2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
            printf("maxTime = %2.3f sec (FeatId %ld)\n", maxTime, maxFeatId);
    }
    catch (FdoException* ge)
    {
            printf ("%2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
            printf("maxTime = %2.3f sec (FeatId %ld)\n", maxTime, maxFeatId);
            printf("--last featid=%ld\n", featId);
        TestCommonFail (ge);
    }
}

