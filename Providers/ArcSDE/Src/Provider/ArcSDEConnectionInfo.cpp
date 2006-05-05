/*
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
 */

#include "stdafx.h"

ArcSDEConnectionInfo::ArcSDEConnectionInfo (ArcSDEConnection* connection) :
    mConnection (connection)
{
    // NOTE: we use a weak reference here to avoid a circular dependancy:
    // FDO_SAFE_ADDREF(mConnection.p);
}

ArcSDEConnectionInfo::~ArcSDEConnectionInfo (void)
{
}

void ArcSDEConnectionInfo::Dispose()
{
    delete this;
}

void ArcSDEConnectionInfo::validate ()
{
    if (mConnection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* ArcSDEConnectionInfo::GetProviderName ()
{
    validate ();
    return (ARCSDE_PROVIDER_NAME);
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* ArcSDEConnectionInfo::GetProviderDisplayName()
{
    validate ();
    return NlsMsgGet(ARCSDE_PROVIDER_DISPLAY_NAME, ARCSDE_PROVIDER_DEFAULT_DISPLAY_NAME);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* ArcSDEConnectionInfo::GetProviderDescription ()
{
    validate ();
    return NlsMsgGet(ARCSDE_PROVIDER_DESCRIPTION, ARCSDE_PROVIDER_DEFAULT_DESCRIPTION);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* ArcSDEConnectionInfo::GetProviderVersion ()
{
    validate ();
    return (ARCSDE_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* ArcSDEConnectionInfo::GetFeatureDataObjectsVersion ()
{
    validate ();
    return (ARCSDE_FDO_VERSION);
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* ArcSDEConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
        mPropertyDictionary = new ArcSDEConnectionPropertyDictionary (mConnection);
    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

