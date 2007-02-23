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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant_sp2/FdoCommon/inc/c_KgOraFdoCommand.h#1 $
 *     $Author: lee $
 *   $DateTime: 2006/06/02 11:06:52 $
 *     $Change: 13046 $
 *
 */

#ifndef _c_KgOraFdoCommand_h
#define _c_KgOraFdoCommand_h


template <class FDO_COMMAND> class c_KgOraFdoCommand : public FDO_COMMAND
{
    friend class CONNECTION_CLASS;

protected:
    /// The connection this command was created from.
    FdoPtr<c_KgOraConnection> m_Connection;
    
    FdoPtr<FdoParameterValueCollection> m_Parameters;

protected:

    /// Constructs an instance of a command for the given connection.    
    c_KgOraFdoCommand (c_KgOraConnection* Connection) : m_Connection (Connection)
    {
        FDO_SAFE_ADDREF (m_Connection.p);
    }

    /// Default destructor for a command.
    virtual ~c_KgOraFdoCommand() 
    { 
    }

    virtual void Dispose()
    {
        delete this;
    }

public:
    /// Gets the connection this command will operate on.
    virtual FdoIConnection* GetConnection () 
    {
        return (FDO_SAFE_ADDREF(m_Connection.p));
    }

    /// Sets the connection this command will operate on.
    virtual void SetConnection (c_KgOraConnection* Connection)
    {
        m_Connection = FDO_SAFE_ADDREF(Connection);
    }

    /// Gets the wait time before terminating the attempt to execute a command
    /// and generating an error.
    virtual int GetCommandTimeout ()
    {
      #ifdef _FDO_3_1
        throw GisException::Create (GisException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
      #else
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
      #endif
    }

    /// Sets the wait time before terminating the attempt to execute a command
    /// and generating an error.
    virtual void SetCommandTimeout (int value)
    {
        (void)value;  // prevent compiler from complaining about unreferenced arguments
      #ifdef _FDO_3_1
        throw GisException::Create (GisException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
      #else
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
      #endif
    }

    /// Gets the transaction in which the command executes.
    virtual FdoITransaction* GetTransaction ();

    /// Sets the transaction in which the command executes.
    virtual void SetTransaction (FdoITransaction* value);
 
    /// Validates and optimizes the command for execution. Calling this method is
    /// optional, but recommended if the bound to different sets of parameters and
    /// executed multiple times.
    virtual void Prepare ();

    /// Attempts to cancel command execution. Cancel may be called on a separate
    /// thread after the commands Execute method has been called and before 
    /// Execute has returned. If successful an exception will be thrown from the
    /// Execute method. If there is nothing to cancel, nothing happens. However,
    /// if command execution is in process, and the attempt to cancel fails or is
    /// not supported, no exception is generated.
    virtual void Cancel ();

    /// Returns a ParameterValueCollection. If the command requires parameters, the 
    /// literal values to bind to each of those named parameters must be added to
    /// this collection. 
    virtual FdoParameterValueCollection* GetParameterValues ();
};

// Gets the transaction in which the command executes.
template <class FDO_COMMAND> 
FdoITransaction* c_KgOraFdoCommand<FDO_COMMAND>::GetTransaction ()
{
    return (NULL);
}

// Sets the transaction in which the command executes.
template <class FDO_COMMAND> 
void c_KgOraFdoCommand<FDO_COMMAND>::SetTransaction (FdoITransaction* value)
{
    (void)value;  // avoid compile-time warning about unreference argument
}

// Validates and optimizes the command for execution. Calling this method is
// optional, but recommended if the bound to different sets of parameters and
// executed multiple times.
template <class FDO_COMMAND> 
void c_KgOraFdoCommand<FDO_COMMAND>::Prepare ()
{
}

// Attempts to cancel command execution. Cancel may be called on a separate
// thread after the commands Execute method has been called and before 
// Execute has returned. If successful an exception will be thrown from the
// Execute method. If there is nothing to cancel, nothing happens. However,
// if command execution is in process, and the attempt to cancel fails or is
// not supported, no exception is generated.
template <class FDO_COMMAND> 
void c_KgOraFdoCommand<FDO_COMMAND>::Cancel ()
{
}

// Returns a ParameterValueCollection. If the command requires parameters, the 
// literal values to bind to each of those named parameters must be added to
// this collection. 
template <class FDO_COMMAND> 
FdoParameterValueCollection* c_KgOraFdoCommand<FDO_COMMAND>::GetParameterValues()
{
  if( !m_Parameters.p )
  {
    m_Parameters = FdoParameterValueCollection::Create();
  }
  
  return FDO_SAFE_ADDREF(m_Parameters.p);
    //throw FdoCommandException::Create (FdoException::NLSGetMessage(GIS_68_COMMAND_PARAMETERS_NOT_SUPPORTED, "Command parameters are not supported."));
}


#endif // FDOCOMMONCOMMAND_H


