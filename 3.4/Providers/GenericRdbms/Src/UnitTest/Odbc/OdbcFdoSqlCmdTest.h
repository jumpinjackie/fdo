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

#ifndef ODBC_FDOSQLCMDTEST_H
#define ODBC_FDOSQLCMDTEST_H

#include "FdoSqlCmdTest.h"

#ifdef _WIN32
class OdbcSqlServerFdoSqlCmdTest : public FdoSqlCmdTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcSqlServerFdoSqlCmdTest, FdoSqlCmdTest);
    CPPUNIT_TEST( CreateAndDrop8bitTable );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void CreateAndDrop8bitTable ();
    virtual void doGetSC() {}
    virtual void connect ();
	virtual FdoString * GetConnectString();

    virtual void CreateActivateDestroySC() {}
    wchar_t *get_datetime_type(){return L"datetime";}
    wchar_t *get_datetime (){return L"1999-05-29-02-24-32";}
    wchar_t *get_datetime_sys(){return L"CURRENT_TIMESTAMP";}// or "getdate()"; - doesn't work! freezes
    wchar_t *get_number_type(){return L"numeric";}
};

class OdbcAccessFdoSqlCmdTest : public FdoSqlCmdTest
{
    CPPUNIT_TEST_SUB_SUITE (OdbcAccessFdoSqlCmdTest, FdoSqlCmdTest);
    CPPUNIT_TEST (EmployeesTest);
    CPPUNIT_TEST (Table1Test);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    virtual void EmployeesTest();
    virtual void Table1Test();
    virtual void connect ();

    virtual void doGetSC() {}
    virtual void CreateActivateDestroySC() {}
    virtual void CreateTable () {}
    virtual void BasicInsert () {}
    virtual void BindInsert () {}
    virtual void Query () {}
    virtual void QueryGeometry() {}
    virtual void Delete () {}
    virtual void DropTable () {}
};
#endif

#endif // ODBC_FDOSQLCMDTEST_H
