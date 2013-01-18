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

#ifndef _SCHEMATEST_H_
#define _SCHEMATEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include <TestCommonMiscUtil.h>

/* 
 * The unit test for schema.
 */

class SchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( SchemaTest );
  CPPUNIT_TEST( TestMultipleGeom );
  CPPUNIT_TEST_SUITE_END();

public:
    SchemaTest(void);
    virtual ~SchemaTest(void);
    void setUp ();
	void tearDown ();

    void TestMultipleGeom();

private:
    void OpenConnection(FdoIConnection* conn, const wchar_t* path , bool placeDQ = false);
};

#endif

