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
// The following header files sequence is very important
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Expression\DataValue.h>
#include <Fdo\Commands\Feature\IReader.h>
#include <Fdo\Commands\Feature\IDataReader.h>

#include "FDO\Commands\Feature\mgIDataReaderImp.h"

FdoIDataReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetImpObj()
{
    return static_cast<FdoIDataReader*>(__super::UnmanagedObject.ToPointer());
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyCount()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyCount())

	return result;
}

System::String* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyName(System::Int32 index)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyName(index))

	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetDataType(System::String* name)
{
	FdoDataType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyType(System::String* name)
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::Dispose()
{
	NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose();
}
