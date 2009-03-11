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

#pragma once

#include "FDO\Connections\Capabilities\mgIGeometryCapabilities.h"

class FdoIGeometryCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
/// The IGeometryCapabilities class delineates available support for raster processing from a provider.
private ref class IGeometryCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                              public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities
{
public:
    /// \brief
    /// Return the list of supported geometry types. For example, if a client wanted to know if a provider supported
    /// multi-polygons, it would call GetGeometryTypes and check if the MultiPolygon type was listed.
    /// 
    /// \return
    /// Returns the list of geometry types
    /// 
    virtual property array<NAMESPACE_OSGEO_COMMON::GeometryType>^ GeometryTypes
    {
        array<NAMESPACE_OSGEO_COMMON::GeometryType>^ get();
    }

    /// \brief
    /// Return the list of supported component types. For example, if a client wanted to know if circular arcs were supported 
    /// by a provider, it would call GetGeometryComponentTypes and check for CircularArcSegment in the returned list.
    /// 
    /// \return
    /// Returns the list of component types
    /// 
    virtual property array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>^ GeometryComponentTypes
    {
        array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>^ get();
    }

    /// \brief
    /// Returns the supported dimensionalities which are based on the bit masks defined in the Dimensionality enum. 
    /// The Z and M bits indicate if the provider supports 3d or Measure dimensions in geometry data. The XY bit should be set too.
    /// 
    /// \return
    /// Returns the dimensionalities
    /// 
    virtual property System::Int32 Dimensionalities
    {
        System::Int32 get();
    }

internal:
	IGeometryCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIGeometryCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


