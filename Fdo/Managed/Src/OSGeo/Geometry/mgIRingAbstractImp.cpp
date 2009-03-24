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
#include "mgIRingAbstractImp.h"
#include <FdoGeometry.h>
#include "mgIEnvelope.h"
#include "mgObjectFactory.h"

FdoIRingAbstract* NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp::GetImpObj()
{
	return static_cast<FdoIRingAbstract*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp::IRingAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp::Envelope::get()
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEnvelope())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(IntPtr(ret), true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp::Dimensionality::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

