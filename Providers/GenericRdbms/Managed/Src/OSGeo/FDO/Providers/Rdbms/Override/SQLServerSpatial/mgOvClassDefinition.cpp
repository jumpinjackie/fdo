/*
* (C) Copyright 2005 by Autodesk, Inc. All Rights Reserved.
*
* By using this code, you are agreeing to the terms and conditions of
* the License Agreement included in the documentation for this code.
*
* AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
* CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
* IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
* DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
* DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
*
* Use, duplication, or disclosure by the U.S. Government is subject
* to restrictions set forth in FAR 52.227-19 (Commercial Computer
* Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
* (Rights in Technical Data and Computer Software), as applicable.
*
*
*/

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