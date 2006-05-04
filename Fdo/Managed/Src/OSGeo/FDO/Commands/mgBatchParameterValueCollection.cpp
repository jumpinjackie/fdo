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
#include <Fdo\Commands\BatchParameterValueCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgBatchParameterValueCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgParameterValueCollection.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Dispose(System::Boolean disposing)
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

FdoBatchParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::GetImpObj()
{
    return static_cast<FdoBatchParameterValueCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::BatchParameterValueCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoBatchParameterValueCollection::Create(), true))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::CopyTo(ParameterValueCollection* array[], System::Int32 index)
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
		array[index+i] = __try_cast<ParameterValueCollection*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<ParameterValueCollection*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<ParameterValueCollection*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<ParameterValueCollection*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<ParameterValueCollection*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<ParameterValueCollection*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<ParameterValueCollection*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Add(ParameterValueCollection* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IndexOf(ParameterValueCollection* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Insert(System::Int32 index, ParameterValueCollection* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Remove(ParameterValueCollection* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Contains(ParameterValueCollection* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for BatchParameterValueCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoParameterValueCollection* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValueCollection(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::get_RealTypeItem(System::Int32 index)
{
	FdoParameterValueCollection* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValueCollection(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::set_RealTypeItem(System::Int32 index, ParameterValueCollection* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::set_Item(System::Int32 index, ParameterValueCollection* value)
{
	set_RealTypeItem(index, value);
}
