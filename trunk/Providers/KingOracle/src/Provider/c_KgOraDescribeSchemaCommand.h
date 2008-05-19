/*
* Copyright (C) 2006  SL-King d.o.o
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
*/
#ifndef _c_KGORADESCRIBESCHEMACOMMAND_H
#define _c_KGORADESCRIBESCHEMACOMMAND_H


class c_KgOraDescribeSchemaCommand :    public FdoIDescribeSchema
{
    friend class c_KgOraConnection;
    friend class c_KgOraFeatureReader; // friends rather than make a SchemaUtilities class just for that
   

protected:
  FdoPtr<c_KgOraConnection> m_KgOraConnection;
  FdoStringP m_SchemaName;

private:
   
    // Constructs an instance of a DescribeSchema command using the given connection.
    c_KgOraDescribeSchemaCommand (c_KgOraConnection* Connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~c_KgOraDescribeSchemaCommand (void);

    virtual void Dispose()
    {
        delete this;
    }
    
public:
//---------------------------------------------------------------------------
//      FdoIDescribeSchema Interface
//---------------------------------------------------------------------------
    /// \brief
    /// Gets the name of the schema to describe. This function is optional;
    /// if not specified, execution of the command will describe all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    FDOKGORA_API virtual FdoString* GetSchemaName();

    /// \brief
    /// Sets the name of the schema to describe. This function is optional; if not
    /// specified execution of the command will describe all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGORA_API virtual void SetSchemaName(FdoString* SchemaName);

    /// \brief
    /// Executes the DescribeSchema command and returns a 
    /// FdoFeatureSchemaCollection. If a schema name is given that has 
    /// references to another schema, the dependent schemas will 
    /// be returned as well. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the schema collection representing the schema created.
    /// The element states for all elements will be set to FdoSchemaElementState_Unchanged.
    /// Each provider-specific implementation of Execute() can ensure 
    /// that this is the case by 
    /// calling FdoFeatureSchema::AcceptChanges() for each feature schema
    /// in the returned collection.
    /// 
    FDOKGORA_API virtual FdoFeatureSchemaCollection* Execute();    
//---------------------------------------------------------------------------
//      FdoICommand Interface
//---------------------------------------------------------------------------    
  /// \brief
    /// Gets the FdoIConnection that this command will operate on.
    /// 
    /// \return
    /// Returns the connection object
    /// 
    FDOKGORA_API virtual FdoIConnection* GetConnection();

    /// \brief
    /// Gets the transaction in which the command executes.
    /// 
    /// \return
    /// Returns the transaction object
    /// 
    FDOKGORA_API virtual FdoITransaction* GetTransaction();

    /// \brief
    /// Sets the transaction in which the command executes.
    /// 
    /// \param value 
    /// Input the transaction object
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGORA_API virtual void SetTransaction(FdoITransaction* Transaction);

    /// \brief
    /// Gets the number of milliseconds to wait before terminating the attempt
    /// to execute a command and generating an error. If the provider does not
    /// support the timeout capability, 0 is returned.
    /// 
    /// \return
    /// Returns the time (in milliseconds)
    /// 
    FDOKGORA_API virtual FdoInt32 GetCommandTimeout();

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
    FDOKGORA_API virtual void SetCommandTimeout(FdoInt32 value);

    /// \brief
    /// Returns an FdoParameterValueCollection. If the command requires parameters, the 
    /// literal values to bind to each of those named parameters must be added to
    /// this collection.
    /// 
    /// \return
    /// Returns the list of parameters and their respective values
    /// 
    FDOKGORA_API virtual FdoParameterValueCollection* GetParameterValues();

    /// \brief
    /// Validates and optimizes the command for execution. Calling this method is
    /// optional, but recommended if bound to different sets of parameters and
    /// executed multiple times.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGORA_API virtual void Prepare();

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
    FDOKGORA_API virtual void Cancel();
};

#endif 

