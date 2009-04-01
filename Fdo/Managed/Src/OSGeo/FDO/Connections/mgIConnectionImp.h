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

#include "FDO\Connections\mgIConnection.h"

class FdoIConnection;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS

/// \ingroup (OSGeoFDOConnections)
/// \brief
/// The IConnection interface enables a feature provider to implement a
/// connection class, which represents a unique session with a data
/// store.
public ref class IConnectionImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection
{
public:
    /// \brief
    /// Gets an IConnectionCapabilities interface describing the capabilities
    /// of the connection.
    /// 
    /// \return
    /// Returns the connection capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities^ ConnectionCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities^ get();
    }

    /// \brief
    /// Gets an ISchemaCapabilities interface describing the provider's support for the feature schema.
    /// 
    /// \return
    /// Returns schema capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities^ SchemaCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities^ get();
    }

    /// \brief
    /// Gets an ICommandCapabilities interface describing the commands a provider supports.
    /// 
    /// \return
    /// Returns the command capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities^ CommandCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities^ get();
    }

    /// \brief
    /// Gets an IFilterCapabilities interface describing the provider's support for filters.
    /// 
    /// \return
    /// Returns the filter capabilities
    /// 
	virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities^ FilterCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities^ get();
    }

    /// \brief
    /// Gets an IExpressionCapabilities interface describing the provider's support for expressions.
    /// 
    /// \return
    /// Returns the expression capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities^ ExpressionCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities^ get();
    }

    /// \brief
    /// Gets an IRasterCapabilities interface describing the provider's support for raster images.
    /// 
    /// \return
    /// Returns the raster capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities^ RasterCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities^ get();
    }

    /// \brief
    /// Gets an ITopologyCapabilities interface describing the provider's support for topology.
    /// 
    /// \return
    /// Returns the topology capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities^ TopologyCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities^ get();
    }

    /// \brief
    /// Gets an IGeometryCapabilities interface describing the provider's support for geometry.
    /// 
    /// \return
    /// Returns the geometry capabilities
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities^ GeometryCapabilities
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities^ get();
    }

    /// \brief
    /// Gets the connection string used to open a DataStore.
    /// 
    /// \return
    /// Returns the connection string
    /// 
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
    virtual property System::String^ ConnectionString
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets an IConnectionInfo interface that can be used to interrogate and set connection properties.
    /// 
    /// \return
    /// Returns the connection info
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo^ ConnectionInfo
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo^ get();
    }

    /// \brief
    /// Gets the current state of the connection.
    /// 
    /// \return
    /// Returns the current state of the connection
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState ConnectionState
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState get();
    }

    /// \brief
    /// Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned
    /// 
    /// \return
    /// Returns the time to wait (in milliseconds)
    /// 
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
    virtual property System::Int32 ConnectionTimeout
    {
        System::Int32 get();
        System::Void set(System::Int32 value);
    }

    /// \brief
    /// Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState Open();

    /// \brief
    /// Closes the connection to the DataStore
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Close();

    /// \brief
    /// Begins a transaction and returns an object that realizes
    /// ITransaction.
    /// 
    /// \return
    /// Returns the transaction
    /// 
	virtual NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ BeginTransaction();

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
	virtual NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ CreateCommand(NAMESPACE_OSGEO_FDO_COMMANDS::CommandType commandType);

    /// \brief
    /// Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.
    /// 
    /// \return
    /// Returns PhysicalSchemaMapping
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ CreateSchemaMapping();

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
    virtual property NAMESPACE_OSGEO_COMMON_IO::IoStream^ Configuration
    {
        System::Void set(NAMESPACE_OSGEO_COMMON_IO::IoStream^ configStream);
    }

    /// \brief
    /// Forces the writes of any cached data to the targed datastore.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual System::Void Flush();

internal:
	inline FdoIConnection* GetImpObj();

public:
    virtual IntPtr GetDisposableObject() override;
	IConnectionImp(System::IntPtr unmanaged, System::Boolean autoDelete);
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


