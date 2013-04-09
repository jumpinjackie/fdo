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
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary (mConnection);

        char* mbPropName = NULL;
        wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_SERVER);
        FdoPtr<ConnectionProperty> newProp = new ConnectionProperty (CONNECTIONPROPERTY_SERVER,
            NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_SERVER, mbPropName),
            L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty (newProp);

        wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_INSTANCE);
        newProp = new ConnectionProperty (CONNECTIONPROPERTY_INSTANCE,
            NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_INSTANCE, mbPropName),
            L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty (newProp);

        wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_USERNAME);
        newProp = new ConnectionProperty (CONNECTIONPROPERTY_USERNAME,
            NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_USERNAME, mbPropName),
            L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty (newProp);

        wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_PASSWORD);
        newProp = new ConnectionProperty (CONNECTIONPROPERTY_PASSWORD,
            NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_PASSWORD, mbPropName),
            L"", true, true, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty (newProp);

        wide_to_multibyte(mbPropName, CONNECTIONPROPERTY_DATASTORE);
        newProp = new ConnectionProperty (CONNECTIONPROPERTY_DATASTORE,
            NlsMsgGet(ARCSDE_CONNECTION_PROPERTY_DATASTORE, mbPropName),
            L"", false, false, true, false, false, true, false, 0, NULL);
        mPropertyDictionary->AddProperty (newProp);
    }
    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

/// <summary>Returns the provider type. A provider can be a file-based, database-based or
/// web-based provider. The valid values the function may return are defined in
/// the enumeration FdoProviderDatastoreType. The enumeration includes the following
/// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
/// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.</summary>
/// <returns>Returns the provider data store type.</returns>
FdoProviderDatastoreType ArcSDEConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_DatabaseServer;
}

/// <summary>File-based providers depend on a various files. This function returns a list
/// of fully qualified dependend file names. The return parameter will be NULL if
/// the provider is not a file-based provider.</summary>
/// <returns>Returns the list of fully-qualified dependend file names if the provider is a
/// file-based provider, NULL otherwise.</returns>
FdoStringCollection* ArcSDEConnectionInfo::GetDependentFileNames()
{
    return NULL;
}

