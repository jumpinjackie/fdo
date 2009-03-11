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

#pragma once

#include "FDO\Connections\Capabilities\mgIConnectionCapabilities.h"

class FdoIConnectionCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

enum class ThreadCapability;

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
/// The IConnectionCapabilities interface declares the feature provider's capabilities.
private ref class IConnectionCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                                public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities
{
public:
    /// \brief
    /// Gets an ThreadCapability value that declares the feature provider's level of thread safety.
    /// 
    /// \return
    /// Returns the connection thread capability.
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability ThreadCapability
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability get();
    }

    /// \brief
    /// Gets the spatial context extent types supported by the feature provider.
    /// 
    /// \return
    /// Returns the list of spatial context extent types.
    /// 
    virtual property array<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>^ SpatialContextTypes
    {
        array<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>^ get();
    }

    /// \brief
    /// Determines if the feature provider supports persistent locking.
    /// 
    /// \return
    /// Returns true if the feature provider supports persistent locking.
    /// 
	virtual System::Boolean SupportsLocking();

    /// \brief
    /// Gets an array of the LockType values supported by the feature provider.
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
    virtual property array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ LockTypes
    {
        array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ get();
    }

    /// \brief
    /// Determines if the feature provider supports connection timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports connection timeout.
    /// 
	virtual System::Boolean SupportsTimeout();

    /// \brief
    /// Determines if the feature provider supports transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports transactions.
    /// 
	virtual System::Boolean SupportsTransactions();

    /// \brief
    /// Determines true if the feature provider supports long transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports long transactions.
    /// 
	virtual System::Boolean SupportsLongTransactions();

    /// \brief
    /// Determines if the feature provider supports SQL commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports SQL commands.
    /// 
	virtual System::Boolean SupportsSQL();

    /// \brief
    /// Determines if the feature provider supports XML configuration.
    /// 
    /// \return
    /// Returns true if the feature provider supports the setting of a configuration.
    /// 
	virtual System::Boolean SupportsConfiguration();

    /// \brief
    /// Determines if the provider supports multiple spatial contexts.
    /// 
    /// \return
    /// Returns true if the provider supports multiple spatial contexts.
    /// 
	virtual System::Boolean SupportsMultipleSpatialContexts();

    /// \brief
    /// Determines if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    /// \return
    /// Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    virtual System::Boolean SupportsCSysWKTFromCSysName();

    /// \brief
    /// Determines if the provider can support the flush function. Flush is used to write any outstanding data
    /// to the datastore. This is mainly used by the file based providers to ensure that any cached data is writen to the file.
    ///
    /// \return
    /// Returns true if the provider or datastore can support the flush function.
    ///
    virtual System::Boolean SupportsFlush();

internal:
	IConnectionCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIConnectionCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


