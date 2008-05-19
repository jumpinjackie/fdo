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

#ifndef CPP_UNIT_SpatialContextTests_H
#define CPP_UNIT_SpatialContextTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class SpatialContextTests :
    public ArcSDETests
{
    CPPUNIT_TEST_SUITE (SpatialContextTests);
    CPPUNIT_TEST (test_create_delete);
    CPPUNIT_TEST (test_active);
    CPPUNIT_TEST (test_tables);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SpatialContextTests (void);
    virtual ~SpatialContextTests (void);
	void setUp ();
	void tearDown ();

protected:
    // test methods
	void test_create_delete ();
	void test_active ();
    void test_tables();

    // helper methods:
    void create_spatial_context ();
    void create_spatial_context2 ();
    FdoClassDefinition* create_class(FdoFeatureSchema *&schema);
    void delete_class(void);
    void ValidateSC2();

};

#endif // CPP_UNIT_SpatialContextTests_H

