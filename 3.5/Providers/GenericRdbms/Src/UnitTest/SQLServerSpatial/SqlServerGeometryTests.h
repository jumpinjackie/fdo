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

#ifndef SQLSERVER_GEOMETRYTESTS_H
#define SQLSERVER_GEOMETRYTESTS_H

#include "GeometryTests.h"

class SqlServerGeometryTests : public GeometryTests
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerGeometryTests, GeometryTests);
    CPPUNIT_TEST_SUITE_END ();

    void set_provider();

    int do_rdbi_init ();
    int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
	char *get_geometry_text (FdoIGeometry *geometry);
	int do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry );
    
    char *get_bind_var (int n){ return (n != 2 ? "?" : "geometry::STGeomFromWKB(?, 0)");}
    char *get_geom_conv () { return ".STAsBinary()"; };
    char *get_geometry_type (){return ("Geometry");}
};

#endif // SQLSERVER_GEOMETRYTESTS_H
