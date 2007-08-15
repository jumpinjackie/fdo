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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ClientServiceException.h>

// Constructs a default instance of a FdoProviderCollection.
FdoProvider::FdoProvider(const wchar_t* name, 
                         const wchar_t* displayName, 
                         const wchar_t* description, 
                         const wchar_t* version, 
                         const wchar_t* fdoVersion,
                         const wchar_t* libraryPath,
                         bool isManaged)
{
    Set(name, displayName, description, version, fdoVersion, libraryPath, isManaged);
}

// Default destructor for FdoProviderCollection.
FdoProvider::~FdoProvider()
{
}

void FdoProvider::Dispose()
{
    delete this;
}

// Constructs a default instance of a FdoProviderCollection.
void FdoProvider::Set(const wchar_t* name, 
                      const wchar_t* displayName, 
                      const wchar_t* description, 
                      const wchar_t* version, 
                      const wchar_t* fdoVersion,
                      const wchar_t* libraryPath,
                      bool isManaged)
{
    if (name == NULL || displayName == NULL || description == NULL || version == NULL || fdoVersion == NULL || libraryPath == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    m_name = name;
    m_displayName = displayName;
    m_description = description;
    m_version = version;
    m_fdoVersion = fdoVersion;
    m_libraryPath = libraryPath;
    m_isManaged = isManaged;
}

// Gets the unique name of the feature provider. This name should be of the form 
// <Company>.<Provider>.<Version>
const wchar_t* FdoProvider::GetName() const
{
    return m_name.c_str();
}

// Gets a user friendly display name of the feature provider
const wchar_t* FdoProvider::GetDisplayName() const
{
    return m_displayName.c_str();
}

// Gets a brief description of the feature provider
const wchar_t* FdoProvider::GetDescription() const
{
    return m_description.c_str();
}

// Gets the version of the feature provider. The version number string has the form 
// <VersionMajor>.<VersionMinor>.<BuildMajor>.<BuildMinor>
const wchar_t* FdoProvider::GetVersion() const
{
    return m_version.c_str();
}

// Gets the version of the feature data objects specification the feature provider conforms to. 
// The version number string has the form <VersionMajor>.<VersionMinor>.<BuildMajor>.<BuildMinor>
const wchar_t* FdoProvider::GetFeatureDataObjectsVersion() const
{
    return m_fdoVersion.c_str();
}

// Gets the FULL library path + library name of the provider. 
const wchar_t* FdoProvider::GetLibraryPath() const
{
    return m_libraryPath.c_str();
}

// Gets a boolean flag indicating if the provider is a managed or unmanaged provider.
bool FdoProvider::GetIsManaged() const
{
    return m_isManaged;
}

