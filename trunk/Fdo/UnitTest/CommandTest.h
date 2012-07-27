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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_COMMANDTEST_H
#define CPP_UNIT_COMMANDTEST_H

// Test cases for commands
class CommandTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testBatchParameterValueCollection);
    FDO_CPPUNIT_DEFINE(testIdentifierCollection);
    FDO_CPPUNIT_DEFINE(testParameterValueCollection);
    FDO_CPPUNIT_DEFINE(testPropertyValueCollection);

    CPPUNIT_TEST_SUITE(CommandTest);
    CPPUNIT_TEST(testBatchParameterValueCollection);
    CPPUNIT_TEST(testIdentifierCollection);
    CPPUNIT_TEST(testParameterValueCollection);
    CPPUNIT_TEST(testPropertyValueCollection);
    CPPUNIT_TEST_SUITE_END();

public:
    CommandTest(void);
    virtual ~CommandTest(void);
    void setUp ();

protected:
    void testBatchParameterValueCollection();
    void testIdentifierCollection();
    void testParameterValueCollection();
    void testPropertyValueCollection();
};

#endif

