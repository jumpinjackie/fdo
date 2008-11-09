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

#ifndef SQLSERVER_DATATYPETESTS_H
#define SQLSERVER_DATATYPETESTS_H

#include "DataTypeTests.h"

class SqlServerDataTypeTests :	public DataTypeTests
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerDataTypeTests, DataTypeTests);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    int do_rdbi_init ();
    int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    char *get_bind_var (int n, int rdbiType)
    {
        if ( rdbiType == RDBI_GEOMETRY ) 
        {
            return ("geometry::STGeomFromWKB(?,0)");
        }
        else
        {
            return ("?");
        }
    

    }
	virtual char *get_geometry_type() { return "GEOMETRY"; }
	char *get_datetime_type() { return "DATETIME"; }
	bool allow_timestamp_notnull() { return false; }
    char *get_date_time (const struct tm *when);
    virtual char *get_select_statement(int rdbiType) {
        if ( rdbiType == RDBI_GEOMETRY )
        {
            return "select xyz1.STAsBinary() as xyz1, xyz2.STAsBinary() as xyz2 from bar";
        }
        else
        {
            return "select * from bar";
        }
    }
        
};

#endif //SQLSERVER_DATATYPETESTS_H
