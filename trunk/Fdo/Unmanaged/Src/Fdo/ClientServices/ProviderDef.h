#ifndef _FDOPROVIDERDEF_H_
#define _FDOPROVIDERDEF_H_
/***************************************************************************
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
 ***************************************************************************/

#ifdef _WIN32
#pragma once
#endif

// Include std::string definition
#include <string>

// A slash 
const std::wstring c_slash = L"\\";

// Handle to sub key in the windows registry where the Autodesk keys reside 
const std::wstring c_AutodeskKey = L"SOFTWARE\\Autodesk";

// Handle to sub key in the windows registry relavive to c_AutodeskKey where the providers are registered 
const std::wstring c_ProvidersKey = L"FDO\\R1.0\\Providers";

// Name of the String value in the registry key that holds the name of the provider 
const std::wstring c_keyNameValue = L"Name";

// Name of the String value in the registry key that holds the display name of the provider 
const std::wstring c_keyDisplayNameValue = L"DisplayName";

// Name of the String value in the registry key that holds the library location of the provider 
const std::wstring c_keyDescriptionValue = L"Description";

// Name of the String value in the registry key that holds the version of the provider 
const std::wstring c_keyVersionValue = L"Version";

// Name of the String value in the registry key that holds the FDO version of the provider 
const std::wstring c_keyFDOVersionValue = L"FeatureDataObjectsVersion";

// Name of the String value in the registry key that holds the library location of the provider 
const std::wstring c_keyLibraryValue = L"LibraryPath";

// Name of the DWORD value in the registry key that indicates if the provider in Managed/Unmanaged 
const std::wstring c_keyIsManagedValue = L"Managed";

// Name of the client services binary 
const std::wstring c_clientServiceLibraryName = L"FdoClientServices.dll";

// Client services undefined error text
const std::wstring c_undefinedError = L"Undefined Error";

// Provider DLL entry point function name
const std::string c_providerEntryPoint = "CreateConnection";

const std::string c_featureProviderRegistryName = "FeatureProviderRegistry";
const std::string c_featureProviderName = "FeatureProvider";
const std::wstring c_featureProviderNameW = L"FeatureProvider";
#endif

