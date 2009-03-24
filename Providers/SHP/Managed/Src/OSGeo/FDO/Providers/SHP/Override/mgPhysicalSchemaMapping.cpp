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
#include "Fdo.h"
#include "SHP\FdoShpProvider.h"
#include "FdoShpPhysicalSchemaMapping.h"

#include "FDO\Providers\SHP\Override\mgPhysicalSchemaMapping.h"
#include "FDO\Providers\SHP\Override\mgObjectFactory.h"
#include "FDO\Providers\SHP\Override\mgClassDefinition.h"
#include "FDO\Providers\SHP\Override\mgClassCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::PhysicalSchemaMapping() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoShpOvPhysicalSchemaMapping::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::PhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(schemaMapping, autoDelete)
{
    // Validate IN Parameter
    if (nullptr == schemaMapping)
        return;

    // Retrieve the provider name for the FdoShpOvPhysicalSchemaMapping class
    FdoShpOvPhysicalSchemaMappingP shpSchemamapping = FdoShpOvPhysicalSchemaMapping::Create();
    FdoStringP shpProviderName = shpSchemamapping->GetProvider();

    // Retrieve the provider name of the schema mapping object passed into the constructor
    System::String^ sProviderName = schemaMapping->Provider;

    // If the provider named do not match throw an Invalid Argument exception
    if (shpProviderName != StringToUni(sProviderName)) {
        //Wait for changing again
		throw gcnew System::ArgumentException();
    }
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::PhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(unmanaged, autoDelete)
{
}

FdoShpOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::GetImpObj()
{
	return static_cast<FdoShpOvPhysicalSchemaMapping*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::Classes::get()
{
	FdoShpOvClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateClassCollection(IntPtr(result), true);
}


NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::FindByShapefile(System::String^ value)
{
	FdoShpOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindByShapefile(StringToUni(value)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PhysicalSchemaMapping::FindByClassName(System::String^ value)
{
	FdoShpOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FindByClassName(StringToUni(value)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}
