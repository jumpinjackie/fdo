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
#include "SQLServerSpatial\SqlServerOvColumn.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvColumn.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::OvColumn() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSqlServerOvColumn::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::OvColumn(System::String* name) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSqlServerOvColumn::Create(StringToUni(name)), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::OvColumn(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvColumn(unmanaged, autoDelete)
{

}

FdoSqlServerOvColumn* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::GetImpObj()
{
	return static_cast<FdoSqlServerOvColumn*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::get_Formula()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFormula())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn::set_Formula(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFormula(StringToUni(value)))
}