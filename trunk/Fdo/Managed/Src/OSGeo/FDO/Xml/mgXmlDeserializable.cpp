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
#include <Fdo\Xml\Deserializable.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlDeserializable.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::XmlDeserializable(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
{
	
}

FdoXmlDeserializable* NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetImpObj()
{
    return static_cast<FdoXmlDeserializable*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(UnmanagedObject.ToPointer()));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(System::String^ fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(System::String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoTextReader*>(textReader->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->ReadXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags^ NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetDeserializationFlags()
{
	FdoXmlFlags* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDeserializationFlags())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetFromExternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromExternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ NAMESPACE_OSGEO_FDO_XML::XmlDeserializable::GetSaxContext()
{
	FdoXmlSaxContext* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSaxContext())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlSaxContext(IntPtr(result), true);
}
