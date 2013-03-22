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
#include "Common\mgDictionary.h"
#include "Common\mgObjectFactory.h"
#include "Common\mgDictionaryElement.h"
#include "Common\mgException.h"

NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::DictionaryElementCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDictionary::Create()), true))
}

NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::DictionaryElementCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoDictionary* NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::GetImpObj()
{
	return static_cast<FdoDictionary*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::CopyTo(array<DictionaryElement^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value);
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value));
}

System::Boolean NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value));
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_COMMON::DictionaryElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Count::get(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Add(DictionaryElement^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::IndexOf(DictionaryElement^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::IndexOf(String^ name)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Insert(System::Int32 index, DictionaryElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Remove(DictionaryElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Contains(DictionaryElement^ value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Contains(String^ name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_COMMON::DictionaryElement^ NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Item::get(System::String^ index)
{
	FdoDictionaryElement* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(index)))

	return ObjectFactory::CreateDictionaryElement(IntPtr(upElement), true);
}

NAMESPACE_OSGEO_COMMON::DictionaryElement^ NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Item::get(System::Int32 index)
{
	FdoDictionaryElement* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return ObjectFactory::CreateDictionaryElement(IntPtr(upElement), true);
}

System::Void NAMESPACE_OSGEO_COMMON::DictionaryElementCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_COMMON::DictionaryElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
