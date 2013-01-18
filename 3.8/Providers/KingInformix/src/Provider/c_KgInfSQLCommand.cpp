/*
* Copyright (C) 2007  Haris Kurtagic
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
#include "StdAfx.h"
#include "c_KgInfSQLCommand.h"


c_KgInfSQLCommand::c_KgInfSQLCommand(c_KgInfConnection* Connection) : c_KgInfFdoCommand(Connection)
{
}

c_KgInfSQLCommand::~c_KgInfSQLCommand(void)
{
}


/// \brief
/// Gets the SQL statement to be executed as a string.
/// 
/// \return
/// Returns the SQL statement.
/// 
FdoString* c_KgInfSQLCommand::GetSQLStatement()
{
  return m_SqlStr;
}//end of c_KgInfSQLCommand::GetSQLStatement

/// \brief
/// Sets the SQL statement to be executed as a string.
/// 
/// \param value 
/// Input the SQL statement.
/// 
/// \return
/// Returns nothing
/// 
void c_KgInfSQLCommand::SetSQLStatement(FdoString* SqlStr)
{
  m_SqlStr =SqlStr;
}

/// \brief
/// Executes the SQL statement against the connection object and returns
/// the number of rows affected.
/// 
/// \return
/// Returns the number of rows affected.
/// 
FdoInt32 c_KgInfSQLCommand::ExecuteNonQuery()
{
  unsigned int ret=0;
  otl_stream* stream=NULL;
  
  try
  {
    //occi_stm = m_Connection->OCCI_CreateStatement();    
    //occi_stm->setSQL( (const char*)m_SqlStr);
    stream = m_Connection->OTL_CreateStream(1,m_SqlStr);
    m_Connection->OTL_SetCommit(stream,0);
// set parameters
    FdoPtr<FdoParameterValueCollection> params = GetParameterValues();
    if( params.p && (params->GetCount() > 0) )
    {
      long count = params->GetCount();
      for(long ind=0;ind<count;ind++)
      {
        FdoPtr<FdoParameterValue> param = params->GetItem(ind);
        FdoPtr<FdoLiteralValue> lval = param->GetValue();
        //FdoDataValue*
        FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(lval.p);
        if( dataval )
        {    
          c_FdoInf_API::SetStreamData(stream,ind+1, dataval);
        }
      }
    }
    
    //ret = occi_stm->executeUpdate();
    //m_Connection->OCCI_Commit();
    m_Connection->OTL_FlushStream(stream);
    m_Connection->OTL_Commit();
    
    m_Connection->OTL_DeleteStream(stream);
    
  }
  catch(c_KgOtlException& ea)
  { 
    m_Connection->OTL_DeleteStream(stream);
    
    throw FdoCommandException::Create( ea.what() );    
  }
  
  return ret;
}

/// \brief
/// Executes the SQL statement against the connection object and returns
/// an FdoISQLDataReader.
/// 
/// \return
/// Returns the data reader.
/// 
FdoISQLDataReader* c_KgInfSQLCommand::ExecuteReader()
{
  otl_stream* stream=NULL;
  c_KgOtlStreamReader* streamreader=NULL;
  
  try
  {
    
    //occi_stm = m_Connection->OCCI_CreateStatement();
    //occi_stm->setSQL( (const char*)m_SqlStr);
    //occi_stm->setPrefetchRowCount(50);
    stream = m_Connection->OTL_CreateStream(50,m_SqlStr);
    

// set parameters
    FdoPtr<FdoParameterValueCollection> params = GetParameterValues();
    if( params.p && (params->GetCount() > 0) )
    {
      long count = params->GetCount();
      for(long ind=0;ind<count;ind++)
      {
        FdoPtr<FdoParameterValue> param = params->GetItem(ind);
        FdoPtr<FdoLiteralValue> lval = param->GetValue();
        //FdoDataValue*
        FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(lval.p);
        if( dataval )
        {    
          c_FdoInf_API::SetStreamData(stream,ind+1, dataval);
        }
      }
    }

    streamreader = new c_KgOtlStreamReader(stream);
    //occi_rset = occi_stm->executeQuery();
  }
  catch(c_KgOtlException& ea)
  {
    
    m_Connection->OTL_DeleteStream(stream);
    
    throw FdoCommandException::Create( ea.what() );    
  }

  
  return new c_KgInfSQLDataReader(m_Connection,stream,streamreader);
  
}

