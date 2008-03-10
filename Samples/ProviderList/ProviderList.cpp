// 
// Copyright (C) 2008  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 

#include "stdafx.h"

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif

{
    IProviderRegistry* registry;
    const FdoProviderCollection* providers;
    const FdoProvider* provider;
    int ret;

    ret = 1;
    
    try
    {
        registry = FdoFeatureAccessManager::GetProviderRegistry ();
        providers = registry->GetProviders ();
        for (int i = 0; i < providers->GetCount (); i++)
        {
            provider = providers->GetItem (i);
            wprintf (L"%ls:\n", provider->GetName ());
            wprintf (L"    Display Name: %ls\n", provider->GetDisplayName ());
            wprintf (L"    Version: %ls\n", provider->GetVersion ());
            wprintf (L"    Description: %ls\n", provider->GetDescription ());
            wprintf (L"    FDO Version: %ls\n", provider->GetFeatureDataObjectsVersion ());
            wprintf (L"    Path: %ls\n", provider->GetLibraryPath ());
            wprintf (L"    Managed: %ls\n", provider->GetIsManaged () ? L"true" : L"false");
            wprintf (L"\n");
            ret = 0;
        }
    }
    catch (FdoClientServiceException* exception)
    {
        wprintf (L"%ls\n", exception->GetExceptionMessage ());
        delete exception;
    }

    return (ret);
}


