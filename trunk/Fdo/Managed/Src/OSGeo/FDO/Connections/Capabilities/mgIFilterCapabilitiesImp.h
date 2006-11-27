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

#include "FDO\Connections\Capabilities\mgIFilterCapabilities.h"

class FdoIFilterCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \brief
/// The FdoIFilterCapabilities interface declares the feature provider's level of support for FdoFilter classes.
private __gc class IFilterCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities
{
public:
    /// \brief
    /// Returns an array of FdoConditionType objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of condition types
    /// 
    /// \return
    /// Returns the list of condition types
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::ConditionType get_ConditionTypes() [];

    /// \brief
    /// Returns an array of FdoSpatialOperation objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of spatial operations
    /// 
    /// \return
    /// Returns the list of spartial operations
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations get_SpatialOperations() [];

    /// \brief
    /// Returns an array of FdoDistanceOperation objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of distance operations
    /// 
    /// \return
    /// Returns the list of distance operations
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations get_DistanceOperations() [];

    /// \brief
    /// Determines if the feature provider supports geodesic distance measurement. Returns false if the feature provider supports only linear distance measurement.
    /// 
    /// \return
    /// Returns true if the feature provider supports geodesic distance measurement
    /// 
	System::Boolean SupportsGeodesicDistance();

    /// \brief
    /// Determines if spatial and distance operations can be applied between two 
    /// geometric properties. Returns false if spatial and distance operations can be applied only between 
    /// a geometric property and a literal geometry.
    /// 
    /// \return
    /// Returns true if spatial and distance operations can be applied
    /// 
	System::Boolean SupportsNonLiteralGeometricOperations();

protected:
	System::Void ReleaseUnmanagedObject();

public private:
	IFilterCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIFilterCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


