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
#include <Fdo\Schema\ReadOnlyPropertyDefinitionCollection.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\SchemaElementState.h>

#include "FDO\Schema\mgReadOnlyPropertyDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\Schema\mgPropertyDefinitionCollection.h"

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::ReadOnlyPropertyDefinitionCollection(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection^ parent) : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoReadOnlyPropertyDefinitionCollection::Create(parent->GetImpObj())), true))
}

FdoReadOnlyPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::GetImpObj()
{
	return static_cast<FdoReadOnlyPropertyDefinitionCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::CopyTo(array<PropertyDefinition^>^ pArray, System::Int32 index)
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

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::IndexOf(PropertyDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::Contains(PropertyDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::Item::get(System::String^ name)
{
	FdoPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::Item::get(System::Int32 index)
{
	FdoPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinition(IntPtr(result), true);
}

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}
