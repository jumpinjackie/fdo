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
#include "mgStringCollection.h"

#include "Common\mgObjectFactory.h"
#include "Common\mgStringElement.h"
#include "Common\mgException.h"

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoStringCollection::Create()), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(StringCollection^ collection) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoStringCollection::Create(collection->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::String^ data, System::String^ delimiters) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoStringCollection::Create(FdoStringP(StringToUni(data)), StringToUni(delimiters))), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::String^ data, System::String^ delimiters, System::Boolean nullTokens) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoStringCollection::Create(FdoStringP(StringToUni(data)), StringToUni(delimiters), nullTokens)), true))
}

NAMESPACE_OSGEO_COMMON::StringCollection::StringCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoStringCollection* NAMESPACE_OSGEO_COMMON::StringCollection::GetImpObj()
{
	return static_cast<FdoStringCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON::StringCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Append(StringCollection^ col)
{
	EXCEPTION_HANDLER(GetImpObj()->Append(*col->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_COMMON::StringCollection::String::get(System::Int32 index)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetString(index))

    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::CopyTo(array<StringElement^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_COMMON::StringCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value);
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value));
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value));
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_COMMON::StringElement^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::Count::get()
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::Add(StringElement^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add(((value == nullptr ? nullptr : value->GetImpObj()))->GetString()))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON::StringCollection::IndexOf(StringElement^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(((value == nullptr ? nullptr : value->GetImpObj()))->GetString()))

	return index;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Insert(System::Int32 index, StringElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Remove(StringElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_COMMON::StringCollection::Contains(StringElement^ value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_COMMON::StringElement^ NAMESPACE_OSGEO_COMMON::StringCollection::Item::get(System::Int32 index)
{
	FdoStringElement* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return ObjectFactory::CreateStringElement(IntPtr(upElement), true);
}

System::Void NAMESPACE_OSGEO_COMMON::StringCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_COMMON::StringElement^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
