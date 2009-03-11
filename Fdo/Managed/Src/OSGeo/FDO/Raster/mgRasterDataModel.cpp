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
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Raster\RasterDataModelType.h>
#include <Fdo\Raster\RasterDataOrganization.h>

#include "FDO\Raster\mgRasterDataModel.h"

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::RasterDataModel(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoRasterDataModel* NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::GetImpObj()
{
	return static_cast<FdoRasterDataModel*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::RasterDataModel() : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoRasterDataModel::Create()), true))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::DataModelType::get()
{
	FdoRasterDataModelType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataModelType())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType>(unobj);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::DataModelType::set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType type)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataModelType(static_cast<FdoRasterDataModelType>(type)))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::BitsPerPixel::get()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetBitsPerPixel())
	return unobj;
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::BitsPerPixel::set(System::Int32 bitsPerPixel)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBitsPerPixel(bitsPerPixel))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::Organization::get()
{
	FdoRasterDataOrganization unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOrganization())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::Organization::set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization organization)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOrganization(static_cast<FdoRasterDataOrganization>(organization)))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::TileSizeX::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTileSizeX())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::TileSizeX::set(System::Int32 sizex)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTileSizeX(sizex))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::TileSizeY::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTileSizeY())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::TileSizeY::set(System::Int32 sizey)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTileSizeY(sizey))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataType NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::DataType::get()
{
	FdoRasterDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataType>(unobj);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::DataType::set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataType dataType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataType(static_cast<FdoRasterDataType>(dataType)))
}