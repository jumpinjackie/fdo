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
#include <Fdo\Commands\Schema\PhysicalElementMapping.h>
#include <RdbmsOv.h>
#include <RdbmsOvTable.h>

#include "mgOvTable.h"

FdoRdbmsOvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable::GetImpObj()
{
	return static_cast<FdoRdbmsOvTable*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable::OvTable(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

System::String * NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable::get_PKeyName()
{
	FdoString * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetPKeyName())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable::set_PKeyName(System::String *keyName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPKeyName(StringToUni(keyName)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable::set_Parent(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetParent(static_cast<FdoPhysicalElementMapping*>(value->UnmanagedObject.ToPointer())))
}

