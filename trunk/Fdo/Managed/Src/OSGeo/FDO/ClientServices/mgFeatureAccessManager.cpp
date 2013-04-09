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
#include <Fdo\Commands\CommandType.h>
#include <Fdo/ClientServices/FeatureAccessManager.h>

#include "mgFeatureAccessManager.h"
#include "../mgIConnectionManagerImp.h"
#include "../mgIProviderRegistryImp.h"
#include "../mgObjectFactory.h"

NAMESPACE_OSGEO_FDO::IConnectionManager^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::FeatureAccessManager::GetConnectionManager()
{
	::IConnectionManager *ret;
	EXCEPTION_HANDLER(ret = FdoFeatureAccessManager::GetConnectionManager());
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionManager(IntPtr(ret), true);
}

NAMESPACE_OSGEO_FDO::IProviderRegistry^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::FeatureAccessManager::GetProviderRegistry()
{
	::IProviderRegistry *ret;
	EXCEPTION_HANDLER(ret = FdoFeatureAccessManager::GetProviderRegistry());
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIProviderRegistry(IntPtr(ret), true);
}

