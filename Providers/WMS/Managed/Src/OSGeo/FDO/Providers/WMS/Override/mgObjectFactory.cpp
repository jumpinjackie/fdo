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

#include "stdafx.h"
#include "WMS\Override\FdoWmsOvFormatType.h"

#include "FDO\Providers\WMS\Override\mgOvFormatType.h"
#include "FDO\Providers\WMS\Override\mgObjectFactory.h"
#include "FDO\Providers\WMS\Override\mgOvStyleDefinition.h"
#include "FDO\Providers\WMS\Override\mgOvLayerCollection.h"
#include "FDO\Providers\WMS\Override\mgOvLayerDefinition.h"
#include "FDO\Providers\WMS\Override\mgOvRasterDefinition.h"
#include "FDO\Providers\WMS\Override\mgOvClassDefinition.h"
#include "FDO\Providers\WMS\Override\mgOvClassCollection.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable* CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas *>(p) ?  new clas(p, autoDispose) : NULL);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvStyleDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvLayerCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvLayerDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvRasterDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::ObjectFactory::CreateOvClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvClassCollection(ptr, autoDispose);
}