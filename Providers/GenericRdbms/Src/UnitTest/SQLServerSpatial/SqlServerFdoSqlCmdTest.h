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

#ifndef SQLSERVER_FDOSQLCMDTEST_H
#define SQLSERVER_FDOSQLCMDTEST_H

#include "FdoSqlCmdTest.h"

class SqlServerFdoSqlCmdTest : public FdoSqlCmdTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoSqlCmdTest, FdoSqlCmdTest);
    CPPUNIT_TEST( CreateAndDrop8bitTable );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
	void CreateAndDrop8bitTable ();

	wchar_t *get_datetime_type() 
    { 
        return L"datetime"; 
    }

    wchar_t *get_datetime ()
    {
        return L"1999-05-29-02-24-32";
    }
  
    wchar_t *get_datetime_sys()
    {
        return L"CURRENT_TIMESTAMP"; // or "getdate()"; - doesn't work! freezes
    }

    wchar_t *get_number_type() 
    { 
        return L"numeric"; 
    }
};

#endif // SQLSERVER_FDOSQLCMDTEST_H
