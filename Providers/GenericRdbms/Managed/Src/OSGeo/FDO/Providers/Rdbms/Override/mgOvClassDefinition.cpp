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
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>
#include <RdbmsOvClassDefinition.h>
#include <RdbmsOvPropertyDefinitionCollection.h>
#include <RdbmsOvTable.h>
#include <RdbmsOvGeometricColumn.h>
#include <Fdo\Commands\Schema\PhysicalElementMapping.h>
#include <Common\Xml\Writer.h>
#include <Fdo\Xml\Flags.h>

#include "mgOvTableMappingType.h"
#include "mgOvReadOnlyPropertyDefinitionCollection.h"
#include "mgOvTable.h"
#include "mgOvGeometricColumn.h"
#include "mgOvClassDefinition.h"
#include "mgObjectFactory.h"

FdoRdbmsOvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::GetImpObj()
{
	return static_cast<FdoRdbmsOvClassDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::Properties::get()
{
	FdoRdbmsOvReadOnlyPropertyDefinitionCollection * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvReadOnlyPropertyDefinitionCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTable^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::Table::get()
{
	FdoRdbmsOvTable * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTable())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvTable(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::Parent::set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetParent(static_cast<FdoPhysicalElementMapping*>(value->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::WriteXmlAttributes(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->_writeXmlAttributes(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), static_cast<FdoXmlFlags*>(flags->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::TableMapping::get()
{
	FdoSmOvTableMappingType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTableMapping())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition::TableMapping::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvTableMappingType mappingType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTableMapping(static_cast<FdoSmOvTableMappingType>(mappingType)))
}

