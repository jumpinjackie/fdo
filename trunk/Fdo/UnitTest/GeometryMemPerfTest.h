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

#ifndef _GEOMETRYMEMPERFTEST_H_
#define _GEOMETRYMEMPERFTEST_H_


#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_GEOMETRYMEMPERFTEST_H
#define CPP_UNIT_GEOMETRYMEMPERFTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

/* 
 * A test case for the in-memory performance of Geometry API.
 */

class GeometryMemPerfTest : public CppUnit::TestCase
{
  FDO_CPPUNIT_DEFINE(testFgfConversion);

  CPPUNIT_TEST_SUITE( GeometryMemPerfTest );
  CPPUNIT_TEST( testFgfConversion );
  CPPUNIT_TEST_SUITE_END();

public:
    GeometryMemPerfTest(void);
    virtual ~GeometryMemPerfTest(void);
    void setUp ();

    void testFgfConversion ();
};

#endif
#endif

