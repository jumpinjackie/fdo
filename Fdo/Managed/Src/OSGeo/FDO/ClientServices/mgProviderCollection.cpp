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

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::CopyTo(array<System::String^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i]->Name;
}

System::Int32 NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Count::get()
{
	System::Int32 ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->GetCount())

	return ret;
}

System::Boolean NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Contains(System::String^ name)
{
	System::Boolean ret;

	EXCEPTION_HANDLER(ret = !!GetImpObj()->Contains(StringToUni(name)))

	return ret;
}

System::Int32 NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::IndexOf(System::String^ name)
{
	System::Int32 ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->IndexOf(StringToUni(name)))

	return ret;
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::Item::get(System::Int32 index)
{
	FdoProvider* ret;

	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProvider(IntPtr(ret), true);
}

System::Object^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}
