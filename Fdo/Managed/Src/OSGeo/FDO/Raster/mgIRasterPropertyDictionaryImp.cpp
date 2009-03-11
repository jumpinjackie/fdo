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
#include <Fdo.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Raster\mgIRasterPropertyDictionaryImp.h"
#include "FDO\Expression\mgDataValue.h"
#include "FDO\Raster\mgDataValueCollection.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::IRasterPropertyDictionaryImp(IntPtr unmanaged, Boolean autoDelete) 
: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoIRasterPropertyDictionary* NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::GetImpObj()
{
	return static_cast<FdoIRasterPropertyDictionary*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON::StringCollection^ NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::PropertyNames::get()
{
	FdoStringCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyNames())
	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::PropertyDataType::get(System::String^ name)
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetPropertyDataType(StringToUni(name)))
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::Property::get(System::String^ name)
{
	FdoDataValue* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetProperty(StringToUni(name)))
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::Property::set(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetProperty(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::PropertyDefault::get(System::String^ name)
{
	FdoDataValue* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyDefault(StringToUni(name)))
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::IsPropertyRequired (System::String^ name)
{
	System::Boolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsPropertyRequired(StringToUni(name)))
	return unobj;
}

System::Boolean NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::IsPropertyEnumerable (System::String^ name)
{
	System::Boolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsPropertyEnumerable(StringToUni(name)))
	return unobj;
}

NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection^ NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::PropertyValues::get(System::String^ name)
{
	FdoDataValueCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyValues(StringToUni(name)))
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValueCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp::PropertyValues::set(System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection^ collection)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPropertyValues(StringToUni(name), collection->GetImpObj()))
}
