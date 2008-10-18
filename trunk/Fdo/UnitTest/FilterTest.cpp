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
#include "FilterTest.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#include "TestProcessor.h"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (FilterTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FilterTest, "FilterTest");

static FdoString*   aszNames[] =
{
    L"One",
    L"Two",
    L"Three"
};

FilterTest::FilterTest(void)
{
}

FilterTest::~FilterTest(void)
{
}

void FilterTest::setUp()
{
    // nothing to do at setup time... yet
}

void FilterTest::testFilterTree()
{
    // expression classes needed for test
    FdoDataValue*           pDVal = FdoStringValue::Create(L"abcabc'");
    FdoFunction*            pFunc = FdoFunction::Create(L"FunctionName", NULL, 0);
    FdoBinaryExpression*    pExprMult = FdoBinaryExpression::Create(pDVal, FdoBinaryOperations_Multiply, pFunc);
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"th is.is.a.test");
	FdoByteArray*			pFGF = FdoByteArray::Create((FdoByte*)"SomeGeometryData", 17);
    FdoGeometryValue*       pGVal = FdoGeometryValue::Create(pFGF);

    // Filter Classes
    FdoComparisonCondition* pCompare = FdoComparisonCondition::Create(pExprMult, FdoComparisonOperations_EqualTo, pDVal);
    FdoDistanceCondition*   pDistance = FdoDistanceCondition::Create(pIdent, FdoDistanceOperations_Beyond, pGVal, 3.14);
    FdoBinaryLogicalOperator*   pEqual = FdoBinaryLogicalOperator::Create(pCompare, FdoBinaryLogicalOperations_And, pDistance);
    printf("%ls\n", pEqual->ToString());

    FdoInCondition*         pIn = FdoInCondition::Create(pIdent, aszNames, sizeof(aszNames) / sizeof(aszNames[0]));
#ifdef _WIN32
    _putws(pIn->ToString());
#else
    printf("%ld\n", pIn->ToString());
#endif

    FdoNullCondition*       pNull = FdoNullCondition::Create(pIdent);
    printf("%ls\n", pNull->ToString());

    FdoSpatialCondition*    pSpatial = FdoSpatialCondition::Create(pIdent, FdoSpatialOperations_Contains, pGVal);
    FdoUnaryLogicalOperator*    pNot = FdoUnaryLogicalOperator::Create(pSpatial, FdoUnaryLogicalOperations_Not);
    printf("%ls\n", pNot->ToString());

    // release Filter classes
    pCompare->Release();
    pDistance->Release();
    pEqual->Release();
    pIn->Release();
    pNull->Release();
    pSpatial->Release();
    pNot->Release();

    // expression classes
    pIdent->Release();
    pExprMult->Release();
    pFunc->Release();
    pDVal->Release();
	pFGF->Release();
    pGVal->Release();
}

void FilterTest::testBinaryLogicalOperator()
{
    // create some data for the test.
    FdoDataValue*           pDVal = FdoStringValue::Create(L"'abcabc'");
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"this.is.a.t'e\"st");
    FdoComparisonCondition* pCompare1 = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pDVal);
    FdoComparisonCondition* pCompare2 = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_GreaterThan, pDVal);

    // check that the constructor put everything in it's place
    FdoBinaryLogicalOperator*   pEqual = FdoBinaryLogicalOperator::Create(pCompare1, FdoBinaryLogicalOperations_Or, pCompare2);
    FDO_CPPUNIT_ASSERT(pEqual->GetOperation() == FdoBinaryLogicalOperations_Or);
    FdoFilter*   pFilter = pEqual->GetLeftOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare1);
    pFilter->Release();
    pFilter = pEqual->GetRightOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare2);
    pFilter->Release();

    // check setting to null
    pEqual->SetLeftOperand(NULL);
    pEqual->SetRightOperand(NULL);
    FDO_CPPUNIT_ASSERT(pEqual->GetLeftOperand() == NULL);
    FDO_CPPUNIT_ASSERT(pEqual->GetRightOperand() == NULL);

    // check the set methods.
    pEqual->SetLeftOperand(pCompare2);
    pEqual->SetRightOperand(pCompare1);
    pEqual->SetOperation(FdoBinaryLogicalOperations_And);
    FDO_CPPUNIT_ASSERT(pEqual->GetOperation() == FdoBinaryLogicalOperations_And);
    pFilter = pEqual->GetLeftOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare2);
    pFilter->Release();
    pFilter = pEqual->GetRightOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare1);
    pFilter->Release();

    // release everything
    pDVal->Release();
    pIdent->Release();
    pCompare1->Release();
    pCompare2->Release();
    pEqual->Release();
}

void FilterTest::testComparisonCondition()
{
    // create some data for the test.
    FdoDataValue*           pDVal = FdoInt32Value::Create(123);
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"thisisatest");

    // test the constructor
    FdoComparisonCondition* pCompare = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pDVal);
    FDO_CPPUNIT_ASSERT(pCompare->GetOperation() == FdoComparisonOperations_EqualTo);
    FdoExpression*  pExpr = pCompare->GetLeftExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pIdent);
    pExpr->Release();
    pExpr = pCompare->GetRightExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pDVal);
    pExpr->Release();

    // test the setters and getters
    pCompare->SetOperation(FdoComparisonOperations_GreaterThan);
    pCompare->SetLeftExpression(pDVal);
    pCompare->SetRightExpression(pIdent);
    FDO_CPPUNIT_ASSERT(pCompare->GetOperation() == FdoComparisonOperations_GreaterThan);
    pExpr = pCompare->GetLeftExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pDVal);
    pExpr->Release();
    pExpr = pCompare->GetRightExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pIdent);
    pExpr->Release();

    // release everything
    pDVal->Release();
    pIdent->Release();
    pCompare->Release();
}

void FilterTest::testDistanceCondition()
{
    // create some data for the test.
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"how.now.brown.cow");
	FdoByteArray*			pFGF1 = FdoByteArray::Create((FdoByte*)"testing123", 10);
    FdoGeometryValue*       pGVal1 = FdoGeometryValue::Create(pFGF1);
	FdoByteArray*			pFGF2 = FdoByteArray::Create((FdoByte*)"testtesttest", 12);
    FdoGeometryValue*       pGVal2 = FdoGeometryValue::Create(pFGF2);

    // check constructor.
    FdoDistanceCondition*   pCond1 = FdoDistanceCondition::Create(L"PropName", FdoDistanceOperations_Beyond, pGVal1, 3.14);
    FDO_CPPUNIT_ASSERT(pCond1->GetDistance() == 3.14);
    FdoExpression*   pExpr = pCond1->GetGeometry();
    FDO_CPPUNIT_ASSERT(pExpr == pGVal1);
    pExpr->Release();
    FDO_CPPUNIT_ASSERT(pCond1->GetOperation() == FdoDistanceOperations_Beyond);
    FdoIdentifier*  pIdentTest = pCond1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(wcscmp(pIdentTest->GetText(), L"PropName") == 0);
    pIdentTest->Release();

    // test the setters and getters
    pCond1->SetDistance(1.21);
    pCond1->SetGeometry(NULL);
    pCond1->SetPropertyName(pIdent);
    FDO_CPPUNIT_ASSERT(pCond1->GetDistance() == 1.21);
    FDO_CPPUNIT_ASSERT(pCond1->GetGeometry() == NULL);
    pIdentTest = pCond1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // check the other constructor
    FdoDistanceCondition*   pCond2 = FdoDistanceCondition::Create(pIdent, FdoDistanceOperations_Within, pGVal2, .9944);
    FDO_CPPUNIT_ASSERT(pCond2->GetDistance() == .9944);
    pExpr = pCond2->GetGeometry();
    FDO_CPPUNIT_ASSERT(pExpr == pGVal2);
    pExpr->Release();
    FDO_CPPUNIT_ASSERT(pCond2->GetOperation() == FdoDistanceOperations_Within);
    pIdentTest = pCond2->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // more tests of the setters and getters
    pCond2->SetGeometry(pGVal1);
    pCond2->SetOperation(FdoDistanceOperations_Beyond);
    pCond2->SetPropertyName(NULL);
    pExpr = pCond2->GetGeometry();
    FDO_CPPUNIT_ASSERT(pExpr == pGVal1);
    pExpr->Release();
    FDO_CPPUNIT_ASSERT(pCond2->GetOperation() == FdoDistanceOperations_Beyond);
    FDO_CPPUNIT_ASSERT(pCond2->GetPropertyName() == NULL);

    // release everything
    pCond1->Release();
    pCond2->Release();
	pFGF1->Release();
	pFGF2->Release();
    pGVal1->Release();
    pGVal2->Release();
    pIdent->Release();
}

void FilterTest::testInCondition()
{
    // create some data for the test.
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"zip.a.dee.doo.da");

    // check constructor.
    FdoInCondition*         pIn1 = FdoInCondition::Create(L"RogerWilco", aszNames, 3);
    FdoIdentifier*   pIdentTest = pIn1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(wcscmp(pIdentTest->GetText(), L"RogerWilco") == 0);
    pIdentTest->Release();
    FdoValueExpressionCollection* pColl = pIn1->GetValues();
    FDO_CPPUNIT_ASSERT(pColl->GetCount() == 3);
    for (FdoInt32 i = 0; i < pColl->GetCount(); i++)
    {
        FdoParameter*   pParam = dynamic_cast<FdoParameter*>(pColl->GetItem(i));

        FDO_CPPUNIT_ASSERT(pParam != NULL);
        FDO_CPPUNIT_ASSERT(wcscmp(pParam->GetName(), aszNames[i]) == 0);
        pParam->Release();
    }
    pColl->Release();

    // test the setters and getters
    pIn1->SetPropertyName(pIdent);
    pIdentTest = pIn1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // check the other constructor
    FdoInCondition*         pIn2 = FdoInCondition::Create(pIdent, aszNames, 1);
    FdoValueExpressionCollection* pColl2 = pIn2->GetValues();
    FDO_CPPUNIT_ASSERT(pColl2->GetCount() == 1);
    pColl2->Release();
    pIdentTest = pIn2->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // test the setters and getters with null values
    pIn2->SetPropertyName(NULL);
    FDO_CPPUNIT_ASSERT(pIn2->GetPropertyName() == NULL);

    // check the constructor with null values
    FdoInCondition*         pIn3 = FdoInCondition::Create(pIdent, NULL, 0);
    FdoValueExpressionCollection* pColl3 = pIn3->GetValues();
    FDO_CPPUNIT_ASSERT(pColl3->GetCount() == 0);
    pColl3->Release();

    // release everything
    pIdent->Release();
    pIn1->Release();
    pIn2->Release();
    pIn3->Release();
}

void FilterTest::testNullCondition()
{
    // create some data for the test.
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"Sham.A.Lam.A.Ding.Dong");

    // check constructor.
    FdoNullCondition*       pNull = FdoNullCondition::Create(pIdent);
    FdoIdentifier*  pIdentTest = pNull->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();
	
    // test the setters and getters
    pNull->SetPropertyName(NULL);
    FDO_CPPUNIT_ASSERT(pNull->GetPropertyName() == NULL);

    // check the other constructor
    FdoNullCondition*       pNull2 = FdoNullCondition::Create(L"DooWap");
    pIdentTest = pNull2->GetPropertyName();
    FDO_CPPUNIT_ASSERT(wcscmp(pIdentTest->GetText(), L"DooWap") == 0);
    pIdentTest->Release();

    // release everything
    pNull->Release();
    pNull2->Release();
    pIdent->Release();
}

void FilterTest::testSpatialCondition()
{
    // create some data for the test.
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"TestIdent");
	FdoByteArray*			pFGF1 = FdoByteArray::Create((FdoByte*)"testing123", 10);
    FdoGeometryValue*       pGVal1 = FdoGeometryValue::Create(pFGF1);
	FdoByteArray*			pFGF2 = FdoByteArray::Create((FdoByte*)"testtesttest", 12);
    FdoGeometryValue*       pGVal2 = FdoGeometryValue::Create(pFGF2);

    FdoIdentifier*      pIdentTest;
    FdoExpression*      pGeomTest;

    // check constructor.
    FdoSpatialCondition*    pSC1 = FdoSpatialCondition::Create(L"StringName", FdoSpatialOperations_Within, pGVal1);
    pGeomTest = pSC1->GetGeometry();
    FDO_CPPUNIT_ASSERT(pGeomTest == pGVal1);
    pGeomTest->Release();
    FDO_CPPUNIT_ASSERT(pSC1->GetOperation() == FdoSpatialOperations_Within);
    pIdentTest = pSC1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(wcscmp(pIdentTest->GetText(), L"StringName") == 0);
    pIdentTest->Release();

    // test the setters and getters
    pSC1->SetGeometry(pGVal2);
    pSC1->SetOperation(FdoSpatialOperations_Crosses);
    pSC1->SetPropertyName(pIdent);
    pGeomTest = pSC1->GetGeometry();
    FDO_CPPUNIT_ASSERT(pGeomTest == pGVal2);
    pGeomTest->Release();
    FDO_CPPUNIT_ASSERT(pSC1->GetOperation() == FdoSpatialOperations_Crosses);
    pIdentTest = pSC1->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // test the setters and getters with null values
    pSC1->SetPropertyName(NULL);
    pSC1->SetGeometry(NULL);
    FDO_CPPUNIT_ASSERT(pSC1->GetGeometry() == NULL);
    FDO_CPPUNIT_ASSERT(pSC1->GetPropertyName() == NULL);

    // check the other constructor
    FdoSpatialCondition*    pSC2 = FdoSpatialCondition::Create(pIdent, FdoSpatialOperations_Crosses, pGVal2);
    pGeomTest = pSC2->GetGeometry();
    FDO_CPPUNIT_ASSERT(pGeomTest == pGVal2);
    pGeomTest->Release();
    FDO_CPPUNIT_ASSERT(pSC2->GetOperation() == FdoSpatialOperations_Crosses);
    pIdentTest = pSC2->GetPropertyName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    // release everything
    pSC1->Release();
    pSC2->Release();
    pIdent->Release();
	pFGF1->Release();
	pFGF2->Release();
    pGVal1->Release();
    pGVal2->Release();
}

void FilterTest::testUnaryLogicalOperator()
{
    // create some data for the test.
    FdoDataValue*           pDVal = FdoStringValue::Create(L"abc\"abc");
    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"'this.is.a.test");
    FdoComparisonCondition* pCompare1 = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pDVal);
    FdoComparisonCondition* pCompare2 = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_GreaterThan, pDVal);

    // check constructor.
    FdoUnaryLogicalOperator*    pNot = FdoUnaryLogicalOperator::Create(pCompare1, FdoUnaryLogicalOperations_Not);
    FDO_CPPUNIT_ASSERT(pNot->GetOperation() == FdoUnaryLogicalOperations_Not);
    FdoFilter*  pFilter = pNot->GetOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare1);
    pFilter->Release();

    // set to null and check
    pNot->SetOperand(NULL);
    FDO_CPPUNIT_ASSERT(pNot->GetOperand() == NULL);

    // set to operand and check.
    pNot->SetOperand(pCompare2);
    pFilter = pNot->GetOperand();
    FDO_CPPUNIT_ASSERT(pFilter == pCompare2);
    pFilter->Release();

    // release everything
    pDVal->Release();
    pIdent->Release();
    pCompare1->Release();
    pCompare2->Release();
    pNot->Release();
}

void FilterTest::testComputedIdentifier()
{
    FdoPtr<FdoIdentifier>pWidth = FdoIdentifier::Create(L"Width");
    FdoPtr<FdoIdentifier>pLength = FdoIdentifier::Create(L"Length");

    FdoPtr<FdoBinaryExpression>pArea = FdoBinaryExpression::Create(pWidth, FdoBinaryOperations_Multiply, pLength);

    FdoPtr<FdoComputedIdentifier>pCompIdent = FdoComputedIdentifier::Create(L"Area", pArea);
    FdoPtr<FdoDataValue>pDVal = FdoInt32Value::Create(123);

    // test the constructor
    FdoPtr<FdoComparisonCondition>pCompare = FdoComparisonCondition::Create(pCompIdent, FdoComparisonOperations_EqualTo, pDVal);
    printf("Filter = %ls\n",pCompare->ToString() );
}


void FilterTest::testConditionRoundtrip()
{

	// Use this identifier 
	FdoPtr<FdoIdentifier>          pIdent = FdoIdentifier::Create(L"Identifier with spaces");

	/// NullCondition
	FdoPtr<FdoNullCondition>       pNull = FdoNullCondition::Create(pIdent);
	doRoundtrip( "NullCondition", pNull );

	/// ComparisonCondition
	FdoPtr<FdoDataValue>           pDVal = FdoInt32Value::Create(123);
	FdoComparisonCondition* pCompare = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pDVal);
	doRoundtrip( "ComparisonCondition", pCompare );

	/// IN condition
    FdoPtr<FdoInCondition>         pIn = FdoInCondition::Create(pIdent, aszNames, 3);
	doRoundtrip( "InCondition", pIn );

	double*		ordsXYZ = new double[3];
	ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;

	FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIGeometry>			pnt = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ);
    FdoPtr<FdoByteArray> fgfByteArray = gf->GetFgf(pnt);
	FdoPtr<FdoGeometryValue>		pGVal1 = FdoGeometryValue::Create(fgfByteArray);


	delete [] ordsXYZ;

	/// Distance condition
	FdoPtr<FdoDistanceCondition>   pDistance = FdoDistanceCondition::Create(pIdent, FdoDistanceOperations_Beyond, pGVal1, 3.14);
	doRoundtrip( "DistanceCondition", pDistance );


	/// Spatial Condition
    FdoPtr<FdoSpatialCondition>    pSpatial = FdoSpatialCondition::Create(pIdent, FdoSpatialOperations_Within, pGVal1);
	doRoundtrip( "SpatialCondition", pSpatial );
}

void FilterTest::doRoundtrip( char *testName, FdoFilter *filter)
{
	try
	{
		FdoString * str1 = filter->ToString ();
		FdoPtr<FdoFilter> flt2 = FdoFilter::Parse (str1);  
		FdoString * str2 = flt2->ToString();

		CPPUNIT_ASSERT_MESSAGE( testName, wcscmp(str1, str2) == 0);
	}
	catch( FdoException *e)
	{
		printf("ERROR: FdoFilter::Parse() failed - %s\n", testName);
		e->Release();

		CPPUNIT_FAIL("Parsing failed");
	}
}

