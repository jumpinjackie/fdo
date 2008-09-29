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
	return static_cast<FdoMySQLOvClassDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoMySQLOvClassDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::OvClassDefinition(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoMySQLOvClassDefinition::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::get_Properties()
{
	FdoMySQLOvPropertyDefinitionCollection * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetProperties())

	return ObjectFactory::CreateOvPropertyDefinitionCollection(unobj, true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::get_Table()
{
	FdoMySQLOvTable * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTable())

	return ObjectFactory::CreateOvTable(unobj, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::set_Table(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* table)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTable(table->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::set_AutoIncrementPropertyName(System::String* autoIncrementPropertyName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAutoIncrementPropertyName(StringToUni(autoIncrementPropertyName)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::get_AutoIncrementPropertyName()
{
	FdoString * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetAutoIncrementPropertyName())

	return unobj;
}

System::Int64 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::get_AutoIncrementSeed()
{
	FdoInt64 unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetAutoIncrementSeed())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition::set_AutoIncrementSeed(System::Int64 autoIncrementSeed)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAutoIncrementSeed(autoIncrementSeed))
}

