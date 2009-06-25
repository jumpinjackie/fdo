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

#ifndef _TRANSACTIONTEST_H_
#define _TRANSACTIONTEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

class TransactionTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( TransactionTest );
  CPPUNIT_TEST( TestStartTransaction );
  CPPUNIT_TEST( TestCommit );
  CPPUNIT_TEST( TestRollback );
  CPPUNIT_TEST_SUITE_END();
public:
    TransactionTest(void);
    ~TransactionTest(void);
    void setUp ();
	void tearDown ();

    void TestStartTransaction();
    void TestCommit();
    void TestRollback();

private:
    void InsertOneValue(FdoIConnection* conn, FdoString* nameProp, FdoValueExpression* value);
    bool CheckForValue(FdoIConnection* conn, FdoString* filter);
};

#endif
