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
using OSGeo.FDO.Spatial;
using OSGeo.FDO.Common;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for CommonFuntion.
	/// </summary>
	public class Common
	{
        public const int Dimensionality_XY = 0;
        public const int Dimensionality_Z = 1;
        public const int Dimensionality_M = 2;

        //////////////////////////////////////////////////////////////////////
        // Dump...
        //////////////////////////////////////////////////////////////////////
        public static void DumpPosition(string context, IDirectPosition p)
        {
#if DUMPTESTCONTENT
            if (p.Dimensionality == Dimensionality_XY)
                Console.WriteLine(" {0} XY=({1}, {2})", context, p.X, p.Y);
            else if (p.Dimensionality == (Dimensionality_XY|Dimensionality_Z))
                Console.WriteLine(" {0} XYZ=({1}, {2}, {3})", context, p.X, p.Y, p.Z);
            else if (p.Dimensionality == (Dimensionality_XY|Dimensionality_M))
                Console.WriteLine(" {0} XYM=({1}, {2}, {3})", context, p.X, p.Y, p.M);
            else if (p.Dimensionality == (Dimensionality_XY|Dimensionality_Z|Dimensionality_M))
                Console.WriteLine(" {0} XYZM=({1}, {2}, {3}, {4})", context, p.X, p.Y, p.Z, p.M);
            else
                Debug.Assert(false, "DumpPosition::UNKNOWN_DIMENSION_TYPE!");
#endif
        }

        public static void DumpEnvelope(IEnvelope env)
        {
#if DUMPTESTCONTENT
            Console.WriteLine(" Envelope=min({0}, {1}, {2}) max({3}, {4}, {5})", env.MinX, env.MinY, env.MinZ, env.MaxX, env.MaxY, env.MaxZ);
#endif
        }

        // Dump FGFT
        public static void DumpFGFT(IGeometry p)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("FGFT = {0}", p.Text);
 #endif
       }

        public static void DumpLineString(ILineString line)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3}) closed({4})", "LINESTRING", line.Count, line.Dimensionality, line.DerivedType, line.IsClosed);
#endif

            IDirectPosition pStart = line.StartPosition;
            DumpPosition("", pStart);
        
            IDirectPosition pEnd = line.EndPosition;
            DumpPosition("", pEnd);
        
            IEnvelope envl = line.Envelope;
            DumpEnvelope(envl);
        
            for (int i=0; i<line.Count; i++)
            {
                IDirectPosition pos = line.get_Item(i);
                DumpPosition("", pos);
            }
            DumpFGFT(line);
        }
        
        public static void DumpLinearRing(ILinearRing lRing)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2})", "(LINEARRING)", lRing.Count, lRing.Dimensionality);
#endif

            IEnvelope envl = lRing.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<lRing.Count; i++)
            {
                IDirectPosition pos = lRing.get_Item(i);
                DumpPosition("", pos);
            }

        }

        public static void DumpLineStringSegment(ILineStringSegment lSeg)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3}) closed({4})", "(LINESTRINGSEGMENT)", lSeg.Count, lSeg.Dimensionality, lSeg.DerivedType, lSeg.IsClosed);
#endif

            IDirectPosition pStart = lSeg.StartPosition;
            DumpPosition("", pStart);

            IDirectPosition pEnd = lSeg.EndPosition;
            DumpPosition("", pEnd);

            IEnvelope envl = lSeg.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<lSeg.Count; i++)
            {
                IDirectPosition pos = lSeg.get_Item(i);
                DumpPosition("", pos);
            }
        }

        
        public static void DumpPolygon(IPolygon polygon)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:interiorRingCount({1}) dimtype({2}) derivedtype({3})", "POLYGON", polygon.InteriorRingCount, polygon.Dimensionality, polygon.DerivedType);
#endif

            ILinearRing extRing = polygon.ExteriorRing;
            DumpLinearRing(extRing);

            for (int i = 0; i<polygon.InteriorRingCount; i++)
            {
                ILinearRing intRing = polygon.GetInteriorRing(i);
                DumpLinearRing(intRing);
            }

            IEnvelope envl = polygon.Envelope;
            DumpEnvelope(envl);

            DumpFGFT(polygon);
        }

        public static void DumpGeometry(IGeometry geometry)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}: dimtype({1}) derivedtype({2})", "GEOMETRY", geometry.Dimensionality, geometry.DerivedType);
#endif
            DumpFGFT(geometry);
        }

        public static void DumpPoint(IPoint pnt)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:dimtype({1}) derivedtype({2})", "POINT", pnt.Dimensionality, pnt.DerivedType);
#endif

            IEnvelope envl = pnt.Envelope;
            DumpEnvelope(envl);

            IDirectPosition pos = pnt.Position;
            DumpPosition("", pos);

            DumpFGFT(pnt);
        }

        public static void DumpMultiPoint(IMultiPoint multiPnt)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3})", "MULTIPOINT", multiPnt.Count, multiPnt.Dimensionality, multiPnt.DerivedType);
#endif

            IEnvelope envl = multiPnt.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<multiPnt.Count; i++)
            {
                IPoint pnt = multiPnt.get_Item(i);
                DumpPoint(pnt);
            }
            DumpFGFT(multiPnt);
        }

        public static void DumpMultiLineString(IMultiLineString multiLine)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3})", "MULTILINESTRING", multiLine.Count, multiLine.Dimensionality, multiLine.DerivedType);
#endif

            IEnvelope envl = multiLine.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<multiLine.Count; i++)
            {
                ILineString lineString = multiLine.get_Item(i);
                DumpLineString(lineString);
            }
            DumpFGFT(multiLine);
        }

        public static void DumpMultiPolygon(IMultiPolygon multiPoly)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3})", "MULTIPOLYGON", multiPoly.Count, multiPoly.Dimensionality, multiPoly.DerivedType);
#endif
            IEnvelope envl = multiPoly.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<multiPoly.Count; i++)
            {
                IPolygon polygon = multiPoly.get_Item(i);
                DumpPolygon(polygon);
            }
            DumpFGFT(multiPoly);
        }

        public static void DumpCircularArcSegment(ICircularArcSegment arcSeg)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:dimtype({1}) derivedtype({2}) closed({3})", "(CIRCULARARCSEGMENT)", arcSeg.Dimensionality, arcSeg.DerivedType, arcSeg.IsClosed);
#endif

            IEnvelope envl = arcSeg.Envelope;
            DumpEnvelope(envl);

            IDirectPosition startPos = arcSeg.StartPosition;
            DumpPosition("", startPos);

            IDirectPosition midPos = arcSeg.MidPoint;
            DumpPosition("", midPos);

            IDirectPosition endPos = arcSeg.EndPosition;
            DumpPosition("", endPos);
        }

        public static void DumpCurveString(ICurveString curveString)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3}) closed({4})", "CURVESTRING", curveString.Count, curveString.Dimensionality, curveString.DerivedType, curveString.IsClosed);
#endif

            IDirectPosition startPos = curveString.StartPosition;
            DumpPosition("StartPoint", startPos);

            IDirectPosition endPos = curveString.EndPosition;
            DumpPosition("EndPoint", endPos);

            IEnvelope envl = curveString.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<curveString.Count; i++)
            {
                ICurveSegmentAbstract seg = curveString.get_Item(i);
                switch (seg.DerivedType)
                {
                    case GeometryComponentType.GeometryComponentType_LineStringSegment:
                        {
                            DumpLineStringSegment((ILineStringSegment)(seg));
                            break;
                        }
                    case GeometryComponentType.GeometryComponentType_CircularArcSegment:
                        {
                            DumpCircularArcSegment((ICircularArcSegment)(seg));
                            break;
                        }
                    default:
                        Debug.Assert(false);
                        break;
                } 
            } 

            DumpFGFT(curveString);
        }



        public static void DumpRing(IRing ring)
        {
#if DUMPTESTCONTENT
           Console.WriteLine("{0}:count({1}) dimtype({2})", "(RING)", ring.Count, ring.Dimensionality);
#endif

            IEnvelope envl = ring.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<ring.Count; i++)
            {
                ICurveSegmentAbstract seg = ring.get_Item(i);
                switch(seg.DerivedType)
                {
                    case GeometryComponentType.GeometryComponentType_LineStringSegment:
                    {
                        DumpLineStringSegment((ILineStringSegment)seg);
                        break;
                    }
                    case GeometryComponentType.GeometryComponentType_CircularArcSegment:
                    {
                        DumpCircularArcSegment((ICircularArcSegment)seg);
                        break;
                    }
            } 
            } 
        }

        public static void DumpCurvePolygon(ICurvePolygon curvePoly)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:interiorRingCount({1}) dimtype({2}) derivedtype({3})", "CURVEPOLYGON", curvePoly.InteriorRingCount, curvePoly.Dimensionality, curvePoly.DerivedType);
#endif

            IRing extRing = curvePoly.ExteriorRing;
            DumpRing(extRing);

            for (int i=0; i<curvePoly.InteriorRingCount; i++)
            {
                IRing ring = curvePoly.get_InteriorRing(i);
                DumpRing(ring);
            }

            IEnvelope envl = curvePoly.Envelope;
            DumpEnvelope(envl);

            DumpFGFT(curvePoly);
        }

        public static void DumpMultiCurvePolygon(IMultiCurvePolygon multiCurvePoly)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3})", "MULTICURVEPOLYGON", multiCurvePoly.Count, multiCurvePoly.Dimensionality, multiCurvePoly.DerivedType);
#endif

            for (int i=0; i<multiCurvePoly.Count; i++)
            {
                ICurvePolygon curvePoly = multiCurvePoly.get_Item(i);
                DumpCurvePolygon(curvePoly);
            }

            IEnvelope envl = multiCurvePoly.Envelope;
            DumpEnvelope(envl);
            DumpFGFT(multiCurvePoly);
        }

        public static void DumpMultiCurveString(IMultiCurveString multiCurveString)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3})", "MULTICURVESTRING", multiCurveString.Count, multiCurveString.Dimensionality, multiCurveString.DerivedType);
#endif

            for (int i=0; i<multiCurveString.Count; i++)
            {
                ICurveString curveString = multiCurveString.get_Item(i);
                DumpCurveString(curveString);
            }

            IEnvelope envl = multiCurveString.Envelope;
            DumpEnvelope(envl);
            DumpFGFT(multiCurveString);
        }

        public static void DumpMultiGeometry(IMultiGeometry multiGeometry)
        {
#if DUMPTESTCONTENT
            Console.WriteLine("{0}:count({1}) dimtype({2}) derivedtype({3}))", "MULTIGEOMETRY", multiGeometry.Count, multiGeometry.Dimensionality, multiGeometry.DerivedType);
#endif

            IEnvelope envl = multiGeometry.Envelope;
            DumpEnvelope(envl);

            for (int i=0; i<multiGeometry.Count; i++)
            {
                IGeometry geometry = multiGeometry.get_Item(i);
                switch (geometry.DerivedType)
                {
                    case GeometryType.GeometryType_LineString:
                        DumpLineString((ILineString)geometry);
                        break;

                    case GeometryType.GeometryType_Point:
                        DumpPoint((IPoint)geometry);
                        break;

                    case GeometryType.GeometryType_Polygon:
                        DumpPolygon((IPolygon)geometry);
                        break;

                    case GeometryType.GeometryType_CurveString:
                        DumpCurveString((ICurveString)geometry);
                        break;

                    case GeometryType.GeometryType_CurvePolygon:
                        DumpCurvePolygon((ICurvePolygon)geometry);
                        break;
                    default:
                        Debug.Assert(false, string.Format("MultiGeometry test does not support item type of {0}.", geometry.DerivedType));
                        break;
                }
            }
            DumpFGFT(multiGeometry);
        }
        //////////////////////////////////////////////////////////////////////
        // Checkers...
        //////////////////////////////////////////////////////////////////////
        public static void CheckPositionXY(IDirectPosition pos, double x, double y)
        {
            Debug.Assert(pos.X == x, "X value mismatch!");
            Debug.Assert(pos.Y == y, "Y value mismatch!");
        }

        public static void CheckPositionXYM(IDirectPosition pos, double x, double y, double m)
        {
            CheckPositionXY(pos, x, y);
            Debug.Assert(pos.M == m, "M value mismatch!");
        }

        public static void CheckPositionXYZ(IDirectPosition pos, double x, double y, double z)
        {
            CheckPositionXY(pos, x, y);
            Debug.Assert(pos.Z == z, "Z value mismatch!");
        }

        public static void CheckPositionXYZM(IDirectPosition pos, double x, double y, double z, double m)
        {
            CheckPositionXYZ(pos, x, y, z);
            Debug.Assert(pos.M == m, "M value mismatch!");
        }

        public static void CheckEnvelope(IEnvelope envl, double[] ordsXY)
        {
            Debug.Assert(envl.MinX == ordsXY[0], "MinX mismatch!");
            Debug.Assert(envl.MinY == ordsXY[1], "MinY mismatch!");
            Debug.Assert(envl.MaxX == ordsXY[2], "MaxX mismatch!");
            Debug.Assert(envl.MaxY == ordsXY[3], "MaxY mismatch!");
        }

        public static void CheckEnvelope(IEnvelope envl, IDirectPosition pos1, IDirectPosition pos2)
        {
            Debug.Assert(envl.MinX == pos1.X, "MinX mismatch!");
            Debug.Assert(envl.MinY == pos1.Y, "MinY mismatch!");
            Debug.Assert((pos1.Dimensionality & Dimensionality_Z) == 0 || envl.MinZ == pos1.Z, "MinZ mismatch!");
            Debug.Assert(envl.MaxX == pos2.X, "MaxX mismatch!");
            Debug.Assert(envl.MaxY == pos2.Y, "MaxY mismatch!");
            Debug.Assert((pos2.Dimensionality & Dimensionality_Z) == 0 || envl.MaxZ == pos2.Z, "MaxZ mismatch!");
        }

        public static void CheckEnvelopeXY(IEnvelope envl, double minx, double miny, double maxx, double maxy)
        {
            Debug.Assert(envl.MinX == minx, "MinX mismatch!");
            Debug.Assert(envl.MinY == miny, "MinY mismatch!");
            Debug.Assert(envl.MaxX == maxx, "MaxX mismatch!");
            Debug.Assert(envl.MaxY == maxy, "MaxY mismatch!");
        }

        public static void CheckEnvelopeXYZ(IEnvelope envl, double minx, double miny, double minz, double maxx, double maxy, double maxz)
        {
            CheckEnvelopeXY(envl, minx, miny, maxx, maxy);
            Debug.Assert(envl.MinZ == minz, "MinZ mismatch!");
            Debug.Assert(envl.MaxZ == maxz, "MaxZ mismatch!");
        }

        public static void CheckEqualEnvelopes(IEnvelope envl1, IEnvelope envl2)
        {
            Debug.Assert(envl1.MinX == envl2.MinX, "Envelope MinX mismatch!");
            Debug.Assert(envl1.MinY == envl2.MinY, "Envelope MinY mismatch!");
            Debug.Assert(envl1.MinZ == envl2.MinZ, "Envelope MinZ mismatch!");
            Debug.Assert(envl1.MaxX == envl2.MaxX, "Envelope MaxX mismatch!");
            Debug.Assert(envl1.MaxY == envl2.MaxY, "Envelope MaxY mismatch!");
            Debug.Assert(envl1.MaxZ == envl2.MaxZ, "Envelope MaxZ mismatch!");
        }

        //TODO: use parens in FGFT formatter
        public static void CheckFGFT(IGeometry geom, string fgft)
        {
            DumpFGFT(geom);
            if (geom.Text != fgft)
            {
                mgUnitTestUtil.PrintException(geom.Text, @"c:\1.txt");
                mgUnitTestUtil.PrintException(fgft, @"c:\2.txt");
            }
            Debug.Assert(!(geom.Text != fgft), string.Format("FGFT mismatch! <{0}> should be <{1}>", geom.Text, fgft));
        }

        public static void CheckGeometryValidity(SpatialGeometryValidity validity1, SpatialGeometryValidity validity2)
        {
            Debug.Assert(validity1 == validity2, "Geometry Validity mismatch.");
        }

        //////////////////////////////////////////////////////////////////////
        // Helpers to create repetitive geometries

        public static ICurveString CreateCurveString(double offset)
        {
            FgfGeometryFactory gf = new FgfGeometryFactory();

            IDirectPosition startPos = gf.CreatePositionXY(offset+0.0, offset+0.0);
            IDirectPosition midPos = gf.CreatePositionXY(offset+0.0, offset+1.0);
            IDirectPosition endPos = gf.CreatePositionXY(offset+1.0, offset+2.0);

            ICircularArcSegment arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            DirectPositionCollection points = new DirectPositionCollection();
            IDirectPosition pt1 = gf.CreatePositionXY(offset+1.0, offset+2.0);
            IDirectPosition pt2 = gf.CreatePositionXY(offset+3.0, offset+0.0);
            IDirectPosition pt3 = gf.CreatePositionXY(offset+3.0, offset+2.0);
            points.Add(pt1);
            points.Add(pt2);
            points.Add(pt3);

            ILineStringSegment lineSeg = gf.CreateLineStringSegment(points);

            CurveSegmentCollection curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);
            curveSegs.Add(lineSeg);

            return gf.CreateCurveString(curveSegs);
        }

        public static IRing CreateRing(double offset)
        {
            FgfGeometryFactory gf = new FgfGeometryFactory();

            IDirectPosition startPos = gf.CreatePositionXY(offset+0.0, offset+0.0);
            IDirectPosition midPos = gf.CreatePositionXY(offset+0.0, offset+1.0);
            IDirectPosition endPos = gf.CreatePositionXY(offset+1.0, offset+2.0);

            ICircularArcSegment arcSeg = gf.CreateCircularArcSegment(startPos, midPos, endPos);

            DirectPositionCollection points = new DirectPositionCollection();
            IDirectPosition fromPt = gf.CreatePositionXY(offset+1.0, offset+2.0);
            IDirectPosition toPt = gf.CreatePositionXY(offset+0.0, offset+0.0);
            points.Add(fromPt);
            points.Add(toPt);

            ILineStringSegment lineSeg = gf.CreateLineStringSegment(points);

            CurveSegmentCollection curveSegs = new CurveSegmentCollection();
            curveSegs.Add(arcSeg);
            curveSegs.Add(lineSeg);

            return gf.CreateRing(curveSegs);
        }

        public static ICurvePolygon CreateCurvePolygon(double offset)
        {
            IRing extRing = CreateRing(offset + 100);

            RingCollection intRings = new RingCollection();

            IRing ring1 = CreateRing(offset + 200);
            IRing ring2 = CreateRing(offset + 300);
            intRings.Add(ring1);
            intRings.Add(ring2);

            FgfGeometryFactory gf = new FgfGeometryFactory();

            return gf.CreateCurvePolygon(extRing, intRings);
        }

        public static IMultiCurvePolygon CreateMultiCurvePolygon(int numCurvePolys, double offset)
        {
            CurvePolygonCollection curvePolys = new CurvePolygonCollection();

            for (int i=0; i<numCurvePolys; i++)
            {
                ICurvePolygon curvePoly = CreateCurvePolygon(i+offset);
                curvePolys.Add(curvePoly);
            }

            FgfGeometryFactory gf = new FgfGeometryFactory();

            return gf.CreateMultiCurvePolygon(curvePolys);
        }

        public static IMultiGeometry CreateMultiGeometry()
        {
            GeometryCollection geometries = new GeometryCollection();

            FgfGeometryFactory gf = new FgfGeometryFactory();

            IGeometry geometry;

            // CurvePolygon
            geometry = CreateCurvePolygon(0);
            geometries.Add(geometry);

            // CurveString
            // Not doing CurveString because of some unfixed defect.
            // It may be the same one that sometimes affects MultiPolygon.
            geometry = CreateCurveString(100);
            geometries.Add(geometry);

            // LineString
            double[] ordsXY = new double[6];
            ordsXY[0] = 0.0; ordsXY[1] = 1.0;
            ordsXY[2] = 2.0; ordsXY[3] = 3.0;
            ordsXY[4] = 4.0; ordsXY[5] = 5.0;
            geometry = gf.CreateLineString(Dimensionality_XY, 6, ordsXY);
            geometries.Add(geometry);

            // Point
            double[] ordsXYZ = new double[3];
            ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;
            geometry = gf.CreatePoint(Dimensionality_XY|Dimensionality_Z, ordsXYZ);
            geometries.Add(geometry);

            // Polygon
            // Not doing Polygon because of some unfixed defect.
            // It may be the same one that sometimes affects MultiPolygon.
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

            ILinearRing extRing = gf.CreateLinearRing(Dimensionality_XY, 10, ordsXYExt);
            ILinearRing intRing1 = gf.CreateLinearRing(Dimensionality_XY, 8, ordsXYInt1);
            ILinearRing intRing2 = gf.CreateLinearRing(Dimensionality_XY, 8, ordsXYInt2);
            LinearRingCollection intRings = new LinearRingCollection();
            intRings.Add(intRing1);
            intRings.Add(intRing2);
            geometry = gf.CreatePolygon(extRing, intRings);
            geometries.Add(geometry);

            // Make MultiGeometry from the many geometries collected above.
            return gf.CreateMultiGeometry(geometries);
        }
	}
}

