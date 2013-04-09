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

#ifndef CPP_UNIT_FILEACCESSTESTS_H
#define CPP_UNIT_FILEACCESSTESTS_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class FileAccessTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (FileAccessTests);
    CPPUNIT_TEST (open_shp);
    CPPUNIT_TEST (open_dbf);
    CPPUNIT_TEST (open_shx);
    CPPUNIT_TEST (object);
    CPPUNIT_TEST (create_shx);
    CPPUNIT_TEST (only_shp);
    CPPUNIT_TEST (delete_shp);
    CPPUNIT_TEST_SUITE_END ();

public:
    FileAccessTests (void);
    virtual ~FileAccessTests (void);

protected:
    void open_shp ();
    void open_dbf ();
    void open_shx ();
    void object ();
    void create_shx ();
    void only_shp ();
    void delete_shp ();
};

#endif // CPP_UNIT_FILEACCESSTESTS_H


