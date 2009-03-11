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
#include <Fdo\Schema\DataPropertyDefinitionCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"
#include "FDO\Schema\mgSchemaElement.h"

FdoDataPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::GetImpObj()
{
	return static_cast<FdoDataPropertyDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::DataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDataPropertyDefinitionCollection::Create(parent->GetImpObj())), true))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::CopyTo(array<DataPropertyDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<DataPropertyDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<DataPropertyDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<DataPropertyDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<DataPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<DataPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<DataPropertyDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Add(DataPropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::IndexOf(DataPropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::IndexOf(String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Insert(System::Int32 index, DataPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Remove(DataPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Contains(DataPropertyDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Contains(String^ name)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Item::get(System::String^ index)
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
