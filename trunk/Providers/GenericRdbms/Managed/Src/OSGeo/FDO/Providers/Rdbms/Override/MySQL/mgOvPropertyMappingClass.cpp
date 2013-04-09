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
#include <MySQLOvPropertyMappingClass.h>
#include <MySQLOvClassDefinition.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingRelation.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvClassDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyMappingClass.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvPropertyMappingClass* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::GetImpObj()
{
	return dynamic_cast<FdoMySQLOvPropertyMappingClass*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::OvPropertyMappingClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::OvPropertyMappingClass() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvPropertyMappingClass::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::InternalClass::get()
{
	FdoMySQLOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInternalClass())

	return ObjectFactory::CreateOvClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyMappingClass::InternalClass::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvClassDefinition^ classDefinition)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInternalClass(classDefinition->GetImpObj()))
}
