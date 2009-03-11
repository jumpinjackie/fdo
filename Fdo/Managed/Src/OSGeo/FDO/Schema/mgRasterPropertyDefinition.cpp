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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\RasterPropertyDefinition.h>

#include "FDO\Schema\mgRasterPropertyDefinition.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Raster\mgRasterDataModel.h"

NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::RasterPropertyDefinition() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoRasterPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::RasterPropertyDefinition(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoRasterPropertyDefinition::Create(StringToUni(name), StringToUni(description))), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::RasterPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoRasterPropertyDefinition::Create(StringToUni(name), StringToUni(description), system)), true))
}

FdoRasterPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::GetImpObj()
{
	return static_cast<FdoRasterPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::ReadOnly::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetReadOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::ReadOnly::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReadOnly(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::Nullable::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetNullable())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::Nullable::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNullable(value))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultDataModel::get()
{
	FdoRasterDataModel* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultDataModel())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateRasterDataModel(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultDataModel::set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultDataModel((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultImageXSize::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultImageXSize())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultImageXSize::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultImageXSize(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultImageYSize::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultImageYSize())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::DefaultImageYSize::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultImageYSize(value))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::SpatialContextAssociation::set(System::String^ spatialContextName)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextAssociation(StringToUni(spatialContextName)))
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition::SpatialContextAssociation::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSpatialContextAssociation())

	return CHECK_STRING(result);
}