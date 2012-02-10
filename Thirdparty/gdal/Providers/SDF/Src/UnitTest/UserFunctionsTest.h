
// Copyright (C) 2004-2007  Autodesk, Inc.
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

class UserFunctionsTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(UserFunctionsTest);
    CPPUNIT_TEST(selectUserDefinedNonAggregatesTest);
    CPPUNIT_TEST(selectUserDefinedAggregatesTest);
    CPPUNIT_TEST_SUITE_END();

    // You may define any private variables that are used in your test
    // suite here.
private:
    
public:

    UserFunctionsTest();
    virtual ~UserFunctionsTest();
   
    void setUp();
    void tearDown();
    void selectUserDefinedNonAggregatesTest();
    void selectUserDefinedAggregatesTest();
};

#endif


