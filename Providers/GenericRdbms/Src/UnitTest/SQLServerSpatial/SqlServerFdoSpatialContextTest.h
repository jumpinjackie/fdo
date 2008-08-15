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

#ifndef SQLSERVER_FDOSPATIALCONTEXTTEST_H
#define SQLSERVER_FDOSPATIALCONTEXTTEST_H

#include "FdoSpatialContextTest.h"

class SqlServerFdoSpatialContextTest : public FdoSpatialContextTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoSpatialContextTest, FdoSpatialContextTest);
    CPPUNIT_TEST( testAdjustExtents );
    CPPUNIT_TEST( testNumericCoordinateSystemNames );
    CPPUNIT_TEST_SUITE_END ();

protected:
    virtual void set_provider();

    // Unit test methods:
    virtual void testAdjustExtents ();
    virtual void testNumericCoordinateSystemNames ();

    void VldAdjustedExtent( FdoStringP scName, FdoByteArray* ba, double minx, double miny, double maxx, double maxy );
};

#endif // SQLSERVER_FDOSPATIALCONTEXTTEST_H
