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
#include <MySQLOv.h>
#include <MySQLOvPhysicalSchemaMapping.h>
#include <MySQLOvClassCollection.h>
#include <MySQLOvStorageEngineType.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvStorageEngineType.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvClassCollection.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPhysicalSchemaMapping.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::GetImpObj()
{	
	return static_cast<FdoMySQLOvPhysicalSchemaMapping*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(schemaMapping, autoDelete)
{
    // Validate IN Parameter
    if (nullptr == schemaMapping)
        return;

    // Retrieve the provider name for the FdoMySQLOvPhysicalSchemaMapping class
    FdoPtr<FdoMySQLOvPhysicalSchemaMapping> mySqlSchemaMapping = FdoMySQLOvPhysicalSchemaMapping::Create();
    FdoStringP shpProviderName = mySqlSchemaMapping->GetProvider();

    // Retrieve the provider name of the schema mapping object passed into the constructor
    System::String^ sProviderName = schemaMapping->Provider;

    // If the provider named do not match throw an Invalid Argument exception
    if (shpProviderName != StringToUni(sProviderName)) {
        //Wait for changing again
		throw gcnew System::ArgumentException();
    }
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvPhysicalSchemaMapping::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvPhysicalSchemaMapping::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::Classes::get()
{
	FdoMySQLOvClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return ObjectFactory::CreateOvClassCollection(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::DataDirectory::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataDirectory())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::DataDirectory::set(System::String^ dataDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataDirectory(StringToUni(dataDirectory)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::IndexDirectory::set(System::String^ indexDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIndexDirectory(StringToUni(indexDirectory)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::IndexDirectory::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIndexDirectory())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::StorageEngine::get()
{
	MySQLOvStorageEngineType unobj;
	
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetStorageEngine())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::StorageEngine::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType storageEngine)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStorageEngine(static_cast<MySQLOvStorageEngineType>(storageEngine)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::Database::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDatabase())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::Database::set(System::String^ database)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDatabase(StringToUni(database)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPhysicalSchemaMapping::Provider::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProvider())

	return CHECK_STRING(result);
}

