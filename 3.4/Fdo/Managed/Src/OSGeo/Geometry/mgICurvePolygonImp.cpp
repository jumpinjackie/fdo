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

FdoCurvePolygonCollection * NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::GetImpObj()
{
	return static_cast<FdoCurvePolygonCollection *>(__super::UnmanagedObject.ToPointer());
}

FdoICurvePolygon *NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::GetImpObj()
{
	return static_cast<FdoICurvePolygon *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::ICurvePolygonImp(System::IntPtr unmanaged, System::Boolean autoDelete) 
	: NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IRing *NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::get_ExteriorRing()
{
	FdoIRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetExteriorRing())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(ret, true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::get_InteriorRingCount()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRingCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IRing *NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp::get_InteriorRing(System::Int32 index)
{
	FdoIRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRing(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(ret, true);
}

//-----------------------------------------------------------------------
// CurvePolygonCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CurvePolygonCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CurvePolygonCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoCurvePolygonCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
{
    if (NULL == array)
        throw new System::ArgumentNullException();
    if (index < 0)
        throw new System::ArgumentOutOfRangeException();
    if ( array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
        throw new System::ArgumentException();
	for (System::Int32 i=0;i<this->Count;i++)
        array->set_Item(index+i,get_Item(i));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<ICurvePolygon *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<ICurvePolygon *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<ICurvePolygon *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<ICurvePolygon *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<ICurvePolygon *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<ICurvePolygon *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp*>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp*>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp*>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp*>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp*>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon *array[], System::Int32 index)
{
	if (NULL == array)
        throw new System::ArgumentNullException();
    if (index < 0)
        throw new System::ArgumentOutOfRangeException();
    if ( array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
        throw new System::ArgumentException();
    for (System::Int32 i=0;i<this->Count;i++)
        array[index+i] = get_RealTypeItem(i);
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon * NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::get_RealTypeItem(System::Int32 index)
{
	FdoICurvePolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurvePolygonImp *>(value)->GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon * NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoICurvePolygon *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

