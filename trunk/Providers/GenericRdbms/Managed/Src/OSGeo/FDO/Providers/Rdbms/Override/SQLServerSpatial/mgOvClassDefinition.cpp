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
#include "SQLServerSpatial\SqlServerOvClassDefinition.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvTextInRowOption.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyDefinitionCollection.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvTable.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvGeometricColumn.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::OvClassDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSqlServerOvClassDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::OvClassDefinition(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSqlServerOvClassDefinition::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition(unmanaged, autoDelete)
{

}

FdoSqlServerOvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::GetImpObj()
{
	return static_cast<FdoSqlServerOvClassDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_Properties()
{
	FdoSqlServerOvPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())
	
	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvPropertyDefinitionCollection(result, true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_Table()
{
	FdoSqlServerOvTable* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTable())
	
	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvTable(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::set_Table(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTable((NULL == value ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::set_IdentityPropertyName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIdentityPropertyName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_IdentityPropertyName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentityPropertyName())
	
	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_IdentityIsGloballyUnique()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj() ->GetIdentityIsGloballyUnique())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::set_IdentityIsGloballyUnique(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIdentityIsGloballyUnique(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_IdentitySeed()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentitySeed())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::set_IdentitySeed(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIdentitySeed(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::get_IdentityIncrement()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentityIncrement())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition::set_IdentityIncrement(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIdentityIncrement(value))
}