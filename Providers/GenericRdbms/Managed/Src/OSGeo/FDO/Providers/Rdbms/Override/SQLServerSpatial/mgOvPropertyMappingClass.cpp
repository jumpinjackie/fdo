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
#include "SQLServerSpatial\SqlServerOvPropertyMappingClass.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingClass.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass::OvPropertyMappingClass() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach((FdoIDisposable*)FdoSqlServerOvPropertyMappingClass::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass::OvPropertyMappingClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingClass(unmanaged, autoDelete)
{

}

FdoSqlServerOvPropertyMappingClass* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass::GetImpObj()
{
	return dynamic_cast<FdoSqlServerOvPropertyMappingClass*>((FdoIDisposable*)__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass::get_InternalClass()
{
	FdoSqlServerOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInternalClass())
	
	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvClassDefinition(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingClass::set_InternalClass(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInternalClass((NULL == value ? NULL : value->GetImpObj())))
}