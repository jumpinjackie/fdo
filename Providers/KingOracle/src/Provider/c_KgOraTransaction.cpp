/*
* Copyright (C) 2007  SL-King d.o.o
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

#include "c_KgOraTransaction.h"
#include "c_KgOraConnection.h"




c_KgOraTransaction::c_KgOraTransaction(c_KgOraConnection* Connection) : m_Connection(Connection)
{
    FDO_SAFE_ADDREF (m_Connection.p);
}

c_KgOraTransaction::~c_KgOraTransaction()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void c_KgOraTransaction::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIc_KgOraTransaction interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnection* c_KgOraTransaction::GetConnection()
{
    FDO_SAFE_ADDREF(m_Connection.p);
    return m_Connection.p;
}

void c_KgOraTransaction::Commit()
{
  c_Oci_Statement* occi_stm=NULL;
try
{  
  occi_stm = m_Connection->OCI_CreateStatement();

  occi_stm->Prepare( L"COMMIT");  
  occi_stm->ExecuteNonQuery();
}
catch(c_Oci_Exception* ea)
{ 
  if( occi_stm ) m_Connection->OCI_TerminateStatement(occi_stm);
  FdoStringP gstr = ea->what();
  delete ea;
  throw FdoCommandException::Create( gstr );    
}
}

void c_KgOraTransaction::Rollback()
{
  c_Oci_Statement* occi_stm=NULL;
  try
  {  
    occi_stm = m_Connection->OCI_CreateStatement();

    occi_stm->Prepare( L"ROLLBACK");  
    occi_stm->ExecuteNonQuery();
  }
  catch(c_Oci_Exception* ea)
  { 
    if( occi_stm ) m_Connection->OCI_TerminateStatement(occi_stm);
    FdoStringP gstr = ea->what();
    delete ea;
    throw FdoCommandException::Create( gstr );    
  }

}
