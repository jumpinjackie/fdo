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

#ifndef MYSQL_FDODATASTORETEST_H
#define MYSQL_FDODATASTORETEST_H

#include "FdoDatastoreTest.h"

class MySqlFdoDatastoreTest : public FdoDatastoreTest
{
    CPPUNIT_TEST_SUB_SUITE (MySqlFdoDatastoreTest, FdoDatastoreTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();

	virtual FdoStringP get_lt_mode(){return L"NONE";}

    virtual void ReservedName(FdoIConnection* connection);
};

#endif // MYSQL_FDODATASTORETEST_H
