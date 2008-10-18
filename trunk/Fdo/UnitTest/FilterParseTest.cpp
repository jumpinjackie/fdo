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
#include "FilterParseTest.h"
#ifdef _DEBUG
//#define DEBUG_TREE	1
//#define DEBUG_DETAIL	1
#include "TestProcessor.h"
#endif

#include "UnitTestUtil.h"

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (FilterParseTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FilterParseTest, "FilterParseTest");

FilterParseTest::FilterParseTest(void)
{
}

FilterParseTest::~FilterParseTest(void)
{
}

void FilterParseTest::setUp()
{
    // nothing to do at setup time... yet
}

// parse an FDO Filter that we except to return NULL because of error
void FilterParseTest::ParseFilterToNull(FdoString* pwzFilter)
{
#ifdef DEBUG_DETAIL
    // input expression to parse
	printf("[%S]\n", pwzFilter);
#endif

    // get root node of expression parse tree
    FdoFilter*  pFilter = NULL;
    try
    {
    	pFilter = FdoFilter::Parse(pwzFilter);
    }
    catch(FdoException* exception)
    {
// Note: commenting out this DEBUG_DETAIL allows you to confirm that
// release build reporting of exceptions does not include file and line numbers
#ifdef DEBUG_DETAIL
        PrintException(exception);
#endif
        exception->Release();
        return;
    }
    CPPUNIT_ASSERT_MESSAGE("FdoFilter::Parse() should have returned NULL!\n", pFilter == NULL);
}

// parse an FDO Filter
void FilterParseTest::ParseFilter(FdoString* pwzFilter, FdoString* pwzResult)
{
#ifdef DEBUG_DETAIL
    // input expression to parse
	printf("[%S]\n", pwzFilter);
#endif

	// get root node of expression parse tree
	FdoFilter* pFilter = FdoFilter::Parse(pwzFilter);
    CPPUNIT_ASSERT_MESSAGE("FdoFilter::Parse() returned NULL!\n", pFilter != NULL);

	// output back to string if successful
	FdoString* pwzOut = pFilter->ToString();
	CPPUNIT_ASSERT_MESSAGE("FdoFilter::ToString() returned NULL!\n", pwzOut != NULL);

    char    szError[512];
    if (pwzResult == NULL)
    {
        sprintf(szError, "Parse/ToString do not match!\n\t<%S> should be <%S>\n", pwzOut, pwzFilter);
        CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(pwzOut, pwzFilter) == 0);
    }
    else
    {
        sprintf(szError, "Parse/ToString do not match!\n\t<%S> should be <%S>\n", pwzOut, pwzResult);
        CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(pwzOut, pwzResult) == 0);
    }

#ifdef DEBUG_TREE
	UnitTestProcessor    testProc;
    pFilter->Process(&testProc);
#endif
#ifdef DEBUG_DETAIL
    printf("<%S>\n\n", pwzOut);
#endif
	pFilter->Release();
}

void FilterParseTest::test()
{
#ifdef DEBUG_TREE
	// clear the ...... from other tests to new line
	printf("\n");
#endif
    
   

	ParseFilter(L"colourIndex > -a", L"colourIndex > -(a)");
	ParseFilter(L"colourIndex > -(5)");
	ParseFilter(L"colourIndex > -5");
	ParseFilter(L"colourIndex < -5");
	ParseFilter(L"colourIndex >= -5");
	ParseFilter(L"colourIndex <= -5");
	ParseFilter(L"\"colour$Index\" > -a", L"\"colour$Index\" > -(a)");
	ParseFilter(L"(Id = 1 and Name = 'Fred') or (Id = 2 and Name = 'John')",
		L"Id = 1 AND Name = 'Fred' OR Id = 2 AND Name = 'John'");

	// identifiers with special characters
	ParseFilter(L"\"A\"\"B\" = '123'");
	ParseFilter(L"\"@#$%OR\" = '123'");	// DID 550139
	ParseFilter(L"\"OR\" = '123'");
	ParseFilter(L"\"or\" = '123' OR \"Or\" = 'abc'");

	// grammatical nightmares
	ParseFilterToNull(L"1 12345678901234567 1.234 1.2e13 -2 --2 +2 ++2");
	ParseFilterToNull(L"+ - * / < <= = == >= > : || @");
	ParseFilterToNull(L" AND BEYOND BOUNDINGBOX COMPARE CONTAINS COVEREDBY CROSSES DATE DAY DISJOINT DISTANCE ENVELOPEINTERSECTS EQUALS FALSE HOUR IN INSIDE INTERSECTS LIKE MINUTE MONTH NOT NULL OR OVERLAPS SECOND SPATIAL TIME TIMESTAMP TOUCHES TRUE WITHIN YEAR");
	ParseFilterToNull(L" And beyond BoundingBox Compare conTAINS coveredby");
	ParseFilterToNull(L" \"missing quote");
//?	ParseFilterToNull(L"tooooolonnnnngggggidentifierakdadjsaljdasljlasjdsljdslj");
	ParseFilterToNull(L"\"tooooooolonnngggstringliteralsasad;sklad;sls                                                                                                                                                                                                              ");
	ParseFilterToNull(L"+=4=");

	ParseFilter(L"a<4", L"a < 4");
	ParseFilter(L"a<4 or col= 2",
        L"a < 4 OR col = 2");
	ParseFilter(L"a = b OR c = d");
	ParseFilter(L"a = b AND c = d");
	ParseFilter(L"a = b OR c = d or a = b AND c = d",
        L"a = b OR c = d OR a = b AND c = d");
	ParseFilter(L"a = b AND c = d or a = b AND c = d",
        L"a = b AND c = d OR a = b AND c = d");
	ParseFilter(L"a = b");
	ParseFilter(L"a <> b");
	ParseFilter(L"a != b",
		L"a <> b");
	ParseFilter(L"a < b");
	ParseFilter(L"a <= b");
	ParseFilter(L"a > b");
	ParseFilter(L"a >= b");
	ParseFilter(L"a LIKE 'abc*'");
	ParseFilter(L"\"Feature Number\" = 4642");

	// Sascha Nikolayev sample
	ParseFilter(L"col1 > 10 and col2 in (1,2,3,4)",
        L"col1 > 10 AND col2 IN (1, 2, 3, 4)");

    // Example from Vinay.  Identifier were only returning their name, not 
    // the fully qualified text.
    ParseFilter(L"NOT Entity.Color NULL");
    ParseFilter(L"Entity.Color IN ('Red', 'Blue', 'Green')");

	// Examples from FDO Expressions and FiltersRpt.doc
	// Hence a filter to select roads with four or more lanes might look like:
	ParseFilter(L"Lanes >= 4");

	// Similarly in the Civil model a filter to select all PipeNetworks that
	// have at least one Pipe in the proposed state might look like:
	ParseFilter(L"Pipes.state = 'proposed'");

	// Also using the Civil model a filter to select all existing Parcels
	// whose owner contains the text "Smith" might look something like:
	ParseFilter(L"state = 'existing' and owner like '%Smith%'",
        L"state = 'existing' AND owner LIKE '%Smith%'");

	// Likewise a filter to select all Parcels that are either affected
	// or encroached upon by some change might look like:
	ParseFilter(L"state in ('affected', 'encroached')",
        L"state IN ('affected', 'encroached')");

	ParseFilter(L"state in (:'Enter State', 'encroached', TRUE, 123.333, 14)",
        L"state IN (:'Enter State', 'encroached', TRUE, 123.333, 14)");

	// Distance Conditions
	ParseFilter(L"feature beyond GeomFromText('POINT(0.0 0.0)') 12.0",
        L"feature BEYOND GeomFromText('POINT (0 0)') 12");
	ParseFilter(L"feature withindistance GeomFromText('POINT(0.0 0.0)') 12",
        L"feature WITHINDISTANCE GeomFromText('POINT (0 0)') 12");
	ParseFilter(L"feature withindistance GeomFromText('POINT(0.0 0.0)') 12.0",
        L"feature WITHINDISTANCE GeomFromText('POINT (0 0)') 12");

	// Spatial Conditions
	ParseFilter(L"feature contains GeomFromText('POINT(0.0 0.0)')",
        L"feature CONTAINS GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature crosses GeomFromText('POINT(0.0 0.0)')",
        L"feature CROSSES GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature disjoint GeomFromText('POINT(0.0 0.0)')",
        L"feature DISJOINT GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature equals GeomFromText('POINT(0.0 0.0)')",
        L"feature EQUALS GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature intersects GeomFromText('POINT(0.0 0.0)')",
        L"feature INTERSECTS GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature overlaps GeomFromText('POINT(0.0 0.0)')",
        L"feature OVERLAPS GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature touches GeomFromText('POINT(0.0 0.0)')",
        L"feature TOUCHES GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature within GeomFromText('POINT(0.0 0.0)')",
        L"feature WITHIN GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature coveredby GeomFromText('POINT(0.0 0.0)')",
        L"feature COVEREDBY GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature inside GeomFromText('POINT(0.0 0.0)')",
        L"feature INSIDE GeomFromText('POINT (0 0)')");
	ParseFilter(L"feature envelopeintersects GeomFromText('POINT(0.0 0.0)')",
        L"feature ENVELOPEINTERSECTS GeomFromText('POINT (0 0)')");

	ParseFilter(L"feature touches GeomFromText('POINT(0.0 0.0)') AND feature beyond GeomFromText('POINT(0.0 0.0)') 12.0 AND state = 'troubled'",
        L"feature TOUCHES GeomFromText('POINT (0 0)') AND feature BEYOND GeomFromText('POINT (0 0)') 12 AND state = 'troubled'");

    ParseFilter(L"( NumberOfLanes*Speed ) AS Capacity, Capacity > 200");

    ParseFilter(L"( Width*Length ) AS Area, Area > 100 AND Area < 1000");
    ParseFilter(L"( (Width+Length)*2 ) AS perimeter, perimeter > 100 AND perimeter < 1000");
    ParseFilter(L"( (Width+Length)*2 ) AS perimeter, ( Width*Length ) AS Area, Area > 1000 OR perimeter < 100");
    ParseFilter(L"( (Width+Length)*2 ) AS perimeter, ( Width*Length ) AS Area, Area > perimeter");
}

void FilterParseTest::testBoundaryValues()
{
    try
    {
        // Test boundary values for "in" filter
    
        FdoPtr<FdoIdentifier> ident = FdoIdentifier::Create( L"amount" );

        FdoPtr<FdoValueExpressionCollection> expressions = FdoValueExpressionCollection::Create();

        expressions->Add( FdoByteValue::Create(0) );
        expressions->Add( FdoByteValue::Create(UCHAR_MAX) );
        expressions->Add( FdoInt16Value::Create(SHRT_MIN) );
        expressions->Add( FdoInt16Value::Create(SHRT_MAX) );
        expressions->Add( FdoInt32Value::Create(INT_MIN) );
        expressions->Add( FdoInt32Value::Create(INT_MAX) );
        expressions->Add( FdoInt64Value::Create(LLONG_MIN) );
        expressions->Add( FdoInt64Value::Create(LLONG_MAX) );
        expressions->Add( FdoInt64Value::Create(INT_MIN) );
        expressions->Add( FdoInt64Value::Create(INT_MAX) );

        FdoPtr<FdoInCondition> in1 = FdoInCondition::Create( ident, expressions );
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(in1->ToString());
        CPPUNIT_ASSERT( filter != NULL );
        FdoInCondition* in2 = dynamic_cast<FdoInCondition*>((FdoFilter*) filter);
        CPPUNIT_ASSERT( in2 != NULL );

        FdoPtr<FdoValueExpressionCollection> expressions2 = in2->GetValues();
        CPPUNIT_ASSERT( expressions2->GetCount() == 10 );

        FdoPtr<FdoValueExpression> pVal2 = expressions2->GetItem(0);
        FdoInt32Value* pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == 0);   
    
        pVal2 = expressions2->GetItem(1);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == UCHAR_MAX);  

        pVal2 = expressions2->GetItem(2);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == SHRT_MIN);   
    
        pVal2 = expressions2->GetItem(3);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == SHRT_MAX);  

        pVal2 = expressions2->GetItem(4);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MIN);   
    
        pVal2 = expressions2->GetItem(5);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MAX);  

        pVal2 = expressions2->GetItem(6);
        FdoInt64Value* pLLVal2 = dynamic_cast<FdoInt64Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLLVal2 != NULL);   
        CPPUNIT_ASSERT(pLLVal2->GetInt64() == LLONG_MIN);   

        pVal2 = expressions2->GetItem(7);
        pLLVal2 = dynamic_cast<FdoInt64Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLLVal2 != NULL);   
        CPPUNIT_ASSERT(pLLVal2->GetInt64() == LLONG_MAX);   

        pVal2 = expressions2->GetItem(8);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MIN);   
    
        pVal2 = expressions2->GetItem(9);
        pLVal2 = dynamic_cast<FdoInt32Value*>((FdoValueExpression*) pVal2);
        CPPUNIT_ASSERT(pLVal2 != NULL);   
        CPPUNIT_ASSERT(pLVal2->GetInt32() == INT_MAX);   
    }
    catch (FdoException *e)
    {
        UnitTestUtil::FailOnException( e );
    }
}

