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

/// <summary>The RdbmsSpatialIndexType enumeration defines how the SDO spatial data indexing
/// should be handled by the provider.</summary>
/// <param name="RdbmsSpatialIndexType_QuadTree">The coordinate space is subjected to tessellation,
/// which defines exclusive and exhaustive cover tiles for every stored geometry</param>
/// <param name="RdbmsSpatialIndexType_RTree">An R-tree index approximates each geometry by a single rectangle that minimally
/// encloses the geometry (called the minimum bounding rectangle, or MBR).</param>
public __value enum RdbmsSpatialIndexType
{
    // The spatial indexing schema is QuadTree
    RdbmsSpatialIndexType_QuadTree = SpatialIndexType_QuadTree,

    // The spatial indexing schema is RTree
    RdbmsSpatialIndexType_RTree = SpatialIndexType_RTree
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

