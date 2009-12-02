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

#include "mgICurveSegmentAbstract.h"

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IDirectPosition;
public __gc class DirectPositionCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::ILineStringSegment
/// \brief
/// The ILineStringSegment class is a LineString curve segment type.  
/// The shape of ILineStringSegment is the set of positions defined by the contained collection, 
/// plus linear interpolation between consecutive points.
/// This is a helper type for Geometries in the Geometry package.
/// Note: It does not derive from IGeometry.
public __gc __interface ILineStringSegment : public NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract
{
public:
    /// \brief
    /// Gets the number of positions in this object.
    /// 
    /// \return
    /// Returns the number of positions
    /// 
	__property System::Int32 get_Count();

    /// \brief
    /// Gets the position at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// 
    /// \return
    /// Returns the position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_Item(System::Int32 index);

    /// \brief
    /// Gets a collection of all of the positions in this object.
    /// 
    /// \return
    /// Returns the positions
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection *get_Positions();
};

END_NAMESPACE_OSGEO_GEOMETRY


