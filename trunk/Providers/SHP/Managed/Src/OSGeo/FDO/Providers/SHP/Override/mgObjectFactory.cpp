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
#include "FDO\Providers\SHP\Override\mgObjectFactory.h"
#include "FDO\Providers\SHP\Override\mgColumnDefinition.h"
#include "FDO\Providers\SHP\Override\mgPropertyDefinition.h"
#include "FDO\Providers\SHP\Override\mgPropertyDefinitionCollection.h"
#include "FDO\Providers\SHP\Override\mgClassDefinition.h"
#include "FDO\Providers\SHP\Override\mgClassCollection.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable* CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas *>(p) ?  new clas(p, autoDispose) : NULL);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateColumnDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ColumnDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreatePropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreatePropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ObjectFactory::CreateClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
    return new NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::ClassCollection(ptr, autoDispose);
}
