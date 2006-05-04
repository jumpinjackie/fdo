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
#include <Fdo\IDisposableCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\mgIDisposableCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgSchemaElement.h"



System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

FdoIDisposableCollection* NAMESPACE_OSGEO_FDO::IDisposableCollection::GetImpObj()
{
	return static_cast<FdoIDisposableCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO::IDisposableCollection::IDisposableCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoIDisposableCollection::Create(), true))
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array->set_Item(index + i, get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::CopyTo(NAMESPACE_OSGEO_RUNTIME::Disposable* array[], System::Int32 index)
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array[index+i] = __try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO::IDisposableCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO::IDisposableCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::Add(NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add(static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::IndexOf(NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Remove(NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove(static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::Contains(NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for IDisposableCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO::IDisposableCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoIDisposable* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_RUNTIME::Disposable* NAMESPACE_OSGEO_FDO::IDisposableCollection::get_RealTypeItem(System::Int32 index)
{
	FdoIDisposable* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO::IDisposableCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<FdoDisposable*>((value->get_UnmanagedObject()).ToPointer())))
}

NAMESPACE_OSGEO_RUNTIME::Disposable* NAMESPACE_OSGEO_FDO::IDisposableCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO::IDisposableCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_RUNTIME::Disposable* value)
{
	set_RealTypeItem(index, value);
}