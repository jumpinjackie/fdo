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

#ifndef INSERTTESTS_H
#define INSERTTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class InsertTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (InsertTests);
    CPPUNIT_TEST (insert);
    CPPUNIT_TEST (insert_geometry_point_xy);
    CPPUNIT_TEST (insert_geometry_point_xyz);
    CPPUNIT_TEST (insert_geometry_point_xym);
    CPPUNIT_TEST (insert_geometry_point_xyzm);
    CPPUNIT_TEST (insert_geometry_points_xy);
    CPPUNIT_TEST (insert_geometry_points_xyz);
    CPPUNIT_TEST (insert_geometry_points_xym);
    CPPUNIT_TEST (insert_geometry_points_xyzm);
    CPPUNIT_TEST (insert_geometry_line_xy);
    CPPUNIT_TEST (insert_geometry_line_xyz);
    CPPUNIT_TEST (insert_geometry_line_xym);
    CPPUNIT_TEST (insert_geometry_line_xyzm);
    CPPUNIT_TEST (insert_geometry_polygon_xy);
    CPPUNIT_TEST (insert_geometry_polygon_xyz);
    CPPUNIT_TEST (insert_geometry_polygon_xym);
    CPPUNIT_TEST (insert_geometry_polygon_xyzm);
    CPPUNIT_TEST (insert_invalid_polygons);
    CPPUNIT_TEST (insert_geometry_multilinestring_xy);
    CPPUNIT_TEST (insert_geometry_multilinestring_xyz);
    CPPUNIT_TEST (insert_geometry_multilinestring_xym);
    CPPUNIT_TEST (insert_geometry_multilinestring_xyzm);
    CPPUNIT_TEST (insert_geometry_multipolygon_xy);
    CPPUNIT_TEST (insert_geometry_multipolygon_xyz);
    CPPUNIT_TEST (insert_geometry_multipolygon_xym);
    CPPUNIT_TEST (insert_geometry_multipolygon_xyzm);
    CPPUNIT_TEST (batch_insert);
    CPPUNIT_TEST (null_geometry_insert);
    CPPUNIT_TEST (null_data_insert);
    CPPUNIT_TEST (insert_large_first_geometry);
    CPPUNIT_TEST (insert_no_geometry);
	CPPUNIT_TEST (insert_2connects);
	CPPUNIT_TEST (insert_2connects_flush);
    //CPPUNIT_TEST (wide2mbPerformaceTest);
    CPPUNIT_TEST (insert_with_locale);
	CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    InsertTests (void);
    virtual ~InsertTests (void);
	void setUp ();
	void tearDown ();

protected:
    // Test methods:
	void insert ();
	void insert_with_locale ();
	void insert_locale (char *orig_locale, char *new_locale, FdoString *expected_cpg, const wchar_t *value);
    	void wide2mbPerformaceTest();
	void wide2multibyte(char *mb, wchar_t *in, int cpgWin, char *cpgLinux);
    void insert_geometry_point_xy ();
    void insert_geometry_point_xyz ();
    void insert_geometry_point_xym ();
    void insert_geometry_point_xyzm ();
    void insert_geometry_points_xy ();
    void insert_geometry_points_xyz ();
    void insert_geometry_points_xym ();
    void insert_geometry_points_xyzm ();
    void insert_geometry_line_xy ();
    void insert_geometry_line_xyz ();
    void insert_geometry_line_xym ();
    void insert_geometry_line_xyzm ();
    void insert_geometry_polygon_xy ();
    void insert_geometry_polygon_xyz ();
    void insert_geometry_polygon_xym ();
    void insert_geometry_polygon_xyzm ();
    void insert_invalid_polygons();
    void insert_geometry_multilinestring_xy ();
    void insert_geometry_multilinestring_xyz ();
    void insert_geometry_multilinestring_xym ();
    void insert_geometry_multilinestring_xyzm ();
    void insert_geometry_multipolygon_xy ();
    void insert_geometry_multipolygon_xyz ();
    void insert_geometry_multipolygon_xym ();
    void insert_geometry_multipolygon_xyzm ();
    void batch_insert ();
    void null_geometry_insert ();
    void null_data_insert ();
    void insert_large_first_geometry ();
    void insert_no_geometry ();
	void insert_2connects();
	void insert_2connects_flush();

    // Helper methods:
	void insert_connection( FdoIConnection* connection,  FdoIInsert *pInsert, FdoString* className, FdoString* id);
	int  get_count( FdoIConnection* connection, FdoString* class_name );
	void TestGeometrylessClass(FdoString* schemaName, FdoString* className, FdoClassType classType);

private:
    void create_schema (FdoGeometricType type, bool elevation, bool measure);
};

#endif // INSERTTESTS_H

