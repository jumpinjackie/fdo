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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

/// <summary>The RdbmsSpatialIndexDimensionType enumeration defines the spatial data number of dimensions
/// used by spatial indexing.</summary>
/// <param name="RdbmsSpatialIndexDimension_Default">The number of dimensions used by the spatial index is defaulted.</param>
/// <param name="RdbmsSpatialIndexDimension_2D">The number of dimensions used by the spatial index is 2.</param>
/// <param name="RdbmsSpatialIndexDimension_3D">The number of dimensions used by the spatial index is 3.</param>

public enum class RdbmsSpatialIndexDimensionType
{
    // The number of dimensions used by the spatial index is defaulted
    RdbmsSpatialIndexDimension_Default = SpatialIndexDimension_Default,

    // The number of dimensions used by the spatial index is 2
    RdbmsSpatialIndexDimension_2D =  SpatialIndexDimension_2D,

    // The number of dimensions used by the spatial index is 3
    RdbmsSpatialIndexDimension_3D = SpatialIndexDimension_3D
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

