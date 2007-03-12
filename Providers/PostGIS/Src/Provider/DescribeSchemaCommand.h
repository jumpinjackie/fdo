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
#ifndef FDOPOSTGIS_DESCRIBESCHEMACOMMAND_H_INCLUDED
#define FDOPOSTGIS_DESCRIBESCHEMACOMMAND_H_INCLUDED

#include "Connection.h"
#include "PgGeometryColumn.h"

//
// Forward declarations
//
class FdoIConnection;
class FdoITransaction;
class FdoFeatureSchemaCollection;
class FdoParameterValueCollection;

namespace fdo { namespace postgis {

class SpatialContext;

/// Implementation of interface defining DescribeSchema command.
/// This command describes feature schema available from currently
/// connected PostGIS datastore.
///
/// \todo Check correct base class, ie. Command<FdoIDescribeSchema>
///
class DescribeSchemaCommand : public FdoIDescribeSchema
{
    friend class Connection;
    
public:

    //
    // FdoICommand interface
    //

    /// Get instance of connection that this command will operate on.
    FdoIConnection* GetConnection();
 	
    /// Get transaction in frames of which the command executes.
    FdoITransaction* GetTransaction();

    /// Set transaction for the command.
    void SetTransaction(FdoITransaction* value);
 	
    /// Get number of milliseconds to wait before terminating execution of the command.
    /// Termination take place before generating any error.
    FdoInt32 GetCommandTimeout();
 	
    /// Set number of milliseconds to wait before terminating execution of the command.
    /// Termination take place before generating any error.
    void SetCommandTimeout(FdoInt32 value);
 	
    /// Return collection of command parameters and its values.
    /// If the command requires any parameters, the literal values to bind to
    /// each of those named parameters must be added to this collection.
    FdoParameterValueCollection* GetParameterValues();
 	
    /// Prepare command for execution.
    /// Preparation is used to validate and optimize command execution.
    /// Currently, it HAS NO EFFECT for PostGIS.
    void Prepare();
 	
    /// Send request to cancel command execution.
    /// Currently, it HAS NO EFFECT for PostGIS.
    void Cancel();

    //
    // FdoIDescribeSchema interface
    //

    /// Get name of the schema requested to describe.
    /// \return Name of schema being described.
    FdoString* GetSchemaName();
 	
    /// Sets the name of the schema to describe.
    /// This function is optional and if not specified, execution of the command will
    /// describe all schemas.
    /// \param name [in] - name of schema to describe by the command.
    void SetSchemaName(FdoString* name);
 	
    /// Execute the DescribeSchema command and return collection of
    /// feature schemas available from the connection.
    FdoFeatureSchemaCollection* Execute();

protected:

    //
    // Data members
    //

    /// The connection this command was created from.
    Connection::Ptr mConn;

    /// Name of schema being described.
    FdoStringP mSchemaName;

    //
    // Protected operations
    //

    /// Default constructor.
    DescribeSchemaCommand(Connection* conn);

    /// Destructor.
    virtual ~DescribeSchemaCommand();

    //
    // FdoDisposable interface
    //

    void Dispose();

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_DESCRIBESCHEMACOMMAND_H_INCLUDED
