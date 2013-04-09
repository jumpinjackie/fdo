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

#include "c_KgOraFilterProcessor.h"


c_KgOraDelete::c_KgOraDelete(c_KgOraConnection *Conn)
  : c_KgOraFdoFeatureCommand<FdoIDelete>(Conn)
{
}

c_KgOraDelete::~c_KgOraDelete(void)
{
}




/// \brief
/// Executes the update command and returns the number of modified 
/// instances.
/// 
/// \return
/// Returns nothing
/// 
FdoInt32 c_KgOraDelete::Execute()
{
  FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoIdentifier> classid = GetFeatureClassName();
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
  if( !classdef.p ) return NULL;
  
  
  FdoPtr<FdoKgOraClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
  FdoStringP fultablename = phys_class->GetOracleFullTableName();
  FdoStringP table_alias = phys_class->GetOraTableAlias();
  
  
  c_KgOraSridDesc orasrid;
  m_Connection->GetOracleSridDesc(classdef,orasrid);
  
  
  c_FilterStringBuffer sqlstr;
  sqlstr.AppendString(L"DELETE ");
  sqlstr.AppendString(fultablename);
  sqlstr.AppendString(L" ");
  sqlstr.AppendString(table_alias);
  
  
  // process filter
  const wchar_t* filtertext=NULL;
  c_KgOraFilterProcessor fproc(m_Connection->GetOracleMainVersion(),schemadesc,classid,orasrid);
  if( m_Filter )
  {      
    m_Filter->Process( &fproc );
  
    filtertext = fproc.GetFilterText();
  }
  else filtertext = NULL;
  
  if( filtertext && *filtertext )
  {
    sqlstr.AppendString(L" WHERE ");
    sqlstr.AppendString(filtertext);
  }
  
  int delete_num=0;
  c_Oci_Statement* oci_stm=NULL;
  
  try
  {
    oci_stm = m_Connection->OCI_CreateStatement();
    
    #ifdef _KGORA_EXTENDED_LOG
      FdoStringP s1 = sqlstr.GetString();
      D_KGORA_ELOG_WRITE1("Execute Delete: '%s'",(const char*)s1);
    #endif
    
    
    oci_stm->Prepare(sqlstr.GetString());
    
    fproc.GetExpressionProcessor().ApplySqlParameters(oci_stm,orasrid.m_IsGeodetic,orasrid.m_OraSrid);
    

    delete_num = oci_stm->ExecuteNonQuery();
    
    
    if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
  }
  catch(c_Oci_Exception* ea)
  {
    if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);  
    FdoStringP gstr = ea->what();
    
    D_KGORA_ELOG_WRITE2("c_KgOraDelete::Execute%d Exception '%s'",m_Connection->m_ConnNo,(const char*)gstr);
    
    delete ea;
    throw FdoCommandException::Create( gstr );    
  }


  
  return delete_num;
}//end of c_KgOraDelete::Execute

FdoILockConflictReader* c_KgOraDelete::GetLockConflicts()
{
  return NULL;
}
