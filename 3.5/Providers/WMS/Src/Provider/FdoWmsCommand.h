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

#ifndef FDOWMSCOMMAND_H
#define FDOWMSCOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Fdo.h"
#include "FdoWmsCommand.h"
#include "FdoWmsConnection.h"

template <class FDO_COMMAND> class FdoWmsCommand : public FDO_COMMAND
{
    friend class FdoWmsConnection;

protected:

    // <summary>Constructs an instance of a command for the given connection.</summary>   
    // <returns>Returns nothing</returns> 
    FdoWmsCommand (FdoIConnection* connection) :
        mConnection (static_cast<FdoWmsConnection*>(connection))
    {
        FDO_SAFE_ADDREF (mConnection.p);
    }

    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsCommand() 
    { 
    }

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose()
    {
        delete this;
    }

public:
    // Gets the connection this command will operate on.
    virtual FdoIConnection* GetConnection () 
    {
        return (FDO_SAFE_ADDREF(mConnection.p));
    }

    // Sets the connection this command will operate on.
    virtual void SetConnection (FdoIConnection* value)
    {
        mConnection = (FdoWmsConnection *)FDO_SAFE_ADDREF(value);
    }

    // Gets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual int GetCommandTimeout ()
    {
        throw FdoException::Create (NlsMsgGet(FDOWMS_COMMAND_TIMEOUT_NOT_SUPPORTED, "Command timeout is not supported."));
    }

    // Sets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual void SetCommandTimeout (int value)
    {
        throw FdoException::Create (NlsMsgGet(FDOWMS_COMMAND_TIMEOUT_NOT_SUPPORTED, "Command timeout is not supported."));
    }

    // Gets the transaction in which the command executes.
    virtual FdoITransaction* GetTransaction ();

    // Sets the transaction in which the command executes.
    virtual void SetTransaction (FdoITransaction* value);
 
    // Validates and optimizes the command for execution. Calling this method is
    // optional, but recommended if the bound to different sets of parameters and
    // executed multiple times.
    virtual void Prepare ();

    // Attempts to cancel command execution. Cancel may be called on a separate
    // thread after the commands Execute method has been called and before 
    // Execute has returned. If successful an exception will be thrown from the
    // Execute method. If there is nothing to cancel, nothing happens. However,
    // if command execution is in process, and the attempt to cancel fails or is
    // not supported, no exception is generated.
    virtual void Cancel ();

    // Returns a ParameterValueCollection. If the command requires parameters, the 
    // literal values to bind to each of those named parameters must be added to
    // this collection. 
    virtual FdoParameterValueCollection* GetParameterValues ();

protected:
    // The connection this command was created from.
    FdoPtr<FdoWmsConnection> mConnection;
};

// Gets the transaction in which the command executes.
template <class FDO_COMMAND> 
FdoITransaction* FdoWmsCommand<FDO_COMMAND>::GetTransaction ()
{
    return (NULL);
}

// Sets the transaction in which the command executes.
template <class FDO_COMMAND> 
void FdoWmsCommand<FDO_COMMAND>::SetTransaction (FdoITransaction* value)
{
}

// Validates and optimizes the command for execution. Calling this method is
// optional, but recommended if the bound to different sets of parameters and
// executed multiple times.
template <class FDO_COMMAND> 
void FdoWmsCommand<FDO_COMMAND>::Prepare ()
{
}

// Attempts to cancel command execution. Cancel may be called on a separate
// thread after the commands Execute method has been called and before 
// Execute has returned. If successful an exception will be thrown from the
// Execute method. If there is nothing to cancel, nothing happens. However,
// if command execution is in process, and the attempt to cancel fails or is
// not supported, no exception is generated.
template <class FDO_COMMAND> 
void FdoWmsCommand<FDO_COMMAND>::Cancel ()
{
}

// Returns a ParameterValueCollection. If the command requires parameters, the 
// literal values to bind to each of those named parameters must be added to
// this collection. 
template <class FDO_COMMAND> 
FdoParameterValueCollection* FdoWmsCommand<FDO_COMMAND>::GetParameterValues()
{
    throw FdoCommandException::Create (NlsMsgGet(FDOWMS_COMMAND_PARAMETERS_NOT_SUPPORTED, "Command parameters are not supported."));
}


#endif // FDOWMSCOMMAND_H
