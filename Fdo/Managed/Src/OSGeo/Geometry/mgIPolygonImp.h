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
#include "mgISurfaceAbstractImp.h"
#include "mgIPolygon.h"

class FdoIPolygon;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class ILinearRing;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IPolygonImp class is a polygon Geometry type.  
/// IPolygonImp is defined by one exterior boundary and zero or more 
/// interior boundaries.
/// Each of the boundaries must be a closed, simple LinearRing.
public ref class IPolygonImp :
	public NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp, public NAMESPACE_OSGEO_GEOMETRY::IPolygon
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
	IPolygonImp(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	FdoIPolygon* GetImpObj();

public:
    /// \brief
    /// Gets the ring defining the outer boundary.
    /// 
    /// \return
    /// Returns the exterior ring
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ ExteriorRing
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ get();
    }

    /// \brief
    /// Gets the number of interior rings.
    /// 
    /// \return
    /// Returns the number of interior rings
    /// 
    property System::Int32 InteriorRingCount
    {
        virtual System::Int32 get();
    }

    /// \brief
    /// Gets the interior ring at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of interior rings.
    /// 
    /// \return
    /// Returns the interior ring
    /// 
	virtual NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ GetInteriorRing(System::Int32 index);
};

END_NAMESPACE_OSGEO_GEOMETRY


