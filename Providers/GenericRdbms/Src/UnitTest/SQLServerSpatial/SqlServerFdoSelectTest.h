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

#ifndef SQLSERVER_FDOSELECTTEST_H
#define SQLSERVER_FDOSELECTTEST_H

#include "FdoSelectTest.h"

class SqlServerFdoSelectTest : public FdoSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoSelectTest, FdoSelectTest);
    CPPUNIT_TEST (TestDateFilter);
    CPPUNIT_TEST (TestSpatialGeog);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void TestDateFilter();
    void TestSpatialGeog();

	void spatial_query_defect813611();

protected:
    void SpatialGeogSchema( FdoIConnection* connection );
    void SpatialGeogData( FdoIConnection* connection );
    void SpatialGeogSelect( FdoIConnection* connection, FdoIGeometry* spatialArea );
    FdoStringsP SpatialGeogSelectNames( FdoIConnection* connection, FdoIGeometry* spatialArea, FdoStringP propName );

private:
    static const FdoStringP mSpatialGeogSuffix;

};

#endif // SQLSERVER_FDOSELECTTEST_H
