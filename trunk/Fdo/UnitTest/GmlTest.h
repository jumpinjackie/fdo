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


#ifndef CPP_UNIT_GMLTEST_H
#define CPP_UNIT_GMLTEST_H

class FdoXmlFeatureReader;

// Test cases for the Schema classes.
class GmlTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testInheritedClass);
    FDO_CPPUNIT_DEFINE(testBLOBHex);
    FDO_CPPUNIT_DEFINE(testBLOBBase64);
    FDO_CPPUNIT_DEFINE(testInstantiateFeatureReader);
    FDO_CPPUNIT_DEFINE(testSimpleFeature);
    FDO_CPPUNIT_DEFINE(testNestedFeature);
    FDO_CPPUNIT_DEFINE(testNestedCollection);
    FDO_CPPUNIT_DEFINE(testSimpleGeometry);
    FDO_CPPUNIT_DEFINE(testNestedGeometry);
    FDO_CPPUNIT_DEFINE(testMultiGeometry);
    FDO_CPPUNIT_DEFINE(testSimpleObject);
    FDO_CPPUNIT_DEFINE(testNestedObject);
	FDO_CPPUNIT_DEFINE(testGML3SimpleGeometry);
	FDO_CPPUNIT_DEFINE(testGML3MultiGeometry);
	FDO_CPPUNIT_DEFINE(testLargeGeoms);
 
    CPPUNIT_TEST_SUITE(GmlTest);
	CPPUNIT_TEST(testGML3SimpleGeometry);
	CPPUNIT_TEST(testGML3MultiGeometry);
    CPPUNIT_TEST(testInheritedClass);
    CPPUNIT_TEST(testBLOBHex);
    CPPUNIT_TEST(testBLOBBase64);
    CPPUNIT_TEST(testInstantiateFeatureReader);
    CPPUNIT_TEST(testSimpleFeature);
    CPPUNIT_TEST(testNestedFeature);
    CPPUNIT_TEST(testNestedCollection);
    CPPUNIT_TEST(testSimpleGeometry);
    CPPUNIT_TEST(testNestedGeometry);
    CPPUNIT_TEST(testMultiGeometry);
    CPPUNIT_TEST(testSimpleObject);
    CPPUNIT_TEST(testNestedObject);
    CPPUNIT_TEST(testLargeGeoms);
    CPPUNIT_TEST_SUITE_END();

public:
    GmlTest(void);
    virtual ~GmlTest(void);
    void setUp ();

protected:
    void testInstantiateFeatureReader();
	void testSimpleFeature();
	void testNestedFeature();
	void testNestedCollection();
	void testSimpleGeometry();
	void testNestedGeometry();
	void testMultiGeometry();
	void testSimpleObject();
	void testNestedObject();
    void testBLOBBase64();
    void testBLOBHex();
    void testInheritedClass();

	void testGML3SimpleGeometry();
	void testGML3MultiGeometry();
    void testLargeGeoms();

private:
    void testGeometryProperty(FdoXmlFeatureReader* featureReader, FdoString* propName);
    void roundTrip(FdoString* GMLFileIn, FdoString* SchemaFile, FdoString* GMLFileOut );
};


#endif


