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

#include "Pch.h"
#include "GeometryMemPerfTest.h"
#include "UnitTestUtil.h"
#include "stdio.h"
#include "fcntl.h" // _O_TEXT

#ifdef _WIN32   
#include <sys\timeb.h>
#include <time.h>
#else
#include    <sys/time.h>
// gcc 4 complains that errno doesn't has its name scope.
#include    <errno.h>
#endif

#include "Geometry.h"

static bool VERBOSE = true;

CPPUNIT_TEST_SUITE_REGISTRATION( GeometryMemPerfTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GeometryMemPerfTest, "GeometryMemPerfTest");

GeometryMemPerfTest::GeometryMemPerfTest(void)
{
}

GeometryMemPerfTest::~GeometryMemPerfTest(void)
{
}


// Get time on system clock (in seconds) -- used for computing elapsed time.
static double getTime_S(
        void
        )
{
#ifdef _WIN32   

   //struct _timeb	timebuffer;
   //double			micro;
   double           our_time;
 
   //_ftime( &timebuffer );
   //micro = (double) ((double) timebuffer.millitm / (double) 1000000.0);
   //our_time = micro + timebuffer.time;
   our_time = GetTickCount() / 1000.0;
   return our_time;
 
#else

	struct timeval			this_time;
	static struct timezone	time_zone = { 0, 0 };
	double					sec, micro, time;


	if (gettimeofday(&this_time, &time_zone) == -1)
		return (double) 0.0;

	micro = (double) ((double) this_time.tv_usec / (double) 1000000.0);
	sec = (double) this_time.tv_sec;
	time = micro + sec;
	return time;
	
#endif 
}

void GeometryMemPerfTest::setUp ()
{
    // nothing to do at setup time... yet
}
typedef struct _featinfo2_ {
    FdoInt32      featId;
    FdoInt32      numOrdinates;
    double * ordinates; // Points into an array.
} FeatInfo2_t;

static FeatInfo2_t  features_S[1001];
#define MAX_ACTUAL_NUM_FEATS   (sizeof(features_S)/sizeof(features_S[0]))
//#define MAX_NUM_FEATS          (552319)       // Extra features will be simulated.
//#define MAX_NUM_FEATS          (15000000)       // Extra features will be simulated.
//#define MAX_NUM_FEATS          (700000)       // Extra features will be simulated.
#define MAX_NUM_FEATS          (7000)       // Extra features will be simulated.
static FdoInt32 actualNumFeats = 0;

static double featureOrdinates_S[3*30000];  // FeatInfo_2_t::ordinates values point into here.
#define MAX_ACTUAL_NUM_ORDINATES   (sizeof(featureOrdinates_S)/sizeof(featureOrdinates_S[0]))
static FdoInt32 actualNumOrdinates = 0;

static FdoPtr<FdoIGeometry> geoms_S[MAX_NUM_FEATS];
static FdoInt32 numGeoms_S = 0;

static FdoPtr<FdoByteArray> byteArrays_S[MAX_NUM_FEATS];
static FdoInt32 numByteArrays_S = 0;

// Read the textual test feature file, as far as memory buffers allow.
static void readTextFile_S()
{
    char                coord_file[256];

#ifdef _WIN32
    strcpy(coord_file,".\\g_coords.txt");
#else
    char *root = getenv("ROOT");
    char dir[256];
    if (root)
        sprintf(dir, "%s/UnitTest/", root);
    else
        strcpy(dir, "./");
    sprintf(coord_file, "%s%s", dir, "g_coords.txt");
#endif

#ifdef _WIN32
    double time1 = getTime_S();
#endif
    FILE *coor = fopen(coord_file, "r");
    if (NULL == coor)
    {
        printf("Failed to open input file '%s'.\n", coord_file);
    }

    actualNumFeats = 0;
    actualNumOrdinates = 0;
    FdoInt32 currentFeatId = -1;
    FdoInt32 nextFeatId;
    FdoInt32 nextSequenceNum;
    double nextX;
    double nextY;
    FeatInfo2_t * currentFeature = NULL;
    FeatInfo2_t * nextFeature = features_S;
    double      * nextOrdinate = featureOrdinates_S;

    while (!feof(coor) &&
            actualNumFeats < MAX_ACTUAL_NUM_FEATS && 
            actualNumOrdinates < MAX_ACTUAL_NUM_ORDINATES)
	{
        FdoInt32 numRead = fscanf(coor, "%d,%d,%lf,%lf",
            &nextFeatId, &nextSequenceNum, &nextX, &nextY);
        if (4 == numRead)
        {
            if (nextFeatId != currentFeatId)
            {
                nextFeature->featId = nextFeatId;
                nextFeature->ordinates = nextOrdinate;
                nextFeature->numOrdinates = 0;
                currentFeatId = nextFeatId;
                currentFeature = nextFeature;
                nextFeature++;
                actualNumFeats++;
            }
            *(nextOrdinate++) = nextX;
            *(nextOrdinate++) = nextY;
            *(nextOrdinate++) = 0.0;        // Z ordinate
            currentFeature->numOrdinates += 3;
            actualNumOrdinates += 3;
        }
	}

    fclose(coor);
    coor = NULL;

#ifdef _WIN32
    double time2 = getTime_S();
    double elapsed = time2 - time1;
    if (VERBOSE)
        printf("Elapsed: %lf sec\n",elapsed);
#endif
}

// Write the byte arrays to a file.
static void writeByteArraysToFile_S()
{
    char                coord_file[256];

#ifdef _WIN32
    char *              mode = "wb";
    strcpy(coord_file,".\\g_coords.fg");
#else
    char *              mode = "w";
    char *root = getenv("ROOT");
    char dir[256];
    if (root)
        sprintf(dir, "%s/Src/Fdo/UnitTest/", root);
    else
        strcpy(dir, "./");
    sprintf(coord_file, "%s%s", dir, "g_coords.fg");
#endif

#ifdef _WIN32
    double time1 = getTime_S();
#endif
    FILE *coor = fopen(coord_file, mode);

    for (FdoInt32 i = 0;  i < numByteArrays_S;  i++ )
	{
        FdoPtr<FdoByteArray> ba = byteArrays_S[i];
        FdoInt32 size = ba->GetCount();
        size_t numWritten = fwrite( &size, sizeof(size), 1, coor);
        if (numWritten != 1)
            CPPUNIT_FAIL ( "Error writing FGF file" );
        numWritten = fwrite(ba->GetData(), size, 1, coor);
	}

    fclose(coor);
    coor = NULL;

#ifdef _WIN32
    double time2 = getTime_S();
    double elapsed = time2 - time1;
    if (VERBOSE)
        printf("Elapsed: %lf sec\n",elapsed);
#endif
}

// Read the byte arrays from a file.
static void readByteArraysFromFile_S()
{
    char                coord_file[256];

#ifdef _WIN32
    char *              mode = "rb";
    strcpy(coord_file,".\\g_coords.fg");
#else
    char *              mode = "r";
    char *root = getenv("ROOT");
    char dir[256];
    if (root)
        sprintf(dir, "%s/Src/Fdo/UnitTest/", root);
    else
        strcpy(dir, "./");
    sprintf(coord_file, "%s%s", dir, "g_coords.fg");
#endif

#ifdef _WIN32
    double time1 = getTime_S();
#endif
    FILE *coor = fopen(coord_file, mode);

    numByteArrays_S = 0;

    while (!feof(coor) &&
            numByteArrays_S < MAX_NUM_FEATS)
	{
        FdoInt32 size = 0;
        size_t numRead = fread( &size, sizeof(size), 1, coor);
        if (numRead != 1)
            CPPUNIT_FAIL ( "Error reading FGF file" );
        FdoByteArray * ba = FdoByteArray::Create(size);
        ba = FdoByteArray::SetSize(ba, size);
        numRead = fread(ba->GetData(), size, 1, coor);
        if (numRead != 1)
        {
            char buffer[1024];
            sprintf (buffer, "Error reading FGF file (rc=%d, ferror=%d)", numRead, ferror(coor));
            CPPUNIT_FAIL ( buffer );
        }
        if( ferror( coor ) )
        {
            char buffer[1024];
			// gcc 4 complains that errno doesn't has its name scope.
            //sprintf (buffer, "Read error: %s", strerror (errno));
            CPPUNIT_FAIL ( buffer );
        }
        byteArrays_S[numByteArrays_S++] = ba;
	}

    fclose(coor);
    coor = NULL;

#ifdef _WIN32
    double time2 = getTime_S();
    double elapsed = time2 - time1;
    if (VERBOSE)
        printf("Elapsed: %lf sec for %d byte arrays.\n",elapsed, numByteArrays_S);
#endif
}

static FeatInfo2_t * getFeatureByIndex(FdoInt32 indx)
{
    FeatInfo2_t * feature = NULL;

    if (actualNumFeats > 0)
    {
        // We simulate a large # of features by just cycling over
        // a limited list of them.
        FdoInt32 actualIndex = indx % actualNumFeats;
        feature = &features_S[actualIndex];
    }
    return feature;
}


static FdoByteArray * getByteArrayByIndex(FdoInt32 indx)
{
    FdoByteArray * array = NULL;

    if (numByteArrays_S > 0)
    {
        // We simulate a large # of arrays by just cycling over
        // a limited list of them.
        FdoInt32 actualIndex = indx % numByteArrays_S;
        array = byteArrays_S[actualIndex];
        array->AddRef();
    }
    return array;
}


static void convertBinaryOrdinatesToLineStrings(
                                    FdoFgfGeometryFactory * gf1,
                                    FdoFgfGeometryFactory * gf2,
                                    FdoInt32  numFeatures,
                                    bool useAccessorsByValue,
                                    bool streamed,
                                    bool retainAsFgf
                                    )
{
    try
    {
        try
        {
            FdoInt32                 i, j;
            FdoFgfGeometryFactory * theGf = gf1;
            //FdoPtr<FdoILineString> theLine = NULL;
            FdoILineString *         theLine = NULL;  // Avoid smart pointer to enable geometry pooling (in factory)


            FeatInfo2_t * feat = NULL;
            numGeoms_S = 0;
            FdoInt32 numGeometries = 0;
            FdoInt32 numOrdinates = 0;

            double time1b = getTime_S();

            for (i = 0;  i < numFeatures;  i++)
            {
                feat = getFeatureByIndex(i);
                if (feat->numOrdinates >= 6)    // If >= 2 3D positions
                {
				    FdoPtr<FdoILineString> line1 = gf1->CreateLineString(
                                    FdoDimensionality_XY | FdoDimensionality_Z,
                                    feat->numOrdinates, 
                                    feat->ordinates);
                    theLine = line1;
                    if (NULL != gf2)
                    {
				        FdoPtr<FdoILineString> line2 = (FdoILineString *)gf2->CreateGeometry(line1);
                        theLine = line2;
                        theGf = gf2;
                    }
                    numGeometries++;
                    numOrdinates += feat->numOrdinates;

                    if (streamed) // Use accessors immediately
                    {
				        FdoPtr<FdoByteArray> byteArray = theGf->GetFgf(theLine);

                        FdoInt32 numPositions = theLine->GetCount();
                        if (useAccessorsByValue)
                        {
                            double x, y, z, m;
                            FdoInt32 dim;
                            for (j = 0;  j < numPositions;  j++)
                            {
                                theLine->GetItemByMembers(j, &x, &y, &z, &m, &dim);
                            }
                        }
                        else
                        {
                            for (j = 0;  j < numPositions;  j++)
                            {
                                FdoPtr<FdoIDirectPosition> pos = theLine->GetItem(j);
                            }
                        }
                    }
                    else // Non-streamed -- save up for later access
                    {
                        if (retainAsFgf)
                        {
                            byteArrays_S[numByteArrays_S++] = theGf->GetFgf(theLine);
                        }
                        else // retain as geometry objects
                        {
                            geoms_S[numGeoms_S++] = theLine;
                        }
                    }
                }
            }

            double time2b = getTime_S();
            double elapsedB = time2b - time1b;
            if (VERBOSE)
                printf("%lf sec for %d positions in %d %sLineStrings (%d geom/s)\n",
                    elapsedB, numOrdinates/3, numGeometries,
                    streamed ? "streamed " : "", (FdoInt32)(numGeometries/elapsedB) );

            FdoInt32 numRetained = retainAsFgf ? numByteArrays_S : numGeoms_S;

            if (numRetained > 0)
            {
                 // Use accessors to test time on retained (non-streamed) geometries.
                double time1c = getTime_S();
                FdoILineString * line1c = 0;
    		    FdoByteArray *   byteArrayC = 0;

                for (i=0; i<numRetained; i++)
                {
                    if (retainAsFgf)
                    {
    		            byteArrayC = byteArrays_S[i];
                        line1c = (FdoILineString *) theGf->CreateGeometryFromFgf(byteArrayC);
                    }
                    else
                    {
                        line1c = (FdoILineString *)((geoms_S[i]).p);
    		            byteArrayC = theGf->GetFgf(line1c);
                    }
                    FdoInt32 numPositions = line1c->GetCount();
                    if (useAccessorsByValue)
                    {
                        double x, y, z, m;
                        FdoInt32 dim;
                        for (j = 0;  j < numPositions;  j++)
                        {
                            line1c->GetItemByMembers(j, &x, &y, &z, &m, &dim);
                        }
                    }
                    else
                    {
                        for (j = 0;  j < numPositions;  j++)
                        {
                            FdoPtr<FdoIDirectPosition> pos = line1c->GetItem(j);
                        }
                    }
                    if (retainAsFgf)
                    {
                        line1c->Release();
                        line1c = 0;
                    }
                }

                double time2c = getTime_S();
                double elapsedC = time2c - time1c;
                if (VERBOSE)
                    if (numGeometries > 0)
                    {
                        printf("Accessing: %lf sec for %d positions in %d retained LineStrings (%d geom/s)\n",
                            elapsedC, numOrdinates/3, numGeometries, (FdoInt32)(numGeometries/elapsedC) );
                    }
            }
        }
        catch (...)
        {
            throw;
        }
	}
   	catch (FdoException *e)
	{
		UnitTestUtil::FailOnException( e );
	}
}


static void convertBinaryOrdinatesToPolygons(
                                    FdoFgfGeometryFactory * gf1,
                                    FdoInt32  numFeatures
                                    )
{
    try
    {
        try
        {
            FdoInt32                 i;
            FdoFgfGeometryFactory * theGf = gf1;

            FeatInfo2_t * feat1 = NULL;
            FeatInfo2_t * feat2 = NULL;
            numGeoms_S = 0;
            FdoInt32 numGeometries = 0;
            FdoInt32 numOrdinates = 0;

            double time1b = getTime_S();

            for (i = 0;  i < numFeatures-1;  i += 2)
            {
                // Treat each input feature as a polygon ring.  Assign every
                // two input rings to a new polygon.
                feat1 = getFeatureByIndex(i);
                feat2 = getFeatureByIndex(i+1);
                if (feat1->numOrdinates >= 12 && feat2->numOrdinates >= 12)    // If >= 4 3D positions to make a ring
                {
                    // Ensure closedness of rings.
                    double * ords = feat1->ordinates;
                    FdoInt32 numOrds = feat1->numOrdinates;
                    ords[numOrds-3] = ords[0];
                    ords[numOrds-2] = ords[1];
                    ords[numOrds-1] = ords[2];
                    ords = feat2->ordinates;
                    numOrds = feat2->numOrdinates;
                    ords[numOrds-3] = ords[0];
                    ords[numOrds-2] = ords[1];
                    ords[numOrds-1] = ords[2];

				    FdoPtr<FdoILinearRing> extRing = gf1->CreateLinearRing(
                                    FdoDimensionality_XY | FdoDimensionality_Z,
                                    feat1->numOrdinates, 
                                    feat1->ordinates);
				    FdoPtr<FdoILinearRing> intRing = gf1->CreateLinearRing(
                                    FdoDimensionality_XY | FdoDimensionality_Z,
                                    feat2->numOrdinates, 
                                    feat2->ordinates);
                    FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create();
	                intRings->Add(intRing);
	                FdoPtr<FdoIPolygon> geometry = gf1->CreatePolygon(extRing, intRings);

                    numGeometries++;
                    numOrdinates += feat1->numOrdinates + feat2->numOrdinates;
                    byteArrays_S[numByteArrays_S++] = gf1->GetFgf(geometry);
                }
            }

            double time2b = getTime_S();
            double elapsedB = time2b - time1b;
            if (VERBOSE)
                printf("%lf sec for %d positions in %d polygons (%d geom/s)\n",
                    elapsedB, numOrdinates/3, numGeometries,
                    (FdoInt32)(numGeometries/elapsedB) );

        }
        catch (...)
        {
            throw;
        }
	}
   	catch (FdoException *e)
	{
		UnitTestUtil::FailOnException( e );
	}
}


static void convertFromFgf(
                            FdoFgfGeometryFactory * gf1,
                            FdoFgfGeometryFactory * gf2,
                            FdoInt32  numFeatures,
                            bool useAccessorsByValue,
                            bool streamed
                            )
{
    try
    {
        try
        {
            FdoInt32                 i, j;
            FdoFgfGeometryFactory * theGf = gf1;
            FdoILineString *         theLine = NULL;

            FeatInfo2_t * feat = NULL;
            numGeoms_S = 0;
            FdoInt32 numGeometries = 0;
            FdoInt32 totalNumPositions = 0;

            double time1b = getTime_S();

            for (i = 0;  i < numFeatures;  i++)
            {
                FdoPtr<FdoByteArray> ba = getByteArrayByIndex(i);
                FdoPtr<FdoILineString> line1 = (FdoILineString *) gf1->CreateGeometryFromFgf(ba);
                theLine = line1;
                if (NULL != gf2)
                {
				    FdoPtr<FdoILineString> line2 = (FdoILineString *)gf2->CreateGeometry(line1);
                    theLine = line2;
                    theGf = gf2;
                }
                numGeometries++;

                if (streamed) // Use accessors immediately
                {
				    FdoPtr<FdoByteArray> byteArray = theGf->GetFgf(theLine);

                    FdoInt32 numPositions = theLine->GetCount();
                    totalNumPositions += numPositions;

                    if (useAccessorsByValue)
                    {
                        double x, y, z, m;
                        FdoInt32 dim;
                        for (j = 0;  j < numPositions;  j++)
                        {
                            theLine->GetItemByMembers(j, &x, &y, &z, &m, &dim);
                        }
                    }
                    else
                    {
                        for (j = 0;  j < numPositions;  j++)
                        {
                            FdoPtr<FdoIDirectPosition> pos = theLine->GetItem(j);
                        }
                    }
                }
                else // Non-streamed -- save up for later access
                {
                    geoms_S[numGeoms_S++] = theLine;
                }
            }

            double time2b = getTime_S();
            double elapsedB = time2b - time1b;
            if (VERBOSE)
                printf("%lf sec for %d positions in %d %sgeometries (%d geom/s)\n",
                    elapsedB, totalNumPositions, numGeometries,
                    streamed ? "streamed " : "", (FdoInt32)(numGeometries/elapsedB) );

            totalNumPositions = 0;

            if (numGeoms_S > 0) // Use accessors to test time on retained (non-streamed) geometries.
            {
                double time1c = getTime_S();

                for (i=0; i<numGeoms_S; i++)
                {
                    FdoILineString * line1c = (FdoILineString *)((geoms_S[i]).p);
    		        FdoPtr<FdoByteArray> byteArrayC = theGf->GetFgf(line1c);
                    FdoInt32 numPositions = line1c->GetCount();
                    totalNumPositions += numPositions;
                    if (useAccessorsByValue)
                    {
                        double x, y, z, m;
                        FdoInt32 dim;
                        for (j = 0;  j < numPositions;  j++)
                        {
                            line1c->GetItemByMembers(j, &x, &y, &z, &m, &dim);
                        }
                    }
                    else
                    {
                        for (j = 0;  j < numPositions;  j++)
                        {
                            FdoPtr<FdoIDirectPosition> pos = line1c->GetItem(j);
                        }
                    }
                }

                double time2c = getTime_S();
                double elapsedC = time2c - time1c;
                if (VERBOSE)
                    if (numGeometries > 0)
                    {
                        printf("Accessing: %lf sec for %d positions in %d retained geometries (%d geom/s)\n",
                            elapsedC, totalNumPositions, numGeometries, (FdoInt32)(numGeometries/elapsedC) );
                    }
            }
        }
        catch (...)
        {
            throw;
        }
	}
   	catch (FdoException *e)
	{
		UnitTestUtil::FailOnException( e );
	}
}


static FdoInt32 accessLineString(
                            FdoILineString * geometry,
                            bool useAccessorsByValue)
{
    FdoInt32 numPositions = geometry->GetCount();

    if (useAccessorsByValue)
    {
        double x, y, z, m;
        FdoInt32 dim;
        for (FdoInt32 i = 0;  i < numPositions;  i++)
        {
            geometry->GetItemByMembers(i, &x, &y, &z, &m, &dim);
        }
    }
    else
    {
        for (FdoInt32 i = 0;  i < numPositions;  i++)
        {
            FdoPtr<FdoIDirectPosition> pos = geometry->GetItem(i);
        }
    }
    return numPositions;
}


static FdoInt32 accessLinearRing(
                            FdoILinearRing * ring,
                            bool useAccessorsByValue)
{
    FdoInt32 numPositions = ring->GetCount();

    if (useAccessorsByValue)
    {
        double x, y, z, m;
        FdoInt32 dim;
        for (FdoInt32 i = 0;  i < numPositions;  i++)
        {
            ring->GetItemByMembers(i, &x, &y, &z, &m, &dim);
        }
    }
    else
    {
        for (FdoInt32 i = 0;  i < numPositions;  i++)
        {
            FdoPtr<FdoIDirectPosition> pos = ring->GetItem(i);
        }
    }
    return numPositions;
}


static FdoInt32 accessPolygon(
                            FdoIPolygon * geometry,
                            bool useAccessorsByValue)
{
    FdoInt32 numPositions = 0;
    FdoInt32 numRings = geometry->GetInteriorRingCount();
	FdoPtr<FdoILinearRing> ring = geometry->GetExteriorRing();
    numPositions += accessLinearRing(ring, useAccessorsByValue);

	for (FdoInt32 i=0; i<numRings; i++)
	{
		ring = geometry->GetInteriorRing(i);
        numPositions += accessLinearRing(ring, useAccessorsByValue);
	}
    return numPositions;
}

static void accessFgfStreamed(
                            FdoFgfGeometryFactory * gf1,
                            FdoInt32  numFeatures,
                            bool useAccessorsByValue)
{
    try
    {
        try
        {
            FeatInfo2_t * feat = NULL;
            numGeoms_S = 0;
            FdoInt32 numGeometries = 0;
            FdoInt32 totalNumPositions = 0;

            double time1b = getTime_S();

            for (FdoInt32 i = 0;  i < numFeatures;  i++)
            {
                FdoPtr<FdoByteArray> ba = getByteArrayByIndex(i);
                FdoPtr<FdoIGeometry> geometry = gf1->CreateGeometryFromFgf(ba);
                numGeometries++;
                FdoGeometryType geometryType = geometry->GetDerivedType();
        	    switch ( geometryType )
	            {
                case FdoGeometryType_LineString:
                    {
                        FdoILineString * ls = (FdoILineString *)(geometry.p);
                        totalNumPositions += accessLineString(ls, useAccessorsByValue);
                    }
                    break;
            	case FdoGeometryType_Polygon:
                    {
                        FdoIPolygon * poly = (FdoIPolygon *)(geometry.p);
                        totalNumPositions += accessPolygon(poly, useAccessorsByValue);
                    }
            	default:
                    ;   // No report for unsupported types.
                }
            }

            double time2b = getTime_S();
            double elapsedB = time2b - time1b;
            if (VERBOSE)
                printf("%lf sec for %d positions in %d streamed geometries (%d geom/s)\n",
                    elapsedB, totalNumPositions, numGeometries,
                    (FdoInt32)(numGeometries/elapsedB) );
        }
        catch (...)
        {
            throw;
        }
	}
   	catch (FdoException *e)
	{
		UnitTestUtil::FailOnException( e );
	}
}


void GeometryMemPerfTest::testFgfConversion ()
{
    try
    {
        try
        {
            // There are several variations here, of which only a trivial one is normally compiled.
            // Other variations must be turned on and off by hand with recompiling.
            // Runs of GeometryMemPerfTest must be done in isolation, as the behaviour of the heap
            // is only predictable when re-running the unit_test program from scratch, 
            // without other tests selected.

	        FdoPtr<FdoFgfGeometryFactory> gfUnaligned = FdoFgfGeometryFactory::GetInstance();
	        //FdoPtr<FdoFgfGeometryFactory> gfAligned = FdoFgfAlignedGeometryFactory::GetInstance();

            readTextFile_S();

	        //printf("\nIn-memory conversion to aligned FGF, retained as geometries...\n");
            //convertBinaryOrdinatesToLineStrings(gfAligned, NULL, MAX_NUM_FEATS, true, false, false);

            numGeoms_S = 0;;

	        //printf("\nIn-memory conversion to unaligned FGF, retained as geometries...\n");
            //convertBinaryOrdinatesToLineStrings(gfUnaligned, NULL, MAX_NUM_FEATS, true, false, false);

            numGeoms_S = 0;;

	        //printf("\nIn-memory conversion to unaligned FGF, retained as FGF...\n");
            //convertBinaryOrdinatesToLineStrings(gfUnaligned, NULL, MAX_NUM_FEATS, true, false, true);

            numGeoms_S = 0;;

	        //printf("\nIn-memory conversion to unaligned FGF, streamed...\n");
            //convertBinaryOrdinatesToLineStrings(gfUnaligned, NULL, MAX_NUM_FEATS, true, true, true);

            numGeoms_S = 0;;

	        //printf("\nIn-memory conversion to aligned FGF, retained as FGF...\n");
            //convertBinaryOrdinatesToLineStrings(gfAligned, NULL, MAX_NUM_FEATS, true, false, true);

            numGeoms_S = 0;;

            //printf("\nIn-memory conversion to unaligned FGF and then aligned FGF, retained as geometries...\n");
            //convertBinaryOrdinatesToLineStrings(gfUnaligned, gfAligned, MAX_NUM_FEATS, true, false, false);

	        //printf("\nIn-memory limited conversion to unaligned FGF LineStrings, retained as FGF...\n");
            //convertBinaryOrdinatesToLineStrings(gfUnaligned, NULL, MAX_ACTUAL_NUM_FEATS, true, false, true);

	        printf("\nIn-memory limited conversion to unaligned FGF Polygons, retained as FGF...\n");
            convertBinaryOrdinatesToPolygons(gfUnaligned,MAX_ACTUAL_NUM_FEATS);

            numGeoms_S = 0;;

            //writeByteArraysToFile_S();

            //readByteArraysFromFile_S();

            //printf("\nIn-memory use of retained, unaligned FGF, accessed via streamed geometries...\n");
            //convertFromFgf(gfUnaligned, NULL, numByteArrays_S, true, true);

            //printf("\nIn-memory use of limited retained, unaligned FGF, accessed via streamed geometries...\n");
            //convertFromFgf(gfUnaligned, NULL, MAX_NUM_FEATS, true, true);

            //printf("\nIn-memory use of retained, aligned FGF, accessed via streamed geometries...\n");
            //convertFromFgf(gfAligned, NULL, numByteArrays_S, true, true);

            printf("\nIn-memory use of limited retained, unaligned FGF, accessed via streamed geometries...\n");
            accessFgfStreamed(gfUnaligned, MAX_NUM_FEATS, true);
        }
        catch (...)
        {
            throw;
        }
	}
   	catch (FdoException *e)
	{
		UnitTestUtil::FailOnException( e );
	}
}

