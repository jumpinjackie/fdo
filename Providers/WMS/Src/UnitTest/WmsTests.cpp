/*
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
 */

#include "Pch.h"
#include "WmsTests.h"

WmsTests::WmsTests(void)
{
}

WmsTests::~WmsTests(void)
{
}

void WmsTests::setUp ()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    mConnection = manager->CreateConnection (L"OSGeo.WMS");
}

void WmsTests::tearDown ()
{
    mConnection = NULL;
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    manager->FreeLibrary(L"OSGeo.WMS");
}

FdoIConnection* WmsTests::GetConnection ()
{
    return FDO_SAFE_ADDREF(mConnection.p);
}

void WmsTests::fail (FdoException* ge)
{
    wchar_t message[2000];
    char* multibyte;
	FdoPtr<FdoException> cause;

    wcscpy(message, (wchar_t*)ge->GetExceptionMessage ());
	cause = ge->GetCause();
	
    while (cause != NULL)
	{
		wcscat(message, L" [");
		wcscat(message, cause->GetExceptionMessage());
		wcscat(message, L"]");
		cause = cause->GetCause();
	}
    
    w2mb (multibyte, message);
    if (NULL == multibyte)
        multibyte = "Exception message could not be converted.";

    ge->Release ();
    CPPUNIT_FAIL (multibyte);
}

void WmsTests::fail (char* error)
{
    char message[1024];
    sprintf (message, "\nFAILURE: %s\n", error);
    CPPUNIT_FAIL (message);
}

