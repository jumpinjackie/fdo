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
#include <RdbmsOvSchemaAutoGeneration.h>
#include <Common\StringCollection.h>

#include "mgOvSchemaAutoGeneration.h"

FdoRdbmsOvSchemaAutoGeneration* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::GetImpObj()
{
	return static_cast<FdoRdbmsOvSchemaAutoGeneration*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::OvSchemaAutoGeneration(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::OvSchemaAutoGeneration() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoRdbmsOvSchemaAutoGeneration::Create()), true))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::GenTablePrefix::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGenTablePrefix())

	return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::GenTablePrefix::set(System::String^ tablePrefix)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGenTablePrefix(StringToUni(tablePrefix)))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::RemoveTablePrefix::get()
{
	System::Boolean unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetRemoveTablePrefix())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::RemoveTablePrefix::set(System::Boolean removeTablePrefix)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRemoveTablePrefix(removeTablePrefix))
}

NAMESPACE_OSGEO_COMMON::StringCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::GenTableList::get()
{
	FdoStringCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGenTableList())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(IntPtr(result), true);
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::MaxSampleRows::get()
{
	System::Int32 unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetMaxSampleRows())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::MaxSampleRows::set(System::Int32 maxRows)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxSampleRows(maxRows))
}
