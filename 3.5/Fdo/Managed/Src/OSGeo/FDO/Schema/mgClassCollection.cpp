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

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::ClassCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoClassCollection::Create(parent->GetImpObj())), true))
}

FdoClassCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::GetImpObj()
{
	return static_cast<FdoClassCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::CopyTo(array<ClassDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<ClassDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<ClassDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<ClassDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<ClassDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<ClassDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<ClassDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Add(ClassDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexOf(ClassDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::IndexOf(String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Insert(System::Int32 index, ClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Remove(ClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Contains(ClassDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Contains(String^ name)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Item::get(System::String^ index)
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Item::get(System::Int32 index)
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
