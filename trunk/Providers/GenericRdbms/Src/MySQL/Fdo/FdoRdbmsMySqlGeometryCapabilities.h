#ifndef _IRDBMSMYSQLGEOMETRYCAPABILITIES_H_
#define _IRDBMSMYSQLGEOMETRYCAPABILITIES_H_
//
 //
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
//
/// <library>FDO.lib</library>

#ifdef _WIN32
#pragma once
#endif
#include <FdoStd.h>
#include "../../Fdo/Capability/FdoRdbmsGeometryCapabilities.h"

/// <summary>The FdoRdbmsGeometryCapabilities class delineates available support for raster processing from a provider.</summary>
class FdoRdbmsMySqlGeometryCapabilities : public FdoRdbmsGeometryCapabilities
{

protected:
    ~FdoRdbmsMySqlGeometryCapabilities(void) { }
    virtual void Dispose() { delete this; }
public:
    FdoRdbmsMySqlGeometryCapabilities(void) { }


    /// <summary>Return the list of supported geometry types. For example, if a client wanted to know if a provider supported
    /// multi-polygons, it would call GetGeometryTypes and check if the MultiPolygon type was listed.</summary>
    /// <param name="length">Output the number of geometry types.</param>
    /// <returns>Returns the list of geometry types</returns>
    virtual FdoGeometryType* GetGeometryTypes( FdoInt32& length);

    /// <summary>Return the list of supported component types. For example, if a client wanted to know if circular arcs were supported
    /// by a provider, it would call GetGeometryComponentTypes and check for CircularArcSegment in the returned list.</summary>
    /// <param name="length">Output the number of component types.</param>
    /// <returns>Returns the list of component types</returns>
    virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& length );

    /// <summary>Returns the supported dimensionalities which are based on the bit masks defined in the FdoDimensionality enum.
    /// The Z and M bits indicate if the provider supports 3d or Measure dimensions in geometry data. The XY bit should be set too.</summary>
    /// <returns>Returns the dimensionalities</returns>
    virtual FdoInt32 GetDimensionalities()
    {
        return (0 | FdoDimensionality_XY );
    }
};

#endif //_IGEOMETRYCAPABILITIES_H_
