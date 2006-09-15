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

#ifndef ODBC_CAPABILITIESTEST_H
#define ODBC_CAPABILITIESTEST_H

#include "FdoUpdateTest.h"
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

class OdbcBaseFdoUpdateTest : public FdoUpdateTest
{
public:
    virtual void setUp ();
    virtual void tearDown();
    virtual void connect ();

    virtual void updateCities();
    virtual void updateTable1();

    virtual void MainFdoUpdateTest (FdoIConnection* Conn=NULL) {}
	virtual void Update () {}
  	virtual void UpdateAttributesOnly (){}
  	virtual void UpdateNonFeatureClass (){}
  	virtual void FdoUpdateTestTypes (){}
  	virtual void UpdateObjectProperty(){}
  	virtual void UpdateMultiIdFeatureClass (){}
  	virtual void UpdateSingleIdFeatureClass (){}
  	virtual void ConditionalUpdate (){}
  	virtual void UpdateNoMeta (){}

protected:
    OdbcBaseSetup mSetup;
    FdoPtr<FdoIConnection> mConnection;
};

class OdbcOracleFdoUpdateTest : public OdbcBaseFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateCities );
    CPPUNIT_TEST( updateTable1 );
    CPPUNIT_TEST_SUITE_END ();

public:
    OdbcOracleFdoUpdateTest(void)   { this->mSetup.SetTypeDB(DataBaseType_Oracle); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcOracle" ); }
};

class OdbcMySqlFdoUpdateTest : public OdbcBaseFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateCities );
    CPPUNIT_TEST( updateTable1 );
    CPPUNIT_TEST_SUITE_END ();

public:
    OdbcMySqlFdoUpdateTest(void)   { this->mSetup.SetTypeDB(DataBaseType_MySQL); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcMySql" ); }
};

#ifdef _WIN32
class OdbcSqlServerFdoUpdateTest : public OdbcBaseFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateCities );
    CPPUNIT_TEST( updateTable1 );
    CPPUNIT_TEST_SUITE_END ();

    OdbcSqlServerFdoUpdateTest(void)   { this->mSetup.SetTypeDB(DataBaseType_SqlServer); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcSqlServer" ); }
};

class OdbcAccessFdoUpdateTest : public OdbcBaseFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateCities );
    CPPUNIT_TEST( updateTable1 );
    CPPUNIT_TEST_SUITE_END ();

public:
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcAccess" ); }
    virtual void updateCities();
    virtual void updateTable1();

    virtual FdoString * GetClassNameCities()            { return L"Cities"; }
    virtual FdoString * GetPropertyNameCitiesName()     { return L"Name"; }
    virtual FdoString * GetPropertyNameCitiesCity()     { return L"City"; }

    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
    FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
};

class OdbcDbaseFdoUpdateTest : public OdbcAccessFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcDbaseFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateCities );
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcDbase" ); }
    virtual void updateCities()     { OdbcAccessFdoUpdateTest::updateCities(); }
    virtual FdoString * GetClassNameCities()            { return L"CITIES"; }
    virtual FdoString * GetPropertyNameCitiesName()     { return L"NAME"; }
    virtual FdoString * GetPropertyNameCitiesCity()     { return L"CITY"; }
};

class OdbcExcelFdoUpdateTest : public OdbcBaseFdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcExcelFdoUpdateTest, FdoUpdateTest);
    CPPUNIT_TEST( updateTable1 );
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcExcel" ); }
    virtual void updateTable1();


    // Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"ConnectionString=\"Driver={Microsoft Excel Driver (*.xls)};Dbq=Dbg/MsTest.xls;ReadOnly=False\"";}
    FdoString * GetConfigFile() {return L"MsTestXlsConfig.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
};
#endif

#endif // ODBC_CAPABILITIESTEST_H
