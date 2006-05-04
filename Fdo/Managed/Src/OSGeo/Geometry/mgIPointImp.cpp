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
#include "mgIPointImp.h"
#include <Geometry.h>
#include "mgIDirectPosition.h"
#include "mgObjectFactory.h"

FdoPointCollection * NAMESPACE_OSGEO_GEOMETRY::PointCollection::GetImpObj()
{
	return static_cast<FdoPointCollection *>(__super::UnmanagedObject.ToPointer());
}

FdoIPoint * NAMESPACE_OSGEO_GEOMETRY::IPointImp::GetImpObj()
{
	return static_cast<FdoIPoint *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::IPointImp::IPointImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IGeometryImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::IPointImp::get_Position()
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::IPointImp::GetPositionByMembers(
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
	FdoInt32 __pin *idim = &dimensionality;

	EXCEPTION_HANDLER(GetImpObj()->GetPositionByMembers(ix, iy, iz, im, idim));
}

//-----------------------------------------------------------------------
// PointCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::PointCollection::PointCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::PointCollection::PointCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoPointCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::PointCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<IPoint *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<IPoint *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<IPoint *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<IPoint *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<IPoint *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<IPoint *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::Add(NAMESPACE_OSGEO_GEOMETRY::IPoint *value)
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj())))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPoint *value)
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::IPoint *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::IPoint *value)
{
	FdoBoolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::PointCollection::get_Count()
{
	FdoInt32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::PointCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::IPoint *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::IPoint * NAMESPACE_OSGEO_GEOMETRY::PointCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIPoint *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::IPointImp *>(value)->GetImpObj())));
}

NAMESPACE_OSGEO_GEOMETRY::IPoint * NAMESPACE_OSGEO_GEOMETRY::PointCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::PointCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoIPoint *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::PointCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::PointCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

