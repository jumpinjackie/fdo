 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef INSERTTEST_H
#define INSERTTEST_H

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class InsertTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( InsertTest );
    CPPUNIT_TEST( TestInsInvalidGeomTypes );
    CPPUNIT_TEST_SUITE_END();

public:
    InsertTest(void);
    virtual ~InsertTest(void);
    void setUp ();
	void tearDown ();

    void TestInsInvalidGeomTypes ();
};

#endif	//INSERTTEST_H
