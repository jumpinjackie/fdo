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

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \brief
/// The FdoSpatialOperations enumeration lists the spatial conditions available.
/// <param name="FdoSpatialOperations_Contains">
/// Test to see if the geometric property value spatially contains the
/// literal geometric value
/// </param>
/// <param name="FdoSpatialOperations_Crosses">
/// Test to see if the geometric property value spatially crosses the given
/// geometry
/// </param>
/// <param name="FdoSpatialOperations_Disjoint">
/// Test to see if the geometric property value spatially is spatially
/// disjoint from the given geometry
/// </param>
/// <param name="FdoSpatialOperations_Equals">
/// Test to see if the geometric property value is spatially equal to the
/// given geometry
/// </param>
/// <param name="FdoSpatialOperations_Intersects">
/// Test to see if the geometric property value spatially intersects the
/// given geometry
/// </param>
/// <param name="FdoSpatialOperations_Overlaps">
/// Test to see if the geometric property value spatially overlaps the given
/// geometry
/// </param>
/// <param name="FdoSpatialOperations_Touches">
/// Test to see if the geometric property value spatially touches the given
/// geometry
/// </param>
/// <param name="FdoSpatialOperations_Within">
/// Test to see if the geometric property value is spatially within the
/// given geometry
/// </param>
/// <param name="FdoSpatialOperations_CoveredBy">
/// Test to see if the geometric property value is covered by the interior
/// and boundary of the given geometry
/// </param>
/// <param name="FdoSpatialOperations_Inside">
/// Test to see if the geometric property value is inside the interior
/// of the given geometry, not touching the boundary
/// </param>
/// <param name="FdoSpatialOperations_EnvelopeIntersects">
/// Test to see if the envelope of the referenced geometric property value intersects the
/// given geometry
/// </param>
[System::FlagsAttribute]
public __value enum SpatialOperations
{
    /// Test to see if the geometric property value spatially contains the
    /// literal geometric value.
	SpatialOperations_Contains = FdoSpatialOperations_Contains,

    /// Test to see if the geometric property value spatially crosses the given
    /// geometry.
	SpatialOperations_Crosses = FdoSpatialOperations_Crosses,

    /// Test to see if the geometric property value spatially is spatially
    /// disjoint from the given geometry.
	SpatialOperations_Disjoint = FdoSpatialOperations_Disjoint,

    /// Test to see if the geometric property value is spatially equal to the
    /// given geometry.
	SpatialOperations_Equals = FdoSpatialOperations_Equals,

    /// Test to see if the geometric property value spatially intersects the
    /// given geometry.
	SpatialOperations_Intersects = FdoSpatialOperations_Intersects,

    /// Test to see if the geometric property value spatially overlaps the given
    /// geometry.
	SpatialOperations_Overlaps = FdoSpatialOperations_Overlaps,

    /// Test to see if the geometric property value spatially touches the given
    /// geometry.
 	SpatialOperations_Touches = FdoSpatialOperations_Touches,

    /// Test to see if the geometric property value is spatially within the
    /// given geometry.
	SpatialOperations_Within = FdoSpatialOperations_Within,

    /// Test to see if the geometric property value is covered by the interior
    /// and boundary of the given geometry.
	SpatialOperations_CoveredBy = FdoSpatialOperations_CoveredBy,

    /// Test to see if the geometric property value is inside the interior
    /// of the given geometry, not touching the boundary.
	SpatialOperations_Inside = FdoSpatialOperations_Inside,

    /// Test to see if the envelope of the referenced geometric property value spatially intersects the
    /// given geometry.
	SpatialOperations_EnvelopeIntersects = FdoSpatialOperations_EnvelopeIntersects

};

END_NAMESPACE_OSGEO_FDO_FILTER


