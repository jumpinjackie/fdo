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
#include <Fdo\Commands\CommandType.h>
#include <Fdo/ClientServices/ProviderCollection.h>

#include "mgProviderCollection.h"
#include "mgProvider.h"
#include "../mgObjectFactory.h"

FdoProviderCollection* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::GetImpObj()
{
	return static_cast<FdoProviderCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::ReleaseUnmanagedObject()
{
    // Since the lifecycle of this unmanged object is in the charge of FdoProviderRegistry,
    // here we don't need to release this unmanaged object.
	Detach();
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::CopyTo(System::String* array[], System::Int32 index)
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
		array[index+i] = __try_cast<System::String*>(get_RealTypeItem(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::ICollection::get_IsSynchronized()
{
	return false;
}


System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::get_Count()
{
	FdoInt32 ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())

	return ret;
}

System::Boolean NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Contains(System::String* name)
{
	FdoBoolean ret;

	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(StringToUni(name)))

	return ret;
}

System::Int32 NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::IndexOf(System::String* name)
{
	FdoInt32 ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf(StringToUni(name)))

	return ret;
}

/*
Implementation for ReadOnlyPropertyDefinitionCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoProvider* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProvider(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider *NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::get_RealTypeItem(System::Int32 index)
{
	FdoProvider* ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProvider(ret, true);
}