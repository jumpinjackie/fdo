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
 */

#ifndef UPDATETESTS_H
#define UPDATETESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class UpdateTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (UpdateTests);
    CPPUNIT_TEST (update);
    CPPUNIT_TEST (update_geometry_point_xy);
    CPPUNIT_TEST (update_geometry_line_xy);
    CPPUNIT_TEST (update_geometry_polygon_xy);
    CPPUNIT_TEST (update_null_data_values);
    CPPUNIT_TEST (update_int32);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    UpdateTests (void);
    virtual ~UpdateTests (void);
	void setUp ();
	void tearDown ();

protected:
	void update ();
    void update_geometry_point_xy ();
    void update_geometry_line_xy ();
    void update_geometry_polygon_xy ();
    void update_null_data_values ();
    void update_int32();

private:
    void create_schema (FdoGeometricType type, bool elevation, bool measure);
};

#endif // UPDATETESTS_H

