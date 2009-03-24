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
#include <Fdo\Clientservices\ProviderRegistry.h>
#include <Fdo\Clientservices\ProviderCollection.h>
#include <Fdo\IProviderRegistry.h>

#include "FDO\ClientServices\mgProviderRegistry.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\ClientServices\mgProviderCollection.h"

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::ProviderRegistry(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoProviderRegistry* NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::GetImpObj()
{
	return static_cast<FdoProviderRegistry*>(__super::UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection^ NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::GetProviders()
{
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProviderCollection(IntPtr(const_cast<FdoProviderCollection*>(GetImpObj()->GetProviders())), true);
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::RegisterProvider(System::String^ name, 
													System::String^ displayName, 
													System::String^ description, 
													System::String^ version, 
													System::String^ fdoVersion, 
													System::String^ libraryPath,
													System::Boolean isManaged)
{
	EXCEPTION_HANDLER(GetImpObj()->RegisterProvider(StringToUni(name),
		StringToUni(displayName),
		StringToUni(description),
		StringToUni(version),
		StringToUni(fdoVersion),
		StringToUni(libraryPath),
		isManaged))
}

System::Void NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderRegistry::UnregisterProvider(System::String^ name)
{
	EXCEPTION_HANDLER(GetImpObj()->UnregisterProvider(StringToUni(name)))
}


