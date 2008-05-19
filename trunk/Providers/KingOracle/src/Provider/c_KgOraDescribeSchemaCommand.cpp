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

#include "stdafx.h"

#include "c_LogAPI.h"
#include "c_KgOraSchemaPool.h"

c_KgOraDescribeSchemaCommand::c_KgOraDescribeSchemaCommand (c_KgOraConnection* Connection) 
    
{
  m_KgOraConnection = FDO_SAFE_ADDREF( Connection);
}



c_KgOraDescribeSchemaCommand::~c_KgOraDescribeSchemaCommand (void)
{
}


//---------------------------------------------------------------------------
//
//      FdoIDescribeSchema Interface
//
//---------------------------------------------------------------------------

/// <summary>Gets the name of the schema to describe. This function is optional;
/// if not specified, execution of the command will describe all schemas.</summary>
/// <returns>Returns the schema name</returns> 
const wchar_t* c_KgOraDescribeSchemaCommand::GetSchemaName ()
{
    return (m_SchemaName);
}


/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraDescribeSchemaCommand::SetSchemaName (const wchar_t* SchemaName)
{
    m_SchemaName = SchemaName;
}


/// <summary>Executes the DescribeSchema command and returns a 
/// FdoFeatureSchemaCollection. If a schema name is given that has 
/// references to another schema, the dependent schemas will 
/// be returned as well. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema collection representing the schema created.</returns> 
FdoFeatureSchemaCollection* c_KgOraDescribeSchemaCommand::Execute ()
{
    #ifdef _KGORA_EXTENDED_LOG
      D_KGORA_ELOG_START_FUNC_TIME
    #endif
    
    
    
    FdoPtr<c_KgOraSchemaDesc> schemadesc = m_KgOraConnection->GetSchemaDesc();
    
    FdoPtr<FdoFeatureSchemaCollection> ret = schemadesc->GetFeatureSchema();
    
    //FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas
    FdoFeatureSchemaCollection* ret2 = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(ret);
    
    //FdoFeatureSchemaCollection* ret = m_KgOraConnection->DescribeSchema();
    
    #ifdef _KGORA_EXTENDED_LOG
      D_KGORA_ELOG_WRITE_FUNC_TIME("DescribeSchemaCommand")
    #endif
    
    return ret2;
}


//---------------------------------------------------------------------------
//
//      FdoICommand Interface Implementation
//
//---------------------------------------------------------------------------

FdoIConnection* c_KgOraDescribeSchemaCommand::GetConnection() 
{ 
  return FDO_SAFE_ADDREF(m_KgOraConnection.p); 
} 

FdoITransaction* c_KgOraDescribeSchemaCommand::GetTransaction()
{
  return NULL;
}

void c_KgOraDescribeSchemaCommand::SetTransaction(FdoITransaction* Transaction)
{
}

/// \brief
/// Gets the number of milliseconds to wait before terminating the attempt
/// to execute a command and generating an error. If the provider does not
/// support the timeout capability, 0 is returned.
/// 
/// \return
/// Returns the time (in milliseconds)
///
FdoInt32 c_KgOraDescribeSchemaCommand::GetCommandTimeout()
{ 
  #ifdef _FDO_3_1
    throw GisException::Create (GisException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
  #else
  throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
  #endif
}

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
void c_KgOraDescribeSchemaCommand::SetCommandTimeout(FdoInt32 Timeout)
{
  #ifdef _FDO_3_1
    throw GisException::Create (GisException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
  #else
  throw FdoCommandException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED)));
  #endif
}

/// \brief
/// Returns an FdoParameterValueCollection. If the command requires parameters, the 
/// literal values to bind to each of those named parameters must be added to
/// this collection.
/// 
/// \return
/// Returns the list of parameters and their respective values
/// 
FdoParameterValueCollection* c_KgOraDescribeSchemaCommand::GetParameterValues()
{
  #ifdef _FDO_3_1
    throw FdoCommandException::Create (GisException::NLSGetMessage(FDO_68_COMMAND_PARAMETERS_NOT_SUPPORTED, "Command parameters are not supported."));
  #else
  throw FdoCommandException::Create (FdoException::NLSGetMessage(FDO_68_COMMAND_PARAMETERS_NOT_SUPPORTED, "Command parameters are not supported."));
  #endif
}

/// \brief
/// Validates and optimizes the command for execution. Calling this method is
/// optional, but recommended if bound to different sets of parameters and
/// executed multiple times.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraDescribeSchemaCommand::Prepare()
{
}

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
void c_KgOraDescribeSchemaCommand::Cancel()
{
}
