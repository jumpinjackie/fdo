//
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
 

#include "stdafx.h"
#include "Common/FdoProxyConnectionManager.h"


FdoProxyConnectionManager::FdoProxyConnectionManager(void)
{
}

FdoProxyConnectionManager::~FdoProxyConnectionManager(void)
{
}


FdoIConnection* FdoProxyConnectionManager::CreateConnection(const wchar_t *providerName) {
	try {
		const FdoProvider* provider = NULL;
		const FdoProviderCollection* providers = NULL;
		IConnectionManager* manager = NULL;
		IProviderRegistry* registry = NULL;
		FdoIConnection *pFdoConnection = NULL;
		int providerIndex = 0;

		// Validate the input parameters
		if (providerName == NULL) {
			throw FdoException::Create(L"Cannot create connection. Provider name is NULL.");
		}

		// Retrieve the providers from the registry
		registry = FdoFeatureAccessManager::GetProviderRegistry();
		providers = registry->GetProviders();

		// Find the requested provider		
		providerIndex = providers->IndexOf(providerName);
		if (providerIndex == -1) {
			StringBuffer *pBuffer = new StringBuffer();
			pBuffer->append(L"Cannot create connection. Cannot find FDO Provider named ");
			pBuffer->append(providerName);
			FdoString *pErrorMsg = pBuffer->toString();
			delete pBuffer;
			throw FdoException::Create(pErrorMsg);
		}

		// Create the connection object
		manager = FdoFeatureAccessManager::GetConnectionManager ();
		pFdoConnection = manager->CreateConnection(providerName);

		return pFdoConnection;
	}
	catch (FdoClientServiceException* exception) {
		FdoException *newException = FdoException::Create(exception->ToString() );
        exception->Release();
		throw newException;
    }
	catch (FdoException* exception) {
		throw exception;
	}
	catch (...){
		throw;
	}
}
