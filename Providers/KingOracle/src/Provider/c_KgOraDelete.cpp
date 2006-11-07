#include "StdAfx.h"

#include "c_KgOraFilterProcessor.h"


c_KgOraDelete::c_KgOraDelete(c_KgOraConnection *Conn)
  : c_KgOraFdoFeatureCommand(Conn)
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
  sqlstr.AppendString("DELETE ");
  sqlstr.AppendString(fultablename);
  sqlstr.AppendString(" ");
  sqlstr.AppendString(table_alias);
  
  
  // process filter
  const char* filtertext=NULL;
  c_KgOraFilterProcessor fproc(schemadesc,classid,orasrid);
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
  oracle::occi::Statement* occi_stm=NULL;
  oracle::occi::ResultSet* occi_rset=NULL;
  try
  {
    occi_stm = m_Connection->OCCI_CreateStatement();
    
    D_KGORA_ELOG_WRITE1("Execute Dslete: '%s",sqlstr.GetString());
    
    occi_stm->setSQL(sqlstr.GetString());
    
    fproc.GetExpressionProcessor().ApplySqlParameters(occi_stm);
    

    delete_num = occi_stm->executeUpdate();
    
    m_Connection->OCCI_Commit();
  }
  catch(oracle::occi::SQLException& ea)
  {
    occi_rset = NULL;
    FdoStringP gstr = ea.what();
    throw FdoCommandException::Create( gstr );    
  }


  
  return delete_num;
}//end of c_KgOraDelete::Execute

FdoILockConflictReader* c_KgOraDelete::GetLockConflicts()
{
  return NULL;
}