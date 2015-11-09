//
// Copyright (C) 2006 Refractions Research, Inc. 
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
#ifndef FDORDBMSPOSTGISCONNECTIONINFO_H
#define FDORDBMSPOSTGISCONNECTIONINFO_H

#include <Fdo.h>
#include <FdoRdbmsConnectionPropertyDictionary.h>

/// \brief
/// Provides information about datastore access for PostGIS provider.
/// 
///
class FdoRdbmsPostGisConnectionInfo : public FdoIConnectionInfo
{
public:

    /// Constructor
    /// @param connection [in] - 
    FdoRdbmsPostGisConnectionInfo(FdoRdbmsConnection* connection);

    /// Gets the name of the PostGIS feature provider.
    virtual const wchar_t* GetProviderName();

    /// Gets the display name of the PostGIS feature provider.
    virtual const wchar_t* GetProviderDisplayName();

    /// Gets the description of the PostGIS feature provider.
    virtual const wchar_t* GetProviderDescription();

    /// Gets the version of the PostGIS feature provider.
    virtual const wchar_t* GetProviderVersion();

    /// Gets the version of the FDO specification to which
    /// PostGIS provider conforms.
    virtual const wchar_t* GetFeatureDataObjectsVersion();

    /// Gets the interface that can be used to query and
    /// set the properties required to establish a connection.
    /// Provides, lazy construction of properties collection., on request.
    ///
    /// Following properties are supported by PostGIS provider:
    ///
    ///
    virtual FdoIConnectionPropertyDictionary* GetConnectionProperties();

    /// \tood To be documented.
    virtual FdoProviderDatastoreType GetProviderDatastoreType();

    /// \tood To be documented.
    virtual FdoStringCollection* GetDependentFileNames();

public:

    FdoPtr<FdoRdbmsConnectionPropertyDictionary> mPropertyDictionary;

    FdoRdbmsConnection* mConnection;

protected:

    /// Destructor.
    ~FdoRdbmsPostGisConnectionInfo();

    virtual void Dispose();
};

#endif // FDORDBMSPOSTGISCONNECTIONINFO_H
