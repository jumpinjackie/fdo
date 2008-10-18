#ifndef FDORDBMSFEATURECOMMAND_H
#define FDORDBMSFEATURECOMMAND_H        1
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
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsConnection.h"
#include <Inc/Util/thread.h>
#include <Inc/ut.h>

class  FdoRdbmsUtil;
extern char *fdordbms_cat;

#define  LOCAL_BUFFER_SIZE      GDBI_SCHEMA_ELEMENT_NAME_SIZE + 20   // We only need a slightly larger buffer than the maximum schema element size

template <class FDO_FEATURE_COMMAND> class FdoRdbmsFeatureCommand: public FDO_FEATURE_COMMAND
{
protected:
    friend class FdoRdbmsConnection;

    FdoRdbmsConnection* mFdoConnection;
    
    // Constructs a default instance of a FeatureClassCommand.
    FdoRdbmsFeatureCommand()
    {
        m_Filter = NULL;
        m_ClassName = NULL;
        mFdoConnection = NULL;
        m_TimeOut = 0;
    }

    // Constructs an instance of a FeatureClassCommand using the specified arguments.
    FdoRdbmsFeatureCommand(FdoIConnection* connection)
    {
        m_Filter = NULL;
        m_ClassName = NULL;
        if( connection != NULL )
            connection->AddRef();

        mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(connection);
        m_TimeOut = 0;
    }

    // Default destructor for FeatureCommand.
    virtual ~FdoRdbmsFeatureCommand()
    {
        if( m_ClassName != NULL )
            m_ClassName->Release();

        FDO_SAFE_RELEASE(mFdoConnection);
        FDO_SAFE_RELEASE(m_Filter);
    }

    virtual void Dispose() { delete this; }

public:
    // Gets the name of the class to be operated upon as an Identifier.
    virtual FdoIdentifier* GetFeatureClassName()
    {
        if( m_ClassName )
            m_ClassName->AddRef();

        return m_ClassName;
    }

    virtual FdoIdentifier* GetClassNameRef()
    {
        return m_ClassName;
    }

    // Sets the name of the class to be operated upon as an Identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value)
    {
        DbiConnection *dbiConnection = NULL;
        if (value)
        {
            if (mFdoConnection)
            {
                FdoRdbmsConnection *connection = (FdoRdbmsConnection *) mFdoConnection;
                dbiConnection = connection->GetDbiConnection();
                if (dbiConnection)
                {
                    const FdoSmLpClassDefinition *classDefinition = dbiConnection->GetSchemaUtil()->GetClass(value->GetText());

                    if ( classDefinition == NULL )
                        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value->GetText()));

                    if (classDefinition->GetIsAbstract())
                        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_276, "Setting an abstract class is not currently supported for this command"));
                }
            }
        }

        if( m_ClassName != NULL )
            m_ClassName->Release();

        m_ClassName = NULL;

        if( value != NULL )
        {
            if( ! ut_utf8_from_unicode( value->GetText(), ConversionBuffer,  LOCAL_BUFFER_SIZE ) ||
                  strlen( ConversionBuffer ) >= GDBI_SCHEMA_ELEMENT_NAME_SIZE )
                throw FdoCommandException::Create( NlsMsgGet( FDORDBMS_273, "Class name size exceeds the internal storage limit"));
            value->AddRef();
            m_ClassName = value;
        }
    }

    // Sets the name of the class to be operated upon as an String
    virtual void SetFeatureClassName(const wchar_t* value)
    {
        FdoRdbmsConnection *connection = (FdoRdbmsConnection *) mFdoConnection;
        DbiConnection *dbiConnection = NULL;
        if (connection)
        {
            dbiConnection = connection->GetDbiConnection();
            if (dbiConnection)
            {
                const FdoSmLpClassDefinition *classDefinition = dbiConnection->GetSchemaUtil()->GetClass(value);
                if ( classDefinition == NULL )
                    throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value));

                if (classDefinition->GetIsAbstract())
                    throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_276, "Setting an abstract class is not currently supported for this command"));
            }
        }

        if( m_ClassName != NULL )
            m_ClassName->Release();
        m_ClassName = NULL;

        if( value != NULL )
        {
            if( ! ut_utf8_from_unicode( value, ConversionBuffer,  LOCAL_BUFFER_SIZE ) ||
                  strlen( ConversionBuffer ) >= GDBI_SCHEMA_ELEMENT_NAME_SIZE )
                throw FdoCommandException::Create( NlsMsgGet(  FDORDBMS_273, "Class name size exceeds the internal storage limit"));

            m_ClassName = FdoIdentifier::Create( value );
        }
    }

    // Gets the filter as a Filter tree.
    virtual FdoFilter* GetFilter()
    {
        if( m_Filter )
            m_Filter->AddRef();

        return m_Filter;
    }

    // Gets the filter as a Filter tree.
    virtual FdoFilter* GetFilterRef()
    {
        return m_Filter;
    }

    // Sets the filter as a Filter tree.
    virtual void SetFilter(FdoFilter* value)
    {
        if( m_Filter != NULL )
            m_Filter->Release();

        m_Filter = NULL;

        if( value != NULL )
        {
            value->AddRef();
            m_Filter = value;
        }
    }


    // Sets the filter as a String.
    virtual void SetFilter(const wchar_t* value)
    {
        if( m_Filter != NULL )
            m_Filter->Release();

        m_Filter = NULL;

        if( value != NULL )
        {
            m_Filter = FdoFilter::Parse( value );
        }
    }

    // Gets the IFeatureConnection this command will operate on.
    virtual FdoIConnection* GetConnection()
    {
        if( mFdoConnection != NULL )
            mFdoConnection->AddRef();
        return mFdoConnection;
    }

    // Sets the IFeatureConnection this command will operate on.
    virtual void SetConnection(FdoIConnection* value)
    {
        if (mFdoConnection != NULL)
            mFdoConnection->Release();

        value->AddRef();
        mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(value);
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

    FdoIdentifier*  m_ClassName;

    FdoFilter*      m_Filter;


    int             m_TimeOut;

    char  ConversionBuffer[LOCAL_BUFFER_SIZE];
};

// Gets the transaction in which the Command executes.
template <class FDO_FEATURE_COMMAND>
FdoITransaction* FdoRdbmsFeatureCommand<FDO_FEATURE_COMMAND>::GetTransaction()
{
    return NULL;
}

// Sets the transaction in which the Command executes.
template <class FDO_FEATURE_COMMAND>
void FdoRdbmsFeatureCommand<FDO_FEATURE_COMMAND>::SetTransaction(FdoITransaction* value)
{
}

// Validates and optimizes the command for execution. Calling this method is
// optional, but recommended if the bound to different sets of parameters and
// executed multiple times.
template <class FDO_FEATURE_COMMAND>
void FdoRdbmsFeatureCommand<FDO_FEATURE_COMMAND>::Prepare()
{
}

// Attempts to cancel command execution. Cancel may be called on a separate
// thread after the commands Execute method has been called and before
// Execute has returned. If successful an exception will be thrown from the
// Execute method. If there is nothing to cancel, nothing happens. However,
// if command execution is in process, and the attempt to cancel fails or is
// not supported, no exception is generated.
template <class FDO_FEATURE_COMMAND>
void FdoRdbmsFeatureCommand<FDO_FEATURE_COMMAND>::Cancel()
{
}

// Returns a ParameterValueCollection. If the command requires parameters, the
// literal values to bind to each of those named parameters must be added to
// this collection.
template <class FDO_FEATURE_COMMAND>
FdoParameterValueCollection* FdoRdbmsFeatureCommand<FDO_FEATURE_COMMAND>::GetParameterValues()
{
    return NULL;
}

#endif // FDORDBMSFEATURECOMMAND_H

