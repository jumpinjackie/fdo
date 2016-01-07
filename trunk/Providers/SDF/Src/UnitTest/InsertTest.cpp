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
#include "InsertTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( InsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InsertTest, "InsertTest");

#ifdef _WIN32
static const wchar_t* INSERT_TEST_FILE = L"..\\..\\TestData\\InsertTest";
#else
#include <unistd.h>
static const wchar_t* INSERT_TEST_FILE = L"../../TestData/InsertTest";
#endif

InsertTest::InsertTest()
{
}

InsertTest::~InsertTest()
{
}

void InsertTest::CreateConnection( Context& context, FdoBoolean recreateDb )
{
    FdoStringP fileName = INSERT_TEST_FILE;
    fileName += context.mSuffix + L".SDX";

    context.connection = UnitTestUtil::OpenConnection( fileName, recreateDb );
}

bool InsertTest::InsertsDefaultValues()
{
    return false;
}

FdoDataType InsertTest::GetAutoIncrementType()
{
    return FdoDataType_Int32;
}
