/*
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
*
*/

#include "stdafx.h"
#include "Fdo.h"
#include "FdoWmsOvPhysicalSchemaMapping.h"

#include "FDO\Providers\WMS\Override\mgOvPhysicalSchemaMapping.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvClassCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoWmsOvPhysicalSchemaMapping::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(schemaMapping, autoDelete)
{
    // Validate IN Parameter
    if (NULL == schemaMapping) {
        return;
    }

    // Retrieve the provider name for the FdowmsPhysicalSchemaMapping class
    FdoWmsOvPhysicalSchemaMappingP wmsSchemaMapping = FdoWmsOvPhysicalSchemaMapping::Create();
    FdoStringP wmsProviderName = wmsSchemaMapping->GetProvider();

    // Retrieve the provider name of the schema mapping object passed into the constructor
    System::String* sProviderName = schemaMapping->Provider;

    // If the provider names do not match throw an Invalid Argument exception
    if (wmsProviderName != StringToUni(sProviderName)) {
        //Wait for changing again
		throw new System::ArgumentException();
    }
}

FdoWmsOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvPhysicalSchemaMapping::GetImpObj()
{
	return static_cast<FdoWmsOvPhysicalSchemaMapping*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvPhysicalSchemaMapping::get_Classes()
{
	FdoWmsOvClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvClassCollection(result, true);
}