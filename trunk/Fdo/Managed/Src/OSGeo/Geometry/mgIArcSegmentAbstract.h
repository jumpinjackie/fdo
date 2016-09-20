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
interface class ICurveSegmentAbstract;
interface class IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \interface OSGeo::FDO::Geometry::IArcSegmentAbstract
/// \brief
/// The IArcSegmentAbstract class is an arc curve segment (abstract)
public interface class IArcSegmentAbstract : public NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract
{
public:
	
    /// \brief
    /// Gets some position along the curve, between the starting and ending positions.
    /// 
    /// \remarks
    /// Depending on the derived type and its implementation, this may be a 
    /// computed value, or a persisted value used as part
    /// of the definition of the curve segment.  This position is the only
    /// means to deduce the curve segment's orientation in some cases, such as
    /// when it is closed or vertically aligned ('on edge' when looking along the Z axis).
    /// 
    /// \return
    /// Returns a midpoint on the curve
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ MidPoint
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


