/*
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
*
*/

#include "stdafx.h"
#include "Fdo.h"
#include "FdoWmsOvLayerCollection.h"

#include "FDO\Providers\WMS\Override\mgOvLayerCollection.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvLayerDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::OvLayerCollection(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoWmsOvLayerCollection::Create((nullptr == parent ? nullptr : static_cast<FdoPhysicalElementMapping*>(parent->UnmanagedObject.ToPointer())))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::OvLayerCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoWmsOvLayerCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::GetImpObj()
{
	return static_cast<FdoWmsOvLayerCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i];
}

System::Object^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Add(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::IndexOf(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::IndexOf(System::String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Remove(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Contains(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Contains(System::String^ name)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Item::get(System::Int32 index)
{
	FdoWmsOvLayerDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvLayerDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
