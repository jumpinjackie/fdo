//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "ConnectionInfo.h"
#include "Connection.h"
// FDO
#include <FdoCommonStringUtil.h>
// std
#include <cassert>

namespace fdo { namespace postgis {

ConnectionInfo::ConnectionInfo() : mConn(NULL)
{
}

ConnectionInfo::ConnectionInfo(Connection* conn) :
    mConn(conn)
{
}

ConnectionInfo::~ConnectionInfo()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void ConnectionInfo::Dispose()
{
    //FDOLOG_MARKER("ConnectionInfo::#Dispose");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIConnectionInfo interface
///////////////////////////////////////////////////////////////////////////////

FdoString* ConnectionInfo::GetProviderName()
{
    return ProviderName;
}

FdoString* ConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(MSG_POSTGIS_PROVIDER_DISPLAY_NAME,
                     const_cast<char*>(ProviderDisplayName));
}

FdoString* ConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(MSG_POSTGIS_PROVIDER_DESCRIPTION,
                     const_cast<char*>(ProviderDescription));
}

FdoString* ConnectionInfo::GetProviderVersion()
{
    return ProviderVersion;
}

FdoString* ConnectionInfo::GetFeatureDataObjectsVersion()
{
    return FdoVersion;
}

FdoIConnectionPropertyDictionary* ConnectionInfo::GetConnectionProperties()
{
    FDOLOG_MARKER("ConnectionInfo::+GetConnectionProperties");
    
    if (NULL == mPropertyDict)
    {
        FDOLOG_WRITE("Creating connection properties:");
        
        mPropertyDict = new FdoCommonConnPropDictionary(mConn);

        char* name = NULL;
        FdoPtr<ConnectionProperty> prop;

        // Username: isRequired
        wide_to_multibyte(name, PropertyUsername);
        prop = new ConnectionProperty(PropertyUsername,
               NlsMsgGet(MSG_POSTGIS_CONNECTION_PROPERTY_USERNAME, name),
               L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDict->AddProperty(prop);
        
        FDOLOG_WRITE("1. %s", name);

        // Password: isRequired + isProtected
        wide_to_multibyte(name, PropertyPassword);
        prop = new ConnectionProperty(PropertyPassword,
               NlsMsgGet(MSG_POSTGIS_CONNECTION_PROPERTY_PASSWORD, name),
               L"", true, true, false, false, false, false, false, 0, NULL);
        mPropertyDict->AddProperty(prop);
        
        FDOLOG_WRITE("2. %s", name);

        // Service: isRequired
        wide_to_multibyte(name, PropertyService);
        prop = new ConnectionProperty(PropertyService,
               NlsMsgGet(MSG_POSTGIS_CONNECTION_PROPERTY_SERVICE_NAME, name),
               L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDict->AddProperty(prop);
        
        FDOLOG_WRITE("3. %s", name);

        // Datastore: isEnumerable + isDatastoreName
        wide_to_multibyte(name, PropertyDatastore);
        prop = new ConnectionProperty(PropertyDatastore,
               NlsMsgGet(MSG_POSTGIS_CONNECTION_PROPERTY_DATASTORE, name),
               L"", false, false, false, false, false, false, false, 0, NULL);
               //L"", false, false, true, false, false, true, false, 0, NULL);
        mPropertyDict->AddProperty(prop);
        
        FDOLOG_WRITE("4. %s", name);
    }

    FDO_SAFE_ADDREF(mPropertyDict.p);
    return mPropertyDict.p;
}

FdoProviderDatastoreType ConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_DatabaseServer;
}

FdoStringCollection* ConnectionInfo::GetDependentFileNames()
{
    return NULL;
}

}} // namespace fdo::postgis
