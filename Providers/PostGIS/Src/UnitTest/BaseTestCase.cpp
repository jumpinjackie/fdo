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
#include "TestConfig.h"

// std
#include <sstream>
#include <string>
#include <iomanip>

namespace fdo { namespace postgis { namespace test {

BaseTestCase::BaseTestCase() : mConnection(NULL)
{
}

BaseTestCase::~BaseTestCase()
{
}

void BaseTestCase::setUp()
{
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mConnection = mgr->CreateConnection(gTestConfig.getProviderName());
}

void BaseTestCase::tearDown()
{
    mConnection = NULL;
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mgr->FreeLibrary(gTestConfig.getProviderName());
}

FdoIConnection* BaseTestCase::GetConnection()
{
    FDO_SAFE_ADDREF(mConnection.p);
    return mConnection.p;
}

void BaseTestCase::fail(FdoException* ex)
{
    std::streamsize offset = 5; // number of blanks + asterix characters

    std::wostringstream msg;
    msg << L"\n*** FDO FAILURE ***\n";
    msg << std::setw(++offset) << L"*** " << ex->GetExceptionMessage() << std::endl;
    
    FdoPtr<FdoException> cause(ex->GetCause());
    while(NULL != cause)
    {
        msg << std::setw(++offset)
             << L"*** " << cause->GetExceptionMessage() << std::endl;
        cause = cause->GetCause();
    }
    ex->Release();

    std::string tmp(fdo::conv::tonarrow(msg.str()));
    CPPUNIT_FAIL(tmp);
}

void BaseTestCase::fail(char const* error)
{
    std::string msg("\n*** FAILURE ***\n");
    msg += error;
    CPPUNIT_FAIL(msg);
}

}}} // namespace fdo::postgis::test
