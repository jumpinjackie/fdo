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
#include <MySQLOvTable.h>
#include <MySQLOvStorageEngineType.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvStorageEngineType.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvTable.h"

FdoMySQLOvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::GetImpObj()
{
	return static_cast<FdoMySQLOvTable*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::OvTable(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::OvTable() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvTable::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::OvTable(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvTable::Create(StringToUni(name))), true))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::DataDirectory::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataDirectory())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::DataDirectory::set(System::String^ dataDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataDirectory(StringToUni(dataDirectory)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::IndexDirectory::set(System::String^ indexDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIndexDirectory(StringToUni(indexDirectory)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::IndexDirectory::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIndexDirectory())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::StorageEngine::get()
{
	MySQLOvStorageEngineType unobj;
	
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetStorageEngine())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::StorageEngine::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType storageEngine)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStorageEngine(static_cast<MySQLOvStorageEngineType>(storageEngine)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::Database::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDatabase())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::Database::set(System::String^ database)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDatabase(StringToUni(database)))
}


