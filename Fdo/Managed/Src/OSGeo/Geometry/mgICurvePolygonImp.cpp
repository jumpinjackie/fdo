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
#include "mgICurvePolygonImp.h"
#include <FdoGeometry.h>
#include "mgIRing.h"
#include "mgObjectFactory.h"

FdoCurvePolygonCollection* NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::GetImpObj()
{
	return static_cast<FdoCurvePolygonCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

FdoICurvePolygon* NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::GetImpObj()
{
	return static_cast<FdoICurvePolygon*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::ICurvePolygonImp(System::IntPtr unmanaged, System::Boolean autoDelete) 
	: NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IRing^ NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::ExteriorRing::get()
{
	FdoIRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetExteriorRing())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(IntPtr(ret), true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::InteriorRingCount::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRingCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IRing^ NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::InteriorRing::get(System::Int32 index)
{
	FdoIRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRing(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(IntPtr(ret), true);
}

//-----------------------------------------------------------------------
// CurvePolygonCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CurvePolygonCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CurvePolygonCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoCurvePolygonCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Item::get(System::Int32 index)
{
	FdoICurvePolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp^>(value)->GetImpObj()));
}
