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

#ifndef CPP_UNIT_BasicConnectionTests_H
#define CPP_UNIT_BasicConnectionTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class BasicConnectionTests : public ArcSDETests
{
    CPPUNIT_TEST_SUITE (BasicConnectionTests);
    CPPUNIT_TEST (info);
    CPPUNIT_TEST (get_properties);
    CPPUNIT_TEST (set_properties);
    CPPUNIT_TEST (connection_capabilities);
    CPPUNIT_TEST (schema_capabilities);
    CPPUNIT_TEST (command_capabilities);
    CPPUNIT_TEST (filter_capabilities);
    CPPUNIT_TEST (expression_capabilities);
    CPPUNIT_TEST (geometry_capabilities);
    CPPUNIT_TEST_SUITE_END ();

    FdoIConnection* mConnection;

public:
    BasicConnectionTests (void);
    virtual ~BasicConnectionTests (void);
	void setUp ();
	void tearDown ();

protected:
	void info ();
    void get_properties ();
    void set_properties ();
    void connection_capabilities ();
    void schema_capabilities ();
    void command_capabilities ();
    void filter_capabilities ();
    void expression_capabilities ();
    void geometry_capabilities ();
};

#endif // CPP_UNIT_BasicConnectionTests_H
