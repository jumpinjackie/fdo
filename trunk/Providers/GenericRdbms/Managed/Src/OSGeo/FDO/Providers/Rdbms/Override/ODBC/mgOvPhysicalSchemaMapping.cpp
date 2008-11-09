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
#include "Rdbms\Override\RdbmsOv.h"
#include "Rdbms\Override\Odbc\OdbcOv.h"
#include "Odbc\OdbcOvPhysicalSchemaMapping.h"

#include "FDO\Providers\Rdbms\Override\ODBC\mgOvPhysicalSchemaMapping.h"
#include "FDO\Providers\Rdbms\Override\ODBC\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\ODBC\mgOvClassCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoOdbcOvPhysicalSchemaMapping::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoOdbcOvPhysicalSchemaMapping::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(schemaMapping, autoDelete)
{
    // Validate IN Parameter
    if (NULL == schemaMapping) {
        throw new System::ArgumentNullException();
    }

    // Retrieve the provider name for the FdoOdbcOvPhysicalSchemaMapping class
    FdoPtr<FdoOdbcOvPhysicalSchemaMapping> odbcSchemaMapping = FdoOdbcOvPhysicalSchemaMapping::Create();
    FdoStringP odbcProviderName = odbcSchemaMapping->GetProvider();

    // Retrieve the provider name of the schema mapping object passed into the constructor
    System::String* sProviderName = schemaMapping->Provider;

    // If the provider named do not match throw an Invalid Argument exception
    if (odbcProviderName != StringToUni(sProviderName)) {
        throw new System::ArgumentException();
    }
}

FdoOdbcOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::GetImpObj()
{
	return static_cast<FdoOdbcOvPhysicalSchemaMapping*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::get_Classes()
{
	FdoOdbcOvClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::ObjectFactory::CreateOvClassCollection(result, true);
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::OvPhysicalSchemaMapping::get_Provider()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetProvider())

	return unobj;
}

