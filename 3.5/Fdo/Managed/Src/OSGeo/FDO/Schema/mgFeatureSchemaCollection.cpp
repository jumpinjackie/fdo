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
#include <Fdo\Schema\FeatureSchemaCollection.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFlags.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgFeatureSchema.h"
#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\mgIDisposableCollection.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMappingCollection.h"

FdoFeatureSchemaCollection* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetImpObj()
{
	return static_cast<FdoFeatureSchemaCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::FeatureSchemaCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent) : NAMESPACE_OSGEO_COMMON::CollectionBase(System::IntPtr::Zero, false)
{
	if(parent == nullptr)
	{
		EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureSchemaCollection::Create(nullptr)), true))
	}
	else
	{
		EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureSchemaCollection::Create(parent->GetImpObj())), true))
	}
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>^ pArray, System::Int32 index)
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

System::Object^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexInternal::set(System::Int32 index, System::Object^ value)
{
	this->Item[index] = dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Add(System::Object^ value)
{
	return Add(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Contains(System::Object^ value)
{
	return Contains(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexOf(System::Object^ value)
{
	return IndexOf(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Insert(System::Int32 index, System::Object^ value)
{
	Insert(index, dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Remove(System::Object^ value)
{
	return Remove(dynamic_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Add(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexOf(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexOf(String^ name)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

	return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Remove(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Contains(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == nullptr ? nullptr : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Contains(String^ name)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Item::get(System::String^ index)
{
	FdoFeatureSchema* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Item::get(System::Int32 index)
{
	FdoFeatureSchema* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Item::set(System::Int32 index, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == nullptr ? nullptr : value->GetImpObj())))
}

/*
	Additional interfaces
*/

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetFromInternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromInternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetFromExternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromExternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO::IDisposableCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::FindClass(System::String^ className)
{
	FdoIDisposableCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindClass(StringToUni(className)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlSchemaMappings::get()
{
	FdoPhysicalSchemaMappingCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlSchemaMappings())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMappingCollection(IntPtr(result), true);
}

/*
	IXmlSaxHandler Implemenations
*/
NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result= GetImpObj()->XmlStartDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlEndDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ resourceLocation, System::String^ name, System::String^ qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->XmlStartElement(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ resourceLocation, System::String^ name, System::String^ qualifiedName)
{
	System::Boolean rv;

	EXCEPTION_HANDLER(rv = !!GetImpObj()->XmlEndElement((context == nullptr ? nullptr : static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName)))

	return rv;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ characters)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlCharacters(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(characters)))
}

/*
	IXmlDeserializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(System::String^ fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(System::String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetDeserializationFlags()
{
	FdoXmlFlags* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDeserializationFlags())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetSaxContext()
{
	FdoXmlSaxContext* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSaxContext())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlSaxContext(IntPtr(result), true);
}

/*
	IXmlSerializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(String^ fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}
