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
#include <Fdo\Schema\ClassCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgClassCollection.h"

#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgSchemaElement.h"

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ClassCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement* parent) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoClassCollection::Create(parent->GetImpObj()), true))
}

FdoClassCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::GetImpObj()
{
	return static_cast<FdoClassCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::CopyTo(ClassDefinition* array[], System::Int32 index)
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
		array[index+i] = __try_cast<ClassDefinition*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<ClassDefinition*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<ClassDefinition*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<ClassDefinition*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<ClassDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<ClassDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<ClassDefinition*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Add(ClassDefinition* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexOf(ClassDefinition* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexOf(String* name)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Insert(System::Int32 index, ClassDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Remove(ClassDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Contains(ClassDefinition* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Contains(String* name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for ClassCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoClassDefinition* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::get_RealTypeItem(System::Int32 index)
{
	FdoClassDefinition* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(upElement, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::get_RealTypeItem(System::String* index)
{
	FdoClassDefinition* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::set_RealTypeItem(System::Int32 index, ClassDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::set_Item(System::Int32 index, ClassDefinition* value)
{
	set_RealTypeItem(index, value);
}