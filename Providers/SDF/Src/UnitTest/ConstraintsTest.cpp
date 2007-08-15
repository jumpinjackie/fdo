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

#include "stdafx.h"
#include "ConstraintsTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ConstraintsTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConstraintsTest, "ConstraintsTest");

#ifdef _WIN32
static const wchar_t* CONSTRAINTS_TEST_FILE = L"..\\..\\TestData\\ConstraintsTest.SDX";
#else
#include <unistd.h>
static const wchar_t* CONSTRAINTS_TEST_FILE = L"../../TestData/ConstraintsTest.SDX";
#endif

ConstraintsTest::ConstraintsTest()
{
}

ConstraintsTest::~ConstraintsTest()
{
}

#if 0
void ConstraintsTest::setUp ()
{
}
#endif

void ConstraintsTest::CreateConnection( Context& context, FdoBoolean recreateDb )
{
    context.connection = UnitTestUtil::OpenConnection( CONSTRAINTS_TEST_FILE, recreateDb );
}

FdoBoolean ConstraintsTest::CanRestrictCheckConstraint()
{
    return false;
}
