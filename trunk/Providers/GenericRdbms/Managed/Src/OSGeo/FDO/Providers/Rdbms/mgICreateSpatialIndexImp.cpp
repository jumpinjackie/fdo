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
#include <Rdbms.h>
#include <SpatialIndexType.h>
#include <SpatialIndexDimensionType.h>
#include <Fdo\Schema\GeometricPropertyDefinition.h>
#include <ICreateSpatialIndex.h>

#include "mgSpatialIndexType.h"
#include "mgSpatialIndexDimensionType.h"
#include "mgICreateSpatialIndexImp.h"

FdoICreateSpatialIndex* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::GetImpObj()
{
	return static_cast<FdoICreateSpatialIndex*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::ICreateSpatialIndexImp(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
{

}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::get_Name()
{
	const wchar_t* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetName())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::get_SpatialContextName()
{
	const wchar_t* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialContextName())

	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::set_SpatialContextName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::get_RdbmsSpatialIndexType()
{
	SpatialIndexType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialIndexType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::set_RdbmsSpatialIndexType(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialIndexType(static_cast<SpatialIndexType>(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::get_NumDimensions()
{
	SpatialIndexDimensionType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNumDimensions())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::set_NumDimensions(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNumDimensions(static_cast<SpatialIndexDimensionType>(value)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::set_GeometricProperty(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition *value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricProperty(static_cast<FdoGeometricPropertyDefinition*>(value->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::get_GeometricProperty()
{
	FdoGeometricPropertyDefinition* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometricProperty())

		return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateGeometricPropertyDefinition(unobj, true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

