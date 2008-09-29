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
	EXCEPTION_HANDLER(Attach(FdoMySQLOvTable::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::OvTable(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoMySQLOvTable::Create(StringToUni(name)), true))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::get_DataDirectory()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataDirectory())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::set_DataDirectory(System::String* dataDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataDirectory(StringToUni(dataDirectory)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::set_IndexDirectory(System::String* indexDirectory)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIndexDirectory(StringToUni(indexDirectory)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::get_IndexDirectory()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetIndexDirectory())

	return unobj;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::get_StorageEngine()
{
	MySQLOvStorageEngineType unobj;
	
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetStorageEngine())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::set_StorageEngine(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvStorageEngineType storageEngine)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStorageEngine(static_cast<MySQLOvStorageEngineType>(storageEngine)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::get_Database()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDatabase())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable::set_Database(System::String* database)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDatabase(StringToUni(database)))
}


