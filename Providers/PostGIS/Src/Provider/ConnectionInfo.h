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


namespace fdo { namespace postgis {


/// Implementation of interface providing access to information
/// about the feature provider and connection properties for PostGIS provider.
///
class ConnectionInfo : public FdoIConnectionInfo
{
public:

    /// Default constructor.
    ConnectionInfo();

    /// Destructor.
    virtual ~ConnectionInfo();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

    //
    // FdoIConnectionInfo interface
    //

    /// Get name of the feature provider - PostGIS Provider.
    FdoString* GetProviderName();

    /// Get display name of the feature provider.
    FdoString* GetProviderDisplayName();

    /// Get description of the feature provider.
    FdoString* GetProviderDescription();
 	
    /// Get version of the feature provider.
    FdoString* GetProviderVersion();
 	
    /// Get version of the Feature Data Objects specification with which 
    /// this PostGIS provider is compatible.
    FdoString* GetFeatureDataObjectsVersion();
 	
    /// Get pointer to FdoIConnectionPropertyDictionary interface that can be used to
    /// dynamically query and set properties required to establish a connection.
    FdoIConnectionPropertyDictionary* GetConnectionProperties();
 	
    /// Returns the provider type.
    FdoProviderDatastoreType GetProviderDatastoreType();
 	
    /// File-based providers depend on a various files.
    FdoStringCollection* GetDependentFileNames();

protected:

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTIONINFO_H_INCLUDED
