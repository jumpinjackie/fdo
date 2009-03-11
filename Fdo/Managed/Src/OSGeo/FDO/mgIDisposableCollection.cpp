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
#include <Fdo\IDisposableCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\mgIDisposableCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgSchemaElement.h"

FdoIDisposableCollection* NAMESPACE_OSGEO_FDO::IDisposableCollection::GetImpObj()
{
	return static_cast<FdoIDisposableCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO::IDisposableCollection::IDisposableCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoIDisposableCollection::Create()), true))
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::CopyTo(array<NAMESPACE_OSGEO_RUNTIME::Disposable^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO::IDisposableCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(value));
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::Add(NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add(static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO::IDisposableCollection::IndexOf(NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Remove(NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove(static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))
}

System::Boolean NAMESPACE_OSGEO_FDO::IDisposableCollection::Contains(NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_RUNTIME::Disposable^ NAMESPACE_OSGEO_FDO::IDisposableCollection::Item::get(System::Int32 index)
{
	FdoIDisposable* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO::IDisposableCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_RUNTIME::Disposable^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, static_cast<FdoDisposable*>(value->UnmanagedObject.ToPointer())))
}
