//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_CONNECTIONINFO_H_INCLUDED
#define FDOPOSTGIS_CONNECTIONINFO_H_INCLUDED

#include <PostGIS/FdoPostGisProvider.h>

//
// Forward declarations
//
class FdoStringCollection;
class FdoIConnectionPropertyDictionary;

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of interface providing access to information
/// about the feature provider and connection properties for PostGIS provider.
///
class ConnectionInfo : public FdoIConnectionInfo
{
public:

    /// Default constructor.
    ConnectionInfo();

    ConnectionInfo(Connection* conn);

    //
    // FdoIConnectionInfo interface
    //

    /// Get name of the feature provider - PostGIS Provider.
    FDOPOSTGIS_API FdoString* GetProviderName();

    /// Get display name of the feature provider.
    FDOPOSTGIS_API FdoString* GetProviderDisplayName();

    /// Get description of the feature provider.
    FDOPOSTGIS_API FdoString* GetProviderDescription();
 	
    /// Get version of the feature provider.
    FDOPOSTGIS_API FdoString* GetProviderVersion();
 	
    /// Get version of the Feature Data Objects specification with which 
    /// this PostGIS provider is compatible.
    FDOPOSTGIS_API FdoString* GetFeatureDataObjectsVersion();
 	
    /// Get pointer to FdoIConnectionPropertyDictionary interface that can be used to
    /// dynamically query and set properties required to establish a connection.
    FDOPOSTGIS_API FdoIConnectionPropertyDictionary* GetConnectionProperties();
 	
    /// Returns the provider type.
    /// \return Value of FdoProviderDatastoreType_DatabaseServer enumerator.
    FdoProviderDatastoreType GetProviderDatastoreType();
 	
    /// This is file-based providers specific function.
    /// \return For the PostGIS provider it always returns NULL.
    FdoStringCollection* GetDependentFileNames();

protected:

    /// Destructor.
    virtual ~ConnectionInfo();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
    // Weak reference to connection object of current session.
    Connection* mConn;

    // Dictionary of connection properties.
    // Created upon first request in GetConnectionProperties() function.
    FdoPtr<FdoCommonConnPropDictionary> mPropertyDict;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTIONINFO_H_INCLUDED
