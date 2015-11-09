/*
 * Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef POSTGIS_CONNECTIONINFOTEST_H
#define POSTGIS_CONNECTIONINFOTEST_H

#include "FdoConnectionInfoTest.h"

//
// Checks whether or not the provider type and list of dependent files
// are reported correctly.
//
// The test is to establish a connection and get the connection info.
// Then the connection info APIs to retrieve the provider type and the
// list of dependent files is executed. For this test it is expected
// that the provider type is a "Database Server" and that there are
// no dependent files. Any other result should trigger exceptions.
//
class PostGisFdoConnectionInfoTest : public FdoConnectionInfoTest
{
    CPPUNIT_TEST_SUB_SUITE(PostGisFdoConnectionInfoTest, FdoConnectionInfoTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void set_provider();
    void SetExpectedFileList();
    void TestProviderInfo();
};

#endif // POSTGIS_CONNECTIONINFOTEST_H
