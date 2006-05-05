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

#ifndef FDORDBMSODBCCONNECTIONCAPABILITIES_H
#define FDORDBMSODBCCONNECTIONCAPABILITIES_H        1

#ifdef _WIN32
#pragma once
#endif


class FdoRdbmsOdbcConnectionCapabilities : public FdoIConnectionCapabilities
{
public:
    // Constructor
    FdoRdbmsOdbcConnectionCapabilities(void);

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

    // Returns true if the feature provider supports the setting of a configuration.
    virtual bool SupportsConfiguration();

    // Returns true if the provider supports multiple spatial contexts.
    virtual bool SupportsMultipleSpatialContexts();

    // Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    // when creating a new spatial context.
    virtual bool SupportsCSysWKTFromCSysName();

protected:
    ~FdoRdbmsOdbcConnectionCapabilities(void);
    virtual void Dispose();
};

#endif // FDORDBMSODBCCONNECTIONCAPABILITIES_H
