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
#include <Fdo\Commands\ParameterValueCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgParameterValueCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgParameterValue.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Dispose(System::Boolean disposing)
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


NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::ParameterValueCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoParameterValueCollection::Create(), true))
}

FdoParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::GetImpObj()
{
    return static_cast<FdoParameterValueCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::CopyTo(NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* array[], System::Int32 index)
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
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Add(NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Remove(NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Contains(NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for ParameterValueCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoParameterValue* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::get_RealTypeItem(System::Int32 index)
{
	FdoParameterValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* value)
{
	set_RealTypeItem(index, value);
}

/* 
*	Special interface implementation
*/
NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::GetItem(System::String* name)
{
	FdoParameterValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(name)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(upElement, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::FindItem(System::String* name)
{
	FdoParameterValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->FindItem(StringToUni(name)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(upElement, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection::GetItem(System::Int32 index)
{
	FdoParameterValue* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(upElement, true);
}
