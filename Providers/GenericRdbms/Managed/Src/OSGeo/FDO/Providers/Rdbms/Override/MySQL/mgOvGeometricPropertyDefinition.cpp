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
#include <MySQLOvGeometricPropertyDefinition.h>
#include <MySQLOvGeometricColumn.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgOvGeometricColumn.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvGeometricPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvGeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoMySQLOvGeometricPropertyDefinition*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvGeometricPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvGeometricPropertyDefinition::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricColumn^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::Column::get()
{
	FdoMySQLOvGeometricColumn* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return ObjectFactory::CreateOvGeometricColumn(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricPropertyDefinition::Column::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvGeometricColumn ^column)
{
	EXCEPTION_HANDLER(GetImpObj()->SetColumn(column->GetImpObj()))
}
