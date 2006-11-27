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

#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ConnectionManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Fdo/ClientServices/ClientServiceException.h>

// Define static variables
FdoConnectionManager * FdoFeatureAccessManager::m_connectionManager = NULL;
FdoProviderRegistry  * FdoFeatureAccessManager::m_providerRegistry  = NULL;

// Static method that gets an object that implements IConnection Manager
IConnectionManager* FdoFeatureAccessManager::GetConnectionManager()
{
    if (m_connectionManager == NULL) {
        m_connectionManager = new FdoConnectionManager();
        if (m_connectionManager == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
        }
    }
    
    return FDO_SAFE_ADDREF(m_connectionManager);
}

// Static method that gets an object that implements IProviderRegistry
IProviderRegistry* FdoFeatureAccessManager::GetProviderRegistry()
{
    if (m_providerRegistry == NULL) {
        m_providerRegistry = new FdoProviderRegistry();
        if (m_providerRegistry == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
        }
    }

	return FDO_SAFE_ADDREF(m_providerRegistry);
}

void FdoFeatureAccessManager::Reset()
{
   FDO_SAFE_RELEASE(m_connectionManager);
   FDO_SAFE_RELEASE(m_connectionManager);
   FDO_SAFE_RELEASE(m_providerRegistry);
   FDO_SAFE_RELEASE(m_providerRegistry);
}




