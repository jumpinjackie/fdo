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

#ifndef READONLYTESTS_H
#define READONLYTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class ReadOnlyTests :
    public ShpTests
{
    /// Note: theses tests are commented out because they require a floppy disk
    /// (with the read-only tab set) that contains:
    /// D:\>dir a:\
    ///  Volume in drive A is SHP SAMPLES
    ///  Volume Serial Number is AC6F-4ADF
    ///
    ///  Directory of a:\
    ///
    /// 18/04/2005  09:46 AM            27,644 ontario.shp
    /// 18/04/2005  09:46 AM               108 ontario.shx
    /// 18/04/2005  09:46 AM               403 readme.txt
    /// 18/04/2005  09:46 AM               209 ontario.dbf
    ///                4 File(s)         28,364 bytes
    ///                0 Dir(s)       1,428,480 bytes free
    CPPUNIT_TEST_SUITE (ReadOnlyTests);
    //CPPUNIT_TEST (select);
    //CPPUNIT_TEST (apply_schema);
    //CPPUNIT_TEST (insert);
    //CPPUNIT_TEST (update);
    //CPPUNIT_TEST (del);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    ReadOnlyTests (void);
    virtual ~ReadOnlyTests (void);
	void setUp ();
	void tearDown ();

protected:
	void select();
    void apply_schema ();
    void insert ();
    void update ();
    void del ();

private:
    void create_schema (FdoGeometricType type, bool elevation, bool measure);
    bool contains (const wchar_t* pattern, const wchar_t* string);
};

#endif // READONLYTESTS_H

