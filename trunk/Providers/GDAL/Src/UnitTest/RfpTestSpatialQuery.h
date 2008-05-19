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

#ifndef RfpTestSpatialQuery_H
#define RfpTestSpatialQuery_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestSpatialQuery : public RfpTestCase
{
    FDO_CPPUNIT_DEFINE(testConfigWithBounds);
    FDO_CPPUNIT_DEFINE(testConfigWithoutBounds);
    FDO_CPPUNIT_DEFINE(testWithoutConfig);
	
    CPPUNIT_TEST_SUITE(RfpTestSpatialQuery);	
    CPPUNIT_TEST(testConfigWithBounds);
    CPPUNIT_TEST(testConfigWithoutBounds);
    CPPUNIT_TEST(testWithoutConfig);
    CPPUNIT_TEST_SUITE_END();

public:
    RfpTestSpatialQuery(void);
    ~RfpTestSpatialQuery(void);

    virtual void _setUp();
    virtual void _tearDown();

    void testConfigWithBounds();
    void testConfigWithoutBounds();
    void testWithoutConfig();
};

#endif
