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
 *
 */

//#include "stdafx.h"
#include "Geometry.h"
#include <string.h>

/// Enumeration of possible byte orders.
enum ByteOrder
{
    eWkbXDR = 0, // Big Endian
    eWkbNDR = 1  // Little Endian
};

/// Enumeration of various PostGIS-specific flags stored in EWKB data.
enum GeometryTypeFlags
{
    eWkbFlagZ    = 0x80000000,
    eWkbFlagM    = 0x40000000,
    eWkbFlagSrid = 0x20000000,
    eWkbFlagBBox = 0x10000000
};

static pgdrv_ulong GetOrdinatesFromDimension(pgdrv_ulong dim);

static void FgfFromExtendedWkb(FdoByteArray*& fgfBytes,
                                       unsigned char* ewkb,
                                       pgdrv_ulong size,
                                       pgdrv_ulong& currentByte);

static void ExtendedWkbFromFgf(FdoByteArray*& ewkbBytes,
                                       FdoByte* fgf,
                                       pgdrv_ulong size,
                                       pgdrv_ulong srid,
                                       pgdrv_ulong& currentByte,
                                       pgdrv_ulong& totalDims);

static void CopyGeom(FdoByteArray*& dest,
                                       FdoByte* src,
                                       pgdrv_ulong size,
                                       pgdrv_ulong& currentByte,
                                       pgdrv_ulong geomType,
                                       pgdrv_ulong geomDim);
 

void *GeometryFromWkb (void *wkb, pgdrv_ulong size)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoByteArray* array;
    FdoIGeometry *g = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    array = FdoByteArray::Create ((FdoByte*)wkb, size);
    try
    {
        g = factory->CreateGeometryFromWkb (array);
        array->Release ();
    }
    catch (...)
    {
        array->Release ();
    }

    return (void *)g;
}

void FreeGeometry (void*& geometry)
{
	FdoIGeometry *g = (FdoIGeometry *)geometry;

    FDO_SAFE_RELEASE(g);
}

char *WkbFromGeometry (void *geometry, int *size)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoByteArray* array;
    void *ret = NULL;
	
	*size = 0;

    if ( geometry == NULL )
		return (char*)ret;

    factory = FdoFgfGeometryFactory::GetInstance ();
    array = factory->GetWkb ((FdoIGeometry *)geometry);
    try
    {
        *size = array->GetCount () + 4;
        ret = malloc (*size);
        memset (ret, 0, *size);
        memmove (((char*)ret) + 4, array->GetData (), *size - 4);
        array->Release ();
    }
    catch (...)
    {
        array->Release ();
    }

    return (char*)(ret);
}

void* GeometryFromExtendedWkb(void *ewkb, pgdrv_ulong size)
{
    unsigned char* l_ewkb = (unsigned char*) ewkb;
    pgdrv_ulong currentByte = 0;

    //
    // Generate FGF stream from EWKB
    //

    FdoPtr<FdoByteArray> fgfBytes = FdoByteArray::Create(size);
   FgfFromExtendedWkb(fgfBytes.p, l_ewkb, size, currentByte);

    //
    // Create FDO geometry from FGF binary data
    //
    FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());

    FdoIGeometry* geometry = factory->CreateGeometryFromFgf(fgfBytes);

    return (void*)(geometry);
}

void FgfFromExtendedWkb(FdoByteArray*& fgfBytes,
                                       unsigned char* ewkb,
                                       pgdrv_ulong size,
                                       pgdrv_ulong& currentByte)
{
   //
    // Endian flag
    //
    unsigned char endianByte = ewkb[currentByte];
    if (eWkbNDR != endianByte)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }
    currentByte++;

    //
    // Geometry type
    //
    pgdrv_ulong ewkbFlags = *(pgdrv_ulong*)(&ewkb[currentByte]);
    pgdrv_ulong geomType = ewkbFlags & 0xff;    

    currentByte += sizeof(ewkbFlags);

    //
    // EWKB flags
    //

    // Dimensionality
    pgdrv_ulong geomDim = FdoDimensionality_XY;
    if (ewkbFlags & eWkbFlagZ)
        geomDim |= FdoDimensionality_Z;
    if (ewkbFlags & eWkbFlagM)
        geomDim |= FdoDimensionality_M;

    // SRID - PostGIS specific flag
    bool isSrid = (0 != (ewkbFlags & eWkbFlagSrid));

    // BBOX - PostGIS specific flag
    bool isBbox = (0 != (ewkbFlags & eWkbFlagBBox));

    if (isBbox)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    //
    // Generate FGF stream from EWKB
    //

    // Write Geometry type
    fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomType), (FdoByte*)&geomType);

    // Skip over Srid if present. FGF does not store it.
    if (isSrid)
    {
        currentByte += sizeof(pgdrv_ulong);
    }

    // Processing:
    // - for simple geometries types, just copy the array
    // - for multi-geometries, process sub-geometries recursively
    //
    switch(geomType)
    {
    case FdoGeometryType_MultiPoint:
    case FdoGeometryType_MultiGeometry:
    case FdoGeometryType_MultiLineString:
    case FdoGeometryType_MultiPolygon:
        {
            pgdrv_ulong numSubGeoms = *(pgdrv_ulong*)(&ewkb[currentByte]);
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(numSubGeoms), (FdoByte*)&numSubGeoms);
            currentByte += sizeof(numSubGeoms);

            for (pgdrv_ulong i = 0;  i < numSubGeoms && currentByte < size;  i++)
            {
                // Recurse on sub-geometry.
                FgfFromExtendedWkb(fgfBytes, ewkb, size, currentByte);
            }
        }
        break;
    default:
        {
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomDim), (FdoByte*)&geomDim);

            CopyGeom(fgfBytes, ewkb, size, currentByte, geomType, geomDim );
        }
        break;
    }
}

FdoByteArray *ExtendedWkbFromGeometry (void *geometry, pgdrv_ulong srid)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoByteArray> fgfBytes;
    pgdrv_ulong currentByte = 0;
    pgdrv_ulong totalDims = 0;
    FdoByteArray* ewkbBytes = NULL;
	
    if ( geometry == NULL )
		return ewkbBytes;

    factory = FdoFgfGeometryFactory::GetInstance ();
    fgfBytes = factory->GetFgf((FdoIGeometry *)geometry);

    // Make ewkb buffer a bit longer to account for endian flag and srid.
    // Might not be enough for multi geometry types but the Byte array will
    // grow if needed.
    ewkbBytes = FdoByteArray::Create(fgfBytes->GetCount() + 20);
    FdoByte* fgfData = fgfBytes->GetData();
    ExtendedWkbFromFgf(ewkbBytes, fgfData, fgfBytes->GetCount(), srid, currentByte, totalDims);
    
    return ewkbBytes;
}

void ExtendedWkbFromFgf(FdoByteArray*& ewkbBytes,
                                       FdoByte* fgf,
                                       pgdrv_ulong size,
                                       pgdrv_ulong srid,
                                       pgdrv_ulong& currentByte,
                                       pgdrv_ulong& totalDims)
{
    pgdrv_ulong geomType = *(pgdrv_ulong*)(&fgf[currentByte]);
    currentByte += sizeof(geomType);

    pgdrv_ulong geomFlags = geomType;

    if ( srid > 0 ) 
        geomFlags = (geomFlags | eWkbFlagSrid);

    unsigned char endianByte = eWkbNDR;
    ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(unsigned char), (FdoByte*)&endianByte);

    // Processing:
    // - for simple geometries types, just copy the array
    // - for multi-geometries, process sub-geometries recursively
    //
    switch(geomType)
    {
    case FdoGeometryType_MultiPoint:
    case FdoGeometryType_MultiGeometry:
    case FdoGeometryType_MultiLineString:
    case FdoGeometryType_MultiPolygon:
        {
            pgdrv_ulong numSubGeoms = *(pgdrv_ulong*)(&fgf[currentByte]);
            currentByte += sizeof(numSubGeoms);

            FdoInt32 flagsPosn = ewkbBytes->GetCount();
            
            ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(geomFlags), (FdoByte*)&geomFlags);

            if ( srid > 0 ) 
                ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(srid), (FdoByte*)&srid);

            ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(numSubGeoms), (FdoByte*)&numSubGeoms);

            pgdrv_ulong subTotalDims = 0;

            for (pgdrv_ulong i = 0;  i < numSubGeoms && currentByte < size;  i++)
            {
                // Recurse on sub-geometry.
                ExtendedWkbFromFgf(ewkbBytes, fgf, size, srid, currentByte, subTotalDims);
            }

            FdoByte* ewkbData = ewkbBytes->GetData();

            geomFlags = (geomFlags | subTotalDims);

            *(pgdrv_ulong*)(&ewkbData[flagsPosn]) = geomFlags;
        }
        break;
    default:
        {
            pgdrv_ulong geomDim = *(pgdrv_ulong*)(&fgf[currentByte]);
            currentByte += sizeof(geomDim);

            // Dimensionality
            if ( geomDim & FdoDimensionality_Z )
            {
                geomFlags = (geomFlags | eWkbFlagZ);
                totalDims = (totalDims | eWkbFlagZ);
            }

            if ( geomDim & FdoDimensionality_M )
            {
                geomFlags = (geomFlags | eWkbFlagM);
                totalDims = (totalDims | eWkbFlagM);
            }

            ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(geomFlags), (FdoByte*)&geomFlags);

            if ( srid > 0 ) 
                ewkbBytes = FdoByteArray::Append(ewkbBytes, sizeof(srid), (FdoByte*)&srid);

            CopyGeom(ewkbBytes, fgf, size, currentByte, geomType, geomDim );
        }
        break;
    }
}

void CopyGeom(FdoByteArray*& dest,
                                       FdoByte* src,
                                       pgdrv_ulong size,
                                       pgdrv_ulong& currentByte,
                                       pgdrv_ulong geomType,
                                       pgdrv_ulong geomDim)
{
    // Valid combinations: X,Y or X,Y,Z or X,Y,M or X,Y,Z,M
    pgdrv_ulong ordinateSize =
        sizeof(double) * GetOrdinatesFromDimension(geomDim);

    switch (geomType)
    {
    case FdoGeometryType_Point:
        {
            dest = FdoByteArray::Append(dest, ordinateSize, &src[currentByte]);
            currentByte += ordinateSize;
        }
        break;
    case FdoGeometryType_LineString:
        {
            pgdrv_ulong numCoords = *(pgdrv_ulong*)(&src[currentByte]);
            dest = FdoByteArray::Append(dest, sizeof(numCoords), (FdoByte*)&numCoords);
            currentByte += sizeof(numCoords);

            pgdrv_ulong ordinatesSize =
                numCoords * ordinateSize;
            dest = FdoByteArray::Append(dest, ordinatesSize, &src[currentByte]);
            currentByte += ordinatesSize;

            //TODO: Add post-condition
        }
        break;
    case FdoGeometryType_Polygon:
        {
            pgdrv_ulong numRings = *(pgdrv_ulong*)(&src[currentByte]);
            dest = FdoByteArray::Append(dest, sizeof(numRings), (FdoByte*)&numRings);
            currentByte += sizeof(numRings);

            for (pgdrv_ulong i = 0;  i < numRings && currentByte < size;  i++)
            {
                pgdrv_ulong numCoords = *(pgdrv_ulong*)(&src[currentByte]);
                dest = FdoByteArray::Append(dest, sizeof(numCoords), (FdoByte*)&numCoords);
                currentByte += sizeof(numCoords);

                pgdrv_ulong ordinatesSize =
                    numCoords * ordinateSize;
                dest = FdoByteArray::Append(dest, ordinatesSize, &src[currentByte]);
                currentByte += ordinatesSize;
            }

        }
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }
}

pgdrv_ulong GetOrdinatesFromDimension(pgdrv_ulong dim)
{
   pgdrv_ulong value = 0;

    if (dim == FdoDimensionality_XY)
        value = 2;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_Z))
        value = 3;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_M))
        value = 3;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M))
        value = 4;

    return value;
}

