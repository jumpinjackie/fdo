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

FdoILinearRing * NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::GetImpObj()
{
	return static_cast<FdoILinearRing *>(__super::UnmanagedObject.ToPointer());
}

FdoLinearRingCollection * NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::GetImpObj()
{
	return static_cast<FdoLinearRingCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::ILinearRingImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::get_Item(System::Int32 index)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::GetItemByMembers(
	System::Int32 index,
	System::Double &coordinateX,
	System::Double &coordinateY,
	System::Double &coordinateZ,
	System::Double &coordinateM,
	System::Int32  &dimensionality)
{
	// NEED_TEST
	FdoDouble __pin *ix = &coordinateX;
	FdoDouble __pin *iy = &coordinateY;
	FdoDouble __pin *iz = &coordinateZ;
	FdoDouble __pin *im = &coordinateM;
	System::Int32 __pin *idim = &dimensionality;

	EXCEPTION_HANDLER(GetImpObj()->GetItemByMembers(index, ix, iy, iz, im, idim));
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection *NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp::get_Positions()
{
	FdoDirectPositionCollection *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPositions())
	return new DirectPositionCollection(ret, true);
}

//-----------------------------------------------------------------------
// LinearRingCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::LinearRingCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::LinearRingCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoLinearRingCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<ILinearRing *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<ILinearRing *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<ILinearRing *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<ILinearRing *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<ILinearRing *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<ILinearRing *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ILinearRing *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ILinearRing *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILinearRing *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ILinearRing *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ILinearRing *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::ILinearRing *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::ILinearRing * NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::get_RealTypeItem(System::Int32 index)
{
	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILinearRing * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing * NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILinearRing * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

