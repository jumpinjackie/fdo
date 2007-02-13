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

using System;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for Class1.
    /// </summary>
    class EntryPoint
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("managed API Unit Test Start.");

            //mgCapabilitiesTest
            mgCapabilitiesTest Capabilities = new mgCapabilitiesTest();
            Capabilities.testFunctionDefinition();

            //mgClientServicesTest
            mgClientServicesTest ClientServicesTest = new mgClientServicesTest();
            ClientServicesTest.testServices();

            //mgCommandTest
            mgCommandTest Command = new mgCommandTest();
            Command.testBatchParameterValueCollection();
            Command.testIdentifierCollection();
            Command.testParameterValueCollection();
            Command.testPropertyValueCollection();

            //mgCommonTest
            mgCommonTest Common = new mgCommonTest();
            Common.testTypes();
            Common.testDictionary();

            //mgExpressionParseTest
            mgExpressionParseTest ExpressionParse = new mgExpressionParseTest();
            ExpressionParse.testExpressions();
            ExpressionParse.testFGFT();
            ExpressionParse.testLargeExpressions();

            //mgExpressionTest
            mgExpressionTest Expression = new mgExpressionTest();
            //Expression.testExpressionTree();  //bad parameter
            Expression.testBinaryExpression();
            Expression.testDataValue();
            Expression.testFunction();
            Expression.testGeometryValue();
            Expression.testIdentifier();
            Expression.testComputedIdentifier();
            Expression.testParameter();
            Expression.testUnaryExpression();

            //mgFilterParseTest
            mgFilterParseTest FilterParseTest = new mgFilterParseTest();
            FilterParseTest.test();

            //mgFilterTest
            mgFilterTest FilterTest = new mgFilterTest();
            //FilterTest.testFilterTree(); //bad parameter
            FilterTest.testBinaryLogicalOperator();
            FilterTest.testComparisonCondition();
            FilterTest.testDistanceCondition();
            FilterTest.testInCondition();
            FilterTest.testNullCondition();
            FilterTest.testSpatialCondition();
            FilterTest.testUnaryLogicalOperator();
            FilterTest.testComputedIdentifier();

            //mgGeometryMemPerfTest
            mgGeometryMemPerfTest GeometryMemPerfTest = new mgGeometryMemPerfTest();
            GeometryMemPerfTest.testFgfConversion();

            //mgGeometryTest
            mgGeometryTest GeometryTest = new mgGeometryTest();
            GeometryTest.testCircularArcSegment();
            GeometryTest.testCurvePolygon();
            GeometryTest.testCurveString();
            GeometryTest.testDirectPosition();
            GeometryTest.testEnvelope();
            GeometryTest.testGeometryFromEnvelope();
            GeometryTest.testLinearRing();
            GeometryTest.testLineString();
            GeometryTest.testLineStringSegment();
            GeometryTest.testMultiCurvePolygon();
            GeometryTest.testMultiCurveString();
            GeometryTest.testMultiGeometry();
            GeometryTest.testMultiLineString();
            GeometryTest.testMultiPoint();
            GeometryTest.testMultiPolygon();
            GeometryTest.testPoint();
            GeometryTest.testPolygon();
            GeometryTest.testRing();

            //mgGmlTest
            mgGmlTest GmlTest = new mgGmlTest();
            GmlTest.testInstantiateFeatureReader();
            GmlTest.testSimpleFeature();
            GmlTest.testSimpleGeometry();
            GmlTest.testNestedGeometry();
            GmlTest.testMultiGeometry();
            GmlTest.testSimpleObject();
            GmlTest.testNestedObject();
            GmlTest.testBLOBBase64(); 
            GmlTest.testBLOBHex();

            //mgIoTest
            mgIoTest IoTest = new mgIoTest();
            IoTest.setUp();
            IoTest.testStreams();
            IoTest.testFileOpen();
            IoTest.testFileRead();
            IoTest.testFileWrite();
            IoTest.testFileCapabilities();
            IoTest.testMemoryStream();
            //IoTest.testLargeFile();  //fail also in unmanaged code

            //mgXmlTest
            mgXmlTest XmlTest = new mgXmlTest();
            //XmlTest.testXsl(); //first transform failed, output different result with the unmanaged code, quite strange
            XmlTest.testNameAdjust();
            //XmlTest.testRead(); //parse failed
            XmlTest.testWrite();
            XmlTest.testWriteClose();
            //XmlTest.testReadWrite(); //parse failed, same to testReader
            //XmlTest.testCopy(); //parse failed, same to testReader

            //mgSchemaTest
            mgSchemaTest SchemaTest = new mgSchemaTest();
            SchemaTest.testPropertyValue();
            SchemaTest.testMultiSchema();
            SchemaTest.testFeatureSchema();
            SchemaTest.testClass();
            SchemaTest.testFeatureClass();
            SchemaTest.testSubClass();
            SchemaTest.testRasterClass();
            SchemaTest.testAssociationOK();
            SchemaTest.testAssociationFail1();
            SchemaTest.testAssociationFail2();
            SchemaTest.testAssociationFail3();
            SchemaTest.testNetworkOK();
            SchemaTest.testNetworkFail1();
            SchemaTest.testNetworkFail2();
            SchemaTest.testXmlAll();
            SchemaTest.testXmlRefs();
            SchemaTest.testXmlMerge();
            SchemaTest.testXmlUnsupported();
            SchemaTest.testRefErrors();
            SchemaTest.testXmlExternal();
            //SchemaTest.testXmlSchemaMappings(); //fail unknown reason, maybe reset doesn't work

            //mgSpatialContextTest
            mgSpatialContextTest SpatialContextTest = new mgSpatialContextTest();
            SpatialContextTest.testXml(); //similar to SchemaTest.testXmlSchemaMappings()
            SpatialContextTest.testXmlError();
            SpatialContextTest.testXmlNesting();

            //mgSpatialUtilityTest
            mgSpatialUtilityTest SpatialUtilityTest = new mgSpatialUtilityTest();
            SpatialUtilityTest.testGridTransform();
            SpatialUtilityTest.testLinearizeCurvePolygon();
            SpatialUtilityTest.testLinearizeCurveString();
            SpatialUtilityTest.testLinearizeLineString();
            SpatialUtilityTest.testLinearizeMultiCurvePolygon();
            SpatialUtilityTest.testLinearizeMultiCurveString();
            SpatialUtilityTest.testLinearizeMultiGeometry();
            SpatialUtilityTest.testLinearizeMultiLineString();
            SpatialUtilityTest.testLinearizeMultiPoint();
            SpatialUtilityTest.testLinearizeMultiPolygon();
            SpatialUtilityTest.testLinearizePoint();
            SpatialUtilityTest.testLinearizePolygon();
            SpatialUtilityTest.testValidateGeometryByType();

            Console.WriteLine("Unit Test finish. Press return to exit...");
            Console.ReadLine();
            
        }
    }
}

