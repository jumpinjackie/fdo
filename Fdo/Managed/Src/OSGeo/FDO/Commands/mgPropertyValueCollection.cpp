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
#include <Fdo\Commands\PropertyValueCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgPropertyValue.h"

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::PropertyValueCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValueCollection::Create()), true))
}

FdoPropertyValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetImpObj()
{
    return static_cast<FdoPropertyValueCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Add(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Remove(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Contains(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Item::get(System::Int32 index)
{
	FdoPropertyValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

/* 
*	Special interface implementation
*/
NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetItem(System::String^ name)
{
	FdoPropertyValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::FindItem(System::String^ name)
{
	FdoPropertyValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindItem(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection::GetItem(System::Int32 index)
{
	FdoPropertyValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(IntPtr(result), true);
}
