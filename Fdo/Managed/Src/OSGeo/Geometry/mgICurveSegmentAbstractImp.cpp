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
#include <FdoGeometry.h>
#include <Geometry/ienvelope.h>

#include "mgICurveSegmentAbstractImp.h"
#include "mgIEnvelope.h"
#include "mgIDirectPosition.h"
#include "mgObjectFactory.h"

FdoICurveSegmentAbstract* NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::GetImpObj()
{
	return static_cast<FdoICurveSegmentAbstract*>(UnmanagedObject.ToPointer());
}

FdoCurveSegmentCollection* NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::GetImpObj()
{
	return static_cast<FdoCurveSegmentCollection*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::ICurveSegmentAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::Envelope::get()
{
	FdoIEnvelope* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEnvelope())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(IntPtr(ret), true);
}


NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::StartPosition::get()
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetStartPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::EndPosition::get()
{
	FdoIDirectPosition* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEndPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(IntPtr(ret), true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::IsClosed::get()
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetIsClosed())
	return ret;
}

NAMESPACE_OSGEO_COMMON::GeometryComponentType NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::DerivedType::get()
{
	FdoGeometryComponentType ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDerivedType())
	return static_cast<NAMESPACE_OSGEO_COMMON::GeometryComponentType>(ret);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::Dimensionality::get()
{
    System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
    return ret;
}

//-----------------------------------------------------------------------
// CurveSegmentCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CurveSegmentCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
    : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CurveSegmentCollection()
	: NAMESPACE_OSGEO_COMMON::CollectionBase(IntPtr(FdoCurveSegmentCollection::Create()), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CopyTo(array<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj())));
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Count::get()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Item::get(System::Int32 index)
{
	FdoICurveSegmentAbstract* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(IntPtr(ret), true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp^>(value)->GetImpObj()));
}
