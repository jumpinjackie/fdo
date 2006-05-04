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

Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::Dispose(Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::RasterDataModel() : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoRasterDataModel::Create(), true))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_DataModelType ()
{
	FdoRasterDataModelType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataModelType())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType>(unobj);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_DataModelType (NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType type)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataModelType(static_cast<FdoRasterDataModelType>(type)))
}

System ::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_BitsPerPixel ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetBitsPerPixel())
	return unobj;
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_BitsPerPixel (System::Int32 bitsPerPixel)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBitsPerPixel(bitsPerPixel))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_Organization()
{
	FdoRasterDataOrganization unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOrganization())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_Organization (NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization organization)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOrganization(static_cast<FdoRasterDataOrganization>(organization)))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_TileSizeX ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTileSizeX())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_TileSizeX (System::Int32 sizex)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTileSizeX(sizex))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_TileSizeY ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTileSizeY())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_TileSizeY (System::Int32 sizey)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTileSizeY(sizey))
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataType NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::get_DataType ()
{
	FdoRasterDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_RASTER::RasterDataType>(unobj);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel::set_DataType (NAMESPACE_OSGEO_FDO_RASTER::RasterDataType dataType)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataType(static_cast<FdoRasterDataType>(dataType)))
}