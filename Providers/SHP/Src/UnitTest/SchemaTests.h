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

#ifndef SCHEMATESTS_H
#define SCHEMATESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class SchemaTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (SchemaTests);
    CPPUNIT_TEST (describe);
    CPPUNIT_TEST (describe_bogus);
    CPPUNIT_TEST (apply);
    CPPUNIT_TEST (apply_schema);
    CPPUNIT_TEST (apply_nameless);
    CPPUNIT_TEST (create_nameless);
    CPPUNIT_TEST (schema_mapping_defaults_test);
    CPPUNIT_TEST (schema_mapping_no_defaults_test);
    CPPUNIT_TEST (add_class);
    CPPUNIT_TEST (apply_two);
    CPPUNIT_TEST (apply_three);
    CPPUNIT_TEST (add_class_no_identity);
    CPPUNIT_TEST (switch_schema);
    CPPUNIT_TEST (destroy_schema);
    CPPUNIT_TEST (non_ascii_property_name);
    CPPUNIT_TEST (non_ascii_class_name1);
    CPPUNIT_TEST (non_ascii_class_name2);
    CPPUNIT_TEST (non_ascii_schema_name);
#ifdef _WIN32
    CPPUNIT_TEST (non_ascii_directory);
#endif
    CPPUNIT_TEST (modify_class);
    CPPUNIT_TEST (ignore_states);
    CPPUNIT_TEST (describe_Fix784301);
    CPPUNIT_TEST (read_only_files);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SchemaTests (void);
    virtual ~SchemaTests (void);
	void setUp ();
	void tearDown ();

protected:
	void describe();
    void describe_bogus ();
    void apply ();
    void apply_nameless ();
    void create_nameless ();
    void apply_two ();
    void apply_three ();

    // Executes a describe schema mapping (with default values, no config file) command.
    void schema_mapping_defaults_test();

    // Executes a describe schema mapping (without default values, no config file) command.
    void schema_mapping_no_defaults_test();

    // Test adding a class to the default schema
    void add_class ();

    // Test adding a class with no identity property to the default schema
    void add_class_no_identity ();

    // Test default file location with differing .prj files
    void switch_schema ();

    // Test destroying a schema
    void destroy_schema ();

    // Test different characters in property names
    void non_ascii_property_name ();

    // Test different characters in class names
    void non_ascii_class_name1 ();
    void non_ascii_class_name2 ();

    // Test different characters in schema names
    void non_ascii_schema_name ();

    // Test different characters in a directory name
    void non_ascii_directory ();

    // Test modifying a class:
    void modify_class();

    // Test the ignorestates flag:
    void ignore_states();

	void apply_schema();
    void describe_Fix784301 ();
    void read_only_files ();


    // Helper methods
    ///////////////////////////////////////////////////////////////////////////

    // Prints to stdout verbose info about the given schema collection:
	void show_schema (FdoFeatureSchemaCollection* schemas);

    // Helper method; iterates the given schema mappings, comparing the mappings to 
    // the logical schema (if one is given).
    void _TestSchemaMappings(FdoPhysicalSchemaMappingCollection* mappings, FdoFeatureSchemaCollection* logicalSchemas);

    // sets a file-set's read-write access mode:
    void SetShpFileAccess(FdoString* shpPath, bool bReadWrite);

    // apply and describe schema, then compare results
    void apply_helper(bool bUseSpecificGeometryTypes);
};

#endif // SCHEMATESTS_H

