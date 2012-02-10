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

#ifndef MYSQL_DATATYPETESTS_H
#define MYSQL_DATATYPETESTS_H

#include "DataTypeTests.h"

class MySqlDataTypeTests :	public DataTypeTests
{
    CPPUNIT_TEST_SUB_SUITE (MySqlDataTypeTests, DataTypeTests);
    CPPUNIT_TEST_SUITE_END ();

    virtual void single_char ();
    virtual void single_character ();

    void set_provider();
    int do_rdbi_init ();
    int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    char *get_bind_var (int n, int rdbiType)
	{
		return ("?");
	}
    bool is_datatype_equal(int rdbi_type, int type);
	virtual char *get_geometry_type() { return "GEOMETRY"; }
	virtual char *get_geometry_storage() { return " engine=MyIsam"; }
    virtual char *get_date_time (const struct tm *when);
};

#endif // MYSQL_DATATYPETESTS_H
