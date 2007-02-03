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
#include "Pch.h"
#include "PostGisTestCase.h"

namespace fdo { namespace postgis { namespace test {

const wchar_t* providerName = L"OSGeo.PostGIS.3.2";

}}} // namespace fdo::postgis::test

PostGisTestCase::PostGisTestCase() : mConnection(NULL)
{
}

PostGisTestCase::~PostGisTestCase()
{
}

void PostGisTestCase::setUp ()
{
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mConnection = mgr->CreateConnection(fdo::postgis::test::providerName);
}

void PostGisTestCase::tearDown ()
{
    mConnection = NULL;
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mgr->FreeLibrary(fdo::postgis::test::providerName);
}

FdoIConnection* PostGisTestCase::GetConnection()
{
    return NULL;
}
