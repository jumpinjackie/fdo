/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#ifndef ODBC_MULTIPLECONNECTTEST_H
#define ODBC_MULTIPLECONNECTTEST_H

#include "FdoConnectTest.h"

class OdbcMySqlFdoConnectTest : public FdoConnectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoConnectTest, FdoConnectTest);
    CPPUNIT_TEST (ConnectByDsn);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void connect() {};
    void query () {};
    
	void ConnectByDsn();
};

class OdbcOracleFdoConnectTest : public FdoConnectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoConnectTest, FdoConnectTest);
    CPPUNIT_TEST (ConfigFileTest);
    CPPUNIT_TEST (AllDatabaseTypesTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();

    void connect() {};
    void query () {};

    void ConfigFileTest();
    void AllDatabaseTypesTest();

    // Some values that can vary by target datastore...
    FdoString * GetConfigFile() {return L"OracleTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"OracleTestConfig2.xml";}
    FdoString * GetSchemaName() {return L"Acdb";}
    FdoString * GetPhysicalSchemaName() {return L"";}

    void AssertAllDatabaseTypes(FdoClassDefinition * classDef);
};

#ifdef _WIN32

class OdbcAccessFdoConnectTest : public FdoConnectTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoConnectTest, FdoConnectTest);
    CPPUNIT_TEST (StaticCreateTest);
    CPPUNIT_TEST (CapabilitiesTest);
    CPPUNIT_TEST (InfoTest);
    // CPPUNIT_TEST (StringTest);
    CPPUNIT_TEST (OpenTest);
    CPPUNIT_TEST (ConfigFileTest);
	CPPUNIT_TEST (ConnectWithParmTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void connect();

	// We don't have the AcDb schema set up for ODBC yet.
	virtual void query () {};
    void StaticCreateTest();
    void CapabilitiesTest();
    void InfoTest();
    void StringTest();
    void OpenTest();
    void ConfigFileTest();
	virtual void connectWithInvalidString(){};

    // Some values that can vary by target datastore...
	FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
	FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}

	void ConnectWithParmTest();
};

#endif

#endif // ODBC_MULTIPLECONNECTTEST_H
