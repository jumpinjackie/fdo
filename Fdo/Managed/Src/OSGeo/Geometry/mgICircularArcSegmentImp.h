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
#include "mgIArcSegmentAbstractImp.h"
#include "mgICircularArcSegment.h"

class FdoICircularArcSegment;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The ICircularArcSegment class is a circular arc curve segment
public ref class ICircularArcSegmentImp :
	public NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment, public NAMESPACE_OSGEO_GEOMETRY::IArcSegmentAbstractImp
{
public:
    /// \brief
    /// Constructs an ICircularArcSegmentImp managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ICircularArcSegmentImp(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	FdoICircularArcSegment* GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


