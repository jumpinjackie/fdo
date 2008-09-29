#ifndef FDORDBMSCOMMAND_H
#define FDORDBMSCOMMAND_H       1
#ifdef _WIN32
#pragma once
#endif
//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "Fdo.h"

class FdoRdbmsConnection;

template <class FDO_COMMAND> class FdoRdbmsCommand: public FDO_COMMAND
{
protected:
    friend class FdoRdbmsConnection;


    FdoRdbmsConnection* mFdoConnection;

    // Constructs a default instance of a FeatureClassCommand.
    FdoRdbmsCommand()
    {
        this->mFdoConnection = NULL;
        m_TimeOut = 0;
    }

    // Constructs an instance of a FeatureClassCommand using the specified arguments.
    FdoRdbmsCommand(FdoIConnection* connection)
    {
        if( connection != NULL )
            connection->AddRef();
        this->mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(connection);
        m_TimeOut = 0;
    }

    // Default destructor for FeatureCommand.
    virtual ~FdoRdbmsCommand()
    {
        FDO_SAFE_RELEASE(this->mFdoConnection);
    }

    virtual void Dispose() { delete this; }
public:
    // Gets the IFeatureConnection this command will operate on.
    virtual FdoIConnection* GetConnection()
    {
        if( this->mFdoConnection != NULL )
            this->mFdoConnection->AddRef();
        return this->mFdoConnection;
    }

    // Sets the IFeatureConnection this command will operate on.
    virtual void SetConnection(FdoIConnection* value)
    {
        if (this->mFdoConnection != NULL)
            this->mFdoConnection->Release();

        value->AddRef();
        this->mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(value);
    }

    // Gets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual int GetCommandTimeout() { return m_TimeOut; }

    // Sets the wait time before terminating the attempt to execute a command
    // and generating an error.
    virtual void SetCommandTimeout(int value) { m_TimeOut = value; }

    // Gets the transaction in which the Command executes.
    virtual FdoITransaction* GetTransaction();

    // Sets the transaction in which the Command executes.
    virtual void SetTransaction(FdoITransaction* value);

    // Validates and optimizes the command for execution. Calling this method is
    // optional, but recommended if the bound to different sets of parameters and
    // executed multiple times.
    virtual void Prepare();

    // Attempts to cancel command execution. Cancel may be called on a separate
    // thread after the commands Execute method has been called and before
    // Execute has returned. If successful an exception will be thrown from the
    // Execute method. If there is nothing to cancel, nothing happens. However,
    // if command execution is in process, and the attempt to cancel fails or is
    // not supported, no exception is generated.
    virtual void Cancel();

    // Returns a ParameterValueCollection. If the command requires parameters, the
    // literal values to bind to each of those named parameters must be added to
    // this collection.
    virtual FdoParameterValueCollection* GetParameterValues();


private:
    

    int             m_TimeOut;
};

// Gets the transaction in which the Command executes.
template <class FDO_COMMAND>
FdoITransaction* FdoRdbmsCommand<FDO_COMMAND>::GetTransaction()
{
    return NULL;
}

// Sets the transaction in which the Command executes.
template <class FDO_COMMAND>
void FdoRdbmsCommand<FDO_COMMAND>::SetTransaction(FdoITransaction* value)
{
}

// Validates and optimizes the command for execution. Calling this method is
// optional, but recommended if the bound to different sets of parameters and
// executed multiple times.
template <class FDO_COMMAND>
void FdoRdbmsCommand<FDO_COMMAND>::Prepare()
{
}

// Attempts to cancel command execution. Cancel may be called on a separate
// thread after the commands Execute method has been called and before
// Execute has returned. If successful an exception will be thrown from the
// Execute method. If there is nothing to cancel, nothing happens. However,
// if command execution is in process, and the attempt to cancel fails or is
// not supported, no exception is generated.
template <class FDO_COMMAND>
void FdoRdbmsCommand<FDO_COMMAND>::Cancel()
{
}

// Returns a ParameterValueCollection. If the command requires parameters, the
// literal values to bind to each of those named parameters must be added to
// this collection.
template <class FDO_COMMAND>
FdoParameterValueCollection* FdoRdbmsCommand<FDO_COMMAND>::GetParameterValues()
{
    return NULL;
}

#endif // FDORDBMSCOMMAND_H

