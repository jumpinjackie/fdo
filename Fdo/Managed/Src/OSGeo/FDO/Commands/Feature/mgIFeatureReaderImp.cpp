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
#include <Fdo\Commands\Feature\IFeatureReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassDefinition.h"

FdoIFeatureReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetImpObj()
{
    return static_cast<FdoIFeatureReader*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetClassDefinition()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassDefinition())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetDepth()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDepth())

	return result;
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetGeometry(System::String^ propertyName)
{
	const FdoByte* result;
	FdoInt32 count = 0;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometry(StringToUni(propertyName), &count))

	return FdoByteArrayToByteArray(result, count);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp::GetFeatureObject(System::String^ propertyName)
{
	FdoIFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureObject(StringToUni(propertyName)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFeatureReader(IntPtr(result), true);
}
