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

#ifndef CPP_UNIT_BasicInsertTests_H
#define CPP_UNIT_BasicInsertTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class BasicInsertTests :
    public ArcSDETests
{
    CPPUNIT_TEST_SUITE (BasicInsertTests);
    CPPUNIT_TEST (user_managed_ids);
    CPPUNIT_TEST (simple_insert);
    CPPUNIT_TEST (toobig_insert);
    CPPUNIT_TEST (full_insert);
    CPPUNIT_TEST (geometry_insert);
    CPPUNIT_TEST (geometry_insert_ZM);
    CPPUNIT_TEST (failure_insert);
    CPPUNIT_TEST (batch_insert);
    CPPUNIT_TEST (defaults_insert);
    CPPUNIT_TEST (nullable_insert);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    BasicInsertTests (void);
    virtual ~BasicInsertTests (void);
	void setUp ();
	void tearDown ();

protected:
	void simple_insert ();
    void toobig_insert ();
	void full_insert ();
    void geometry_insert ();
    void geometry_insert_ZM ();
	void failure_insert ();
    void batch_insert ();
    void defaults_insert ();
    void user_managed_ids ();
    void nullable_insert ();


    // helper methods:
    FdoILinearRing* CreatePolygonRing(double x, double y, double z, double radius, bool bExterior = true);

};

#endif // CPP_UNIT_BasicInsertTests_H

