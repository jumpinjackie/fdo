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
#include "mgICurveString.h"
#include "mgICurveAbstractImp.h"

class FdoICurveString;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface ICurveSegmentAbstract;
public __gc class CurveSegmentCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The ICurveStringImp class is a concrete geometric CurveString Geometry type. ICurveString is the most general non-abstract curve type.  
/// It is defined by an ordered collection of contiguous curve segments.
private __gc class ICurveStringImp 
	: public NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp, public NAMESPACE_OSGEO_GEOMETRY::ICurveString
{
public:
    /// \brief
    /// Constructs an ICurveStringImp managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ICurveStringImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoICurveString *GetImpObj();

public:
    /// \brief
    /// Gets the number of curve segments in this curve.  
    /// 
    /// \return
    /// Returns the number of curve segments
    /// 
	__property System::Int32 get_Count();
	
    /// \brief
    /// Gets the curve segment at the given zero-based index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of curve segments.
    /// 
    /// \return
    /// Returns the requested curve segment
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *get_Item(System::Int32 index);
	
    /// \brief
    /// Gets all the curve segments.
    /// 
    /// \return
    /// Returns a collection of all the curve segments
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection *get_CurveSegments();
};

END_NAMESPACE_OSGEO_GEOMETRY


