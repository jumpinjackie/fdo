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

FdoICurveSegmentAbstract *NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::GetImpObj()
{
	return static_cast<FdoICurveSegmentAbstract *>(__super::UnmanagedObject.ToPointer());
}

FdoCurveSegmentCollection *NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::GetImpObj()
{
	return static_cast<FdoCurveSegmentCollection *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::ICurveSegmentAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::Dispose(System::Boolean disposing)
{
	EXCEPTION_HANDLER(GetImpObj()->Release());
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope *NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_Envelope()
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEnvelope())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}


NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_StartPosition()
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetStartPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_EndPosition()
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetEndPosition())
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_IsClosed()
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->GetIsClosed())
	return ret;
}

NAMESPACE_OSGEO_COMMON::GeometryComponentType NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_DerivedType()
{
	FdoGeometryComponentType ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetDerivedType())
	return static_cast<NAMESPACE_OSGEO_COMMON::GeometryComponentType>(ret);
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp::get_Dimensionality()
{
	return GetImpObj()->GetDimensionality();
}

//-----------------------------------------------------------------------
// CurveSegmentCollection
//-----------------------------------------------------------------------
NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CurveSegmentCollection(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CurveSegmentCollection()
	: NAMESPACE_OSGEO_RUNTIME::Disposable(FdoCurveSegmentCollection::Create(), true)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::ICollection::CopyTo(System::Array *array, System::Int32 index)
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

System::Object* NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::ICollection::get_SyncRoot()
{
    return NULL;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::ICollection::get_IsSynchronized()
{
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::get_IsFixedSize() 
{ 
    return false;
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::get_IsReadOnly() 
{ 
    return false;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::Add(System::Object *value)
{
    return Add(__try_cast<ICurveSegmentAbstract *>(value));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::Contains(System::Object *value)
{
	return Contains(__try_cast<ICurveSegmentAbstract *>(value));
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::IndexOf(System::Object *value)
{
    return IndexOf(__try_cast<ICurveSegmentAbstract *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::Insert(System::Int32 Index, System::Object *value)
{
    Insert(Index, __try_cast<ICurveSegmentAbstract *>(value));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::Remove(System::Object *value)
{
    Remove(__try_cast<ICurveSegmentAbstract *>(value));
}

System::Object* NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::get_Item(System::Int32 index)
{
    return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IList::set_Item(System::Int32 index, System::Object *value)
{
    set_RealTypeItem( index, __try_cast<ICurveSegmentAbstract *>(value));
}


System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Add(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->Add((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(value)->GetImpObj())));
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::IndexOf(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value)
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(value)->GetImpObj())))
	return ret;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(value)->GetImpObj())));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Remove(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(value)->GetImpObj())));
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Contains(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *value)
{
	System::Boolean ret;
	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp*>(value)->GetImpObj()))
	return ret;
}

System::Int32 NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::get_Count()
{
	System::Int32 ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())
	return ret;
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::GetEnumerator(System::Void)
{
	return new Enumerator(this);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index));
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear());
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::CopyTo(NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract *array[], System::Int32 index)
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

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract * NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::get_RealTypeItem(System::Int32 index)
{
	FdoICurveSegmentAbstract *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(ret, true);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract * value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstractImp *>(value)->GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract * NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract * value)
{
	set_RealTypeItem(index, value);
}

System::Object *NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
		throw new InvalidOperationException();

	FdoICurveSegmentAbstract *ret;
	EXCEPTION_HANDLER(ret = m_pCol->GetImpObj()->GetItem(m_nIdx))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveSegmentAbstract(ret, true);
}

System::Boolean NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

