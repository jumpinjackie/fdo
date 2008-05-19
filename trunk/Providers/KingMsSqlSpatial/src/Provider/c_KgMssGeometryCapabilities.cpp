/*
* Copyright (C) 2007  Haris Kurtagic
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

#include "stdafx.h"

c_KgMssGeometryCapabilities::c_KgMssGeometryCapabilities ()
{
}

c_KgMssGeometryCapabilities::~c_KgMssGeometryCapabilities ()
{
}

void c_KgMssGeometryCapabilities::Dispose()
{
    delete this;
}

/// <summary>Return the list of supported geometry types. For example, if a client wanted to know if a provider supported
/// multi-polygons, it would call GetGeometryTypes and check if the MultiPolygon type was listed.</summary>
/// <param name="length">Output the number of geometry types.</param> 
/// <returns>Returns the list of geometry types</returns> 
FdoGeometryType* c_KgMssGeometryCapabilities::GetGeometryTypes (FdoInt32& Length)
{
    static const FdoGeometryType geomTypes[] =
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        FdoGeometryType_CurveString,
        FdoGeometryType_CurvePolygon,
        FdoGeometryType_MultiCurveString,
        FdoGeometryType_MultiCurvePolygon
    };

    Length = ELEMENTS(geomTypes);
    return ((FdoGeometryType *)geomTypes);
}//end of c_KgMssGeometryCapabilities::GetGeometryTypes 

/// <summary>Return the list of supported component types. For example, if a client wanted to know if circular arcs were supported 
/// by a provider, it would call GetGeometryComponentTypes and check for CircularArcSegment in the returned list.</summary>
/// <param name="length">Output the number of component types.</param> 
/// <returns>Returns the list of component types</returns> 
FdoGeometryComponentType* c_KgMssGeometryCapabilities::GetGeometryComponentTypes (FdoInt32& Length)
{
    static const FdoGeometryComponentType geomComponentTypes[] =
    {
        FdoGeometryComponentType_LinearRing,
        FdoGeometryComponentType_CircularArcSegment,
        FdoGeometryComponentType_LineStringSegment,
        FdoGeometryComponentType_Ring,
    };
    Length = ELEMENTS(geomComponentTypes);
    return ((FdoGeometryComponentType *)geomComponentTypes);
}//end of 

/// <summary>Returns the supported dimensionalities which are based on the bit masks defined in the GisDimensionality enum. 
/// The Z and M bits indicate if the provider supports 3d or Measure dimensions in geometry data. The XY bit should be set too.</summary>
/// <returns>Returns the dimensionalities</returns> 
FdoInt32 c_KgMssGeometryCapabilities::GetDimensionalities ()
{
    return (FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M); 
}

