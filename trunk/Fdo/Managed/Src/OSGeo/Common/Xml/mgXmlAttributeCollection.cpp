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
#include "Common\mgException.h"
#include "Common\mgObjectFactory.h"
#include "Common\Xml\mgXmlAttribute.h"
#include "Common\Xml\mgXmlAttributeCollection.h"

NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::XmlAttributeCollection() : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlAttributeCollection::Create()), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::XmlAttributeCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
{

}

FdoXmlAttributeCollection* NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::GetImpObj()
{
	return static_cast<FdoXmlAttributeCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::CopyTo(array<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value);
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value));
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value));
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^>(value));
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Count::get(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Add(NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::IndexOf(NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::IndexOf(String^ name)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Remove(NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Contains(NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Contains(String^ name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Item::get(System::Int32 index)
{
	FdoXmlAttribute* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return ObjectFactory::CreateXmlAttribute(IntPtr(upElement), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}
