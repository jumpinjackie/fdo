/*
 * 
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


#ifndef SELECTTESTS_H
#define SELECTTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"


class SelectTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (SelectTests);
    CPPUNIT_TEST (select);
    CPPUNIT_TEST (get_spatial_contexts);
    CPPUNIT_TEST (get_class_definition);
    CPPUNIT_TEST (requery);
    CPPUNIT_TEST (select_subset);
    CPPUNIT_TEST (select_all);
    CPPUNIT_TEST (select_mixed_decimals);
    CPPUNIT_TEST (get_spatial_context_text);
    CPPUNIT_TEST (select_only_geometry);
    CPPUNIT_TEST (select_hawaii);
    CPPUNIT_TEST (select_states);
    CPPUNIT_TEST (select_date_time);
    CPPUNIT_TEST (select_large_geometries);
    CPPUNIT_TEST (analyze_polygons);
    CPPUNIT_TEST (select_with_aggregates_should_fail);
    CPPUNIT_TEST (upper_lower);
    CPPUNIT_TEST (ceil_floor);
	CPPUNIT_TEST (select_length_area );
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SelectTests (void);
    virtual ~SelectTests (void);
	void setUp ();
	void tearDown ();

protected:
	void select();
    void get_spatial_contexts ();
    void get_class_definition ();
    void requery ();
    void select_subset ();
    void select_all ();
    void select_mixed_decimals ();
    void get_spatial_context_text ();
	void select_only_geometry();
    void select_hawaii ();
    void select_states ();
    void select_date_time ();
    void select_large_geometries ();
    void analyze_polygons();
    void select_with_aggregates_should_fail();
    void upper_lower();
    void ceil_floor();
	void select_length_area();

private:
    // utility methods:
    void get_spatial_context (FdoIConnection* connection, FdoString* cs_name, int expected_num_sc);
    void create_schema (FdoString *schemaName, FdoString* className, FdoGeometricType type, bool elevation, bool measure, bool bInsertTestData, bool bUseDoubleType);
	void select_length_area2(bool GEODETIC);
};

#endif // SELECTTESTS_H
