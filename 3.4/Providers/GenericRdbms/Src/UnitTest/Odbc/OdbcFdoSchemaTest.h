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

#ifndef ODBC_FDOSCHEMATEST_H
#define ODBC_FDOSCHEMATEST_H

#include "FdoSchemaTest.h"
#include "OdbcBaseSetup.h"

class OdbcFdoSchemaTest : public FdoSchemaTest
{
public:
	OdbcFdoSchemaTest(DataBaseType typeDB)
	{
		m_OdbcSetup.SetTypeDB(typeDB);
	}
    virtual void CreateFreshDb();

    virtual void OverrideXmlAllTest() {};
    virtual void OverrideXmlErrTest() {};

protected:
	OdbcBaseSetup m_OdbcSetup;

	static const wchar_t* mApplySchemaSqlServerCt[];
	static const wchar_t* mApplySchemaMySqlCt[];
	static const wchar_t* mApplySchemaOracleCt[];
	static const wchar_t* mApplySchemaOracleCtTrigger;
};

class OdbcMySqlFdoSchemaTest : public OdbcFdoSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoSchemaTest, FdoSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

public:
	OdbcMySqlFdoSchemaTest() : OdbcFdoSchemaTest(DataBaseType_MySQL){}

protected:
    virtual void set_provider();
};

class OdbcOracleFdoSchemaTest : public OdbcFdoSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoSchemaTest, FdoSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

public:
	OdbcOracleFdoSchemaTest() : OdbcFdoSchemaTest(DataBaseType_Oracle){}

protected:
    virtual void set_provider();
};

#ifdef _WIN32
class OdbcSqlServerFdoSchemaTest : public OdbcFdoSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoSchemaTest, FdoSchemaTest);
    CPPUNIT_TEST (LowerCaseXYZ);
    CPPUNIT_TEST_SUITE_END ();

public:
	OdbcSqlServerFdoSchemaTest() : OdbcFdoSchemaTest(DataBaseType_SqlServer){}

protected:
    virtual void set_provider();
    virtual void LowerCaseXYZ();
};

class OdbcAccessFdoSchemaTest : public FdoSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoSchemaTest, FdoSchemaTest);
    CPPUNIT_TEST (SchemaMappingTest);
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider();
    void SchemaMappingTest();

    void schema () {};
    void UnconnectedTest() {};
    void OverrideXmlAllTest() {};
    void OverrideXmlErrTest() {};
    void OverrideTablesWithOddAsciiChars() {};
    
    FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
};
#endif

#endif // ODBC_FDOSCHEMATEST_H
