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
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlClassMapping.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlElementMappingCollection.h"


NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::XmlClassMapping(System::String* name, System::String* schemaName, System::String* wkBaseName) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlClassMapping::Create(StringToUni(name), StringToUni(schemaName), StringToUni(wkBaseName)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::XmlClassMapping(System::String* name, System::String* schemaName, System::String* wkBaseName, System::String* wkSchemaName) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlClassMapping::Create(StringToUni(name), StringToUni(schemaName), StringToUni(wkBaseName), StringToUni(wkSchemaName)), true))
}

FdoXmlClassMapping* NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::GetImpObj()
{
    return static_cast<FdoXmlClassMapping*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::get_GmlName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGmlName())
	
	return result;
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::get_WkBaseName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetWkBaseName())

	return result;
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::get_WkSchemaName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetWkSchemaName())

	return result;
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection* NAMESPACE_OSGEO_FDO_XML::XmlClassMapping::get_ElementMappings()
{
	FdoXmlElementMappingCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetElementMappings())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMappingCollection(result, true);
}
