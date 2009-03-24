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
	return static_cast<FdoIRaster*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Boolean NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::IsNull ()
{
	System::Boolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsNull())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::SetNull ()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::NumberOfBands::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetNumberOfBands())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::NumberOfBands::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNumberOfBands(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::CurrentBand::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetCurrentBand())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::CurrentBand::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCurrentBand(value))
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::Bounds::get()
{
	FdoByteArray* arr = nullptr;
    array<System::Byte>^ result;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetBounds())

	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::Bounds::set(array<System::Byte>^ bounds)
{
	FdoByteArray* byteArray = ByteArrayToFdoByteArray(bounds);
    try
    {
	    EXCEPTION_HANDLER(GetImpObj()->SetBounds(byteArray))
    }
    finally
    {
        if (byteArray != nullptr)
	        byteArray->Release();
    }
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::ImageXSize::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetImageXSize())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::ImageXSize::set(System::Int32 size)
{
	EXCEPTION_HANDLER(GetImpObj()->SetImageXSize(size))
}

System::Int32 NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::ImageYSize::get()
{
	System::Int32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetImageYSize())
	return unobj;
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::ImageYSize::set(System ::Int32 size)
{
	EXCEPTION_HANDLER(GetImpObj()->SetImageYSize(size))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::NullPixelValue::get()
{
	FdoDataValue* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetNullPixelValue())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(IntPtr(result), true);
}

System ::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::StreamReader::set(NAMESPACE_OSGEO_COMMON::IStreamReaderImp^ reader)
{
	EXCEPTION_HANDLER(GetImpObj()->SetStreamReader(static_cast<FdoIStreamReader*>(reader->UnmanagedObject.ToPointer())))
}

NAMESPACE_OSGEO_COMMON::IStreamReaderImp^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::StreamReader::get()
{
	FdoIStreamReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetStreamReader())

    return static_cast<NAMESPACE_OSGEO_COMMON::IStreamReaderImp^>(NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(IntPtr(result), true));
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::DataModel::get()
{
	FdoRasterDataModel* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetDataModel())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateRasterDataModel(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::DataModel::set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ dataModel)
{	
	EXCEPTION_HANDLER(GetImpObj()->SetDataModel(dataModel->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionary^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::GetAuxiliaryProperties()
{	
	FdoIRasterPropertyDictionary* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetAuxiliaryProperties())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterPropertyDictionary(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::VerticalUnits::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetVerticalUnits())
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterImp::VerticalUnits::set(System::String^ units)
{
	EXCEPTION_HANDLER(GetImpObj()->SetVerticalUnits(StringToUni(units)))
}