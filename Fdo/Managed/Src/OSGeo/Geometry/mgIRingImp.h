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

#include "mgIRingAbstractImp.h"
#include "mgIRing.h"

class FdoIRing;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class ICurveSegmentAbstract;
ref class CurveSegmentCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IRingImp class is ring Geometry helper type.  This is the most general non-abstract ring type.  
/// FdoIRing is defined by an ordered collection of contiguous curve segments.
private ref class IRingImp :
	public NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp, public NAMESPACE_OSGEO_GEOMETRY::IRing
{
public:
    /// \brief
    /// Constructs a managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed element should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IRingImp(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	FdoIRing* GetImpObj();

public:
    /// \brief
    /// Gets the number of curve segments in this ring.  
    /// 
    /// \return
    /// Returns the number of curve segments
    /// 
    property System::Int32 Count
    {
        virtual System::Int32 get();
    }

    /// \brief
    /// Gets the curve segment at the given zero-based index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of curve segments.
    /// 
    /// \return
    /// Returns the requested curve segment
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ default[System::Int32]
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ get(System::Int32 index);
    }
	
    /// \brief
    /// Gets all the curve segments.
    /// 
    /// \return
    /// Returns a collection of all the curve segments
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ CurveSegments
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ get();
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


