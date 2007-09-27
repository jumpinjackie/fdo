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
	EXCEPTION_HANDLER(Attach(FdoWmsOvRasterDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::OvRasterDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

FdoWmsOvRasterDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::GetImpObj()
{
	return static_cast<FdoWmsOvRasterDefinition*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_ImageFormat()
{
    FdoString* result; 
    EXCEPTION_HANDLER(result = GetImpObj()->GetImageFormat())
    return new System::String(result);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_ImageFormat(System::String*  value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetImageFormat(StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_Transparent()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetTransparent())
	
	return result;
}
    
System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_Transparent(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTransparent(value))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_BackgroundColor()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBackgroundColor())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_BackgroundColor(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBackgroundColor(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_TimeDimension()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTimeDimension())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_TimeDimension(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTimeDimension(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_ElevationDimension()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetElevationDimension())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_ElevationDimension(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetElevationDimension(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_SpatialContextName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSpatialContextName())
	
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::set_SpatialContextName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSpatialContextName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition::get_Layers()
{
	FdoWmsOvLayerCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLayers())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvLayerCollection(result, true);
}
