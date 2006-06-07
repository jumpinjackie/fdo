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

FdoPolygonCollection * NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::GetImpObj()
{
	return static_cast<FdoPolygonCollection *>(__super::UnmanagedObject.ToPointer());
}

FdoIPolygon * NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::GetImpObj()
{
	return static_cast<FdoIPolygon *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::IPolygonImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::ISurfaceAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing *NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::get_ExteriorRing()
{
	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetExteriorRing())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::get_InteriorRingCount()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRingCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing *NAMESPACE_OSGEO_GEOMETRY::IPolygonImp::GetInteriorRing(System::Int32 index)
{
	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetInteriorRing(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

//-----------------------------------------------------------------------
// PolygonCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::PolygonCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::PolygonCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoPolygonCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Dispose(System::Boolean disposing)
{
	if (disposing)
	{

	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
			Detach();
	}
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<IPolygon *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<IPolygon *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<IPolygon *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<IPolygon *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<IPolygon *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<IPolygon *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IPolygon *value)
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPolygon *value)
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPolygon *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IPolygon *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IPolygon *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::get_Count()
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::IPolygon *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::IPolygon * NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIPolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPolygon * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon * NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPolygon * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoIPolygon *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PolygonCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

