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
#include "mgIRingImp.h"
#include <FdoGeometry.h>
#include "mgICurveSegmentAbstractImp.h"
#include "mgObjectFactory.h"

FdoIRing* NAMESPACE_OSGEO_GEOMETRY::IRingImp::GetImpObj()
{
	return static_cast<FdoIRing*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IRingImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

FdoRingCollection* NAMESPACE_OSGEO_GEOMETRY::RingCollection::GetImpObj()
{
	return static_cast<FdoRingCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::RingCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IRingImp::IRingImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IRingImp::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ NAMESPACE_OSGEO_GEOMETRY::IRingImp::default::get(System::Int32 index)
{
	FdoICurveSegmentAbstract* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ NAMESPACE_OSGEO_GEOMETRY::IRingImp::CurveSegments::get()
{
	FdoCurveSegmentCollection* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCurveSegments())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateCurveSegmentCollection(IntPtr(ret), true);
}

//-----------------------------------------------------------------------
// RingCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::RingCollection::RingCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::RingCollection::RingCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoRingCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IRing^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::RingCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IRing^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::IRing^ NAMESPACE_OSGEO_GEOMETRY::RingCollection::Item::get(System::Int32 index)
{
	FdoIRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp^>(value)->GetImpObj())));
}
