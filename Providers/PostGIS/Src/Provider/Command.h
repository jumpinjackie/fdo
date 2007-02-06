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
#ifndef FDOPOSTGIS_COMMAND_H_INCLUDED
#define FDOPOSTGIS_COMMAND_H_INCLUDED

#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

/// Implementation of interface defining common behavior to all
/// commands in PostGIS provider for FDO.
///
template <typename T>
class Command : public T
{
    friend class Connection;

public:

    //
    // FdoICommand interface
    //

    /// Get instance of connection that this command will operate on.
    virtual FdoIConnection* GetConnection();
 	
    /// Get transaction in frames of which the command executes.
    virtual FdoITransaction* GetTransaction();

    /// Set transaction for the command.
    virtual void SetTransaction(FdoITransaction* value);
 	
    /// Get number of milliseconds to wait before terminating execution of the command.
    /// Termination take place before generating any error.
    virtual FdoInt32 GetCommandTimeout();
 	
    /// Set number of milliseconds to wait before terminating execution of the command.
    /// Termination take place before generating any error.
    virtual void SetCommandTimeout(FdoInt32 value);
 	
    /// Return collection of command parameters and its values.
    /// If the command requires any parameters, the literal values to bind to
    /// each of those named parameters must be added to this collection.
    virtual FdoParameterValueCollection* GetParameterValues();
 	
    /// Prepare command for execution.
    /// Preparation is used to validate and optimization of the command.
    virtual void Prepare();
 	
    /// Send request to cancel command execution.
    virtual void Cancel();

protected:

    /// Construct instance of a command for the given connection.
    Command(Connection* conn);

    /// Destructor.
    virtual ~Command();

    //
    // FdoIDisposable interface
    //
    
    void Dispose();

    //
    // Data members
    //

    // The connection this command was created from.
    FdoPtr<Connection> mConn;
private:
	
}; // class Command

template <typename T>
Command<T>::Command(Connection* conn) : mConn(conn)
{
    FDO_SAFE_ADDREF(mConn.p);
}

template <typename T>
Command<T>::~Command()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void Command<T>::Dispose()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
FdoIConnection* Command<T>::GetConnection()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
FdoITransaction* Command<T>::GetTransaction()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
void Command<T>::SetTransaction(FdoITransaction* value)
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
FdoInt32 Command<T>::GetCommandTimeout()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
void Command<T>::SetCommandTimeout(FdoInt32 value)
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
FdoParameterValueCollection* Command<T>::GetParameterValues()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
void Command<T>::Prepare()
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
void Command<T>::Cancel()
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_COMMAND_H_INCLUDED
