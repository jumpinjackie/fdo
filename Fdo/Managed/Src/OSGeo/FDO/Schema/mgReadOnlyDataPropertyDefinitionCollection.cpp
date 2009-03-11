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
#include <Fdo\Schema\ReadOnlyDataPropertyDefinitionCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgReadOnlyDataPropertyDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"
#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"

FdoReadOnlyDataPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::GetImpObj()
{
	return static_cast<FdoReadOnlyDataPropertyDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::ReadOnlyDataPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ parent) : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoReadOnlyDataPropertyDefinitionCollection::Create(parent->GetImpObj())), true))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::CopyTo(array<DataPropertyDefinition^>^ pArray, System::Int32 index)
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

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::IndexOf(DataPropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::Contains(DataPropertyDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::Item::get(System::String^ name)
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}
