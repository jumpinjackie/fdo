/*
* 
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

#include "stdafx.h"
#include "mgSpatialGridTransform.h"
#include <FdoSpatial.h>

FdoSpatialGridTransform* NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::GetImpObj()
{
	return static_cast<FdoSpatialGridTransform*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::SpatialGridTransform(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::SpatialGridTransform(NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ extents, System::Double tolerance)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSpatialGridTransform::Create((extents == nullptr ? nullptr : static_cast<FdoIEnvelope*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp^>(extents)->UnmanagedObject.ToPointer())), tolerance)), true))
	
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::TransformToGrid(System::Double coordinateX, System::Double coordinateY, System::Int64% gridX, System::Int64% gridY)
{
    pin_ptr<FdoInt64> upXGrid = &gridX;
    pin_ptr<FdoInt64> upYGrid = &gridY;

	EXCEPTION_HANDLER(GetImpObj()->TransformToGrid(coordinateX, coordinateY, upXGrid, upYGrid))
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialGridTransform::TransformFromGrid(System::Int64 gridX, System::Int64 gridY, System::Double% coordinateX, System::Double% coordinateY)
{
    pin_ptr<FdoDouble> upX = &coordinateX;
    pin_ptr<FdoDouble> upY = &coordinateY;

	EXCEPTION_HANDLER(GetImpObj()->TransformFromGrid(gridX, gridY, upX, upY))
}