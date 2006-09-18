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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnectionCapabilities.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

 #ifndef FDORFPCONNECTIONCAPABILITIES_H
 #define FDORFPCONNECTIONCAPABILITIES_H

#ifdef _WIN32
#pragma once
#endif //_WIN32


class FdoRfpConnectionCapabilities : public FdoIConnectionCapabilities
{
public:
	// Constructor
	FdoRfpConnectionCapabilities(void);

	// Get the FDO provider thread capability
    virtual FdoThreadCapability GetThreadCapability();

    // Gets the spatial context extent types supported by the feature provider.
	virtual FdoSpatialContextExtentType* GetSpatialContextTypes(int& length);

    // Returns true if the feature provider supports feature locking.
    virtual bool SupportsLocking();

    // Returns the list of lock types
    virtual FdoLockType* GetLockTypes(FdoInt32& size);

    // Returns true if the feature provider supports connection timeout.
    virtual bool SupportsTimeout();

    // Returns true if the feature provider supports transactions.
    virtual bool SupportsTransactions();

    // Returns true if the feature provider supports long transactions.
    virtual bool SupportsLongTransactions();

    // Returns true if the feature provider supports SQL commands.
    virtual bool SupportsSQL();

    /// Determines if the feature provider supports XML configuration.
    /// Returns true if the feature provider supports the setting of a configuration.
    virtual bool SupportsConfiguration();

    /// Determines if the provider supports multiple spatial contexts.
    /// Returns true if the provider supports multiple spatial contexts.
    virtual bool SupportsMultipleSpatialContexts();

    /// Determines if the provider supports specifying the coordinate system by name 
    /// or ID without specifying the WKT when creating a new spatial context.
    /// Returns true if the provider supports specifying the coordinate system by name 
    /// or ID without specifying the WKT when creating a new spatial context.
    virtual bool SupportsCSysWKTFromCSysName();

    // Determines if write is supported by the provider or by the datastore depending on whether this request is at
    // the provider or datastore level.
    virtual bool SupportsWrite();

    // Determines if the provider or datastore can support more than one user writing to a single datastore at
    // one time.
    virtual bool SupportsMultiUserWrite();

protected:
	~FdoRfpConnectionCapabilities(void);
	virtual void Dispose();
};

 #endif //FDORFPCONNECTIONCAPABILITIES_H
