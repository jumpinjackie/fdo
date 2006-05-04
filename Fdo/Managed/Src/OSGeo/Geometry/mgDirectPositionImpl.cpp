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
#include "mgDirectPositionImpl.h"
#include "mgIDirectPositionImp.h"
#include <Geometry.h>

FdoDirectPositionImpl *NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::GetImpObj()
{
	return static_cast<FdoDirectPositionImpl *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl() 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(coordinateX, coordinateY), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(coordinateX, coordinateY, coordinateZ), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ, System::Double coordinateM) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(coordinateX, coordinateY, coordinateZ, coordinateM), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(DirectPositionImpl *position) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(*(position->GetImpObj())), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *position) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDirectPositionImpl::Create(static_cast<IDirectPositionImp *>(position)->GetImpObj()), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::IntPtr unmanaged, System::Boolean autoDelete) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::op_Assign(DirectPositionImpl *left, DirectPositionImpl *right)
{
	EXCEPTION_HANDLER((left->GetImpObj())->operator=(*(right->GetImpObj())));
}


System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::op_Assign(DirectPositionImpl *left, IDirectPosition *right)
{
	EXCEPTION_HANDLER((left->GetImpObj())->operator=(*(static_cast<IDirectPositionImp *>(right)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::op_Equality(DirectPositionImpl *left, DirectPositionImpl *right)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = left->GetImpObj()->operator==(*(right->GetImpObj())))
	return ret;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::op_Inequality(DirectPositionImpl *left, DirectPositionImpl *right)
{
	return !op_Equality(left, right);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Equals(System::Object* obj)
{
	NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl* directPosition = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl*>(obj);
	if (NULL == directPosition)
	{
		return false;
	}
	else
	{
		return op_Equality(this, directPosition);;
	}
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::GetHashCode()
{
	return (int)X + (int)Y + (int)Z + (int)M;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::get_X()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetX())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::set_X(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetX(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::get_Y()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetY())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::set_Y(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetY(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::get_Z()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetZ());
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::set_Z(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZ(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::get_M()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetM())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::set_M(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetM(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::get_Dimensionality()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::set_Dimensionality(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDimensionality(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
        // Add DirectPositionImpl specific code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}
