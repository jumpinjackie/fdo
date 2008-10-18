/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */

#include "Pch.h"
#include "IndexTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (IndexTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (IndexTests, "IndexTests");

#include <ShpSpatialIndex.h>
#include <ShapeFile.h>
#include <ShapeIndex.h>

#include <limits.h>
#undef min
#undef max
#define LOW_MASK 0x7fff

#ifdef _WIN32
#define SHP_FILE L"..\\..\\TestData\\Ontario\\roads.shp"
#define SHX_FILE L"..\\..\\TestData\\Ontario\\roads.shx"
#define DBF_FILE L"..\\..\\TestData\\Ontario\\roads.dbf"
#define IDX_FILE L"..\\..\\TestData\\Ontario\\roads.idx"
#define SHP_FILE2 L"..\\..\\TestData\\Nanboku\\Nanboku_3dLine.shp"
#define SHX_FILE2 L"..\\..\\TestData\\Nanboku\\Nanboku_3dLine.shx"
#define DBF_FILE2 L"..\\..\\TestData\\Nanboku\\Nanboku_3dLine.dbf"
#define IDX_FILE2 L"..\\..\\TestData\\Nanboku\\Nanboku_3dLine.idx"
#define IDX_FILE3 L"..\\..\\TestData\\DeleteMe.idx"
#else
#define SHP_FILE L"../../TestData/Ontario/roads.shp"
#define SHX_FILE L"../../TestData/Ontario/roads.shx"
#define DBF_FILE L"../../TestData/Ontario/roads.dbf"
#define IDX_FILE L"../../TestData/Ontario/roads.idx"
#define SHP_FILE2 L"../../TestData/Nanboku/Nanboku_3dLine.shp"
#define SHX_FILE2 L"../../TestData/Nanboku/Nanboku_3dLine.shx"
#define DBF_FILE2 L"../../TestData/Nanboku/Nanboku_3dLine.dbf"
#define IDX_FILE2 L"../../TestData/Nanboku/Nanboku_3dLine.idx"
#define IDX_FILE3 L"../../TestData/DeleteMe.idx"
#endif

IndexTests::IndexTests (void)
{
}

IndexTests::~IndexTests (void)
{
}

void IndexTests::create ()
{
	if (FdoCommonFile::FileExists (IDX_FILE))
        CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE, true));

    // close the file when exiting this block
    {
        ShpSpatialIndex index ((const wchar_t*)IDX_FILE, NULL, eNullShape, false);
    }

    CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE, true));
}

void IndexTests::populate ()
{
    unsigned long size;
    ULONG offset;
    int length;
    eShapeTypes type;
    Shape* shape;
    BoundingBoxEx box;
    BoundingBoxEx extents;

    // close the file when exiting this block
    {
        ShapeFile shp (SHP_FILE);
        ShapeIndex shx (SHX_FILE);
        ShpSpatialIndex index (IDX_FILE, NULL, shp.GetFileShapeType (), shp.HasMData ());
        shx.GetFileSize (size);

	    // general information methods
	    CPPUNIT_ASSERT_MESSAGE ("file code bad", 9994 == shx.GetFileCode ());
	    CPPUNIT_ASSERT_MESSAGE ("length bad", size == 2 * shx.GetFileLength ()); // 16 bit words
	    CPPUNIT_ASSERT_MESSAGE ("version bad", 1000 == shx.GetFileVersion ());
	    CPPUNIT_ASSERT_MESSAGE ("shape type bad", ePolylineShape == shx.GetFileShapeType ());
	    CPPUNIT_ASSERT_MESSAGE ("number of objects bad", 1088 == shx.GetNumObjects ());

        // bounding box methods
	    CPPUNIT_ASSERT_MESSAGE ("min x bad", 202293.67187500000 == shx.GetBoundingBoxMinX ());
	    CPPUNIT_ASSERT_MESSAGE ("max x bad", 1763872.6250000000 == shx.GetBoundingBoxMaxX ());
	    CPPUNIT_ASSERT_MESSAGE ("min y bad", 11709796.000000000 == shx.GetBoundingBoxMinY ());
	    CPPUNIT_ASSERT_MESSAGE ("max y bad", 12897880.000000000 == shx.GetBoundingBoxMaxY ());
	    CPPUNIT_ASSERT_MESSAGE ("min z bad", 0.0 == shx.GetBoundingBoxMinZ ());
	    CPPUNIT_ASSERT_MESSAGE ("max z bad", 0.0 == shx.GetBoundingBoxMaxZ ());
	    CPPUNIT_ASSERT_MESSAGE ("min m bad", 0.0 == shx.GetBoundingBoxMinM ());
	    CPPUNIT_ASSERT_MESSAGE ("max m bad", 0.0 == shx.GetBoundingBoxMaxM ());

        BoundingBox limit;
        limit.xMin = numeric_limits<double>::max ();
        limit.xMax = -numeric_limits<double>::max ();
        limit.yMin = numeric_limits<double>::max ();
        limit.yMax = -numeric_limits<double>::max ();
        for (int i = 0; i < shx.GetNumObjects (); i++)
        {
            shx.GetObjectAt (i, offset, length);
            shape = shp.GetObjectAt (offset, type);
            shape->GetBoundingBoxEx (box);
            if (limit.xMin > box.xMin)
                limit.xMin = box.xMin;
            if (limit.xMax < box.xMax)
                limit.xMax = box.xMax;
            if (limit.yMin > box.yMin)
                limit.yMin = box.yMin;
            if (limit.yMax < box.yMax)
                limit.yMax = box.yMax;
            index.InsertObject (type, &box, offset);
            delete shape;
        }

        CPPUNIT_ASSERT_MESSAGE ("wrong count of index entries", shx.GetNumObjects () == index.GetNObjects ());
        index.GetSSIExtent (extents);
	    CPPUNIT_ASSERT_MESSAGE ("min x bad", extents.xMin == shx.GetBoundingBoxMinX ());
	    CPPUNIT_ASSERT_MESSAGE ("max x bad", extents.xMax == shx.GetBoundingBoxMaxX ());
	    CPPUNIT_ASSERT_MESSAGE ("min y bad", extents.yMin == shx.GetBoundingBoxMinY ());
	    CPPUNIT_ASSERT_MESSAGE ("max y bad", extents.yMax == shx.GetBoundingBoxMaxY ());
	    CPPUNIT_ASSERT_MESSAGE ("min x bad", extents.xMin == limit.xMin);
	    CPPUNIT_ASSERT_MESSAGE ("max x bad", extents.xMax == limit.xMax);
	    CPPUNIT_ASSERT_MESSAGE ("min y bad", extents.yMin == limit.yMin);
	    CPPUNIT_ASSERT_MESSAGE ("max y bad", extents.yMax == limit.yMax);
    }

    CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE, true));
}

void IndexTests::query ()
{
    int istatus;
    ULONG offset;
    int length;
    eShapeTypes type;
    Shape* shape;
    BoundingBoxEx box;
    BoundingBoxEx area;
    bool done;
    unsigned long another_offset;
    BoundingBoxEx extents;
    bool found;
    int  n;

    // close the file when exiting this block
    {
        ShapeFile shp (SHP_FILE);
        ShapeIndex shx (SHX_FILE);
        ShpSpatialIndex index (IDX_FILE, NULL, shp.GetFileShapeType (), shp.HasMData ());

        for (int i = 0; i < shx.GetNumObjects (); i++)
        {
            shx.GetObjectAt (i, offset, length);
            shape = shp.GetObjectAt (offset, type);
            shape->GetBoundingBoxEx (box);
            index.InsertObject (type, &box, offset);
            delete shape;
        }

        area.xMin = 250893.43750000000;
        area.xMax = 252774.15625000000;
        area.yMin = 12616630.000000000;
        area.yMax = 12617222.000000000;
        index.InitializeSearch (&area, type);
        done = false;
        found = false;
        n = 0;
        do
        {
            istatus = index.GetNextObject (another_offset, extents);
            switch (istatus)
            {
                case SHP_SI_SEARCH_COMPLETE:
                    done = true;
                    break;
                case SHP_OK:
                    found = true;
                    n++;
                    // check extents of the returned object lie within queried area
                    CPPUNIT_ASSERT_MESSAGE ("maxx less than minx of query", area.xMax >= extents.xMin);
                    CPPUNIT_ASSERT_MESSAGE ("minx greater than maxx of query", area.xMin <= extents.xMax);
                    CPPUNIT_ASSERT_MESSAGE ("maxy less than miny of query", area.yMax >= extents.yMin);
                    CPPUNIT_ASSERT_MESSAGE ("miny greater than maxy of query", area.yMin <= extents.yMax);
                    break;
                default:
                    CPPUNIT_FAIL ("unexpected status from GetNextObject");
                    break;
            }
        }
        while (!done);
        CPPUNIT_ASSERT_MESSAGE ("failed to find an object", found);
        CPPUNIT_ASSERT_MESSAGE ("failed to retrieve the correct number of objects", n == 4);
    }

    CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE, true));
}

void IndexTests::check_z ()
{
    unsigned long size;
    ULONG offset;
    int length;
    eShapeTypes type;
    Shape* shape;
    BoundingBoxEx box;
    BoundingBoxEx extents;

    try
    {
        if (FdoCommonFile::FileExists (IDX_FILE2))
            CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE2, true));

        // close the file when exiting this block
        {
            ShapeFile shp (SHP_FILE2);
            ShapeIndex shx (SHX_FILE2);
            ShpSpatialIndex index (IDX_FILE2, NULL, shp.GetFileShapeType (), shp.HasMData ());
            shx.GetFileSize (size);

	        // general information methods
	        CPPUNIT_ASSERT_MESSAGE ("file code bad", 9994 == shx.GetFileCode ());
	        CPPUNIT_ASSERT_MESSAGE ("length bad", size == 2 * shx.GetFileLength ()); // 16 bit words
	        CPPUNIT_ASSERT_MESSAGE ("version bad", 1000 == shx.GetFileVersion ());
	        CPPUNIT_ASSERT_MESSAGE ("shape type bad", ePolylineZShape == shx.GetFileShapeType ());
	        CPPUNIT_ASSERT_MESSAGE ("number of objects bad", 1303 == shx.GetNumObjects ());

            // bounding box methods
	        CPPUNIT_ASSERT_MESSAGE ("min x bad", -10383.223000000000 == shx.GetBoundingBoxMinX ());
	        CPPUNIT_ASSERT_MESSAGE ("max x bad", -6482.8280000000004 == shx.GetBoundingBoxMaxX ());
	        CPPUNIT_ASSERT_MESSAGE ("min y bad", -41236.980000000003 == shx.GetBoundingBoxMinY ());
	        CPPUNIT_ASSERT_MESSAGE ("max y bad", -24352.287000000000 == shx.GetBoundingBoxMaxY ());
	        CPPUNIT_ASSERT_MESSAGE ("min z bad", -38.270000000000003 == shx.GetBoundingBoxMinZ ());
	        CPPUNIT_ASSERT_MESSAGE ("max z bad", 8.2400000000000002 == shx.GetBoundingBoxMaxZ ());
	        CPPUNIT_ASSERT_MESSAGE ("min m bad", shx.GetBoundingBoxMinM () < -1e-38);
	        CPPUNIT_ASSERT_MESSAGE ("max m bad", shx.GetBoundingBoxMaxM () < -1e-38);

            BoundingBoxEx limit;
            limit.xMin = numeric_limits<double>::max ();
            limit.xMax = -numeric_limits<double>::max ();
            limit.yMin = numeric_limits<double>::max ();
            limit.yMax = -numeric_limits<double>::max ();
            limit.zMin = numeric_limits<double>::max ();
            limit.zMax = -numeric_limits<double>::max ();
            for (int i = 0; i < shx.GetNumObjects (); i++)
            {
                shx.GetObjectAt (i, offset, length);
                shape = shp.GetObjectAt (offset, type);
                shape->GetBoundingBoxEx (box);
                if (limit.xMin > box.xMin)
                    limit.xMin = box.xMin;
                if (limit.xMax < box.xMax)
                    limit.xMax = box.xMax;
                if (limit.yMin > box.yMin)
                    limit.yMin = box.yMin;
                if (limit.yMax < box.yMax)
                    limit.yMax = box.yMax;
                if (limit.zMin > box.zMin)
                    limit.zMin = box.zMin;
                if (limit.zMax < box.zMax)
                    limit.zMax = box.zMax;
                index.InsertObject (type, &box, offset);
                delete shape;
            }

            CPPUNIT_ASSERT_MESSAGE ("wrong count of index entries", shx.GetNumObjects () == index.GetNObjects ());
            index.GetSSIExtent (extents);
	        CPPUNIT_ASSERT_MESSAGE ("min x bad", extents.xMin == shx.GetBoundingBoxMinX ());
	        CPPUNIT_ASSERT_MESSAGE ("max x bad", extents.xMax == shx.GetBoundingBoxMaxX ());
	        CPPUNIT_ASSERT_MESSAGE ("min y bad", extents.yMin == shx.GetBoundingBoxMinY ());
	        CPPUNIT_ASSERT_MESSAGE ("max y bad", extents.yMax == shx.GetBoundingBoxMaxY ());
	        CPPUNIT_ASSERT_MESSAGE ("min z bad", extents.zMin == shx.GetBoundingBoxMinZ ());
	        CPPUNIT_ASSERT_MESSAGE ("max z bad", extents.zMax == shx.GetBoundingBoxMaxZ ());
	        CPPUNIT_ASSERT_MESSAGE ("min x bad", extents.xMin == limit.xMin);
	        CPPUNIT_ASSERT_MESSAGE ("max x bad", extents.xMax == limit.xMax);
	        CPPUNIT_ASSERT_MESSAGE ("min y bad", extents.yMin == limit.yMin);
	        CPPUNIT_ASSERT_MESSAGE ("max y bad", extents.yMax == limit.yMax);
	        CPPUNIT_ASSERT_MESSAGE ("min z bad", extents.zMin == limit.zMin);
	        CPPUNIT_ASSERT_MESSAGE ("max z bad", extents.zMax == limit.zMax);
        }

        CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE2, true));
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void IndexTests::check_limits ()
{
#define NUMFEATURES 10
    BoundingBoxEx** features;
    DoublePoint p;
    double z[2];
    double m[2];

    try
    {
        if (FdoCommonFile::FileExists (IDX_FILE3))
            CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3, true));

        // close the file when exiting this block
        {
            ShpSpatialIndex index (IDX_FILE3, NULL, ePolygonMShape, true);
            features = new BoundingBoxEx*[NUMFEATURES];
            srand ((unsigned)time (NULL));
            for (int i = 0; i < NUMFEATURES; i++)
            {
                p.x = (double)(LOW_MASK & rand ()) / 256.0;
                p.y = (double)(LOW_MASK & rand ()) / 256.0;
                z[0] = (double)(LOW_MASK & rand ()) / 256.0;
                z[1] = z[0];
                m[0] = (double)(LOW_MASK & rand ()) / 256.0;
                m[1] = m[0];
                features[i] = new BoundingBoxEx (&p, z, m);
                features[i]->xMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->yMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->zMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->mMax = (double)(LOW_MASK & rand ()) / 256.0;
                index.InsertObject (ePolygonMShape, features[i], i);
            }
        }
        // close and reopen
        {
            ShpSpatialIndex index (IDX_FILE3, NULL, ePolygonMShape, true);

            // compute the real limits
            BoundingBoxEx limit;
            limit.xMin = numeric_limits<double>::max ();
            limit.xMax = -numeric_limits<double>::max ();
            limit.yMin = numeric_limits<double>::max ();
            limit.yMax = -numeric_limits<double>::max ();
            limit.zMin = numeric_limits<double>::max ();
            limit.zMax = -numeric_limits<double>::max ();
            limit.mMin = numeric_limits<double>::max ();
            limit.mMax = -numeric_limits<double>::max ();
            for (int i = 0; i < NUMFEATURES; i++)
            {
                if (limit.xMin > features[i]->xMin)
                    limit.xMin = features[i]->xMin;
                if (limit.xMax < features[i]->xMax)
                    limit.xMax = features[i]->xMax;
                if (limit.yMin > features[i]->yMin)
                    limit.yMin = features[i]->yMin;
                if (limit.yMax < features[i]->yMax)
                    limit.yMax = features[i]->yMax;
                if (limit.zMin > features[i]->zMin)
                    limit.zMin = features[i]->zMin;
                if (limit.zMax < features[i]->zMax)
                    limit.zMax = features[i]->zMax;
                if (limit.mMin > features[i]->mMin)
                    limit.mMin = features[i]->mMin;
                if (limit.mMax < features[i]->mMax)
                    limit.mMax = features[i]->mMax;
            }
            // check
            BoundingBoxEx test;
            index.GetSSIExtent (test);

            for (int i = 0; i < NUMFEATURES; i++)
                delete features[i];
            delete[] features;

            CPPUNIT_ASSERT_MESSAGE ("limits not correct", test == limit);
        }

        CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3, true));
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void IndexTests::check_remove ()
{
#define NUMFEATURES 10
    BoundingBoxEx** features;
    DoublePoint p;
    double z[2];
    double m[2];

    try
    {
        if (FdoCommonFile::FileExists (IDX_FILE3))
            CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3, true));

        // close the file when exiting this block
        {
            ShpSpatialIndex index (IDX_FILE3, NULL, ePolygonMShape, true);
            features = new BoundingBoxEx*[NUMFEATURES];
            srand ((unsigned)time (NULL));
            for (int i = 0; i < NUMFEATURES; i++)
            {
                p.x = (double)(LOW_MASK & rand ()) / 256.0;
                p.y = (double)(LOW_MASK & rand ()) / 256.0;
                z[0] = (double)(LOW_MASK & rand ()) / 256.0;
                z[1] = z[0];
                m[0] = (double)(LOW_MASK & rand ()) / 256.0;
                m[1] = m[0];
                features[i] = new BoundingBoxEx (&p, z, m);
                features[i]->xMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->yMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->zMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->mMax = (double)(LOW_MASK & rand ()) / 256.0;
                index.InsertObject (ePolygonMShape, features[i], i);
            }
            // remove half of them
            for (int i = 0; i < NUMFEATURES; i += 2)
                index.DeleteObject (features[i], i);

        }
        // close and reopen
        {
            ShpSpatialIndex index (IDX_FILE3, NULL, ePolygonMShape, true);

            // compute the real limits
            BoundingBoxEx limit;
            limit.xMin = numeric_limits<double>::max ();
            limit.xMax = -numeric_limits<double>::max ();
            limit.yMin = numeric_limits<double>::max ();
            limit.yMax = -numeric_limits<double>::max ();
            limit.zMin = numeric_limits<double>::max ();
            limit.zMax = -numeric_limits<double>::max ();
            limit.mMin = numeric_limits<double>::max ();
            limit.mMax = -numeric_limits<double>::max ();
            for (int i = 1; i < NUMFEATURES; i += 2) // note the +1 starting offset
            {
                if (limit.xMin > features[i]->xMin)
                    limit.xMin = features[i]->xMin;
                if (limit.xMax < features[i]->xMax)
                    limit.xMax = features[i]->xMax;
                if (limit.yMin > features[i]->yMin)
                    limit.yMin = features[i]->yMin;
                if (limit.yMax < features[i]->yMax)
                    limit.yMax = features[i]->yMax;
                if (limit.zMin > features[i]->zMin)
                    limit.zMin = features[i]->zMin;
                if (limit.zMax < features[i]->zMax)
                    limit.zMax = features[i]->zMax;
                if (limit.mMin > features[i]->mMin)
                    limit.mMin = features[i]->mMin;
                if (limit.mMax < features[i]->mMax)
                    limit.mMax = features[i]->mMax;
            }
            // check
            BoundingBoxEx test;
            index.GetSSIExtent (test);

            for (int i = 0; i < NUMFEATURES; i++)
                delete features[i];
            delete[] features;

            CPPUNIT_ASSERT_MESSAGE ("limits not correct", test == limit);
        }

        CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3));
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void IndexTests::check_defragment ()
{
#define BIGNUMFEATURES 100
    BoundingBoxEx** features;
    DoublePoint p;
    double z[2];
    double m[2];

    try
    {
        if (FdoCommonFile::FileExists (IDX_FILE3))
            CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3, true));

        // close the file when exiting this block
        {
            ShpSpatialIndex index (IDX_FILE3, NULL, ePolygonMShape, true);
            features = new BoundingBoxEx*[BIGNUMFEATURES];
            srand ((unsigned)time (NULL));
            for (int i = 0; i < BIGNUMFEATURES; i++)
            {
                p.x = (double)(LOW_MASK & rand ()) / 256.0;
                p.y = (double)(LOW_MASK & rand ()) / 256.0;
                z[0] = (double)(LOW_MASK & rand ()) / 256.0;
                z[1] = z[0];
                m[0] = (double)(LOW_MASK & rand ()) / 256.0;
                m[1] = m[0];
                features[i] = new BoundingBoxEx (&p, z, m);
                features[i]->xMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->yMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->zMax = (double)(LOW_MASK & rand ()) / 256.0;
                features[i]->mMax = (double)(LOW_MASK & rand ()) / 256.0;
                index.InsertObject (ePolygonMShape, features[i], i);
            }
            // remove all but one of them
            for (int i = 1; i < BIGNUMFEATURES; i++) // note the +1 starting offset
                index.DeleteObject (features[i], i);

            CPPUNIT_ASSERT_MESSAGE ("defragment failed", SHP_OK == index.Defragment (NULL, NULL));

            // compute the real limits
            BoundingBoxEx limit;
            limit.xMin = numeric_limits<double>::max ();
            limit.xMax = -numeric_limits<double>::max ();
            limit.yMin = numeric_limits<double>::max ();
            limit.yMax = -numeric_limits<double>::max ();
            limit.zMin = numeric_limits<double>::max ();
            limit.zMax = -numeric_limits<double>::max ();
            limit.mMin = numeric_limits<double>::max ();
            limit.mMax = -numeric_limits<double>::max ();
            for (int i = 0; i < 1; i++)
            {
                if (limit.xMin > features[i]->xMin)
                    limit.xMin = features[i]->xMin;
                if (limit.xMax < features[i]->xMax)
                    limit.xMax = features[i]->xMax;
                if (limit.yMin > features[i]->yMin)
                    limit.yMin = features[i]->yMin;
                if (limit.yMax < features[i]->yMax)
                    limit.yMax = features[i]->yMax;
                if (limit.zMin > features[i]->zMin)
                    limit.zMin = features[i]->zMin;
                if (limit.zMax < features[i]->zMax)
                    limit.zMax = features[i]->zMax;
                if (limit.mMin > features[i]->mMin)
                    limit.mMin = features[i]->mMin;
                if (limit.mMax < features[i]->mMax)
                    limit.mMax = features[i]->mMax;
            }
            // check
            BoundingBoxEx test;
            index.GetSSIExtent (test);

            for (int i = 0; i < BIGNUMFEATURES; i++)
                delete features[i];
            delete[] features;

            CPPUNIT_ASSERT_MESSAGE ("limits not correct", test == limit);
        }

        CPPUNIT_ASSERT_MESSAGE ("cannot delete file", FdoCommonFile::Delete (IDX_FILE3, true));
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

