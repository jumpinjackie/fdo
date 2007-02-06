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
    if (NULL == mPropertyDict)
    {
        mPropertyDict = new ::FdoCommonConnPropDictionary(mConn);
    }
    return NULL;
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
