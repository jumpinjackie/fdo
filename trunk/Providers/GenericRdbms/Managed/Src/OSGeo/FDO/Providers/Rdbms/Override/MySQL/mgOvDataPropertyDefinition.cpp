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
#include <MySQLOvDataPropertyDefinition.h>
#include <MySQLOvColumn.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvColumn.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvDataPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoMySQLOvDataPropertyDefinition*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::OvDataPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::OvDataPropertyDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvDataPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::OvDataPropertyDefinition(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvDataPropertyDefinition::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::Column::get()
{
	FdoMySQLOvColumn* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return ObjectFactory::CreateOvColumn(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvDataPropertyDefinition::Column::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn ^column)
{
	EXCEPTION_HANDLER(GetImpObj()->SetColumn(column->GetImpObj()))
}
