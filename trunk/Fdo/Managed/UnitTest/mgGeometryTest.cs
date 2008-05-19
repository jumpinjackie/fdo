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

using OSGeo.FDO.Geometry;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgGeometryTest.
	/// </summary>
	public class mgGeometryTest
	{   
		public mgGeometryTest()
		{
            Console.WriteLine("start mgGeometryTest");
		}

        public void testDirectPosition()
        {
            Console.WriteLine("start mgGeometryTest.testDirectPosition");

            FgfGeometryFactory gf = new FgfGeometryFactory();

            IDirectPosition pointXY = gf.CreatePositionXY(5.0, 6.0);
            Common.DumpPosition("", pointXY);
            Common.CheckPositionXY(pointXY, 5.0, 6.0);

	        IDirectPosition pointXYM = gf.CreatePositionXYM(1.0, 3.0, 5.0);
            Common.DumpPosition("", pointXYM);
	        Common.CheckPositionXYM(pointXYM, 1.0, 3.0, 5.0);

	        IDirectPosition pointXYZ = gf.CreatePositionXYZ(2.0, 7.0, 10.0);
            Common.DumpPosition("", pointXYZ);
	        Common.CheckPositionXYZ(pointXYZ, 2.0, 7.0, 10.0);

	        IDirectPosition pointXYZM = gf.CreatePositionXYZM(1.23, 4.26, 3.67, 3.14);
            Common.DumpPosition("", pointXYZM);
	        Common.CheckPositionXYZM(pointXYZM, 1.23, 4.26, 3.67, 3.14);

            //Console.WriteLine("finish mgGeometryTest.testDirectPosition");
            
        }

        public void testEnvelope()
        {
            Console.WriteLine("start mgGeometryTest.testDirectPosition");

            FgfGeometryFactory gf = new FgfGeometryFactory();

            double[] ordsXY = new double[4];
            ordsXY[0] = -1.0;
            ordsXY[1] = -2.0;
            ordsXY[2] = 3.0;
            ordsXY[3] = 4.0;

            IEnvelope envl1 = gf.CreateEnvelope(Common.Dimensionality_XY, ordsXY);
            Common.DumpEnvelope(envl1);
            Common.CheckEnvelope(envl1, ordsXY);

            IDirectPosition pos1 = gf.CreatePositionXY(5.0, 6.0);
            IDirectPosition pos2 = gf.CreatePositionXY(10.0, 13.0);
            IEnvelope envl2 = gf.CreateEnvelope(pos1, pos2);
            Common.DumpEnvelope(envl2);
            Common.CheckEnvelope(envl2, pos1, pos2);

            IEnvelope envl3 = gf.CreateEnvelopeXY(4.0, 3.0, 12.0, 120.0);
            Common.DumpEnvelope(envl3);
            Common.CheckEnvelopeXY(envl3, 4.0, 3.0, 12.0, 120.0);

            IEnvelope envl4 = gf.CreateEnvelopeXYZ(12.0, 45.0, 1.0, 34.0, 39.0, 2.0);
            Common.DumpEnvelope(envl4);
            Common.CheckEnvelopeXYZ(envl4, 12.0, 45.0, 1.0, 34.0, 39.0, 2.0);

            IEnvelope envl5 = gf.CreateEnvelope(envl4);
            Common.DumpEnvelope(envl5);
            Common.CheckEqualEnvelopes(envl4, envl5);

            //Console.WriteLine("finish mgGeometryTest.testDirectPosition");
            
        }

        public void testLinearRing()
        {
            Console.WriteLine("start mgGeometryTest.testLinearRing");

            FgfGeometryFactory gf = new FgfGeometryFactory();
            double[] ordsXY = new double[10];
            ordsXY[0] = 0.0; ordsXY[1] = 0.0;
            ordsXY[2] = 1.0; ordsXY[3] = 0.0;
            ordsXY[4] = 1.0; ordsXY[5] = 1.0;
            ordsXY[6] = 0.0; ordsXY[7] = 1.0;
            ordsXY[8] = 0.0; ordsXY[9] = 0.0;

            ILinearRing lRing = gf.CreateLinearRing(Common.Dimensionality_XY, 10, ordsXY);
            Common.DumpLinearRing(lRing);

            //Console.WriteLine("finish mgGeometryTest.testLinearRing");
            
        }

        public void testLineStringSegment()
        {
            Console.WriteLine("start mgGeometryTest.testLineStringSegment");

            FgfGeometryFactory gf = new FgfGeometryFactory();
            double[] ordsXY = new double[10];
            ordsXY[0] = 0.0; ordsXY[1] = 0.0;
            ordsXY[2] = 1.0; ordsXY[3] = 0.0;
            ordsXY[4] = 1.0; ordsXY[5] = 1.0;
            ordsXY[6] = 0.0; ordsXY[7] = 1.0;
            ordsXY[8] = 0.0; ordsXY[9] = 0.0;

            ILineStringSegment lSeg = gf.CreateLineStringSegment(Common.Dimensionality_XY, 10, ordsXY);
            Common.DumpLineStringSegment(lSeg);

            //Console.WriteLine("finish mgGeometryTest.testLineStringSegment");
            
        }

        public void testCircularArcSegment()
        {
            Console.WriteLine("start mgGeometryTest.testCircularArcSegment");

            FgfGeometryFactory gf = new FgfGeometryFactory();

            IDirectPosition startPos = gf.CreatePositionXYZ(0.0, 0.0, 0.0);
            IDirectPosition midPos = gf.CreatePositionXYZ(1.0, 1.0, 1.0);
            IDirectPosition endPos = gf.CreatePositionXYZ(2.0, 2.0, 2.0);

            ICircularArcSegment arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);
            Common.DumpCircularArcSegment(arcSeg);

            //Console.WriteLine("finish mgGeometryTest.testCircularArcSegment");
            
        }

        public void testRing()
        {
            Console.WriteLine("start mgGeometryTest.testRing");

            IRing ring = Common.CreateRing(3.14);
            Common.DumpRing(ring);

            //Console.WriteLine("finish mgGeometryTest.testRing");
            
        }

        public void testPoint()
        {
            Console.WriteLine("start mgGeometryTest.testPoint");

            double[] ordsXYZ = new double[3];
            ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;

            FgfGeometryFactory gf = new FgfGeometryFactory();
            IPoint pnt = gf.CreatePoint(Common.Dimensionality_XY|Common.Dimensionality_Z, ordsXYZ);
            Common.DumpPoint(pnt);
            Common.CheckFGFT(pnt, "POINT XYZ (5 3 2)");

            //Console.WriteLine("finish mgGeometryTest.testPoint");
            
        }

        public void testLineString()
        {
            Console.WriteLine("start mgGeometryTest.testLineString");

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
            byte[] wkb2 = gf.GetWkb(line1);
//            Common.CheckWkb(wkb2.GetData());

            //Console.WriteLine("finish mgGeometryTest.testLineString");
            
        }

        public void testPolygon()
        {
            Console.WriteLine("start mgGeometryTest.testPolygon");

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

            byte[] wkb = gf.GetWkb(polygon);
            IGeometry g2 = gf.CreateGeometryFromWkb(wkb);
            Common.CheckFGFT(g2, "POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
            byte[] wkb2 = gf.GetWkb(g2);
            //    Common.CheckWkb(wkb2.GetData());

            //Console.WriteLine("finish mgGeometryTest.testPolygon");
            
        }

        public void testMultiPoint()
        {
            Console.WriteLine("start mgGeometryTest.testMultiPoint");

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

            //Console.WriteLine("finish mgGeometryTest.testMultiPoint");
            
        }

        public void testMultiLineString()
        {
            Console.WriteLine("start mgGeometryTest.testMultiLineString");

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

            //Console.WriteLine("finish mgGeometryTest.testMultiLineString");
            
        }

        public void testMultiPolygon()
        {
            Console.WriteLine("start mgGeometryTest.testMultiPolygon");

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

            byte[] wkb = gf.GetWkb(multiPoly);
            IGeometry g2 = gf.CreateGeometryFromWkb(wkb);
            Common.CheckFGFT(g2, "MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
            byte[] wkb2 = gf.GetWkb(g2);
            //    Common.CheckWkb(wkb2.GetData());

            //Console.WriteLine("finish mgGeometryTest.testMultiPolygon");
            
        }

        public void testMultiGeometry()
        {
            Console.WriteLine("start mgGeometryTest.testMultiGeometry");

            IMultiGeometry multiGeometry = Common.CreateMultiGeometry();
            Common.DumpMultiGeometry(multiGeometry);
            Common.CheckFGFT(multiGeometry, "GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

            //Console.WriteLine("finish mgGeometryTest.testMultiGeometry");
            
        }

        public void testCurveString()
        {
            Console.WriteLine("start mgGeometryTest.testCurveString");

            FgfGeometryFactory gf = new FgfGeometryFactory();

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

            //Console.WriteLine("finish mgGeometryTest.testCurveString");
            
        }

        public void testCurvePolygon()
        {
            Console.WriteLine("start mgGeometryTest.testCurvePolygon");

            ICurvePolygon curvePoly = Common.CreateCurvePolygon(100);
            Common.DumpCurvePolygon(curvePoly);
            Common.CheckFGFT(curvePoly, "CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");

            //Console.WriteLine("finish mgGeometryTest.testCurvePolygon");
            
        }

        public void testMultiCurveString()
        {
            Console.WriteLine("start mgGeometryTest.testMultiCurveString");

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

            //Console.WriteLine("finish mgGeometryTest.testMultiCurveString");
            
        }

        public void testMultiCurvePolygon()
        {
            Console.WriteLine("start mgGeometryTest.testMultiCurvePolygon");

            IMultiCurvePolygon multiCurvePoly = Common.CreateMultiCurvePolygon(3, 100);
            Common.DumpMultiCurvePolygon(multiCurvePoly);
            Common.CheckFGFT(multiCurvePoly, "MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");

            //Console.WriteLine("finish mgGeometryTest.testMultiCurvePolygon");
            
        }

        public void testGeometryFromEnvelope()
        {
            Console.WriteLine("start mgGeometryTest.testGeometryFromEnvelope");

            FgfGeometryFactory gf = new FgfGeometryFactory();

            IEnvelope envelope = gf.CreateEnvelopeXY(1.2345678, 1.2, 2, 2);
            IGeometry geometry = gf.CreateGeometry(envelope);

            Common.DumpGeometry(geometry);
            Common.CheckFGFT(geometry, "POLYGON ((1.2345678 1.2, 2 1.2, 2 2, 1.2345678 2, 1.2345678 1.2))");

            envelope = gf.CreateEnvelopeXYZ(1, 1, 0, 2, 2, 0);
            geometry = gf.CreateGeometry(envelope);

            Common.DumpGeometry(geometry);
            Common.CheckFGFT(geometry, "POLYGON XYZ ((1 1 0, 2 1 0, 2 2 0, 1 2 0, 1 1 0))");

            //Console.WriteLine("finish mgGeometryTest.testGeometryFromEnvelope");
            
        }
	}
}
