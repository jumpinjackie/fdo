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
#include "mgIGeometricAggregateAbstractImp.h"
#include "mgIMultiCurveString.h"

class FdoIMultiCurveString;
BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface ICurveString;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The IMultiCurveStringImp class is a multi-CurveString collection Geometry type.
/// The curves contained here do not have to
/// be contiguous, nor spatially interact in any other way.
private __gc class IMultiCurveStringImp : 
	public NAMESPACE_OSGEO_GEOMETRY::IGeometricAggregateAbstractImp, public NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString
{
public:
	IMultiCurveStringImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoIMultiCurveString *GetImpObj();

public:
    /// \brief
    /// Gets a CurveString at the given zero-based index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of curve strings.
    /// 
    /// \return
    /// Returns a CurveString
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::ICurveString *get_Item(System::Int32 index);
};

END_NAMESPACE_OSGEO_GEOMETRY


