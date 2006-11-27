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

#ifndef CPP_UNIT_SCHEMATEST_H
#define CPP_UNIT_SCHEMATEST_H

// Test cases for the Schema classes.
class SchemaTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testPropertyValue);
    FDO_CPPUNIT_DEFINE(testFeatureSchema);
    FDO_CPPUNIT_DEFINE(testMultiSchema);
    FDO_CPPUNIT_DEFINE(testAssociationOK);
    FDO_CPPUNIT_DEFINE(testAssociationFail1);
    FDO_CPPUNIT_DEFINE(testAssociationFail2);
    FDO_CPPUNIT_DEFINE(testAssociationFail3);
    FDO_CPPUNIT_DEFINE(testClass);
    FDO_CPPUNIT_DEFINE(testFeatureClass);
    FDO_CPPUNIT_DEFINE(testSubClass);
    FDO_CPPUNIT_DEFINE(testRasterClass);
    FDO_CPPUNIT_DEFINE(testXmlV2);
    FDO_CPPUNIT_DEFINE(testXmlAll);
    FDO_CPPUNIT_DEFINE(testXmlRefs);
    FDO_CPPUNIT_DEFINE(testXmlMerge);
    FDO_CPPUNIT_DEFINE(testXmlMultiDoc);
    FDO_CPPUNIT_DEFINE(testXmlUnsupported);
    FDO_CPPUNIT_DEFINE(testXmlExternal);
    FDO_CPPUNIT_DEFINE(testXmlSchemaMappings);
    FDO_CPPUNIT_DEFINE(testNetworkOK);
    FDO_CPPUNIT_DEFINE(testNetworkFail1);
    FDO_CPPUNIT_DEFINE(testNetworkFail2);
    FDO_CPPUNIT_DEFINE(testRefErrors);

    CPPUNIT_TEST_SUITE(SchemaTest);

    // concrete classes
    CPPUNIT_TEST(testPropertyValue);

    // concrete classes derived from SchemaElement
    CPPUNIT_TEST(testFeatureSchema);
    CPPUNIT_TEST(testMultiSchema);
    CPPUNIT_TEST(testAssociationOK);
    CPPUNIT_TEST(testAssociationFail1);
    CPPUNIT_TEST(testAssociationFail2);
    CPPUNIT_TEST(testAssociationFail3);
    // concrete classes derived from ClassDefinition
    CPPUNIT_TEST(testClass);
    CPPUNIT_TEST(testFeatureClass);

	// tests setting id properties on subclasses.
    CPPUNIT_TEST(testSubClass);

    CPPUNIT_TEST(testRasterClass);
//TODO:R2    CPPUNIT_TEST(testLineSegmentClass);
//TODO:R2    CPPUNIT_TEST(testTinEdgeClass);
//TODO:R2    CPPUNIT_TEST(testTinPointClass);
//TODO:R2    CPPUNIT_TEST(testTopologyFeatureClass);

    // concrete classes derived from CoverageFeatureClass
//TODO:R2    CPPUNIT_TEST(testTinFeatureClass);

    // concrete classes derived from SegmentFeatureClass
//TODO:R2    CPPUNIT_TEST(testLineSegmentFeatureClass);
//TODO:R2    CPPUNIT_TEST(testPointSegmentFeatureClass);

    CPPUNIT_TEST(testXmlV2);
    CPPUNIT_TEST(testXmlAll);
    CPPUNIT_TEST(testXmlRefs);
    CPPUNIT_TEST(testXmlMerge);
    CPPUNIT_TEST(testXmlMultiDoc);
    CPPUNIT_TEST(testXmlUnsupported);
    CPPUNIT_TEST(testXmlExternal);
    CPPUNIT_TEST(testXmlSchemaMappings);
    CPPUNIT_TEST(testNetworkOK);
    CPPUNIT_TEST(testNetworkFail1);
    CPPUNIT_TEST(testNetworkFail2);
    CPPUNIT_TEST(testRefErrors);
    CPPUNIT_TEST_SUITE_END();

public:
    SchemaTest(void);
    virtual ~SchemaTest(void);
    void setUp ();

protected:
    // base classe
    void testSchemaElement(FdoSchemaElement* pElement, FdoString* pszName, FdoString* pszDesc);

    // base class derived from SchemaElement
    void testClassDefinition(FdoClassDefinition* pClassDef, FdoString* pszName, FdoString* pszDesc);

    // base classes derived from ClassDefinition
//TODO:R2    void testCoverageFeatureClass(FdoCoverageFeatureClass* pCoverageFeature, FdoString* pszName, FdoString* pszDesc);
//TODO:R2    void testSegmentFeatureClass(FdoSegmentFeatureClass* pSegFeature, FdoString* pszName, FdoString* pszDesc);

    // concrete classes
    void testPropertyValue();

    // concrete classes derived from SchemaElement
    void testFeatureSchema();
    void testMultiSchema();
    void testPropertyDefinition();
    void testAssociationOK();
    void testAssociationFail1();
    void testAssociationFail2();
    void testAssociationFail3();

    // concrete classes derived from ClassDefinition
    void testClass();
    void testFeatureClass();
    void testSubClass();
    void testRasterClass ();
//TODO:R2    void testLineSegmentClass();
//TODO:R2    void testTinEdgeClass();
//TODO:R2    void testTinPointClass();
//TODO:R2    void testTopologyFeatureClass();

    // concrete classes derived from CoverageFeatureClass
//TODO:R2    void testTinFeatureClass();

    // concrete classes derived from SegmentFeatureClass
//TODO:R2    void testLineSegmentFeatureClass();
//TODO:R2    void testPointSegmentFeatureClass();

    // TODO: test ReadOnlyPropertyDefinition, DataPropertyDefinition, GeometricPropertyDefinition, 
    // ObjectPropertyDefinition, PropertyValueCollection and SchemaException??

    // XML Serialization and Deserialization tests:

    // Schema with old (version 2) fdo namespace.
    void testXmlV2();

    // Multiple Feature Schemas at once
    void testXmlAll();

    // Serialize/Deserialize Feature Schemas with cross-schema references.
    // References are by base class, object property class and 
    // associated class.
    void testXmlRefs();

    // Merge mods from XML into existing Feature Schemas.
    void testXmlMerge();

    void testXmlMultiDoc();

    // Test reading schema with unsupported elements.
    void testXmlUnsupported();

    void testXmlExternal();

    void testXmlSchemaMappings();

    void testNetworkOK();
    void testNetworkFail1();
    void testNetworkFail2();

    // Test creating a schema and then removing classes without removing their
    // references (e.g. references by Object Properties). When this happens, the 
    // referencers point to classes that aren't in schemas.
    // The schema is then serialized; the serialization should trap and report these
    // bad references.
    void testRefErrors();

    void eachUnsupported( FdoXmlFlags::ErrorLevel level, char* masterFile, char* outFile);
    FdoFeatureSchema* createAcadSchema();
    FdoFeatureSchema* createElectricSchema( FdoFeatureSchema* pAcadSchema );
    FdoFeatureSchema* createRefSchema( FdoInt32 idx, FdoFeatureSchema* pBaseSchema = NULL );
};

#endif

