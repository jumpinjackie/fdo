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
#include "mgIPointImp.h"
#include <FdoGeometry.h>
#include "mgIDirectPosition.h"
#include "mgObjectFactory.h"

FdoPointCollection* NAMESPACE_OSGEO_GEOMETRY::PointCollection::GetImpObj()
{
	return static_cast<FdoPointCollection*>(UnmanagedObject.ToPointer());
}

FdoIPoint* NAMESPACE_OSGEO_GEOMETRY::IPointImp::GetImpObj()
{
	return static_cast<FdoIPoint*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IPointImp::IPointImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IGeometryImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::IPointImp::Position::get()
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::IPointImp::GetPositionByMembers(
		System::Double% coordinateX,
		System::Double% coordinateY,
		System::Double% coordinateZ,
		System::Double% coordinateM,
		System::Int32% dimensionality)
{
	pin_ptr<FdoDouble> ix = &coordinateX;
	pin_ptr<FdoDouble> iy = &coordinateY;
	pin_ptr<FdoDouble> iz = &coordinateZ;
	pin_ptr<FdoDouble> im = &coordinateM;
	pin_ptr<FdoInt32> idim = &dimensionality;

	EXCEPTION_HANDLER(GetImpObj()->GetPositionByMembers(ix, iy, iz, im, idim));
}

//-----------------------------------------------------------------------
// PointCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::PointCollection::PointCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
    : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::PointCollection::PointCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoPointCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IPoint^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i];
}

System::Object^ NAMESPACE_OSGEO_GEOMETRY::PointCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPoint^>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::IPoint^ NAMESPACE_OSGEO_GEOMETRY::PointCollection::Item::get(System::Int32 index)
{
	FdoIPoint* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp^>(value)->GetImpObj())));
}
