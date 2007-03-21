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
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

class OdbcBaseFdoInsertTest : public FdoInsertTest
{
public:
    virtual void setUp ();
    virtual void tearDown();
    virtual void connect ();

    virtual void insertCities();
    virtual void insertTable1();

    virtual void insert ();
    virtual void insertEmptyProps() {};
    virtual void insertFlushTest() {};
    virtual void insert2 () {};
    virtual void insert3 () {};
    virtual void insertBoundary () {};
    virtual void smartPointerInsert() {};
    virtual void conditionalInsert() {};
	virtual void insertAutoGen() {};
	virtual void featureReaderTest() {};

    virtual bool isPkeyAutogeneratedCities()            { return false; }
    virtual bool isPkeyAutogeneratedTable1()            { return false; }
    virtual bool isPkeyAutogeneratedAcdb3dpolyline()    { return true; }

    virtual FdoString * GetClassNameCities()            { return mSetup.GetClassNameCities(); }
    virtual FdoString * GetPropertyNameCitiesCityId()   { return mSetup.GetPropertyNameCitiesCityId(); }
    virtual FdoString * GetPropertyNameCitiesName()     { return mSetup.GetPropertyNameCitiesName(); }
    virtual FdoString * GetPropertyNameCitiesCity()     { return mSetup.GetPropertyNameCitiesCity(); }
	virtual FdoString * GetPropertyNameCitiesDate()     { return mSetup.GetPropertyNameCitiesDate(); }


protected:
    OdbcBaseSetup mSetup;
    FdoPtr<FdoIConnection> mConnection;
};


class OdbcOracleFdoInsertTest : public OdbcBaseFdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST_SUITE_END ();

public:
    OdbcOracleFdoInsertTest(void)   { this->mSetup.SetTypeDB(DataBaseType_Oracle); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcOracle" ); }

    virtual bool isPkeyAutogeneratedAcdb3dpolyline()    { return false; }
};

class OdbcMySqlFdoInsertTest : public OdbcBaseFdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST (ConfigFileTest);
    CPPUNIT_TEST_SUITE_END ();

public:
    OdbcMySqlFdoInsertTest(void)   { this->mSetup.SetTypeDB(DataBaseType_MySQL); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcMySql" ); }

    virtual void ConfigFileTest();
    FdoString * GetConfigFile2() {return L"MySqlTestConfig2.xml";}
};

#ifdef _WIN32
class OdbcSqlServerFdoInsertTest : public OdbcBaseFdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST_SUITE_END ();

    OdbcSqlServerFdoInsertTest(void) { this->mSetup.SetTypeDB(DataBaseType_SqlServer); }
    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcSqlServer" ); }
};

class OdbcAccessFdoInsertTest : public OdbcBaseFdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertCities );
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST( insertLidar );
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcAccess" ); }
    virtual void insert () {};
    virtual FdoString * GetClassNameCities()            { return L"Cities"; }
    virtual FdoString * GetPropertyNameCitiesCityId()   { return L"CityId"; }
    virtual FdoString * GetPropertyNameCitiesName()     { return L"Name"; }
    virtual FdoString * GetPropertyNameCitiesCity()     { return L"City"; }
	virtual FdoString * GetPropertyNameCitiesDate()		{ return L"Cdate";}
    void insertLidar();

    // Some values that can vary by target datastore...
    FdoString * GetConnectStringLidar() {return L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=Dbg/Lidar.mdb;ReadOnly=False\"";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
};

class OdbcExcelFdoInsertTest : public OdbcBaseFdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcExcelFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertTable1 );
    CPPUNIT_TEST( insertPoints );
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcExcel" ); }
    virtual void insert () {};
    virtual void insertCities() {};
    virtual void insertTable1();
    void insertPoints();

    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"ConnectionString=\"Driver={Microsoft Excel Driver (*.xls)};Dbq=Dbg/MsTest.xls;ReadOnly=False\"";}
    FdoString * GetConfigFile() {return L"MsTestXlsConfig.xml";}
};

#endif

#endif // ODBC_INSERTTESTS_H
