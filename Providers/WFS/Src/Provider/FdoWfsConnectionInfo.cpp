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

#include "stdafx.h"
#include <FdoCommonStringUtil.h>

FdoWfsConnectionInfo::FdoWfsConnectionInfo (FdoWfsConnection* connection) :
    mConnection (connection)
{
    // NOTE: we use a weak reference here to avoid a circular dependancy:
    // FDO_SAFE_ADDREF(mConnection.p);
}

FdoWfsConnectionInfo::~FdoWfsConnectionInfo (void)
{
}

void FdoWfsConnectionInfo::Dispose()
{
    delete this;
}

void FdoWfsConnectionInfo::validate ()
{
    if (mConnection == NULL)
        throw FdoException::Create (NlsMsgGet (WFS_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* FdoWfsConnectionInfo::GetProviderName ()
{
    validate ();
    return (WFS_PROVIDER_NAME);
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* FdoWfsConnectionInfo::GetProviderDisplayName()
{
    validate ();
    return NlsMsgGet(WFS_PROVIDER_DISPLAY_NAME, WFS_PROVIDER_DEFAULT_DISPLAY_NAME);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* FdoWfsConnectionInfo::GetProviderDescription ()
{
    validate ();
    return NlsMsgGet(WFS_PROVIDER_DESCRIPTION, WFS_PROVIDER_DEFAULT_DESCRIPTION);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* FdoWfsConnectionInfo::GetProviderVersion ()
{
    validate ();
    return (WFS_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* FdoWfsConnectionInfo::GetFeatureDataObjectsVersion ()
{
    validate ();
    return (WFS_FDO_VERSION);
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* FdoWfsConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary (mConnection);

        char* mbServerName = NULL;
        wide_to_multibyte (mbServerName, FdoWfsGlobals::FeatureServer);
        FdoPtr<ConnectionProperty> featureServerProperty = new ConnectionProperty (
                        FdoWfsGlobals::FeatureServer,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_FEATURESRVER, mbServerName),
                        L"", 
                        true, 
                        false, 
                        false,
                        false,
                        false,
                        false,
                        0, 
                        NULL);
        mPropertyDictionary->AddProperty(featureServerProperty);

        char* mbUserName = NULL;
        wide_to_multibyte (mbUserName, FdoWfsGlobals::Username);
        FdoPtr<ConnectionProperty> usernameProperty = new ConnectionProperty (
                        FdoWfsGlobals::Username,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_USERNAME, mbUserName),
                        L"", 
                        false, 
                        false, 
                        false, 
                        false,
                        false,
                        false,
                        0, 
                        NULL);
        mPropertyDictionary->AddProperty(usernameProperty);

        char* mbPasword = NULL;
        wide_to_multibyte (mbPasword, FdoWfsGlobals::Password);
        FdoPtr<ConnectionProperty> passwordProperty = new ConnectionProperty (
                        FdoWfsGlobals::Password,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PASSWORD, mbPasword),
                        L"", 
                        false, 
                        false, 
                        false, 
                        false,
                        false,
                        false,
                        0, 
                        NULL);
        mPropertyDictionary->AddProperty    (passwordProperty);
    }

    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}
