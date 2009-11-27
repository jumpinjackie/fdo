 //
 // Copyright (C) 2004-2009  Autodesk, Inc.
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

#ifndef SQLCmdTest_H
#define SQLCmdTest_H

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class SQLCmdTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( SQLCmdTest );
    
    CPPUNIT_TEST( TestSimpleBind );

    CPPUNIT_TEST( TestUpdateWithBind );

    CPPUNIT_TEST( TestDeleteWithBind );

    CPPUNIT_TEST( TestSelectWithBind );

    CPPUNIT_TEST( TestExpressionWithBind );

    CPPUNIT_TEST( TestComplexBind );

    CPPUNIT_TEST( TestErrorMessageSimpleBind );

    CPPUNIT_TEST( TestSelectWithTrans );

    CPPUNIT_TEST_SUITE_END();

public:
    SQLCmdTest(void);
    virtual ~SQLCmdTest(void);
    void setUp ();
	void tearDown ();

    void TestSimpleBind ();

    void TestUpdateWithBind ();

    void TestDeleteWithBind ();

    void TestSelectWithBind ();

    void TestExpressionWithBind ();

    void TestComplexBind();
    
    void TestErrorMessageSimpleBind();

    void TestSelectWithTrans();
};

#endif	//SQLCmdTest_H
