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
using System.IO;

using OSGeo.Geometry;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgGeometryMemPerfTest.
	/// </summary>
	public class mgGeometryMemPerfTest
	{
        public const int Dimensionality_XY = 0;
        public const int Dimensionality_Z = 1;
        public const int Dimensionality_M = 2;

        public const int MAX_ACTUAL_NUM_ORDINATES = 3*30000;
        public const int MAX_ACTUAL_NUM_FEATS = 1001;
        public const int MAX_NUM_FEATS = 700;

        private FeatInfo2_t[] features_S = new FeatInfo2_t[MAX_ACTUAL_NUM_FEATS];
        private double[] featureOrdinates_S = new double[MAX_ACTUAL_NUM_ORDINATES];
        private byte[][] byteArrays_S = new byte[MAX_NUM_FEATS][];

        private int numGeoms_S = 0;
        private int numByteArrays_S = 0;
        private int actualNumFeats = 0;
        private int actualNumOrdinates = 0;

        private class FeatInfo2_t
        {
            public int featId;
            public int numOrdinates;
            public double[] ordinates;
        }

		public mgGeometryMemPerfTest()
		{
            Console.WriteLine("start mgGeometryMemPerfTest");
		}

        public void testFgfConversion()
        {
            Console.WriteLine("start mgGeometryMemPerfTest.testFgfConversion");

            FgfGeometryFactory gfUnaligned = new FgfGeometryFactory();
	      
            readTextFile_S();

#if DUMPTESTCONTENT
            Console.WriteLine("In-memory limited conversion to unaligned FGF Polygons, retained as FGF...");
#endif
            convertBinaryOrdinatesToPolygons(gfUnaligned, MAX_ACTUAL_NUM_FEATS);

            numGeoms_S = 0;;

#if DUMPTESTCONTENT
            Console.WriteLine("In-memory use of limited retained, unaligned FGF, accessed via streamed geometries...");
#endif
            accessFgfStreamed(gfUnaligned, MAX_NUM_FEATS, true);

            //Console.WriteLine("finish mgGeometryMemPerfTest.testFgfConversion");
            
        }
        
        private void readTextFile_S()
        {
            string coord_file = "TestFiles\\g_coords.txt";
            StreamReader coor = new StreamReader(coord_file);
            Debug.Assert(coor != null, "Failed to open input file!");

            actualNumFeats = 0;
            actualNumOrdinates = 0;
            int currentFeatId = -1;
            int nextFeatId;
            int nextSequenceNum;
            double nextX;
            double nextY;
            FeatInfo2_t currentFeature = null;
            FeatInfo2_t[] nextFeature = features_S;
            int featureIndex = 0;

            double[] nextOrdinate = featureOrdinates_S;
            int ordinateIndex = 0;

            string line = null;
            while ((line = coor.ReadLine()) != null && actualNumFeats < MAX_ACTUAL_NUM_FEATS && actualNumOrdinates < MAX_ACTUAL_NUM_ORDINATES)
            {
                string[] values = line.Split(new char[] {','});
                if (4 == values.Length)
                {
                    nextFeatId = Convert.ToInt32(values[0]);
                    nextSequenceNum = Convert.ToInt32(values[1]);
                    nextX = Convert.ToSingle(values[2]);
                    nextY = Convert.ToSingle(values[3]);

                    if (nextFeatId != currentFeatId)
                    {
                        nextFeature[featureIndex] = new FeatInfo2_t();
                        nextFeature[featureIndex].featId = nextFeatId;
                        nextFeature[featureIndex].ordinates = nextOrdinate;
                        nextFeature[featureIndex].numOrdinates = 0;
                        currentFeatId = nextFeatId;
                        currentFeature = nextFeature[featureIndex];

                        featureIndex++;
                        actualNumFeats++;
                    }
                    nextOrdinate[ordinateIndex++] = nextX;
                    nextOrdinate[ordinateIndex++] = nextY;
                    nextOrdinate[ordinateIndex++] = 0.0;
     
                    currentFeature.numOrdinates += 3;
                    actualNumOrdinates += 3;
                }
            }
        }

        private void convertBinaryOrdinatesToPolygons(FgfGeometryFactory gf1, int numFeatures)
        {
            int i;
            FgfGeometryFactory theGf = gf1;

            FeatInfo2_t feat1 = null;
            FeatInfo2_t feat2 = null;
            numGeoms_S = 0;
            int numGeometries = 0;
            int numOrdinates = 0;

            for (i = 0;  i < numFeatures-1;  i += 2)
            {
                // Treat each input feature as a polygon ring.  Assign every
                // two input rings to a new polygon.
                feat1 = getFeatureByIndex(i);
                feat2 = getFeatureByIndex(i+1);
                if (feat1.numOrdinates >= 12 && feat2.numOrdinates >= 12)    // If >= 4 3D positions to make a ring
                {
                    // Ensure closedness of rings.
                    double[] ords = feat1.ordinates;
                    int numOrds = feat1.numOrdinates;
                    ords[numOrds-3] = ords[0];
                    ords[numOrds-2] = ords[1];
                    ords[numOrds-1] = ords[2];

                    ords = feat2.ordinates;
                    numOrds = feat2.numOrdinates;
                    ords[numOrds-3] = ords[0];
                    ords[numOrds-2] = ords[1];
                    ords[numOrds-1] = ords[2];

                    ILinearRing extRing = gf1.CreateLinearRing(Dimensionality_XY | Dimensionality_Z, feat1.numOrdinates, feat1.ordinates);
                    ILinearRing intRing = gf1.CreateLinearRing(Dimensionality_XY | Dimensionality_Z, feat2.numOrdinates, feat2.ordinates);
                    LinearRingCollection intRings = new LinearRingCollection();
                    intRings.Add(intRing);
                    IPolygon geometry = gf1.CreatePolygon(extRing, intRings);

                    numGeometries++;
                    numOrdinates += feat1.numOrdinates + feat2.numOrdinates;
                    byteArrays_S[numByteArrays_S++] = gf1.GetFgf(geometry);
                }
            }
        }

        private void accessFgfStreamed(FgfGeometryFactory gf1, int numFeatures, bool useAccessorsByValue)
        {
            numGeoms_S = 0;
            int numGeometries = 0;

            for (int i = 0;  i < numFeatures;  i++)
            {
                byte[] ba = getByteArrayByIndex(i);
                IGeometry geometry = gf1.CreateGeometryFromFgf(ba);
                numGeometries++;
//                GeometryType geometryType = geometry.DerivedType;
//                switch ( geometryType )
//                {
//                    case GeometryType_LineString:
//                    {
//                        ILineString ls = (ILineString) geometry;
//                        totalNumPositions += accessLineString(ls, useAccessorsByValue);
//                    }
//                        break;
//                    case GeometryType_Polygon:
//                    {
//                        IPolygon poly = (IPolygon) geometry;
//                        totalNumPositions += accessPolygon(poly, useAccessorsByValue);
//                    }
//                    default:
//                        ;   // No report for unsupported types.
//                }
            }
        }

        private FeatInfo2_t getFeatureByIndex(int indx)
        {
            FeatInfo2_t feature = null;

            if (actualNumFeats > 0)
            {
                // We simulate a large # of features by just cycling over
                // a limited list of them.
                int actualIndex = indx % actualNumFeats;
                feature = features_S[actualIndex];
            }
            return feature;
        }

        private byte[] getByteArrayByIndex(int indx)
        {
            byte[] array = null;

            if (numByteArrays_S > 0)
            {
                // We simulate a large # of arrays by just cycling over
                // a limited list of them.
                int actualIndex = indx % numByteArrays_S;
                array = byteArrays_S[actualIndex];
            }
            return array;
        }

        private int accessLineString(ILineString geometry, bool useAccessorsByValue)
        {
            int numPositions = geometry.Count;

            if (useAccessorsByValue)
            {
                double x = 0.0 , y = 0.0, z = 0.0, m = 0.0;
                int dim = 0;
                for (int i = 0;  i < numPositions;  i++)
                {
                    geometry.GetItemByMembers(i, ref x, ref y, ref z, ref m, ref dim);
                }
            }
            else
            {
                for (int i = 0;  i < numPositions;  i++)
                {
                    IDirectPosition pos = geometry.get_Item(i);
                }
            }
            return numPositions;
        }
	}
}
