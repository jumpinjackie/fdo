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
#include <Geometry.h>
#include "mgObjectFactory.h"

FdoIDirectPosition *NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::GetImpObj()
{
	return static_cast<FdoIDirectPosition *>(__super::UnmanagedObject.ToPointer());
}

FdoDirectPositionCollection * NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::GetImpObj()
{
	return static_cast<FdoDirectPositionCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::IDirectPositionImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: Disposable(unmanaged, autoDelete)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::Dispose(System::Boolean disposing)
{
	EXCEPTION_HANDLER(GetImpObj()->Release());
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::get_X()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetX())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::get_Y()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetY())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::get_Z()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetZ())
	return ret;
}

System::Double NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::get_M()
{
	FdoDouble ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetM())
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp::get_Dimensionality()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDimensionality())
	return ret;
}

//-----------------------------------------------------------------------
// DirectPositionCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::DirectPositionCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::DirectPositionCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoDirectPositionCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<IDirectPosition *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<IDirectPosition *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<IDirectPosition *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<IDirectPosition *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<IDirectPosition *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<IDirectPosition *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<NAMESPACE_OSGEO_GEOMETRY::IDirectPositionImp *>(value)->GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

