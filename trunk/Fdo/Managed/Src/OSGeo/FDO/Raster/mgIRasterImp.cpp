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
#include <Fdo\Expression\DataValue.h>
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Raster\RasterDataModelType.h>
#include <Fdo\Raster\RasterDataOrganization.h>
#include <Fdo\Raster\IRaster.h>
#include <Fdo\Commands\CommandType.h>

#include "mgIRasterImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Raster\mgRasterDataModel.h"
#include "FDO\Raster\mgIRasterPropertyDictionaryImp.h"

NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::IRasterImp(IntPtr unmanaged, Boolean autoDelete) 
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoIRaster* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::GetImpObj()
{
	return static_cast<FdoIRaster*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Boolean NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::IsNull ()
{
	FdoBoolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsNull())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::SetNull ()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_NumberOfBands()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNumberOfBands())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_NumberOfBands (System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNumberOfBands(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_CurrentBand ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetCurrentBand())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_CurrentBand (System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCurrentBand(value))
}

System::Byte NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_Bounds () []
{
	FdoByteArray* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBounds())

	System::Byte mgBuffer __gc[] = FdoByteArrayToByteArray(result->GetData(), result->GetCount());
	result->Release();

	return mgBuffer;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_Bounds(System::Byte bounds[])
{
	FdoByteArray* byteArray = ByteArrayToFdoByteArray(bounds);

	EXCEPTION_HANDLER(GetImpObj()->SetBounds(byteArray))

	byteArray->Release();
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_ImageXSize ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetImageXSize())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_ImageXSize (System::Int32 size)
{
	EXCEPTION_HANDLER(GetImpObj()->SetImageXSize(size))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_ImageYSize ()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetImageYSize())
	return unobj;
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_ImageYSize (System ::Int32 size)
{
	EXCEPTION_HANDLER(GetImpObj()->SetImageYSize(size))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_NullPixelValue()
{
	FdoDataValue* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNullPixelValue())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(unobj, true);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_StreamReader (NAMESPACE_OSGEO_COMMON::IStreamReaderImp* reader)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStreamReader(static_cast<FdoIStreamReader*>(reader->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON::IStreamReaderImp* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_StreamReader ()
{
	FdoIStreamReader* unobj;

	EXCEPTION_HANDLER(unobj = GetImpObj()->GetStreamReader())

    return static_cast<NAMESPACE_OSGEO_COMMON::IStreamReaderImp*>(NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(unobj, true));
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_DataModel ()
{
	FdoRasterDataModel* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataModel())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateRasterDataModel(unobj, true);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_DataModel(NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel* dataModel)
{	
	EXCEPTION_HANDLER(GetImpObj()->SetDataModel(dataModel->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionary* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::GetAuxiliaryProperties()
{	
	FdoIRasterPropertyDictionary* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetAuxiliaryProperties())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterPropertyDictionary(unobj, true);
}

System::String* NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::get_VerticalUnits ()
{
	FdoString* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetVerticalUnits())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::set_VerticalUnits (System::String* units)
{
	EXCEPTION_HANDLER(GetImpObj()->SetVerticalUnits(StringToUni(units)))
}