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
#include <Fdo\Commands\Schema\PhysicalSchemaMapping.h>
#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping::PhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping::PhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(System::IntPtr::Zero, false)
{
    if (nullptr == schemaMapping)
		return;

	EXCEPTION_HANDLER(Attach(IntPtr(schemaMapping->GetImpObj()), autoDelete))
}

FdoPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping::GetImpObj()
{
    return static_cast<FdoPhysicalSchemaMapping*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping::Provider::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProvider())

	return CHECK_STRING(result);
}

