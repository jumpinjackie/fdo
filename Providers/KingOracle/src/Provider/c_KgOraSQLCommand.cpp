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
#include "StdAfx.h"
#include "c_KgOraSQLCommand.h"
#include "c_FdoOra_API.h"


c_KgOraSQLCommand::c_KgOraSQLCommand(c_KgOraConnection* Connection) : c_KgOraFdoCommand(Connection)
{
}

c_KgOraSQLCommand::~c_KgOraSQLCommand(void)
{
}


/// \brief
/// Gets the SQL statement to be executed as a string.
/// 
/// \return
/// Returns the SQL statement.
/// 
FdoString* c_KgOraSQLCommand::GetSQLStatement()
{
  return m_SqlStr;
}//end of c_KgOraSQLCommand::GetSQLStatement

/// \brief
/// Sets the SQL statement to be executed as a string.
/// 
/// \param value 
/// Input the SQL statement.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraSQLCommand::SetSQLStatement(FdoString* SqlStr)
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
FdoInt32 c_KgOraSQLCommand::ExecuteNonQuery()
{
  unsigned int ret;
  oracle::occi::Statement* occi_stm=NULL;
  oracle::occi::ResultSet* occi_rset=NULL;
  try
  {
    occi_stm = m_Connection->OCCI_CreateStatement();
    
    occi_stm->setSQL( (const char*)m_SqlStr);
 
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
          c_FdoOra_API::SetOracleStatementData(occi_stm,ind+1, dataval);
        }
      }
    }
    
    ret = occi_stm->executeUpdate();
    m_Connection->OCCI_Commit();
    
    if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  }
  catch(oracle::occi::SQLException& ea)
  { 
    if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
    FdoStringP gstr = ea.what();
    throw FdoCommandException::Create( gstr );    
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
FdoISQLDataReader* c_KgOraSQLCommand::ExecuteReader()
{
  oracle::occi::Statement* occi_stm=NULL;
  oracle::occi::ResultSet* occi_rset=NULL;
  try
  {
    occi_stm = m_Connection->OCCI_CreateStatement();
    
    occi_stm->setSQL( (const char*)m_SqlStr);
    occi_stm->setPrefetchRowCount(50);

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
          c_FdoOra_API::SetOracleStatementData(occi_stm,ind+1, dataval);
        }
      }
    }


    occi_rset = occi_stm->executeQuery();
  }
  catch(oracle::occi::SQLException& ea)
  {
    
    if( occi_stm ) 
    {
      if( occi_rset ) occi_stm->closeResultSet(occi_rset);
      occi_rset=NULL;
      m_Connection->OCCI_TerminateStatement(occi_stm);
      occi_stm=NULL;
    }
    
    FdoStringP gstr = ea.what();
    throw FdoCommandException::Create( gstr );    
  }

  
  return new c_KgOraSQLDataReader(m_Connection,occi_stm, occi_rset);
  
}
