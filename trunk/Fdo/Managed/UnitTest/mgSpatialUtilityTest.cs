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
using System.Diagnostics;

using OSGeo.FDO.Common;
using OSGeo.FDO.Spatial;
using OSGeo.FDO.Geometry;

namespace Fdo_Test
{
	/// <summary
	/// Summary description for mgSpatialUtilityTest.
	/// </summary
	public class mgSpatialUtilityTest
	{
		public mgSpatialUtilityTest()
		{

		}

        public void testValidateGeometryByType()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testValidateGeometryByType");

            IGeometry geomA = Common.CreateCurvePolygon(0);

            GeometryType[] typesA =
            {
                GeometryType.GeometryType_Point,
                GeometryType.GeometryType_LineString,
                GeometryType.GeometryType_Polygon,
                GeometryType.GeometryType_MultiPoint,
                GeometryType.GeometryType_MultiLineString,
                GeometryType.GeometryType_MultiPolygon,
                GeometryType.GeometryType_MultiGeometry,
                GeometryType.GeometryType_CurveString,
                GeometryType.GeometryType_CurvePolygon,
                GeometryType.GeometryType_MultiCurveString,
                GeometryType.GeometryType_MultiCurvePolygon
            };

            GeometryComponentType[] compTypesA =
            {
                GeometryComponentType.GeometryComponentType_CircularArcSegment,
                GeometryComponentType.GeometryComponentType_LineStringSegment,
                GeometryComponentType.GeometryComponentType_LinearRing,
                GeometryComponentType.GeometryComponentType_Ring
            };

            int dimensionalityA = Common.Dimensionality_XY|Common.Dimensionality_Z|Common.Dimensionality_M;

            SpatialGeometryValidity validity = SpatialUtility.ValidateGeometryByType(geomA, typesA, compTypesA, dimensionalityA);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);

            // See if lack of arc support is noticed.

            GeometryComponentType[]    compTypesB =
            {
                GeometryComponentType.GeometryComponentType_LineStringSegment,
                GeometryComponentType.GeometryComponentType_LinearRing
            };

            validity = SpatialUtility.ValidateGeometryByType(geomA, typesA, compTypesB, dimensionalityA);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_InvalidButCanBeApproximated);

            // Try it without the needed approximation type, "Polygon".

            GeometryType[] typesB =
            {
                GeometryType.GeometryType_Point,
                GeometryType.GeometryType_LineString,
                //GeometryType.GeometryType_Polygon,
                GeometryType.GeometryType_MultiPoint,
                GeometryType.GeometryType_MultiLineString,
                GeometryType.GeometryType_MultiPolygon,
                GeometryType.GeometryType_MultiGeometry,
                GeometryType.GeometryType_CurveString,
                GeometryType.GeometryType_CurvePolygon,
                GeometryType.GeometryType_MultiCurveString,
                GeometryType.GeometryType_MultiCurvePolygon
            };

            validity = SpatialUtility.ValidateGeometryByType(geomA, typesB, compTypesB, dimensionalityA);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Invalid);

            // Test dimensionality failure.

            FgfGeometryFactory gf = new FgfGeometryFactory();
            IGeometry geomB = gf.CreateGeometry("POINT XYZ (5 3 2)");
            int dimensionalityB = Common.Dimensionality_XY;

            validity = SpatialUtility.ValidateGeometryByType(geomB, typesA, compTypesA, dimensionalityB);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_InvalidDueToDimensionality);

            // Common.Check that a failure due to dimensionality has precedence over a failure due
            // to typing.

            GeometryType[] typesC =
            {
                //GeometryType.GeometryType_Point,
                GeometryType.GeometryType_LineString,
                GeometryType.GeometryType_Polygon,
                GeometryType.GeometryType_MultiPoint,
                GeometryType.GeometryType_MultiLineString,
                GeometryType.GeometryType_MultiPolygon,
                GeometryType.GeometryType_MultiGeometry,
                GeometryType.GeometryType_CurveString,
                GeometryType.GeometryType_CurvePolygon,
                GeometryType.GeometryType_MultiCurveString,
                GeometryType.GeometryType_MultiCurvePolygon
            };

            int numTypesC = typesC.Length;

            validity = SpatialUtility.ValidateGeometryByType(geomB, typesC, compTypesA, dimensionalityB);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_InvalidDueToDimensionality);

            // Common.Check that not specifying component types results in them all being accepted.

            validity = SpatialUtility.ValidateGeometryByType(geomA, typesA, null, dimensionalityA);

            //Console.WriteLine("validity={0}", GeometryValidityToString(validity));
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);

            // Common.Check all supported geometry types.

            IGeometry pt =   gf.CreateGeometry("POINT (5 3)");
            IGeometry ls =   gf.CreateGeometry("LINESTRING (0 1, 2 3, 4 5)");
            IGeometry pg =   gf.CreateGeometry("POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
            IGeometry mpt =  gf.CreateGeometry("MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");
            IGeometry mls =  gf.CreateGeometry("MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");
            IGeometry mpg =  gf.CreateGeometry("MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
            IGeometry mg =   gf.CreateGeometry("GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
            IGeometry cs =   gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            IGeometry cpg =  gf.CreateGeometry("CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");
            IGeometry mcs =  gf.CreateGeometry("MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200, 203 202))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300, 303 302))))");
            IGeometry mcpg = gf.CreateGeometry("MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");

            validity = SpatialUtility.ValidateGeometryByType(pt, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(ls, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(pg, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mpt,typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mls,typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mpg,typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mg, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(cs, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(cpg,typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mcs,typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);
            validity = SpatialUtility.ValidateGeometryByType(mcpg, typesA, compTypesA, dimensionalityA);
            Common.CheckGeometryValidity(validity, SpatialGeometryValidity.SpatialGeometryValidity_Valid);

            //Console.WriteLine("finish mgSpatialUtilityTest.testValidateGeometryByType");
            
        }

        //////////////////////////////////////////////////////////////////////
        // Test each geometry type...


        //////////////////////////////////////////////////////////////////////
        // geometries...
        //
        // GeometryType.GeometryType_Point
        // GeometryType.GeometryType_LineString
        // GeometryType.GeometryType_Polygon
        // GeometryType.GeometryType_MultiPoint
        // GeometryType.GeometryType_MultiLineString
        // GeometryType.GeometryType_MultiPolygon
        // GeometryType.GeometryType_MultiGeometry
        // GeometryType.GeometryType_CurveString
        // GeometryType.GeometryType_CurvePolygon
        // GeometryType.GeometryType_MultiCurveString
        // GeometryType.GeometryType_MultiCurvePolygon
        //////////////////////////////////////////////////////////////////////

        public void testLinearizePoint()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizePoint");

	        double[] ordsXYZ = new double[3];
	        ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;

            FgfGeometryFactory gf = new FgfGeometryFactory();
	        IPoint pnt = gf.CreatePoint(Common.Dimensionality_XY|Common.Dimensionality_Z, ordsXYZ);
	        Common.DumpPoint(pnt);
	        Common.CheckFGFT(pnt, "POINT XYZ (5 3 2)");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(pnt, 1.0, 0.001, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "POINT XYZ (5 3 2)");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizePoint");
            
        }

        public void testLinearizeLineString()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeLineString");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        double[] ordsXY = new double[6];
	        ordsXY[0] = 0.0; ordsXY[1] = 1.0;
	        ordsXY[2] = 2.0; ordsXY[3] = 3.0;
	        ordsXY[4] = 4.0; ordsXY[5] = 5.0;
	        ILineString line1 = gf.CreateLineString(Common.Dimensionality_XY, 6, ordsXY);
	        Common.DumpLineString(line1);
	        Common.CheckFGFT(line1, "LINESTRING (0 1, 2 3, 4 5)");

            byte[] fgf = gf.GetFgf(line1);
            ILineString line2 = (ILineString)gf.CreateGeometryFromFgf(fgf);
	        Common.CheckFGFT(line2, "LINESTRING (0 1, 2 3, 4 5)");

	        byte[] wkb = gf.GetWkb(line1);
	        ILineString line3 = (ILineString) gf.CreateGeometryFromWkb(wkb);
	        Common.CheckFGFT(line3, "LINESTRING (0 1, 2 3, 4 5)");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(line1, 1.0, 0.001, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 1, 2 3, 4 5)");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeLineString");
            
        }

        public void testLinearizePolygon()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizePolygon");

            FgfGeometryFactory gf = new FgfGeometryFactory();
	        double[] ordsXYExt = new double[10];
	        ordsXYExt[0] = 0.0; ordsXYExt[1] = 0.0;
	        ordsXYExt[2] = 5.0; ordsXYExt[3] = 0.0;
	        ordsXYExt[4] = 5.0; ordsXYExt[5] = 5.0;
	        ordsXYExt[6] = 0.0; ordsXYExt[7] = 5.0;
	        ordsXYExt[8] = 0.0; ordsXYExt[9] = 0.0;

	        double[] ordsXYInt1 = new double[8];
	        ordsXYInt1[0] = 1.0; ordsXYInt1[1] = 1.0;
	        ordsXYInt1[2] = 2.0; ordsXYInt1[3] = 1.0;
	        ordsXYInt1[4] = 2.0; ordsXYInt1[5] = 2.0;
	        ordsXYInt1[6] = 1.0; ordsXYInt1[7] = 1.0;

	        double[] ordsXYInt2 = new double[8];
	        ordsXYInt2[0] = 3.0; ordsXYInt2[1] = 3.0;
	        ordsXYInt2[2] = 4.0; ordsXYInt2[3] = 3.0;
	        ordsXYInt2[4] = 4.0; ordsXYInt2[5] = 4.0;
	        ordsXYInt2[6] = 3.0; ordsXYInt2[7] = 3.0;

	        ILinearRing extRing = gf.CreateLinearRing(Common.Dimensionality_XY, 10, ordsXYExt);
	        ILinearRing intRing1 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt1);
	        ILinearRing intRing2 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt2);

            LinearRingCollection intRings = new LinearRingCollection();
	        intRings.Add(intRing1);
	        intRings.Add(intRing2);

	        IPolygon polygon = gf.CreatePolygon(extRing, intRings);
	        Common.DumpPolygon(polygon);
	        Common.CheckFGFT(polygon, "POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(polygon, 1.0, 0.001, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizePolygon");
            
        }

        public void testLinearizeMultiPoint()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiPoint");

	        double[] ordsXYZ1 = new double[3];
	        ordsXYZ1[0] = 1.0; ordsXYZ1[1] = 2.0; ordsXYZ1[2] = 3.0;

	        double[] ordsXYZ2 = new double[3];
	        ordsXYZ2[0] = 4.0; ordsXYZ2[1] = 5.0; ordsXYZ2[2] = 6.0;

	        double[] ordsXYZ3 = new double[3];
	        ordsXYZ3[0] = 7.0; ordsXYZ3[1] = 8.0; ordsXYZ3[2] = 9.0;

            FgfGeometryFactory gf = new FgfGeometryFactory();
	        IPoint pnt1 = gf.CreatePoint(Common.Dimensionality_XY|Common.Dimensionality_Z, ordsXYZ1);
	        IPoint pnt2 = gf.CreatePoint(Common.Dimensionality_XY|Common.Dimensionality_Z, ordsXYZ2);
	        IPoint pnt3 = gf.CreatePoint(Common.Dimensionality_XY|Common.Dimensionality_Z, ordsXYZ3);

            PointCollection pnts = new PointCollection();
	        pnts.Add(pnt1);
            pnts.Add(pnt2);
	        pnts.Add(pnt3);

	        IMultiPoint multiPnt = gf.CreateMultiPoint(pnts);
	        Common.DumpMultiPoint(multiPnt);
	        Common.CheckFGFT(multiPnt, "MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(multiPnt, 1.0, 0.01, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeMutiPoint");
            
        }

        public void testLinearizeMultiLineString()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiLineString");

	        double[] ordsXYZ1 = new double[9];
	        ordsXYZ1[0] = 0.0; ordsXYZ1[1] = 1.0; ordsXYZ1[2] = 2.0;
	        ordsXYZ1[3] = 3.0; ordsXYZ1[4] = 4.0; ordsXYZ1[5] = 5.0;
	        ordsXYZ1[6] = 6.0; ordsXYZ1[7] = 7.0; ordsXYZ1[8] = 8.0;

	        double[] ordsXYZ2 = new double[9];
	        ordsXYZ2[0] = 9.0; ordsXYZ2[1] = 10.0; ordsXYZ2[2] = 11.0;
	        ordsXYZ2[3] = 12.0; ordsXYZ2[4] = 13.0; ordsXYZ2[5] = 14.0;
	        ordsXYZ2[6] = 15.0; ordsXYZ2[7] = 16.0; ordsXYZ2[8] = 17.0;

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        ILineString lineString1 = gf.CreateLineString(Common.Dimensionality_XY|Common.Dimensionality_Z, 9, ordsXYZ1);
	        ILineString lineString2 = gf.CreateLineString(Common.Dimensionality_XY|Common.Dimensionality_Z, 9, ordsXYZ2);

            LineStringCollection lineStrings = new LineStringCollection();
	        lineStrings.Add(lineString1);
	        lineStrings.Add(lineString2);

	        IMultiLineString multiLine = gf.CreateMultiLineString(lineStrings);
	        Common.DumpMultiLineString(multiLine);
	        Common.CheckFGFT(multiLine, "MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");
 
            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(multiLine, 1.0, 0.01, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeLineString");
            
        }

        public void testLinearizeMultiPolygon()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiPolygon");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        // 1st polygon
	        double[] ordsXYExt1 = new double[10];
	        ordsXYExt1[0] = 0.0; ordsXYExt1[1] = 0.0;
	        ordsXYExt1[2] = 5.0; ordsXYExt1[3] = 0.0;
	        ordsXYExt1[4] = 5.0; ordsXYExt1[5] = 5.0;
	        ordsXYExt1[6] = 0.0; ordsXYExt1[7] = 5.0;
	        ordsXYExt1[8] = 0.0; ordsXYExt1[9] = 0.0;

	        double[] ordsXYInt11 = new double[8];
	        ordsXYInt11[0] = 1.0; ordsXYInt11[1] = 1.0;
	        ordsXYInt11[2] = 2.0; ordsXYInt11[3] = 1.0;
	        ordsXYInt11[4] = 2.0; ordsXYInt11[5] = 2.0;
	        ordsXYInt11[6] = 1.0; ordsXYInt11[7] = 1.0;

	        double[] ordsXYInt12 = new double[8];
	        ordsXYInt12[0] = 3.0; ordsXYInt12[1] = 3.0;
	        ordsXYInt12[2] = 4.0; ordsXYInt12[3] = 3.0;
	        ordsXYInt12[4] = 4.0; ordsXYInt12[5] = 4.0;
	        ordsXYInt12[6] = 3.0; ordsXYInt12[7] = 3.0;

	        ILinearRing extRing1 = gf.CreateLinearRing(Common.Dimensionality_XY, 10, ordsXYExt1);
	        ILinearRing intRing11 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt11);
	        ILinearRing intRing12 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt12);

            LinearRingCollection intRings1 = new LinearRingCollection();
	        intRings1.Add(intRing11);
	        intRings1.Add(intRing12);

	        IPolygon polygon1 = gf.CreatePolygon(extRing1, intRings1);

	        // 2nd polygon
	        double[] ordsXYExt2 = new double[10];
	        ordsXYExt2[0] = 0.0; ordsXYExt2[1] = 0.0;
	        ordsXYExt2[2] = 5.0; ordsXYExt2[3] = 0.0;
	        ordsXYExt2[4] = 5.0; ordsXYExt2[5] = 5.0;
	        ordsXYExt2[6] = 0.0; ordsXYExt2[7] = 5.0;
	        ordsXYExt2[8] = 0.0; ordsXYExt2[9] = 0.0;

	        double[] ordsXYInt21 = new double[8];
	        ordsXYInt21[0] = 1.0; ordsXYInt21[1] = 1.0;
	        ordsXYInt21[2] = 2.0; ordsXYInt21[3] = 1.0;
	        ordsXYInt21[4] = 2.0; ordsXYInt21[5] = 2.0;
	        ordsXYInt21[6] = 1.0; ordsXYInt21[7] = 1.0;

	        double[] ordsXYInt22 = new double[8];
	        ordsXYInt22[0] = 3.0; ordsXYInt22[1] = 3.0;
	        ordsXYInt22[2] = 4.0; ordsXYInt22[3] = 3.0;
	        ordsXYInt22[4] = 4.0; ordsXYInt22[5] = 4.0;
	        ordsXYInt22[6] = 3.0; ordsXYInt22[7] = 3.0;

	        ILinearRing extRing2 = gf.CreateLinearRing(Common.Dimensionality_XY, 10, ordsXYExt2);
	        ILinearRing intRing21 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt21);
	        ILinearRing intRing22 = gf.CreateLinearRing(Common.Dimensionality_XY, 8, ordsXYInt22);

            LinearRingCollection intRings2 = new LinearRingCollection();
	        intRings2.Add(intRing21);
	        intRings2.Add(intRing22);

	        IPolygon polygon2 = gf.CreatePolygon(extRing2, intRings2);

            PolygonCollection polygons = new PolygonCollection();
	        polygons.Add(polygon1);
	        polygons.Add(polygon2);

	        IMultiPolygon multiPoly = gf.CreateMultiPolygon(polygons);
	        Common.DumpMultiPolygon(multiPoly);
	        Common.CheckFGFT(multiPoly, "MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(multiPoly, 1.0, 0.01, gf);

            // Should be unchanged.
	        Common.CheckFGFT(linearizedGeometry, "MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeMutiPolygon");
            
        }

        public void testLinearizeMultiGeometry()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiGeometry");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        IMultiGeometry multiGeometry = Common.CreateMultiGeometry();
	        Common.DumpMultiGeometry(multiGeometry);
	        Common.CheckFGFT(multiGeometry, "GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

            IGeometry linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(multiGeometry, 1.0, 0.01, gf);

            // Console.WriteLine("Linearized 2D MultiGeometry:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);

	        Common.CheckFGFT(linearizedGeometry,
                "GEOMETRYCOLLECTION (POLYGON ((100 100, "
                + "99.9263476647491 100.346317444844, "
                + "99.9316048285876 100.700341174725, "
                + "100.015507875369 101.044318961554, "
                + "100.173849555978 101.361002322773, "
                + "100.398689957372 101.634511432295, "
                + "100.678754642975 101.851131401295, "
                + "101 102, "
                + "100 100), "
                + "(200 200, "
                + "199.926347664749 200.346317444844, "
                + "199.931604828588 200.700341174725, "
                + "200.015507875369 201.044318961554, "
                + "200.173849555978 201.361002322773, "
                + "200.398689957372 201.634511432295, "
                + "200.678754642975 201.851131401295, "
                + "201 202, "
                + "200 200), "
                + "(300 300, "
                + "299.926347664749 300.346317444844, "
                + "299.931604828588 300.700341174725, "
                + "300.015507875369 301.044318961554, "
                + "300.173849555978 301.361002322773, "
                + "300.398689957372 301.634511432295, "
                + "300.678754642975 301.851131401295, "
                + "301 302, "
                + "300 300)), "
                + "LINESTRING (100 100, "
                + "99.9263476647491 100.346317444844, "
                + "99.9316048285876 100.700341174725, "
                + "100.015507875369 101.044318961554, "
                + "100.173849555978 101.361002322773, "
                + "100.398689957372 101.634511432295, "
                + "100.678754642975 101.851131401295, "
                + "101 102, "
                + "103 100, "
                + "103 102), "
                + "LINESTRING (0 1, 2 3, 4 5), "
                + "POINT XYZ (5 3 2), "
                + "POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeMutiGeometry");
            
        }

        public void testLinearizeCurveString()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeCurveString");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        // Common.Create a curve string that consists of an arcsegment and a linear segment

	        // arcseg  = (0,0), (0,1), (1,2)
	        // lineseg = (1,2), (3,0), (3,2)

	        IDirectPosition startPos = gf.CreatePositionXY(0.0, 0.0);
	        IDirectPosition midPos = gf.CreatePositionXY(0.0, 1.0);
	        IDirectPosition endPos = gf.CreatePositionXY(1.0, 2.0);

	        ICircularArcSegment arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            DirectPositionCollection points = new DirectPositionCollection();
	        IDirectPosition pt1 = gf.CreatePositionXY(1.0, 2.0);
	        IDirectPosition pt2 = gf.CreatePositionXY(3.0, 0.0);
	        IDirectPosition pt3 = gf.CreatePositionXY(3.0, 2.0);
            points.Add(pt1);
            points.Add(pt2);
	        points.Add(pt3);

	        ILineStringSegment lineSeg = gf.CreateLineStringSegment(points);

            CurveSegmentCollection curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);
            curveSegs.Add(lineSeg);

	        ICurveString curveString = gf.CreateCurveString(curveSegs);
	        Common.DumpCurveString(curveString);            
	        Common.CheckFGFT(curveString, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");

            IGeometry linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(curveString, 1.0, 0.001, gf);

            //Console.WriteLine("Linearized 2D CurveString:");
            // Geometry should be very changed (all linear -- no arcs).
	        
            Common.DumpGeometry(linearizedGeometry);
            Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (0 0, "
                + "-0.030624360468144 0.103529235451737, "
                + "-0.05411224360262 0.208907000626204, "
                + "-0.070354138049801 0.315641975737789, "
                + "-0.079274316600211 0.423236513061636, "
                + "-0.080831189263724 0.531188957197069, "
                + "-0.0750174971813 0.638995984016635, "
                + "-0.061860346469139 0.74615494739554, "
                + "-0.04142108183745 0.852166222779906, "
                + "-0.013795000573097 0.95653553666708, "
                + "0.020889091780344 1.05877627113687, "
                + "0.062469481973487 1.15841173268892, "
                + "0.110752303032957 1.25497737480786, "
                + "0.165512438158711 1.3480229638936, "
                + "0.226494570322478 1.43711467845823, "
                + "0.293414372673606 1.52183713180202, "
                + "0.365959834202084 1.60179530873799, "
                + "0.443792714477911 1.67661640733506, "
                + "0.52655012068411 1.74595157709274, "
                + "0.613846199590564 1.80947754544319, "
                + "0.705273936579888 1.86689812499717, "
                + "0.800407053337464 1.91794559450637, "
                + "0.898801995357724 1.96238194710347, "
                + "1 2, 3 0, 3 2)");

            // Now try it again, in 3D.

	        startPos = gf.CreatePositionXYZ(0.0, 0.0, 0.0);
	        midPos = gf.CreatePositionXYZ(0.0, 1.0, 0.1);
	        endPos = gf.CreatePositionXYZ(1.0, 2.0, 0.0);

	        arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            points = new DirectPositionCollection();
	        pt1 = gf.CreatePositionXYZ(1.0, 2.0, 0.0);
	        pt2 = gf.CreatePositionXYZ(3.0, 0.0, 0.0);
	        pt3 = gf.CreatePositionXYZ(3.0, 2.0, 0.0);
            points.Add(pt1);
            points.Add(pt2);
	        points.Add(pt3);

	        lineSeg = gf.CreateLineStringSegment(points);

            curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);
            curveSegs.Add(lineSeg);

	        curveString = gf.CreateCurveString(curveSegs);
	        Common.DumpCurveString(curveString);
	        Common.CheckFGFT(curveString, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");

            linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(curveString, 1.0, 0.001, gf);

            //Console.WriteLine("Linearized 3D CurveString:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING XYZ (0 0 0, "
                + "-0.03085536765629 0.102689577511987 0.016440031282457, "
                + "-0.054535138833766 0.207452799842112 0.031652307750964, "
                + "-0.070924015276176 0.313779568444088 0.045562759899644, "
                + "-0.079942198540299 0.421152171769058 0.058103656884966, "
                + "-0.081545778539459 0.529047806034846 0.069213936311376, "
                + "-0.07572694734442 0.636941120789586 0.078839501547843, "
                + "-0.062514037200674 0.744306776875256 0.08693348512766, "
                + "-0.041971382577004 0.85062200433621 0.093456476949022, "
                + "-0.014199006917014 0.955369147818116 0.098376716165215, "
                + "0.020667864381145 1.05803818706358 0.101670245830129, "
                + "0.062459462385521 1.15812922023203 0.103321029546099, "
                + "0.110972301245689 1.25515489795237 0.103321029546099, "
                + "0.16597016897782 1.34864279625693 0.101670245830129, "
                + "0.227185277595298 1.43813771684274 0.098376716165215, "
                + "0.29431956698483 1.52320390345988 0.093456476949022, "
                + "0.367046156179391 1.60342716363539 0.08693348512766, "
                + "0.445010934961679 1.67841688540178 0.078839501547843, "
                + "0.527834288048447 1.74780793921066 0.069213936311376, "
                + "0.615112943460573 1.8112624557708 0.058103656884966, "
                + "0.706421936079024 1.86847147115449 0.045562759899644, "
                + "0.80131667682605 1.91915643116175 0.031652307750964, "
                + "0.899335117396636 1.96307054761784 0.016440031282457, "
                + "1 2 0, 3 0 0, 3 2 0)");

	        // Test 2D circle, centered at (2,0) and with radius 2.0.

	        startPos = gf.CreatePositionXY(0.0, 0.0);
	        midPos = gf.CreatePositionXY(4.0, 0.0);
	        endPos = gf.CreatePositionXY(0.0, 0.0);

	        arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);

	        curveString = gf.CreateCurveString(curveSegs);
	        Common.DumpCurveString(curveString);
	        Common.CheckFGFT(curveString, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (4 0, 0 0)))");

            linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(curveString, 1.0, 0.01, gf);

            //Console.WriteLine("Linearized 2D CurveString with circle:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (0 0, "
                + "0.038429439193539 -0.390180644032257, "
                + "0.152240934977426 -0.765366864730179, "
                + "0.337060775394909 -1.1111404660392, "
                + "0.585786437626905 -1.41421356237309, "
                + "0.888859533960796 -1.66293922460509, "
                + "1.23463313526982 -1.84775906502257, "
                + "1.60981935596774 -1.96157056080646, "
                + "2 -2, "
                + "2.39018064403226 -1.96157056080646, "
                + "2.76536686473018 -1.84775906502257, "
                + "3.1111404660392 -1.66293922460509, "
                + "3.41421356237309 -1.4142135623731, "
                + "3.66293922460509 -1.1111404660392, "
                + "3.84775906502257 -0.765366864730181, "
                + "3.96157056080646 -0.390180644032257, "
                + "4 0, "
                + "3.96157056080646 0.390180644032257, "
                + "3.84775906502257 0.76536686473018, "
                + "3.66293922460509 1.1111404660392, "
                + "3.4142135623731 1.41421356237309, "
                + "3.1111404660392 1.66293922460509, "
                + "2.76536686473018 1.84775906502257, "
                + "2.39018064403226 1.96157056080646, "
                + "2 2, "
                + "1.60981935596774 1.96157056080646, "
                + "1.23463313526982 1.84775906502257, "
                + "0.888859533960797 1.66293922460509, "
                + "0.585786437626907 1.4142135623731, "
                + "0.33706077539491 1.1111404660392, "
                + "0.152240934977427 0.765366864730181, "
                + "0.038429439193539 0.390180644032256, "
                + "0 0)");

	        // Test 3D circle (a tilted variation of the 2D circle test).

	        startPos = gf.CreatePositionXYZ(0.0, 0.0, 0.0);
	        midPos = gf.CreatePositionXYZ(4.0, 0.0, 0.1);
	        endPos = gf.CreatePositionXYZ(0.0, 0.0, 0.0);

	        arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);

	        curveString = gf.CreateCurveString(curveSegs);
	        Common.DumpCurveString(curveString);
	        Common.CheckFGFT(curveString, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (4 0 0.1, 0 0 0)))");

            linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(curveString, 1.0, 0.01, gf);

            //Console.WriteLine("Linearized 3D CurveString with circle:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING XYZ (0 0 0, "
                + "0.038429439193539 -0.390302556437679 0.000960735979838, "
                + "0.152240934977427 -0.765606004515653 0.003806023374436, "
                + "0.337060775394909 -1.11148764319688 0.008426519384873, "
                + "0.585786437626905 -1.41465543507951 0.014644660940673, "
                + "0.888859533960795 -1.66345881193994 0.02222148834902, "
                + "1.23463313526982 -1.84833639953597 0.030865828381746, "
                + "1.60981935596774 -1.96218345585682 0.040245483899194, "
                + "2 -2.00062490237426 0.05, "
                + "2.39018064403226 -1.96218345585682 0.059754516100806, "
                + "2.76536686473018 -1.84833639953597 0.069134171618254, "
                + "3.1111404660392 -1.66345881193994 0.07777851165098, "
                + "3.41421356237309 -1.41465543507951 0.085355339059327, "
                + "3.66293922460509 -1.11148764319688 0.091573480615127, "
                + "3.84775906502257 -0.765606004515653 0.096193976625564, "
                + "3.96157056080646 -0.390302556437679 0.099039264020161, "
                + "4 0 0.1, "
                + "3.96157056080646 0.390302556437679 0.099039264020161, "
                + "3.84775906502257 0.765606004515652 0.096193976625564, "
                + "3.66293922460509 1.11148764319688 0.091573480615127, "
                + "3.41421356237309 1.41465543507951 0.085355339059327, "
                + "3.1111404660392 1.66345881193994 0.07777851165098, "
                + "2.76536686473018 1.84833639953597 0.069134171618254, "
                + "2.39018064403226 1.96218345585682 0.059754516100806, "
                + "2 2.00062490237426 0.05, "
                + "1.60981935596774 1.96218345585682 0.040245483899194, "
                + "1.23463313526982 1.84833639953597 0.030865828381745, "
                + "0.888859533960796 1.66345881193994 0.02222148834902, "
                + "0.585786437626905 1.41465543507951 0.014644660940673, "
                + "0.337060775394909 1.11148764319688 0.008426519384873, "
                + "0.152240934977427 0.765606004515654 0.003806023374436, "
                + "0.038429439193539 0.39030255643768 0.000960735979838, "
                + "0 0 0)");

            // Test 2D curve with collinear arc positions.

            IGeometry cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 1, 2 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 1, 2 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with arc having collinear positions:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 1 1, 2 2, 3 0, 3 2)");

            // Test 3D curve with collinear arc positions.

            cs = gf.CreateGeometry("CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (1 1 1, 2 2 2), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (1 1 1, 2 2 2), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
            //Console.WriteLine("Linearized 3D CurveString with arc having collinear positions:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING XYZ (0 0 0, 1 1 1, 2 2 2, 3 0 2, 3 2 1)");

            // Test 2D curve with collinear arc positions forming a spike.

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (2 2, 1 1), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (2 2, 1 1), LINESTRINGSEGMENT (3 0, 3 2)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with arc having collinear positions:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 2 2, 1 1, 3 0, 3 2)");

            // Test 3D curve with collinear arc positions forming a spike.

            cs = gf.CreateGeometry("CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (2 2 2, 1 1 1), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (2 2 2, 1 1 1), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
            //Console.WriteLine("Linearized 3D CurveString with arc having collinear positions:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING XYZ (0 0 0, 2 2 2, 1 1 1, 3 0 2, 3 2 1)");

            // Test 2D curve with arc positions that are too close to interpolate (just use large tolerances).

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 2.0, 2.0, gf);
            //Console.WriteLine("Linearized 2D CurveString with arc having positions too close to interpolate:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 0 1, 1 2, 3 0, 3 2)");

            // Test 3D curve with arc positions that are too close to interpolate (just use large tolerances).

            cs = gf.CreateGeometry("CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 2.0, 2.0, gf);
            //Console.WriteLine("Linearized 3D CurveString with arc having positions too close to interpolate:");
            // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING XYZ (0 0 0, 0 1 0.1, 1 2 0, 3 0 0, 3 2 0)");

            // Test zero-valued parameters.

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.001, gf);
            //Console.WriteLine("Linearized 2D CurveString with zero maxSpacing:");
            // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (0 0, "
                + "-0.030624360468144 0.103529235451737, "
                + "-0.05411224360262 0.208907000626204, "
                + "-0.070354138049801 0.315641975737789, "
                + "-0.079274316600211 0.423236513061636, "
                + "-0.080831189263724 0.531188957197069, "
                + "-0.0750174971813 0.638995984016635, "
                + "-0.061860346469139 0.74615494739554, "
                + "-0.04142108183745 0.852166222779906, "
                + "-0.013795000573097 0.95653553666708, "
                + "0.020889091780344 1.05877627113687, "
                + "0.062469481973487 1.15841173268892, "
                + "0.110752303032957 1.25497737480786, "
                + "0.165512438158711 1.3480229638936, "
                + "0.226494570322478 1.43711467845823, "
                + "0.293414372673606 1.52183713180202, "
                + "0.365959834202084 1.60179530873799, "
                + "0.443792714477911 1.67661640733506, "
                + "0.52655012068411 1.74595157709274, "
                + "0.613846199590564 1.80947754544319, "
                + "0.705273936579888 1.86689812499717, "
                + "0.800407053337464 1.91794559450637, "
                + "0.898801995357724 1.96238194710347, "
                + "1 2, 3 0, 3 2)");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.2, 0.0, gf);
            //Console.WriteLine("Linearized 2D CurveString with zero maxOffset:");
            // Geometry should be very changed (all linear -- no arcs -- similar to 2D case above).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (0 0, "
                + "-0.049331651274743 0.184450583333957, "
                + "-0.076070558282857 0.37350258771831, "
                + "-0.07982680754939 0.564399209221096, "
                + "-0.060545624501699 0.754356745239048, "
                + "-0.018508172206063 0.940605187149905, "
                + "0.045672548622951 1.12042861327564, "
                + "0.131060638553256 1.29120479313625, "
                + "0.236410946956438 1.45044342547491, "
                + "0.360187229107742 1.59582245245793, "
                + "0.500584548093489 1.72522192050685, "
                + "0.655555594855637 1.8367548939953, "
                + "0.822840542567988 1.92879497101942, "
                + "1 2, 3 0, 3 2)");

            // Test non-zero maxOffset that is superceded by maxSpacing. (opposite precedence is covered in first case in this test).
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.2, 1.0, gf);
            //Console.WriteLine("Linearized 2D CurveString with zero maxOffset:");
            // Geometry should be very changed (all linear -- no arcs -- as 2D case above with zero maxOffset).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (0 0, "
                + "-0.049331651274743 0.184450583333957, "
                + "-0.076070558282857 0.37350258771831, "
                + "-0.07982680754939 0.564399209221096, "
                + "-0.060545624501699 0.754356745239048, "
                + "-0.018508172206063 0.940605187149905, "
                + "0.045672548622951 1.12042861327564, "
                + "0.131060638553256 1.29120479313625, "
                + "0.236410946956438 1.45044342547491, "
                + "0.360187229107742 1.59582245245793, "
                + "0.500584548093489 1.72522192050685, "
                + "0.655555594855637 1.8367548939953, "
                + "0.822840542567988 1.92879497101942, "
                + "1 2, 3 0, 3 2)");

            // Test interpolation of M ordinate.

            cs = gf.CreateGeometry("CURVESTRING XYM (0 0 5 (CIRCULARARCSEGMENT (0 1 7, 1 2 10), LINESTRINGSEGMENT (3 0 12, 3 2 15)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYM (0 0 5 (CIRCULARARCSEGMENT (0 1 7, 1 2 10), LINESTRINGSEGMENT (3 0 12, 3 2 15)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            // Console.WriteLine("Linearized XYM CurveString:");
            // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING XYM (0 0 5, "
                + "-0.073652335250893 0.34631744484356 5.71428571428571, "
                + "-0.068395171412408 0.700341174725127 6.42857142857143, "
                + "0.015507875368588 1.04431896155409 7.14285714285714, "
                + "0.173849555977885 1.36100232277268 7.85714285714286, "
                + "0.398689957372453 1.63451143229485 8.57142857142857, "
                + "0.678754642974616 1.85113140129458 9.28571428571428, "
                + "1 2 10, 3 0 12, 3 2 15)");
            cs = gf.CreateGeometry("CURVESTRING XYZM (0 0 0 5 (CIRCULARARCSEGMENT (0 1 0.1 7, 1 2 0 10), LINESTRINGSEGMENT (3 0 0 12, 3 2 -0.5 15)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYZM (0 0 0 5 (CIRCULARARCSEGMENT (0 1 0.1 7, 1 2 0 10), LINESTRINGSEGMENT (3 0 0 12, 3 2 -0.5 15)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            //Console.WriteLine("Linearized XYZM CurveString:");
            // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING XYZM (0 0 0 5, "
                + "-0.069277197762034 0.300420851841192 0.043897524736526 5.625, "
                + "-0.077876573725876 0.609996763299854 0.076574991075161 6.25, "
                + "-0.025453058417013 0.916305298429713 0.096721141526374 6.875, "
                + "0.085889735651764 1.20705513217412 0.103527566087059 7.5, "
                + "0.251683926206021 1.47057926767578 0.096721141526374 8.125, "
                + "0.465276645124591 1.69630320100079 0.076574991075161 8.75, "
                + "0.718096999869825 1.87516924710491 0.043897524736526 9.375, "
                + "1 2 0 10, 3 0 0 12, 3 2 -0.5 15)");

            // Flat circles (in XY plane) are treated distinctly, not just from general 3D case, but
            // w.r.t. orientation.  Make sure that the counterclockwise case works.

            cs = gf.CreateGeometry("CURVESTRING (1 2 (CIRCULARARCSEGMENT (0 1, 0 0)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (1 2 (CIRCULARARCSEGMENT (0 1, 0 0)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            // Console.WriteLine("Linearized 2D CurveString with counterclockwise orientation:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);
	        Common.CheckFGFT(linearizedGeometry,
                "LINESTRING (1 2, "
                + "0.678754642974617 1.85113140129458, "
                + "0.398689957372453 1.63451143229485, "
                + "0.173849555977885 1.36100232277268, "
                + "0.015507875368588 1.04431896155409, "
                + "-0.068395171412408 0.700341174725128, "
                + "-0.073652335250893 0.34631744484356, "
                + "0 0)");

            // Test degenerate conditions.  They should result in just using known 
            // positions, much like collinear cases.

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 0 0), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 0 0), LINESTRINGSEGMENT (3 0, 3 2)))");
            //Console.WriteLine("CurveString with arc defined by three identical positions:");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with counterclockwise orientation:");
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 3 0, 3 2)");

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            //Console.WriteLine("CurveString with arc having identical start and mid positions:");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with counterclockwise orientation:");
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 1 2, 3 0, 3 2)");

            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 2, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 2, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            //Console.WriteLine("CurveString with arc having identical mid and end positions:");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with counterclockwise orientation:");
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING (0 0, 1 2, 3 0, 3 2)");

            cs = gf.CreateGeometry("CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 0 1, 0 0 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 0 1, 0 0 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
            //Console.WriteLine("CurveString with arc having identical startref end positions, and mid pos differing only in Z (degenerate 'vertical' circle):");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
            //Console.WriteLine("Linearized 2D CurveString with counterclockwise orientation:");
	        Common.CheckFGFT(linearizedGeometry, "LINESTRING XYZ (0 0 0, 0 0 1, 0 0 0, 3 0 0, 3 2 0)");

            // Test fail-safe limit of 5000 positions per arc (use a tiny tolerance).
            cs = gf.CreateGeometry("CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	        Common.DumpGeometry(cs);
	        Common.CheckFGFT(cs, "CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
            linearizedGeometry = SpatialUtility.ApproximateGeometryWithLineStrings(cs, 0.0, 0.00000001, gf);
            //Console.WriteLine("Linearized 2D CurveString with tiny maxOffset -- expect fail-safe 5000 positions for arc:");
            // Geometry should be a LineString with 5002 positions (5000 for arc, 2 
            // more for original linestringsegment).
            Debug.Assert(linearizedGeometry.DerivedType == GeometryType.GeometryType_LineString, "Wrong type for linearized geometry.");
            ILineString ls = (ILineString) linearizedGeometry;
            int count = ls.Count;
            Debug.Assert(count == 5002, "Wrong # of positions for linearized geometry.");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeCurveString");
            
        }

        public void testLinearizeCurvePolygon()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeCurvePolygon");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        ICurvePolygon curvePoly = Common.CreateCurvePolygon(100);
	        Common.DumpCurvePolygon(curvePoly);
	        Common.CheckFGFT(curvePoly, "CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");

            IGeometry linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(curvePoly, 1.0, 0.001, gf);

            //Console.WriteLine("Linearized 2D CurvePolygon:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);

	        Common.CheckFGFT(linearizedGeometry,
                "POLYGON ((200 200, "
                + "199.969375639532 200.103529235452, "
                + "199.945887756397 200.208907000626, "
                + "199.92964586195 200.315641975738, "
                + "199.9207256834 200.423236513062, "
                + "199.919168810736 200.531188957197, "
                + "199.924982502819 200.638995984017, "
                + "199.938139653531 200.746154947396, "
                + "199.958578918163 200.85216622278, "
                + "199.986204999427 200.956535536667, "
                + "200.02088909178 201.058776271137, "
                + "200.062469481973 201.158411732689, "
                + "200.110752303033 201.254977374808, "
                + "200.165512438159 201.348022963894, "
                + "200.226494570322 201.437114678458, "
                + "200.293414372674 201.521837131802, "
                + "200.365959834202 201.601795308738, "
                + "200.443792714478 201.676616407335, "
                + "200.526550120684 201.745951577093, "
                + "200.613846199591 201.809477545443, "
                + "200.70527393658 201.866898124997, "
                + "200.800407053337 201.917945594506, "
                + "200.898801995358 201.962381947103, "
                + "201 202, 200 200), "
                + "(300 300, "
                + "299.969375639532 300.103529235452, "
                + "299.945887756397 300.208907000626, "
                + "299.92964586195 300.315641975738, "
                + "299.9207256834 300.423236513062, "
                + "299.919168810736 300.531188957197, "
                + "299.924982502819 300.638995984017, "
                + "299.938139653531 300.746154947396, "
                + "299.958578918163 300.85216622278, "
                + "299.986204999427 300.956535536667, "
                + "300.02088909178 301.058776271137, "
                + "300.062469481973 301.158411732689, "
                + "300.110752303033 301.254977374808, "
                + "300.165512438159 301.348022963894, "
                + "300.226494570322 301.437114678458, "
                + "300.293414372674 301.521837131802, "
                + "300.365959834202 301.601795308738, "
                + "300.443792714478 301.676616407335, "
                + "300.526550120684 301.745951577093, "
                + "300.613846199591 301.809477545443, "
                + "300.70527393658 301.866898124997, "
                + "300.800407053337 301.917945594506, "
                + "300.898801995358 301.962381947103, "
                + "301 302, "
                + "300 300), "
                + "(400 400, "
                + "399.969375639532 400.103529235452, "
                + "399.945887756397 400.208907000626, "
                + "399.92964586195 400.315641975738, "
                + "399.9207256834 400.423236513062, "
                + "399.919168810736 400.531188957197, "
                + "399.924982502819 400.638995984017, "
                + "399.938139653531 400.746154947396, "
                + "399.958578918163 400.85216622278, "
                + "399.986204999427 400.956535536667, "
                + "400.02088909178 401.058776271137, "
                + "400.062469481973 401.158411732689, "
                + "400.110752303033 401.254977374808, "
                + "400.165512438159 401.348022963894, "
                + "400.226494570322 401.437114678458, "
                + "400.293414372674 401.521837131802, "
                + "400.365959834202 401.601795308738, "
                + "400.443792714478 401.676616407335, "
                + "400.526550120684 401.745951577093, "
                + "400.613846199591 401.809477545443, "
                + "400.70527393658 401.866898124997, "
                + "400.800407053337 401.917945594506, "
                + "400.898801995358 401.962381947103, "
                + "401 402, 400 400))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeCurvePolygon");
            
        }

        public void testLinearizeMultiCurveString()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiCurveString");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        ICurveString curveString1 = Common.CreateCurveString(100);
	        ICurveString curveString2 = Common.CreateCurveString(200);
	        ICurveString curveString3 = Common.CreateCurveString(300);

            CurveStringCollection curveStrings = new CurveStringCollection();
	        curveStrings.Add(curveString1);
	        curveStrings.Add(curveString2);
	        curveStrings.Add(curveString3);

	        IMultiCurveString multiCurveString = gf.CreateMultiCurveString(curveStrings);
	        Common.DumpMultiCurveString(multiCurveString);
	        Common.CheckFGFT(multiCurveString, "MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200, 203 202))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300, 303 302))))");

            IGeometry linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(multiCurveString, 1.0, 0.01, gf);

            //Console.WriteLine("Linearized 2D MultiCurveString:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);

	        Common.CheckFGFT(linearizedGeometry,
                "MULTILINESTRING ((100 100, "
                + "99.9263476647491 100.346317444844, "
                + "99.9316048285876 100.700341174725, "
                + "100.015507875369 101.044318961554, "
                + "100.173849555978 101.361002322773, "
                + "100.398689957372 101.634511432295, "
                + "100.678754642975 101.851131401295, "
                + "101 102, "
                + "103 100, "
                + "103 102), "
                + "(200 200, "
                + "199.926347664749 200.346317444844, "
                + "199.931604828588 200.700341174725, "
                + "200.015507875369 201.044318961554, "
                + "200.173849555978 201.361002322773, "
                + "200.398689957372 201.634511432295, "
                + "200.678754642975 201.851131401295, "
                + "201 202, "
                + "203 200, "
                + "203 202), "
                + "(300 300, "
                + "299.926347664749 300.346317444844, "
                + "299.931604828588 300.700341174725, "
                + "300.015507875369 301.044318961554, "
                + "300.173849555978 301.361002322773, "
                + "300.398689957372 301.634511432295, "
                + "300.678754642975 301.851131401295, "
                + "301 302, "
                + "303 300, "
                + "303 302))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeMutiCurveString");
            
        }

        public void testLinearizeMultiCurvePolygon()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testLinearizeMultiCurvePolygon");

            FgfGeometryFactory gf = new FgfGeometryFactory();

	        IMultiCurvePolygon multiCurvePoly = Common.CreateMultiCurvePolygon(3, 100);
	        Common.DumpMultiCurvePolygon(multiCurvePoly);
	        Common.CheckFGFT(multiCurvePoly, "MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");

            IGeometry linearizedGeometry =
                SpatialUtility.ApproximateGeometryWithLineStrings(multiCurvePoly, 1.0, 0.01, gf);

            //Console.WriteLine("Linearized 2D MultiCurvePolygon:");
            // Geometry should be very changed (all linear -- no arcs).
	        Common.DumpGeometry(linearizedGeometry);

	        Common.CheckFGFT(linearizedGeometry,
                "MULTIPOLYGON (((200 200, "
                + "199.926347664749 200.346317444844, "
                + "199.931604828588 200.700341174725, "
                + "200.015507875369 201.044318961554, "
                + "200.173849555978 201.361002322773, "
                + "200.398689957372 201.634511432295, "
                + "200.678754642975 201.851131401295, "
                + "201 202, "
                + "200 200), "
                + "(300 300, "
                + "299.926347664749 300.346317444844, "
                + "299.931604828588 300.700341174725, "
                + "300.015507875369 301.044318961554, "
                + "300.173849555978 301.361002322773, "
                + "300.398689957372 301.634511432295, "
                + "300.678754642975 301.851131401295, "
                + "301 302, "
                + "300 300), "
                + "(400 400, "
                + "399.926347664749 400.346317444844, "
                + "399.931604828588 400.700341174725, "
                + "400.015507875369 401.044318961554, "
                + "400.173849555978 401.361002322773, "
                + "400.398689957372 401.634511432295, "
                + "400.678754642975 401.851131401295, "
                + "401 402, "
                + "400 400)), "
                + "((201 201, "
                + "200.926347664749 201.346317444844, "
                + "200.931604828588 201.700341174725, "
                + "201.015507875369 202.044318961554, "
                + "201.173849555978 202.361002322773, "
                + "201.398689957372 202.634511432295, "
                + "201.678754642975 202.851131401295, "
                + "202 203, "
                + "201 201), "
                + "(301 301, "
                + "300.926347664749 301.346317444844, "
                + "300.931604828588 301.700341174725, "
                + "301.015507875369 302.044318961554, "
                + "301.173849555978 302.361002322773, "
                + "301.398689957372 302.634511432295, "
                + "301.678754642975 302.851131401295, "
                + "302 303, "
                + "301 301), "
                + "(401 401, "
                + "400.926347664749 401.346317444844, "
                + "400.931604828588 401.700341174725, "
                + "401.015507875369 402.044318961554, "
                + "401.173849555978 402.361002322773, "
                + "401.398689957372 402.634511432295, "
                + "401.678754642975 402.851131401295, "
                + "402 403, "
                + "401 401)), "
                + "((202 202, "
                + "201.926347664749 202.346317444844, "
                + "201.931604828588 202.700341174725, "
                + "202.015507875369 203.044318961554, "
                + "202.173849555978 203.361002322773, "
                + "202.398689957372 203.634511432295, "
                + "202.678754642975 203.851131401295, "
                + "203 204, "
                + "202 202), "
                + "(302 302, "
                + "301.926347664749 302.346317444844, "
                + "301.931604828588 302.700341174725, "
                + "302.015507875369 303.044318961554, "
                + "302.173849555978 303.361002322773, "
                + "302.398689957372 303.634511432295, "
                + "302.678754642975 303.851131401295, "
                + "303 304, "
                + "302 302), "
                + "(402 402, "
                + "401.926347664749 402.346317444844, "
                + "401.931604828588 402.700341174725, "
                + "402.015507875369 403.044318961554, "
                + "402.173849555978 403.361002322773, "
                + "402.398689957372 403.634511432295, "
                + "402.678754642975 403.851131401295, "
                + "403 404, "
                + "402 402)))");

            //Console.WriteLine("finish mgSpatialUtilityTest.testLinearizeMutiCurvePolygon");
            
        }

        public void testGridTransform()
        {
            Console.WriteLine("start mgSpatialUtilityTest.testGridTransform");

            FgfGeometryFactory gf = new FgfGeometryFactory();

            IEnvelope extents = gf.CreateEnvelopeXY(0, 0, 1000, 1000);

            // With a tolerance of 0.5, the grid has a resolution of 1.0, effectively
            // making round-trips through it a rounding exercise.
            SpatialGridTransform gt = new SpatialGridTransform(extents, 0.5);

            long ix = -1;
            long iy = -1;
            gt.TransformToGrid(10, 10, ref ix, ref iy);
            double x = -12345.0;
            double y = -67890.0;
            gt.TransformFromGrid(ix, iy, ref x, ref y);
	        Debug.Assert(Math.Abs(10-x) < 0.00000001, "X value mismatch");
	        Debug.Assert(Math.Abs(10-y) < 0.00000001, "Y value mismatch");

            gt.TransformToGrid(10.1, 10.1, ref ix, ref iy);
            gt.TransformFromGrid(ix, iy, ref x, ref y);
            Debug.Assert(Math.Abs(10-x) < 0.00000001, "X value mismatch");
            Debug.Assert(Math.Abs(10-y) < 0.00000001, "Y value mismatch");

            gt.TransformToGrid(9.9, 9.9, ref ix, ref iy);
            gt.TransformFromGrid(ix, iy, ref x, ref y);
            Debug.Assert(Math.Abs(10-x) < 0.00000001, "X value mismatch");
            Debug.Assert(Math.Abs(10-y) < 0.00000001, "Y value mismatch");

            // Note that the extents are just a parameter for determining scale.
            // The grid transform tool doesn't really check for exceeding the extents.
            gt.TransformToGrid(-10.1, -10.1, ref ix, ref iy);
            gt.TransformFromGrid(ix, iy, ref x, ref y);
            Debug.Assert(Math.Abs(-10-x) < 0.00000001, "X value mismatch");
            Debug.Assert(Math.Abs(-10-y) < 0.00000001, "Y value mismatch");

            gt.TransformToGrid(-9.9, -9.9, ref ix, ref iy);
            gt.TransformFromGrid(ix, iy, ref x, ref y);
            Debug.Assert(Math.Abs(-10-x) < 0.00000001, "X value mismatch");
            Debug.Assert(Math.Abs(-10-y) < 0.00000001, "Y value mismatch");

            //Console.WriteLine("finish mgSpatialUtilityTest.testGridTransform");
            
        }

        private string GeometryValidityToString(SpatialGeometryValidity validity)
        {
            string stringVal = null;
            switch(validity)
            {
                case SpatialGeometryValidity.SpatialGeometryValidity_None:
                    stringVal = "None";
                    break;
                case SpatialGeometryValidity.SpatialGeometryValidity_Valid:
                    stringVal = "Valid";
                    break;
                case SpatialGeometryValidity.SpatialGeometryValidity_Invalid:
                    stringVal = "Invalid";
                    break;
                case SpatialGeometryValidity.SpatialGeometryValidity_InvalidButCanBeApproximated:
                    stringVal = "InvalidButCanBeApproximated";
                    break;
                case SpatialGeometryValidity.SpatialGeometryValidity_InvalidDueToDimensionality:
                    stringVal = "InvalidDueToDimensionality";
                    break;
            }
            return stringVal;
        }
	}
}

