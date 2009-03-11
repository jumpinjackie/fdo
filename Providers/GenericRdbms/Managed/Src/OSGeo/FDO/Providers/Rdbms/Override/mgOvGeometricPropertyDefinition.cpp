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
#include <RdbmsOvGeometricPropertyDefinition.h>

#include "mgOvGeometricColumnType.h"
#include "mgOvGeometricContentType.h"
#include "mgOvGeometricColumn.h"
#include "mgOvPropertyDefinition.h"
#include "mgOvGeometricPropertyDefinition.h"
#include "mgObjectFactory.h"

FdoRdbmsOvGeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoRdbmsOvGeometricPropertyDefinition*>((FdoIDisposable*)__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::OvGeometricPropertyDefinition(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition(unmanaged, autoDelete)
{
	
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumn^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::Column::get()
{
	FdoRdbmsOvGeometricColumn* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumn())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvGeometricColumn(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::GeometricColumnType::get()
{
	FdoSmOvGeometricColumnType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometricColumnType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::GeometricColumnType::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricColumnType columnType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricColumnType(static_cast<FdoSmOvGeometricColumnType>(columnType)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::GeometricContentType::get()
{
	FdoSmOvGeometricContentType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometricContentType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::GeometricContentType::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricContentType contentType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricContentType(static_cast<FdoSmOvGeometricContentType>(contentType)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::XColumnName::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetXColumnName())

	return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::XColumnName::set(System::String^ columnName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetXColumnName(StringToUni(columnName)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::YColumnName::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetYColumnName())

	return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::YColumnName::set(System::String^ columnName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetYColumnName(StringToUni(columnName)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::ZColumnName::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetZColumnName())

	return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition::ZColumnName::set(System::String^ columnName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZColumnName(StringToUni(columnName)))
}

