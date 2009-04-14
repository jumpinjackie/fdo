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

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
public __gc __interface IConnection;
public __gc __interface ITransaction;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc class ParameterValueCollection;

/// \ingroup (OSGeoFDOCommands)
/// \interface OSGeo::FDO::Commands::ICommand
/// \brief
/// The ICommand interface defines behavior common to all commands in FDO. In
/// order to be executed, commands must have an association to a connection.
/// Commands can also be optionally associated with a transaction if the
/// connection supports transactions. The parameter values collection allows
/// values to be specified for commands that support expressions and/or filters.
public __gc __interface ICommand : public System::IDisposable
{
public:
    /// \brief
    /// Gets the IConnection that this command will operate on.
    /// 
    /// \return
    /// Returns the connection object
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* get_Connection();

    /// \brief
    /// Gets the transaction in which the command executes.
    /// 
    /// \return
    /// Returns the transaction object
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction* get_Transaction();

    /// \brief
    /// Sets the transaction in which the command executes.
    /// 
    /// \param value 
    /// Input the transaction object
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Transaction(NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction* value);

    /// \brief
    /// Gets the number of milliseconds to wait before terminating the attempt
    /// to execute a command and generating an error. If the provider does not
    /// support the timeout capability, 0 is returned.
    /// 
    /// \return
    /// Returns the time (in milliseconds)
    /// 
	__property System::Int32 get_CommandTimeOut();

    /// \brief
    /// Sets the number of milliseconds to wait before terminating the attempt
    /// to execute a command and generating an error. If the provider does not
    /// support the timeout capability, then attempting to set a timeout will
    /// result in an exception.
    /// 
    /// \param value 
    /// Input the time (in milliseconds)
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CommandTimeOut(System::Int32 value);

    /// \brief
    /// Returns an ParameterValueCollection. If the command requires parameters, the 
    /// literal values to bind to each of those named parameters must be added to
    /// this collection.
    /// 
    /// \return
    /// Returns the list of parameters and their respective values
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection* get_ParameterValues();

    /// \brief
    /// Validates and optimizes the command for execution. Calling this method is
    /// optional, but recommended if bound to different sets of parameters and
    /// executed multiple times.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Prepare();

    /// \brief
    /// Attempts to cancel command execution. Cancel may be called on a separate
    /// thread after the commands Execute method has been called and before 
    /// Execute has returned. If successful, an exception is thrown from the
    /// Execute method. If there is nothing to cancel, nothing happens. If
    /// command execution is in process, and the attempt to cancel fails or is
    /// not supported, an exception is thrown.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Cancel();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


