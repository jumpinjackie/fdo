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

#include "c_KgMssFilterProcessor.h"


c_KgMssDelete::c_KgMssDelete(c_KgMssConnection *Conn)
  : c_KgMssFdoFeatureCommand(Conn)
{
}

c_KgMssDelete::~c_KgMssDelete(void)
{
}




/// \brief
/// Executes the update command and returns the number of modified 
/// instances.
/// 
/// \return
/// Returns nothing
/// 
FdoInt32 c_KgMssDelete::Execute()
{
  FdoPtr<c_KgMssSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoIdentifier> classid = GetFeatureClassName();
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
  if( !classdef.p ) return NULL;
  
  
  FdoPtr<c_KgMssOverClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
  FdoStringP fultablename = phys_class->GetSqlQueryFullName();
  FdoStringP table_alias = phys_class->GetOraTableAlias();
  
  
  c_KgMssSridDesc orasrid;
  m_Connection->GetOracleSridDesc(classdef,orasrid);
  
  
  c_FilterStringBuffer sqlstr;
  sqlstr.AppendString("DELETE ");
  sqlstr.AppendString(fultablename);
  sqlstr.AppendString(" ");
  //sqlstr.AppendString(table_alias);
  
  
  // process filter
  const char* filtertext=NULL;
  c_KgMssFilterProcessor fproc(schemadesc,classid,orasrid);
  fproc.GetExpressionProcessor().SetUseTableAllias(false);
  if( m_Filter )
  {      
    m_Filter->Process( &fproc );
  
    filtertext = fproc.GetFilterText();
  }
  else filtertext = NULL;
  
  if( filtertext && *filtertext )
  {
    sqlstr.AppendString(" WHERE ");
    sqlstr.AppendString(filtertext);
  }
  
  int delete_num=0;
  //oracle::occi::Statement* occi_stm=NULL;
  otl_stream *stream;
  
  try
  {
    //occi_stm = m_Connection->OCCI_CreateStatement();    
    //D_KGORA_ELOG_WRITE1("Execute Dslete: '%s",sqlstr.GetString());    
    //occi_stm->setSQL(sqlstr.GetString());
    
    stream = m_Connection->OTL_CreateStream(1,sqlstr.GetString());
    m_Connection->OTL_SetCommit(stream,0);
    
    fproc.GetExpressionProcessor().ApplySqlParameters(stream);
    
    delete_num = m_Connection->OTL_GetRpc(stream);
    //delete_num = occi_stm->executeUpdate();
    //m_Connection->OTL_FlushStream(stream);
    
    m_Connection->OTL_Commit();
    m_Connection->OTL_DeleteStream(stream);
    //m_Connection->OCCI_Commit();
    //if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  }
  catch(c_KgOtlException& ea)
  {
    if( stream ) m_Connection->OTL_DeleteStream(stream);
    
    FdoStringP gstr = ea.what();
    throw FdoCommandException::Create( gstr );    
  }


  
  return delete_num;
}//end of c_KgMssDelete::Execute

FdoILockConflictReader* c_KgMssDelete::GetLockConflicts()
{
  return NULL;
}