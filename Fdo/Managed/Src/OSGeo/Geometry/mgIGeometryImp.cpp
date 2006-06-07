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

FdoIGeometry * NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::GetImpObj()
{
	return static_cast<FdoIGeometry *>(__super::UnmanagedObject.ToPointer());
}

FdoGeometryCollection * NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GetImpObj()
{
	return static_cast<FdoGeometryCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::IGeometryImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}


System::Void NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::Dispose(System::Boolean disposing)
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

NAMESPACE_OSGEO_GEOMETRY::IEnvelope *NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::get_Envelope()
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEnvelope())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::get_Dimensionality()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

NAMESPACE_OSGEO_COMMON::GeometryType NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::get_DerivedType()
{
	FdoGeometryType ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDerivedType())
	return static_cast<NAMESPACE_OSGEO_COMMON::GeometryType>(ret);
}

System::String *NAMESPACE_OSGEO_GEOMETRY::IGeometryImp::get_Text()
{
	String *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetText())
	return ret;
}

//-----------------------------------------------------------------------
// GeometryCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GeometryCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GeometryCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoGeometryCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<IGeometry *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<IGeometry *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<IGeometry *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<IGeometry *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<IGeometry *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<IGeometry *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IGeometry *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IGeometry *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IGeometry *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IGeometry *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::IGeometry *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIGeometry *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IGeometry * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoIGeometry *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

