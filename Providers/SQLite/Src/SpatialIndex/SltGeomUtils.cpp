// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include "SltGeomUtils.h"
#include <math.h>

#ifdef LINUX
	#include <algorithm>
	using namespace std;
#endif // LINUX

//For reference, duplicated from ogr_geometry.h
///**
// * List of well known binary geometry types.  These are used within the BLOBs
// * but are also returned from OGRGeometry::getGeometryType() to identify the
// * type of a geometry object.
// */
//
//typedef enum 
//{
//    wkbUnknown = 0,             /* non-standard */
//    wkbPoint = 1,               /* rest are standard WKB type codes */
//    wkbLineString = 2,
//    wkbPolygon = 3,
//    wkbMultiPoint = 4,
//    wkbMultiLineString = 5,
//    wkbMultiPolygon = 6,
//    wkbGeometryCollection = 7,
//    wkbNone = 100,              /* non-standard, for pure attribute records */
//    wkbLinearRing = 101,        /* non-standard, just for createGeometry() */
//    wkbPoint25D = 0x80000001,   /* 2.5D extensions as per 99-402 */
//    wkbLineString25D = 0x80000002,
//    wkbPolygon25D = 0x80000003,
//    wkbMultiPoint25D = 0x80000004,
//    wkbMultiLineString25D = 0x80000005,
//    wkbMultiPolygon25D = 0x80000006,
//    wkbGeometryCollection25D = 0x80000007
//} OGRwkbGeometryType;
//
//#define wkb25DBit 0x80000000
//#define wkbFlatten(x)  ((OGRwkbGeometryType) ((x) & (~wkb25DBit)))
//
//#define ogrZMarker 0x21125711
//
//const char CPL_DLL * OGRGeometryTypeToName( OGRwkbGeometryType eType );
//
//typedef enum 
//{
//    wkbXDR = 0,         /* MSB/Sun/Motoroloa: Most Significant Byte First   */
//    wkbNDR = 1          /* LSB/Intel/Vax: Least Significant Byte First      */
//} OGRwkbByteOrder;

struct ArcSegmentDetails
{
    bool isCounterClockWise;
    bool isCircle;
    double center[2];
    double length;
    double radius;
    double thetaEndAngle;
    double thetaStartAngle;
};

double ComputeLength(int point_count, unsigned int dim, const double *ordinates, bool computeGeodetic);
double ComputeLengthWithStartPoint(int point_count, unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic);
double ComputeCircularArcSegmentLength(unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic);
double ComputeGeodeticDistance2D(double lat1, double lon1, double lat2, double lon2);
bool IsDirectionCounterClockWise(const double *startpoint, const double *midpoint, const double *endpoint);
bool ComputeCenterFromThreePositions(unsigned int dim, const double *startpoint, const double *ordinates, double* center);
void SkipGeometry(const unsigned char** inputStream);
double ComputeLinearRingArea(int point_count, unsigned int dim, const double *ordinates, bool computeGeodetic, bool isExterior);
double ComputeLinearRingAreaWithStartPoint(int point_count, unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic);
bool GetCircularArcSegmentDetails(unsigned int dim, const double *startpoint, const double *ordinates, ArcSegmentDetails* details);
double ComputeUsingTesselateArcSegment(unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic, bool isAreaCalculation);

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

class BinaryReader
{
public:
    BinaryReader(const unsigned char* stream)
        : m_stream((unsigned char*)stream),
        m_bigEndian(0)
    {
    }

    inline unsigned char ReadByte()
    {
        return *m_stream++;
    }

    inline int ReadInt()
    {
        if (!m_bigEndian)
        {
            int ret = *(int*)m_stream;
            m_stream += sizeof(int);
            return ret;
        }
        else
        {
            int ret;
            unsigned char* cret = (unsigned char*)&ret;
            cret[3] = *m_stream++;
            cret[2] = *m_stream++;
            cret[1] = *m_stream++;
            cret[0] = *m_stream++;
            return ret;
        }
    }

    inline double ReadDouble()
    {
        if (!m_bigEndian)
        {
            double ret = *(double*)m_stream;
            m_stream += sizeof(double);
            return ret;
        }
        else
        {
            //there exists a faster way to do this...
            double ret;
            unsigned char* cret = (unsigned char*)&ret;
            cret[7] = *m_stream++;
            cret[6] = *m_stream++;
            cret[5] = *m_stream++;
            cret[4] = *m_stream++;
            cret[3] = *m_stream++;
            cret[2] = *m_stream++;
            cret[1] = *m_stream++;
            cret[0] = *m_stream++;
            return ret;
        }
    }

    void SetBigEndian(int isBigEndian)
    {
        m_bigEndian = isBigEndian;
    }

    int IsBigEndian() { return m_bigEndian; }

    unsigned char*& RawStreamPtr() { return m_stream; }
private:
    int m_bigEndian;
    unsigned char* m_stream;
};


class BinaryWriter
{
public:
    BinaryWriter(unsigned char* stream /*, int len*/)
        : m_stream(stream),
        m_begin(stream)
    {
    }

    inline void WriteByte(unsigned char b)
    {
        *m_stream++ = b;
    }

    inline void WriteInt(int i)
    {
        *((int*)m_stream) = i;
        m_stream += sizeof(int);
    }

    inline void WriteDouble(double d)
    {
        *((double*)m_stream) = d;
        m_stream += sizeof(double);
    }

    inline void WriteDoubles(BinaryReader& rdr, int ndoubles)
    {
        if (rdr.IsBigEndian())
        {
            for (int j=0; j<ndoubles; j++) WriteDouble(rdr.ReadDouble());
        }
        else
        {
            double * pts = (double*)m_stream;
            double * src = (double*)rdr.RawStreamPtr();
            
            for (int i=0; i<ndoubles; i++)
                *pts++ = *src++;

            m_stream = (unsigned char*)pts;
            rdr.RawStreamPtr() += ndoubles * sizeof(double);
        }
    }

    inline int GetLength() 
    {
        return (int)(m_stream - m_begin);
    }

    unsigned char* m_begin;
    unsigned char* m_stream;
};



int Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf)
{
    BinaryReader src(wkb);
    BinaryWriter dst(fgf);

    unsigned char byte_order = src.ReadByte(); 
    src.SetBigEndian(!byte_order); //0=BE, 1=LE

    // the geometry type
    int geom_type = src.ReadInt();

    // the coordinate type
    int skip = (geom_type < 0) ? 1 : 0; //0=XY, 1=XYZ or XYM, 2 = XYZM
    geom_type = geom_type & 0x7FFFFFFF; //now convert to FGF geom type

    dst.WriteInt(geom_type); //strip out WKB 2.5D indicator (FDO doesn't use that)

    bool is_multi =  (geom_type == FdoGeometryType_MultiLineString)
                  || (geom_type == FdoGeometryType_MultiPolygon
                  || (geom_type == FdoGeometryType_MultiPoint));

    int num_geoms = 1;

    //in case of multipolygon or multilinestring or multipoint,
    //read poly or linestring count
    if (is_multi) 
    {
        num_geoms = src.ReadInt();
        dst.WriteInt(num_geoms);
    }

    for (int q=0; q<num_geoms; q++)
    {
        if (is_multi)
        {
            //skip byte order
            src.ReadByte();

            //geom type
            geom_type = src.ReadInt();
            skip = geom_type < 0 ? 1 : 0; //is it 2.5D
            geom_type &= 0x7FFFFFFF;
            dst.WriteInt(geom_type);
        }

        //read cordinate type
        int dim = skip ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY;
        dst.WriteInt(dim);

        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings

        if ((geom_type == FdoGeometryType_Polygon)
            || (geom_type == FdoGeometryType_MultiPolygon))
        {
            contour_count = src.ReadInt();
            dst.WriteInt(contour_count);
        }

        for (int i=0; i<contour_count; i++)
        {
            int point_count = 1;

            //point geoms do not have a point count, since
            //each piece is just one point each
            if ((geom_type != FdoGeometryType_MultiPoint)
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = src.ReadInt();
                dst.WriteInt(point_count);
            }
  
            int numd = point_count * (skip + 2);
            dst.WriteDoubles(src, numd);
        }
    }

    return dst.GetLength();
}


int Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb)
{
    BinaryReader src(fgf);
    BinaryWriter dst((unsigned char*)wkb);

    dst.WriteByte(1); //NDR -- little endian Intel

    // the geometry type
    int geom_type = src.ReadInt();

    //we will need to do a second pass to set the 3D flag on this once we read the FDO dimensionality flag
    dst.WriteInt(geom_type); 

    //The FDO constants match WKB, so we don't really need to use the OGR wkb defines
    bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
        || (geom_type == FdoGeometryType_MultiPolygon
        || (geom_type == FdoGeometryType_MultiPoint));

    int num_geoms = 1;

    //in case of multipolygon or multilinestring or multipoint,
    //read poly or linestring count
    if (is_multi) 
    {
        num_geoms = src.ReadInt();
        dst.WriteInt(num_geoms);
    }

    for (int q=0; q<num_geoms; q++)
    {
        if (is_multi)
        {
            //set byte order
            dst.WriteByte(1);

            //geom type
            geom_type = src.ReadInt();
            dst.WriteInt(geom_type);
        }

        //read coordinate type
        int dim = src.ReadInt();
        int skip = ((dim & FdoDimensionality_Z) != 0) ? 1 : 0;

        if (skip)
        {
            //roll back and set the geom_type to wkb 2.5D
            //TODO: do we need to set both the geom type for the parent multi geom
            //and the child geom? For example in the case of a multipolygon, do we need to 
            //set it for both the current polygon and the parent multipolygon?
            *(int*)(dst.m_begin+1) = geom_type | 0x80000000; 
        }
        
        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings

        if ((geom_type == FdoGeometryType_Polygon)
            || (geom_type == FdoGeometryType_MultiPolygon))
        {
            contour_count = src.ReadInt();
            dst.WriteInt(contour_count);
        }

        for (int i=0; i<contour_count; i++)
        {
            int point_count = 1;

            //point geoms do not have a point count, since
            //each piece is just one point each
            if ((geom_type != FdoGeometryType_MultiPoint)
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = src.ReadInt();
                dst.WriteInt(point_count);
            }

            int numd = point_count * (skip + 2);
            for (int j=0; j<numd; j++) dst.WriteDouble(src.ReadDouble());
        }
    }

    return dst.GetLength();
}

//Converts FdoDimensionality value to # of coordinates per point
#define FDODIM2NCOORDS(x) (x + 2 - x/2)

void GetFgfExtents(const unsigned char* fgf, int len, double ext[4])
{ 
    int* ireader = (int*)fgf;

    // the geometry type
    int geom_type = (FdoGeometryType) ireader[0];

    switch (geom_type)
    {
    case FdoGeometryType_Point :
        {
            FgfPoint* pt = (FgfPoint*)fgf;
            AssignBounds(ext, pt->coords);
        }
        break;
    case FdoGeometryType_LineString :
        {
            FgfPolyline* pl = (FgfPolyline*)fgf;
            unsigned int tmp = pl->dim;
            unsigned int dim = FDODIM2NCOORDS(tmp);

            //Add the points to the bounds
            AddToEmptyExtent(pl->np, dim, pl->p, ext);
        }
        break;
    case FdoGeometryType_Polygon :
        {
            //read cordinate type
            unsigned int tmp = (unsigned int)(FdoDimensionality) ireader[1];
            unsigned int dim = FDODIM2NCOORDS(tmp);

            // the number of contours in current polygon/linestring
            int contour_count = ireader[2];

            //common case of a single contour
            if (contour_count == 1)
            {
                int point_count = ireader[3];
                double* dreader = (double*)&ireader[4];

                AddToEmptyExtent(point_count, dim, dreader, ext);
            }
            else
            {
                ireader += 3; //skip past geom_type, dimensionality, and contour count

                ext[0] = ext[1] = DBL_MAX;
                ext[2] = ext[3] = -DBL_MAX;

                for (int i=0; i<contour_count; i++)
                {
                    int point_count = *ireader++;
                    double* dreader = (double*) ireader;
                    AddToExtent(point_count, dim, dreader, ext);
                    ireader = (int*)(dreader + point_count * dim);
                }
            }
        }
        break;
    case FdoGeometryType_MultiLineString :
    case FdoGeometryType_MultiPolygon :
    case FdoGeometryType_MultiPoint :
        {
            ireader++;
            ext[0] = ext[1] = DBL_MAX;
            ext[2] = ext[3] = -DBL_MAX;

            bool is_multi = true; /*(geom_type == FdoGeometryType_MultiLineString)
                || (geom_type == FdoGeometryType_MultiPolygon
                || (geom_type == FdoGeometryType_MultiPoint));*/

            //temp variables used inside the loop
            int num_geoms = 1;
            int point_count;
            int contour_count;

            //in case of multipolygon or multilinestring or multipoint, 
            //read poly or linestring count
            if (is_multi) num_geoms = *ireader++;

            for (int q=0; q<num_geoms; q++)
            {
                //skip past geometry type of subgeometry
                //we know it is LineString or Polygon or Point respectively
                if (is_multi) ireader++;

                //read cordinate type
                unsigned int tmp = (FdoDimensionality) *ireader++;
                unsigned int dim = FDODIM2NCOORDS(tmp);

                // the number of contours in current polygon/linestring
                contour_count = 1; //for linestrings

                switch (geom_type)
                {
                //case FdoGeometryType_Polygon:
                case FdoGeometryType_MultiPolygon:
                    contour_count = *ireader++;
                default: break;
                }

                for (int i=0; i<contour_count; i++)
                {
                    point_count = 1;

                    //point geoms do not have a point count, since 
                    //each piece is just one point each
                    if (geom_type != FdoGeometryType_MultiPoint)
                        point_count = *ireader++;
                    
                    double* dreader = (double*) ireader;
                    AddToExtent(point_count, dim, dreader, ext);
                    ireader = (int*)(dreader + point_count * dim);
                }
            }
        }

        break;

    case FdoGeometryType_CurvePolygon: //all the *Curve* types
    case FdoGeometryType_CurveString:
    case FdoGeometryType_MultiCurveString:
    case FdoGeometryType_MultiCurvePolygon:
        {
            ireader++;
            ext[0] = ext[1] =  DBL_MAX;
            ext[2] = ext[3] = -DBL_MAX;

            bool is_multi = (geom_type == FdoGeometryType_MultiCurveString)
                || (geom_type == FdoGeometryType_MultiCurvePolygon);

            //temp variables used inside the loop
            int num_geoms = 1;
            int part_count;
            int contour_count;

            //in case of multipolygon or multilinestring or multipoint, 
            //read poly or linestring count
            if (is_multi) num_geoms = *ireader++;

            for (int q=0; q<num_geoms; q++)
            {
                //skip past geometry type of subgeometry
                //we know it is CurveString or CurvePolygon or Point respectively
                if (is_multi) ireader++;

                //read cordinate type
                int tmp = (FdoDimensionality) *ireader++;
                int dim = FDODIM2NCOORDS(tmp);

                // the number of contours in current curvepolygon/curvestring
                contour_count = 1; //for curvestrings

                switch (geom_type)
                {
                case FdoGeometryType_CurvePolygon:
                case FdoGeometryType_MultiCurvePolygon:
                    contour_count = *ireader++;
                default: break;
                }

                for (int i=0; i<contour_count; i++)
                {
                    //read the startpoint of the curve geometry, it's stored
                    //separately, before the other points
                    double* startpt = (double*)ireader;
                    AddToExtent(1, dim, startpt, ext);
                    ireader = (int*)(startpt + dim);

                    //this block not needed for curves since there is no CurvePoint :)
                    //point geoms do not have a point count, since 
                    //each piece is just one point each
                    //point_count = 1;
                    //if (geom_type != FdoGeometryType_MultiPoint)
                        part_count = *ireader++;

                    for (int j=0; j<part_count; j++)
                    {
                        int point_type = *ireader++;

                        switch (point_type)
                        {
                        case FdoGeometryComponentType_CircularArcSegment:
                            {
                                //circular arc segment contains two points
                                //mid, and end, start point is the last point
                                //of the previous segment.
                                double* pts = (double*)ireader;
                                // add points we have
                                AddToExtent(2, dim, pts, ext);
                                AddCurveExtents(dim, startpt, pts, ext);
                                ireader = (int*)(pts + 2 * dim);
                                startpt = pts+dim;
                            }
                            break;

                        case FdoGeometryComponentType_LineStringSegment:
                            {
                                int num_pts = *ireader++;
                                double* pts = (double*)ireader;
                                AddToExtent(num_pts, dim, pts, ext);
                                ireader = (int*)(pts + num_pts * dim);
                                startpt = pts+dim*(num_pts-1);
                            }
                            break;
                    
                        default: //Hmmm, huh?
                            throw FdoException::Create(L"Invalid FGF stream!");
                            break;

                        }
                    }
                }
            }
        }

        break;
    case FdoGeometryType_MultiGeometry:
        {
            ireader++;
            int num_geom = *ireader++;
            ext[0] = ext[1] =  DBL_MAX;
            ext[2] = ext[3] = -DBL_MAX;
            for(int i = 0; i < num_geom; i++)
            {
                double ext_geom[4];
                const unsigned char* sub_geom = (const unsigned char*)ireader;
                GetFgfExtents(sub_geom, -1 /* it's hard to know the len, however is not used */, ext_geom);
                SkipGeometry(&sub_geom);
                ireader = (int*)sub_geom;
                AddToExtent(2, 2, ext_geom, ext);
            }
        }   
        break;
    default:
            ext[0] = ext[1] =  DBL_MAX;
            ext[2] = ext[3] = -DBL_MAX;
        break;
    }
}

double ComputeGeometryLength(const unsigned char* fgf, bool computeGeodetic)
{
    double retVal = 0.0;
    
    int* ireader = (int*)fgf;
    // the geometry type
    int geom_type = (FdoGeometryType) ireader[0];
    switch (geom_type)
    {
	case FdoGeometryType_LineString:
    {
        unsigned int tmp = (unsigned int)(FdoDimensionality)*(ireader+1);
        unsigned int dim = FDODIM2NCOORDS(tmp);
        int point_count = *(ireader+2);
        const double* ordinates = (const double*)(ireader+3);
        retVal += ComputeLength(point_count, dim, ordinates, computeGeodetic);
    }
		break;
	case FdoGeometryType_Point:
	case FdoGeometryType_MultiPoint:
		// Do nothing
		break;
	case FdoGeometryType_Polygon:
    {
        unsigned int tmp = (unsigned int)(FdoDimensionality)*(ireader+1);
        unsigned int dim = FDODIM2NCOORDS(tmp);
        int contour_count = *(ireader+2);
        //common case of a single contour
        if (contour_count == 1)
        {
            int point_count = *(ireader+3);
            const double* ordinates = (const double*)(ireader+4);
            retVal += ComputeLength(point_count, dim, ordinates, computeGeodetic);
        }
        else
        {
            ireader += 3; //skip past geom_type, dimensionality, and contour count
            for (int i = 0; i < contour_count; i++)
            {
                int point_count = *ireader++;
                const double* ordinates = (const double*)ireader;
                retVal += ComputeLength(point_count, dim, ordinates, computeGeodetic);
                ireader = (int*)(ordinates + point_count * dim);
            }
        }
    }
		break;

	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
    {
        ireader++;
        int num_geoms = *ireader++;
        int point_count;
        int contour_count;
        for (int q=0; q<num_geoms; q++)
        {
            //skip past geometry type of subgeometry
            //we know it is LineString or Polygon respectively
            ireader++;
            //read cordinate type
            unsigned int tmp = (FdoDimensionality) *ireader++;
            unsigned int dim = FDODIM2NCOORDS(tmp);
            contour_count = (geom_type == FdoGeometryType_MultiPolygon) ? *ireader++ : 1; // 1 for linestrings
            for (int i = 0; i < contour_count; i++)
            {
                point_count = *ireader++;
                const double* ordinates = (const double*)ireader;
                retVal += ComputeLength(point_count, dim, ordinates, computeGeodetic);
                ireader = (int*)(ordinates + point_count * dim);
            }
        }
    }
		break;

	case FdoGeometryType_CurveString:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_CurvePolygon:
	case FdoGeometryType_MultiCurvePolygon:
    {
        ireader++;
        bool is_multi = (geom_type == FdoGeometryType_MultiCurveString)
            || (geom_type == FdoGeometryType_MultiCurvePolygon);

        //temp variables used inside the loop
        int num_geoms = 1;
        int part_count;
        int contour_count;

        //in case of multipolygon or multilinestring or multipoint, 
        //read poly or linestring count
        if (is_multi) num_geoms = *ireader++;

        for (int q=0; q<num_geoms; q++)
        {
            //skip past geometry type of subgeometry
            //we know it is CurveString or CurvePolygon or Point respectively
            if (is_multi) ireader++;

            //read cordinate type
            int tmp = (FdoDimensionality) *ireader++;
            int dim = FDODIM2NCOORDS(tmp);

            // the number of contours in current curvepolygon/curvestring
            contour_count = (geom_type == FdoGeometryType_CurvePolygon || 
                geom_type == FdoGeometryType_MultiCurvePolygon) ? *ireader++ : 1; //1 for curvestrings

            for (int i = 0; i < contour_count; i++)
            {
                //read the startpoint of the curve geometry, it's stored
                //separately, before the other points
                const double* startpt = (const double*)ireader;
                ireader = (int*)(startpt + dim);

                part_count = *ireader++;

                for (int j=0; j<part_count; j++)
                {
                    int point_type = *ireader++;

                    switch (point_type)
                    {
                    case FdoGeometryComponentType_CircularArcSegment:
                        {
                            // circular arc segment contains two points mid, and end,
                            // start point is the last point of the previous segment.
                            int num_pts = 2;
                            const double* ordinates = (const double*)ireader;
                            retVal += ComputeCircularArcSegmentLength(dim, startpt, ordinates, computeGeodetic);
                            // remember last point for next segment
                            startpt = ordinates + dim * (num_pts - 1); 
                            ireader = (int*)(ordinates + num_pts * dim);
                        }
                        break;

                    case FdoGeometryComponentType_LineStringSegment:
                        {
                            int num_pts = *ireader++;
                            const double* ordinates = (const double*)ireader;
                            retVal += ComputeLengthWithStartPoint(num_pts, dim, startpt, ordinates, computeGeodetic);
                            // remember last point for next segment
                            startpt = ordinates + dim * (num_pts - 1); 
                            ireader = (int*)(ordinates + num_pts * dim);
                        }
                        break;
                
                    default: //Hmmm, huh?
                        throw FdoException::Create(L"Invalid FGF stream!");
                        break;
                    }
                }
            }
        }
    }
		break;

	case FdoGeometryType_MultiGeometry:
    {
        ireader++;
        int num_geom = *ireader++;
        for(int i = 0; i < num_geom; i++)
        {
            const unsigned char* sub_geom = (const unsigned char*)ireader;
            retVal += ComputeGeometryLength(sub_geom, computeGeodetic);
            SkipGeometry(&sub_geom);
            ireader = (int*)sub_geom;
        }
    }
		break;

    default:
        // exception !?
		break;
    }    
    return retVal;
}

double ComputeGeometryArea(const unsigned char* fgf, bool computeGeodetic)
{
    double retVal = 0.0;
    
    int* ireader = (int*)fgf;
    // the geometry type
    int geom_type = (FdoGeometryType) ireader[0];
    switch (geom_type)
    {
	case FdoGeometryType_LineString:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_CurveString:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_Point:
	case FdoGeometryType_MultiPoint:
        break;

	case FdoGeometryType_Polygon:
    {
        unsigned int tmp = (unsigned int)(FdoDimensionality)*(ireader+1);
        unsigned int dim = FDODIM2NCOORDS(tmp);
        int contour_count = *(ireader+2);
        //common case of a single contour
        if (contour_count == 1)
        {
            int point_count = *(ireader+3);
            const double* ordinates = (const double*)(ireader+4);
            retVal += ComputeLinearRingArea(point_count, dim, ordinates, computeGeodetic, true);
        }
        else
        {
            ireader += 3; //skip past geom_type, dimensionality, and contour count
            for (int i = 0; i < contour_count; i++)
            {
                int point_count = *ireader++;
                const double* ordinates = (const double*)ireader;
                retVal += ComputeLinearRingArea(point_count, dim, ordinates, computeGeodetic, (i == 0));
                ireader = (int*)(ordinates + point_count * dim);
            }
        }
    }
		break;

	case FdoGeometryType_MultiPolygon:
    {
        ireader++;
        int num_geoms = *ireader++;
        int point_count;
        int contour_count;
        for (int q=0; q<num_geoms; q++)
        {
            //skip past geometry type of subgeometry
            //we know it is Polygon respectively
            ireader++;
            //read cordinate type
            unsigned int tmp = (FdoDimensionality) *ireader++;
            unsigned int dim = FDODIM2NCOORDS(tmp);
            contour_count = *ireader++;
            for (int i = 0; i < contour_count; i++)
            {
                point_count = *ireader++;
                const double* ordinates = (const double*)ireader;
                retVal += ComputeLinearRingArea(point_count, dim, ordinates, computeGeodetic, (i == 0));
                ireader = (int*)(ordinates + point_count * dim);
            }
        }
    }
		break;

    case FdoGeometryType_CurvePolygon:
    case FdoGeometryType_MultiCurvePolygon:
    {
        ireader++;
        bool is_multi = (geom_type == FdoGeometryType_MultiCurvePolygon);

        //temp variables used inside the loop
        int num_geoms = 1;
        int part_count;
        int contour_count;

        //in case of multipolygon or multilinestring or multipoint, 
        //read poly or linestring count
        if (is_multi) num_geoms = *ireader++;

        for (int q=0; q<num_geoms; q++)
        {
            //skip past geometry type of subgeometry
            //we know it is CurveString or CurvePolygon or Point respectively
            if (is_multi) ireader++;

            //read cordinate type
            int tmp = (FdoDimensionality) *ireader++;
            int dim = FDODIM2NCOORDS(tmp);

            // the number of contours in current curvepolygon
            contour_count = *ireader++;

            for (int i = 0; i < contour_count; i++)
            {
                //read the startpoint of the curve geometry, it's stored
                //separately, before the other points
                const double* startpt = (const double*)ireader;
                ireader = (int*)(startpt + dim);

                part_count = *ireader++;

                double ringArea = 0.0;
                for (int j=0; j<part_count; j++)
                {
                    int point_type = *ireader++;

                    switch (point_type)
                    {
                    case FdoGeometryComponentType_CircularArcSegment:
                        {
                            // circular arc segment contains two points mid, and end,
                            // start point is the last point of the previous segment.
                            int num_pts = 2;
                            const double* ordinates = (const double*)ireader;
                            ringArea += ComputeUsingTesselateArcSegment(dim, startpt, ordinates, computeGeodetic, true);
                            // remember last point for next segment
                            startpt = ordinates + dim * (num_pts - 1); 
                            ireader = (int*)(ordinates + num_pts * dim);
                        }
                        break;

                    case FdoGeometryComponentType_LineStringSegment:
                        {
                            int num_pts = *ireader++;
                            const double* ordinates = (const double*)ireader;
                            ringArea += ComputeLinearRingAreaWithStartPoint(num_pts, dim, startpt, ordinates, computeGeodetic);
                            // remember last point for next segment
                            startpt = ordinates + dim * (num_pts - 1); 
                            ireader = (int*)(ordinates + num_pts * dim);
                        }
                        break;
                
                    default: //Hmmm, huh?
                        throw FdoException::Create(L"Invalid FGF stream!");
                        break;
                    }
                }
                ringArea = fabs(ringArea)/2.0;
	            ringArea = (i == 0) ? ringArea : -ringArea; //isExterior !?
                retVal += ringArea;
            }
        }
    }
		break;

	case FdoGeometryType_MultiGeometry:
    {
        ireader++;
        int num_geom = *ireader++;
        for(int i = 0; i < num_geom; i++)
        {
            const unsigned char* sub_geom = (const unsigned char*)ireader;
            retVal += ComputeGeometryArea(sub_geom, computeGeodetic);
            SkipGeometry(&sub_geom);
            ireader = (int*)sub_geom;
        }
    }
		break;

    default:
        // exception !?
		break;
    }
    return retVal;
}

#define ComputeEuclidianTrapezoidArea2D(x1, y1, x2, y2) ((x2) - (x1)) * ((y2) + (y1))

/************************************************************************/
/* ComputeGeodeticTrapezoidArea2D										*/
/*																		*/
/* Use an appproximate formula: A = b*h + (B - b)*h/2					*/
/************************************************************************/
double ComputeGeodeticTrapezoidArea2D(double lat1, double lon1, double lat2, double lon2, double minLat )
{
	// Compute the distances to the Ecuator
	double dist1 = ComputeGeodeticDistance2D( lat1, lon1, minLat, lon1 );
	double dist2 = ComputeGeodeticDistance2D( lat2, lon2, minLat, lon2 );
	double minDist = std::min<double>(dist1, dist2);

	// Compute the distance between points at the same latitude. Take the average
	double distLat1 = ComputeGeodeticDistance2D( lat1, lon1, lat1, lon2 );
	double distLat2 = ComputeGeodeticDistance2D( lat2, lon1, lat2, lon2 );
	double h = (distLat1 + distLat2)/2;

	// Multiplied by 2
	double area = minDist * h * 2 + fabs(dist1 - dist2) * h;

    return (lon2 < lon1) ? (-area) : area;
}

double ComputeLinearRingAreaWithStartPoint(int point_count, unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic)
{
    double retVal = 0.0;
	double minLat = 0.0;
    int maxCnt = point_count*dim - dim;
    double x1, y1, x2, y2;

	if ( computeGeodetic )
	{
        minLat = *(startpoint+1);
        for (int i = 0; i < maxCnt; i += dim)
        {
			double lat = *(ordinates+i+1);
            minLat = (lat < minLat) ? lat : minLat;
		}
	}

	for (int i = -1; i < maxCnt;)
	{
        if (i == -1)
        {
	        x1 = *startpoint;
	        y1 = *(startpoint+1);
	        i = 0;
        }
        else
        {
	        x1 = ordinates[i];
	        y1 = ordinates[i + 1];
	        i += dim;  // advance
        }
	    x2 = ordinates[i];
	    y2 = ordinates[i + 1];

		if (computeGeodetic)
			retVal += ComputeGeodeticTrapezoidArea2D(y1, x1, y2, x2, minLat); 
		else
			retVal += ComputeEuclidianTrapezoidArea2D(x1, y1, x2, y2);
	}
	return retVal;
}

double ComputeLinearRingArea(int point_count, unsigned int dim, const double *ordinates, bool computeGeodetic, bool isExterior)
{
    double retVal = 0.0;
	double minLat = 0.0;
    double x1, y1, x2, y2;
    int maxCnt = point_count*dim - dim;

	if ( computeGeodetic )
	{
        for (int i = 0; i < maxCnt; i += dim)
        {
			double lat = *(ordinates+i+1);
            minLat = (i == 0) ? lat : ((lat < minLat) ? lat : minLat);
		}
	}

	for (int i = 0; i < maxCnt;)
	{
		x1 = ordinates[i];
		y1 = ordinates[i + 1];
		i += dim;  // advance

		x2 = ordinates[i];
		y2 = ordinates[i + 1];

		if (computeGeodetic)
			retVal += ComputeGeodeticTrapezoidArea2D(y1, x1, y2, x2, minLat); 
		else
			retVal += ComputeEuclidianTrapezoidArea2D(x1, y1, x2, y2);
	}

	retVal = fabs(retVal)/2.0;
	retVal = isExterior ? retVal : - retVal;
	return retVal;
}


void SkipGeometry(const unsigned char** inputStream)
{
    int geom_type = (FdoGeometryType)*(const int*)(*inputStream);
    *inputStream += sizeof(int);
    unsigned int tmp;
    unsigned int dim;
    int obj_count;
    int point_count;
    int i;

    switch (geom_type)
    {
	case FdoGeometryType_LineString:
        tmp = (FdoDimensionality)*(const int*)(*inputStream);
        dim = FDODIM2NCOORDS(tmp);
        *inputStream += sizeof(int);

        point_count = *(const int*)(*inputStream);
        *inputStream += sizeof(int) + dim * point_count * sizeof(double);
		break;

	case FdoGeometryType_Point:
        tmp = (FdoDimensionality)*(const int*)(*inputStream);
        dim = FDODIM2NCOORDS(tmp);
        *inputStream += sizeof(int) + dim * sizeof(double);
		break;

	case FdoGeometryType_Polygon:
        tmp = (FdoDimensionality)*(const int*)(*inputStream);
        dim = FDODIM2NCOORDS(tmp);
        *inputStream += sizeof(int);

        obj_count = *(const int*)(*inputStream);
        *inputStream += sizeof(int);
	    for (i = 0; i < obj_count; i++)
	    {
            point_count = *(const int*)(*inputStream);
            *inputStream += sizeof(int) + dim * point_count * sizeof(double);
	    }
		break;

	case FdoGeometryType_CurveString:
        tmp = (FdoDimensionality)*(const int*)(*inputStream);
        dim = FDODIM2NCOORDS(tmp);
        *inputStream += sizeof(int) + dim * sizeof(double); // dim + Start position.
        
        obj_count = *(const int*)(*inputStream);
        *inputStream += sizeof(int);
	    for (i = 0; i < obj_count; i++)
	    {
            geom_type = (FdoGeometryType)*(const int*)(*inputStream);
            *inputStream += sizeof(int);
            switch(geom_type)
            {
            case FdoGeometryComponentType_LineStringSegment:
                point_count = *(const int*)(*inputStream);
                *inputStream += sizeof(int);
                break;
            case FdoGeometryComponentType_CircularArcSegment:
                point_count = 2;
                break;
            default: //Hmmm, huh?
                throw FdoException::Create(L"Invalid FGF stream!");
                break;
            }
            *inputStream += dim * point_count * sizeof(double);
	    }
		break;

	case FdoGeometryType_CurvePolygon:
        tmp = (FdoDimensionality)*(const int*)(*inputStream);
        dim = FDODIM2NCOORDS(tmp);
        *inputStream += sizeof(int);

        obj_count = *(const int*)(*inputStream);
        *inputStream += sizeof(int);
	    for (i = 0; i < obj_count; i++)
	    {
            *inputStream += dim * sizeof(double); // Start position.

            int seg_count = *(const int*)(*inputStream);
            *inputStream += sizeof(int);
            for (int j = 0; j < seg_count; j++)
            {
                geom_type = (FdoGeometryType)*(const int*)(*inputStream);
                *inputStream += sizeof(int);
                switch(geom_type)
                {
                case FdoGeometryComponentType_LineStringSegment:
                    point_count = *(const int*)(*inputStream);
                    *inputStream += sizeof(int);
                    break;
                case FdoGeometryComponentType_CircularArcSegment:
                    point_count = 2;
                    break;
                default: //Hmmm, huh?
                    throw FdoException::Create(L"Invalid FGF stream!");
                    break;
                }
                *inputStream += dim * point_count * sizeof(double);
            }
        }
		break;

	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
	case FdoGeometryType_MultiGeometry:
        obj_count = *(const int*)(*inputStream);
        *inputStream += sizeof(int);
	    for (i = 0; i < obj_count; i++)
            SkipGeometry(inputStream);
		break;

    default:
		break;
    }
}

#define	EPS2               0.00000001
#define ArePositionsEqualXY(x1, y1, x2, y2) ( (fabs(x1-x2) < EPS2) && (fabs(y1-y2) < EPS2) )
#define DistanceBetweenPositionsXY(x1, y1, x2, y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
// vectorToRight determines whether the vector (x2, y2) points to right or
// left side of vector (x1,y1) and returns false for right and true for left.
#define vectorToLeft( x1, y1, x2, y2 ) ( ((x1)*(y2) - (x2)*(y1)) < 0 ? false : true )

// circular arc segment contains two points mid, and end, start point is the last point of the previous segment.
double ComputeCircularArcSegmentLength(unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic)
{
    if (computeGeodetic)
        return ComputeUsingTesselateArcSegment(dim, startpoint, ordinates, computeGeodetic, false);

    ArcSegmentDetails details;
    GetCircularArcSegmentDetails(dim, startpoint, ordinates, &details);
    return details.length;
}

bool GetCircularArcSegmentDetails(unsigned int dim, const double *startpoint, const double *ordinates, ArcSegmentDetails* details)
{
    details->isCircle = false;
    // Special cases
    if (ArePositionsEqualXY(*startpoint, *(startpoint+1), *(ordinates+dim), *(ordinates+dim+1)))
    {
        // This might be a supported circle case.
        if (ArePositionsEqualXY(*startpoint, *(startpoint+1), *ordinates, *(ordinates+1)))
        {
            details->length = 0.0; // Degenerated arc, all 3 points are coincident.
		    return false;
        }
	    else
        {
            details->radius = DistanceBetweenPositionsXY(*startpoint, *(startpoint+1), *ordinates, *(ordinates+1)) / 2.0;
            details->length = M_PI * details->radius;
            details->isCircle = true;
            details->center[0] = (*ordinates+*startpoint)*0.5;
            details->center[1] = (*(ordinates+1)+*(startpoint+1))*0.5;
		    return true; // Full circle.
        }
    }

    double* center = &details->center[0]; // 2D only
    if(ComputeCenterFromThreePositions(dim, startpoint, ordinates, &center[0]))
    {
        details->radius = DistanceBetweenPositionsXY(*startpoint, *(startpoint+1), *center, *(center+1));
        details->thetaStartAngle = atan2(*(startpoint+1) - *(center+1), *startpoint - *center);
        if (details->thetaStartAngle == -M_PI)
            details->thetaStartAngle = M_PI;
        details->thetaEndAngle = atan2(*(ordinates+dim+1) - *(center+1), *(ordinates+dim) - *center);
        if (details->thetaEndAngle == -M_PI)
            details->thetaEndAngle = M_PI;

        details->isCounterClockWise = IsDirectionCounterClockWise(startpoint, ordinates, ordinates+dim);

        // Make sure that angles' magnitudes reflect direction.
        if ( details->isCounterClockWise && details->thetaStartAngle > details->thetaEndAngle )
            details->thetaEndAngle += 2.0 * M_PI;
        else if ( !details->isCounterClockWise && details->thetaStartAngle < details->thetaEndAngle )
            details->thetaEndAngle -= 2.0 * M_PI;

        details->length = details->radius * fabs(details->thetaEndAngle - details->thetaStartAngle);
        return true;
    }
    details->length = 0.0;
    return false;
}

#define MATRIX(i, j) ( *( ptr_matrix + ( (i)*dim + (j) ) ) )
#define VERY_SMALL (1.0e-17)
double SnapToZero(double n)
{
    return (fabs(n) <= VERY_SMALL)? 0.0 : n;
}


int LUDecompose(int n, double a[], int eindex[])
{
    /* 
     * Decompose a into LU, where L is lower triangular matrix and U is upper
     * triangular matrix. We also have L[i][i] = 1 for i=1,2,...
     */

    int dim = 0;
    double *ptr_matrix = NULL;
    int i, j, k, col_max;
    int sign = 1;   /* det(matrix) sign change as a result of row exchange */
    double largest; /* the pivot element */
    double tmp, *scale = NULL;

    /* initialize static vars */

    if ( n <= 0 || a == NULL || eindex == NULL )
        return 0;

    dim = n;
    ptr_matrix = a;

    /* fill scale for each row */   
    scale = new double[n];
    if (NULL == scale)
        return 0;

    for ( i = 0; i < n; ++i ) {
        largest = 0;
        for ( j = 0; j < n; ++j ) {
            if ( ( tmp=fabs(MATRIX(i,j)) ) > largest )
                largest = tmp;
        }
        if ( SnapToZero(largest) == 0.0 ) {
            delete [] scale;
            return 0;
        }
        scale[i] = 1.0 / largest;
    }

    for ( j = 0; j < n; ++j ) {
        for ( i = 0; i <= j; ++i ) {
            for ( k = 0; k < i; ++k ) {
                MATRIX(i,j) -= MATRIX(i,k) * MATRIX(k,j);
            }
        }
        col_max = j;
        largest = scale[j]*fabs(MATRIX(j,j));
        for ( i = j + 1; i < n; ++i ) {
            for ( k = 0; k < j; ++k ) {
                MATRIX(i,j) -= MATRIX(i,k) * MATRIX(k,j);
            }
            if ( ( tmp = scale[i]*fabs(MATRIX(i,j)) ) > largest ) {
                col_max = i;
                largest = tmp;
            }
        }
        if ( j != col_max ) {
            /* interchange rows */
            for ( k=0; k<n; ++k ) {
                tmp = MATRIX(j,k);
                MATRIX(j,k) = MATRIX(col_max, k);
                MATRIX(col_max, k) = tmp;
            }
            /* interchange the scale factor */
            scale[col_max] = scale[j];
            sign = -sign;
        }
        eindex[j] = col_max;
        if ( SnapToZero(largest) == 0.0 ) {
            /* if pivot element is near zero, matrix is singular */
            delete [] scale;
            return 0;
        }
        /* devided by pivot element */
        if ( j < n-1 ) {
            tmp = 1.0 / MATRIX(j,j);
            for ( i = j+1; i < n; ++i ) {
                MATRIX(i,j) *= tmp;
            }
        }
    }
    delete [] scale;
    return sign;
}


/************************************************************************
*         Find the center point of the circular arc determined by three *
*         given points                                                  *
*                                                                       *
*   Denote V, V1, V2, V3 the vectors of center, start, mid, end         *
*       respectively,                                                   *
*   then V = (x,y,z) satisfies the following equtions:                  *
*   ( V - ( V1 + V2 ) / 2 ) * ( V2 - V1 ) = 0                           *
*   ( V - ( V1 + V3 ) / 2 ) * ( V3 - V1 ) = 0                           *
*   ( V - V1 ) * ( ( V2 - V1 ) X ( V3 - V1 ) ) = 0                      *
*                                                                       *
*   i.e.,                                                               *
*                                                                       *
*   x21 * x + y21 * y + z21 * z = ( ||V2||^2 - ||V1||^2 ) / 2           *
*   x31 * x + y31 * y + z31 * z = ( ||V3||^2 - ||V1||^2 ) / 2           *
*   Dyz * x + Dzx * y + Dxy * z = Dxyz                                  *
*                                                                       *
*   where                                                               *
*   ( x21, y21, z21 ) = V2 - V1,                                        *
*   ( x31, y31, z31 ) = V3 - V1,                                        *
*   Dyz = y21 * z31 - y31 * z21,                                        *
*   Dzx = z21 * x31 - z31 * x21,                                        *
*   Dxy = x21 * y31 - x31 * y21,                                        *
*                                                                       *
*   Dxyz = V1 * ( V2 X V3 )                                             *
*                                                                       *
*         | x1  y1  z1  |                                               *
*       = | x21 y21 z21 | = x1 * Dyz + y1 * Dzx + z1 * Dxy              *
*         | x31 y31 z31 |                                               *
*                                                                       *
*   ||Vi||^2 = xi*xi + yi*yi + zi*zi                                    *
*                                                                       *
************************************************************************/
// no support for 3D yet
bool ComputeCenterFromThreePositions(unsigned int dim, const double *startpoint, const double *ordinates, double* center)
{
    const double *endpoint = (ordinates+dim);

    double a[9], b[3], tmp;
    int eindex[3];
    int i;

    a[0] = *(ordinates) - *(startpoint); /* x21 */
    a[1] = *(ordinates+1) - *(startpoint+1); /* y21 */
    a[2] = 0.0; /* z21 */

    a[3] = *(endpoint) - *(startpoint); /* x31 */
    a[4] = *(endpoint+1) - *(startpoint+1); /* y31 */
    a[5] = 0.0; /* z31 */

              /* y21 * z31 - y31 * z21 */
    a[6] = a[1] * a[5] - a[4] * a[2]; /* Dyz */
              /* z21 * x31 - z31 * x21 */
    a[7] = a[2] * a[3] - a[5] * a[0]; /* Dzx */
              /* x21 * y31 - x31 * y21 */
    a[8] = a[0] * a[4] - a[3] * a[1]; /* Dxy */

    tmp = *(startpoint) * *(startpoint) + *(startpoint+1) * *(startpoint+1);

    b[0] = *(ordinates) * *(ordinates) + *(ordinates+1) * *(ordinates+1);

    b[1] = *(endpoint) * *(endpoint) + *(endpoint+1) * *(endpoint+1);

    b[0] -= tmp;
    b[1] -= tmp;
    b[0] *= 0.5;
    b[1] *= 0.5;
    /* Dxyz = x1 * Dyz + y1 * Dzx + z1 * Dxy */
    b[2] = *(startpoint) * a[6] + *(startpoint+1) * a[7];

    /* Decompose a into LU */
    if ( LUDecompose( 3, a, eindex ) == 0 )
    {
        // Zero result means that points were collinear.
        return false;
    }

    /* Perform row exchanges for b */
    for ( i = 0; i < 3; ++i ) {
        if ( eindex[i] != i ) {
            tmp = b[i];
            b[i] = b[eindex[i]];
            b[eindex[i]] = tmp;
        }
    }

    /* Use L (UV) = b to solve UV */
    b[1] -= a[3] * b[0];
    b[2] -= a[6] * b[0] + a[7] * b[1];

    /* Use UV = b to solve V, the center */
    *(center+1) = b[1] / a[4];
    *(center) = (b[0] - a[1] * *(center+1)) / a[0];

    return true;
}

bool IsDirectionCounterClockWise(const double *startpoint, const double *midpoint, const double *endpoint)
{
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

    return ((a * normalZ) > 0.0);
}

double ComputeLengthWithStartPoint(int point_count, unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic)
{
    double length = 0.0;
    double x1, y1, x2, y2;
    int maxCnt = point_count*dim - dim;
    for (int i = -1; i < maxCnt;)
    {
        if (i == -1)
        {
	        x1 = *startpoint;
	        y1 = *(startpoint+1);
	        i = 0;
        }
        else
        {
	        x1 = ordinates[i];
	        y1 = ordinates[i + 1];
	        i += dim;  // advance
        }
	    x2 = ordinates[i];
	    y2 = ordinates[i + 1];

	    if (computeGeodetic)
		    length += ComputeGeodeticDistance2D( y1, x1, y2, x2);
	    else
		    length += sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)); // ComputeEuclidianDistance
    }
    return length;
}

double ComputeLength(int point_count, unsigned int dim, const double *ordinates, bool computeGeodetic)
{
    double length = 0.0;
    double x1, y1, x2, y2;
    int maxCnt = point_count*dim - dim;
    for (int i = 0; i < maxCnt;)
    {
	    x1 = ordinates[i];
	    y1 = ordinates[i + 1];
	    i += dim;  // advance

	    x2 = ordinates[i];
	    y2 = ordinates[i + 1];

	    if (computeGeodetic)
		    length += ComputeGeodeticDistance2D( y1, x1, y2, x2);
	    else
		    length += sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)); // ComputeEuclidianDistance
    }
    return length;
}

/************************************************************************/
/*	ComputeGeodeticDistance												*/
/*																		*/
/* Credits: the code has been ported from http://www.codeguru.com/Cpp/Cpp/algorithms/article.php/c5115/
/* It includes an optimization for small angles:
/*		One (1st edition) reader provided an alternate distance formula that is better for small angles. 
/*
/*		dlon = lon2 - lon1
/*		dlat = lat2 - lat1
/*		a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
/*		c = 2 * arcsin(min(1,sqrt(a)))
/*		distance = (Earth Radius) * c
/* 
/*		
/* The algorithm coincides with PROJ4 implementation for geodetic distance.
/************************************************************************/
#define DE2RA             0.01745329252
#define	RA2DE             57.2957795129
#define	ERADM             6378135.0			
#define	AVG_ERAD          6371000.0			// Average radius
#define	FLATTENING        1.0/298.257223563 // Earth flattening (WGS '84)
#define	EPS               0.000000000005

#define	GEODETIC_DIST_OPTIMIZATION	false	// TODO: doesn't work yet

double ComputeGeodeticDistance2D(double lat1, double lon1, double lat2, double lon2)
{
	double	lat1r		= lat1 * DE2RA;
	double	lon1r		= lon1 * DE2RA;
	double	lat2r		= lat2 * DE2RA;
	double	lon2r		= lon2 * DE2RA;
	double	deltaLat	= lat1r - lat2r; 
	double	deltaLon	= lon1r - lon2r; 

	double	sin1		= sin(lat1r);
	double	sin2		= sin(lat2r);
	double	cos1		= cos(lat1r);
	double	cos2		= cos(lat2r);

	double	d = 0.0;

	if ( !GEODETIC_DIST_OPTIMIZATION )
	{
		d = sin1*sin2 + cos1*cos2*cos(deltaLon);
	}
	else if ( fabs(deltaLat) > 1. || fabs(deltaLon) > 1. )
	{
		d = sin1*sin2 + cos1*cos2*cos(deltaLon);
	}
	else
	{
		// alternate distance formula that is better for small angles
		double	deltaLat	= lat2r - lat1r; 
		double	deltaLon	= lon2r - lon1r; 

		double	sinDlat = sin(deltaLat/2);
		double	sinDlon = sin(deltaLon/2);

		double a = sinDlat*sinDlat + cos1*cos2*sinDlon*sinDlon;
        double aSqrt =  sqrt(a);
        double minValue = (1 < aSqrt) ? 1 : aSqrt; 
		d = 2 * asin(minValue);
	}

    // Adjust the value to [0, 1] interval.
    if ( d > 1 )
        d = 1;

	return (ERADM * acos(d));
}

/************************************************************************
*      BestArcStep - Compute best interval for interpolating arc        *
*                                                                       *
*      Consider a small arc from a circle centered at the origin.       *
*      Consider approximating the arc with a chord between the arc's    *
*      endpoints.  Position the arc so that its mid-point lies on       *
*      the positive x-axis and the chord is vertical.                   *
*      The maximum error will be between the arc's and chord's midpoints,*
*      which both lie on the x-axis.  For an arc of length C            *
*      depicting an angle of THETA from a circle of radius R, the       *
*      chord will be at:                                                *
*          X = R cos( THETA/2 )                                         *
*      and the difference, DX, from R is:                               *
*          DX = R - X                                                   *
*             = R ( 1 - cos( THETA/2 ) )                                *
*             = R ( 1 - cos( C / 2R ) )                                 *
*      DX is given as max_err.  Solving for C, we get:                  *
*                                                                       * 
*          C = 2R arccos( 1 - DX/R )                                    *
*                                                                       *
*      NOTE: this formula should be reviewed for ill-conditioning. The  *
*      use of arccosine may be inappropriate for small angles, and an   *
*      alternative may be needed, similar to the use of Haversine       *
*      formula instead of Law of Cosines in                             *
*      ComputeLengthFromRadiusAngles().                                 *
*                                                                       *
************************************************************************/
double BestArcStep(double radius, double arcLength, double maximumError)
{
    // Avoid mathematical exceptions.
    if (SnapToZero(radius) <= 0.0 || SnapToZero(maximumError) <= 0.0)
        throw FdoException::Create(L"Invalid FGF stream!");

    double           arcStep = 0.0;
    double           resolution = maximumError * 2.0;

    double  portion = 1.0 - ( maximumError / radius );

    if ( portion < 1.0 && portion > 0.0 )
    {
        arcStep = 2.0 * radius * acos( portion );

        // Avoid step length that is too short for the resolution.
        if ( arcStep < resolution )
            arcStep = resolution;

        // Arc step is too long for interpolation.
        if ( arcStep > arcLength / 2.0 )
            arcStep = 0.0;
    }
    else
    {
        // Arc step is too long for interpolation.
        ;   // Leave as default 0.0.
    }

    return arcStep;
}

#define MAX_NUM_POSITIONS       (5000)  // Fail-safe maximum # positions in linearised arc.
#define MAX_NUM_LINESEGMENTS    (MAX_NUM_POSITIONS-1)
#define CURVE_APPROX_FACTOR 1e-3
double ComputeUsingTesselateArcSegment(unsigned int dim, const double *startpoint, const double *ordinates, bool computeGeodetic, bool isAreaCalculation)
{
    double retVal = 0.0;
    double ext[4];
    ext[0] = ext[1] =  DBL_MAX;
    ext[2] = ext[3] = -DBL_MAX;
    AddToExtent(1, 2, (double*)startpoint, ext);
    AddToExtent(1, 2, (double*)ordinates, ext);
    AddToExtent(1, 2, (double*)(ordinates+dim), ext);
    
    double maxSpacing = max(ext[2] - ext[0], ext[3] - ext[1]);
    double maxOffset = maxSpacing * CURVE_APPROX_FACTOR;
    if (maxSpacing < 0.0 || maxOffset < 0.0)
        return retVal;
    
    // Avoid mathematical exceptions later.
    maxSpacing = SnapToZero(maxSpacing);
    maxOffset = SnapToZero(maxOffset);
    double spacing = 0.0;

    ArcSegmentDetails details;
    if(!GetCircularArcSegmentDetails(dim, startpoint, ordinates, &details))
        return retVal;

    bool maxSpacingIsUseful = (maxSpacing > 0.0 && maxSpacing <= details.length/2.0);
    bool maxOffsetIsUseful = (maxOffset > 0.0);
    
    if ((details.length != 0.0) && (maxSpacingIsUseful || maxOffsetIsUseful))
    {
        if (maxOffsetIsUseful)
            spacing = BestArcStep(details.radius, details.length, maxOffset);
        if (spacing <= 0.0)
            maxOffsetIsUseful = false;

        if (maxSpacingIsUseful && (spacing > maxSpacing || !maxOffsetIsUseful))
            spacing = maxSpacing;

        if (maxOffsetIsUseful)
        {
            double resolution = maxOffset * 2.0;
            if (spacing < resolution)
                spacing = resolution;
        }
    }
    int numLineSegments = (spacing > 0.0) ? (int) ceil(details.length / spacing) : 0;
    if (numLineSegments > MAX_NUM_LINESEGMENTS)
        numLineSegments = MAX_NUM_LINESEGMENTS;

    if (spacing <= 0.0 || numLineSegments < 3)
    {
        // Interpolation is not possible.  Just use start mid & end
        if (isAreaCalculation)
            retVal = ComputeLinearRingAreaWithStartPoint(2, dim, startpoint, ordinates, computeGeodetic);
        else
            retVal = ComputeLengthWithStartPoint(2, dim, startpoint, ordinates, computeGeodetic);
    }
    else
    {
        // Slightly adjust for even spacing of positions along curve.
        spacing  = details.length / numLineSegments;

        double angleIncrement = spacing / details.radius;
        // 2D only
        double* newpoints = new double[2*numLineSegments];
        // Compute intermediate positions.
        int i;
        for (i = 1;  i < numLineSegments;  i++)
        {
            double angleOffset = i * angleIncrement;
            // Use simplest possible calculations in 2D case.
            if (!details.isCounterClockWise)
                angleOffset *= -1.0; // Adjust for arc orientation about Z axis.

            double angle = details.thetaStartAngle + angleOffset;
            newpoints[2*(i-1)] = details.center[0] + (details.radius * cos(angle)); // x
            newpoints[2*(i-1)+1] = details.center[1] + (details.radius * sin(angle)); //y
        }
        // add end position
        newpoints[2*(i-1)] = *(ordinates+dim); // x
        newpoints[2*(i-1)+1] = *(ordinates+dim+1); //y
        if (isAreaCalculation)
            retVal = ComputeLinearRingAreaWithStartPoint(numLineSegments, 2, startpoint, newpoints, computeGeodetic);
        else
            retVal = ComputeLengthWithStartPoint(numLineSegments, 2, startpoint, newpoints, computeGeodetic);
        delete[] newpoints;
    }
    return retVal;
}

#define __dX     *(ordinates+dim)-*startpoint
#define __dY     *(ordinates+dim+1)-*(startpoint+1)
#define __sX     *startpoint
#define __sY     *(startpoint+1)
#define __eX     *(ordinates+dim)
#define __eY     *(ordinates+dim+1)

/************************************************************************/
/// A different solution would be to use SSS Theorem to calculate radius and center
///
// double a = DistanceBetweenPositionsXY(*ordinates, *(ordinates+1), *startpoint, *(startpoint+1));
// double b = DistanceBetweenPositionsXY(*ordinates, *(ordinates+dim+1), *(ordinates+dim), *(ordinates+1));
// double c = DistanceBetweenPositionsXY(*(ordinates+dim), *(startpoint+1), *startpoint, *(ordinates+dim+1));
// double s = (a+b+c)/2.0;
// SSS Theorem
// double radius = (a*b*c)/(4*sqrt(s*(a+b-s)*(a+c-s)*(b+c-s)));
// double h = sqrt(radius*radius - (a*a/4.0));

// double mx = (*startpoint + *ordinates)/2.0;
// double my = (*(startpoint+1) + *(ordinates+1))/2.0;
// bool isCounterClockWise = IsDirectionCounterClockWise(startpoint, ordinates, ordinates+dim);
// double norm = isCounterClockWise ? 1 : -1;
// center[0] = mx + norm*h*(*(startpoint+1) - *(ordinates+1))/a;
// center[1] = my + norm*h*(*ordinates - *startpoint)/a;
/************************************************************************/
void AddCurveExtents(int dim, double* startpoint, double* ordinates, double* ext)
{
    ArcSegmentDetails details;
    if(GetCircularArcSegmentDetails(dim, startpoint, ordinates, &details))
    {
        double extTmp[4];
        if (details.isCircle)
        {
            extTmp[0] = details.center[0] - details.radius;
            extTmp[1] = details.center[1] - details.radius;
            extTmp[2] = details.center[0] + details.radius;
            extTmp[3] = details.center[1] + details.radius;
        }
        else
        {
            extTmp[0] = __eX < __sX ? __eX : __sX;
            extTmp[1] = __eY < __sY ? __eY : __sY;
            extTmp[2] = __eX > __sX ? __eX : __sX;
            extTmp[3] = __eY > __sY ? __eY : __sY;
            
            if (vectorToLeft(details.center[0]-__sX, details.center[1]+details.radius-__sY, __dX, __dY) == details.isCounterClockWise)
                extTmp[3] = details.center[1] + details.radius; // Use circle top.

            if (vectorToLeft(details.center[0]-__sX, details.center[1]-details.radius-__sY, __dX, __dY) == details.isCounterClockWise)
                extTmp[1] = details.center[1] - details.radius; // Use circle bottom.

            if (vectorToLeft(details.center[0]-details.radius-__sX, details.center[1]-__sY, __dX, __dY) == details.isCounterClockWise)
                extTmp[0] = details.center[0] - details.radius; // Use circle left.

            if (vectorToLeft(details.center[0]+details.radius-__sX, details.center[1]-__sY, __dX, __dY) == details.isCounterClockWise)
                extTmp[2] = details.center[0] + details.radius; // Use circle right.
        }
        AddToExtent(2, 2, extTmp, ext);
    }
}
