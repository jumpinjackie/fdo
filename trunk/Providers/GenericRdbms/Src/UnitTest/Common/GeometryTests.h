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

#ifndef CPP_UNIT_GEOMETRYTESTS_H
#define CPP_UNIT_GEOMETRYTESTS_H

#include <Inc/Rdbi/proto.h>

class GeometryTests :
    public CppUnit::TestCase
{
protected:
    rdbi_context_def* mRdbiContext;
    int mId;
    char    mSql[1024];


private:
    CPPUNIT_TEST_SUITE (GeometryTests);
    CPPUNIT_TEST (ddl);
    CPPUNIT_TEST (define);
    CPPUNIT_TEST (bind);
    CPPUNIT_TEST_SUITE_END ();

public:
    GeometryTests (void);
    virtual ~GeometryTests (void);
	void setUp ();
	void tearDown ();

protected:
	void ddl ();
	void define ();
	void bind ();
	void describe ();
    int rdbi_sql_Ex( rdbi_context_def *context, int sqlid, FdoStringP sql );

    virtual void set_provider() {  CPPUNIT_FAIL ("naked set_provider() called"); };
    virtual int do_rdbi_init ();
    virtual int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    virtual char *get_bind_var (int n);
    virtual char *get_geom_conv () { return ""; };
    virtual char *get_date_time (const struct tm *when);
    virtual char *get_geometry_type ();
    virtual char *get_geometry_text (FdoIGeometry *geometry);
    virtual int  do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry );
};

#endif // CPP_UNIT_GEOMETRYTESTS_H

