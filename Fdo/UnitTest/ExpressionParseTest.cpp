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
#include "ExpressionParseTest.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#include "TestProcessor.h"
#endif

#define DEBUG_DETAIL	1

#include "UnitTestUtil.h"
#include "malloc.h"  // for alloca()

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (ExpressionParseTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ExpressionParseTest, "ExpressionParseTest");

ExpressionParseTest::ExpressionParseTest(void)
{
}

ExpressionParseTest::~ExpressionParseTest(void)
{
}

void ExpressionParseTest::setUp()
{
    // nothing to do at setup time... yet
}

// parse an FDO expression we expect to fail
void ExpressionParseTest::ParseExpressionToNull(FdoString* pwzExpression)
{
#ifdef DEBUG_DETAIL
	// input expression to parse
	printf("[%S]\n", pwzExpression);
#endif

	// get root node of expression parse tree
    FdoExpression*  pExpression = NULL;
    try
    {
    	pExpression = FdoExpression::Parse(pwzExpression);
    }
    catch(FdoException* exception)
    {
#ifdef DEBUG_DETAIL
        printf("caught FdoException=%S\n", exception->GetExceptionMessage());
#endif
        exception->Release();
        return;
	}
    CPPUNIT_ASSERT_MESSAGE("FdoExpression::Parse() should have returned NULL!\n", pExpression == NULL);
}

// parse an FDO expression
void ExpressionParseTest::ParseExpression(FdoString* pwzExpression, FdoString* pwzResult)
{
#ifdef DEBUG_DETAIL
    // input expression to parse
	printf("[%S]\n", pwzExpression);
#endif

	// get root node of expression parse tree
	FdoExpression* pExpression = FdoExpression::Parse(pwzExpression);
	CPPUNIT_ASSERT_MESSAGE("FdoExpression::Parse() returned NULL!\n", pExpression != NULL);

	// output back to string if successful
	FdoString* pwzOut = pExpression->ToString();
	CPPUNIT_ASSERT_MESSAGE("FdoExpression::ToString() returned NULL!\n", pwzOut != NULL);

	// verify that intput and output expression strings match accordingly
    char    *szError = (char*)alloca(50 + 4*(wcslen(pwzOut) + (pwzResult!=NULL ? wcslen(pwzResult) : wcslen(pwzExpression))));
    if (pwzResult == NULL)
    {
        sprintf(szError, "Parse/ToString do not match!\n\t<%S> should be <%S>\n", pwzOut, pwzExpression);
        CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(pwzOut, pwzExpression) == 0);
    }
    else
    {
        sprintf(szError, "Parse/ToString do not match!\n\t<%S> should be <%S>\n", pwzOut, pwzResult);
        CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(pwzOut, pwzResult) == 0);
    }

#ifdef DEBUG_DETAIL
    printf("<%S>\n\n", pwzOut);
#endif
	pExpression->Release();
} 

// test expressions other than FGFT geometry values
void ExpressionParseTest::testExpressions()
{
	// trivial test
//	ParseExpression(L" \"literal\" \'literal\' :parameter identifier");
//	ParseExpression(L" + a+b a - 2.3 a -2.3 a-2.3 a + 2.3 a +2.3 a+2.3");
	ParseExpression(L"4");
	ParseExpression(L"4.5");
	ParseExpression(L"TRUE");
	ParseExpression(L"true", 
        L"TRUE");
	ParseExpression(L"True", 
        L"TRUE");
	ParseExpression(L"4+4.5");
	ParseExpression(L"4.5+5.4*6.6-3.2/12");
	ParseExpression(L"-3.4");
	ParseExpression(L"-3.4-5.6");
	ParseExpression(L"-(3.4*5.6)");
	ParseExpression(L"-'abc'",
		L"-('abc')");
	ParseExpression(L"-'abc'-'abc'",
		L"-('abc')-'abc'");
	ParseExpression(L"(2*6)-(3*5)",
		L"2*6-3*5");
	ParseExpression(L"-TRUE", 
		L"-(TRUE)");
	ParseExpression(L"'abc'");
	ParseExpression(L"'abc''def'");
	ParseExpression(L"a+b*34/12");
	//ParseExpression(L"“a", L"a");
	//ParseExpression(L"‘a", L"'a'");
	ParseExpression(L"12345678901234567");
#ifdef _WIN32
	ParseExpression(L"123456789012345678901", 
        L"1.23456789012346e+020");
#else
	ParseExpression(L"123456789012345678901", 
        L"1.23456789012346e+20");
#endif
	ParseExpression(L"1.2e13", 
        L"12000000000000");
	ParseExpression(L"-2 --2 +2 ++2", 
        L"-2--2+2+2");

	// dates 
	ParseExpression(L"DATE '1951-12-24'");
	ParseExpression(L"DATE '1971-12-24'");

	// time is meaningless since it would be stored since Jan 1, 1970
	ParseExpression(L"TIME '12:01:30'");
	ParseExpression(L"TIMESTAMP '2003-10-23 11:00:02'");

	// compound expressions (some from Oracle manual)
	ParseExpression(L"SQRT(144)");
	ParseExpression(L"SQRT('abc')");
	ParseExpression(L"SQRT(SQRT(2)+3)*2");
	ParseExpression(L"my_fun(TO_CHAR('abc','DD-MMM-YY'))", 
        L"my_fun(TO_CHAR('abc', 'DD-MMM-YY'))");
	ParseExpression(L":EnterName");
	ParseExpression(L":'Enter Name'");
	ParseExpression(L"MIN(a,b,2*3+45.67,d,e)", 
        L"MIN(a, b, 2*3+45.67, d, e)");
	ParseExpression(L"'CLARK' || 'SMITH'", 
        L"'CLARK'+'SMITH'");
	ParseExpression(L"'Lewis'+'Clark'");
	ParseExpression(L"LENGTH('MOOSE') * 57", 
        L"LENGTH('MOOSE')*57");
	ParseExpression(L"Pipes.state");
	ParseExpression(L"\"Pipes . state\"");
	ParseExpression(L"\"%one two thre#EE\"");
	ParseExpression(L"'%one two thre#EE'");
#ifdef _WIN32
    ParseExpression(L"2.3e400", L"1.#INF");
#else
    ParseExpression(L"2.3e400", L"inf");
#endif
	ParseExpression(L"(2+3)*12");
	ParseExpression(L"sqrt(144)+(12-32/12)", 
        L"sqrt(144)+12-32/12");

    ParseExpression(L"( Width*Height ) AS Area");
    
    FdoPtr<FdoExpression> exprTest = (FdoExpression*)FdoExpression::Parse(L"Lower(layer) AS testFld");
    if (dynamic_cast<FdoComputedIdentifier*>(exprTest.p) == NULL)
        UnitTestUtil::FailOnException( FdoException::Create(L"Parsing \"Lower(layer) AS testFld\" failed!") );
    try
    {
        exprTest = (FdoExpression*)FdoExpression::Parse(L"Lower(layer) * - AS testFld");
        UnitTestUtil::FailOnException( FdoException::Create(L"Parsing \"Lower(layer) * - AS testFld\" should fail!"));
    }
    catch(FdoException* ex)
    {
        printf("Expected exception: %ls\n", ex->GetExceptionMessage());
        ex->Release();
    }
}

void ExpressionParseTest::testFGFT()
{
	// negative value tests
	ParseExpression(L"GeomFromText('POINT XYZ (-1051999.72951478 -.232884005 -0.00000001)')",
		L"GeomFromText('POINT XYZ (-1051999.72951478 -0.232884005 -0.00000001)')");
	ParseExpression(L"GeomFromText('POINT XYZ (- 1051999.72951478 - .232884005 - 0.00000001)')",
		L"GeomFromText('POINT XYZ (-1051999.72951478 -0.232884005 -0.00000001)')");

	// Trivial example of how to get a geometry value from FGFT string.
	// Works for this special case because the parse tree has only one
	// node and the root is a FdoGeometryValue
	FdoGeometryValue* value = dynamic_cast<FdoGeometryValue*>(FdoExpression::Parse(L"GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')"));
#ifdef DEBUG_DETAIL
	printf("<%S>\n", value->ToString());
#endif
	CPPUNIT_ASSERT_MESSAGE("Trivial test failed!", wcscmp(value->ToString(), L"GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')") == 0);
	FDO_SAFE_RELEASE(value);
	ParseExpression(L"GeomFromText('POLYGON XYZ ((1051999.72951478 563999.232884005 0.00000001, 1055009.22168253 563999.232884005 0.00000001, 1055009.22168253 566002.587400644 0.00000001, 1051999.72951478 566002.587400644 0.00000001, 1051999.72951478 563999.232884005 0.00000001))')");
	ParseExpression(L"GeomFromText('POINT(0.0 0.0)')",
		L"GeomFromText('POINT (0 0)')");

	// Examples from Open FDO Simple Feature Specification for SQL
	ParseExpression(L"GeomFromText('POINT (10 10)')");
	ParseExpression(L"GeomFromText('LINESTRING (10 10, 20 20, 30 40)')");
	ParseExpression(L"GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')");
	ParseExpression(L"GeomFromText('MULTIPOINT (10 10, 20 20)')");
	ParseExpression(L"GeomFromText('MULTILINESTRING ((10 10, 20 20), (15 15, 30 15))')");
	ParseExpression(L"GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)), ((60 60, 70 70, 80 60, 60 60)))')");

	// other more complicated WKT samples
	// ... multiple linear rings
	ParseExpression(L"GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10), (11 11, 11 21, 21 21, 21 16, 11 11))')");
	// two exterior ring only polygons
	ParseExpression(L"GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)), ((60 60, 70 70, 80 60, 60 60)))')");
	// two rings in one polygon
	ParseExpression(L"GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)))')");
	// two rings in two polygons
	ParseExpression(L"GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)), ((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)))')");

	// geometry collection of WKT types
	ParseExpression(L"GeomFromText('GEOMETRYCOLLECTION (POINT (10 10),POINT (30 30), LINESTRING (15 15, 20 20))')",
		L"GeomFromText('GEOMETRYCOLLECTION (POINT (10 10), POINT (30 30), LINESTRING (15 15, 20 20))')");
//TODO:GC "Multi" types not yet supported in GEOMETRYCOLLECTION
//    ParseExpression(L"GeomFromText('GEOMETRYCOLLECTION (POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10)), MULTIPOINT (10 10, 20 20), MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)),((60 60, 70 70, 80 60, 60 60 ) )))')",
//		L"GeomFromText('NULL')");

	// Let's do a geometry collection within a geometry collection, just to make sure it's 
	// possible to parse, but there is no code to support it yet
	//TODO:GC geometry says bad parameter with nested geometry collections
	// see Fgf/MultiGeometry.cpp 	case FdoGeometryType_MultiGeometry:     // No nesting currently supported.
//	ParseExpression(L"GeomFromText('GEOMETRYCOLLECTION (POINT (10 10), GEOMETRYCOLLECTION(POINT (30 30)))')",
//		L"GeomFromText('NULL')");

	// Autodesk extensions FGFT
	// ... dimensionality types
	ParseExpression(L"GeomFromText('POINT (10 11)')");
	ParseExpression(L"GeomFromText('POINT XY (10 11)')",
        L"GeomFromText('POINT (10 11)')");
	ParseExpression(L"GeomFromText('POINT XYZ (10 11 12)')");
	ParseExpression(L"GeomFromText('POINT XYM (10 11 1.2)')");
	ParseExpression(L"GeomFromText('POINT XYZM (10 11 12 1.2)')");

	// .. geometry collection of extended types 
	ParseExpression(L"GeomFromText('GEOMETRYCOLLECTION (POINT xyz (10 11 12),POINT XYM (30 20 1.8), LINESTRING XYZM(1 2 3 4, 3 5 15, 3 20 20))')",
        L"GeomFromText('GEOMETRYCOLLECTION (POINT XYZ (10 11 12), POINT XYM (30 20 1.8), LINESTRING XYZM (1 2 3 4, 3 5 15 0, 3 20 20 0))')");

	// ... additional entity types
	ParseExpression(L"GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
	ParseExpression(L"GeomFromText('CURVESTRING (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
	ParseExpression(L"GeomFromText('CURVESTRING (0 0 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')",
		L"GeomFromText('CURVESTRING (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
	ParseExpression(L"GeomFromText('CURVESTRING XYZ (0 0 0 (LINESTRINGSEGMENT (10 10 1, 20 20 1, 30 40 1)))')");
	ParseExpression(L"GeomFromText('MULTICURVESTRING ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40))),(0 0 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40))))')",
		L"GeomFromText('MULTICURVESTRING ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40))), (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40))))')");
	ParseExpression(L"GeomFromText('MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300))))')");
	// this can throw an exception if it is not closed (defined by exact double == of first and last point
	ParseExpression(L"GeomFromText('CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40, 0 0))), (0 0 (LINESTRINGSEGMENT (10 10, 40 40, 90 90, 0 0))))')");
	ParseExpression(L"GeomFromText('CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))))')");
	ParseExpression(L"GeomFromText('MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))')");
	ParseExpression(L"GeomFromText('MULTICURVEPOLYGON (((10 10 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), ARC (20 15, 10 10))), (90 90 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))),((8 8 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), ARC (20 15, 8 8))), (90 90 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))))')",
		            L"GeomFromText('MULTICURVEPOLYGON (((10 10 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), CIRCULARARCSEGMENT (20 15, 10 10))), (90 90 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))), ((8 8 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), CIRCULARARCSEGMENT (20 15, 8 8))), (90 90 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))))')");

	// TODO: add more ParseExpressionToNull examples

	// ... missing , between rings causes NULL GeometryValue()
	// in ValueExpressionCollection, DistanceCondition, and SpatialCondition
	ParseExpressionToNull(L"GeomFromText('POLYGON ((10 10)(11 11)))')");
}



void ExpressionParseTest::testLargeExpressions()
{
    try
    {

        // Test large FGFT geometry expression:
        ParseExpression(L"GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT ("
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000,"
            L"10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000"
            L")))')"
            ,
            L"GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT (10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, "
            L"10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20)))')"
            );


        // Test large literal string expression:
        const long STRING_TEST_LENGTH = 20000;
        wchar_t veryLargeLiteralString[STRING_TEST_LENGTH];
        for (long i=0; i<STRING_TEST_LENGTH; i++)
        {
            if (i==0 || i==STRING_TEST_LENGTH-2)
                veryLargeLiteralString[i] = L'\'';
            else if (i==STRING_TEST_LENGTH-1)
                veryLargeLiteralString[i] = L'\0';
            else
                veryLargeLiteralString[i] = (wchar_t)(97 + i % 26);  // ascii-7 alphabet characters
        }
        ParseExpression(veryLargeLiteralString, veryLargeLiteralString);
    }
    catch (FdoException *e)
    {
        UnitTestUtil::FailOnException( e );
    }
}

void ExpressionParseTest::testBoundaryValues()
{
    try
    {
        // Test boundary value in binary expression

        FdoPtr<FdoDataValue> pVal = FdoByteValue::Create((FdoByte) 0);
        FdoPtr<FdoDataValue> pVal2 = roundTripBoundaryValue(pVal);
        FdoInt32Value* pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == 0);   
    
        pVal = FdoByteValue::Create(UCHAR_MAX);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == UCHAR_MAX);  

        pVal = FdoInt16Value::Create(SHRT_MIN);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == SHRT_MIN);   
    
        pVal = FdoInt16Value::Create(SHRT_MAX);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == SHRT_MAX);  

        pVal = FdoInt32Value::Create(INT_MIN);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MIN);   
    
        pVal = FdoInt32Value::Create(INT_MAX);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MAX);  

        pVal = FdoInt64Value::Create(LLONG_MIN);
        pVal2 = roundTripBoundaryValue(pVal);
        FdoInt64Value* pLLVal2 = dynamic_cast<FdoInt64Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLLVal2 != NULL);  
        CPPUNIT_ASSERT(pLLVal2->GetInt64() == LLONG_MIN);   

        pVal = FdoInt64Value::Create(LLONG_MAX);
        pVal2 = roundTripBoundaryValue(pVal);
        pLLVal2 = dynamic_cast<FdoInt64Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLLVal2 != NULL);   
        CPPUNIT_ASSERT(pLLVal2->GetInt64() == LLONG_MAX);   

        pVal = FdoInt64Value::Create(INT_MIN);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MIN);   
    
        pVal = FdoInt64Value::Create(INT_MAX);
        pVal2 = roundTripBoundaryValue(pVal);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoDataValue*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MAX);   
    }
    catch (FdoException *e)
    {
        UnitTestUtil::FailOnException( e );
    }
}

FdoPtr<FdoDataValue> ExpressionParseTest::roundTripBoundaryValue( FdoDataValue* pVal)
{
    FdoPtr<FdoParameter> pParam = FdoParameter::Create(L"AndSoIsThis");
    FdoPtr<FdoBinaryExpression> pBExpr = FdoBinaryExpression::Create(pVal, FdoBinaryOperations_Multiply, pParam);
        
    FdoStringP exprString = pBExpr->ToString();
        
    FdoPtr<FdoExpression> pExpression = FdoExpression::Parse(exprString);
    CPPUNIT_ASSERT( pExpression != NULL );

    FdoBinaryExpression* pBExpr2 = dynamic_cast<FdoBinaryExpression*>((FdoExpression*) pExpression);
    CPPUNIT_ASSERT( pBExpr2 != NULL );
    
    FdoPtr<FdoExpression> pVal2 = pBExpr2->GetLeftExpression();
    CPPUNIT_ASSERT( pVal2 != NULL );

    FdoPtr<FdoDataValue> pDVal2 = FDO_SAFE_ADDREF(dynamic_cast<FdoDataValue*>((FdoExpression*) pVal2));
    CPPUNIT_ASSERT( pDVal2 != NULL );

    return pDVal2;
}
