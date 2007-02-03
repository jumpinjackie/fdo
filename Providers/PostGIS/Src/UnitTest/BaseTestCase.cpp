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
#include "BaseTestCase.h"

namespace fdo { namespace postgis { namespace test {

const wchar_t* providerName = L"OSGeo.PostGIS.3.2";

BaseTestCase::BaseTestCase() : mConnection(NULL)
{
}

BaseTestCase::~BaseTestCase()
{
}

void BaseTestCase::setUp ()
{
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mConnection = mgr->CreateConnection(providerName);
}

void BaseTestCase::tearDown ()
{
    mConnection = NULL;
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mgr->FreeLibrary(providerName);
}

FdoIConnection* BaseTestCase::GetConnection()
{
    return NULL;
}

}}} // namespace fdo::postgis::test
