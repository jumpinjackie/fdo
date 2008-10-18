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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnectionInfo.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpConnectionInfo.h"
#include "FdoRfpGlobals.h"
#include <FdoCommonStringUtil.h>

FdoRfpConnectionInfo::FdoRfpConnectionInfo(FdoRfpConnection* connection) : m_connection(connection)
{
}

FdoRfpConnectionInfo::~FdoRfpConnectionInfo(void)
{
}

void FdoRfpConnectionInfo::Dispose ()
{
    delete this;
}

void FdoRfpConnectionInfo::validate ()
{
    if (m_connection == NULL)
        throw FdoException::Create(NlsMsgGet(GRFP_37_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* FdoRfpConnectionInfo::GetProviderName ()
{
    return FdoGrfpGlobals::GRFPProviderName;
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* FdoRfpConnectionInfo::GetProviderDescription ()
{
    return FdoGrfpGlobals::GRFPProviderDescription;
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* FdoRfpConnectionInfo::GetProviderVersion ()
{
    return FdoGrfpGlobals::GRFPProviderVersion;
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* FdoRfpConnectionInfo::GetFeatureDataObjectsVersion ()
{
    return FdoGrfpGlobals::GRFPFeatureDataObjectsVersion;
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name.</returns>
FdoString* FdoRfpConnectionInfo::GetProviderDisplayName()
{
    return FdoGrfpGlobals::GRFPProviderDisplayName;
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* FdoRfpConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary ((FdoIConnection*)m_connection);

        char* mbServerName = NULL;
        wide_to_multibyte(mbServerName, FdoGrfpGlobals::DefaultRasterFileLocation);
        FdoPtr<ConnectionProperty> fileLocationProperty = new ConnectionProperty (
                        FdoGrfpGlobals::DefaultRasterFileLocation,
                        NlsMsgGet(GRFP_70_DEFAULT_RASTER_FILE_LOCATION, mbServerName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(fileLocationProperty);
    }

    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

/// <summary>Returns the provider type. A provider can be a file-based, database-based or
/// web-based provider. The valid values the function may return are defined in
/// the enumeration FdoProviderDatastoreType. The enumeration includes the following
/// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
/// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.</summary>
/// <returns>Returns the provider data store type.</returns>
FdoProviderDatastoreType FdoRfpConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_File;
}

/// <summary>File-based providers depend on a various files. This function returns a list
/// of fully qualified dependend file names. The return parameter will be NULL if
/// the provider is not a file-based provider or the routine is invoked before a
/// connection is established.</summary>
/// <returns>Returns the list of fully-qualified dependend file names if the provider is a
/// file-based provider, NULL otherwise.</returns>
FdoStringCollection* FdoRfpConnectionInfo::GetDependentFileNames()
{
    return NULL;
}
