// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
/// <summary>
/// A stream writer for the FGF geometry data
/// format.  This class extends the functionality of the OpenGIS standard.
/// </summary>

#pragma once

#ifdef WIN32
#include "windows.h"
#else
#include <memory.h>
#include <stdlib.h>
#endif


enum FgfGeometryType
{
        FgfGeometryType_None            = 0,
        FgfGeometryType_Point           = 1,
        FgfGeometryType_LineString      = 2,
        FgfGeometryType_Polygon         = 3,
        FgfGeometryType_MultiPoint      = 4,
        FgfGeometryType_MultiLineString = 5,
        FgfGeometryType_MultiPolygon    = 6,
        FgfGeometryType_MultiGeometry   = 7,
        FgfGeometryType_Text            = 8,
        FgfGeometryType_Raster          = 9
};

// the type of coordinates used for this object.
enum FgfCoordinateType
{
	FgfCoordinateType_XY	= 0,
	FgfCoordinateType_XYZ	= 1,
	FgfCoordinateType_M	= 2
};

// A stream writer for the FGF geometry data
// format.  This class extends the functionality of the OpenGIS standard.


class FgfStreamWriter
{

public:
    FgfStreamWriter();

    ~FgfStreamWriter();

public:
    long getLength();

    const unsigned char* getData();

    void WriteGeometryType(FgfGeometryType value);

    void WriteCoordinateType(FgfCoordinateType value);

    void WriteInt(int value);

    void WriteDouble(double value);

    void WriteDoubles(double* src, int count);

    void WriteByte(unsigned char value);

    void WriteChar(const wchar_t value);

    void Clear();

private:
    //Write cursor for FGF data
    unsigned char* m_data;

    //Last byte of FGF data
    unsigned char* m_end_data;

    //Beginning of FGF data
    unsigned char* m_begin_data;

    // makes sure the byte array is big enough
    void EnsureCapacity(int requiredCapacity);
};

