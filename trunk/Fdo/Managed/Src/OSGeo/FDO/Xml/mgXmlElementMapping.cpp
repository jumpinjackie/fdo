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
#include <Fdo\Xml\ClassMapping.h>
#include <Fdo\Xml\ElementMapping.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlElementMapping.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlClassMapping.h"

NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::XmlElementMapping(System::String* name) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlElementMapping::Create(StringToUni(name)), true))
}

FdoXmlElementMapping* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::GetImpObj()
{
    return static_cast<FdoXmlElementMapping*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::set_ClassMapping(NAMESPACE_OSGEO_FDO_XML::XmlClassMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetClassMapping((value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_XML::XmlClassMapping* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::get_ClassMapping()
{
	FdoXmlClassMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassMapping())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlClassMapping(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::set_ClassName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetClassName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::get_ClassName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::set_SchemaName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSchemaName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::get_SchemaName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::set_GmlUri(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGmlUri(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::get_GmlUri()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGmlUri())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::set_GmlLocalName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGmlUri(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlElementMapping::get_GmlLocalName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGmlLocalName())

	return result;
}
