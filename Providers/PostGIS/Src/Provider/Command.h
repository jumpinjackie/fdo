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
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
#include <utility>

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

    typedef Command<T> Base;

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

    // Input parameters used by a command to bind to specified placeholders.
    FdoPtr<FdoParameterValueCollection> mParams;

    //
    // Private operations
    //

    // Generate simplified collection of input parameters.
    // Parameters are taken from collection returned by GetParameterValues().
    // Mostly, this collection is passed to commands execution procedures or
    // cursor declaration.
    void PgGenerateExecParams(details::pgexec_params_t& pgParams);

}; // class Command

template <typename T>
Command<T>::Command(Connection* conn) : mConn(conn), mParams(NULL)
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
    //FDOLOG_MARKER("Command::#Dispose()");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
FdoIConnection* Command<T>::GetConnection()
{
    FDO_SAFE_ADDREF(mConn.p);
    return mConn.p;
}

template <typename T>
FdoITransaction* Command<T>::GetTransaction()
{
    throw FdoException::Create(
        NlsMsgGet(MSG_POSTGIS_COMMAND_TRANSACTIONS_NOT_SUPPORTED,
        "The PostGIS Provider does not support direct transactions access from command."));
}

template <typename T>
void Command<T>::SetTransaction(FdoITransaction* value)
{
    throw FdoException::Create(
        NlsMsgGet(MSG_POSTGIS_COMMAND_TRANSACTIONS_NOT_SUPPORTED,
        "The PostGIS Provider does not support direct transactions access from command."));
    
}

template <typename T>
FdoInt32 Command<T>::GetCommandTimeout()
{
    throw FdoException::Create(
        NlsMsgGet(MSG_POSTGIS_CONNECTION_TIMEOUT_UNSUPPORTED,
            "Connection timeout is not supported."));
}

template <typename T>
void Command<T>::SetCommandTimeout(FdoInt32 value)
{
    throw FdoException::Create(
        NlsMsgGet(MSG_POSTGIS_CONNECTION_TIMEOUT_UNSUPPORTED,
        "Connection timeout is not supported."));
}

template <typename T>
FdoParameterValueCollection* Command<T>::GetParameterValues()
{
    if (NULL == mParams)
    {
        mParams = FdoParameterValueCollection::Create();
        assert(NULL != mParams);
    }

    FDO_SAFE_ADDREF(mParams.p);
    return mParams.p;
}

template <typename T>
void Command<T>::Prepare()
{
    assert(!"NOT SUPPORTED");
}

template <typename T>
void Command<T>::Cancel()
{
    assert(!"NOT SUPPORTED");
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void Command<T>::PgGenerateExecParams(details::pgexec_params_t& pgParams)
{
    FDOLOG_MARKER("Command::-PgGenerateExecParams");

    // Make sure the params container is empty
    details::pgexec_params_t().swap(pgParams);

    if (NULL != mParams && mParams->GetCount() > 0)
    {
        FdoStringP name;
        FdoInt32 const paramsCount = mParams->GetCount();

        for (FdoInt32 i = 0; i < paramsCount; ++i)
        {
            name = FdoStringP::Format(L"$%d", i + 1);
            FdoPtr<FdoParameterValue> paramValue = mParams->FindItem(name);
            if (NULL == paramValue)
            {
                FDOLOG_WRITE("Command parameter missing");

                throw FdoCommandException::Create(
                    NlsMsgGet(MSG_POSTGIS_COMMAND_PARAMETER_MISSING,
                    "The command parameter '%1$ls' is missing or named incorrectly.",
                    static_cast<FdoString*>(name)));
            }

            FdoPtr<FdoLiteralValue> literalValue = paramValue->GetValue();
            
            FdoDataValue* dataValue = static_cast<FdoDataValue*>(literalValue.p);
            assert(NULL != dataValue);

            if (dataValue->IsNull())
            {
                // Pair of empty string and NULL flag set
                pgParams.push_back(std::make_pair(std::string(), 1));
            }
            else
            {
                // Buffer for string representation of a parameter value
                std::string value;

                FdoDataType type = dataValue->GetDataType();
                switch (type)
                {
                case FdoDataType_Boolean:
                    {
                        FdoBooleanValue* pval = static_cast<FdoBooleanValue*>(dataValue);
                        value = (pval->GetBoolean() ? "TRUE" : "FALSE");
                    }
                    break;
                case FdoDataType_Byte:
                    {
                        FdoByteValue* pval = static_cast<FdoByteValue*>(dataValue);
                        FdoStringP tmp(pval->ToString());
                        value = static_cast<char const*>(tmp);
                    }
                    break;
                case FdoDataType_Int16:
                case FdoDataType_Int32:
                case FdoDataType_Int64:
                case FdoDataType_Single:
                case FdoDataType_Double:
                case FdoDataType_Decimal:
                    {                        
                        FdoStringP tmp(dataValue->ToString());
                        value = static_cast<char const*>(tmp);
                    }
                    break;
                case FdoDataType_String:
                    {                   
                        // NOTE: FdoStringValue::ToString() adds extra single quotes
                        // around the value. We need a plain value, otherwise it
                        // will be inserted into table with extra quotes.

                        FdoStringValue* pval = static_cast<FdoStringValue*>(dataValue);
                        FdoStringP tmp(pval->GetString());
                        value = static_cast<char const*>(tmp);
                    }
                    break;
                case FdoDataType_DateTime:
                // TODO: Add conversion of DateTime to string.
                    assert(!"TO BE IMPLEMENTED");
                    break;
                case FdoDataType_BLOB:
                    assert(!"TO BE IMPLEMENTED");
                    break;
                case FdoDataType_CLOB:
                    assert(!"TO BE IMPLEMENTED");
                    break;
                default:
                    // TODO: Replace with an exception
                    assert(!"UNKNOWN TYPE");
                }

                // Pair of non-null value and NULL flag unset
                pgParams.push_back(std::make_pair(value, 0));
            }
        } // for

        assert(!pgParams.empty());
        assert(pgParams.size() == static_cast<details::pgexec_params_t::size_type>(paramsCount));
    }

    FDOLOG_WRITE("Number of processed parameters: %u", pgParams.size());
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_COMMAND_H_INCLUDED
