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

#include "Pch.h"
#include "OdbcConnectionUtil.h"
#include "UnitTestUtil.h"
#include "MessageTest.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcMySqlTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcSqlServerTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcTextTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcAccessTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcOracleTests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "OdbcExcelTests");

static OdbcConnectionUtil pOdbcConnectionUtil;
ConnectionUtil* UnitTestUtil::InfoUtilConnection = &pOdbcConnectionUtil;
FdoString *UnitTestUtil::ActiveProviderName = L"OSGeo.ODBC.3.3";
