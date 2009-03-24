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
#include "mgIPolygonImp.h"
#include <FdoGeometry.h>
#include "mgILinearRing.h"
#include "mgObjectFactory.h"

FdoPolygonCollection* NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::GetImpObj()
{
	return static_cast<FdoPolygonCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

FdoIPolygon* NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::GetImpObj()
{
	return static_cast<FdoIPolygon*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::IPolygonImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::ExteriorRing::get()
{
	FdoILinearRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetExteriorRing())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(IntPtr(ret), true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::InteriorRingCount::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRingCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::GetInteriorRing(System::Int32 index)
{
	FdoILinearRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRing(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(IntPtr(ret), true);
}

//-----------------------------------------------------------------------
// PolygonCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::PolygonCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::PolygonCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoPolygonCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygon^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon^ NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Item::get(System::Int32 index)
{
	FdoIPolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(value)->GetImpObj())));
}
