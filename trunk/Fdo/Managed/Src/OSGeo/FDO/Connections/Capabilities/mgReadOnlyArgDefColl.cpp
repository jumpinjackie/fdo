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
#include <Fdo\Connections\Capabilities\ReadOnlyArgumentDefinitionCollection.h>
#include <Fdo\Connections\Capabilities\ArgumentDefinition.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\Capabilities\mgReadOnlyArgDefColl.h"
#include "FDO\Connections\Capabilities\mgArgumentDefinition.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ReadOnlyArgumentDefinitionCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoArgumentDefinitionCollection::Create(), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ReadOnlyArgumentDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoReadOnlyArgumentDefinitionCollection* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::GetImpObj()
{
    return static_cast<FdoReadOnlyArgumentDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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
		array->set_Item(index + i, get_RealTypeItem(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::CopyTo(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition* array[], System::Int32 index)
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
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition*>(get_RealTypeItem(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::ICollection::get_IsSynchronized()
{
	return false;
}


System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::IndexOf(ArgumentDefinition* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::Contains(ArgumentDefinition* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = (!!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj())))

	return exist;
}

/*
Implementation for ArgumentDefinitionCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoArgumentDefinition* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateArgumentDefinition(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::get_RealTypeItem(System::Int32 index)
{
	FdoArgumentDefinition* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateArgumentDefinition(upElement, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection::get_RealTypeItem(System::String* name)
{
	FdoArgumentDefinition* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateArgumentDefinition(upElement, true);
}