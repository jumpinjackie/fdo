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

FdoIRing * NAMESPACE_OSGEO_GEOMETRY::IRingImp::GetImpObj()
{
	return static_cast<FdoIRing *>(__super::UnmanagedObject.ToPointer());
}

FdoRingCollection *NAMESPACE_OSGEO_GEOMETRY::RingCollection::GetImpObj()
{
	return static_cast<FdoRingCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IRingImp::IRingImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::IRingImp::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *NAMESPACE_OSGEO_GEOMETRY::IRingImp::get_Item(System::Int32 index)
{
	FdoICurveSegmentAbstract *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection *NAMESPACE_OSGEO_GEOMETRY::IRingImp::get_CurveSegments()
{
	FdoCurveSegmentCollection *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCurveSegments())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateCurveSegmentCollection(ret, true);
}

//-----------------------------------------------------------------------
// RingCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::RingCollection::RingCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::RingCollection::RingCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoRingCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::RingCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<IRing *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<IRing *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<IRing *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<IRing *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<IRing *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<IRing *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IRing *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IRing *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IRing *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IRing *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IRing *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::RingCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::RingCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::IRing *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::IRing * NAMESPACE_OSGEO_GEOMETRY::RingCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IRing * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IRingImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::IRing * NAMESPACE_OSGEO_GEOMETRY::RingCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IRing * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::RingCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoIRing *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::RingCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::RingCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

