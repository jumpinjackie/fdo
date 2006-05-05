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

#ifndef SELECTAGGREGATESTESTS_H
#define SELECTAGGREGATESTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class SelectAggregatesTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (SelectAggregatesTests);
    CPPUNIT_TEST (select_distinct);
    CPPUNIT_TEST (select_aggregates);
    CPPUNIT_TEST (select_aggregates_null);
    CPPUNIT_TEST (select_orderby);
    CPPUNIT_TEST (count);
    CPPUNIT_TEST (select_orderby_decimal);
    CPPUNIT_TEST (select_orderby_scrambled_property_ids);
    CPPUNIT_TEST (select_orderby_empty_select_list);
    CPPUNIT_TEST (selectAggregatesSpatialExtentsTest);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SelectAggregatesTests (void);
    virtual ~SelectAggregatesTests (void);
	void setUp ();
	void tearDown ();

protected:
	void select_distinct();
    void select_aggregates();
    void select_aggregates_null ();
    void select_orderby();
    void count ();
    void select_orderby_decimal ();
    void select_orderby_scrambled_property_ids();
    void select_orderby_empty_select_list();
    void selectAggregatesSpatialExtentsTest();  // This test is ported from SDF Provider

    // utility methods:
    void create_schema (FdoString *schemaName, FdoString* className, FdoGeometricType type, bool elevation, bool measure, bool bInsertTestData);
};

#endif // SELECTAGGREGATESTESTS_H

