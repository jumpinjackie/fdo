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
#include "mgStringCollection.h"

#include "Common\mgObjectFactory.h"
#include "Common\mgStringElement.h"
#include "Common\mgException.h"

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoStringCollection::Create(), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(StringCollection* collection) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoStringCollection::Create(collection->GetImpObj()), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::String* data, System::String* delimiters) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoStringCollection::Create(FdoStringP(StringToUni(data)), StringToUni(delimiters)), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::String* data, System::String* delimiters, System::Boolean nullTokens) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoStringCollection::Create(FdoStringP(StringToUni(data)), StringToUni(delimiters), nullTokens), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoStringCollection* NAMESPACE_OSGEO_COMMON::StringCollection::GetImpObj()
{
	return static_cast<FdoStringCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Append(StringCollection* col)
{
	EXCEPTION_HANDLER(GetImpObj()->Append(*col->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_COMMON::StringCollection::get_String(System::Int32 index)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetString(index))

	return result;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::CopyTo(StringElement* array[], System::Int32 index)
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
		array[index+i] = __try_cast<StringElement*>(get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Object* NAMESPACE_OSGEO_COMMON::StringCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_COMMON::StringCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<StringElement*>(value) );
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<StringElement*>(value));
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<StringElement*>(value));
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<StringElement*>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<StringElement*>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<StringElement*>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_COMMON::StringCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::Add(StringElement* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add(((value == NULL ? NULL : value->GetImpObj()))->GetString()))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::IndexOf(StringElement* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(((value == NULL ? NULL : value->GetImpObj()))->GetString()))

	return index;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Insert(System::Int32 index, StringElement* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Remove(StringElement* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::Contains(StringElement* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for StringCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_COMMON::StringCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoStringElement* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return ObjectFactory::CreateStringElement(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_COMMON::StringElement* NAMESPACE_OSGEO_COMMON::StringCollection::get_RealTypeItem(System::Int32 index)
{
	FdoStringElement* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return ObjectFactory::CreateStringElement(upElement, true);
}

System::Void  NAMESPACE_OSGEO_COMMON::StringCollection::set_RealTypeItem(System::Int32 index, StringElement* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_COMMON::StringElement* NAMESPACE_OSGEO_COMMON::StringCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_COMMON::StringCollection::set_Item(System::Int32 index, StringElement* value)
{
	set_RealTypeItem(index, value);
}

