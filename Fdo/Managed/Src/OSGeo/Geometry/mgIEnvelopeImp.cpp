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
#include "mgIEnvelopeImp.h"
#include <FdoGeometry.h>

FdoIEnvelope* NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::GetImpObj()
{
	return static_cast<FdoIEnvelope*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::IEnvelopeImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MinX::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinX())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MinY::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinY())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MinZ::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMinZ())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MaxX::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxX())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MaxY::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxY())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::MaxZ::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetMaxZ())
	return ret;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::IEnvelopeImp::IsEmpty::get()
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetIsEmpty())
	return ret;
}

