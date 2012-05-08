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
#include <RdbmsOvPhysicalSchemaMapping.h>
#include <RdbmsOvClassCollection.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>
#include <RdbmsOvSchemaAutoGeneration.h>
#include <Fdo\Commands\Schema\PhysicalSchemaMapping.h>
#include <Fdo\Commands\Schema\PhysicalElementMapping.h>

#include "mgOvGeometricColumnType.h"
#include "mgOvGeometricContentType.h"
#include "mgOvTableMappingType.h"
#include "mgOvReadOnlyClassCollection.h"
#include "mgOvSchemaAutoGeneration.h"
#include "mgOvPhysicalSchemaMapping.h"
#include "mgObjectFactory.h"


FdoRdbmsOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GetImpObj()
{
	return static_cast<FdoRdbmsOvPhysicalSchemaMapping*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(schemaMapping, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::Classes::get()
{
	FdoRdbmsOvReadOnlyClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return ObjectFactory::CreateOvReadOnlyClassCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::TableMapping::get()
{
	FdoSmOvTableMappingType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTableMapping())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::TableMapping::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType mappingType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTableMapping(static_cast<FdoSmOvTableMappingType>(mappingType)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::AutoGeneration::get()
{
	FdoRdbmsOvSchemaAutoGeneration* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAutoGeneration())

	return ObjectFactory::CreateOvSchemaAutoGeneration(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::AutoGeneration::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvSchemaAutoGeneration^ autoGeneration)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAutoGeneration(autoGeneration->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::Parent::set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetParent(static_cast<FdoPhysicalElementMapping*>(value->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GeometricColumnType::get()
{
	FdoSmOvGeometricColumnType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometricColumnType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GeometricColumnType::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricColumnType(static_cast<FdoSmOvGeometricColumnType>(columnType)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GeometricContentType::get()
{
	FdoSmOvGeometricContentType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometricContentType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping::GeometricContentType::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricContentType(static_cast<FdoSmOvGeometricContentType>(contentType)))
}
