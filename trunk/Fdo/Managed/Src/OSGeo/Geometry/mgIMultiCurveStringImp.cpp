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
#include "mgIMultiCurveStringImp.h"
#include <FdoGeometry.h>
#include "mgICurveString.h"
#include "mgObjectFactory.h"

FdoIMultiCurveString* NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp::GetImpObj()
{
	return static_cast<FdoIMultiCurveString*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp::IMultiCurveStringImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IGeometricAggregateAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::ICurveString^ NAMESPACE_OSGEO_GEOMETRY::IMultiCurveStringImp::default::get(System::Int32 index)
{
	FdoICurveString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveString(IntPtr(ret), true);
}

