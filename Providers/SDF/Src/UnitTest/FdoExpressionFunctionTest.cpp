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

#include "stdafx.h"
#include "FdoExpressionFunctionTest.h"
#include "UnitTestUtil.h"
#include <math.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <FdoCommonMiscUtil.h>
#include <FdoExpressionEngine.h>
#include <limits>       // For quiet_NaN()
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FdoExpressionFunctionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FdoExpressionFunctionTest, "FdoExpressionFunctionTest");

#ifdef _WIN32
    static const wchar_t* EE_OPT_TEST_FILE = L"..\\..\\TestData\\OptimizerTest.sdf";
#else
    static const wchar_t* EE_OPT_TEST_FILE = L"../../TestData/OptimizerTest.sdf";
#endif

#ifdef _WIN32
    static const wchar_t* EE_HOSPITAL_TEST_FILE = L"..\\..\\TestData\\HOSPITAL.SDF";
#else
    static const wchar_t* EE_HOSPITAL_TEST_FILE = L"../../TestData/HOSPITAL.SDF";
#endif


// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

FdoExpressionFunctionTest::FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  FdoExpressionFunctionTest ()

FdoExpressionFunctionTest::~FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
}  //  ~FdoExpressionFunctionTest ()

// ----------------------------------------------------------------------------
// --                     Combination Unit Test Functions                    --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::NumberToStringComparison ()

// +---------------------------------------------------------------------------
// | The function executes a filter on the test data set that uses the function
// | TOSTRING on two different properties and compares the function results to
// | determine if a data set qualifies as defined by the filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                      is_valid_result = false;

    FdoInt32                  row_count       =  0,
                              id_val          = -1;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Combination Unit Test: Number to String Comparison        \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"ToString(dbl_val) = ToString(sgl_val)");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command using the filter to   \n");
    printf("  determine the qualified data set. The function should    \n");
    printf("  return two rows. No exceptions are expected.             \n");
    printf("---------------------------------------------------------- \n");

    try {

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext()) {

        row_count++;
        id_val = data_reader->GetInt32(L"id");
        is_valid_result = ((id_val == 0) || (id_val == 20));
        if (!is_valid_result)
            break;
        
      }  //  while (data_reader->ReadNext()) ...

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if ((!is_valid_result) || (row_count != 2))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
      else
        printf(" >>> ... All expected data found\n");

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  NumberToStringComparison ()


void FdoExpressionFunctionTest::RoundNumberToStringComparison ()

// +---------------------------------------------------------------------------
// | The function executes a filter on the test data set that uses the func-
// | tions ROUND and TOSTRING on two different properties and compares the
// | function results to determine if a data set qualifies as defined by the
// | filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                      is_valid_result = false;

    FdoInt32                  row_count       =  0,
                              id_val          = -1;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Combination Unit Test: Round Number to String Comparison  \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter =
        FdoFilter::Parse(
                L"ToString(Round(dbl_val, 2)) = ToString(Round(sgl_val, 2))");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command using the filter to   \n");
    printf("  determine the qualified data set. The function should    \n");
    printf("  return three rows. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext()) {

        row_count++;
        id_val = data_reader->GetInt32(L"id");
        is_valid_result = ((id_val == 0) || (id_val == 17) || (id_val == 20));
        if (!is_valid_result)
            break;
        
      }  //  while (data_reader->ReadNext()) ...

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if ((!is_valid_result) || (row_count != 3))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
      else
        printf(" >>> ... All expected data found\n");

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  RoundNumberToStringComparison ()


// ----------------------------------------------------------------------------
// --                       Special Unit Test Functions                      --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::ModuloOperation ()

// +---------------------------------------------------------------------------
// | The function executes a modulo operation on multiple rows. This is to
// | verify that the returned data is accurate for each row.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val,
                              cmp_val;

    FdoStringP                func_call;

    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: Modulo Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the value of a different property of     \n");
    printf("  type INT32. The test works on all rows in the test class \n");
    printf("  table. The test is to check that not the same value is   \n");
    printf("  returned for each of the rows as the original property   \n");
    printf("  value is different for each row. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and document the returned data.

      func_call   = L"(Mod(id, 10) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", NULL, true, func_call);
      while (data_reader->ReadNext()) {

        id_val  = data_reader->GetInt32(L"id");
        cmp_val = data_reader->GetInt32(L"cmp_id");

        printf(" --->>> (id, mod_val) = (%d, %d) \n", id_val, cmp_val);

      }  //  while ...

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  ModuloOperation ()

void FdoExpressionFunctionTest::RunOptimizerTest()

// +---------------------------------------------------------------------------
// | The function tests the expression optimizer for filters.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                        testResult = true;
    FdoPtr<IConnectionManager>  manager;
    FdoPtr<FdoIConnection>      conn;

    manager = FdoFeatureAccessManager::GetConnectionManager ();
    conn    = manager->CreateConnection (L"OSGeo.SDF");

    try {

#ifdef EE_TEST_INS_FEATURES
      std::wstring connStr =
                    std::wstring(L"File=") + std::wstring(EE_OPT_TEST_FILE);
      connStr += std::wstring(L";ReadOnly=FALSE");
      conn->SetConnectionString(connStr.c_str());
      conn->Open();
      InsertTestFeatures(conn);
#else
      std::wstring connStr =
                    std::wstring(L"File=") + std::wstring(EE_OPT_TEST_FILE);
      connStr += std::wstring(L";ReadOnly=TRUE");
      conn->SetConnectionString(connStr.c_str());
      conn->Open();
#endif

      FdoPtr<FdoIFilterCapabilities> fltCapab = conn->GetFilterCapabilities();
      int szCap = 0;
      FdoSpatialOperations *spConds = fltCapab->GetSpatialOperations(szCap);
        
      // big geometry

      FdoPtr<FdoSpatialCondition> bigGeomCond =
          static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INSIDE GeomFromText('POLYGON ((5726572.25349578 2244716.40810411, 5583530.40649704 2337005.79391714, 5629331.98126834 2207377.9551015, 5534910.27144644 2174970.99624234, 5657517.56705144 2163699.01011146, 5490517.97256231 2048865.65372617, 5632150.54154764 2039707.16436127, 5672315.00001281 2146086.53199315, 5768850.63004419 2006595.70658011, 5787171.26122845 2093953.5965602, 5828040.35976345 2153131.52290257, 5735027.92795496 2179902.49038579, 5816766.1271512 2262328.88622243, 5726572.25349578 2244716.40810411))')"));

      // small geometry inside the big geometry

      FdoPtr<FdoSpatialCondition> smallGeomCond =
          static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5767354.11879699 2012972.41972645, 5695850.38275576 2116555.74670371, 5695850.38275576 2160706.01616614, 5774147.82229251 2166139.89621237, 5824081.54819392 2151706.15381787, 5782639.9532566 2095499.46405879, 5767354.11879699 2012972.41972645))')"));
        
      for (int i = 0; i < szCap; i++) {

        for (int y = 0; y < szCap; y++) {

          FdoStringP txtOp1 =
              FdoCommonMiscUtil::FdoSpatialOperationsToString(
                                                  spConds[i]).Right(L"_");
          FdoStringP txtOp2 =
              FdoCommonMiscUtil::FdoSpatialOperationsToString(
                                                  spConds[y]).Right(L"_");
          printf("\n\nRun[%ls, %ls]", (FdoString*)txtOp1, (FdoString*)txtOp2);

          // Generate the filter
          bigGeomCond->SetOperation(spConds[i]);
          smallGeomCond->SetOperation(spConds[y]);
          FdoPtr<FdoFilter> pFilter =
                      FdoBinaryLogicalOperator::Create(
                                          bigGeomCond,
                                          FdoBinaryLogicalOperations_And,
                                          smallGeomCond);

          // Run query which will be optimized.

          FdoPtr<FdoISelect> selCmd =
                    (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
          selCmd->SetFeatureClassName(L"bayarea_county");
          selCmd->SetFilter(pFilter);
          FdoPtr<FdoIFeatureReader> optReader = selCmd->Execute();

          // Get all data.

          selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
          selCmd->SetFeatureClassName(L"bayarea_county");
          selCmd->SetOrderingOption(FdoOrderingOption_Ascending);
          FdoPtr<FdoIFeatureReader> allReader = selCmd->Execute();
          if (!EvaluateResults(bigGeomCond,
                               smallGeomCond,
                               allReader,
                               optReader)) {

              testResult = true;
              printf("...Failed");

          }  //  if (!EvaluateResults( ...
          else
            printf("...Succeeded");

          allReader->Close();
          optReader->Close();

        }  //  for (int y = 0; ...

      }  //  for (int i = 0; ...

    }  //  try ...

    catch(FdoException *ex) {

      FdoStringP msg = ex->GetExceptionMessage();
      ex->Release();
      CPPUNIT_FAIL ((const char*)msg);

    }  //  catch(FdoException *ex) ...

    if (!testResult)
        CPPUNIT_FAIL ("Evaluation failed see above text!");

    RunOptimizerTest2();
}  //  RunOptimizerTest ()

void FdoExpressionFunctionTest::RunOptimizerTest2()
{
    // PROP = 5 AND Geom INTERSECTS() AND (ENTITYID=400 OR ENTITYID=401) AND Geom ENVELOPEINTERSECTS()
    // no optimize can be done however ENVELOPEINTERSECTS should be moved first
    FdoPtr<FdoFilter> flt = FdoFilter::Parse(L"PROP = 5 AND Geom INTERSECTS GeomFromText('POLYGON ((5995539.74872539 2104905.70098863, 6015088.3917918 2104905.70098863, 6015088.3917918 2123615.90032945, 5995539.74872539 2123615.90032945, 5995539.74872539 2104905.70098863))') AND (ENTITYID=400 OR ENTITYID=401) AND Geom ENVELOPEINTERSECTS GeomFromText('POLYGON ((5995539.74872539 2104905.70098863, 6015088.3917918 2104905.70098863, 6015088.3917918 2123615.90032945, 5995539.74872539 2123615.90032945, 5995539.74872539 2104905.70098863))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);

    FdoString* rezFilter = L"Geom ENVELOPEINTERSECTS GeomFromText('POLYGON ((5995539.74872539 2104905.70098863, 6015088.3917918 2104905.70098863, 6015088.3917918 2123615.90032945, 5995539.74872539 2123615.90032945, 5995539.74872539 2104905.70098863))') AND Geom INTERSECTS GeomFromText('POLYGON ((5995539.74872539 2104905.70098863, 6015088.3917918 2104905.70098863, 6015088.3917918 2123615.90032945, 5995539.74872539 2123615.90032945, 5995539.74872539 2104905.70098863))') AND PROP = 5 AND (ENTITYID = 400 OR ENTITYID = 401)";
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), rezFilter) == 0);

    // No change here since we have only OR operator
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))') OR Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    rezFilter = L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))') OR Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))')";
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), rezFilter) == 0);

    // ENVELOPEINTERSECTS sould be moved first
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')   AND  Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    rezFilter = L"Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))') AND Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')";
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), rezFilter) == 0);

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();
    // create the filter which force an empty result
	// If the condition do not overlap, then replace it with a filter that returns 0 features.
#ifdef _WIN32
	double small_dbl  =(double)(-9223372036854775807i64 - 1);
#else
	double small_dbl  =(double)(-9223372036854775807LL - 1);
#endif
	double coords[] = { small_dbl, small_dbl, 
						small_dbl, small_dbl, 
						small_dbl, small_dbl, 
						small_dbl, small_dbl, 
						small_dbl, small_dbl }; 

	FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);

	FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

	FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
	FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
	FdoPtr<FdoFilter> emptyFlt = FdoSpatialCondition::Create(L"Geometry", FdoSpatialOperations_EnvelopeIntersects, gv);


    // last rectangle is inside left rectangle only, so result filter should the one which produces empty results
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')   AND  Geometry INSIDE GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))') AND Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1873723.82969623 467333.437615339 0, 1873723.82969623 466047.31151245 0, 1876059.83961431 466047.31151245 0, 1876059.83961431 467333.437615339 0, 1873723.82969623 467333.437615339 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), emptyFlt->ToString()) == 0);
    
    // last rectangle is inside right rectangle only, so result filter should the one which produces empty results
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')   AND  Geometry INSIDE GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))') AND Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1908485.9271423 475860.574939836 0, 1908485.9271423 474251.682596432 0, 1911141.36517323 474251.682596432 0, 1911141.36517323 475860.574939836 0, 1908485.9271423 475860.574939836 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), emptyFlt->ToString()) == 0);
    
    // last rectangle is inside both rectangles, so result filter should have ENVELOPEINTERSECTS first followed by INTERSECTS spatial conditions
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')   AND  Geometry INSIDE GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))') AND Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1884426.05101696 468379.216650495 0, 1884426.05101696 467167.473887943 0, 1886476.89825489 467167.473887943 0, 1886476.89825489 468379.216650495 0, 1884426.05101696 468379.216650495 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    rezFilter = L"Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1884426.05101696 468379.216650495 0, 1884426.05101696 467167.473887943 0, 1886476.89825489 467167.473887943 0, 1886476.89825489 468379.216650495 0, 1884426.05101696 468379.216650495 0))') AND Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))') AND Geometry INSIDE GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))')";
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), rezFilter) == 0);
    
    // last rectangle is not inside left/right rectangle, so result filter should the one which produces empty results
    flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((1871309.79925634 469585.887554809 0, 1871309.79925634 449326.771540789 0, 1895996.14397898 449326.771540789 0, 1895996.14397898 469585.887554809 0, 1871309.79925634 469585.887554809 0))')   AND  Geometry INSIDE GeomFromText('POLYGON XYZ ((1882575.29131501 477469.470576763 0, 1882575.29131501 441108.461458735 0, 1925625.57054782 441108.461458735 0, 1925625.57054782 477469.470576763 0, 1882575.29131501 477469.470576763 0))') AND Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON XYZ ((1928120.07473177 458886.741285136 0, 1928120.07473177 456714.735633484 0, 1931741.12493866 456714.735633484 0, 1931741.12493866 458886.741285136 0, 1928120.07473177 458886.741285136 0))')");
    flt = FdoExpressionEngine::OptimizeFilter(flt);
    CPPUNIT_ASSERT(wcscmp(flt->ToString(), emptyFlt->ToString()) == 0);
}

void FdoExpressionFunctionTest::SoundexFilter ()

// +---------------------------------------------------------------------------
// | The function executes a specific test identified by QA that resulted in a
// | MAP crash.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val,
                              row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: SOUNDEX as filter                \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the feature \n");
    printf("  ids of object for which a filter using the expression    \n");
    printf("  function SOUNDEX applies. The test is expected to return \n");
    printf("  a specific number of rows. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      filter = FdoFilter::Parse(
                        L"Soundex(str2_val) = Soundex('The Color is: 777')");
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext()) {

        row_count++;
        id_val  = data_reader->GetInt32(L"id");
        printf(" --->>> (id) = (%d) \n", id_val);

      }  //  while ...

      if (row_count != 31)
          throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
      else
        printf(" >>> ... All expected data found\n");

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  SoundexFilter ()

void FdoExpressionFunctionTest::SubstrOperation ()

// +---------------------------------------------------------------------------
// | The function executes a substring operation on multiple rows. This is to
// | verify that the returned data is accurate for each row.
// +---------------------------------------------------------------------------

{


    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val;

    FdoStringP                cmp_val,
                              func_call;

    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: SUBSTR Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case the test requests a sub-string \n");
    printf("  from a position within the source string to the end of   \n");
    printf("  it by not specifying the optional length parameter. The  \n");
    printf("  test is to verify that the returned value for each of    \n");
    printf("  the rows is accurate and that not one value is returned  \n");
    printf("  for all the rows. No exceptions are expected.            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and document the returned data.

      func_call   = L"(Substr(str2_val, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", NULL, true, func_call);
      while (data_reader->ReadNext()) {

        id_val  = data_reader->GetInt32(L"id");
        cmp_val = data_reader->GetString(L"cmp_id");

        printf(" --->>> (id, mod_val) = (%d, >>%ls<<) \n",
               id_val, (FdoString*)cmp_val);

      }  //  while ...

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  // SubstrOperation ()

void FdoExpressionFunctionTest::ToDateOperation ()

// +---------------------------------------------------------------------------
// | The function tests the expression function TODATE as a filter when con-
// | nected to a specific SDF file provided by QA. Apparently, no data is re-
// | turned although at least one row qualifies.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                  row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: TODATE Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    try {

      // At this point there is already a connection to a different SDF file.
      // For the following test this one needs to be closed and a new connec-
      // tion established to the file QA provided.

      printf("---------------------------------------------------------- \n");
      printf("Test Setup:                                                \n");
      printf("  Close possible current connection and establish a con-   \n");
      printf("  nection to the QA provided SDF file.                     \n");
      printf("---------------------------------------------------------- \n");

      CloseConnection();
      m_connection =
            UnitTestUtil::OpenConnection(
                    L"..\\..\\TestData\\SPR1011594\\CopyofConversions.sdf", false);

      // Execute the request that is to be tested with this test case.

      filter      = FdoFilter::Parse(
                            L"\"DATE\" = ToDate(DateString, 'MM/DD/YYYY hh:mm:ss')");
      data_reader = ExecuteSelectCommand(
                                    L"FeatureClass1", filter, false, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext())
        row_count++;

      //if (row_count != 31)
      //    throw FdoException::Create(
      //                  L"Unexpected result(s) when checking returned data");
      //else
      //  printf(" >>> ... All expected data found\n");

      printf(" >>> Test succeeded \n");

      // Close the connection again.

      CloseConnection();

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  ToDateOperation ()


//-------------------------------------------------------------------------
//                     Embeded INTERSECTS Test Functions
//-------------------------------------------------------------------------
void FdoExpressionFunctionTest::RunEmbeddedIntersectsTest ()
{
    bool                        testResult = true;
    FdoPtr<IConnectionManager>  manager;
    FdoPtr<FdoIConnection>      conn;

    manager = FdoFeatureAccessManager::GetConnectionManager ();
    conn    = manager->CreateConnection (L"OSGeo.SDF");

    try {

      wchar_t fullpath[1024];
      _wfullpath(fullpath, EE_HOSPITAL_TEST_FILE, 1024);
      std::wstring connStr = std::wstring(L"File=") + fullpath;
      connStr += std::wstring(L";ReadOnly=TRUE");
      conn->SetConnectionString(connStr.c_str());
      conn->Open();

      FdoPtr<FdoFilter> spFilter =
          FdoFilter::Parse(FdoStringP(
                "Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON (( "
                "1887474.9312 450098.4312,"
                "1896219.4944 450098.4312,"
                "1896219.4944 458090.2237,"
                "1887474.9312 458090.2237,"
                "1887474.9312 450098.4312"
                "))') "
                "AND Geometry INTERSECTS GeomFromText('POLYGON XYZ (( "
                "1889298.3991 457671.4451 0,"
                "1887474.9312 453675.5488 0,"
                "1890023.7450 450098.4312 0," 
                "1894396.0265 450517.2099 0," 
                "1896219.4944 454513.1061 0," 
                "1893670.6807 458090.2237 0," 
                "1889298.3991 457671.4451 0"
                "))') "
                "AND Geometry INTERSECTS GeomFromText('POLYGON XYZ (( "
                "1892048.0272 456421.0618 0," 
                "1891678.7365 455964.5416 0," 
                "1891805.4093 455391.1828 0," 
                "1892332.6585 455132.7362 0," 
                "1892863.4547 455383.8175 0," 
                "1892998.0974 455955.3572 0," 
                "1892635.2054341326 456416.98213748500 0," 
                "1892048.0272 456421.0618 0"
                "))') "
                "AND Geometry INTERSECTS GeomFromText('POLYGON XYZ (( "
                "1890067.8281 455373.4786 0,"
                "1889011.3434 454714.0822 0," 
                "1888730.5596 453500.7710 0," 
                "1889389.9560 452444.2863 0," 
                "1890603.2672 452163.5025 0," 
                "1891659.7519 452822.8990 0," 
                "1891940.5357 454036.2101 0," 
                "1891281.1392 455092.6948 0," 
                "1890067.8281 455373.4786 0"
                "))')")
            );

        // Run query which will be optimized.

        FdoPtr<FdoIExtendedSelect> selCmd = (FdoIExtendedSelect*)conn->CreateCommand(FdoCommandType_ExtendedSelect);
        selCmd->SetFeatureClassName(L"Hospital");
        selCmd->SetFilter(spFilter);
        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        bool results = reader->ReadNext();
        CPPUNIT_ASSERT(results == false);
        reader->Close();
        testResult = true;

    }  //  try ...

    catch (FdoException *ex) {

        FdoStringP msg = ex->GetExceptionMessage();
        ex->Release();
        CPPUNIT_FAIL ((const char*)msg);

    }  //  catch(FdoException *ex) ...

    if (!testResult)
        CPPUNIT_FAIL ("Evaluation failed see above text!");
}

//-------------------------------------------------------------------------
//                     Embeded INTERSECTS Test Functions II
//-------------------------------------------------------------------------
void FdoExpressionFunctionTest::RunEmbeddedIntersectsTest2 ()
{
    bool                        testResult = true;
    FdoPtr<IConnectionManager>  manager;
    FdoPtr<FdoIConnection>      conn;

    manager = FdoFeatureAccessManager::GetConnectionManager ();
    conn    = manager->CreateConnection (L"OSGeo.SDF");

    try {

      wchar_t fullpath[1024];
      _wfullpath(fullpath, EE_HOSPITAL_TEST_FILE, 1024);
      std::wstring connStr = std::wstring(L"File=") + fullpath;
      connStr += std::wstring(L";ReadOnly=TRUE");
      conn->SetConnectionString(connStr.c_str());
      conn->Open();

      FdoPtr<FdoFilter> spFilter =
          FdoFilter::Parse(FdoStringP(
                "Geometry ENVELOPEINTERSECTS GeomFromText('POLYGON (( "
                "1887474.9312 450098.4312,"
                "1896219.4944 450098.4312,"
                "1896219.4944 458090.2237,"
                "1887474.9312 458090.2237,"
                "1887474.9312 450098.4312"
                "))') "
                "AND Geometry INSIDE GeomFromText('POLYGON XYZ (( "
                "1889298.3991 457671.4451 0,"
                "1887474.9312 453675.5488 0,"
                "1890023.7450 450098.4312 0," 
                "1894396.0265 450517.2099 0," 
                "1896219.4944 454513.1061 0," 
                "1893670.6807 458090.2237 0," 
                "1889298.3991 457671.4451 0"
                "))') "
                "Geometry INTERSECTS GeomFromText('POLYGON XYZ (( "
                "1892048.0272 456421.0618 0," 
                "1891678.7365 455964.5416 0," 
                "1891805.4093 455391.1828 0," 
                "1892332.6585 455132.7362 0," 
                "1892863.4547 455383.8175 0," 
                "1892998.0974 455955.3572 0," 
                "1892635.2054341326 456416.98213748500 0," 
                "1892048.0272 456421.0618 0"
                "))') "
                "AND NOT Geometry INSIDE GeomFromText('POLYGON XYZ (( "
                "1890067.8281 455373.4786 0,"
                "1889011.3434 454714.0822 0," 
                "1888730.5596 453500.7710 0," 
                "1889389.9560 452444.2863 0," 
                "1890603.2672 452163.5025 0," 
                "1891659.7519 452822.8990 0," 
                "1891940.5357 454036.2101 0," 
                "1891281.1392 455092.6948 0," 
                "1890067.8281 455373.4786 0"
                "))')"
                )
            );

        // Run query which will be optimized.

        FdoPtr<FdoIExtendedSelect> selCmd = (FdoIExtendedSelect*)conn->CreateCommand(FdoCommandType_ExtendedSelect);
        selCmd->SetFeatureClassName(L"Hospital");
        selCmd->SetFilter(spFilter);
        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        bool results = reader->ReadNext();
        CPPUNIT_ASSERT(results == true);
        reader->Close();
        testResult = true;

    }  //  try ...

    catch (FdoException *ex) {

        FdoStringP msg = ex->GetExceptionMessage();
        ex->Release();
        CPPUNIT_FAIL ((const char*)msg);

    }  //  catch(FdoException *ex) ...

    if (!testResult)
        CPPUNIT_FAIL ("Evaluation failed see above text!");
}

// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{

    // Establish the connection.

    m_connection = UnitTestUtil::OpenConnection(DATASTORE_EXTENSION, true);

}  //  Connect ()

