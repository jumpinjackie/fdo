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
#include <Fdo\Schema\FeatureClassCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgFeatureClassCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgFeatureClass.h"
#include "FDO\Schema\mgSchemaElement.h"

FdoFeatureClassCollection* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::GetImpObj()
{
	return static_cast<FdoFeatureClassCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::FeatureClassCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureClassCollection::Create(parent->GetImpObj())), true))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::CopyTo(array<FeatureClass^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<FeatureClass^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<FeatureClass^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<FeatureClass^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<FeatureClass^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<FeatureClass^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<FeatureClass^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Add(FeatureClass^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::IndexOf(FeatureClass^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::IndexOf(String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Insert(System::Int32 index, FeatureClass^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Remove(FeatureClass^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Contains(FeatureClass^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Contains(String^ name)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Item::get(System::Int32 index)
{
	FdoFeatureClass* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureClass(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClassCollection::Item::set(System::Int32 index, FeatureClass^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
