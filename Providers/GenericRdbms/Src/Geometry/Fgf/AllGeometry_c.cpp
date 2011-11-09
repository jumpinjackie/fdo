/*
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
*/

#include <Inc/Nls/rdbi_msg.h>
#include <stdio.h>
#include "FdoGeometry.h"
#include <math.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include "FdoSpatial.h"
#include <float.h>      // For _isnan()

#include <Inc/Geometry/Fgf/AllGeometry_c.h>	

// C functions for wrapping C++ calls 

#define ZGEOM_FLAG 0x01
#define MGEOM_FLAG 0x02
#define VALIDGEOM_FLAG 0x04
#define PTGEOM_FLAG 0x08
#define LNGEOM_FLAG 0x10
#define LHGEOM_FLAG 0x20 //larger than a hemisphere
#define GEOM_HASZ(val) (((val)&ZGEOM_FLAG) != 0x00)
#define GEOM_HASM(val) (((val)&MGEOM_FLAG) != 0x00)
#define GEOM_ISVALID(val) (((val)&VALIDGEOM_FLAG) != 0x00)
#define GEOM_ISPT(val) (((val)&PTGEOM_FLAG) != 0x00)
#define GEOM_ISLN(val) (((val)&LNGEOM_FLAG) != 0x00)
#define GEOM_ISNOTPTLN(val) (((val)&(PTGEOM_FLAG|LNGEOM_FLAG)) == 0x00)
#define GEOM_ISLH(val) (((val)&LHGEOM_FLAG) != 0x00)
#define DEF_SRID_GEOMETRY 0
#define DEF_SRID_GEOGRAPHY 4326
#define ADDR_INTSTEP 8
#define NEG_ONE_LE (size_t(-1))
#define CHECK_CLOCKWISE 1 // this check is temporar enabled

#define FdoByte unsigned char
#ifdef _WIN32
#define FdoInt64 _int64
#else
#define FdoInt64 int64_t
#endif

#define LittleEndianToDb(ptr) *(double*)((unsigned FdoInt64*)(ptr))
#define LittleEndianToInt(ptr) *(int*)(ptr); ptr+=sizeof(int)
#define DbToLittleEndian(ptr) *(FdoInt64*)(double*)(ptr)

struct MemoryBuffer
{
    FdoByte* mBuff;
    size_t mBuffAlloc;
    size_t mBuffLen;
    size_t mBuffReqLen;
    MemoryBuffer()
    {
        mBuff = NULL;
        mBuffReqLen = mBuffAlloc = mBuffLen = 0;
    }
    ~MemoryBuffer()
    {
        delete[] mBuff;
    }
    void Reset()
    {
        mBuffLen = mBuffReqLen = 0;
    }
    void EnsureLenBuffer(size_t len)
    {
        mBuffReqLen = len;
        if (mBuff == NULL)
        {
            mBuffAlloc = (size_t)(len*1.4);
            mBuff = new FdoByte[mBuffAlloc];
        }
        else if (mBuffAlloc <= len)
        {
            FdoByte* tmp = new FdoByte[(size_t)(len*1.4)];
            memcpy(tmp, mBuff, mBuffAlloc * sizeof(FdoByte));
            delete [] mBuff;
            mBuff = tmp;
            mBuffAlloc = (size_t)(len*1.4);
        }
    }    
    void EnsureAdditionalLenBuffer(size_t len)
    {
        if ((mBuffReqLen + len) >= mBuffAlloc)
            EnsureLenBuffer(mBuffReqLen + len);
    }
    void FillWith(double val, size_t count)
    {
        double* ptr = (double*)mBuff;
        for (size_t i = 0; i < count; i++)
            *ptr++ = val;
    }
};

#define BUFF_PUSH_BYTE(buff, val) *((FdoByte*)buff) = (FdoByte)(val), buff+=sizeof(FdoByte);
#define BUFF_PUSH_INT(buff, val) *((int*)buff) = (int)(val), buff+=sizeof(int);
#define BUFF_PUSH_DBL(buff, val) *((double*)buff) = (double)(val), buff+=sizeof(double);
#define BUFF_PUSH_INT64(buff, val) *((FdoInt64*)buff) = (FdoInt64)(val), buff+=sizeof(FdoInt64);
#define BUFF_PUSH_LEINT(buff, val) *((int*)buff) = (int)((val)), buff+=sizeof(int);
#define FDODIM2NCOORDS(x) (x + 2 - x/2)
#define GEOM_HEADER_SIZE (2*sizeof(size_t)+2*sizeof(FdoByte))
#define GEOM_FOOTER_SIZE(fig, shp) (2*sizeof(size_t)+(fig)*(sizeof(size_t)+sizeof(FdoByte))+(shp)*(2*sizeof(size_t)+sizeof(FdoByte)))
#define VERY_SMALL (1.0e-17)

#define BUFF_WRITE_POINTHXYZM \
    if (handle.isLatLong){ \
    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian((dreader+1))); \
    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader)); \
    dreader += 2; \
    } else { \
    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++)); \
    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++)); } \
    if ((tmp&FdoDimensionality_Z) != 0) { BUFF_PUSH_INT64(handle.zBuff, DbToLittleEndian(dreader++)); } \
    else if (handle.hasZ) { BUFF_PUSH_INT64(handle.zBuff, DbToLittleEndian(&handle.NaN)); } \
    if ((tmp&FdoDimensionality_M) != 0) { BUFF_PUSH_INT64(handle.mBuff, DbToLittleEndian(dreader++)); } \
    else if (handle.hasM) { BUFF_PUSH_INT64(handle.mBuff, DbToLittleEndian(&handle.NaN)); }

namespace sqlgeomconv
{
    typedef enum
    {
        FigureType_InteriorRing = 0x00, // v2 FigureType_Point
        FigureType_Stroke = 0x01,       // v2 FigureType_Line
        FigureType_ExteriorRing = 0x02, // v2 FigureType_Arc
        FigureType_CompositeCurve = 0x03 // v2 FigureType_CompositeCurve
    }FiguresAttributeType;

    typedef enum
    {
        OpenGISShapeType_Point = 0x01,
        OpenGISShapeType_LineString = 0x02,
        OpenGISShapeType_Polygon = 0x03,
        OpenGISShapeType_MultiPoint = 0x04,
        OpenGISShapeType_MultiLineString = 0x05,
        OpenGISShapeType_MultiPolygon = 0x06,
        OpenGISShapeType_GeometryCollection = 0x07,
        OpenGISShapeType_CircularString = 0x08,
        OpenGISShapeType_CompoundCurve = 0x09,
        OpenGISShapeType_CurvePolygon = 0x0A,
        OpenGISShapeType_FullGlobe = 0x0B
    }OpenGISShapeType;

    typedef enum
    {
        SegmentType_Line = 0x00,
        SegmentType_Arc = 0x01,
        SegmentType_FirstLine = 0x02,
        SegmentType_FirstArc = 0x03
    }SegmentType;

    typedef struct 
    {
        FiguresAttributeType type;
        size_t ptIndex;
        size_t cntPts;
    }FigureDescriptor;

    typedef struct ShapeDescriptor
    {
        OpenGISShapeType type;
        // figures
        size_t figIndex;
        size_t cntFigs;

        // shape details
        size_t parent;
        size_t* subShapes;
        size_t cntShps;
        size_t processed;
        ShapeDescriptor()
        {
            parent = figIndex = NEG_ONE_LE;
            type = OpenGISShapeType_Point;
            subShapes = NULL;
            cntShps = 0;
            processed = false;
        }
        ~ShapeDescriptor()
        {
            free(subShapes);
        }
        void AddShape(size_t index)
        {
            cntShps++;
            size_t* old = subShapes;
            subShapes = (size_t*)realloc(subShapes, cntShps*sizeof(size_t));
            if (subShapes == NULL)
            {
                free(old);
                throw FdoException::Create(L"Cannot allocate memory");
            }
            subShapes[cntShps-1] = index;
        }
    } ShapeDescriptor;

    typedef struct GeomReadHandle
    {
        bool hasZ;
        bool hasM;
        bool isLatLong;
        size_t ptCnt;
        size_t fgCnt;
        size_t shCnt;
        size_t sgCnt;
        const FdoByte* startPt;
        FigureDescriptor* figures;
        ShapeDescriptor* shapes;
        SegmentType* segments;
        size_t currSgIdx;
        MemoryBuffer lCacheBuff;
        FdoFgfGeometryFactory* gf;

        GeomReadHandle()
        {
            isLatLong = hasZ = hasM = false;
            currSgIdx = ptCnt = fgCnt = shCnt = sgCnt = 0;
            startPt = NULL;
            figures = NULL;
            shapes = NULL;
            segments = NULL;
            gf = NULL;
        }
        ~GeomReadHandle()
        {
            delete[] figures;
            delete[] shapes;
            delete[] segments;
            FDO_SAFE_RELEASE(gf);
        }
        void Clear()
        {
            isLatLong = hasZ = hasM = false;
            currSgIdx = ptCnt = fgCnt = shCnt = sgCnt = 0;
            delete[] figures;
            figures = NULL;
            delete[] shapes;
            shapes = NULL;
            delete[] segments;
            segments = NULL;
            lCacheBuff.Reset();
        }
        inline FdoByte* Buffer() { return lCacheBuff.mBuff; }
        inline void EnsureLenBuffer(size_t len) { lCacheBuff.EnsureLenBuffer(len); }
        FdoFgfGeometryFactory* GetGeometryFactory()
        {
            if (gf == NULL)
                gf = FdoFgfGeometryFactory::GetInstance();
            return gf;
        }
    } GeomReadHandle;

    typedef struct GeomWriteHandle
    {
        bool hasZ;
        bool hasM;
        bool isLatLong;
        std::vector<FigureDescriptor*> figures;
        std::vector<ShapeDescriptor*> shapes;
        std::vector<SegmentType> segments;
        int* ireader;
        FdoByte* lBuff;
        FdoByte* zBuff;
        FdoByte* mBuff;
        size_t cntPoints;
        size_t evalCntPts;
        bool hasArcs;
        FdoFgfGeometryFactory* gf;
        MemoryBuffer lCacheBuff;
        MemoryBuffer zCacheBuff;
        MemoryBuffer mCacheBuff;
        double NaN;

        GeomWriteHandle()
        {
            isLatLong = hasArcs = hasZ = hasM = false;
            ireader = NULL;
            lBuff = zBuff = mBuff = NULL;
            evalCntPts = cntPoints = 0;
            gf = NULL;
            NaN = std::numeric_limits<double>::quiet_NaN();
        }
        void Clear ()
        {
            isLatLong = hasArcs = hasZ = hasM = false;
            for(std::vector<FigureDescriptor*>::iterator it = figures.begin(); it < figures.end(); it++)
                delete *it;
            for(std::vector<ShapeDescriptor*>::iterator it = shapes.begin(); it < shapes.end(); it++)
                delete *it;
            figures.clear();
            shapes.clear();
            segments.clear();
            ireader = NULL;
            lBuff = zBuff = mBuff = NULL;
            evalCntPts = cntPoints = 0;
            lCacheBuff.Reset();
            zCacheBuff.Reset();
            mCacheBuff.Reset();
        }
        void Reserve(size_t cntFig, size_t cntShp)
        {
            figures.reserve(cntFig);
            shapes.reserve(cntShp);
        }

        ~GeomWriteHandle()
        {
            Clear ();
            FDO_SAFE_RELEASE(gf);
        }
        inline void MarkBufferLen(FdoByte* act) { lCacheBuff.mBuffLen = act-lCacheBuff.mBuff; }
        inline size_t BufferLen() {return lCacheBuff.mBuffLen ;}
        inline FdoByte* Buffer() { return lCacheBuff.mBuff; }
        inline FdoByte* BufferZ() { return zCacheBuff.mBuff; }
        inline FdoByte* BufferM() { return mCacheBuff.mBuff; }
        inline void EnsureLenBuffer(size_t len) { lCacheBuff.EnsureLenBuffer(len); }

        FdoFgfGeometryFactory* GetGeometryFactory()
        {
            if (gf == NULL)
                gf = FdoFgfGeometryFactory::GetInstance();
            return gf;
        }

        inline double SnapToZero(double n) { return (fabs(n) <= VERY_SMALL)? 0.0 : n; }

        bool PointsAreClockwise(size_t dim, size_t cntPts, double* points)
        {
            const double *startpoint = points;
            const double *midpoint = points+dim;
            const double *endpoint = midpoint+dim;
            double v21[3], v31[3];

	        v21[0] =  *midpoint - *startpoint;
	        v21[1] =  *(midpoint+1) - *(startpoint+1);
            v21[2] =  0.0;

	        v31[0] =  *endpoint - *startpoint;
	        v31[1] =  *(endpoint+1) - *(startpoint+1);
            v31[2] =  0.0;
            double normalX = SnapToZero(v21[1] * v31[2] - v31[1] * v21[2]);
            double normalY = SnapToZero(v21[2] * v31[0] - v31[2] * v21[0]);
            double normalZ = SnapToZero(v21[0] * v31[1] - v31[0] * v21[1]);
            // Normalize

	        double a;
	        a = (normalX*normalX) + (normalY*normalY) + (normalZ*normalZ);
            a = SnapToZero(a);
            if (0.0 != a)
    	        a = 1 / sqrt ( a );

            return !((a * normalZ) > 0.0);
        }

        void EnsureSpaceIncrease(size_t val)
        {
            int dim = 2 + (int)hasZ + (int)hasM;
            size_t addPts = (size_t)((double)val/(dim*sizeof(double)));
            evalCntPts += addPts;
            if (hasZ)
            {
                FdoInt64 offset = zBuff - zCacheBuff.mBuff;
                zCacheBuff.EnsureAdditionalLenBuffer(evalCntPts*sizeof(double));
                zBuff = zCacheBuff.mBuff + offset;
            }
            if (hasM)
            {
                FdoInt64 offset = mBuff - mCacheBuff.mBuff;
                mCacheBuff.EnsureAdditionalLenBuffer(evalCntPts*sizeof(double));
                mBuff = mCacheBuff.mBuff + offset;
            }
            FdoInt64 offset = lBuff - lCacheBuff.mBuff;
            lCacheBuff.EnsureAdditionalLenBuffer(evalCntPts*sizeof(double));
            lBuff = lCacheBuff.mBuff + offset;
        }
        void EnsureSpaceZM(unsigned int dim)
        {
            // in case we found a polygon with Z and till now we did not have Z add the Z points
            if (!hasZ && (dim&FdoDimensionality_Z) != 0)
            {
                hasZ = true;
                zCacheBuff.EnsureLenBuffer(evalCntPts*sizeof(double));
                if (cntPoints != 0)
                    zCacheBuff.FillWith(NaN, cntPoints);
                zBuff = zCacheBuff.mBuff + cntPoints*sizeof(double);
            }
            // in case we found a polygon with M and till now we did not have M add the M points
            if (!hasM && (dim&FdoDimensionality_M) != 0)
            {
                hasM = true;
                mCacheBuff.EnsureLenBuffer(evalCntPts*sizeof(double));
                if (cntPoints != 0)
                    mCacheBuff.FillWith(NaN, cntPoints);
                mBuff = mCacheBuff.mBuff + cntPoints*sizeof(double);
            }
        }
        void WriteZMPointsIfNeeed()
        {
            // copy Z, M points if needed
            if (hasZ)
            {
                memcpy(lBuff, zCacheBuff.mBuff, cntPoints*sizeof(double));
                lBuff += cntPoints*sizeof(double);
            }
            if (hasM)
            {
                memcpy(lBuff, mCacheBuff.mBuff, cntPoints*sizeof(double));
                lBuff += cntPoints*sizeof(double);
            }
        }
        void WriteFooterGeom()
        {
            if (figures.size() != 0)
            {
                BUFF_PUSH_LEINT(lBuff, figures.size()); // number of figures
                for (size_t idx = 0; idx < figures.size(); idx++)
                {
                    FigureDescriptor* pfig = figures[idx];
                    BUFF_PUSH_BYTE(lBuff, pfig->type); // figure type
                    BUFF_PUSH_LEINT(lBuff, pfig->ptIndex); // figure point offset
                }
            }
            if (shapes.size() != 0)
            {
                BUFF_PUSH_LEINT(lBuff, shapes.size()); // number of shapes
                for (size_t idx = 0; idx < shapes.size(); idx++)
                {
                    ShapeDescriptor* pShp = shapes[idx];
                    BUFF_PUSH_LEINT(lBuff, pShp->parent); // shape parent
                    BUFF_PUSH_LEINT(lBuff, pShp->figIndex); // shape figure offset
                    BUFF_PUSH_BYTE(lBuff, pShp->type); // shape type
                }
            }
            if (segments.size() != 0)
            {
                BUFF_PUSH_LEINT(lBuff, segments.size()); // number of segments
                for (size_t idx = 0; idx < segments.size(); idx++)
                {
                    BUFF_PUSH_BYTE(lBuff, segments[idx]); // segment type
                }
            }
        }
        inline OpenGISShapeType FdoTypeToShapeType(FdoGeometryType type)
        {
            switch (type)
            {
            case FdoGeometryType_MultiPolygon:
                return OpenGISShapeType_MultiPolygon;
            case FdoGeometryType_MultiLineString:
                return OpenGISShapeType_MultiLineString;
            case FdoGeometryType_MultiPoint:
                return OpenGISShapeType_MultiPoint;
            case FdoGeometryType_MultiGeometry:
                return OpenGISShapeType_GeometryCollection;
            }
            throw FdoException::Create(L"Invalid type!");
        }
        bool IsNan(double n)
        {
#ifdef _WIN32
            return _isnan(n) ? true : false;
#else
            return isnan(n) ? true : false;
#endif
        }
    } GeomWriteHandle;

    void ReadAndPushPointByte(FdoByte* &myBuff, GeomReadHandle& handle, size_t idx)
    {
        if (handle.isLatLong) // switch X with Y
        {
            BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + (2*idx+1)*ADDR_INTSTEP));
            BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + 2*idx*ADDR_INTSTEP));
        }
        else
        {
            BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + 2*idx*ADDR_INTSTEP));
            BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + (2*idx+1)*ADDR_INTSTEP));
        }
        if (handle.hasZ) BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + (2*handle.ptCnt+idx)*ADDR_INTSTEP));
        if (handle.hasM) BUFF_PUSH_DBL(myBuff, LittleEndianToDb(handle.startPt + ((2+(int)handle.hasZ)*handle.ptCnt+idx)*ADDR_INTSTEP));
    }

    void ComposeSubGeometry(GeomReadHandle& handle, ShapeDescriptor* shp, FdoByte* &myBuff)
    {
        if (shp->processed)
            return;
        switch (shp->type)
        {
        case OpenGISShapeType_Point:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_Point); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                ReadAndPushPointByte(myBuff, handle, handle.figures[shp->figIndex].ptIndex);
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_LineString:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_LineString); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                size_t cntPt = handle.figures[shp->figIndex].cntPts;
                size_t idxPt = handle.figures[shp->figIndex].ptIndex;
                BUFF_PUSH_INT(myBuff, cntPt); // point count
                for (size_t idx = 0; idx < cntPt; idx++)
                {
                    ReadAndPushPointByte(myBuff, handle, idxPt+idx);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_Polygon:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_Polygon); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                BUFF_PUSH_INT(myBuff, shp->cntFigs); // contour count
                for (size_t idx = 0; idx < shp->cntFigs; idx++)
                {
                    size_t cntPt = handle.figures[shp->figIndex+idx].cntPts;
                    size_t idxPt = handle.figures[shp->figIndex+idx].ptIndex;
                    BUFF_PUSH_INT(myBuff, cntPt); // point count
                    for (size_t pidx = 0; pidx < cntPt; pidx++)
                    {
                        ReadAndPushPointByte(myBuff, handle, idxPt+pidx);
                    }
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_GeometryCollection:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_MultiGeometry); // geom type
                BUFF_PUSH_INT(myBuff, shp->cntShps); // geom count
                for (size_t idx = 0; idx < shp->cntShps; idx++){
                    ComposeSubGeometry(handle, handle.shapes+shp->subShapes[idx], myBuff);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_MultiLineString:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_MultiLineString); // geom type
                BUFF_PUSH_INT(myBuff, shp->cntShps); // geom count
                for (size_t idx = 0; idx < shp->cntShps; idx++){
                    ComposeSubGeometry(handle, handle.shapes+shp->subShapes[idx], myBuff);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_MultiPoint:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_MultiPoint); // geom type
                BUFF_PUSH_INT(myBuff, shp->cntShps); // geom count
                for (size_t idx = 0; idx < shp->cntShps; idx++){
                    ComposeSubGeometry(handle, handle.shapes+shp->subShapes[idx], myBuff);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_MultiPolygon:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_MultiPolygon); // geom type
                BUFF_PUSH_INT(myBuff, shp->cntShps); // geom count
                for (size_t idx = 0; idx < shp->cntShps; idx++){
                    ComposeSubGeometry(handle, handle.shapes+shp->subShapes[idx], myBuff);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_CircularString:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_CurveString); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                FigureDescriptor* fig = handle.figures+shp->figIndex;
                ReadAndPushPointByte(myBuff, handle, fig->ptIndex); // start point
                if (fig->cntPts == 3)
                {
                    BUFF_PUSH_INT(myBuff, 1);
                    BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_CircularArcSegment);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+1);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+2);
                }
                else // we do not support a circle
                {
                    BUFF_PUSH_INT(myBuff, 2);
                    BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_CircularArcSegment);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+1);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+2);
                    
                    BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_CircularArcSegment);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+3);
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex+4);
                }
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_CompoundCurve:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_CurveString); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                FigureDescriptor* fig = handle.figures+shp->figIndex;
                ReadAndPushPointByte(myBuff, handle, fig->ptIndex); // start point
                FdoByte* pcBuff = myBuff;
                size_t cntParts = 0;
                BUFF_PUSH_INT(myBuff, 1); // parts count we will update it later
                for (size_t idx = 1; idx < fig->cntPts; idx++)
                {
                    SegmentType sgType = handle.segments[handle.currSgIdx++];
                    if (sgType == SegmentType_FirstLine || sgType == SegmentType_Line)
                    {
                        // it's a line
                        BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_LineStringSegment);
                        FdoByte* lnBuff = myBuff;
                        size_t cntLnPts = 1;
                        BUFF_PUSH_INT(myBuff, 1); // points count we will update it later
                        ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx); // second point
                        for (size_t ipt = idx+1; ipt < fig->cntPts; ipt++)
                        {
                            SegmentType sgSubType;
                            if (handle.sgCnt == 0)
                                sgSubType = (fig->type == FigureType_ExteriorRing)? SegmentType_Arc : SegmentType_Line;
                            else 
                                sgSubType = handle.segments[handle.currSgIdx];
                            if (sgSubType == SegmentType_FirstLine || sgSubType == SegmentType_Line)
                            {
                                cntLnPts++;
                                handle.currSgIdx++;
                                ReadAndPushPointByte(myBuff, handle, fig->ptIndex + ipt); // next point
                                idx = ipt;
                            }
                            else
                            {
                                idx = ipt-1;
                                break;
                            }
                        }
                        BUFF_PUSH_INT(lnBuff, cntLnPts); // update points count
                        cntParts++;
                    }
                    else
                    {
                        // it's an arc
                        BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_CircularArcSegment);
                        ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx); // mid point
                        ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx+1); // end point
                        idx++;
                        cntParts++;
                    }
                }
                BUFF_PUSH_INT(pcBuff, cntParts); // update points count
                shp->processed = true;
            }
            break;
        case OpenGISShapeType_CurvePolygon:
            {
                BUFF_PUSH_INT(myBuff, FdoGeometryType_CurvePolygon); // geom type
                BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
                BUFF_PUSH_INT(myBuff, shp->cntFigs); // contour count
                for (size_t idx = 0; idx < shp->cntFigs; idx++)
                {
                    FigureDescriptor* fig = handle.figures+shp->figIndex+idx;
                    size_t idxPt = fig->ptIndex;
                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex); // start point
                    FdoByte* pcBuff = myBuff;
                    size_t cntParts = 0;
                    BUFF_PUSH_INT(myBuff, 1); // parts count we will update it later
                    for (size_t idx = 1; idx < fig->cntPts; idx++)
                    {
                        SegmentType sgType;
                        if (handle.sgCnt == 0)
                            sgType = (fig->type == FigureType_ExteriorRing)? SegmentType_Arc : SegmentType_Line;
                        else 
                            sgType = handle.segments[handle.currSgIdx++];
                        if (sgType == SegmentType_FirstLine || sgType == SegmentType_Line)
                        {
                            // it's a line
                            BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_LineStringSegment);
                            FdoByte* lnBuff = myBuff;
                            size_t cntLnPts = 1;
                            BUFF_PUSH_INT(myBuff, 1); // points count we will update it later
                            ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx); // second point
                            for (size_t ipt = idx+1; ipt < fig->cntPts; ipt++)
                            {
                                SegmentType sgSubType;
                                if (handle.sgCnt == 0)
                                    sgSubType = (fig->type == FigureType_ExteriorRing)? SegmentType_Arc : SegmentType_Line;
                                else 
                                    sgSubType = handle.segments[handle.currSgIdx];
                                if (sgSubType == SegmentType_FirstLine || sgSubType == SegmentType_Line)
                                {
                                    cntLnPts++;
                                    if (handle.sgCnt != 0)
                                        handle.currSgIdx++;
                                    ReadAndPushPointByte(myBuff, handle, fig->ptIndex + ipt); // next point
                                    idx = ipt;
                                }
                                else
                                {
                                    idx = ipt-1;
                                    break;
                                }
                            }
                            BUFF_PUSH_INT(lnBuff, cntLnPts); // update points count
                            cntParts++;
                        }
                        else
                        {
                            // it's an arc
                            BUFF_PUSH_INT(myBuff, FdoGeometryComponentType_CircularArcSegment);
                            ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx); // mid point
                            ReadAndPushPointByte(myBuff, handle, fig->ptIndex + idx+1); // end point
                            idx++;
                            cntParts++;
                        }
                    }
                    BUFF_PUSH_INT(pcBuff, cntParts); // update points count
                }
                shp->processed = true;
            }
            break;
        }
    }

    FdoByteArray* ConvertMsToFdoGeometry(GeomReadHandle& handle, FdoByte* &geom, FdoByte* end, size_t srid, FdoByte vers)
    {
        FdoByte sp = *geom++;
        handle.Clear();
        handle.hasZ = GEOM_HASZ(sp);
        handle.hasM = GEOM_HASM(sp);
        handle.isLatLong = (srid >= 4120 && srid <= 4999);
        if (!GEOM_ISVALID(sp))
        {
            // not sure what we can do here
            //printf("Geom is Not Valid\n");
        }

        if (GEOM_ISNOTPTLN(sp))
        {
            handle.ptCnt = LittleEndianToInt(geom);
            if (handle.ptCnt == 0)
                return NULL;
            
            handle.startPt = geom;

            // jump over points
            geom += ADDR_INTSTEP*handle.ptCnt*(2+ (int)handle.hasZ + (int)handle.hasM);

            // figures
            handle.fgCnt = LittleEndianToInt(geom);
            handle.figures = handle.fgCnt ? new FigureDescriptor[handle.fgCnt] : NULL;
            if (handle.fgCnt == 1)
                handle.figures[0].cntPts = handle.ptCnt;
            FigureDescriptor* pfigure = NULL;
            for (size_t idx = 0; idx < handle.fgCnt; idx++)
            {
                FigureDescriptor* figure = handle.figures+idx;
                figure->type = (FiguresAttributeType)*geom++;
                figure->ptIndex = LittleEndianToInt(geom);

                // to have performance calculate numer of points in this shape
                if (pfigure != NULL)
                    pfigure->cntPts = figure->ptIndex-pfigure->ptIndex;

                switch(figure->type)
                {
                case FigureType_InteriorRing:
                case FigureType_Stroke:
                case FigureType_ExteriorRing:
                    break;
                case FigureType_CompositeCurve:
                    if (vers != 0x02)
                        throw FdoException::Create(L"Invalid Figure type!");
                    break;
                default:
                    throw FdoException::Create(L"Invalid Figure type!");
                    break;
                }
                pfigure = figure;
            }
            if (pfigure != NULL)
                pfigure->cntPts = handle.ptCnt - pfigure->ptIndex;

            // shapes
            handle.shCnt = LittleEndianToInt(geom);

            // allocate if necessary space for FDO geom
            handle.EnsureLenBuffer(handle.ptCnt*(2 + (int)handle.hasZ + (handle.hasM?0x01:0x00))*sizeof(double) + 4*(handle.shCnt+handle.fgCnt)*sizeof(int));
            FdoByte* myBuff = handle.Buffer();

            if (handle.shCnt == 0)
                throw FdoException::Create(L"Invalid geometry!");

            handle.shapes = new ShapeDescriptor[handle.shCnt];
            ShapeDescriptor* pshape = NULL;
            for (size_t idx = 0; idx < handle.shCnt; idx++)
            {
                ShapeDescriptor* shape = handle.shapes+idx;
                shape->parent = LittleEndianToInt(geom);
                shape->figIndex = LittleEndianToInt(geom);
                shape->type = (OpenGISShapeType)*geom++;
                if (shape->parent != NEG_ONE_LE)
                {
                    ShapeDescriptor* parent = handle.shapes+shape->parent;
                    parent->AddShape(idx);
                }
                // to have performance calculate numer of figures in this shape
                if (pshape != NULL)
                    pshape->cntFigs = shape->figIndex - pshape->figIndex;

                switch(shape->type)
                {
                case OpenGISShapeType_Point:
                case OpenGISShapeType_LineString:
                case OpenGISShapeType_Polygon:
                case OpenGISShapeType_MultiPoint:
                case OpenGISShapeType_MultiLineString:
                case OpenGISShapeType_MultiPolygon:
                case OpenGISShapeType_GeometryCollection:
                    break;
                case OpenGISShapeType_CompoundCurve:
                    if (vers != 0x02)
                        throw FdoException::Create(L"Invalid Figure type!");
                    break;
                case OpenGISShapeType_CircularString:
                case OpenGISShapeType_CurvePolygon:
                    if (vers != 0x02)
                        throw FdoException::Create(L"Invalid Figure type!");
                    break;
                default:
                    throw FdoException::Create(L"Invalid Shape type!");
                    break;
                }
                pshape = shape;
            }
            if (pshape != NULL)
                pshape->cntFigs = handle.fgCnt - pshape->figIndex;

            if (geom != end)
            {
                handle.sgCnt = LittleEndianToInt(geom);
                if (handle.sgCnt != 0)
                    handle.segments = new SegmentType[handle.sgCnt];

                for (size_t idx = 0; idx < handle.sgCnt; idx++)
                    handle.segments[idx] = (SegmentType)*geom++;
            }

            for (size_t idx = 0; idx < handle.shCnt; idx++)
            {
                ShapeDescriptor* shape = handle.shapes+idx;
                ComposeSubGeometry(handle, shape, myBuff);
            }
            return FdoByteArray::Create(handle.Buffer(), (FdoInt32)(myBuff-handle.Buffer()));
        }
        else if (GEOM_ISPT(sp)) // simple point
        {
            handle.EnsureLenBuffer((2 + (int)handle.hasZ + (handle.hasM?0x01:0x00))*sizeof(double) + 2*sizeof(int));
            FdoByte* myBuff = handle.Buffer();
            handle.ptCnt = 1;
            handle.startPt = geom;
            BUFF_PUSH_INT(myBuff, FdoGeometryType_Point); // geom type
            BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
            ReadAndPushPointByte(myBuff, handle, 0);

            return FdoByteArray::Create(handle.Buffer(), (FdoInt32)(myBuff-handle.Buffer()));
        }
        else if (GEOM_ISLN(sp)) // simple line
        {
            handle.EnsureLenBuffer((4 + (int)handle.hasZ + (int)handle.hasM)*sizeof(double) + 3*sizeof(int));
            FdoByte* myBuff = handle.Buffer();
            handle.ptCnt = 2;
            handle.startPt = geom;

            BUFF_PUSH_INT(myBuff, FdoGeometryType_LineString); // geom type
            BUFF_PUSH_INT(myBuff, (0x00 | (handle.hasZ?0x01:0x00) + (handle.hasM?0x02:0x00))); // dim
            BUFF_PUSH_INT(myBuff, 2); // pt cnt

            // read all points
            ReadAndPushPointByte(myBuff, handle, 0);
            ReadAndPushPointByte(myBuff, handle, 1);

            return FdoByteArray::Create(handle.Buffer(), (FdoInt32)(myBuff-handle.Buffer()));
        }
        throw FdoException::Create(L"Invalid geometry!");
    }

    FdoIGeometry* ConvertMsToFdoGeometry(GeomReadHandle* handle, FdoByte* geom, size_t size)
    {
        FdoIGeometry* retVal = NULL;
        FdoByte* end = geom + size;
        size_t srid = LittleEndianToInt(geom);
        if (srid == NEG_ONE_LE)
            return retVal;

        FdoByte vers = *geom++;
        switch(vers)
        {
        case 0x02: // version 2
        case 0x01: // version 1
            {
                FdoPtr<FdoByteArray> retArr = ConvertMsToFdoGeometry(*handle, geom, end, srid, vers);
                FdoFgfGeometryFactory* gf = handle->GetGeometryFactory();
                retVal = gf->CreateGeometryFromFgf(retArr);
            }
            break;
        default:
            // GetConverter (vers);
            break;
        }
        return retVal;
    }


    void BuildPolygon(GeomWriteHandle& handle, size_t shpParentIdx)
    {
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader++;
        if (geom_type == FdoGeometryType_Polygon)
        {
            unsigned int tmp = (unsigned int)(FdoDimensionality)*handle.ireader++;
            size_t cntContours = *handle.ireader++;
            handle.EnsureSpaceZM(tmp);
            int dim = FDODIM2NCOORDS(tmp);

            handle.shapes.push_back(new ShapeDescriptor());
            ShapeDescriptor* shape = handle.shapes.back();
            shape->figIndex = handle.figures.size();
            shape->parent = shpParentIdx;
            shape->type = OpenGISShapeType_Polygon;
            // copy all points and create all figures and shapes
            for (size_t idx = 0; idx < cntContours; idx++)
            {
                handle.figures.push_back(new FigureDescriptor());
                FigureDescriptor* pfig = handle.figures.back();
                pfig->ptIndex = handle.cntPoints;
                pfig->type = (idx == 0)? FigureType_ExteriorRing : FigureType_InteriorRing;

                size_t cntPts = (size_t)*handle.ireader++;
                double* dreader = (double*)handle.ireader;
#ifdef CHECK_CLOCKWISE
                bool normalCopy = true;
                if (handle.isLatLong)
                {
                    // ensure we have right clock wise
                    bool cw = handle.PointsAreClockwise(dim, cntPts, dreader);
                    normalCopy = !((cw && !idx) || (!cw && idx));
                }
                if (normalCopy)
                {
#endif
                    for (size_t ip = 0; ip < cntPts; ip++)
                    {
                        BUFF_WRITE_POINTHXYZM;
                        handle.cntPoints++;
                    }
#ifdef CHECK_CLOCKWISE
                }
                else
                {
                    double* enddreader = dreader + cntPts * dim;
                    dreader = enddreader;
                    for (size_t ip = 0; ip < cntPts; ip++)
                    {
                        dreader -= dim;
                        BUFF_WRITE_POINTHXYZM;
                        handle.cntPoints++;
                        dreader -= dim;
                    }
                    dreader = enddreader;
                }
#endif
                handle.ireader = (int*)dreader;
            }
        }
        else
            throw FdoException::Create(L"Invalid geometry");
    }

    void BuildCurvePolygon(GeomWriteHandle& handle, size_t shpParentIdx)
    {
        // TODO CHECK_CLOCKWISE in this case!?
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader++;
        if (geom_type == FdoGeometryType_CurvePolygon)
        {
            handle.hasArcs = true;
            unsigned int tmp = (unsigned int)(FdoDimensionality)*handle.ireader++;
            handle.EnsureSpaceZM(tmp);
            size_t cntContours = *handle.ireader++;

            handle.shapes.push_back(new ShapeDescriptor());
            ShapeDescriptor* shape = handle.shapes.back();
            shape->figIndex = handle.figures.size();
            shape->parent = shpParentIdx;
            shape->type = OpenGISShapeType_CurvePolygon;

            for (size_t ic = 0; ic < cntContours; ic++)
            {
                handle.figures.push_back(new FigureDescriptor());
                FigureDescriptor* pfig = handle.figures.back();
                pfig->ptIndex = handle.cntPoints;
                pfig->type = FigureType_CompositeCurve;
                double* dreader = (double*)handle.ireader;
                BUFF_WRITE_POINTHXYZM;
                handle.ireader = (int*)dreader;
                handle.cntPoints++;
                size_t cntParts = *handle.ireader++;            

                if (cntParts == 1 && cntContours == 1)
                {
                    if ((FdoGeometryComponentType)*handle.ireader == FdoGeometryComponentType_CircularArcSegment)
                        pfig->type = FigureType_ExteriorRing; // Arc
                    else
                        pfig->type = FigureType_Stroke; // line
                }

                // copy all points and create all figures and shapes
                bool firstArc = true;
                bool firstLine = true;
                for (size_t idx = 0; idx < cntParts; idx++)
                {
                    FdoGeometryComponentType tpArc = (FdoGeometryComponentType)*handle.ireader++;
                    switch (tpArc)
                    {
                    case FdoGeometryComponentType_CircularArcSegment:
                        {
                            dreader = (double*)handle.ireader;
                            BUFF_WRITE_POINTHXYZM;
                            handle.cntPoints++;
                            BUFF_WRITE_POINTHXYZM;
                            handle.cntPoints++;
                            handle.ireader = (int*)dreader;
                            handle.segments.push_back(firstArc ? SegmentType_FirstArc : SegmentType_Arc);
                            firstArc = false;
                        }
                        break;
                    case FdoGeometryComponentType_LineStringSegment:
                        {
                            size_t cntPts = (size_t)*handle.ireader++;
                            dreader = (double*)handle.ireader;
                            for (size_t ip = 0; ip < cntPts; ip++)
                            {
                                BUFF_WRITE_POINTHXYZM;
                                handle.cntPoints++;
                                handle.segments.push_back(firstLine ? SegmentType_FirstLine : SegmentType_Line);
                                firstLine = false;
                            }
                            handle.ireader = (int*)dreader;
                        }
                        break;
                    default:
                        throw FdoException::Create(L"Invalid geometry");
                    }
                }
            }
        }
        else
            throw FdoException::Create(L"Invalid geometry");
    }

    void BuildLinestring(GeomWriteHandle& handle, size_t shpParentIdx)
    {
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader++;
        if (geom_type == FdoGeometryType_LineString)
        {
            unsigned int tmp = (unsigned int)(FdoDimensionality)*handle.ireader++;
            handle.EnsureSpaceZM(tmp);

            handle.shapes.push_back(new ShapeDescriptor());
            ShapeDescriptor* shape = handle.shapes.back();
            shape->figIndex = handle.figures.size();
            shape->parent = shpParentIdx;
            shape->type = OpenGISShapeType_LineString;
            // copy all points and create all figures and shapes
            handle.figures.push_back(new FigureDescriptor());
            FigureDescriptor* pfig = handle.figures.back();
            pfig->ptIndex = handle.cntPoints;
            pfig->type = FigureType_Stroke;

            size_t cntPts = (size_t)*handle.ireader++;
            double* dreader = (double*)handle.ireader;
            for (size_t ip = 0; ip < cntPts; ip++)
            {
                BUFF_WRITE_POINTHXYZM;
                handle.cntPoints++;
            }
            handle.ireader = (int*)dreader;
        }
        else
            throw FdoException::Create(L"Invalid geometry");
    }

    void BuildCurveString(GeomWriteHandle& handle, size_t shpParentIdx)
    {
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader++;
        if (geom_type == FdoGeometryType_CurveString)
        {
            handle.hasArcs = true;
            unsigned int tmp = (unsigned int)(FdoDimensionality)*handle.ireader++;
            handle.EnsureSpaceZM(tmp);
            double* dreader = (double*)handle.ireader;
            BUFF_WRITE_POINTHXYZM;
            handle.ireader = (int*)dreader;
            handle.cntPoints++;
            size_t cntParts = *handle.ireader++;

            handle.shapes.push_back(new ShapeDescriptor());
            ShapeDescriptor* shape = handle.shapes.back();
            shape->figIndex = handle.figures.size();
            shape->parent = shpParentIdx;
            handle.figures.push_back(new FigureDescriptor());
            FigureDescriptor* pfig = handle.figures.back();
            pfig->ptIndex = handle.cntPoints-1;
            
            if (cntParts == 1 && (FdoGeometryComponentType)*handle.ireader == FdoGeometryComponentType_CircularArcSegment)
            {
                shape->type = OpenGISShapeType_CircularString;
                pfig->type = FigureType_ExteriorRing; // Arc
            }
            else
            {
                 shape->type = OpenGISShapeType_CompoundCurve;
                 pfig->type = FigureType_CompositeCurve;
            }
            // copy all points and create all figures and shapes
            bool firstArc = true;
            bool firstLine = true;
            for (size_t idx = 0; idx < cntParts; idx++)
            {
                FdoGeometryComponentType tpArc = (FdoGeometryComponentType)*handle.ireader++;
                switch (tpArc)
                {
                case FdoGeometryComponentType_CircularArcSegment:
                    {
                        dreader = (double*)handle.ireader;
                        BUFF_WRITE_POINTHXYZM;
                        handle.cntPoints++;
                        BUFF_WRITE_POINTHXYZM;
                        handle.cntPoints++;
                        handle.ireader = (int*)dreader;
                        if (shape->type == OpenGISShapeType_CompoundCurve)
                        {
                            handle.segments.push_back(firstArc ? SegmentType_FirstArc : SegmentType_Arc);
                            firstArc = false;
                        }
                    }
                    break;
                case FdoGeometryComponentType_LineStringSegment:
                    {
                        size_t cntPts = (size_t)*handle.ireader++;
                        dreader = (double*)handle.ireader;
                        for (size_t ip = 0; ip < cntPts; ip++)
                        {
                            BUFF_WRITE_POINTHXYZM;
                            handle.cntPoints++;
                            if (shape->type == OpenGISShapeType_CompoundCurve)
                            {
                                handle.segments.push_back(firstLine ? SegmentType_FirstLine : SegmentType_Line);
                                firstLine = false;
                            }
                        }
                        handle.ireader = (int*)dreader;
                    }
                    break;
                default:
                    throw FdoException::Create(L"Invalid geometry");
                }
            }
        }
        else
            throw FdoException::Create(L"Invalid geometry");
    }

    void BuildPoint(GeomWriteHandle& handle, size_t shpParentIdx)
    {
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader++;
        if (geom_type == FdoGeometryType_Point)
        {
            unsigned int tmp = (unsigned int)(FdoDimensionality)*handle.ireader++;
            handle.EnsureSpaceZM(tmp);

            handle.shapes.push_back(new ShapeDescriptor());
            ShapeDescriptor* shape = handle.shapes.back();
            shape->figIndex = handle.figures.size();
            shape->parent = shpParentIdx;
            shape->type = OpenGISShapeType_Point;
            // copy all points and create all figures and shapes
            handle.figures.push_back(new FigureDescriptor());
            FigureDescriptor* pfig = handle.figures.back();
            pfig->ptIndex = handle.cntPoints;
            pfig->type = FigureType_Stroke;

            double* dreader = (double*)handle.ireader;
            BUFF_WRITE_POINTHXYZM;
            handle.cntPoints++;
            handle.ireader = (int*)dreader;
        }
        else
            throw FdoException::Create(L"Invalid geometry");
    }

    void BuildGeometry(GeomWriteHandle& handle, size_t shpParentIdx, size_t vers)
    {
        FdoGeometryType geom_type = (FdoGeometryType)*handle.ireader;
        switch(geom_type)
        {
        case FdoGeometryType_LineString:
            BuildLinestring(handle, shpParentIdx);
            break;
        case FdoGeometryType_Point:
            BuildPoint(handle, shpParentIdx);
            break;
        case FdoGeometryType_Polygon:
            BuildPolygon(handle, shpParentIdx);
            break;
        case FdoGeometryType_MultiLineString:
        case FdoGeometryType_MultiPolygon:
        case FdoGeometryType_MultiPoint:
            {
                handle.ireader++;
                size_t cntGeoms = *handle.ireader++;
                handle.shapes.push_back(new ShapeDescriptor());
                ShapeDescriptor* parShp = handle.shapes.back();
                parShp->figIndex = handle.figures.size();
                parShp->parent = shpParentIdx;
                parShp->type = handle.FdoTypeToShapeType(geom_type);
                shpParentIdx = handle.shapes.size()-1;
                for (size_t idx = 0; idx < cntGeoms; idx++)
                {
                    switch(geom_type)
                    {
                    case FdoGeometryType_MultiLineString:
                        BuildLinestring(handle, shpParentIdx);
                        break;
                    case FdoGeometryType_MultiPolygon:
                        BuildPolygon(handle, shpParentIdx);
                        break;
                    case FdoGeometryType_MultiPoint:
                        BuildPoint(handle, shpParentIdx);
                        break;
                    default:
                        throw FdoException::Create(L"Invalid type!");
                    }
                }
            }
            break;
        case FdoGeometryType_MultiGeometry:
            {
                handle.ireader++;
                size_t cntGeoms = *handle.ireader++;
                handle.shapes.push_back(new ShapeDescriptor());
                ShapeDescriptor* parShp = handle.shapes.back();
                parShp->figIndex = handle.figures.size();
                parShp->parent = shpParentIdx;
                parShp->type = OpenGISShapeType_GeometryCollection;
                shpParentIdx = handle.shapes.size()-1;
                for (size_t idx = 0; idx < cntGeoms; idx++)
                    BuildGeometry(handle, shpParentIdx, vers);
            }
            break;
        case FdoGeometryType_CurveString:
        case FdoGeometryType_CurvePolygon:
            {
                if (vers == 0x01) {
                    throw FdoException::Create(L"Invalid type!");
                }
                else
                {
                    if (geom_type == FdoGeometryType_CurveString)
                        BuildCurveString(handle, shpParentIdx);
                    else
                        BuildCurvePolygon(handle, shpParentIdx);
                }
            }
            break;
        default:
            throw FdoException::Create(L"Invalid type!");
        }
    }

    FdoByte* ConvertFdoToMsGeometry(GeomWriteHandle& handle, FdoIGeometry* igeom, size_t vers, int srid, size_t* len)
    {
        handle.Clear();
        FdoFgfGeometryFactory* gf = handle.GetGeometryFactory();
        FdoPtr<FdoByteArray> fdoGeom = gf->GetFgf(igeom);
        const FdoByte* fgf = fdoGeom->GetData();
        FdoInt32 szData = fdoGeom->GetCount();
        int* ireader = (int*)fgf;
        // the geometry type
        FdoGeometryType geom_type = (FdoGeometryType)*ireader;
        handle.isLatLong = (srid >= 4120 && srid <= 4999);

        switch (geom_type)
        {
        case FdoGeometryType_Point:
            {
                ireader++;
                unsigned int tmp = (unsigned int)(FdoDimensionality)*ireader++;
                handle.EnsureLenBuffer(FDODIM2NCOORDS(tmp)*sizeof(double)+GEOM_HEADER_SIZE);
                handle.lBuff = handle.Buffer();
                BUFF_PUSH_LEINT(handle.lBuff, srid);
                BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version
                FdoByte type = VALIDGEOM_FLAG|PTGEOM_FLAG;
                double* dreader = (double*)ireader;
                if ((tmp&FdoDimensionality_Z) != 0)
                {
                    // we have Z for FDO geometry however we need to see if we have Z=NaN (in case yes avoid using it)
                    // this is valid for points only
                    handle.hasZ = true;
                    if (!handle.IsNan(*(dreader+2)))
                        type |= ZGEOM_FLAG;
                }
                if ((tmp&FdoDimensionality_M) != 0)
                {
                    // we have M for FDO geometry however we need to see if we have M=NaN (in case yes avoid using it)
                    // this is valid for points only
                    handle.hasM = true;
                    if (!handle.IsNan(*(dreader+2+(int)handle.hasZ)))
                        type |= MGEOM_FLAG;
                }
                BUFF_PUSH_BYTE(handle.lBuff, type);
                if (handle.isLatLong)
                {
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian((dreader+1)));
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader));
                    dreader += 2;
                }
                else
                {
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++));
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++));
                }
                if ((tmp&FdoDimensionality_Z) != 0)
                {
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++));
                }
                else if (handle.hasZ)
                    dreader++; // jump over Z in case is NaN

                if ((tmp&FdoDimensionality_M) != 0)
                    BUFF_PUSH_INT64(handle.lBuff, DbToLittleEndian(dreader++));

                handle.MarkBufferLen(handle.lBuff);
            }
            break;
        case FdoGeometryType_LineString:
            {
                unsigned int tmp = (unsigned int)(FdoDimensionality)ireader[1];
                if (ireader[2] == 2) // special case 2 points line
                {
                    handle.EnsureLenBuffer(ireader[2]*FDODIM2NCOORDS(tmp)*sizeof(double)+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(0, 0));
                    handle.lBuff = handle.Buffer();
                    handle.evalCntPts = 2;
                    handle.EnsureSpaceZM(tmp);
                    BUFF_PUSH_LEINT(handle.lBuff, srid);
                    BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version
                    FdoByte type = VALIDGEOM_FLAG|LNGEOM_FLAG;
                    if ((tmp&FdoDimensionality_Z) != 0)
                        type |= ZGEOM_FLAG;
                    if ((tmp&FdoDimensionality_M) != 0)
                        type |= MGEOM_FLAG;
                    BUFF_PUSH_BYTE(handle.lBuff, type);
                    double* dreader = (double*)(ireader+3);
                    BUFF_WRITE_POINTHXYZM;
                    handle.cntPoints++;
                    BUFF_WRITE_POINTHXYZM;
                    handle.cntPoints++;
                    handle.WriteZMPointsIfNeeed();
                    handle.MarkBufferLen(handle.lBuff);
                }
                else
                {
                    // one shape and one figures
                    handle.Reserve (1, 1);
                    handle.ireader = ireader;
                    handle.evalCntPts = ireader[2];
                    handle.EnsureLenBuffer(ireader[2]*FDODIM2NCOORDS(tmp)*sizeof(double)+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(1, 1));
                    handle.lBuff = handle.Buffer();
                    BUFF_PUSH_LEINT(handle.lBuff, srid);
                    BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version

                    FdoInt64 offType = handle.lBuff-handle.Buffer(); // remember offset of type address
                    BUFF_PUSH_BYTE(handle.lBuff, VALIDGEOM_FLAG);
                    FdoInt64 offCntPts = handle.lBuff-handle.Buffer(); // remember offset of count points
                    BUFF_PUSH_LEINT(handle.lBuff, handle.cntPoints); // number of points - update it later

                    BuildLinestring(handle, NEG_ONE_LE);
                    handle.WriteZMPointsIfNeeed();
                    handle.WriteFooterGeom();

                    FdoByte* tmpBuff = handle.Buffer()+offType;
                    *tmpBuff |= (handle.hasZ?ZGEOM_FLAG:0x00) | (handle.hasM?MGEOM_FLAG:0x00);
                    tmpBuff = handle.Buffer()+offCntPts;
                    BUFF_PUSH_LEINT(tmpBuff, handle.cntPoints); // update number of points
                    handle.MarkBufferLen(handle.lBuff);
                }
            }
            break;
        case FdoGeometryType_Polygon:
            {
                // one shape and ireader[2] figures
                handle.Reserve (ireader[2], 1);
                handle.ireader = ireader;
                unsigned int tmp = (unsigned int)(FdoDimensionality)ireader[1];
                handle.evalCntPts = (size_t)(szData/(FDODIM2NCOORDS(tmp)*sizeof(double)));
                handle.EnsureLenBuffer(handle.evalCntPts*FDODIM2NCOORDS(tmp)*sizeof(double)+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(ireader[2], 1));
                handle.lBuff = handle.Buffer();
                BUFF_PUSH_LEINT(handle.lBuff, srid);
                BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version
                
                FdoInt64 offType = handle.lBuff-handle.Buffer(); // remember offset of type address
                BUFF_PUSH_BYTE(handle.lBuff, VALIDGEOM_FLAG);
                FdoInt64 offCntPts = handle.lBuff-handle.Buffer(); // remember offset of count points
                BUFF_PUSH_LEINT(handle.lBuff, handle.cntPoints); // number of points - update it later

                BuildPolygon(handle, NEG_ONE_LE);
                handle.WriteZMPointsIfNeeed();
                handle.WriteFooterGeom();

                FdoByte* tmpBuff = handle.Buffer()+offType;
                *tmpBuff |= (handle.hasZ?ZGEOM_FLAG:0x00) | (handle.hasM?MGEOM_FLAG:0x00);
                tmpBuff = handle.Buffer()+offCntPts;
                BUFF_PUSH_LEINT(tmpBuff, handle.cntPoints); // update number of points
                handle.MarkBufferLen(handle.lBuff);
            }
            break;
        case FdoGeometryType_MultiPoint:
        case FdoGeometryType_MultiPolygon:
        case FdoGeometryType_MultiLineString:
            {
                ireader++;
                size_t cntGeoms = *ireader++;
                // one ireader[1]+1 shapes and ireader[1] figures
                handle.Reserve(cntGeoms, cntGeoms + 1);
                handle.ireader = ireader;
                // we need to allocate more since we have no idea how many points we have
                handle.evalCntPts = (size_t)(szData/(2*sizeof(double)));
                // for MultiPolygon we need to allocate a bit more since we can have more figures and shapes
                handle.EnsureLenBuffer((size_t)(szData*(geom_type==FdoGeometryType_MultiPolygon?1.2:1.0))+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(cntGeoms, cntGeoms+1));
                handle.lBuff = handle.Buffer();
                BUFF_PUSH_LEINT(handle.lBuff, srid);
                BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version
                
                FdoInt64 offType = handle.lBuff-handle.Buffer(); // remember offset of type address
                BUFF_PUSH_BYTE(handle.lBuff, VALIDGEOM_FLAG);
                FdoInt64 offCntPts = handle.lBuff-handle.Buffer(); // remember offset of count points
                BUFF_PUSH_LEINT(handle.lBuff, handle.cntPoints); // number of points - update it later
                
                handle.shapes.push_back(new ShapeDescriptor());
                ShapeDescriptor* parShp = handle.shapes.back();
                parShp->figIndex = 0;
                parShp->type = handle.FdoTypeToShapeType(geom_type);
                for (size_t idx = 0; idx < cntGeoms; idx++)
                {
                    switch(geom_type)
                    {
                    case FdoGeometryType_MultiLineString:
                        BuildLinestring(handle, 0);
                        break;
                    case FdoGeometryType_MultiPolygon:
                        BuildPolygon(handle, 0);
                        break;
                    case FdoGeometryType_MultiPoint:
                        BuildPoint(handle, 0);
                        break;
                    default:
                        throw FdoException::Create(L"Invalid type!");
                    }
                }

                handle.WriteZMPointsIfNeeed();
                handle.WriteFooterGeom();

                FdoByte* tmpBuff = handle.Buffer()+offType;
                *tmpBuff |= (handle.hasZ?ZGEOM_FLAG:0x00) | (handle.hasM?MGEOM_FLAG:0x00);
                tmpBuff = handle.Buffer()+offCntPts;
                BUFF_PUSH_LEINT(tmpBuff, handle.cntPoints); // update number of points
                handle.MarkBufferLen(handle.lBuff);
            }
            break;
        // MS do not support MultiCurveString & MultiCurvePolygon so we generate geom coll
        case FdoGeometryType_MultiCurveString:
        case FdoGeometryType_MultiCurvePolygon:
        case FdoGeometryType_MultiGeometry:
            {
                if (geom_type != FdoGeometryType_MultiGeometry && vers == 0x01) {
                    throw FdoException::Create(L"Invalid geometry!");
                }
                else
                {
                    ireader++;
                    size_t cntGeoms = *ireader++;
                    // one ireader[1]+1 shapes and ireader[1] figures
                    handle.Reserve(cntGeoms, cntGeoms + 1);
                    handle.ireader = ireader;
                    // we need to allocate more since we have no idea how many points we have
                    handle.evalCntPts = (size_t)(szData/(2*sizeof(double)));
                    // for MultiPolygon we need to allocate a bit more since we can have more figures and shapes
                    handle.EnsureLenBuffer((size_t)(szData*1.3)+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(cntGeoms, cntGeoms+1));
                    handle.lBuff = handle.Buffer();
                    BUFF_PUSH_LEINT(handle.lBuff, srid);
                    FdoInt64 offVers = handle.lBuff-handle.Buffer(); // remember offset of version
                    BUFF_PUSH_BYTE(handle.lBuff, 0x01); // version
                    
                    FdoInt64 offType = handle.lBuff-handle.Buffer(); // remember offset of type address
                    BUFF_PUSH_BYTE(handle.lBuff, VALIDGEOM_FLAG);
                    FdoInt64 offCntPts = handle.lBuff-handle.Buffer(); // remember offset of count points
                    BUFF_PUSH_LEINT(handle.lBuff, handle.cntPoints); // number of points - update it later
                    
                    handle.shapes.push_back(new ShapeDescriptor());
                    ShapeDescriptor* parShp = handle.shapes.back();
                    parShp->figIndex = handle.figures.size();
                    parShp->type = OpenGISShapeType_GeometryCollection;
                    for (size_t idx = 0; idx < cntGeoms; idx++)
                        BuildGeometry(handle, 0, vers);

                    // in case we have arcs update the version
                    FdoByte* tmpBuff = handle.Buffer()+offVers;
                    if (handle.hasArcs)
                    {
                        BUFF_PUSH_BYTE(tmpBuff, 0x02); // version
                    }

                    handle.WriteZMPointsIfNeeed();
                    handle.WriteFooterGeom();

                    tmpBuff = handle.Buffer()+offType;
                    *tmpBuff |= (handle.hasZ?ZGEOM_FLAG:0x00) | (handle.hasM?MGEOM_FLAG:0x00);
                    tmpBuff = handle.Buffer()+offCntPts;
                    BUFF_PUSH_LEINT(tmpBuff, handle.cntPoints); // update number of points
                    handle.MarkBufferLen(handle.lBuff);
                }
            }
            break;
        case FdoGeometryType_CurveString:
        case FdoGeometryType_CurvePolygon:
            {
                if (vers == 0x01)
                {
                    throw FdoException::Create(L"Invalid geometry!");
                }
                else
                {
                    // one shape and one figures
                    handle.Reserve (1, 1);
                    handle.ireader = ireader;
                    handle.evalCntPts = (size_t)(szData/(2*sizeof(double)));
                    // for MultiPolygon we need to allocate a bit more since we can have more figures and shapes
                    handle.EnsureLenBuffer((size_t)(szData*1.1)+GEOM_HEADER_SIZE+GEOM_FOOTER_SIZE(1, 1));
                    handle.lBuff = handle.Buffer();
                    BUFF_PUSH_LEINT(handle.lBuff, srid);
                    BUFF_PUSH_BYTE(handle.lBuff, vers); // version

                    FdoInt64 offType = handle.lBuff-handle.Buffer(); // remember offset of type address
                    BUFF_PUSH_BYTE(handle.lBuff, VALIDGEOM_FLAG);
                    FdoInt64 offCntPts = handle.lBuff-handle.Buffer(); // remember offset of count points
                    BUFF_PUSH_LEINT(handle.lBuff, handle.cntPoints); // number of points - update it later

                    if (geom_type == FdoGeometryType_CurveString)
                        BuildCurveString(handle, NEG_ONE_LE);
                    else
                        BuildCurvePolygon(handle, NEG_ONE_LE);

                    handle.WriteZMPointsIfNeeed();
                    handle.WriteFooterGeom();

                    FdoByte* tmpBuff = handle.Buffer()+offType;
                    *tmpBuff |= (handle.hasZ?ZGEOM_FLAG:0x00) | (handle.hasM?MGEOM_FLAG:0x00);
                    tmpBuff = handle.Buffer()+offCntPts;
                    BUFF_PUSH_LEINT(tmpBuff, handle.cntPoints); // update number of points
                    handle.MarkBufferLen(handle.lBuff);
                }
            }
            break;
        default:
            throw FdoException::Create(L"Invalid geometry!");
            break;
        }
        *len = handle.BufferLen();
        return handle.Buffer();
    }

    typedef struct GeometryHandles
    {
        GeomReadHandle* readHandle;
        GeomWriteHandle* writeHandle;

        GeometryHandles()
        {
            readHandle = NULL;
            writeHandle = NULL;
        }
        ~GeometryHandles()
        {
            delete readHandle;
            delete writeHandle;
        }
    } GeometryHandles;
};


void* IGeometry_CreateGeometryHandleConvertor()
{
    return new sqlgeomconv::GeometryHandles();
}

void IGeometry_ReleaseGeometryHandleConvertor(void* handle)
{
    sqlgeomconv::GeometryHandles* ptr = (sqlgeomconv::GeometryHandles*)handle;
    delete ptr;
}

///////////////////////////////////////////////////////////////////////////
int IGeometry_CreateGeometry( pIGeometry_def geometry, pIGeometry_def *geomOut)
{
    FdoFgfGeometryFactory*  gf = 0;
  
    int  rc = IGeometry_CheckNullArg2( (void **)geomOut, true ) && 
              IGeometry_CheckNullArg( geometry, true );
 
    if ( rc )
    {
        try
        {
            *geomOut = 0;
            FdoIGeometry * g = (FdoIGeometry *) geometry;
            FdoGeometryType type;

            rc = IGeometry_GetDerivedType( geometry, &type );

            if ( rc )
            {
                if (  type == FdoGeometryType_None )
                {
                    IGeometry_AddRef( geometry );
                    *geomOut = geometry;
                }
                else 
                {
                    gf = FdoFgfGeometryFactory::GetInstance();
                    *geomOut = (pIGeometry_def) gf->CreateGeometry( g ); 
                }
            }
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

     return rc;
}

///////////////////////////////////////////////////////////////////////////
// The caller should check for NULL return value
///////////////////////////////////////////////////////////////////////////
pByteArray_def IByteArray_Create(unsigned char* bytes, int count)
{
    FdoByteArray *da = 0;

    try 
    {
        da = FdoByteArray::Create(bytes, count);
    }
    catch (FdoException *ex) 
    {
        ex->Release();
    }
    return (pByteArray_def)da;
}

void IByteArray_Release(pByteArray_def baIn)
{
    FdoByteArray* ba = (FdoByteArray *) baIn;
    if ( ba != 0 )
        ba->Release();

}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetMsWkb(void* handle, pIGeometry_def geometry, int srid, int vers, pByteArray_def *baOut )
{
    sqlgeomconv::GeometryHandles* ptr = (sqlgeomconv::GeometryHandles*)handle;

    if ((void **)baOut == NULL)
        return 0;

    *baOut = NULL;
    FdoIGeometry* g = (FdoIGeometry*) geometry;
    if (g != NULL)
    {
        if (ptr->writeHandle == NULL)
            ptr->writeHandle = new sqlgeomconv::GeomWriteHandle();

        try
        {
            size_t cnt = 0;
            unsigned char* data = sqlgeomconv::ConvertFdoToMsGeometry(*ptr->writeHandle, g, (size_t)vers, srid, &cnt);
            if (cnt != 0)
                *baOut = (pByteArray_def)FdoByteArray::Create(data, (FdoInt32)cnt);
            return true;
        }
        catch (FdoException *ex) 
        {
            ex->Release();
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetFgf( pIGeometry_def geometry, pByteArray_def *baOut )
{
    FdoFgfGeometryFactory*  gf = 0;

    int  rc = IGeometry_CheckNullArg2( (void **)baOut, true );

    *baOut = 0;
    if ( rc )
    {
        try
        {
            if ( geometry != 0 )
            {
                FdoIGeometry * g = (FdoIGeometry *) geometry;
                gf = FdoFgfGeometryFactory::GetInstance();
                
                *baOut = (pByteArray_def) gf->GetFgf( g ); 
            }
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetByteArrayData( pByteArray_def ba, unsigned char **data, int *size )
{
    int  rc = IGeometry_CheckNullArg2( (void **)data, true ) &&
              IGeometry_CheckNullArg( (void *)size, true );  

    *data = 0;
    if ( rc && ba != 0 )
    {
        FdoByteArray *ba2 = (FdoByteArray *) ba;
        *data = ba2->GetData();
        *size = ba2->GetCount();
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int  IGeometry_CreateGeometryFromMsWkb(void* handle, pByteArray_def ba, pIGeometry_def *geomOut, pIGeometry_def *geomOutCopy )
{
    sqlgeomconv::GeometryHandles* ptr = (sqlgeomconv::GeometryHandles*)handle;
  
    if ((void **)geomOut == NULL || ptr == NULL)
        return false;
 
    try
    {
        *geomOut = 0;
        *geomOutCopy = 0;
        FdoByteArray* arr = (FdoByteArray*)ba;
        if (arr != NULL)
        {
            if (ptr->readHandle == NULL)
                ptr->readHandle = new sqlgeomconv::GeomReadHandle();

            *geomOut = (pIGeometry_def)sqlgeomconv::ConvertMsToFdoGeometry(ptr->readHandle, arr->GetData(), arr->GetCount());
        }
		if ( geomOutCopy )
			*geomOutCopy = *geomOut;
        return true;
    }
    catch (FdoException *ex) 
    {
        ex->Release();
    }
    return false;   
}

///////////////////////////////////////////////////////////////////////
int  IGeometry_CreateGeometryFromFgf( pByteArray_def ba, pIGeometry_def *geomOut, pIGeometry_def *geomOutCopy )
{
    FdoFgfGeometryFactory*  gf = 0;
  
    int  rc = IGeometry_CheckNullArg2( (void **)geomOut, true );
 
    if ( rc )
    {
        try
        {
            *geomOut = 0;
            if ( ba != NULL )
            {
                gf = FdoFgfGeometryFactory::GetInstance();
                *geomOut = (pIGeometry_def) gf->CreateGeometryFromFgf( (FdoByteArray *)ba ); 
            }
			if ( geomOutCopy )
			{
				*geomOutCopy = *geomOut;
			}
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

     return rc;
   
}

///////////////////////////////////////////////////////////////////////
int IGeometry_AddRef(pIGeometry_def geometry)
{
    int    rc = IGeometry_CheckNullArg( geometry, true );

    if ( rc )
    {
	    FdoIGeometry * g = (FdoIGeometry *) geometry;
	    g->AddRef();
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
FdoInt32 IGeometry_Release( pIGeometry_def geometry)
{
    FdoInt32 refs = 0;

    if ( geometry != 0 )
    {
	    FdoIGeometry *g = (FdoIGeometry *) geometry;
        if (g != 0)
        {
	        refs = g->Release();
        }
    }
    return refs;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetFgftString( pIGeometry_def geometry, const wchar_t **akwtOut )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) && 
                IGeometry_CheckNullArg2( (void **) akwtOut, true );

    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
            *akwtOut = g->GetText();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetDerivedType( pIGeometry_def geometry, FdoGeometryType *geomType )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) &&
                IGeometry_CheckNullArg( geomType, true );
    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
	        *geomType = g->GetDerivedType();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetDimensionality( pIGeometry_def geometry, int *geomDim )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) &&
                IGeometry_CheckNullArg( geomDim, true );

    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
	        *geomDim = g->GetDimensionality();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_CheckNullArg( void* arg, int report )
{
    return ( arg != 0 );
}

///////////////////////////////////////////////////////////////////////
int IGeometry_CheckNullArg2( void** arg, int report )
{
    return ( arg != 0 );
}

