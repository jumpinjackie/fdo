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
#include "mgILineStringImp.h"
#include <FdoGeometry.h>
#include "mgIDirectPositionImp.h"
#include "mgObjectFactory.h"

FdoILineString* NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::GetImpObj()
{
	return static_cast<FdoILineString*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

FdoLineStringCollection* NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::GetImpObj()
{
	return static_cast<FdoLineStringCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::ILineStringImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::default::get(System::Int32 index)
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::GetItemByMembers(
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

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::Positions::get()
{
	FdoDirectPositionCollection* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPositions())
	return gcnew DirectPositionCollection(IntPtr(ret), true);
}

//-----------------------------------------------------------------------
// LineStringCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::LineStringCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::LineStringCollection()
    : NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoLineStringCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ILineString^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ILineString^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::ILineString^ NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Item::get(System::Int32 index)
{
	FdoILineString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp^>(value)->GetImpObj())));
}
