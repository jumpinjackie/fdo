
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

// This code is provided just for an example. You should delete it from
// the project once you are ready to compile your test suite.  More information
// about using CppUnit can be found on the CppUnit homepage.
// http://cppunit.sourceforge.net/doc/lastest/index.html

// List of all modules can be found here:
// http://cppunit.sourceforge.net/doc/lastest/modules.html

// More information on the test fixture macros can be found at the following:
// http://cppunit.sourceforge.net/doc/lastest/group___writing_test_fixture.html

/*
CPPUNIT_TEST_SUITE(ATestFixtureType) 
  Begin test suite. 
 
CPPUNIT_TEST_SUB_SUITE(ATestFixtureType, ASuperClass) 
  Begin test suite (includes parent suite). 

CPPUNIT_TEST_SUITE_END() 
  End declaration of the test suite. 

CPPUNIT_TEST_SUITE_END_ABSTRACT() 
  End declaration of an abstract test suite. 

CPPUNIT_TEST_SUITE_ADD_TEST(test)   context.addTest( test ) 
  Add a test to the suite (for custom test macro). 
 
CPPUNIT_TEST(testMethod) 
  Add a method to the suite. 
 
CPPUNIT_TEST_EXCEPTION(testMethod, ExceptionType) 
  Add a test which fail if the specified exception is not caught. 
 
CPPUNIT_TEST_FAIL(testMethod)   CPPUNIT_TEST_EXCEPTION( testMethod, CPPUNIT_NS::Exception ) 
  Adds a test case which is excepted to fail. 

CPPUNIT_TEST_SUITE_ADD_CUSTOM_TESTS(testAdderMethod)   testAdderMethod( context ) 
  Adds some custom test cases. 

CPPUNIT_TEST_SUITE_PROPERTY(APropertyKey, APropertyValue) 
  Adds a property to the test suite builder context. 
*/ 


#ifndef TESTEXAMPLE_H
#define TESTEXAMPLE_H

#include "cppunit/extensions/HelperMacros.h"
#include "Fdo.h"
#include "UnitTestUtil.h"

// NOTE: Define __CPPUNIT_MFC_APP in order to have the CPPUNIT UI appear... 
#ifdef __CPPUNIT_MFC_APP
    #ifdef WIN32
	    #ifdef _DEBUG
		    #pragma comment(lib, "testrunnerd.lib")
	    #else
		    #pragma comment(lib, "testrunner.lib")
	    #endif
    #endif
#endif

class MasterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MasterTest);
	CPPUNIT_TEST(rtreeCacheTest); // Leave this test first as most of the other tests depend on insertTest
	CPPUNIT_TEST(testConstraints); 
	CPPUNIT_TEST(insertTest);
    CPPUNIT_TEST(rtreeFilter);
    CPPUNIT_TEST(spatialFilter);
    CPPUNIT_TEST(spatialInsideFilter);
    CPPUNIT_TEST(stringFilter);
    CPPUNIT_TEST(computedPropTest);
    CPPUNIT_TEST(keyFilterBeforeDelete);
    CPPUNIT_TEST(updateTest);
    CPPUNIT_TEST(keyFilterBeforeDelete);
    CPPUNIT_TEST(deleteTest);
    CPPUNIT_TEST(keyFilterAfterDelete);
    CPPUNIT_TEST(dataTypeKeyFilter);
    CPPUNIT_TEST(coordSysTest);
    CPPUNIT_TEST(selectDistinctTests);
    CPPUNIT_TEST(selectAggregatesTest);
    CPPUNIT_TEST(selectAggregatesFailureTests);
	CPPUNIT_TEST(selectSpatialExtentsTest);
	CPPUNIT_TEST(concurencyTest);
	CPPUNIT_TEST(noGeomObject); 
	CPPUNIT_TEST(numberFilter); 
	CPPUNIT_TEST(inFilter); 
	CPPUNIT_TEST(inFilterOptimize); 
	CPPUNIT_TEST(likeFilter);
	CPPUNIT_TEST(orFilter);
	CPPUNIT_TEST(dateFilter); 
	CPPUNIT_TEST(concurencyRtreeTest);
    CPPUNIT_TEST(descReadOnly);
    CPPUNIT_TEST(test_aggregates_datetime_string);
	CPPUNIT_TEST(selectFunctionTest);
	CPPUNIT_TEST(testSpecialClassNames); 
    CPPUNIT_TEST(selectAggregatesSpatialExtentsTest);
    CPPUNIT_TEST_SUITE_END();

    // You may define any private variables that are used in your test
    // suite here.
private:

    FdoIConnection* CreateConnection();
    void openConnection(FdoIConnection* conn, const wchar_t* path, bool readonly = false);
    
public:

    MasterTest();
    virtual ~MasterTest();
   

    void setUp();
    void tearDown();
	void insertTest() { UnitTestUtil::CreateData(true); }
    void deleteTest();
    void updateTest();
    void keyFilterBeforeDelete();
    void keyFilterAfterDelete();
    void dataTypeKeyFilter();
    void stringFilter();
    void rtreeFilter();
    void spatialFilter();
    void spatialInsideFilter();
    void coordSysTest();
    void computedPropTest();
    void selectDistinctTests();
    void selectAggregatesTest();
    void selectAggregatesFailureTests();
	void concurencyTest();
	void concurencyRtreeTest();
    void selectSpatialExtentsTest();
    void descReadOnly();
    void test_aggregates_datetime_string();
	void selectFunctionTest();

	void rtreeCacheTest();
	void testConstraints();
	void noGeomObject();
	void numberFilter();
	void inFilter();

    // Tests that the query optimizer handles in conditions properly.
	void inFilterOptimize();
	void likeFilter();
	void orFilter();
	void dateFilter();
	void testSpecialClassNames();
	void selectAggregatesSpatialExtentsTest();

	void CreateEmptyShpFileWithConstraints(FdoIConnection* conn);

    void inFilterOptimize_CreateData( FdoIConnection* conn );
    void inFilterOptimize_Select( 
        FdoIConnection* conn,
        FdoString* className, 
        FdoString* filterString, 
        FdoString* id1, 
        FdoDataType type1,
        FdoString** expected1,
        FdoString* id2 = NULL, 
        FdoDataType type2 = FdoDataType_String,
        FdoString** expected2 = NULL 
    );
    FdoStringP inFilterOptimize_GetIdValue( 
        FdoIFeatureReader* rdr,
        FdoString* propName, 
        FdoDataType propType
    );

    void dataTypeFilter_CreateData( FdoIConnection* conn );
    void dataTypeFilter_CreateClass( FdoClassesP pClasses, FdoString* className, FdoDataType idType );
};

#endif

