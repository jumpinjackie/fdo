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

#ifndef CPP_UNIT_BasicSchemaTests_H
#define CPP_UNIT_BasicSchemaTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class BasicSchemaTests : public ArcSDETests
{
    CPPUNIT_TEST_SUITE (BasicSchemaTests);
    CPPUNIT_TEST (describe);
    CPPUNIT_TEST (apply);
    CPPUNIT_TEST (apply_odd_names);
    CPPUNIT_TEST (describe_one);
    CPPUNIT_TEST (delete_class);
    CPPUNIT_TEST (destroy_schema);
    CPPUNIT_TEST (apply_different_user);
    CPPUNIT_TEST (describe_many);
    CPPUNIT_TEST (create_unique_constraint);
    CPPUNIT_TEST (describe_read_only);
    CPPUNIT_TEST (apply_specific_geometry_types);
	CPPUNIT_TEST (get_schema_names);
	CPPUNIT_TEST (get_feature_class_names);
	CPPUNIT_TEST (describe_one_specified_class);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    BasicSchemaTests (void);
    virtual ~BasicSchemaTests (void);
    void setUp ();
    void tearDown ();

protected:
    void describe ();
    void describe_one ();
    void apply ();
    void apply_odd_names ();
    void delete_class ();
    void destroy_schema ();
    void apply_different_user ();
    void describe_many ();
    void create_unique_constraint ();
    void describe_read_only();
    void apply_specific_geometry_types();
	void get_schema_names();
	void get_feature_class_names();
	void describe_one_specified_class();
};

#endif // CPP_UNIT_BasicSchemaTests_H
