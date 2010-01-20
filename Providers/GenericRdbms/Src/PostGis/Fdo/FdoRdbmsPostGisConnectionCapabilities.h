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
#ifndef FDORDBMSPOSTGISCONNECTIONCAPABILITIES_H
#define FDORDBMSPOSTGISCONNECTIONCAPABILITIES_H

#include <Fdo.h>
#include "../../Fdo/Capability/FdoRdbmsConnectionCapabilities.h"


/// \brief
/// Implementation of interface to query PostGIS provider capabilities.
/// 
/// \sa FdoIConnectionCapabilities - declaration of interface
/// implemented by following class.
///
class FdoRdbmsPostGisConnectionCapabilities :
    public FdoRdbmsConnectionCapabilities
{
public:

    /// Default constructor.
    FdoRdbmsPostGisConnectionCapabilities();

    /// Checks if PostGIS provider supports feature locking.
    /// 
    /// @return true if feature locking is supported, false otherwise.
    ///
    virtual bool SupportsLocking();

    /// Gets an array of the FdoLockType values supported by
    /// the feature provider.
    ///
    /// @param size [out] - number of lock types.
    /// @return List of lock types and number of list elements.
    ///
    virtual FdoLockType* GetLockTypes(FdoInt32& size);

    /// Checks if PostGis providers supports Long Transactions.
    /// 
    /// @return true if Long Transactions are supported, false otherwise.
    ///
    virtual bool SupportsLongTransactions();

    /// Checks if PostGIS provider supports coordinate
    /// system specificaction by name or ID without specifying the WKT when
    /// creating a new spatial context.
    /// 
    /// @return true if CS specification by name or ID is supported,
    /// false otherwise.
    ///
    virtual bool SupportsCSysWKTFromCSysName();

protected:

    /// Destructor.
    ~FdoRdbmsPostGisConnectionCapabilities();

    /// Named destructor.
    virtual void Dispose();
};
#endif // FDORDBMSPOSTGISCONNECTIONCAPABILITIES_H
