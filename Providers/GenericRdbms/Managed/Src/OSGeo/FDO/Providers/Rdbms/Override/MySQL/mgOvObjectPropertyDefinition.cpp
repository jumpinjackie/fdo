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
#include <MySQLOvObjectPropertyDefinition.h>
#include <MySQLOvPropertyMappingDefinition.h>

#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyMappingRelation.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyMappingSingle.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvPropertyMappingDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgOvObjectPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\MySQL\mgObjectFactory.h"

FdoMySQLOvObjectPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoMySQLOvObjectPropertyDefinition*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::OvObjectPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::OvObjectPropertyDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvObjectPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::OvObjectPropertyDefinition(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoMySQLOvObjectPropertyDefinition::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::MappingDefinition::get()
{
	FdoMySQLOvPropertyMappingDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMappingDefinition())

	return ObjectFactory::CreateIOvPropertyMappingDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvObjectPropertyDefinition::MappingDefinition::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition^ propertyMappingDefinition)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMappingDefinition(dynamic_cast<FdoMySQLOvPropertyMappingDefinition*>((FdoIDisposable*)(static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<System::IDisposable^>(propertyMappingDefinition)))->UnmanagedObject.ToPointer())))
}
