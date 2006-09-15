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

#ifndef ODBC_FDOSELECTTEST_H
#define ODBC_FDOSELECTTEST_H

#include "FdoSelectTest.h"
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

class OdbcFdoSelectTest : public FdoSelectTest
{
public:
    virtual void setUp ();
    virtual void tearDown();
    virtual void connect ();

    virtual void feature_query ();

    virtual void non_feature_query () {};
    virtual void distance_query () {};
    virtual void spatial_query () {};
    virtual void feature_object_query() {};
    virtual void feature_subset_query () {};
    virtual void non_feature_subset_query () {};
    virtual void feature_subset_query2() {};
    virtual void feature_select_obj_distinct() {};
    virtual void feature_geom_query () {};

    virtual FdoString * GetClassName()          { return mSetup.LikesUC() ? L"TESTCLASS" : L"testClass"; };

    virtual int numPropertiesInPolylineClass() { return 16; };

protected:
    OdbcBaseSetup mSetup;
};

class OdbcOracleFdoSelectTest : public OdbcFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST_SUITE_END ();

    OdbcOracleFdoSelectTest(void)   { this->mSetup.SetTypeDB(DataBaseType_Oracle); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcOracle" ); }
    virtual int numPropertiesInPolylineClass() { return 17; }
};

class OdbcMySqlFdoSelectTest : public OdbcFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST (ConfigFileTest);
    CPPUNIT_TEST_SUITE_END ();

    OdbcMySqlFdoSelectTest(void)   { this->mSetup.SetTypeDB(DataBaseType_MySQL); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcMySql" ); }

    virtual void ConfigFileTest();
    FdoString * GetConfigFile2() {return L"MySqlTestConfig2.xml";}

    virtual FdoString * GetClassName()          { return L"testclass"; }
    virtual int numPropertiesInPolylineClass() { return 15; }
};

#ifdef _WIN32
class OdbcSqlServerFdoSelectTest : public OdbcFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST_SUITE_END ();

    virtual void concurrent_select() {};    // Need to set up "testClass" class to run this.

    OdbcSqlServerFdoSelectTest(void) { this->mSetup.SetTypeDB(DataBaseType_SqlServer); }
    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcSqlServer" ); }
};

class OdbcAccessFdoSelectTest : public OdbcFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST (Table1Test);
    //CPPUNIT_TEST (Table2Test);
    CPPUNIT_TEST (ComparisonFilterTable1Test);
    CPPUNIT_TEST (RestrictedPropertiesTable1Test);
    CPPUNIT_TEST (TestDateFilter);
    CPPUNIT_TEST (TestDefect779194);
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcAccess" ); }

public:
    virtual void Table1Test();
    void Table2Test();
    void ComparisonFilterTable1Test();  // Contains a spatial query
    void RestrictedPropertiesTable1Test();
    void TestDefect779194();

    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
    FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetConfigFile3() {return L"MsTestXlsConfig.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}

    virtual void feature_query () {};
    virtual void concurrent_select () {};
};

class OdbcDbaseFdoSelectTest : public OdbcAccessFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcDbaseFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST (Table1Test);
    //CPPUNIT_TEST (Table2Test);
    //CPPUNIT_TEST (ComparisonFilterTable1Test);
    //CPPUNIT_TEST (RestrictedPropertiesTable1Test);
    //CPPUNIT_TEST (TestDateFilter);
    //CPPUNIT_TEST (TestDefect779194);
    CPPUNIT_TEST_SUITE_END ();

    virtual void Table1Test() { OdbcAccessFdoSelectTest::Table1Test(); }

    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcDbase" ); }
};

class OdbcExcelFdoSelectTest : public OdbcFdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcExcelFdoSelectTest, OdbcFdoSelectTest);
    CPPUNIT_TEST (AllTypesTest);
    CPPUNIT_TEST (AllTypesConfigFileTest);
    CPPUNIT_TEST (AllTypesConfigFileTest_defect814052);
    CPPUNIT_TEST (CityTest);
    CPPUNIT_TEST (TestDateFilter);
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcExcel" ); }
    void AllTypesTest();
    void AllTypesConfigFileTest();
    void AllTypesConfigFileTest_defect814052();

    // Tests select from table without primary key.
    void CityTest();

    // Some values that can vary by target datastore...
    //FdoString * GetConnectString() {return L"ConnectionString=\"Driver={Microsoft Excel Driver (*.xls)};Dbq=Dbg/MsTest.xls;\"";}
    FdoString * GetConnectString() {return L"DataSourceName=MsTestXls;UserId=;Password=;";}
    FdoString * GetConfigFile() {return L"MsTestXlsConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestXlsConfigNoIdent.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}

    virtual void feature_query () {};
    virtual void concurrent_select () {};
};
#endif

#endif // ODBC_FDOSELECTTEST_H
