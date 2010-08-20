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

#include "Pch.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <winnls.h>
#endif
#include "ClientServicesTest.h"
#include "Fdo/ClientServices/ClientServices.h"
#include "Fdo/ClientServices/FeatureAccessManager.h"
#include "Fdo/ClientServices/ConnectionManager.h"
#include "Fdo/ClientServices/ProviderRegistry.h"
#include "Fdo/ClientServices/ClientServiceException.h"
#include "Fdo/ClientServices/Provider.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ClientServicesTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ClientServicesTest, "ClientServicesTest");

ClientServicesTest::ClientServicesTest(void)
{
}

ClientServicesTest::~ClientServicesTest(void)
{
}

void ClientServicesTest::setUp ()
{
    dt1 = new FdoCSTestData(std::wstring(L"Autodesk.Oracle.3.2"), 
                            std::wstring(L"Oracle"), 
                            std::wstring(L"Oracle Spatial FDO Provider"), 
                            std::wstring(L"3.2.0.0"), 
                            std::wstring(L"3.2.0.0"), 
#ifdef _WIN32
                            std::wstring(L"FdoRdbms.dll"), // relies on the unit test being in the same 
#else
                            std::wstring(L"libFdoRdbms.so"), // relies on the unit test being in the same 
#endif
                                                           // directory as FdoRdbms.dll -- //danube\Bin\Win32\Debug
                            false);

    dt2 = new FdoCSTestData(std::wstring(L"TestData2"), 
                            std::wstring(L"TestData2"), 
                            std::wstring(L"TestDescription2"), 
                            std::wstring(L"3.0.3.0"), 
                            std::wstring(L"3.2.0.0"), 
                            std::wstring(L"c:\\Test2\\test2.dll"),
                            true);

    dt3 = new FdoCSTestData(std::wstring(L"TestData3"), 
                            std::wstring(L"TestData3"), 
                            std::wstring(L"TestDescription3"), 
                            std::wstring(L"3.0.3.0"), 
                            std::wstring(L"3.2.0.0"), 
                            std::wstring(L"c:\\Test3\\test.dll"),
                            true);

}

void ClientServicesTest::tearDown ()
{
    delete dt1;
    delete dt2;
    delete dt3;
}

void ClientServicesTest::TestServices ()
{
    try 
    {
        FdoPtr<IProviderRegistry> registry = FdoFeatureAccessManager::GetProviderRegistry();
        if (registry == NULL) {
            throw "NULL Registry Pointer";
        }

        const FdoProviderCollection* collection = registry->GetProviders();
        int initialCount = collection->GetCount();

        registry->RegisterProvider(dt1->m_name.c_str(), 
                                   dt1->m_displayName.c_str(), 
                                   dt1->m_description.c_str(), 
                                   dt1->m_version.c_str(), 
                                   dt1->m_fdoVersion.c_str(), 
                                   dt1->m_libraryPath.c_str(),
                                   dt1->m_isManaged);

        registry->RegisterProvider(dt2->m_name.c_str(), 
                                   dt2->m_displayName.c_str(), 
                                   dt2->m_description.c_str(), 
                                   dt2->m_version.c_str(), 
                                   dt2->m_fdoVersion.c_str(), 
                                   dt2->m_libraryPath.c_str(),
                                   dt2->m_isManaged);

        registry->RegisterProvider(dt3->m_name.c_str(), 
                                   dt3->m_displayName.c_str(), 
                                   dt3->m_description.c_str(), 
                                   dt3->m_version.c_str(), 
                                   dt3->m_fdoVersion.c_str(), 
                                   dt3->m_libraryPath.c_str(),
                                   dt3->m_isManaged);

        collection = registry->GetProviders();

        FdoPtr<FdoProvider> provider = collection->GetItem(collection->IndexOf(dt1->m_name.c_str()));
        if (provider == NULL) {
            throw "NULL Provider pointer";
        }

        if (dt1->m_name.compare(provider->GetName()) != 0) {
            throw "Wrong provider name returned from collection";
        }

        if (dt1->m_displayName.compare(provider->GetDisplayName()) != 0) {
            throw "Wrong provider display name returned from collection";
        }

        if (dt1->m_description.compare(provider->GetDescription()) != 0) {
            throw "Wrong provider description returned from collection";
        }

        if (dt1->m_version.compare(provider->GetVersion()) != 0) {
            throw "Wrong provider version returned from collection";
        }

        if (dt1->m_fdoVersion.compare(provider->GetFeatureDataObjectsVersion()) != 0) {
            throw "Wrong provider fdo version returned from collection";
        }

        if (dt1->m_libraryPath.compare(provider->GetLibraryPath()) != 0) {
            throw "Wrong provider library path returned from collection";
        }

        if (dt1->m_isManaged != provider->GetIsManaged()) {
            throw "Wrong provider managed state returned from collection";
        }

        FdoPtr<IConnectionManager> connectionMgr = FdoFeatureAccessManager::GetConnectionManager();
        if (connectionMgr == NULL) {
            throw "NULL ConnectionManager pointer";
        }

        // Note: the actual connection creation is 
        // tested by the SDK installation/build/execute.
        // A few error cases are tested here

        bool failed = false;
        try
        {
            FdoPtr<FdoIConnection> conn = connectionMgr->CreateConnection(L"OSGeo.NoProvider.1.0");
        }
        catch ( FdoException* ex ) 
        {
#ifdef _WIN32
            CPPUNIT_ASSERT( wcsstr(ex->GetExceptionMessage(), L"FDO Provider 'OSGeo.NoProvider.1.0' Not Registered") );
#endif
            FDO_SAFE_RELEASE(ex);
            failed = true;
        }
        CPPUNIT_ASSERT(failed);

        failed = false;
        try
        {
            FdoPtr<FdoIConnection> conn = connectionMgr->CreateConnection(
#ifdef _WIN32
                L"NoExist.dll"
#else
                L"NoExist.so"
#endif
            );
        }
        catch ( FdoException* ex ) 
        {
#ifdef _WIN32
            CPPUNIT_ASSERT( wcsstr(ex->GetExceptionMessage(), L"Unable to load the FDO Provider library 'NoExist.dll'") );
#endif
            FDO_SAFE_RELEASE(ex);
            failed = true;
        }
        CPPUNIT_ASSERT(failed);

        failed = false;
        try
        {
            FdoPtr<FdoIConnection> conn = connectionMgr->CreateConnection(
#ifdef _WIN32
                L"e:/dir1/dir2/NoExist.dll"
#else
                L"/user/local/NoExist.so.1.0"
#endif
            );
        }
        catch ( FdoException* ex ) 
        {
#ifdef _WIN32
            CPPUNIT_ASSERT( wcsstr(ex->GetExceptionMessage(), L"Unable to load the FDO Provider library 'e:/dir1/dir2/NoExist.dll'") );
#endif
            FDO_SAFE_RELEASE(ex);
            failed = true;
        }
        CPPUNIT_ASSERT(failed);

        failed = false;
        try
        {
            FdoPtr<FdoIConnection> conn = connectionMgr->CreateConnection(L"TestData2");
        }
        catch ( FdoException* ex ) 
        {
#ifdef _WIN32
            CPPUNIT_ASSERT( wcsstr(ex->GetExceptionMessage(), L"Unable to load the FDO Provider library 'c:\\Test2\\test2.dll'") );
#endif
            FDO_SAFE_RELEASE(ex);
            failed = true;
        }
        CPPUNIT_ASSERT(failed);

        registry->UnregisterProvider(dt2->m_name.c_str());
        registry->UnregisterProvider(dt3->m_name.c_str());
        registry->UnregisterProvider(dt1->m_name.c_str());
    }
    catch (FdoClientServiceException *cse)
    {
        std::wcerr << L"\n" << std::endl;
        std::wcerr << cse->GetExceptionMessage() << std::endl;
        char msg[1024];
#ifdef _WIN32
        WideCharToMultiByte (
            CP_UTF8,            // code page
            0,                  // performance and mapping flags
            cse->GetExceptionMessage(),              // wide-character string
            (int)wcslen (cse->GetExceptionMessage()) + 1,// number of chars in string
            msg,                // buffer for new string
            sizeof(msg),        // size of buffer
            NULL,               // default for unmappable chars
            NULL                // set when default char used
        );
#else
	wcstombs(msg, cse->GetExceptionMessage(), sizeof(msg));		
#endif
        delete cse;
        CPPUNIT_FAIL (msg);
    }
    catch (char* message)
    {
        std::cerr << "\n" << std::endl;
        std::cerr << message << std::endl;
        CPPUNIT_FAIL (message);
    }
    catch (...)
    {
        std::cerr << "\nUnhandled Exception" << std::endl;
        CPPUNIT_FAIL ("Unhandled Exception");
    }

    return;
}


