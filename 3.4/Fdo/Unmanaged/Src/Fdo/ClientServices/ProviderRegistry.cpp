/***************************************************************************
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
 ***************************************************************************/
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include "RegistryUtility.h"
#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Fdo/ClientServices/ClientServiceException.h>
#include <Fdo/ClientServices/ProviderCollection.h>

// Constructs a default instance of a FdoProviderRegistry object.
FdoProviderRegistry::FdoProviderRegistry()
{
    m_providerCollection = NULL;

    m_providerCollection = new FdoProviderCollection();
    if (m_providerCollection == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
    }

    std::vector<FdoProvider*> providers;
    FdoRegistryUtility::GetProviderCollection(providers);

    for (int i=0; i<(int)providers.size(); i++) {
        FdoProvider *provider = providers.at(i);
        m_providerCollection->Add(provider);
        provider->Release();
    }
}

// Default destructor for FdoProviderRegistry object.
FdoProviderRegistry::~FdoProviderRegistry()
{
    if (m_providerCollection != NULL) {
        delete m_providerCollection;
        m_providerCollection = NULL;
    }
}

void FdoProviderRegistry::Dispose()
{
    delete this;
}

// Gets a read only collection of information describing each of the registered feature providers
const FdoProviderCollection* FdoProviderRegistry::GetProviders() const
{
    return m_providerCollection;
}

// Registers a provider given the necessary information to register the provider. 
// If any of the parameters are missing or invalid and exception is thrown
void FdoProviderRegistry::RegisterProvider(const wchar_t * name, 
                                           const wchar_t * displayName, 
                                           const wchar_t * description, 
                                           const wchar_t * version, 
                                           const wchar_t * fdoVersion, 
                                           const wchar_t * libraryPath,
                                           bool isManaged)
{
    if (name == NULL || displayName == NULL || description == NULL || version == NULL || fdoVersion == NULL || libraryPath == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    FdoRegistryUtility::RegisterProvider(name, displayName, description, version, fdoVersion, libraryPath, isManaged);

    if (m_providerCollection->Contains(name)) {
        FdoProvider* provider = m_providerCollection->GetItem(m_providerCollection->IndexOf(name));
        if (provider == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

        provider->Set(name, displayName, description, version, fdoVersion, libraryPath, isManaged);
        provider->Release();
    }
    else {
        FdoProvider* provider = new FdoProvider(name, displayName, description, version, fdoVersion, libraryPath, isManaged);
        if (provider == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
        }

        m_providerCollection->Add(provider);
        provider->Release();
    }
}

// Unregisters the provider with the specified name
void FdoProviderRegistry::UnregisterProvider(const wchar_t * name)
{
    if (name == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    FdoRegistryUtility::UnregisterProvider(name);

    m_providerCollection->Remove(name);
}

