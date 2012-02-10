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

#ifndef MYSQL_INSERTTESTS_H
#define MYSQL_INSERTTESTS_H

#include "FdoInsertTest.h"
#include "../SchemaMgr/Ph/Mgr.h"

class MySqlFdoInsertTest : public FdoInsertTest
{
    CPPUNIT_TEST_SUB_SUITE (MySqlFdoInsertTest, FdoInsertTest);
    CPPUNIT_TEST( insertBoundaryUnsigned );
    CPPUNIT_TEST( characterSets );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    void insertDate ();
    void charSetCreateTable( GdbiConnection* gdbiConnection, FdoString* charSet, FdoString* dataCharSet );
    void charSetTestTable( FdoIConnection* connection, FdoString* charSet, FdoString* dataCharSet, FdoString* stringVal, FdoString* charVal );
    void charSetVldValue( FdoString* className, FdoString* expectedVal, FdoString* val );

public:
	virtual void conditionalInsert() { }
    virtual void insertBoundaryUnsigned();

    // Verifies that data can be round tripped to tables with various character sets.
    virtual void characterSets();
};


#endif // MYSQL_INSERTTESTS_H
