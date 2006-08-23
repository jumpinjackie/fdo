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

#ifndef ODBC_ADVANCEDSELECTTEST_H
#define ODBC_ADVANCEDSELECTTEST_H

#include "FdoAdvancedSelectTest.h"

#ifdef _WIN32

class OdbcAccessFdoAdvancedSelectTest : public FdoAdvancedSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoAdvancedSelectTest, FdoAdvancedSelectTest);
    CPPUNIT_TEST (TestSelectExpressions);
    CPPUNIT_TEST (TestCount);
    CPPUNIT_TEST (Table1Test);
    CPPUNIT_TEST (RestrictedPropertiesTable1Test);
    CPPUNIT_TEST (ComparisonFilterTable1Test);
    CPPUNIT_TEST (TestReaderDestructor);
    CPPUNIT_TEST (TestUpperLower);
    CPPUNIT_TEST (TestConcat);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();

    // Overridden tests.
    virtual void connect ();
    virtual void compIdentFilterTest() {};
    virtual void compIdentPropertyTest() {};
    virtual void orderByTest();
    virtual void groupByTest();
    virtual void functionTest() {};
    virtual void selectDistinctTest();
    virtual void TestDefect779194() {};
    virtual void TestDefect785616();

    // Extra tests for this class.
    void Table1Test();
    void RestrictedPropertiesTable1Test();
    void ComparisonFilterTable1Test();
    void TestReaderDestructor();
    void TestSelectExpressions();
    void TestCount();
    void TestUpperLower();
    void TestConcat();

protected:
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoStringP GetTable1Classname()
    {
        FdoStringP className = GetSchemaName();
        className += L":";
        className += L"TABLE1";
        return className;
    }
    FdoStringP GetEmployeesClassname()
    {
        FdoStringP className = GetSchemaName();
        className += L":";
        className += L"EMPLOYEES";
        return className;
    }

};

#endif

#endif // ODBC_ADVANCEDSELECTTEST_H
