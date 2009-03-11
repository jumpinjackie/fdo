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
#include "mgIGeometryImp.h"
#include <FdoGeometry.h>
#include "mgIEnvelope.h"
#include "mgObjectFactory.h"

FdoIGeometry* NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::GetImpObj()
{
	return static_cast<FdoIGeometry*>(UnmanagedObject.ToPointer());
}

FdoGeometryCollection* NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GetImpObj()
{
	return static_cast<FdoGeometryCollection*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::IGeometryImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::Envelope::get()
{
	FdoIEnvelope* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEnvelope())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(IntPtr(ret), true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::Dimensionality::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

NAMESPACE_OSGEO_COMMON::GeometryType NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::DerivedType::get()
{
	FdoGeometryType ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDerivedType())
	return static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(ret);
}

System::String^ NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::Text::get()
{
    FdoString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetText())
	return CHECK_STRING(ret);
}

//-----------------------------------------------------------------------
// GeometryCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GeometryCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GeometryCollection()
    : NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoGeometryCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometry^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Item::get(System::Int32 index)
{
	FdoIGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(value)->GetImpObj())));
}
