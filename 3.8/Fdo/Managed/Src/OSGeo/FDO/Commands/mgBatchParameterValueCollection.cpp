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
#include <Fdo\Commands\BatchParameterValueCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgBatchParameterValueCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgParameterValueCollection.h"

FdoBatchParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::GetImpObj()
{
    return static_cast<FdoBatchParameterValueCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::BatchParameterValueCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBatchParameterValueCollection::Create()), true))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::CopyTo(array<ParameterValueCollection^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<ParameterValueCollection^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<ParameterValueCollection^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<ParameterValueCollection^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<ParameterValueCollection^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<ParameterValueCollection^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<ParameterValueCollection^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Add(ParameterValueCollection^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::IndexOf(ParameterValueCollection^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Insert(System::Int32 index, ParameterValueCollection^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Remove(ParameterValueCollection^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Contains(ParameterValueCollection^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection^ NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Item::get(System::Int32 index)
{
	FdoParameterValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValueCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}