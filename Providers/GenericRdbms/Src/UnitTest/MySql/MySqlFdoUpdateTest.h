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

#ifndef MYSQL_FDOUPDATETEST_H
#define MYSQL_FDOUPDATETEST_H

#include "FdoUpdateTest.h"

class MySqlFdoUpdateTest : public FdoUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (MySqlFdoUpdateTest, FdoUpdateTest);
// Test no longer applicable due to Ticket#220 change.
// It might be re-instated in the future if specific geometry type validation is re-introduced.
//    CPPUNIT_TEST( testForeignDataStore );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();
    virtual FdoStringP TableIdGeomName() {return L"t\xe4ble id geom";}

public:
	virtual void ConditionalUpdate () { }
    void testForeignDataStore();

protected:
    virtual FdoString* NoMetaSuffix();
	static const wchar_t* mForeignPolygonTest[];
};

#endif // MYSQL_FDOUPDATETEST_H
