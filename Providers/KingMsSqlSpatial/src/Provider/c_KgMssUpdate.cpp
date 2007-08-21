/*
* Copyright (C) 2006  Haris Kurtagic
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

c_KgMssUpdate::c_KgMssUpdate(c_KgMssConnection *Conn)
  : c_KgMssFdoFeatureCommand(Conn)
{
}

c_KgMssUpdate::~c_KgMssUpdate(void)
{
}



/// \brief
/// Gets the FdoPropertyValueCollection that specifies the names and values
/// of the properties for the instance to be inserted.
/// 
/// \return
/// Returns the list of properties and their values.
/// 
FdoPropertyValueCollection* c_KgMssUpdate::GetPropertyValues()
{
  if( !m_PropertyValues.p )
  {
    m_PropertyValues = FdoPropertyValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_PropertyValues.p );
}


/// \brief
/// Executes the insert command and returns a reference to an
/// FdoIFeatureReader. Some feature providers can generate automatic identity
/// values for features. This will happen automatically as the features are
/// inserted. The returned FdoIFeatureReader allows the client to obtain the
/// automatic identity property value(s) of newly inserted object(s). The
/// returned feature reader at a minimum will read the unique identity
/// properties of the objects just inserted. Multiple objects will be
/// returned through the reader in the case of a batch insert.
/// 
/// \return
/// Returns an FdoIFeatureReader
/// 
FdoInt32 c_KgMssUpdate::Execute()
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
  
  if( m_PropertyValues.p )
  {
    c_FilterStringBuffer strbuff;
    c_KgMssExpressionProcessor expproc(&strbuff,schemadesc,classid,orasrid);
    expproc.SetUseTableAllias(false);
    
    FdoStringP colupdates;
    FdoStringP colvalues;
    FdoStringP sep;
    
    unsigned int count = m_PropertyValues->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
      FdoPtr<FdoIdentifier> propid = propval->GetName();
      FdoPtr<FdoValueExpression> expr = propval->GetValue();
                       
      strbuff.ClearBuffer();
      expr->Process( &expproc );
      
      colupdates += sep + propid->GetName() + " = " + strbuff.GetString();      
      
      sep = ",";
    }
    
    c_FilterStringBuffer sqlstr;
    sqlstr.AppendString("UPDATE ");
    sqlstr.AppendString(fultablename);
    sqlstr.AppendString(" ");
    //sqlstr.AppendString(table_alias);  // ms sql server doesn't support alias in UPDATE
    sqlstr.AppendString(" SET ");
    sqlstr.AppendString(colupdates);
    sqlstr.AppendString(" ");
    
    // process filter
    const char* filtertext=NULL;
    c_KgMssFilterProcessor fproc(schemadesc,classid,orasrid,expproc.GetSqlParametersCount());
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
    
    int update_num=0;
    otl_stream* stream=NULL;
    
    try
    {
      //occi_stm->setSQL(sqlstr.GetString());
      //D_KGORA_ELOG_WRITE1("Execute Update: '%s",sqlstr.GetString());
      
      
      stream = m_Connection->OTL_CreateStream(1,sqlstr.GetString());
      m_Connection->OTL_SetCommit(stream,0);
      
      // fist apply binds from update values
      expproc.ApplySqlParameters(stream);
      // then apply sql binds from filter expresion
      fproc.GetExpressionProcessor().ApplySqlParameters(stream,expproc.GetSqlParametersCount());
      
      
      update_num = m_Connection->OTL_GetRpc(stream);
      //m_Connection->OCCI_Commit();
      //if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
      
      m_Connection->OTL_FlushStream(stream);
      m_Connection->OTL_Commit();
      m_Connection->OTL_DeleteStream(stream);
      
      
      
      
      
      
    }
    catch(c_KgOtlException& ea)
    {      
      m_Connection->OTL_DeleteStream(stream);
      FdoStringP gstr = ea.what();
      throw FdoCommandException::Create( gstr );    
    }

    
    return update_num;
    
  }
  
  
  return 0;
}//end of c_KgMssUpdate::Execute

FdoILockConflictReader* c_KgMssUpdate::GetLockConflicts()
{
  return NULL;
}