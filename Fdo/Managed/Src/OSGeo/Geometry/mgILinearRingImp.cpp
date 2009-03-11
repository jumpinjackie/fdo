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
#include "mgILinearRingImp.h"
#include <FdoGeometry.h>
#include "mgIDirectPositionImp.h"
#include "mgObjectFactory.h"

FdoILinearRing* NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::GetImpObj()
{
	return static_cast<FdoILinearRing*>(UnmanagedObject.ToPointer());
}

FdoLinearRingCollection* NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::GetImpObj()
{
	return static_cast<FdoLinearRingCollection*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::ILinearRingImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::default::get(System::Int32 index)
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::GetItemByMembers(
	System::Int32 index,
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

	EXCEPTION_HANDLER(GetImpObj()->GetItemByMembers(index, ix, iy, iz, im, idim));
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::Positions::get()
{
	FdoDirectPositionCollection* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPositions())
	return gcnew DirectPositionCollection(IntPtr(ret), true);
}

//-----------------------------------------------------------------------
// LinearRingCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::LinearRingCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
    : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::LinearRingCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoLinearRingCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRing^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Item::get(System::Int32 index)
{
	FdoILinearRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(value)->GetImpObj())));
}
