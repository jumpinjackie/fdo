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
#include <Fdo\Clientservices\ConnectionManager.h>

#include "FDO\ClientServices\mgConnectionManager.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\mgIConnectionImp.h"

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ConnectionManager::ConnectionManager(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoConnectionManager* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ConnectionManager::GetImpObj()
{
	return static_cast<FdoConnectionManager*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ConnectionManager::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ConnectionManager::CreateConnection(String* providerName)
{
	FdoIConnection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->CreateConnection(StringToUni(providerName)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ConnectionManager::FreeLibrary(String* providerName)
{
	EXCEPTION_HANDLER(GetImpObj()->FreeLibrary(StringToUni(providerName)))
}
