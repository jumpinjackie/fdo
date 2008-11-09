/*
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
*
*/

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
public __gc class OvStyleDefinition;
public __gc class OvLayerCollection;
public __gc class OvLayerDefinition;
public __gc class OvRasterDefinition;
public __gc class OvClassDefinition;
public __gc class OvClassCollection;

public __gc __sealed class ObjectFactory
{
public:
    static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition* CreateOvStyleDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection* CreateOvLayerCollection(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition* CreateOvLayerDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition* CreateOvRasterDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition* CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassCollection* CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose);
private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
