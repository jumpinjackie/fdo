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
#ifndef SDFCOMMAND_H
#define SDFCOMMAND_H

#include "SdfConnection.h"

//Base class for all Sdf commands. Defines common functionality and
//behavior.
template <class COMMAND> class SdfCommand : public COMMAND
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a command using the specified connection
        SdfCommand(SdfConnection* connection)
        {
            m_connection = connection;
            FDO_SAFE_ADDREF(m_connection);
        }

    protected:
        // default destructor
        virtual ~SdfCommand()
        {
            FDO_SAFE_RELEASE(m_connection);
        }

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        SDF_API virtual void Dispose()
        {
            delete this;
        }

    //-------------------------------------------------------
    // FdoICommand implementation
    //-------------------------------------------------------

    public:
        // Gets the IFeatureConnection this command will operate on.
        SDF_API virtual FdoIConnection* GetConnection()
        {
            // addref the connection because we own it
            FDO_SAFE_ADDREF(m_connection);
            return m_connection;
        }

        // Gets the transaction in which the Command executes.
        SDF_API virtual FdoITransaction* GetTransaction()
        {
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_28_TRANSACTIONS_NOT_SUPPORTED)));
        }

        // Sets the transaction in which the Command executes.
        SDF_API virtual void SetTransaction(FdoITransaction* value)
        {
            // no transaction support for SDF+
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_28_TRANSACTIONS_NOT_SUPPORTED)));
        }

        // Gets the number of milliseconds to wait before terminating the attempt
        // to execute a command and generating an error.  If the provider does not
        // support the timeout capability 0 will be returned.
        SDF_API virtual FdoInt32 GetCommandTimeout()
        {
            // no timeout support for SDF+
            return 0;
        }

        // Sets the number of milliseconds to wait before terminating the attempt
        // to execute a command and generating an error.  If the provider does not
        // support the timeout capability then attempting to set a timeout will
        // result in an exception.
        SDF_API virtual void SetCommandTimeout(FdoInt32 value)
        {
            // no timeout support for SDF+
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_8_TIMEOUT_NOT_SUPPORTED)));
        }

        // Returns a ParameterValueCollection.  If the command requires parameters,
        // the literal values to bind to each of those named parameters must be
        // added to this collection.
        SDF_API virtual FdoParameterValueCollection* GetParameterValues()
        {
            // this command has no parameters
            return NULL;
        }

        // Validates and optimizes the command for execution.  Calling this method is
        // optional, but recommended if the bound to different sets of parameters and
        // executed multiple times.
        SDF_API virtual void Prepare()
        {
        }

        // Attempts to cancel command execution.  Cancel may be called on a separate
        // thread after the command's Execute method has been called and before
        // Execute has returned.  If successful an exception will be thrown from the
        // Execute method.  If there is nothing to cancel, nothing happens.  If
        // command execution is in process, and the attempt to cancel fails or is
        // not supported an exception will be thrown.
        SDF_API virtual void Cancel()
        {
            // no cancel support for SDF+
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_29_CANCEL_NOT_SUPPORTED)));
        }


    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    protected:
        SdfConnection* m_connection;

};

#endif

