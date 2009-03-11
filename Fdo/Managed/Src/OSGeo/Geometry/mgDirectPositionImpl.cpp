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
#include <FdoGeometry.h>

FdoDirectPositionImpl* NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::GetImpObj()
{
	return static_cast<FdoDirectPositionImpl*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl() 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create()), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create(coordinateX, coordinateY)), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create(coordinateX, coordinateY, coordinateZ)), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ, System::Double coordinateM) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create(coordinateX, coordinateY, coordinateZ, coordinateM)), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(DirectPositionImpl^ position) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create(*(position->GetImpObj()))), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ position) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDirectPositionImpl::Create(static_cast<IDirectPositionImp^>(position)->GetImpObj())), true))
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::DirectPositionImpl(System::IntPtr unmanaged, System::Boolean autoDelete) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl^ NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::operator=(IDirectPosition^ right)
{
	EXCEPTION_HANDLER((this->GetImpObj())->operator=(*(static_cast<IDirectPositionImp^>(right)->GetImpObj())));
    return this;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Equals(System::Object^ obj)
{
	NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl^ directPosition = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl^>(obj);
    
    if(nullptr == (System::Object^)directPosition)
        return false;
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = this->GetImpObj()->operator==(*(directPosition->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::GetHashCode()
{
    return UnmanagedObject.ToInt32();
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::X::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetX())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::X::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetX(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Y::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetY())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Y::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetY(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Z::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetZ());
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Z::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZ(value));
}

System::Double NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::M::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetM())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::M::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetM(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Dimensionality::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionImpl::Dimensionality::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDimensionality(value));
}
