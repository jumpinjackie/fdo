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

FdoILineString * NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::GetImpObj()
{
	return static_cast<FdoILineString *>(__super::UnmanagedObject.ToPointer());
}

FdoLineStringCollection * NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::GetImpObj()
{
	return static_cast<FdoLineStringCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::ILineStringImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::ICurveAbstractImp(unmanaged, autoDelete)
{
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::get_Item(System::Int32 index)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::GetItemByMembers(
	System::Int32 index, 
	System::Double &coordinateX, 
	System::Double &coordinateY, 
	System::Double &coordinateZ, 
	System::Double &coordinateM, 
	System::Int32 &dimensionality)
{
	// NEED_TEST
	FdoDouble __pin *ix = &coordinateX;
	FdoDouble __pin *iy = &coordinateY;
	FdoDouble __pin *iz = &coordinateZ;
	FdoDouble __pin *im = &coordinateM;
	System::Int32 __pin *idim = &dimensionality;

	EXCEPTION_HANDLER(GetImpObj()->GetItemByMembers(index, ix, iy, iz, im, idim));
}

NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection *NAMESPACE_OSGEO_GEOMETRY::ILineStringImp::get_Positions()
{
	FdoDirectPositionCollection *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPositions())
	return new DirectPositionCollection(ret, true);
}

//-----------------------------------------------------------------------
// LineStringCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::LineStringCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::LineStringCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoLineStringCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<ILineString *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<ILineString *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<ILineString *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<ILineString *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<ILineString *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<ILineString *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ILineString *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ILineString *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ILineString *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ILineString *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::ILineString *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::ILineString * NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::get_RealTypeItem(System::Int32 index)
{
	FdoILineString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ILineStringImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::ILineString * NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ILineString * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoILineString *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::LineStringCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

