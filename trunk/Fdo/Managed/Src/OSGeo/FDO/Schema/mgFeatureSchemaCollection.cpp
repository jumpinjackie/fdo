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

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

FdoFeatureSchemaCollection* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetImpObj()
{
	return static_cast<FdoFeatureSchemaCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::FeatureSchemaCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement* parent) : Disposable(System::IntPtr::Zero, false)
{
	if(parent == NULL)
	{
		EXCEPTION_HANDLER(Attach(FdoFeatureSchemaCollection::Create(NULL), true))
	}
	else
	{
		EXCEPTION_HANDLER(Attach(FdoFeatureSchemaCollection::Create(parent->GetImpObj()), true))
	}
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array->set_Item(index + i, get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::CopyTo(FeatureSchema* array[], System::Int32 index)
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array[index+i] = __try_cast<FeatureSchema*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<FeatureSchema*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<FeatureSchema*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<FeatureSchema*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<FeatureSchema*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<FeatureSchema*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<FeatureSchema*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Add(FeatureSchema* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexOf(FeatureSchema* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::IndexOf(String* name)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf(StringToUni(name)))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Insert(System::Int32 index, FeatureSchema* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Remove(FeatureSchema* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Contains(FeatureSchema* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Contains(String* name)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(StringToUni(name)))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for FeatureSchemaCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoFeatureSchema* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::get_RealTypeItem(System::Int32 index)
{
	FdoFeatureSchema* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(upElement, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::get_RealTypeItem(System::String* index)
{
	FdoFeatureSchema* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(upElement, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::set_RealTypeItem(System::Int32 index, FeatureSchema* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::set_Item(System::Int32 index, FeatureSchema* value)
{
	set_RealTypeItem(index, value);
}

/*
	Additional interfaces
*/

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetFromInternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromInternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetFromExternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromExternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}

NAMESPACE_OSGEO_FDO::IDisposableCollection* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::FindClass(System::String* className)
{
	FdoIDisposableCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindClass(StringToUni(className)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::get_XmlSchemaMappings()
{
	FdoPhysicalSchemaMappingCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlSchemaMappings())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMappingCollection(result, true);
}

/*
	IXmlSaxHandler Implemenations
*/
NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result= GetImpObj()->XmlStartDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlEndDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->XmlStartElement(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName)
{
	FdoBoolean rv;

	EXCEPTION_HANDLER(rv = !!GetImpObj()->XmlEndElement((context == NULL ? NULL : static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName)))

	return rv;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* characters)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlCharacters(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(characters)))
}

/*
	IXmlDeserializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(System::String* fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(System::String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetDeserializationFlags()
{
	FdoXmlFlags* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDeserializationFlags())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::GetSaxContext()
{
	FdoXmlSaxContext* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSaxContext())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlSaxContext(result, true);
}

/*
	IXmlSerializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(String* fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}
