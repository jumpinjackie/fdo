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
#include "SQLServerSpatial\SqlServerOvDataPropertyDefinition.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvDataPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvColumn.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::OvDataPropertyDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach((FdoIDisposable*)FdoSqlServerOvDataPropertyDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::OvDataPropertyDefinition(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach((FdoIDisposable*)FdoSqlServerOvDataPropertyDefinition::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::OvDataPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition(unmanaged, autoDelete)
{

}

FdoSqlServerOvDataPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoSqlServerOvDataPropertyDefinition*>((FdoIDisposable*)__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::get_Column()
{
	FdoSqlServerOvColumn* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvColumn(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvDataPropertyDefinition::set_Column(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetColumn((NULL == value ? NULL : value->GetImpObj())))
}