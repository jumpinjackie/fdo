/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
*
*/

#include "stdafx.h"
#include "Fdo.h"
#include "FdoWmsOvRasterDefinition.h"

#include "FDO\Providers\WMS\Override\mgOvRasterDefinition.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvLayerCollection.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::OvRasterDefinition() : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoWmsOvRasterDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::OvRasterDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

FdoWmsOvRasterDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::GetImpObj()
{
	return static_cast<FdoWmsOvRasterDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::ImageFormat::get()
{
    FdoString* result; 
    EXCEPTION_HANDLER(result = GetImpObj()->GetImageFormat())
    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::ImageFormat::set(System::String^  value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetImageFormat(StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::Transparent::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetTransparent())
	
	return result;
}
    
System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::Transparent::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTransparent(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::UseTileCache::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetUseTileCache())
	
	return result;
}
    
System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::UseTileCache::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetUseTileCache(value))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::BackgroundColor::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBackgroundColor())
	
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::BackgroundColor::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBackgroundColor(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::TimeDimension::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTimeDimension())
	
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::TimeDimension::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTimeDimension(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::ElevationDimension::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetElevationDimension())
	
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::ElevationDimension::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetElevationDimension(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::SpatialContextName::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSpatialContextName())
	
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::SpatialContextName::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::Layers::get()
{
	FdoWmsOvLayerCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLayers())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvLayerCollection(IntPtr(result), true);
}
