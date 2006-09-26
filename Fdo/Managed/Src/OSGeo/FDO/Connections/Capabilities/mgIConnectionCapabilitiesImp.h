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

public __value enum ThreadCapability;

/// \brief
/// The FdoIConnectionCapabilities interface declares the feature provider's capabilities.
private __gc class IConnectionCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities
{
public:
    /// \brief
    /// Gets an FdoThreadCapability value that declares the feature provider's level of thread safety.
    /// 
    /// \return
    /// Returns the connection thread capability.
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability get_ThreadCapability();

    /// \brief
    /// Gets the spatial context extent types supported by the feature provider.
    /// 
    /// \param length 
    /// Output the number of spatial context types.
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
    /// Gets an array of the FdoLockType values supported by the feature provider.
    /// 
    /// \param size 
    /// Output the number of lock types
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

public private:
	IConnectionCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIConnectionCapabilities* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


