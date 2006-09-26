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
#include <Fdo\Commands\Schema\PhysicalSchemaMappingCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFlags.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMappingCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"
#include "FDO\Connections\mgIConnectionImp.h"

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

FdoPhysicalSchemaMappingCollection* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetImpObj()
{
    return static_cast<FdoPhysicalSchemaMappingCollection*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::PhysicalSchemaMappingCollection() : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoPhysicalSchemaMappingCollection::Create(), true))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
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

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::CopyTo(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* array[], System::Int32 index)
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
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Add(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Remove(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Contains(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = (!!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj())))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for PhysicalSchemaMappingCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoPhysicalSchemaMapping* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::get_RealTypeItem(System::Int32 index)
{
	FdoPhysicalSchemaMapping* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value)
{
	set_RealTypeItem(index, value);
}

/* 
*	Special interface implementation
*/
 
NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetItem(System::Int32 item)
{
	FdoPhysicalSchemaMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(item))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping * NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetItem(System::String* providerName, System::String* schemaName)
{
	FdoPhysicalSchemaMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(providerName), StringToUni(schemaName)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping * NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetItem(NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* connection, System::String* schemaName)
{
	FdoPhysicalSchemaMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem((static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp*>(connection))->GetImpObj(), StringToUni(schemaName)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(result, true);
}

/*
	IXmlSaxHandler Implemenations
*/
NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result= GetImpObj()->XmlStartDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlEndDocument(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->XmlStartElement(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer())))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String*qualifiedName)
{
	FdoBoolean rv;

	EXCEPTION_HANDLER(rv = (!!GetImpObj()->XmlEndElement((context == NULL ? NULL : static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer())), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName))))

	return rv;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* characters)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlCharacters(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(characters)))
}

/*
	IXmlDeserializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(System::String* fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(System::String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetDeserializationFlags()
{
	FdoXmlFlags* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDeserializationFlags())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetSaxContext()
{
	FdoXmlSaxContext* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSaxContext())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlSaxContext(result, true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetFromExternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromExternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}

/*
	IXmlSerializable Implemenations
*/
System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(String* fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection::GetFromInternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromInternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(result, true);
}
