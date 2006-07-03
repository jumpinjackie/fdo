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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpCommand.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPCOMMAND_H
#define FDORFPCOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpConnection.h"

template <class FDO_COMMAND> class FdoRfpCommand : public FDO_COMMAND
{
    friend class FdoRfpConnection;

protected:
    // The connection this command was created from.
    FdoPtr<FdoRfpConnection> mConnection;

protected:

    // Constructs an instance of a command for the given connection.    
    FdoRfpCommand (FdoIConnection* connection) :
        mConnection (static_cast<FdoRfpConnection*>(connection))
    {
        FDO_SAFE_ADDREF (mConnection.p);
    }

    // Default destructor for a command.
    virtual ~FdoRfpCommand() 
    { 
    }

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
        mConnection = (FdoRfpConnection *)FDO_SAFE_ADDREF(value);
    }

    // Gets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual int GetCommandTimeout ()
    {
        throw FdoException::Create (NlsMsgGet(GRFP_5_COMMAND_TIMEOUT_NOT_SUPPORTED, "Command timeout is not supported"));
    }

    // Sets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual void SetCommandTimeout (int value)
    {
        throw FdoException::Create (NlsMsgGet(GRFP_5_COMMAND_TIMEOUT_NOT_SUPPORTED, "Command timeout is not supported"));
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
};

// Gets the transaction in which the command executes.
template <class FDO_COMMAND> 
FdoITransaction* FdoRfpCommand<FDO_COMMAND>::GetTransaction ()
{
    return (NULL);  //TODO
}

// Sets the transaction in which the command executes.
template <class FDO_COMMAND> 
void FdoRfpCommand<FDO_COMMAND>::SetTransaction (FdoITransaction* value)
{
    //TODO
}

// Validates and optimizes the command for execution. Calling this method is
// optional, but recommended if the bound to different sets of parameters and
// executed multiple times.
template <class FDO_COMMAND> 
void FdoRfpCommand<FDO_COMMAND>::Prepare ()
{
    // NOTE: Oracle Provider didn't implement this method, and neither will we
}

// Attempts to cancel command execution. Cancel may be called on a separate
// thread after the commands Execute method has been called and before 
// Execute has returned. If successful an exception will be thrown from the
// Execute method. If there is nothing to cancel, nothing happens. However,
// if command execution is in process, and the attempt to cancel fails or is
// not supported, no exception is generated.
template <class FDO_COMMAND> 
void FdoRfpCommand<FDO_COMMAND>::Cancel ()
{
    // NOTE: Oracle Provider didn't implement this method, and neither will we
}

// Returns a ParameterValueCollection. If the command requires parameters, the 
// literal values to bind to each of those named parameters must be added to
// this collection. 
template <class FDO_COMMAND> 
FdoParameterValueCollection* FdoRfpCommand<FDO_COMMAND>::GetParameterValues()
{
    throw FdoCommandException::Create (NlsMsgGet(GRFP_30_COMMAND_PARAMETERS_NOT_SUPPORTED, "Command parameters are not supported."));
}


#endif // FDORFPCOMMAND_H
