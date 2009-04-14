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
	return static_cast<FdoRdbmsOvSchemaAutoGeneration*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::OvSchemaAutoGeneration(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::OvSchemaAutoGeneration() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoRdbmsOvSchemaAutoGeneration::Create(), true))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::get_GenTablePrefix()
{
	FdoString * unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGenTablePrefix())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::set_GenTablePrefix(System::String* tablePrefix)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGenTablePrefix(StringToUni(tablePrefix)))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::get_RemoveTablePrefix()
{
	FdoBoolean unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetRemoveTablePrefix())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::set_RemoveTablePrefix(System::Boolean removeTablePrefix)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRemoveTablePrefix(removeTablePrefix))
}

NAMESPACE_OSGEO_COMMON::StringCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::get_GenTableList()
{
	FdoStringCollection* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGenTableList())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(unobj, true);
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::get_MaxSampleRows()
{
	FdoInt32 unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetMaxSampleRows())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration::set_MaxSampleRows(System::Int32 maxRows)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxSampleRows(maxRows))
}
