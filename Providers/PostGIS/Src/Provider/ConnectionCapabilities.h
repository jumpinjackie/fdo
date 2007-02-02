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
// Lesser General Public License for more/// Details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_CONNECTIONCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_CONNECTIONCAPABILITIES_H_INCLUDED


namespace fdo { namespace postgis {


/// Implementation of interface describing connection capabilities
/// supported by PostGIS provider.
///
class ConnectionCapabilities : public FdoIConnectionCapabilities
{
public:

    /// Default constructor.
    ConnectionCapabilities();

    //
    // FdoIConnectionCapabilities interface
    //

    /// Get FdoThreadCapability enumerator that declares level of thread safety
    /// supported by PostGIS provider.
    FdoThreadCapability GetThreadCapability();

    /// Get spatial context extent types supported by the PostGIS provider.
    FdoSpatialContextExtentType* GetpatialContextTypes(FdoInt32& size);

    /// Get array of the FdoLockType values supported by the feature provider.
    FdoLockType* GetLockTypes(FdoInt32& size);

    /// Determine if the PostGIS provider supports persistent locking.
    bool SupportsLocking();

    /// Determine if the PostGIS provider supports connection timeout.
    bool SupportsTimeout();

    /// Determine if the PostGIS provider supports transactions.
    bool SupportsTransactions();

    /// Determine if the PostGIS provider supports long transactions.
    bool SupportsLongTransactions();

    /// Determine if the PostGIS provider supports SQL commands.
    bool SupportsSQL();

    /// Determine if the PostGIS provider supports XML configuration.
    bool SupportsConfiguration();

    /// Determine if the PostGIS provider supports multiple spatial contexts.
    bool SupportsMultipleSpatialContexts();

    /// Determine if the PostGIS provider supports coordinate system specification by
    /// name or ID without specifying WKT, during new spatial context creation.
    bool SupportsCSysWKTFromCSysName();

    /// Determine if write operation is supported.
    /// Write operation support can be provided by the provider or by datastore,
    /// depending on whether this request is at the provider or datastore level.
    bool SupportsWrite();

    /// Determine if the provider or datastore supports more than one writing
    /// actions to a single datastore at once.
    bool SupportsMultiUserWrite();

    /// Determine if the provider supports the flush function. 
    bool SupportsFlush();

protected:

    /// Destructor.
    virtual ~ConnectionCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTIONCAPABILITIES_H_INCLUDED
