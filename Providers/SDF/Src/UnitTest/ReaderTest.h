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

#ifndef ReaderTest_H
#define ReaderTest_H

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class ReaderTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( ReaderTest );

    CPPUNIT_TEST( TestDataReaderWithIndex );

    CPPUNIT_TEST( TestFeatureReaderWithIndex );

    //CPPUNIT_TEST( TestSqlDataReaderWithIndex );

    CPPUNIT_TEST( TestReaderMatchClassDefinition );

    CPPUNIT_TEST_SUITE_END();

public:
    ReaderTest(void);
    virtual ~ReaderTest(void);
    void setUp ();
	void tearDown ();

    void TestDataReaderWithIndex ();

    void TestFeatureReaderWithIndex ();

    //void TestSqlDataReaderWithIndex();

    void TestReaderMatchClassDefinition();

private:
    void Prepare();
    void PrepareConnection();
    void PrepareSchema();
    void PrepareData();

    void Done();

    bool SupportsFunction(FdoString* functionName);

    FdoPtr<FdoIConnection> m_connection;
};

#endif	//ReaderTest_H
