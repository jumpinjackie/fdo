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

#include "FDO\Filter\mgConditionType.h"
#include "FDO\Filter\mgSpatialOperations.h"
#include "FDO\Filter\mgDistanceOperations.h"

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __value enum ConditionType;
public __value enum SpatialOperations;
public __value enum DistanceOperations;
END_NAMESPACE_OSGEO_FDO_FILTER

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \ingroup (interfaces)
/// \interface IFilterCapabilities
/// \brief
/// The IFilterCapabilities interface declares the feature provider's level of support for Filter classes.
public __gc __interface IFilterCapabilities : public System::IDisposable
{
public:
    /// \brief
    /// Returns an array of ConditionType objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of condition types
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::ConditionType get_ConditionTypes() [];

    /// \brief
    /// Returns an array of SpatialOperation objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of spartial operations
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations get_SpatialOperations() [];

    /// \brief
    /// Returns an array of DistanceOperation objects the feature provider supports.
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

};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


