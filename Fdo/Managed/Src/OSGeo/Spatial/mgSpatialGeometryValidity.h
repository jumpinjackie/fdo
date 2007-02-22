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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_SPATIAL
/// \endcond

/// \ingroup (OSGeoFDOSpatial)
/// \brief
/// SpatialGeometryValidity is an enumeration to denote the validity of a Geometry
/// during a test of type and dimensionality..
public __value enum SpatialGeometryValidity
{
    /// Indicates no specific validity; used for unknown or default states.
	SpatialGeometryValidity_None = FdoSpatialGeometryValidity_None,

    /// Geometry passes a test.
	SpatialGeometryValidity_Valid = FdoSpatialGeometryValidity_Valid,

    /// Geometry fails a test.
	SpatialGeometryValidity_Invalid = FdoSpatialGeometryValidity_Invalid,

    /// Geometry fails a test, but could be approximated 
    /// using allowed types.
	SpatialGeometryValidity_InvalidButCanBeApproximated = FdoSpatialGeometryValidity_InvalidButCanBeApproximated,

    /// Geometry fails a test due to having an disallowed dimensionality.
	SpatialGeometryValidity_InvalidDueToDimensionality = FdoSpatialGeometryValidity_InvalidDueToDimensionality,
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_SPATIAL
/// \endcond
