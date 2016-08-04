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
#include <Fdo\Xml\SchemaMapping.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlSchemaMapping.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlElementMappingCollection.h"
#include "FDO\Xml\mgXmlClassMappingCollection.h"

FdoXmlSchemaMapping* NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::GetImpObj()
{
    return static_cast<FdoXmlSchemaMapping*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::GetProvider()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProvider())

    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::TargetNamespace::set(System::String^ package)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTargetNamespace(StringToUni(package)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::TargetNamespace::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTargetNamespace())

    return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection^ NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::GetElementMappings()
{
	FdoXmlElementMappingCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetElementMappings())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMappingCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection^ NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping::GetClassMappings()
{
	FdoXmlClassMappingCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassMappings())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlClassMappingCollection(IntPtr(result), true);
}
