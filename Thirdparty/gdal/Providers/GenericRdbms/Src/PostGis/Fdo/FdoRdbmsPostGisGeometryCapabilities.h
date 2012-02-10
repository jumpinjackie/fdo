//
// Copyright (C) 2006 Refractions Research, Inc. 
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
#ifndef FDORDBMSPOSTGISGEOMETRYCAPABILITIES_H
#define FDORDBMSPOSTGISGEOMETRYCAPABILITIES_H

#include "../../Fdo/Capability/FdoRdbmsGeometryCapabilities.h"
#include <FdoStd.h>

/// \brief
/// The FdoRdbmsGeometryCapabilities class delineates available
/// support for raster
/// processing from a provider.
///
/// \sa FdoIGeometryCapabilities
///
class FdoRdbmsPostGisGeometryCapabilities :
    public FdoRdbmsGeometryCapabilities
{
public:

    /// Default constructor.
    FdoRdbmsPostGisGeometryCapabilities();

    /// Return the list of supported geometry types.
    /// For example, if a client wanted to know if a provider
    /// supported multi-polygons, it would call GetGeometryTypes and
    /// check if the MultiPolygon type was listed.
    ///
    /// @param length [out] - number of geometry types.
    /// @param The list of geometry types.
    ///
    virtual FdoGeometryType* GetGeometryTypes(FdoInt32& length);

    /// Return the list of supported component types.
    /// For example, if a client wanted to know if circular arcs
    /// were supported by a provider, it would call GetGeometryComponentTypes
    /// and check for CircularArcSegment in the returned list.
    ///
    /// @param length [out] - number of component types.
    /// @returns The list of component types.
    ///
    virtual FdoGeometryComponentType* GetGeometryComponentTypes(
        FdoInt32& length);

    /// Returns the supported dimensionalities which are based on the bit
    /// masks defined in the FdoDimensionality enum.
    /// The Z and M bits indicate if the provider supports 3D or
    /// Measure dimensions in geometry data. The XY bit should be set too.
    ///
    /// @returns The dimensionalities.
    ///
    /// \todo TODO: mloskot - move the body to .cpp with inline qualificator.
    ///
    virtual FdoInt32 GetDimensionalities();

protected:

    /// Destructor.
    ~FdoRdbmsPostGisGeometryCapabilities();

    /// \todo TODO: mloskot - move the body to .cpp with inline qualificator.
    virtual void Dispose();
};

#endif // FDORDBMSPOSTGISGEOMETRYCAPABILITIES_H
