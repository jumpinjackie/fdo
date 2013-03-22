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
#include <RdbmsOv.h>
#include <Common\Xml\SaxContext.h>
#include <Common\Xml\AttributeCollection.h>
#include <RdbmsOvObjectPropertyDefinition.h>
#include <RdbmsOvPropertyMappingDefinition.h>
#include <RdbmsOvPropertyMappingSingle.h>
#include <RdbmsOvPropertyMappingClass.h>
#include <RdbmsOvPropertyMappingConcrete.h>

#include "mgOvPropertyDefinition.h"
#include "mgOvPropertyMappingDefinition.h"
#include "mgOvPropertyMappingSingle.h"
#include "mgOvPropertyMappingRelation.h"
#include "mgOvPropertyMappingClass.h"
#include "mgOvPropertyMappingConcrete.h"
#include "mgOvObjectPropertyDefinition.h"
#include "mgObjectFactory.h"

FdoRdbmsOvObjectPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoRdbmsOvObjectPropertyDefinition*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::OvObjectPropertyDefinition(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition(unmanaged, autoDelete)
{
	
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::MappingDefinition::get()
{
	FdoRdbmsOvPropertyMappingDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMappingDefinition())

	return ObjectFactory::CreateOvPropertyMappingDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes, System::String^ mappingType, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ mappingAttributes)
{
	EXCEPTION_HANDLER(GetImpObj()->InitFromXml(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer()), StringToUni(mappingType), static_cast<FdoXmlAttributeCollection*>(mappingAttributes->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingSingle^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::CreateSingleMapping(System::Boolean attach)
{
	FdoRdbmsOvPropertyMappingSingle* result;

	EXCEPTION_HANDLER(result = GetImpObj()->CreateSingleMapping(attach))

	return ObjectFactory::CreateOvPropertyMappingSingle(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::CreateClassMapping(System::Boolean attach)
{
	FdoRdbmsOvPropertyMappingClass* result;

	EXCEPTION_HANDLER(result = GetImpObj()->CreateClassMapping(attach))

	return ObjectFactory::CreateOvPropertyMappingClass(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition::CreateConcreteMapping(System::Boolean attach)
{
	FdoRdbmsOvPropertyMappingConcrete* result;

	EXCEPTION_HANDLER(result = GetImpObj()->CreateConcreteMapping(attach))

	return ObjectFactory::CreateOvPropertyMappingConcrete(IntPtr(result), true);
}

