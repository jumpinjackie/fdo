//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_CONNECTIONTEST_H_INCLUDED
#define FDOPOSTGIS_CONNECTIONTEST_H_INCLUDED

#include "BaseTestCase.h"

class ConnectionTest : public fdo::postgis::test::BaseTestCase
{
    CPPUNIT_TEST_SUITE(ConnectionTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();

    //FdoIConnection* mConnection;

public:

    ConnectionTest();
    virtual ~ConnectionTest();

    void setUp();
	void tearDown();

    void testConstructor();

};

#endif // FDOPOSTGIS_CONNECTIONTEST_H_INCLUDED
