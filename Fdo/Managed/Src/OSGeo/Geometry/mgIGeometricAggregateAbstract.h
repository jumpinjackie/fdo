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

#include "mgIGeometry.h"

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (interfaces)
/// \interface IGeometricAggregateAbstract
/// \brief
/// The IGeometricAggregateAbstract class is an aggregate Geometry (abstract) type.
/// IGeometricAggregateAbstract is a single Geometry that is a collection of other geometries.
/// There is no requirement that the geometries interact spatially.
/// Non-abstract derived types should have at least one accessor that returns objects of the appropriate contained type.
public __gc __interface IGeometricAggregateAbstract : public NAMESPACE_OSGEO_GEOMETRY::IGeometry
{
public:

    /// \brief
    /// Gets the number of Geometries in this collection.  
    /// 
    /// \return
    /// Returns the number of Geometries
    /// 
	__property System::Int32 get_Count();
};

END_NAMESPACE_OSGEO_GEOMETRY


