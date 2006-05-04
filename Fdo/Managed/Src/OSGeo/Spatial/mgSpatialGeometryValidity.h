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

BEGIN_NAMESPACE_OSGEO_SPATIAL
/// \brief
///  SpatialGeometryValidity is an enumeration to denote the validity of a Geometry
/// during a test of type and dimensionality..
/// <param name="SpatialGeometryValidity_None">
/// Indicates no specific validity; used for unknown or default states.
/// </param>
/// <param name="SpatialGeometryValidity_Valid">
/// Geometry passes a test.
/// </param>
/// <param name="SpatialGeometryValidity_Invalid">
/// Geometry fails a test.
/// </param>
/// <param name="SpatialGeometryValidity_InvalidButCanBeApproximated">
/// Geometry fails a test, but could be approximated 
/// using allowed types.
/// </param>
/// <param name="SpatialGeometryValidity_InvalidDueToDimensionality">
/// Geometry fails a test due to having an disallowed dimensionality.
/// </param>
public __value enum SpatialGeometryValidity
{
	SpatialGeometryValidity_None = FdoSpatialGeometryValidity_None,

	SpatialGeometryValidity_Valid = FdoSpatialGeometryValidity_Valid,

	SpatialGeometryValidity_Invalid = FdoSpatialGeometryValidity_Invalid,

	SpatialGeometryValidity_InvalidButCanBeApproximated = FdoSpatialGeometryValidity_InvalidButCanBeApproximated,

	SpatialGeometryValidity_InvalidDueToDimensionality = FdoSpatialGeometryValidity_InvalidDueToDimensionality,
};
END_NAMESPACE_OSGEO_SPATIAL


