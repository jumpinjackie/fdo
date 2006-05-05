#ifndef _SELECTTEST_H_
#define _SELECTTEST_H_
#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_SELECTTEST_H
#define CPP_UNIT_SELECTTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo.h>

/* 
 * A test case for the schema.
 * Loads a set of Feature schemas, in logical-physical 
 * form, from a database.
 *
 * The schemas are then written to an XML file.
 */

class SelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( SelectTest );
  CPPUNIT_TEST( TestProps );
  CPPUNIT_TEST( TestBaseProps );
  CPPUNIT_TEST( TestBothProps );
  CPPUNIT_TEST( TestAllProps );
  CPPUNIT_TEST_SUITE_END();

public:
    SelectTest(void);
    virtual ~SelectTest(void);
    void setUp ();

protected:
    void TestProps();
    void TestBaseProps();
    void TestBothProps();
    void TestAllProps();

    FdoIConnection* CreateDb();
    void DoTest ( 
        FdoDictionaryP baseProps, 
        FdoDictionaryP props, 
        FdoStringP exclBaseProp,
        FdoStringP exclProp
    );
};

#endif
#endif
