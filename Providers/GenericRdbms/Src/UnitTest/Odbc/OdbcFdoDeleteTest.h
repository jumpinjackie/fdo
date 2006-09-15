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

#ifndef ODBC_FDODELETETEST_H
#define ODBC_FDODELETETEST_H

#include "FdoDeleteTest.h"
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

class OdbcBaseFdoDeleteTest : public FdoDeleteTest
{
public:
    virtual void setUp ();
    virtual void tearDown();
    virtual void connect ();

    virtual void FeatureDelete ();
    virtual void ObjectPropDelete() {};

protected:
    OdbcBaseSetup mSetup;
    FdoPtr<FdoIConnection> mConnection;
};

class OdbcOracleFdoDeleteTest : public OdbcBaseFdoDeleteTest
{
public:
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleFdoDeleteTest, FdoDeleteTest);
    CPPUNIT_TEST_SUITE_END ();

    OdbcOracleFdoDeleteTest(void)   { this->mSetup.SetTypeDB(DataBaseType_Oracle); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcOracle" ); }
};

class OdbcMySqlFdoDeleteTest : public OdbcBaseFdoDeleteTest
{
public:
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlFdoDeleteTest, FdoDeleteTest);
    CPPUNIT_TEST (ConfigFileTest);
    CPPUNIT_TEST_SUITE_END ();

    OdbcMySqlFdoDeleteTest(void)   { this->mSetup.SetTypeDB(DataBaseType_MySQL); }
    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcMySql" ); }

    virtual void ConfigFileTest();
    FdoString * GetConfigFile2() {return L"MySqlTestConfig2.xml";}
};

#ifdef _WIN32
class OdbcSqlServerFdoDeleteTest : public OdbcBaseFdoDeleteTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoDeleteTest, FdoDeleteTest);
    CPPUNIT_TEST_SUITE_END ();

    OdbcSqlServerFdoDeleteTest(void) { this->mSetup.SetTypeDB(DataBaseType_SqlServer); }
    virtual void set_provider()      { UnitTestUtil::SetProvider( "OdbcSqlServer" ); }
};

class OdbcAccessFdoDeleteTest : public OdbcBaseFdoDeleteTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoDeleteTest, FdoDeleteTest);
    CPPUNIT_TEST_SUITE_END ();

    virtual void set_provider()     { UnitTestUtil::SetProvider( "OdbcAccess" ); }

    virtual void FeatureDelete ();
};
#endif

#endif // ODBC_FDODELETETEST_H
