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

    default:
   
        //all other geometry types -- punt it to the Fdo Spatial utility
        //return false;
#ifdef SLT_USE_FDO_SPATIAL        
        FdoPtr<FdoByteArray> ba = FdoByteArray::Create(fgf, len);
        FdoSpatialUtility::GetExtents(ba, ext[0], ext[1], ext[2], ext[3]);
#endif
        break;
    }
}

