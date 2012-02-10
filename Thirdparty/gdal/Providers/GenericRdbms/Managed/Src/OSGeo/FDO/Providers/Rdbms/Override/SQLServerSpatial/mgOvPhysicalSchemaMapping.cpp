//
// Copyright (C) 2004-2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "stdafx.h"
#include "Rdbms\Override\RdbmsOv.h"
#include "SQLServerSpatial\SqlServerOvPhysicalSchemaMapping.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvTextInRowOption.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPhysicalSchemaMapping.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSqlServerOvPhysicalSchemaMapping::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::String^ name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSqlServerOvPhysicalSchemaMapping::Create(StringToUni(name))), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::OvPhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ schemaMapping, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPhysicalSchemaMapping(schemaMapping, autoDelete)
{
    // Validate IN Parameter
    if (nullptr == schemaMapping)
        return;

    // Retrieve the provider name for the FdoSqlServerOvPhysicalSchemaMapping class
    FdoPtr<FdoSqlServerOvPhysicalSchemaMapping> sqlServerSchemaMapping = FdoSqlServerOvPhysicalSchemaMapping::Create();
    FdoStringP sqlServerProviderName = sqlServerSchemaMapping->GetProvider();

    // Retrieve the provider name of the schema mapping object passed into the constructor
    System::String^ sProviderName = schemaMapping->Provider;

    // If the provider named do not match throw an Invalid Argument exception
    if (sqlServerProviderName != StringToUni(sProviderName)) {

		//Wait for changing again
        throw gcnew System::ArgumentException();
    }
}

FdoSqlServerOvPhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::GetImpObj()
{
	return static_cast<FdoSqlServerOvPhysicalSchemaMapping*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::Classes::get()
{
	FdoSqlServerOvClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvClassCollection(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TableFilegroup::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTableFilegroup())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TableFilegroup::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTableFilegroup(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TextFilegroup::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTextFilegroup())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TextFilegroup::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTextFilegroup(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::IndexFilegroup::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIndexFilegroup())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::IndexFilegroup::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIndexFilegroup(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TextInRow::get()
{
	SqlServerOvTextInRowOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTextInRow())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption>(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::TextInRow::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTextInRowOption value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTextInRow(static_cast<SqlServerOvTextInRowOption>(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::Database::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDatabase())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::Database::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDatabase(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::Owner::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOwner())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPhysicalSchemaMapping::Owner::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOwner(StringToUni(value)))
}