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

#ifndef ODBC_INSERTTESTS_H
#define ODBC_INSERTTESTS_H

#include "FdoInsertTest.h"

class OdbcOracleFdoInsertTest : public FdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST_SUITE_END ();

    void set_provider();
    void insertCities();
    void insertTable1();

    void insert () {};
    void insertEmptyProps() {};
    void insertFlushTest() {};
    void insert2 () {};
    void insert3 () {};
    void smartPointerInsert() {};
    virtual void conditionalInsert() {};
};

class OdbcMySqlFdoInsertTest : public FdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST_SUITE_END ();

    void set_provider();
    void insert ();

    // Some values that can vary by target datastore...
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
    void insertEmptyProps() {};
    void insertFlushTest() {};
    void insert2 () {};
    void insert3 () {};
    void smartPointerInsert() {};
    virtual void conditionalInsert() {};
};

#ifdef _WIN32
class OdbcAccessFdoInsertTest : public FdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST( insertLidar );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void insertCities();
    void insertTable1();
    void insertLidar();

    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
    FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConnectString3() {return L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=Dbg/Lidar.mdb;ReadOnly=False\"";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}

    void insert () {};
    void insertEmptyProps() {};
    void insertFlushTest() {};
    void insert2 () {};
    void insert3 () {};
    void smartPointerInsert() {};
    virtual void conditionalInsert() {};
};

class OdbcExcelFdoInsertTest : public FdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcExcelFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST( insertPoints );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void insertTable1();
    void insertPoints();

    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"ConnectionString=\"Driver={Microsoft Excel Driver (*.xls)};Dbq=Dbg/MsTest.xls;ReadOnly=False\"";}
    FdoString * GetConfigFile() {return L"MsTestXlsConfig.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}

    void insert () {};
    void insertEmptyProps() {};
    void insertFlushTest() {};
    void insert2 () {};
    void insert3 () {};
    void smartPointerInsert() {};
    virtual void conditionalInsert() {};
};

#endif

#endif // ODBC_INSERTTESTS_H
