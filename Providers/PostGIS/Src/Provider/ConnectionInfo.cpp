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

#include <cassert>

namespace fdo { namespace postgis {

ConnectionInfo::ConnectionInfo()
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
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* ConnectionInfo::GetProviderDisplayName()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* ConnectionInfo::GetProviderDescription()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* ConnectionInfo::GetProviderVersion()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* ConnectionInfo::GetFeatureDataObjectsVersion()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoIConnectionPropertyDictionary* ConnectionInfo::GetConnectionProperties()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoProviderDatastoreType ConnectionInfo::GetProviderDatastoreType()
{
    assert(!"NOT IMPLEMENTED");
    return FdoProviderDatastoreType_DatabaseServer;
}

FdoStringCollection* ConnectionInfo::GetDependentFileNames()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

}} // namespace fdo::postgis
