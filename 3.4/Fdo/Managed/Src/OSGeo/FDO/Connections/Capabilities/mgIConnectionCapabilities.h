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

#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"
#include "FDO\Commands\Locking\mgLockType.h"
#include "FDO\Connections\Capabilities\mgThreadCapability.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
public __value enum SpatialContextExtentType;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __value enum LockType;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \interface OSGeo::FDO::Connections::Capabilities::IConnectionCapabilities
/// \brief
/// The IConnectionCapabilities interface declares the feature provider's capabilities.
public __gc __interface IConnectionCapabilities : public System::IDisposable
{
public:
    /// \brief
    /// Gets an ThreadCapability value that declares the feature provider's level of thread safety.
    /// 
    /// \return
    /// Returns the connection thread capability.
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability get_ThreadCapability();

    /// \brief
    /// Gets the spatial context extent types supported by the feature provider.
    /// 
    /// \return
    /// Returns the list of spatial context extent types.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType get_SpatialContextTypes() [];

    /// \brief
    /// Determines if the feature provider supports persistent locking.
    /// 
    /// \return
    /// Returns true if the feature provider supports persistent locking.
    /// 
	System::Boolean SupportsLocking();

    /// \brief
    /// Gets an array of the LockType values supported by the feature provider.
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType get_LockTypes() [];

    /// \brief
    /// Determines if the feature provider supports connection timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports connection timeout.
    /// 
	System::Boolean SupportsTimeout();

    /// \brief
    /// Determines if the feature provider supports transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports transactions.
    /// 
	System::Boolean SupportsTransactions();

    /// \brief
    /// Determines true if the feature provider supports long transactions.
    /// 
    /// \return
    /// Returns true if the feature provider supports long transactions.
    /// 
	System::Boolean SupportsLongTransactions();

    /// \brief
    /// Determines if the feature provider supports SQL commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports SQL commands.
    /// 
	System::Boolean SupportsSQL();

    /// \brief
    /// Determines if the feature provider supports XML configuration.
    /// 
    /// \return
    /// Returns true if the feature provider supports the setting of a configuration.
    /// 
	System::Boolean SupportsConfiguration();

    /// \brief
    /// Determines if the provider supports multiple spatial contexts.
    /// 
    /// \return
    /// Returns true if the provider supports multiple spatial contexts.
    /// 
	System::Boolean SupportsMultipleSpatialContexts();

    /// \brief
    /// Determines if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    /// \return
    /// Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    /// when creating a new spatial context.
    /// 
    System::Boolean SupportsCSysWKTFromCSysName();

    /// \brief
    /// Determines if the provider can support the flush function. Flush is used to write any outstanding data
    /// to the datastore. This is mainly used by the file based providers to ensure that any cached data is writen to the file.
    ///
    /// \return
    /// Returns true if the provider or datastore can support the flush function.
    ///
    System::Boolean SupportsFlush();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


