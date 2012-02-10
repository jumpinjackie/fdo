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

#include <vector>
#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ProviderCollection.h>
#include <Fdo/ClientServices/ClientServiceException.h>

// Create a class to hide the use of std::vector
class FdoProviderVectorArray 
{
public:
    std::vector<FdoProvider*> m_array;
};

// Constructs a default instance of a FdoProviderCollection.
FdoProviderCollection::FdoProviderCollection()
{
    m_providers = new FdoProviderVectorArray();
}

// Default destructor for FdoProviderCollection.
FdoProviderCollection::~FdoProviderCollection()
{
    for (int i=0; i<(int)m_providers->m_array.size(); i++) {
        FDO_SAFE_RELEASE(m_providers->m_array.at(i));
    }
    m_providers->m_array.clear();
    delete m_providers;
}

void FdoProviderCollection::Dispose()
{
    delete this;
}

// Gets the number of items in the collection
int FdoProviderCollection::GetCount() const
{
    return (int)m_providers->m_array.size();
}

// Gets the item in the collection at the specified index. 
// Throws an invalid argument exception if the index is out of range
FdoProvider * const FdoProviderCollection::GetItem(int index) const
{
    if (index < 0 || index >= (int)m_providers->m_array.size()) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_2_EMPTYINPUTPARAMETER)));
    }
    return FDO_SAFE_ADDREF(m_providers->m_array.at(index));
}

// Returns true if the collection contains the specified item, false otherwise
bool FdoProviderCollection::Contains(FdoString* name) const
{
    bool bStatus = false;
    
    if (name == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    for (int i = 0; i < (int)m_providers->m_array.size(); i++) {
        FdoProvider * provider = m_providers->m_array.at(i);
        if (provider == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

        FdoString* providerName = provider->GetName();
        if (providerName == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

#ifdef _WIN32
        if (_wcsicmp(name, providerName) == 0) {
#else
        if (wcscasecmp(name, providerName) == 0) {
#endif
            bStatus = true;
        }
    }

    return bStatus;
}

// Returns the index of the specified item in the collection or -1 if the item does not exist
int FdoProviderCollection::IndexOf(FdoString* name) const
{
    int iVal = -1;
    
    if (name == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    for (int i = 0; i < (int)m_providers->m_array.size(); i++) {
        FdoProvider * provider = m_providers->m_array.at(i);
        if (provider == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

        FdoString* providerName = provider->GetName();
        if (providerName == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

#ifdef _WIN32
        if (_wcsicmp(name, providerName) == 0) {
#else
        if (wcscasecmp(name, providerName) == 0) {
#endif
            iVal = i;
            break;
        }
    }

    return iVal;
}

// Adds a new provider to the collection. The pointer passed to the Add method is stored. 
// Throws an invalid argument exception if the collection alreay holds a provider with the same name
// Adding a provider to the collection only adds the item to the buffered vector contained in the collection.
// The provider is not persisted in the registry. The collection will attempt to deallocate the provider when
// the collection is destructed.
void FdoProviderCollection::Add(FdoProvider* provider)
{
    if (provider == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    if (this->Contains(provider->GetName())) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_6_INVALIDINPUTPARAMETER)));
    }

    m_providers->m_array.push_back(FDO_SAFE_ADDREF(provider));

    return;
}

// Removes a provider from the collection. The allocated provider is freed. removing a provider from the 
// collection only removes the item from the buffered vector contained in the collection. The provider is not 
// removed from the persistant data store. 
void FdoProviderCollection::Remove(FdoString* name)
{
    bool bStatus = false;
    
    if (name == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    for (std::vector<FdoProvider*>::iterator it = m_providers->m_array.begin(); it != m_providers->m_array.end(); it++) {
        FdoProvider * provider = (*it);
        if (provider == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

        FdoString* providerName = provider->GetName();
        if (providerName == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }

#ifdef _WIN32
        if (_wcsicmp(name, providerName) == 0) {
#else
        if (wcscasecmp(name, providerName) == 0) {
#endif
            FDO_SAFE_RELEASE(provider);
            m_providers->m_array.erase(it);
            bStatus = true;
            break;
        }
    }

    if (!bStatus) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_6_INVALIDINPUTPARAMETER)));
    }

    return;
}

