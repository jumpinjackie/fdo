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
public __gc __interface IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The ICurveAbstract class is a curve Geometry type (abstract). ICurveAbstract is the most general curve type.  
public __gc __interface ICurveAbstract : public NAMESPACE_OSGEO_GEOMETRY::IGeometry
{
public:
    /// \brief
    /// Gets the starting position of this Curve.
    /// 
    /// \remarks
    /// This is the start of the entire curve.  In cases where a derived type
    /// is composed of multiple contiguous parts, this position is the start of the
    /// first of those parts.
    /// 
    /// \return
    /// Returns the starting position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_StartPosition();
	
    /// \brief
    /// Gets the ending position of this Curve.
    /// 
    /// \remarks
    /// This is the end of the entire curve.  In cases where a derived type
    /// is composed of multiple contiguous parts, this position is the end of the
    /// last of those parts.
    /// 
    /// \return
    /// Returns the ending position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_EndPosition();
	
    /// \brief
    /// Gets the closure state for the curve.
    /// 
    /// \remarks
    /// The meaning behind this method is not guaranteed
    /// to be uniform between derived types or between implementations
    /// of this package.
    /// It may represent a computed value, an explicit attribute, or be true by definition.
    /// As a computed value, the result is typically from simply testing the starting and 
    /// ending positions for exact equality.  This is only reliable in floating
    /// point arithmetic if these data have identical origins.
    /// As an explicit attribute, it would be persisted with the Geometry and 
    /// typically denoted by a parameter in the relevant factory method.  
    /// Some Geometry types are closed by definition.
    /// 
    /// \return
    /// Returns 'true' if the curve is closed, and false otherwise
    /// 
	__property System::Boolean get_IsClosed();
};
END_NAMESPACE_OSGEO_GEOMETRY


