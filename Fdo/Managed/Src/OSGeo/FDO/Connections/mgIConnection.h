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

#include "FDO\Connections\mgConnectionState.h"
#include "FDO\Commands\mgCommandType.h"

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
public __gc __interface IConnectionCapabilities;
public __gc __interface ISchemaCapabilities;
public __gc __interface ICommandCapabilities;
public __gc __interface IFilterCapabilities;
public __gc __interface IExpressionCapabilities;
public __gc __interface IRasterCapabilities;
public __gc __interface ITopologyCapabilities;
public __gc __interface IGeometryCapabilities;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc __interface ICommand;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;
END_NAMESPACE_OSGEO_COMMON_IO

using namespace NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES;
using namespace NAMESPACE_OSGEO_FDO_COMMANDS;
using namespace NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
public __gc __interface IConnectionInfo;
public __gc __interface ITransaction;

/// \brief
/// The FdoIConnection interface enables a feature provider to implement a
/// connection class, which represents a unique session with a data
/// store.
public __gc __interface IConnection : public System::IDisposable
{
public:
    /// \brief
    /// Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.
    /// 
    /// \return
    /// Returns the connection capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities* get_ConnectionCapabilities();

    /// \brief
    /// Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.
    /// 
    /// \return
    /// Returns schema capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities* get_SchemaCapabilities();

    /// \brief
    /// Gets an FdoICommandCapabilities interface describing the commands a provider supports.
    /// 
    /// \return
    /// Returns the command capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities* get_CommandCapabilities();

    /// \brief
    /// Gets an FdoIFilterCapabilities interface describing the provider's support for filters.
    /// 
    /// \return
    /// Returns the filter capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities* get_FilterCapabilities();

    /// \brief
    /// Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.
    /// 
    /// \return
    /// Returns the expression capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities* get_ExpressionCapabilities();

    /// \brief
    /// Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.
    /// 
    /// \return
    /// Returns the raster capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities* get_RasterCapabilities();

    /// \brief
    /// Gets an FdoITopologyCapabilities interface describing the provider's support for topology.
    /// 
    /// \return
    /// Returns the topology capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities* get_TopologyCapabilities();

    /// \brief
    /// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.
    /// 
    /// \return
    /// Returns the geometry capabilities
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities* get_GeometryCapabilities();

    /// \brief
    /// Gets the connection string used to open a DataStore.
    /// 
    /// \return
    /// Returns the connection string
    /// 
	__property  System::String* get_ConnectionString();

    /// \brief
    /// Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.
    /// 
    /// \param value 
    /// Input the connection string
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ConnectionString( System::String* value);

    /// \brief
    /// Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.
    /// 
    /// \return
    /// Returns the connection info
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo* get_ConnectionInfo();

    /// \brief
    /// Gets the current state of the connection.
    /// 
    /// \return
    /// Returns the current state of the connection
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState get_ConnectionState();

    /// \brief
    /// Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned
    /// 
    /// \return
    /// Returns the time to wait (in milliseconds)
    /// 
	__property System::Int32 get_ConnectionTimeout();

    /// \brief
    /// Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception
    /// 
    /// \param value 
    /// Input the time to wait (in milliseconds)
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ConnectionTimeout(System::Int32 value);

    /// \brief
    /// Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.
    /// 
    /// \return
    /// Returns nothing
    /// 
	NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState Open();

    /// \brief
    /// Closes the connection to the DataStore
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

    /// \brief
    /// Begins a transaction and returns an object that realizes
    /// FdoITransaction.
    /// 
    /// \return
    /// Returns the transaction
    /// 
	NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction* BeginTransaction();

    /// \brief
    /// Creates and returns the specified type of command object associated with
    /// the connection.
    /// 
    /// \param commandType 
    /// Input the command type to be created
    /// 
    /// \return
    /// Returns the command
    /// 
    NAMESPACE_OSGEO_FDO_COMMANDS::ICommand* CreateCommand(NAMESPACE_OSGEO_FDO_COMMANDS::CommandType commandType);

    /// \brief
    /// Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.
    /// 
    /// \return
    /// Returns FdoPhysicalSchemaMapping
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* CreateSchemaMapping();

    /// \brief
    /// Sets the XML configuration stream used to configure the Data Store. 
    /// SetConfiguration can only be called while the connection is closed.
    /// 
    /// \param configStream 
    /// Input the XML configuration stream
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Configuration( NAMESPACE_OSGEO_COMMON_IO::IoStream* configStream);

};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


