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
#include "RdbmsOvPropertyDefinition.h"
#include "FDO\Commands\Schema\PhysicalElementMapping.h"
#include "mgOvPropertyDefinition.h"

FdoRdbmsOvPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition::GetImpObj()
{
	return static_cast<FdoRdbmsOvPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition::OvPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping(unmanaged, autoDelete)
{
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition::set_Parent(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetParent(static_cast<FdoPhysicalElementMapping*>(value->UnmanagedObject.ToPointer())))
}
