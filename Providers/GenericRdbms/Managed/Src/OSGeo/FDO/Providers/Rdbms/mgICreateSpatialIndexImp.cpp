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

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::Name::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetName())

    return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::SpatialContextName::get()
{
	FdoString* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialContextName())

    return CHECK_STRING(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::SpatialContextName::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::RdbmsSpatialIndexType::get()
{
	SpatialIndexType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialIndexType())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::RdbmsSpatialIndexType::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialIndexType(static_cast<SpatialIndexType>(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::NumDimensions::get()
{
	SpatialIndexDimensionType unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNumDimensions())

	return static_cast<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::NumDimensions::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNumDimensions(static_cast<SpatialIndexDimensionType>(value)))
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::GeometricProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometricProperty(static_cast<FdoGeometricPropertyDefinition*>(value->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::GeometricProperty::get()
{
	FdoGeometricPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometricProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateGeometricPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ICreateSpatialIndexImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}

