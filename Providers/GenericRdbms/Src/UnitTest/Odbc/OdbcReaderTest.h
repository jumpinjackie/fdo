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

#ifndef ODBC_FDOREADERTEST_H
#define ODBC_FDOREADERTEST_H

#include "ReaderTest.h"
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

class OdbcReaderTest : public ReaderTest
{
protected:
    virtual void PrepareConnection();

protected:
    OdbcBaseSetup mSetup;
};

class OdbcOracleReaderTest: public OdbcReaderTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcOracleReaderTest, ReaderTest);
    CPPUNIT_TEST_SUITE_END ();
public:

    OdbcOracleReaderTest(void)
    {
        this->mSetup.SetTypeDB(DataBaseType_Oracle);
    }

    virtual void set_provider()
    {
        UnitTestUtil::SetProvider( "OdbcOracle" );
    }
};

class OdbcSqlServerReaderTest: public OdbcReaderTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerReaderTest, ReaderTest);
    CPPUNIT_TEST_SUITE_END ();
public:

    OdbcSqlServerReaderTest(void)
    {
        this->mSetup.SetTypeDB(DataBaseType_SqlServer);
    }

    virtual void set_provider()
    {
        UnitTestUtil::SetProvider( "OdbcSqlServer" );
    }
};

class OdbcMySqlReaderTest: public OdbcReaderTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcMySqlReaderTest, ReaderTest);
    CPPUNIT_TEST_SUITE_END ();
public:

    OdbcMySqlReaderTest(void)
    {
        this->mSetup.SetTypeDB(DataBaseType_MySQL);
    }

    virtual void set_provider()
    {
        UnitTestUtil::SetProvider( "OdbcMySql" );
    }
};

#endif // ODBC_FDOREADERTEST_H
