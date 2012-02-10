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

#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ClientServiceException.h>

// Constructs a default instance of a FdoProviderCollection.
FdoProvider::FdoProvider(FdoString* name, 
                         FdoString* displayName, 
                         FdoString* description, 
                         FdoString* version, 
                         FdoString* fdoVersion,
                         FdoString* libraryPath,
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
void FdoProvider::Set(FdoString* name, 
                      FdoString* displayName, 
                      FdoString* description, 
                      FdoString* version, 
                      FdoString* fdoVersion,
                      FdoString* libraryPath,
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
FdoString* FdoProvider::GetName() const
{
    return (FdoString*)m_name;
}

// Gets a user friendly display name of the feature provider
FdoString* FdoProvider::GetDisplayName() const
{
    return (FdoString*)m_displayName;
}

// Gets a brief description of the feature provider
FdoString* FdoProvider::GetDescription() const
{
    return (FdoString*)m_description;
}

// Gets the version of the feature provider. The version number string has the form 
// <VersionMajor>.<VersionMinor>.<BuildMajor>.<BuildMinor>
FdoString* FdoProvider::GetVersion() const
{
    return (FdoString*)m_version;
}

// Gets the version of the feature data objects specification the feature provider conforms to. 
// The version number string has the form <VersionMajor>.<VersionMinor>.<BuildMajor>.<BuildMinor>
FdoString* FdoProvider::GetFeatureDataObjectsVersion() const
{
    return (FdoString*)m_fdoVersion;
}

// Gets the FULL library path + library name of the provider. 
FdoString* FdoProvider::GetLibraryPath() const
{
    return (FdoString*)m_libraryPath;
}

// Gets a boolean flag indicating if the provider is a managed or unmanaged provider.
bool FdoProvider::GetIsManaged() const
{
    return m_isManaged;
}

