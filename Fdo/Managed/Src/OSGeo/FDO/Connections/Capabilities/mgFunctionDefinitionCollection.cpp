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
#include <Fdo\Connections\Capabilities\FunctionDefinitionCollection.h>
#include <Fdo\Connections\Capabilities\FunctionDefinition.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Connections\Capabilities\mgFunctionDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\Capabilities\mgFunctionDefinition.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::FunctionDefinitionCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoArgumentDefinitionCollection::Create()), true))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::FunctionDefinitionCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoFunctionDefinitionCollection* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::GetImpObj()
{
    return static_cast<FdoFunctionDefinitionCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::CopyTo(array<FunctionDefinition^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Add(FunctionDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::IndexOf(FunctionDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::IndexOf(System::String^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(value)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Insert(System::Int32 index, FunctionDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Remove(FunctionDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Contains(FunctionDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Contains(System::String^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(value)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Item::get(System::Int32 index)
{
	FdoFunctionDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFunctionDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
