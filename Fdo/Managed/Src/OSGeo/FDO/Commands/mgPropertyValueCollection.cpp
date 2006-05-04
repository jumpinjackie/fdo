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
#include <Fdo\Commands\PropertyValueCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValue.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Dispose(System::Boolean disposing)
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

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::PropertyValueCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoPropertyValueCollection::Create(), true))
}

FdoPropertyValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetImpObj()
{
    return static_cast<FdoPropertyValueCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::CopyTo(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* array[], System::Int32 index)
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
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Add(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Remove(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Contains(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for PropertyValueCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoPropertyValue* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::get_RealTypeItem(System::Int32 index)
{
	FdoPropertyValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* value)
{
	set_RealTypeItem(index, value);
}

/* 
*	Special interface implementation
*/
NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetItem(System::String* name)
{
	FdoPropertyValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(upElement, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::FindItem(System::String* name)
{
	FdoPropertyValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->FindItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(upElement, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetItem(System::Int32 index)
{
	FdoPropertyValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(upElement, true);
}
