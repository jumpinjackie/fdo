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
#include <Fdo\Commands\Schema\IApplySchema.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Schema\mgIApplySchemaImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgFeatureSchema.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"

FdoIApplySchema* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::GetImpObj()
{
    return static_cast<FdoIApplySchema*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::FeatureSchema::get()
{
	FdoFeatureSchema* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureSchema())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::FeatureSchema::set(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureSchema((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::PhysicalMapping::get()
{
	FdoPhysicalSchemaMapping* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPhysicalMapping())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::PhysicalMapping::set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPhysicalMapping((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::IgnoreStates::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIgnoreStates())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::IgnoreStates::set(System::Boolean ignoreStates )
{
	EXCEPTION_HANDLER(GetImpObj()->SetIgnoreStates(ignoreStates))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
