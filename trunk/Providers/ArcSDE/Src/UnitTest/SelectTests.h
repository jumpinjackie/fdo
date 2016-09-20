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

#ifndef CPP_UNIT_SelectTests_H
#define CPP_UNIT_SelectTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class SelectTests :
    public ArcSDETests
{
    CPPUNIT_TEST_SUITE (SelectTests);
    CPPUNIT_TEST (select_large_table_with_geom);
    CPPUNIT_TEST (simple);
    CPPUNIT_TEST (get_class_before_readnext);
    CPPUNIT_TEST (simple_filter);
    CPPUNIT_TEST (string_with_quote);
    CPPUNIT_TEST (simple_erroneous_filter);
    CPPUNIT_TEST (simple_filter2);
    CPPUNIT_TEST (multiple_filter);
    CPPUNIT_TEST (spatial_filter);
    CPPUNIT_TEST (spatial_filter_buffer);
    CPPUNIT_TEST (spatial_filter_failures);
    CPPUNIT_TEST (spatial_filter_outofbounds);
    CPPUNIT_TEST (datetime_filter);
    CPPUNIT_TEST (get_null);
    CPPUNIT_TEST (spatial_filter_within_inside_coveredby);
    CPPUNIT_TEST (spatial_query_outside_coordsys_extents);
    CPPUNIT_TEST (multiple_databases);
    CPPUNIT_TEST (spatial_filter_buffer_outside_extents);
    CPPUNIT_TEST (simple_unique);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SelectTests (void);
    virtual ~SelectTests (void);
    void setUp ();
    void tearDown ();

protected:
    // tests data:
    UnitTestData *property0;
    UnitTestData *property1;
    UnitTestClass *nullableClass;

    // unit tests:
    void simple ();
    void get_class_before_readnext ();
    void simple_filter ();
    void string_with_quote ();
    void simple_erroneous_filter ();
    void simple_filter2 ();
    void multiple_filter ();
    void spatial_filter ();
    void spatial_filter_buffer ();
    void spatial_filter_failures ();
    void spatial_filter_outofbounds();
    void datetime_filter ();
    void get_null ();
    void spatial_filter_within_inside_coveredby();
    void spatial_query_outside_coordsys_extents();
    void multiple_databases();
    void spatial_filter_buffer_outside_extents();
    void select_large_table_with_geom();
    void simple_unique();

    // helper methods:
    void create_large_table_with_geom();
};

#endif // CPP_UNIT_SelectTests_H
