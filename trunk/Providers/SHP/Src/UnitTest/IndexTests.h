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

#ifndef CPP_UNIT_INDEXTESTS_H
#define CPP_UNIT_INDEXTESTS_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class IndexTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (IndexTests);
    CPPUNIT_TEST (create);
    CPPUNIT_TEST (populate);
    CPPUNIT_TEST (query);
    CPPUNIT_TEST (check_z);
    CPPUNIT_TEST (check_limits);
    CPPUNIT_TEST (check_remove);
    CPPUNIT_TEST (check_defragment);
    CPPUNIT_TEST_SUITE_END ();

public:
    IndexTests (void);
    virtual ~IndexTests (void);

protected:
    void create ();
    void populate ();
    void query ();
    void check_z ();
    void check_limits ();
    void check_remove ();
    void check_defragment ();
};

#endif // CPP_UNIT_INDEXTESTS_H


