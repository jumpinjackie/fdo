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

#ifndef CPP_UNIT_SelectAggregatesTests_H
#define CPP_UNIT_SelectAggregatesTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class SelectAggregatesTests :
    public ArcSDETests
{
    CPPUNIT_TEST_SUITE (SelectAggregatesTests);
    CPPUNIT_TEST (test_distinct);
    CPPUNIT_TEST (test_aggregate_functions);
    CPPUNIT_TEST (test_orderby);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SelectAggregatesTests (void);
    virtual ~SelectAggregatesTests (void);
	void setUp ();
	void tearDown ();

    // Test methods:
protected:
	void test_distinct ();
    void test_aggregate_functions ();
    void test_orderby ();

    // Helper methods:
protected:
    void IterateDataReaderProperties(FdoIDataReader* dataReader);

};

#endif // CPP_UNIT_SelectAggregatesTests_H
