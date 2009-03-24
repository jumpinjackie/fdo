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
#include <MySQLOvPropertyDefinitionCollection.h>
#include <MySQLOvTable.h>
#include <MySQLOvGeometricColumn.h>
#include <MySQLOvClassDefinition.h>
#include <MySQLOvStorageEngineType.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvStorageEngineType.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvTable.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvGeometricColumn.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvClassDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::GetImpObj()
{
	return static_cast<FdoMySQLOvClassDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvClassDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoMySQLOvClassDefinition::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::Properties::get()
{
	FdoMySQLOvPropertyDefinitionCollection * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return ObjectFactory::CreateOvPropertyDefinitionCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::Table::get()
{
	FdoMySQLOvTable * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTable())

	return ObjectFactory::CreateOvTable(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::Table::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable^ table)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTable(table->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::AutoIncrementPropertyName::set(System::String^ autoIncrementPropertyName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAutoIncrementPropertyName(StringToUni(autoIncrementPropertyName)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::AutoIncrementPropertyName::get()
{
	FdoString * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAutoIncrementPropertyName())

	return CHECK_STRING(result);
}

System::Int64 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::AutoIncrementSeed::get()
{
	System::Int64 unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetAutoIncrementSeed())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::AutoIncrementSeed::set(System::Int64 autoIncrementSeed)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAutoIncrementSeed(autoIncrementSeed))
}

