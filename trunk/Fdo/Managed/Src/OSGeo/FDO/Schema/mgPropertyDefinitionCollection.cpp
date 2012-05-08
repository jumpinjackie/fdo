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
#include <Fdo\Schema\PropertyDefinitionCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgPropertyDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgSchemaElement.h"

FdoPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::GetImpObj()
{
	return static_cast<FdoPropertyDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::PropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	if(parent == nullptr)
	{
		EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyDefinitionCollection::Create(nullptr)), true))
	}
	else
	{
		EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyDefinitionCollection::Create(parent->GetImpObj())), true))
	}
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::CopyTo(array<PropertyDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<PropertyDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<PropertyDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<PropertyDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<PropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<PropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<PropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Add(PropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::IndexOf(PropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::IndexOf(String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Insert(System::Int32 index, PropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Remove(PropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Contains(PropertyDefinition^ value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Contains(String^ name)
{
	System::Boolean exist;
	
	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Item::get(System::String^ index)
{
	FdoPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
