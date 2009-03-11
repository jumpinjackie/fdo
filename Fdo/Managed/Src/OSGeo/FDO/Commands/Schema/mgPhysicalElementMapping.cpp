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
#include <Fdo\Commands\Schema\PhysicalElementMapping.h>
#include <Fdo\Commands\Schema\PhysicalClassMapping.h>
#include <Fdo\Commands\Schema\PhysicalSchemaMapping.h>
#include <Fdo\Commands\Schema\PhysicalPropertyMapping.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::PhysicalElementMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
{

}

FdoPhysicalElementMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::GetImpObj()
{
    return static_cast<FdoPhysicalElementMapping*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IPhysicalElementMapping^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::Parent::get()
{
	FdoPhysicalElementMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParent())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::SchemaMapping::get()
{
	FdoPhysicalSchemaMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaMapping())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::Name::set(System::String^ name)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(name)))
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::QualifiedName::get()
{
	FdoStringP result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetQualifiedName())

	return CHECK_STRING((FdoString*)result);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::CanSetName::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanSetName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes)
{
	EXCEPTION_HANDLER(GetImpObj()->InitFromXml(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->_writeXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::SubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement)
{
	FdoXmlSaxHandler* result;
	
	EXCEPTION_HANDLER(result = GetImpObj()->SubElementError(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(parentElement), StringToUni(subElement)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::MultiSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->MultiSubElementError(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(parentElement), StringToUni(subElement)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::ChoiceSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, NAMESPACE_OSGEO_COMMON::StringCollection^ subElements)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->ChoiceSubElementError(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(parentElement), FdoStringsP(static_cast<FdoStringCollection*>(subElements->UnmanagedObject.ToPointer()))))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping::DuplicateSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement, System::String^ subElementName)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->DuplicateSubElementError(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), StringToUni(parentElement), StringToUni(subElement), StringToUni(subElementName)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIXmlSaxHandler(IntPtr(result), true);
}
