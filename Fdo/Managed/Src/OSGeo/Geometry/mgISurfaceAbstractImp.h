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
#include "mgISurfaceAbstract.h"

class FdoISurfaceAbstract;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The ISurfaceAbstractImp class is a surface (concrete) Geometry type.
public ref class ISurfaceAbstractImp :
	public NAMESPACE_OSGEO_GEOMETRY::IGeometryImp, public NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstract
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
	ISurfaceAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	FdoISurfaceAbstract* GetImpObj();

public:
};
END_NAMESPACE_OSGEO_GEOMETRY


