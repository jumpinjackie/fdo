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

#pragma  once

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (enums)
/// \brief
/// The GeometricType enumeration categorizes all of the geometry types
/// supported by FDO based on their geometric dimensionality.
public __value enum GeometricType
{
    /// Represents 0-dimensional geometric primitives, such as Points.
    GeometricType_Point = 0x01,

    /// Represents 1 dimensional geometric primitives, such as Curves and Lines.
    GeometricType_Curve = 0x02,

    /// Represents 2 dimensional geometric primitives, such as Polygons.
    GeometricType_Surface = 0x04,

    /// Represents 3 dimensional geometric primitives, such as Cubes.
    GeometricType_Solid = 0x08,

	GeometricType_All = 0x01|0x02|0x04|0x08
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


