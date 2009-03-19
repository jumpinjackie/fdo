/*
* Copyright (C) 2009  SL-King d.o.o
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
#include "c_KgOraSQLCommand.h"
#include "c_FdoOra_API2.h"


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
  c_Oci_Statement* oci_stm=NULL;
  try
  {
    oci_stm = m_Connection->OCI_CreateStatement();
    
    #ifdef _KGORA_EXTENDED_LOG 
      
      D_KGORA_ELOG_WRITE1("c_KgOraSQLCommand.ExecuteNonQuery SQL: '%s'",(const char*)m_SqlStr);        
    #endif
    oci_stm->Prepare( m_SqlStr);
 
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
          c_FdoOra_API2::SetOracleStatementData(oci_stm,ind+1, dataval);
        }
      }
    }
    
    ret = oci_stm->ExecuteNonQuery();
    
    
    if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
  }
  catch(c_Oci_Exception* ea)
  { 
    if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
    FdoStringP gstr = ea->what();
    
    D_KGORA_ELOG_WRITE2("(Conn %d) c_KgOraDelete.Execute Exception '%s'",m_Connection->m_ConnNo,(const char*)gstr);
    
    delete ea;
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
  c_Oci_Statement* oci_stm=NULL;
  
  try
  {
    oci_stm = m_Connection->OCI_CreateStatement();
    
    oci_stm->Prepare( m_SqlStr);
    //oci_stm->setPrefetchRowCount(50);

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
          c_FdoOra_API2::SetOracleStatementData(oci_stm,ind+1, dataval);
        }
      }
    }


    oci_stm->ExecuteSelectAndDefine();
  }
  catch(c_Oci_Exception* ea)
  {
    
    if( oci_stm ) 
    {
      
      m_Connection->OCI_TerminateStatement(oci_stm);
      oci_stm=NULL;
    }
    
    FdoStringP gstr = ea->what();
    delete ea;
    throw FdoCommandException::Create( gstr );    
  }

  
  return new c_KgOraSQLDataReader(m_Connection,oci_stm );
  
}
