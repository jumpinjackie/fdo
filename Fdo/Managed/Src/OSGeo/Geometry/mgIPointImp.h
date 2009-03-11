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

#include "mgIGeometryImp.h"
#include "mgIPoint.h"

class FdoIPoint;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IPointImp class is a point Geometry type.
private ref class IPointImp : 
	public NAMESPACE_OSGEO_GEOMETRY::IGeometryImp, public NAMESPACE_OSGEO_GEOMETRY::IPoint
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
	IPointImp(IntPtr unmanaged, Boolean autoDelete);

internal:
	FdoIPoint* GetImpObj();

public:
    /// \brief
    /// Gets the position of this point Geometry.
    /// 
    /// \return
    /// Returns the position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ Position
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }
	
    /// \brief
    /// Gets the position of this point Geometry, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param coordinateX 
    /// Output X ordinate value.
    /// \param coordinateY 
    /// Output Y ordinate value.
    /// \param coordinateZ 
    /// Output Z ordinate value.
    /// \param coordinateM 
    /// Output M ordinate value.
    /// \param dimensionality 
    /// Output Dimensionality of ordinates in this position.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void GetPositionByMembers(
		System::Double% coordinateX,
		System::Double% coordinateY,
		System::Double% coordinateZ,
		System::Double% coordinateM,
		System::Int32% dimensionality);
};

END_NAMESPACE_OSGEO_GEOMETRY


