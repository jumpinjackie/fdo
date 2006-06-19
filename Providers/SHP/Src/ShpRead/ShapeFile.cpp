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

#include "stdafx.h"
#include "DoublePoint.h"
#include "BoundingBox.h"
#include "ShapeFile.h"
#include "Constants.h"
#include <limits>
using namespace std;
#undef min
#undef max

#define SHP_PRINT_STATS             false       
#define SHP_SHAPE_BUFFER_MIN_SIZE   5 * 1024    // 5KB
#define SHP_USE_REALLOC             true

/*****************************************************************************
 * Name         : ShapeFile
 *
 * Description  : Constructor
 *
 * Parameters   : const WCHAR* wszFilename
 *                int&         nStatus
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ShapeFile::ShapeFile(const WCHAR* wszFilename)
{
    ATLTRACE(L"Creating ShapeFile object\n");
    OpenRead (wszFilename);

    m_szRowBuffer = NULL;
    m_nRowBufferSize = 0;
    ClearRowShapeCache();

    m_nCacheHits = 0;
    m_nCacheMisses = 0;
}

ShapeFile::ShapeFile(const WCHAR* wszFilename, eShapeTypes shape_type, bool has_m)
{
    ATLTRACE(L"Creating new ShapeFile object\n");
    OpenWrite (wszFilename, shape_type, has_m);

    m_szRowBuffer = NULL;
    m_nRowBufferSize = 0;
    ClearRowShapeCache();

	CloseFile();
}

/*****************************************************************************
 * Name         : ~ShapeFile
 *
 * Description  : Destructor
 *
 * Parameters   :
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ShapeFile::~ShapeFile()
{
    ATLTRACE(L"Deleting ShapeFile object\n");

    if ( m_szRowBuffer )
    {
        if ( SHP_USE_REALLOC && m_szRowBuffer)
            free(m_szRowBuffer);
        else
            delete[] m_szRowBuffer;
    }

    if ( SHP_PRINT_STATS )
    {   
        double  total = m_nCacheHits + m_nCacheMisses;
        printf(" [Records requests %d: hits %d (%d%%) misses %d (%d%%)]\n", 
                    (int)total, m_nCacheHits, (int)(m_nCacheHits* 100/total), m_nCacheMisses, (int)(m_nCacheMisses*100/total));
    }
}

/*****************************************************************************
 * Name         : ReadRecordHeader
 *
 * Description  : This method reads the record header.
 *
 * Parameters   : ULONG  ulOffset
 *                ULONG& ulNextObjectOffset
 *                int&   nRecordNumber
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
int ShapeFile::ReadRecordHeader (ULONG ulOffset, ULONG& ulNextObjectOffset, int& nRecordNumber)
{
    ATLTRACE(L"ReadRecordHeader()\n");
    SHPRecordHeader shpRecordHeader;
    long read;
    int ret;

    if (!SetFilePointer64 ((FdoInt64)ulOffset))
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRecordHeader(SetFilePointer64)");
    if (ReadFile (&shpRecordHeader, sizeof(SHPRecordHeader), &read))
    {
        if (sizeof(SHPRecordHeader) != read)
        {
            ulNextObjectOffset = 0;
            nRecordNumber = 0;
            ret = SHP_EOF;
        }
        else
        {
            // These are stored in BigEndian so they must be swapped
            shpRecordHeader.nRecordNumber = SWAPLONG(shpRecordHeader.nRecordNumber);
            shpRecordHeader.nContentLength = SWAPLONG(shpRecordHeader.nContentLength);

            // Set content length
            ulNextObjectOffset = (shpRecordHeader.nContentLength + 4) * WORD_SIZE_IN_BYTES;

            // Set Record #
            nRecordNumber = shpRecordHeader.nRecordNumber;
            if (1 > nRecordNumber)
                throw FdoException::Create (NlsMsgGet(SHP_INVALID_RECORD_NUMBER_ERROR, "Invalid record number %1$ld for file '%2$ls'.", nRecordNumber, FileName ()));
            ret = SHP_OK;
        }
    }
    else
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRecordHeader(ReadShape)");

    return (ret);
}

/*****************************************************************************
 * Name         : GetObjectAt
 *
 * Description  : This method gets the object at the specified offset.
 *
 * Parameters   : int      nOffset
 *                int&     nShapeType
 *                Shape**  ppShape
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
Shape* ShapeFile::GetObjectAt (ULONG nOffset, eShapeTypes& nShapeType)
{
    ATLTRACE(L">>>>>>>>>> GetObjectAt() Offset:%d <<<<<<<<<<\n",nOffset);

    // Read the Record Header and details
    int nRecordNumber;
    Shape* ret;
    int nStatus = SHP_OK;

    // Row cached?
    BYTE*  p = GetRowShapeFromCache( nOffset, nRecordNumber );

    // If not, then refresh the read buffer
    if ( p == NULL )
    {
        (void) ReadRawDataBlock (nOffset);
        p = GetRowShapeFromCache( nOffset, nRecordNumber );           
    }

    // peek at the shape type to decide what type of shape to construct
    nShapeType = (eShapeTypes)(*((int*)p));

    // overlay the appropriate shape object
    switch (nShapeType)
    {
        case eNullShape:
            ret = new NullShape (nRecordNumber, p, true);
            break;
        case ePointShape:
            ret = new PointShape (nRecordNumber, p, true);
            break;
        case ePolylineShape:
            ret = new PolylineShape (nRecordNumber, p, true);
            break;
        case ePolygonShape:
            ret = new PolygonShape (nRecordNumber, p, true);
            break;
        case eMultiPointShape:
            ret = new MultiPointShape (nRecordNumber, p, true);
            break;
        case ePointZShape:
            ret = new PointZShape (nRecordNumber, p, true, NULL, HasMData ());
            break;
        case ePolylineZShape:
            ret = new PolylineZShape (nRecordNumber, p, true, -1, -1, NULL, HasMData ());
            break;
        case ePolygonZShape:
            ret = new PolygonZShape (nRecordNumber, p, true,  -1, -1, NULL, HasMData ());
            break;
        case eMultiPointZShape:
            ret = new MultiPointZShape (nRecordNumber, p, true, -1, NULL, HasMData ());
            break;
        case ePointMShape:
            ret = new PointMShape (nRecordNumber, p, true);
            break;
        case ePolylineMShape:
            ret = new PolylineMShape (nRecordNumber, p, true);
            break;
        case ePolygonMShape:
            ret = new PolygonMShape (nRecordNumber, p, true);
            break;
        case eMultiPointMShape:
            ret = new MultiPointMShape (nRecordNumber, p, true);
            break;
        case eMultiPatchShape:
            ret = new MultiPatchShape (nRecordNumber, p, true, -1, -1, NULL, HasMData ());
            break;
        default:
            throw FdoException::Create (NlsMsgGet(SHP_UNKNOWN_SHAPE_TYPE, "The shape type number '%1$d' is unknown.", nShapeType));
            break;
    }

    return (ret);
}

void ShapeFile::WriteRecordHeader (int number, int length)
{
    ATLTRACE(L"WriteRecordHeader()\n");
    SHPRecordHeader header;

    header.nRecordNumber = SWAPLONG(number);
    header.nContentLength = SWAPLONG(length);
    if (!WriteFile (&header, sizeof (SHPRecordHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::WriteRecordHeader");

    ClearRowShapeCache();
}

void ShapeFile::PutShape (Shape* shape)
{
    ATLTRACE(L"PutShape()\n");
    if (!WriteFile (shape->GetContent (), shape->GetContentLength () * WORD_SIZE_IN_BYTES))
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::PutShape");
}

Shape* ShapeFile::ShapeFromPoint (FdoIPoint* point, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    bool has_z;
    bool has_m;
    DoublePoint* points;
    double bogus;
    Shape* ret;

    dimensions = point->GetDimensionality ();
    has_z = (0 != (dimensions & FdoDimensionality_Z));
    has_m = (0 != (dimensions & FdoDimensionality_M));
    if (has_z)
    {
        ret = PointZShape::NewPointZShape (record, box, has_m);
        points = ret->GetPoints ();
        if (has_m)
            // has Z and M
            point->GetPositionByMembers (
                &points->x,
                &points->y,
                ((PointZShape*)ret)->GetZData ()->GetArray (),
                ((PointZShape*)ret)->GetMData ()->GetArray (),
                &dimensions);
        else
            // has Z, no M
            point->GetPositionByMembers (
                &points->x,
                &points->y,
                ((PointZShape*)ret)->GetZData ()->GetArray (),
                &bogus,
                &dimensions);
    }
    else if (has_m)
    {   // has M, no Z
        ret = PointMShape::NewPointMShape (record, box);
        points = ret->GetPoints ();
        point->GetPositionByMembers (
            &points->x,
            &points->y,
            &bogus,
            ((PointMShape*)ret)->GetMData ()->GetArray (),
            &dimensions);
    }
    else
    {   // has no Z or M
        ret = PointShape::NewPointShape (record, box);
        points = ret->GetPoints ();
        point->GetPositionByMembers (
            &(points->x),
            &(points->y),
            &bogus,
            &bogus,
            &dimensions);
    }

    return (ret);
}

Shape* ShapeFile::ShapeFromMultiPoint (FdoIMultiPoint* point, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    bool has_z;
    bool has_m;
    int count;
    DoublePoint* points;
    double* zs;
    double* ms;
    double bogus;
    FdoPtr<FdoIPoint> p;
    double minm;
    double maxm;
    Shape* ret;

    dimensions = point->GetDimensionality ();
    has_z = (0 != (dimensions & FdoDimensionality_Z));
    has_m = (0 != (dimensions & FdoDimensionality_M));
    count = point->GetCount();
    if (has_z)
    {
        ret = MultiPointZShape::NewMultiPointZShape (record, count, box, has_m);
        points = ret->GetPoints ();
        zs = ((MultiPointZShape*)ret)->GetZData ()->GetArray ();
        if (has_m)
        {
            // has Z and M
            ms = ((MultiPointZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < count; i++)
            {
                p = point->GetItem (i);
                p->GetPositionByMembers (
                    &points->x,
                    &points->y,
                    zs,
                    ms,
                    &dimensions);
                points++;
                zs++;
                ms++;
            }
            // update M limits in bounding box
            minm = numeric_limits<double>::max ();
            maxm = -numeric_limits<double>::max ();
            ms = ((MultiPointZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < count; i++)
            {
                if (ms[i] < minm)
                    minm = ms[i];
                if (ms[i] > maxm)
                    maxm = ms[i];
            }
            ((MultiPointZShape*)ret)->GetMData ()->SetRangeMin (minm);
            ((MultiPointZShape*)ret)->GetMData ()->SetRangeMax (maxm);
        }
        else
            // has Z, no M
            for (int i = 0; i < count; i++)
            {
                p = point->GetItem (i);
                p->GetPositionByMembers (
                    &points->x,
                    &points->y,
                    zs,
                    &bogus,
                    &dimensions);
                points++;
                zs++;
            }
    }
    else if (has_m)
    {   // has M, no Z
        ret = MultiPointMShape::NewMultiPointMShape (record, count, box, true);
        points = ret->GetPoints ();
        ms = ((MultiPointMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < count; i++)
        {
            p = point->GetItem (i);
            p->GetPositionByMembers (
                &points->x,
                &points->y,
                &bogus,
                ms,
                &dimensions);
            points++;
            ms++;
        }
        // update M limits in bounding box
        minm = numeric_limits<double>::max ();
        maxm = -numeric_limits<double>::max ();
        ms = ((MultiPointMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < count; i++)
        {
            if (ms[i] < minm)
                minm = ms[i];
            if (ms[i] > maxm)
                maxm = ms[i];
        }
        ((MultiPointMShape*)ret)->GetMData ()->SetRangeMin (minm);
        ((MultiPointMShape*)ret)->GetMData ()->SetRangeMax (maxm);
    }
    else
    {   // has no Z or M
        ret = MultiPointShape::NewMultiPointShape (record, count, box);
        points = ret->GetPoints ();
        for (int i = 0; i < count; i++)
        {
            p = point->GetItem (i);
            p->GetPositionByMembers (
                &(points->x),
                &(points->y),
                &bogus,
                &bogus,
                &dimensions);
            points++;
        }
    }

    return (ret);
}

Shape* ShapeFile::ShapeFromLine (FdoILineString* line, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    int count;
    DoublePoint* points;
    double* ms;
    double* zs;
    double bogus;
    bool has_m;
    double minm;
    double maxm;
    PolyShape* ret;

    dimensions = line->GetDimensionality ();
    count = line->GetCount ();
    has_m = (0 != (dimensions & FdoDimensionality_M));
    if (0 != (dimensions & FdoDimensionality_Z))
    {
        ret = PolylineZShape::NewPolylineZShape (record, 1, count, box, has_m);
        points = ret->GetPoints ();
        zs = ((PolylineZShape*)ret)->GetZData ()->GetArray ();
        if (has_m)
        {
            // has Z and M
            ms = ((PolylineZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < count; i++)
                line->GetItemByMembers (
                    i,
                    &(points[i].x),
                    &(points[i].y),
                    &zs[i],
                    &ms[i],
                    &dimensions);
            // update M limits in bounding box
            minm = numeric_limits<double>::max ();
            maxm = -numeric_limits<double>::max ();
            for (int i = 0; i < count; i++)
            {
                if (ms[i] < minm)
                    minm = ms[i];
                if (ms[i] > maxm)
                    maxm = ms[i];
            }
            ((PolylineMShape*)ret)->GetMData ()->SetRangeMin (minm);
            ((PolylineMShape*)ret)->GetMData ()->SetRangeMax (maxm);
        }
        else
        {   // has Z, no M
            for (int i = 0; i < count; i++)
                line->GetItemByMembers (
                    i,
                    &(points[i].x),
                    &(points[i].y),
                    &zs[i],
                    &bogus,
                    &dimensions);
        }
    }
    else if (has_m)
    {   // has M, no Z
        ret = PolylineMShape::NewPolylineMShape (record, 1, count, box);
        points = ret->GetPoints ();
        ms = ((PolylineMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < count; i++)
            line->GetItemByMembers (
                i,
                &(points[i].x),
                &(points[i].y),
                &bogus,
                &ms[i],
                &dimensions);
        // update M limits in bounding box
        minm = numeric_limits<double>::max ();
        maxm = -numeric_limits<double>::max ();
        for (int i = 0; i < count; i++)
        {
            if (ms[i] < minm)
                minm = ms[i];
            if (ms[i] > maxm)
                maxm = ms[i];
        }
        ((PolylineMShape*)ret)->GetMData ()->SetRangeMin (minm);
        ((PolylineMShape*)ret)->GetMData ()->SetRangeMax (maxm);
    }
    else
    {   // has no Z or M
        ret = PolylineShape::NewPolylineShape (record, 1, count, box);
        points = ret->GetPoints ();
        for (int i = 0; i < count; i++)
            line->GetItemByMembers (
                i,
                &(points[i].x),
                &(points[i].y),
                &bogus,
                &bogus,
                &dimensions);
    }

    return (ret);
}

Shape* ShapeFile::ShapeFromMultiLine (FdoIMultiLineString* line, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    bool has_z;
    bool has_m;
    int lines;
    int count;
    int *parts;
    int index;
    FdoPtr<FdoILineString> l;
    DoublePoint* points;
    double* zs;
    double* ms;
    double bogus;
    double minm;
    double maxm;
    Shape* ret;

    dimensions = line->GetDimensionality ();
    has_z = (0 != (dimensions & FdoDimensionality_Z));
    has_m = (0 != (dimensions & FdoDimensionality_M));
    lines = line->GetCount();
    // count the total number of points
    count = 0;
    for (int i = 0; i < lines; i++)
    {
        l = line->GetItem (i);
        count += l->GetCount ();
    }
    index = 0;
    if (has_z)
    {
        ret = PolylineZShape::NewPolylineZShape (record, lines, count, box, has_m);
        points = ret->GetPoints ();
        parts = ((PolylineZShape*)ret)->GetParts ();
        zs = ((PolylineZShape*)ret)->GetZData ()->GetArray ();
        if (has_m)
        {
            // has Z and M
            ms = ((PolylineZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < lines; i++)
            {
                parts[i] = index;
                l = line->GetItem (i);
                count = l->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    l->GetItemByMembers (
                        j,
                        &points->x,
                        &points->y,
                        zs,
                        ms,
                        &dimensions);
                    points++;
                    zs++;
                    ms++;
                    index++;
                }
            }
            // update M limits in bounding box
            minm = numeric_limits<double>::max ();
            maxm = -numeric_limits<double>::max ();
            ms = ((PolylineZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < index; i++)
            {
                if (ms[i] < minm)
                    minm = ms[i];
                if (ms[i] > maxm)
                    maxm = ms[i];
            }
            ((PolylineZShape*)ret)->GetMData ()->SetRangeMin (minm);
            ((PolylineZShape*)ret)->GetMData ()->SetRangeMax (maxm);
        }
        else
            // has Z, no M
            for (int i = 0; i < lines; i++)
            {
                parts[i] = index;
                l = line->GetItem (i);
                count = l->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    l->GetItemByMembers (
                        j,
                        &points->x,
                        &points->y,
                        zs,
                        &bogus,
                        &dimensions);
                    points++;
                    zs++;
                    index++;
                }
            }
    }
    else if (has_m)
    {   // has M, no Z
        ret = PolylineMShape::NewPolylineMShape (record, lines, count, box, true);
        points = ret->GetPoints ();
        parts = ((PolylineMShape*)ret)->GetParts ();
        ms = ((PolylineMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < lines; i++)
        {
            parts[i] = index;
            l = line->GetItem (i);
            count = l->GetCount ();
            for (int j = 0; j < count; j++)
            {
                l->GetItemByMembers (
                    j,
                    &points->x,
                    &points->y,
                    &bogus,
                    ms,
                    &dimensions);
                points++;
                ms++;
                index++;
            }
        }
        // update M limits in bounding box
        minm = numeric_limits<double>::max ();
        maxm = -numeric_limits<double>::max ();
        ms = ((PolylineMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < index; i++)
        {
            if (ms[i] < minm)
                minm = ms[i];
            if (ms[i] > maxm)
                maxm = ms[i];
        }
        ((PolylineMShape*)ret)->GetMData ()->SetRangeMin (minm);
        ((PolylineMShape*)ret)->GetMData ()->SetRangeMax (maxm);
    }
    else
    {   // has no Z or M
        ret = PolylineShape::NewPolylineShape (record, lines, count, box);
        points = ret->GetPoints ();
        parts = ((PolylineShape*)ret)->GetParts ();
        for (int i = 0; i < lines; i++)
        {
            parts[i] = index;
            l = line->GetItem (i);
            count = l->GetCount ();
            for (int j = 0; j < count; j++)
            {
                l->GetItemByMembers (
                    j,
                    &(points->x),
                    &(points->y),
                    &bogus,
                    &bogus,
                    &dimensions);
                points++;
                index++;
            }
        }
    }

    return (ret);
}

Shape* ShapeFile::ShapeFromPolygon (FdoIPolygon* polygon, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    int count;
    FdoPtr<FdoILinearRing> ring;
    int vertices;
    int rings;
    DoublePoint* points;
    int *parts;
    int index;
    bool has_m;
    double* ms;
    double* zs;
    double bogus;
    double minm;
    double maxm;
    PolyShape* ret;

    dimensions = polygon->GetDimensionality ();
    // count the points
    ring = polygon->GetExteriorRing ();
    vertices = ring->GetCount ();
    rings = polygon->GetInteriorRingCount ();
    for (int i = 0; i < rings; i++)
    {
        ring = polygon->GetInteriorRing (i);
        vertices += ring->GetCount ();
    }
    index = 0;
    has_m = (0 != (dimensions & FdoDimensionality_M));
    if (0 != (dimensions & FdoDimensionality_Z))
    {
        ret = PolygonZShape::NewPolygonZShape (record, rings + 1, vertices, box, has_m);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        zs = ((PolygonZShape*)ret)->GetZData ()->GetArray ();
        if (has_m)
        {   // has Z and M
            ms = ((PolygonZShape*)ret)->GetMData ()->GetArray ();
            ring = polygon->GetExteriorRing ();
            count = ring->GetCount ();
            for (int i = 0; i < count; i++)
            {
                ring->GetItemByMembers (
                    i,
                    &(points[index].x),
                    &(points[index].y),
                    &zs[index],
                    &ms[index],
                    &dimensions);
                index++;
            }
            for (int i = 0; i < rings; i++)
            {
                parts[i + 1] = index;
                ring = polygon->GetInteriorRing (i);
                count = ring->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    ring->GetItemByMembers (
                        j,
                        &(points[index].x),
                        &(points[index].y),
                        &zs[index],
                        &ms[index],
                        &dimensions);
                    index++;
                }
            }
            // update M limits in bounding box
            minm = numeric_limits<double>::max ();
            maxm = -numeric_limits<double>::max ();
            for (int i = 0; i < index; i++)
            {
                if (ms[i] < minm)
                    minm = ms[i];
                if (ms[i] > maxm)
                    maxm = ms[i];
            }
            ((PolygonZShape*)ret)->GetMData ()->SetRangeMin (minm);
            ((PolygonZShape*)ret)->GetMData ()->SetRangeMax (maxm);
        }
        else
        {   // has Z, no M
            ring = polygon->GetExteriorRing ();
            count = ring->GetCount ();
            for (int i = 0; i < count; i++)
            {
                ring->GetItemByMembers (
                    i,
                    &(points[index].x),
                    &(points[index].y),
                    &zs[index],
                    &bogus,
                    &dimensions);
                index++;
            }
            for (int i = 0; i < rings; i++)
            {
                parts[i + 1] = index;
                ring = polygon->GetInteriorRing (i);
                count = ring->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    ring->GetItemByMembers (
                        j,
                        &(points[index].x),
                        &(points[index].y),
                        &zs[index],
                        &bogus,
                        &dimensions);
                    index++;
                }
            }
        }
    }
    else if (has_m)
    {   // has M, no Z
        ret = PolygonMShape::NewPolygonMShape (record, rings + 1, vertices, box);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        ms = ((PolygonMShape*)ret)->GetMData ()->GetArray ();
        ring = polygon->GetExteriorRing ();
        count = ring->GetCount ();
        for (int i = 0; i < count; i++)
        {
            ring->GetItemByMembers (
                i,
                &(points[index].x),
                &(points[index].y),
                &bogus,
                &ms[index],
                &dimensions);
            index++;
        }
        for (int i = 0; i < rings; i++)
        {
            parts[i + 1] = index;
            ring = polygon->GetInteriorRing (i);
            count = ring->GetCount ();
            for (int j = 0; j < count; j++)
            {
                ring->GetItemByMembers (
                    j,
                    &(points[index].x),
                    &(points[index].y),
                    &bogus,
                    &ms[index],
                    &dimensions);
                index++;
            }
        }
        // update M limits in bounding box
        minm = numeric_limits<double>::max ();
        maxm = -numeric_limits<double>::max ();
        for (int i = 0; i < index; i++)
        {
            if (ms[i] < minm)
                minm = ms[i];
            if (ms[i] > maxm)
                maxm = ms[i];
        }
        ((PolygonMShape*)ret)->GetMData ()->SetRangeMin (minm);
        ((PolygonMShape*)ret)->GetMData ()->SetRangeMax (maxm);
    }
    else
    {   // has no Z or M
        ret = PolygonShape::NewPolygonShape (record, rings + 1, vertices, box);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        ring = polygon->GetExteriorRing ();
        count = ring->GetCount ();
        for (int i = 0; i < count; i++)
        {
            ring->GetItemByMembers (
                i,
                &(points[index].x),
                &(points[index].y),
                &bogus,
                &bogus,
                &dimensions);
            index++;
        }
        for (int i = 0; i < rings; i++)
        {
            parts[i + 1] = index;
            ring = polygon->GetInteriorRing (i);
            count = ring->GetCount ();
            for (int j = 0; j < count; j++)
            {
                ring->GetItemByMembers (
                    j,
                    &(points[index].x),
                    &(points[index].y),
                    &bogus,
                    &bogus,
                    &dimensions);
                index++;
            }
        }
    }

    return (ret);
}

/* //ECO 10400 indicates we should never support FdoIMultiPolygon as input
Shape* ShapeFile::ShapeFromMultiPolygon (FdoIMultiPolygon* polygons, BoundingBoxEx* box, int record)
{
    FdoInt32 dimensions;
    int vertices;
    int rings;
    FdoPtr<FdoIPolygon> polygon;
    int polygon_count;
    int ring_count;
    int count;
    FdoPtr<FdoILinearRing> ring;
    DoublePoint* points;
    int *parts;
    int part_index;
    int vertex_index;
    bool has_m;
    double* ms;
    double* zs;
    double bogus;
    double minm;
    double maxm;
    PolyShape* ret;

    dimensions = polygons->GetDimensionality ();
    // count the points
    vertices = 0;
    rings = 0;
    polygon_count = polygons->GetCount ();
    for (int i = 0; i < polygon_count; i++)
    {
        polygon = polygons->GetItem (i);
        ring = polygon->GetExteriorRing ();
        rings++;
        vertices += ring->GetCount ();
        ring_count = polygon->GetInteriorRingCount ();
        rings += ring_count;
        for (int j = 0; j < ring_count; j++)
        {
            ring = polygon->GetInteriorRing (j);
            vertices += ring->GetCount ();
        }
    }
    part_index = 0;
    vertex_index = 0;
    has_m = (0 != (dimensions & FdoDimensionality_M));
    if (0 != (dimensions & FdoDimensionality_Z))
    {
        ret = PolygonZShape::NewPolygonZShape (record, rings, vertices, box, has_m);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        zs = ((PolygonZShape*)ret)->GetZData ()->GetArray ();
        if (has_m)
        {   // has Z and M
            ms = ((PolygonZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < polygon_count; i++)
            {
                parts[part_index] = vertex_index;
                part_index++;
                polygon = polygons->GetItem (i);
                ring = polygon->GetExteriorRing ();
                count = ring->GetCount ();
                for (int i = 0; i < count; i++)
                {
                    ring->GetItemByMembers (
                        i,
                        &(points->x),
                        &(points->y),
                        zs,
                        ms,
                        &dimensions);
                    points++;
                    zs++;
                    ms++;
                    vertex_index++;
                }
                ring_count = polygon->GetInteriorRingCount ();
                for (int i = 0; i < ring_count; i++)
                {
                    parts[part_index] = vertex_index;
                    part_index++;
                    ring = polygon->GetInteriorRing (i);
                    count = ring->GetCount ();
                    for (int j = 0; j < count; j++)
                    {
                        ring->GetItemByMembers (
                            j,
                            &(points->x),
                            &(points->y),
                            zs,
                            ms,
                            &dimensions);
                        points++;
                        zs++;
                        ms++;
                        vertex_index++;
                    }
                }
            }
            // update M limits in bounding box
            minm = numeric_limits<double>::max ();
            maxm = -numeric_limits<double>::max ();
            ms = ((PolygonZShape*)ret)->GetMData ()->GetArray ();
            for (int i = 0; i < vertex_index; i++)
            {
                if (ms[i] < minm)
                    minm = ms[i];
                if (ms[i] > maxm)
                    maxm = ms[i];
            }
            ((PolygonZShape*)ret)->GetMData ()->SetRangeMin (minm);
            ((PolygonZShape*)ret)->GetMData ()->SetRangeMax (maxm);
        }
        else
        {   // has Z, no M
            for (int i = 0; i < polygon_count; i++)
            {
                parts[part_index] = vertex_index;
                part_index++;
                polygon = polygons->GetItem (i);
                ring = polygon->GetExteriorRing ();
                count = ring->GetCount ();
                for (int i = 0; i < count; i++)
                {
                    ring->GetItemByMembers (
                        i,
                        &(points->x),
                        &(points->y),
                        zs,
                        &bogus,
                        &dimensions);
                    points++;
                    zs++;
                    vertex_index++;
                }
                ring_count = polygon->GetInteriorRingCount ();
                for (int i = 0; i < ring_count; i++)
                {
                    parts[part_index] = vertex_index;
                    part_index++;
                    ring = polygon->GetInteriorRing (i);
                    count = ring->GetCount ();
                    for (int j = 0; j < count; j++)
                    {
                        ring->GetItemByMembers (
                            j,
                            &(points->x),
                            &(points->y),
                            zs,
                            &bogus,
                            &dimensions);
                        points++;
                        zs++;
                        vertex_index++;
                    }
                }
            }
        }
    }
    else if (has_m)
    {   // has M, no Z
        ret = PolygonMShape::NewPolygonMShape (record, rings, vertices, box);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        ms = ((PolygonMShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < polygon_count; i++)
        {
            parts[part_index] = vertex_index;
            part_index++;
            polygon = polygons->GetItem (i);
            ring = polygon->GetExteriorRing ();
            count = ring->GetCount ();
            for (int i = 0; i < count; i++)
            {
                ring->GetItemByMembers (
                    i,
                    &(points->x),
                    &(points->y),
                    &bogus,
                    ms,
                    &dimensions);
                points++;
                ms++;
                vertex_index++;
            }
            ring_count = polygon->GetInteriorRingCount ();
            for (int i = 0; i < ring_count; i++)
            {
                parts[part_index] = vertex_index;
                part_index++;
                ring = polygon->GetInteriorRing (i);
                count = ring->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    ring->GetItemByMembers (
                        j,
                        &(points->x),
                        &(points->y),
                        &bogus,
                        ms,
                        &dimensions);
                    points++;
                    ms++;
                    vertex_index++;
                }
            }
        }
        // update M limits in bounding box
        minm = numeric_limits<double>::max ();
        maxm = -numeric_limits<double>::max ();
        ms = ((PolygonZShape*)ret)->GetMData ()->GetArray ();
        for (int i = 0; i < vertex_index; i++)
        {
            if (ms[i] < minm)
                minm = ms[i];
            if (ms[i] > maxm)
                maxm = ms[i];
        }
        ((PolygonMShape*)ret)->GetMData ()->SetRangeMin (minm);
        ((PolygonMShape*)ret)->GetMData ()->SetRangeMax (maxm);
    }
    else
    {   // has no Z or M
        ret = PolygonShape::NewPolygonShape (record, rings, vertices, box);
        points = ret->GetPoints ();
        parts = ret->GetParts ();
        for (int i = 0; i < polygon_count; i++)
        {
            parts[part_index] = vertex_index;
            part_index++;
            polygon = polygons->GetItem (i);
            ring = polygon->GetExteriorRing ();
            count = ring->GetCount ();
            for (int i = 0; i < count; i++)
            {
                ring->GetItemByMembers (
                    i,
                    &(points->x),
                    &(points->y),
                    &bogus,
                    &bogus,
                    &dimensions);
                points++;
                vertex_index++;
            }
            ring_count = polygon->GetInteriorRingCount ();
            for (int i = 0; i < ring_count; i++)
            {
                parts[part_index] = vertex_index;
                part_index++;
                ring = polygon->GetInteriorRing (i);
                count = ring->GetCount ();
                for (int j = 0; j < count; j++)
                {
                    ring->GetItemByMembers (
                        j,
                        &(points->x),
                        &(points->y),
                        &bogus,
                        &bogus,
                        &dimensions);
                    points++;
                    vertex_index++;
                }
            }
        }
    }

    return (ret);
}
*/

Shape* ShapeFile::ShapeFromGeometry (FdoByteArray* bytes, int nRecordNumber)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIGeometry> geometry;
    FdoPtr<FdoIEnvelope> envelope;
    BoundingBoxEx box;
    FdoGeometryType type;
    Shape* ret;

    // ESRI numbers their shapes starting at 1
    nRecordNumber++;

    // construct the geometry from the bytes
    factory = FdoFgfGeometryFactory::GetInstance ();
    geometry = factory->CreateGeometryFromFgf (bytes);

    // set the bounding box, we're going to need it
    envelope = geometry->GetEnvelope ();
    box.xMin = envelope->GetMinX ();
    box.yMin = envelope->GetMinY ();
    box.xMax = envelope->GetMaxX ();
    box.yMax = envelope->GetMaxY ();
    box.zMin = envelope->GetMinZ ();
    box.zMax = envelope->GetMaxZ ();
    // the following aren't used... each ShapeFromXXX fetches it's own M values
    // but envelope really needs to be expanded to handle M values
    box.mMin = fNO_DATA;
    box.mMax = fNO_DATA;

    // construct shape based on the geometry type and dimensionality
    type = geometry->GetDerivedType ();
    switch (type)
    {
        case FdoGeometryType_None:
            ret = NullShape::NewNullShape (nRecordNumber);
            break;
        case FdoGeometryType_Point:
            ret = ShapeFromPoint ((FdoIPoint*)geometry.p, &box, nRecordNumber);
            break;
        case FdoGeometryType_LineString:
            ret = ShapeFromLine ((FdoILineString*)geometry.p, &box, nRecordNumber);
            break;
        case FdoGeometryType_Polygon:
            ret = ShapeFromPolygon ((FdoIPolygon*)geometry.p, &box, nRecordNumber);
            break;
        case FdoGeometryType_MultiPoint:
            ret = ShapeFromMultiPoint ((FdoIMultiPoint*)geometry.p, &box, nRecordNumber);
            break;
        case FdoGeometryType_MultiLineString:
            ret = ShapeFromMultiLine ((FdoIMultiLineString*)geometry.p, &box, nRecordNumber);
            break;
        default:
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_SHAPE, "The '%1$ls' geometry type is not supported by Shp.", (FdoString *) FdoCommonMiscUtil::FdoGeometryTypeToString(type)));
            break;
    }

    return (ret);
}

void ShapeFile::SetObjectAt (Shape* shape, bool batch)
{
    ULONG		current;
	FdoInt64	current64;

    // write the header
    WriteRecordHeader (shape->GetRecordNum (), shape->GetContentLength ());
    // write the shape
    PutShape (shape);
    // update the file length (the bounding box is updated with help from the R-Tree)
    if (GetFilePointer64 (current64))
    {
		current = (ULONG)current64;

        if (current > (unsigned long)m_nFileLength * 2)
        {
            m_nFileLength = current / 2;
            SetHeaderDirty (true);
            if (!batch)
                PutFileHeaderDetails ();
        }
    }
    else
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::SetObjectAt");

    ClearRowShapeCache();
}

/*****************************************************************************
 * Name         : ReadRawDataBlock
 *
 * Description  : This method reads in raw data from the SHP file,
 *                caching at least 1 and at most SHP_FILE_READ_CACHE_SIZE records.
 *
 * Parameters   : ULONG  ulStartOffset:  this is the offset in the SHP file from
 *                                       where to start reading.
 *
 * Return       : void
 *
 * Notes        : N/A
 *
 *****************************************************************************/

void ShapeFile::ReadRawDataBlock(ULONG ulStartOffset )
{
    ATLTRACE(L"ReadRawDataBlock()\n");
    ULONG   nOffset = ulStartOffset;
    int     numRecs = 0;
    long    bytesRead;

    ClearRowShapeCache();

    SHPRecordInfo *firstRecInfo = &m_ReadRecordsBuffer[0];
    firstRecInfo->nOffset = ulStartOffset;

    (void) ReadRecordInfo ( firstRecInfo );
    
    size_t  size = sizeof(SHPRecordHeader) + firstRecInfo->nContentLength * WORD_SIZE_IN_BYTES;

    if ( size < SHP_SHAPE_BUFFER_MIN_SIZE )
        size = SHP_SHAPE_BUFFER_MIN_SIZE;

    // Allocate a new buffer or reallocate if not large enough
    if ( m_szRowBuffer == NULL || m_nRowBufferSize < size )
    {
        if ( !SHP_USE_REALLOC )
        {
            if ( m_szRowBuffer != NULL )
                delete[] m_szRowBuffer;
            m_szRowBuffer = new BYTE[size];
        }
        else
        {
            if ( m_szRowBuffer != NULL )
                m_szRowBuffer = (BYTE *)realloc( (void *)m_szRowBuffer, size );
            else
                m_szRowBuffer = (BYTE *)malloc( size ); 

            if ( m_szRowBuffer == NULL )
                throw FdoException::Create (NlsMsgGet(SHP_OUT_OF_MEMORY_ERROR, "Out of memory."));
        }

        m_nRowBufferSize = size;
    }

    // Read a block of data
    if (!SetFilePointer64((FdoInt64)firstRecInfo->nOffset))
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRawDataBlock(SetFilePointer64)");

    if ( !ReadFile (m_szRowBuffer, (long) m_nRowBufferSize, &bytesRead) )
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRawDataBlock(ReadFile)");

    BYTE* p = m_szRowBuffer;

    // Traverse the buffer, collect info headers and remember them in the cache.
    for ( int i = 0; i < SHP_FILE_READ_CACHE_SIZE; i++)
    {
        SHPRecordInfo   *pRecordInfo = &m_ReadRecordsBuffer[i];
        SHPRecordHeader header = *((SHPRecordHeader *)p);

        pRecordInfo->nOffset = nOffset;
		pRecordInfo->bOffsetValid = true;

        // These are stored in BigEndian so they must be swapped
        pRecordInfo->nRecordNumber = SWAPLONG(header.nRecordNumber);
        pRecordInfo->nContentLength = SWAPLONG(header.nContentLength);

        // Compute the next record's offset
        nOffset = pRecordInfo->nOffset + sizeof(SHPRecordHeader) + pRecordInfo->nContentLength * WORD_SIZE_IN_BYTES;

        // Make sure the next record's header is available.
        if ( (nOffset - firstRecInfo->nOffset + sizeof(SHPRecordHeader)) > (ULONG)bytesRead )
        {
            // Did we read this current record entirely (header + content)?  If not, invalidate the offset:
            if ( (nOffset - firstRecInfo->nOffset) > (ULONG)bytesRead )
			{
                pRecordInfo->bOffsetValid = false;
			}

            // We are done reading (we dont have enough bytes to read the next record's header), exit loop:
            break;
        }

        // Advance in the buffer
        p += nOffset - pRecordInfo->nOffset;
    }
}

/*****************************************************************************
 * Name         : ReadRecordInfo
 *
 * Description  : This method reads the record header.
 *
 * Parameters   : SHPRecordInfo *pRecordInfo
 *
 * Return       : void
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeFile::ReadRecordInfo(SHPRecordInfo *pRecordInfo )
{
    ATLTRACE(L"ReadRecordInfo()\n");
    int     nStatus = SHP_OK;
    long    bytesRead;

    if ( !SetFilePointer64((FdoInt64)pRecordInfo->nOffset) )
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRecordInfo(SetFilePointer64)");

    SHPRecordHeader shpRecordHeader;
    if ( ReadFile(&shpRecordHeader, sizeof(SHPRecordHeader), &bytesRead) && bytesRead != 0)
    {
        // These are stored in BigEndian so they must be swapped
        pRecordInfo->nRecordNumber = SWAPLONG(shpRecordHeader.nRecordNumber);
        pRecordInfo->nContentLength = SWAPLONG(shpRecordHeader.nContentLength);

        if(pRecordInfo->nRecordNumber < 1)
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_RECORD_NUMBER_ERROR, "Invalid record number %1$ld for file '%2$ls'.", pRecordInfo->nRecordNumber, FileName ()));
    } 
    else
        throw FdoCommonFile::LastErrorToException (L"ShapeFile::ReadRecordInfo()");
}

/*****************************************************************************
 * Name         : GetRowShapeFromCache
 *
 * Description  : This method gets the specified record # data from the cache
 *                if it is there.
 *
 * Parameters   : ULONG nOffset
 *                int nRecord
 *
 * Return       : true if found, false otherwise
 *
 * Notes        : Records start with 0
 *
 *****************************************************************************/
BYTE* ShapeFile::GetRowShapeFromCache(ULONG nOffset, int& nRecordNumber)
{
    BYTE    *pShapeRecord = NULL;
    int     rowOffset = 0;

    for ( int i = 0; i < SHP_FILE_READ_CACHE_SIZE && pShapeRecord == NULL; i++ )
    {
        if (!m_ReadRecordsBuffer[i].bOffsetValid)
            break;

        if ( m_ReadRecordsBuffer[i].nOffset == nOffset )
        {
            pShapeRecord = &m_szRowBuffer[nOffset - m_ReadRecordsBuffer[0].nOffset + sizeof(SHPRecordHeader)];
            nRecordNumber = m_ReadRecordsBuffer[i].nRecordNumber;
        }
    }

    if ( pShapeRecord != NULL )
        m_nCacheHits++;
    else
        m_nCacheMisses++;

    return pShapeRecord;
}

/*****************************************************************************/
void ShapeFile::ClearRowShapeCache()
{
    // Mark the slots as unused.
    for ( int i = 0; i < SHP_FILE_READ_CACHE_SIZE; i++)
    {
        m_ReadRecordsBuffer[i].bOffsetValid = false;
    }   
}

