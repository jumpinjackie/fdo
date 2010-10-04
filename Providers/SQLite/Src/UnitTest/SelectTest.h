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

#ifndef Select_TEST_H_
#define Select_TEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

/* 
 * Tests spatial contexts.
 * This is just an initial version to test a defect regression.
 * More tests would need to be added to provide full coverage.
 */

class SelectTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( SelectTest );
    CPPUNIT_TEST( TestSimpleSelect );
    CPPUNIT_TEST( TestBindSelect );
    CPPUNIT_TEST( TestSpatialSelect );
    CPPUNIT_TEST( TestComplexSelect );
    CPPUNIT_TEST(TestComplexWithBindSelect);
    CPPUNIT_TEST(BooleanDataTest);  
    CPPUNIT_TEST(TestAggregatesSelect);  
    CPPUNIT_TEST(TestSelectInsertLowerId);  
    CPPUNIT_TEST(TestSelectInsertLowerIdSQL);
    CPPUNIT_TEST(TestDualConnection);
    CPPUNIT_TEST(TestSelectMultipleCS);
    CPPUNIT_TEST(TestViewSelects);
    CPPUNIT_TEST(TestJoinViewSelects);
    CPPUNIT_TEST(TestSubSelect);
    CPPUNIT_TEST(TestJoin);
    CPPUNIT_TEST(TestMSelect);
    CPPUNIT_TEST(TestJoinAggregatesSelect);
    CPPUNIT_TEST(TestMAggregatesSelect);
    CPPUNIT_TEST(TestSpatialJoins);
    CPPUNIT_TEST(TestSpatialJoinsWFilter1);
    CPPUNIT_TEST(TestSpatialJoinsWFilter2);
    CPPUNIT_TEST(TestSpatialJoinsWFilter3);
    CPPUNIT_TEST(TestSpatialJoinsComplexFilter);
    CPPUNIT_TEST(TestSpatialJoinsFdo);
    CPPUNIT_TEST(TestReleaseSchema);
    CPPUNIT_TEST(TestCrosses);
    CPPUNIT_TEST(TestOverlaps);
    CPPUNIT_TEST(TestTouches);
    CPPUNIT_TEST_SUITE_END();

private:
    int SelectObjects(FdoIConnection* conn, FdoString* txt, FdoString* clsName = NULL);
    int InsertValue(FdoIConnection* conn, FdoString* className, FdoString* valTxtFlt);
    void UpdateValue(FdoIConnection* conn, int rowid, FdoString* className, FdoString* valTxtFlt);
    void CreateFeatureClass(FdoIConnection* conn, FdoString* className, FdoString* scName);
    int SelectObjects(FdoIConnection* conn, FdoFilter* filter, FdoString* clsName, FdoString* id);
    void TestJoinType(FdoIConnection* conn, FdoJoinType jtype, int expCount);
    void TestJoinTypeAggregates (FdoIConnection* conn, FdoJoinType jtype, int expMax);
    int SelectSpatial(FdoIConnection* conn, FdoString* sql);
    void TestSpatialJoinType(FdoIConnection* conn, FdoJoinType jtype, int expCount);
    int SelectSpatial(FdoIConnection* conn, FdoFilter* filter);

public:
    SelectTest(void);
    virtual ~SelectTest(void);
    void setUp ();
	void tearDown ();

    void TestSimpleSelect ();
    void TestBindSelect ();
    void TestSpatialSelect ();
    void TestComplexSelect ();
    void TestComplexWithBindSelect();
    void BooleanDataTest ();
    void TestAggregatesSelect ();
    void TestSelectInsertLowerId ();
    void TestSelectInsertLowerIdSQL ();
    void TestDualConnection ();
    void TestSelectMultipleCS ();
    void TestViewSelects ();
    void TestJoinViewSelects ();
    void TestSubSelect ();
    void TestJoin ();
    void TestMSelect ();
    void TestJoinAggregatesSelect ();
    void TestMAggregatesSelect ();
    void TestSpatialJoins ();
    void TestSpatialJoinsWFilter1 ();
    void TestSpatialJoinsWFilter2 ();
    void TestSpatialJoinsWFilter3 ();
    void TestSpatialJoinsComplexFilter ();
    void TestSpatialJoinsFdo ();
    void TestReleaseSchema ();
    void TestCrosses ();
    void TestOverlaps ();
    void TestTouches ();
};

#endif
