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

#ifndef SQLSERVER_CAPABILITIESTEST_H
#define SQLSERVER_CAPABILITIESTEST_H

#include <Fdo.h>
#include "FdoUpdateTest.h"

class SqlServerFdoUpdateTest : public FdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoUpdateTest, FdoUpdateTest);
    // Todo need to fix primary and foreign key retrieval for non-dbo tables
    // before this test will run successfully.
    // SqlServer 2000 has a bug where the information_schema does not list 
    // primary key columns for non-dbo tables.
    CPPUNIT_TEST( UpdateNonDbo );
    CPPUNIT_TEST(testDefect810181);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
public:
    void UpdateNonDbo();
    virtual FdoString* NoMetaSuffix();
    virtual FdoString* MultiIdSuffix();
    virtual FdoStringP TableIdGeomName();
    virtual void testDefect810181();
    virtual void UpdateSingleIdFeatureClass();

protected:
    virtual FdoStringP FixStringVal( FdoString* val );

    void TestRectangleFilter(
        FdoISelect * selectCommand,
        double xmin,
        double ymin,
        double xmax,
        double ymax,
        FdoSpatialOperations spatialOperation,
        FdoInt32 numRowsExpected);
};

#endif // SQLSERVER_CAPABILITIESTEST_H
