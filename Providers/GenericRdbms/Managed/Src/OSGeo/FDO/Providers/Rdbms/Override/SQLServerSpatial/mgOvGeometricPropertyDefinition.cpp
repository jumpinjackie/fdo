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
#include "SQLServerSpatial\SqlServerOvGeometricPropertyDefinition.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvGeometricPropertyDefinition.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvGeometricColumn.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach((FdoIDisposable*)FdoSqlServerOvGeometricPropertyDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach((FdoIDisposable*)FdoSqlServerOvGeometricPropertyDefinition::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition(unmanaged, autoDelete)
{

}

FdoSqlServerOvGeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoSqlServerOvGeometricPropertyDefinition*>((FdoIDisposable*)__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::get_Column()
{
	FdoSqlServerOvGeometricColumn* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvGeometricColumn(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricPropertyDefinition::set_Column(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvGeometricColumn* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetColumn((NULL == value ? NULL : value->GetImpObj())))
}