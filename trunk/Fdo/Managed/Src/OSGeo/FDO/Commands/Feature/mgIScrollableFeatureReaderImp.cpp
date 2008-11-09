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
#include <Fdo\Commands\Feature\IScrollableFeatureReader.h>
#include "FDO\Commands\mgPropertyValueCollection.h"

#include "FDO\Commands\Feature\mgIScrollableFeatureReaderImp.h"
#include <Fdo\Commands\CommandType.h>
#include "FDO\mgObjectFactory.h"

FdoIScrollableFeatureReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::GetImpObj()
{
    return static_cast<FdoIScrollableFeatureReader*>(__super::UnmanagedObject.ToPointer());
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::Count()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->Count())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::ReadFirst()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadFirst())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::ReadLast()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadLast())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::ReadPrevious()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadPrevious())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::ReadAt(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* key)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadAt(static_cast<FdoPropertyValueCollection*>(key->UnmanagedObject.ToPointer())));

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::ReadAtIndex(System::UInt32 recordindex)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->ReadAtIndex(recordindex));

	return result;
}

System::UInt32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp::IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* key)
{
	unsigned int result;

	EXCEPTION_HANDLER(result = GetImpObj()->IndexOf(static_cast<FdoPropertyValueCollection*>(key->UnmanagedObject.ToPointer())));

	return result;
}



