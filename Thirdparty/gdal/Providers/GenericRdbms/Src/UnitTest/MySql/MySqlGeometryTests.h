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

#ifndef MYSQL_GEOMETRYTESTS_H
#define MYSQL_GEOMETRYTESTS_H

#include "GeometryTests.h"
extern initializer mysql_rdbi_init;

class MySqlGeometryTests : public GeometryTests
{
    CPPUNIT_TEST_SUB_SUITE (MySqlGeometryTests, GeometryTests);
    CPPUNIT_TEST_SUITE_END ();

    void set_provider();

    int do_rdbi_init ()
    {
#ifdef RDBI_STATIC
        return (rdbi_init (&mRdbiContext, mysql_rdbi_init));
#else
        return (rdbi_initialize (&mRdbiContext, "MySqlDriver"));
#endif
    }
    char *get_bind_var (int n){ return ("?"); }
	char *get_geometry_type (){ return ("geometry"); }

	int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    
	virtual char *get_date_time (const struct tm *when);

    char *get_geometry_text (FdoIGeometry *geometry);
    int do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry );
};

#endif // MYSQL_GEOMETRYTESTS_H
