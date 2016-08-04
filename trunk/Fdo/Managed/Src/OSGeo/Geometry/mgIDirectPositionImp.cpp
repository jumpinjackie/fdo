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
#include "mgIDirectPositionImp.h"
#include <FdoGeometry.h>
#include "mgObjectFactory.h"

FdoIDirectPosition* NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::GetImpObj()
{
	return static_cast<FdoIDirectPosition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

FdoDirectPositionCollection* NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::GetImpObj()
{
	return static_cast<FdoDirectPositionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::IDirectPositionImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: Disposable(unmanaged, autoDelete)
{
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::X::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetX())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::Y::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetY())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::Z::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetZ())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::M::get()
{
	System::Double ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetM())
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::Dimensionality::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

//-----------------------------------------------------------------------
// DirectPositionCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::DirectPositionCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::DirectPositionCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoDirectPositionCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
    System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Item::get(System::Int32 index)
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp^>(value)->GetImpObj()));
}
