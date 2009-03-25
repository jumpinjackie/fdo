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
#include "c_KgOraInsert.h"
#include "c_FdoOra_API2.h"
#include "c_Ora_API2.h"
#include "c_FgfToSdoGeom.h"

c_KgOraInsert::c_KgOraInsert(c_KgOraConnection *Conn)
  : c_KgOraFdoCommand(Conn)
{
}

c_KgOraInsert::~c_KgOraInsert(void)
{
}

/// \brief
/// Gets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \return
/// Returns the class name.
/// 
FdoIdentifier* c_KgOraInsert::GetFeatureClassName()
{
  return FDO_SAFE_ADDREF(m_ClassId.p);
}

/// \brief
/// Sets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \param value 
/// Input the identifier for the class.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraInsert::SetFeatureClassName(FdoIdentifier* ClassId)
{
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
}

/// \brief
/// Sets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \param value 
/// Input the class name.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraInsert::SetFeatureClassName(FdoString* ClassName)
{
 
  m_ClassId = FdoIdentifier::Create( ClassName );  
}

/// \brief
/// Gets the FdoPropertyValueCollection that specifies the names and values
/// of the properties for the instance to be inserted.
/// 
/// \return
/// Returns the list of properties and their values.
/// 
FdoPropertyValueCollection* c_KgOraInsert::GetPropertyValues()
{
  if( !m_PropertyValues.p )
  {
    m_PropertyValues = FdoPropertyValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_PropertyValues.p );
}

/// \brief
/// Gets the FdoBatchParameterValueCollection that can be used for optimized
/// batch inserts of multiple features with a single insert command. Batch
/// inserts can be performed by using Parameters for each of the property
/// values, then adding collections of parameter values to the
/// FdoBatchParameterValueCollection. Each FdoParameterValueCollection in the
/// FdoBatchParameterValueCollection should contain one FdoParameterValue for each
/// of the parameters specified for property values.
/// 
/// \return
/// Returns FdoBatchParameterValueCollection
/// 
FdoBatchParameterValueCollection* c_KgOraInsert::GetBatchParameterValues()
{
  if( !m_BatchParameterValues.p )
  {
    m_BatchParameterValues = FdoBatchParameterValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_BatchParameterValues.p );
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
FdoIFeatureReader* c_KgOraInsert::Execute()
{
  FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(m_ClassId);
  if( !classdef.p ) 
  {
    throw FdoException::Create(L"c_KgOraInsert::Execute: Unable to find class definition!");
    
  }

  FdoPtr<FdoKgOraClassDefinition> phys_class = schemadesc->FindClassMapping(m_ClassId);
  FdoStringP fultablename = phys_class->GetOracleFullTableName();
  FdoStringP table_alias = phys_class->GetOraTableAlias();
  
  bool use_seq_for_identity = false;
  FdoPtr<FdoDataPropertyDefinition> ident_for_seq;
  FdoStringP seqname = phys_class->GetUseSequenceForIdentity();
  if( seqname.GetLength() > 0 )
  {
  // take the name of identity property
    FdoPtr<FdoDataPropertyDefinitionCollection> ident_col = classdef->GetIdentityProperties();
    if( ident_col->GetCount() == 1 )
    {
      ident_for_seq = ident_col->GetItem(0);      
      use_seq_for_identity = true;
    }
  }
  
  c_KgOraSridDesc orasrid;
  m_Connection->GetOracleSridDesc(classdef,orasrid);
  
  if( m_BatchParameterValues.p && m_BatchParameterValues->GetCount() != 0 )
  {
  // First I will rearrange property values so, frist will come bacth parametsr
  // and after that other literal values
  // first pick up those property values which are of type paarmeters
    FdoPtr<FdoPropertyValueCollection> batch_propvalcol = FdoPropertyValueCollection::Create();
    
    // add parameters
    unsigned int num_batch_columns=0;
    unsigned int count = m_PropertyValues->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
      FdoPtr<FdoParameter> param = dynamic_cast<FdoParameter*>(propval->GetValue ());
      if( param )
      {
        batch_propvalcol->Add(propval);
        num_batch_columns++;
      }
    }
    
    // Check if this class is using oracle sequence and if it is
    // check if bach parameter is set for it
    // if not than add that parameter
    int index_of_sequence_batch_parameter=-1;
    bool added_new_batch_parameter_for_sequence_identity = false;
    if( use_seq_for_identity )
    {
      unsigned int prop_count = batch_propvalcol->GetCount();
      bool found_identity = false;
      for(unsigned int ind=0;ind<prop_count;ind++)
      { 
        FdoPtr<FdoPropertyValue> propval = batch_propvalcol->GetItem(ind);
        FdoPtr<FdoIdentifier> propid = propval->GetName();
        if( wcscmp(propid->GetName(),ident_for_seq->GetName()) == 0 )
        {
          index_of_sequence_batch_parameter = ind;
          found_identity = true;
          break;
        }
      }
      
      if( !found_identity )
      {
        FdoPtr<FdoParameter> param = FdoParameter::Create(ident_for_seq->GetName());
        FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(ident_for_seq->GetName(),param);
        batch_propvalcol->Insert(0,propval);
        index_of_sequence_batch_parameter = 0;
        added_new_batch_parameter_for_sequence_identity = true;
      }
    }
    
    num_batch_columns = batch_propvalcol->GetCount();
    
    // add non-parameters
    count = m_PropertyValues->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
      FdoPtr<FdoParameter> param = dynamic_cast<FdoParameter*>(propval->GetValue ());
      if( param == NULL )
      {
        batch_propvalcol->Add(propval);
      }
    }
    
    
   
    
    
    
  // then process thos values which    
    c_FilterStringBuffer strbuff;
    c_KgOraExpressionProcessor expproc(&strbuff,schemadesc,m_ClassId,orasrid);
    
    
    FdoStringP colnames;
    FdoStringP colvalues;
    FdoStringP sep;
    
    count = batch_propvalcol->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = batch_propvalcol->GetItem(ind);
      FdoPtr<FdoIdentifier> propid = propval->GetName();
      
      colnames += sep + propid->GetName();
     
      
      FdoPtr<FdoValueExpression> expr = propval->GetValue();
      
      strbuff.ClearBuffer();
      expr->Process( &expproc );
      
      colvalues += sep + strbuff.GetString();
      sep = ",";
    }
    
    c_FilterStringBuffer sqlstr;
    sqlstr.AppendString(L"INSERT INTO ");
    sqlstr.AppendString(fultablename);
    sqlstr.AppendString(L" ( ");
    sqlstr.AppendString(colnames);
    sqlstr.AppendString(L" ) ");
    sqlstr.AppendString(L" VALUES ( ");
    sqlstr.AppendString(colvalues);
    sqlstr.AppendString(L" ) ");
    
    c_Oci_Statement* oci_stm=NULL;
    
    try
    {
      oci_stm = m_Connection->OCI_CreateStatement();
      
      D_KGORA_ELOG_WRITE1("Execute Insert: '%s",sqlstr.GetString());
      
      #ifdef _DEBUG
      #endif
      oci_stm->Prepare(sqlstr.GetString());
      
      unsigned long count = m_BatchParameterValues->GetCount();
      for( unsigned long browind=0;browind<count;browind++)
      {
        // do aply of literal values
        // this apply witll skip parameter values
        expproc.ApplySqlParameters(oci_stm,orasrid.m_IsGeodetic,orasrid.m_OraSrid);
        
        // now i need to aply batch values
        int ora_batch_parameter = 1; // number of parameter in oracle sql statament 
        if( added_new_batch_parameter_for_sequence_identity )
        {
          long seqval = c_Ora_API2::GetSequenceNextVal(m_Connection->GetOciConnection(),seqname);
          FdoPtr<FdoDataValue> dataval = FdoDataValue::Create((FdoInt32)seqval);
          c_FdoOra_API2::SetOracleStatementData( oci_stm,ora_batch_parameter,dataval);
          ora_batch_parameter++;
        }
        
        FdoPtr<FdoParameterValueCollection> bparamcol = m_BatchParameterValues->GetItem(browind);
        if( (bparamcol->GetCount() == num_batch_columns) 
            || 
            (added_new_batch_parameter_for_sequence_identity && ( (bparamcol->GetCount()+1) == num_batch_columns) )
          )
        {
          unsigned long bcount = bparamcol->GetCount();
          for(unsigned long bind=0;bind<bcount;bind++)
          {
            FdoPtr<FdoParameterValue> paramval = bparamcol->GetItem(bind);
            FdoPtr<FdoLiteralValue> lval = paramval->GetValue();
            FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(lval.p);
            if( dataval )
            {
            // if there is identity in batch parameters
            // check if it is a null; then change null to sequence value
              
              if( use_seq_for_identity && !added_new_batch_parameter_for_sequence_identity && (index_of_sequence_batch_parameter==bind) )
              {
                if( dataval->IsNull() )
                {                
                  long seqval = c_Ora_API2::GetSequenceNextVal(m_Connection->GetOciConnection(),seqname);
                  FdoPtr<FdoDataValue> dataval = FdoDataValue::Create((FdoInt32)seqval);
                  c_FdoOra_API2::SetOracleStatementData(oci_stm,ora_batch_parameter,dataval);
                }
                else
                {
                  c_FdoOra_API2::SetOracleStatementData(oci_stm,ora_batch_parameter,dataval);
                }
              }
              else
              {
                c_FdoOra_API2::SetOracleStatementData(oci_stm,ora_batch_parameter,dataval);
              }
            }
            else
            {
              FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(lval.p);
              if( geomval )
              {
                FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();    
                c_SDO_GEOMETRY * sdogeom = c_SDO_GEOMETRY::Create(oci_stm->m_OciConn);
                
                c_FgfToSdoGeom fgftosdo;
                
                if( fgftosdo.ToSdoGeom((int*)fgf->GetData(),orasrid.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
                {
                  oci_stm->BindSdoGeomValue(ora_batch_parameter,sdogeom);
                }
                else
                {
                  throw FdoCommandException::Create( L"Unknown Geometry Type. Unable to convert FGF to SDO geometry!" );    
                }
                
                
              }
              else
              {
                if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
                throw FdoCommandException::Create( L"Unknown parameter batch value type. No data value no geometry value." );    
              }
            }
          
            ora_batch_parameter++;  
          }
          
          int update_num = oci_stm->ExecuteNonQuery();
        }
      }
      m_Connection->OCI_TerminateStatement(oci_stm);
      
      
      // after batch insert reset table sequence if nesseseary
      if( use_seq_for_identity )
      {
        FdoStringP fdostr = ident_for_seq->GetName();
        c_Ora_API2::ResetSequence(m_Connection->GetOciConnection(),seqname,fultablename,fdostr);
      }
      
      
    }
    catch(c_Oci_Exception* ea)
    {
      if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
      FdoStringP gstr = ea->what();
      delete ea;
      throw FdoCommandException::Create( gstr );    
    }
    
    return new c_KgOraFeatureReaderInsert(m_PropertyValues,classdef);
  }
  else
  {
  // Single values insert
    if( m_PropertyValues.p )
    {
      c_FilterStringBuffer strbuff;
      c_KgOraExpressionProcessor expproc(&strbuff,schemadesc,m_ClassId,orasrid,0);
        
      FdoStringP colnames;
      FdoStringP colvalues;
      FdoStringP sep;
      
      
      bool prop_in_seq = false;
      bool used_seq = false;
      
      
      
      // first check if this class is using oracle sequences
      // if so check if identiy is inside properties and check 
      // if value is null than set it to sequence value
      if( use_seq_for_identity )
      {
        unsigned int prop_count = m_PropertyValues->GetCount();
        bool found_identity = false;
        for(unsigned int ind=0;ind<prop_count;ind++)
        { 
          FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
          FdoPtr<FdoIdentifier> propid = propval->GetName();
          if( wcscmp(propid->GetName(),ident_for_seq->GetName()) == 0 )
          {
            long seqval = c_Ora_API2::GetSequenceNextVal(m_Connection->GetOciConnection(),seqname);
            FdoPtr<FdoDataValue> newval = FdoDataValue::Create((FdoInt32)seqval);
            propval->SetValue(newval);
            found_identity = true;
            break;
          }
        }
        
        if( !found_identity )
        {
          
          long seqval = c_Ora_API2::GetSequenceNextVal(m_Connection->GetOciConnection(),seqname);
          FdoPtr<FdoDataValue> newval = FdoDataValue::Create((FdoInt32)seqval);
          
          FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(ident_for_seq->GetName(),newval);
          m_PropertyValues->Insert(0,propval);

        }
      }
      
      
      unsigned int prop_count = m_PropertyValues->GetCount();
      for(unsigned int ind=0;ind<prop_count;ind++)
      {
        FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
        FdoPtr<FdoIdentifier> propid = propval->GetName();
        
        colnames += sep + propid->GetName();

        FdoPtr<FdoValueExpression> expr = propval->GetValue();
        
        
        prop_in_seq = false;
        if( use_seq_for_identity && !used_seq)
        {

          
          if( wcscmp(propid->GetName(),ident_for_seq->GetName()) == 0 )
          {
          // if it is data val end if it is null put sequence inot it 
            FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(expr.p);
            if( dataval && dataval->IsNull() )
            {
             
              colvalues += sep + seqname + L".nextval";
              used_seq = true;
              prop_in_seq = true;
            }
            
          }
          else
          {

            FdoStringP str = propid->GetName();
            
          }
        }
        
        if( !prop_in_seq )
        {
          strbuff.ClearBuffer();
          expr->Process( &expproc );
          colvalues += sep + strbuff.GetString();              
        }
        
        sep = ",";
      }
      
      c_FilterStringBuffer sqlstr;
      sqlstr.AppendString(L"INSERT INTO ");
      sqlstr.AppendString(fultablename);
      sqlstr.AppendString(L" ( ");
      sqlstr.AppendString(colnames);
      sqlstr.AppendString(L" ) ");
      sqlstr.AppendString(L" VALUES ( ");
      sqlstr.AppendString(colvalues);
      sqlstr.AppendString(L" ) ");
      
      c_Oci_Statement* oci_stm=NULL;
      
      try
      {
        oci_stm = m_Connection->OCI_CreateStatement();
        
        #ifdef _KGORA_EXTENDED_LOG
          FdoStringP s1 = sqlstr.GetString();
          D_KGORA_ELOG_WRITE1("Execute Insert: '%s'",(const char*)s1);
        #endif
        
        oci_stm->Prepare(sqlstr.GetString());
        
        expproc.ApplySqlParameters(oci_stm,orasrid.m_IsGeodetic,orasrid.m_OraSrid);
        

        int update_num = oci_stm->ExecuteNonQuery();
        
        
        if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
        
      }
      catch(c_Oci_Exception* ea)
      {
        if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
        FdoStringP gstr = ea->what();
        
        D_KGORA_ELOG_WRITE2("c_KgOraInsert::Execute%d Exception '%s'",m_Connection->m_ConnNo,(const char*)gstr);
        
        delete ea;
        throw FdoCommandException::Create( gstr );    
      }

      
      return new c_KgOraFeatureReaderInsert(m_PropertyValues,classdef);
      
    }
  }
  
  
  return NULL;
}//end of c_KgOraInsert::Execute

