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

#ifndef ODBC_DESCRIBESCHEMA_H
#define ODBC_DESCRIBESCHEMA_H

#include "DescribeSchemaTest.h"
#include "OdbcBaseSetup.h"

class OdbcDescribeSchemaTest : public DescribeSchemaTest
{
public:
	OdbcDescribeSchemaTest(DataBaseType typeDB)
	{
		m_OdbcSetup.SetTypeDB(typeDB);
	}
	virtual void describe ();
	virtual void dropDb(){};
	virtual void LoadTestData(FdoIConnection* connection);
protected:
	OdbcBaseSetup m_OdbcSetup;
};

class OdbcMySqlDescribeSchemaTest : public OdbcDescribeSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlDescribeSchemaTest, DescribeSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

public:
	OdbcMySqlDescribeSchemaTest() : OdbcDescribeSchemaTest(DataBaseType_MySQL){}
    
	void  set_provider();

	FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
};

#ifdef _WIN32
class OdbcSqlServerDescribeSchemaTest : public OdbcDescribeSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerDescribeSchemaTest, DescribeSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

public:
	OdbcSqlServerDescribeSchemaTest() : OdbcDescribeSchemaTest(DataBaseType_SqlServer){}
    virtual void  set_provider();

protected:
    static const wchar_t* mDataSqlServer[];
};

class OdbcTextDescribeSchemaTest : public DescribeSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcTextDescribeSchemaTest, DescribeSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    virtual void describe ();
    
	// Some values that can vary by target datastore...
    FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
    FdoString * GetConnectString2() {return L"DataSourceName=Country_Text";}
    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
    FdoString * GetConfigFile2() {return L"MsTestNoGeometry.xml";}
    FdoString * GetSchemaName() {return L"Fdo";}
    FdoString * GetPhysicalSchemaName() {return L"";}
};

class OdbcAccessDescribeSchemaTest : public DescribeSchemaTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessDescribeSchemaTest, DescribeSchemaTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    virtual void describe ();
    virtual void getSchemaNames();
    virtual void getClassNames();
    virtual void describeWithClassNames();

    FdoString * GetConfigFile() {return L"MsTestConfig.xml";}
	FdoString * GetConnectString() {return L"DataSourceName=MsTest;UserId=;Password=;";}
};

#endif

#endif // ODBC_DESCRIBESCHEMA_H
