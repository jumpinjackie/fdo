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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant_sp2/Shp/src/UnitTest/FilterTests.h#1 $
 *     $Author: lee $
 *   $DateTime: 2006/06/02 11:06:52 $
 *     $Change: 13046 $
 *
 */

#ifndef FILTERTESTS_H
#define FILTERTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class FilterTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (FilterTests);
    CPPUNIT_TEST (equal);
    CPPUNIT_TEST (less);
    CPPUNIT_TEST (greater);
    CPPUNIT_TEST (not_equal);
    CPPUNIT_TEST (in);
	CPPUNIT_TEST (combined);
    CPPUNIT_TEST (is_null);
    CPPUNIT_TEST (arithmetic_expression);
    CPPUNIT_TEST (logical_expression);
    CPPUNIT_TEST (string_expression);
    CPPUNIT_TEST (functions);
    CPPUNIT_TEST (computed);
    CPPUNIT_TEST (computed_geometry);
	CPPUNIT_TEST (non_existent_featid);
    CPPUNIT_TEST (featid_optimizer_tests );
 //   CPPUNIT_TEST (featid_performance_tests);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    FilterTests (void);
    virtual ~FilterTests (void);
	void setUp ();
	void tearDown ();

protected:
	void equal ();
    void less ();
    void greater ();
    void not_equal ();
    void in ();
	void combined ();
    void is_null ();
    void arithmetic_expression ();
    void logical_expression ();
    void string_expression ();
    void functions ();
    void computed ();
    void computed_geometry ();
	void non_existent_featid ();
    void featid_optimizer_tests();
    void featid_performance_tests();

private:
    int featid_roads_query (FdoString* query, int numRuns, int numExpected = -1, long *featidsExpected = NULL);
};

#endif // FILTERTESTS_H

