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
#include "c_KgInfInsert.h"

c_KgInfInsert::c_KgInfInsert(c_KgInfConnection *Conn)
  : c_KgInfFdoCommand(Conn)
{
}

c_KgInfInsert::~c_KgInfInsert(void)
{
}

/// \brief
/// Gets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \return
/// Returns the class name.
/// 
FdoIdentifier* c_KgInfInsert::GetFeatureClassName()
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
void c_KgInfInsert::SetFeatureClassName(FdoIdentifier* ClassId)
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
void c_KgInfInsert::SetFeatureClassName(FdoString* ClassName)
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
FdoPropertyValueCollection* c_KgInfInsert::GetPropertyValues()
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
FdoBatchParameterValueCollection* c_KgInfInsert::GetBatchParameterValues()
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
FdoIFeatureReader* c_KgInfInsert::Execute()
{
  FdoPtr<c_KgInfSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(m_ClassId);
  if( !classdef.p ) 
  {
    throw FdoException::Create(L"c_KgInfInsert::Execute: Unable to find class definition!");
    
  }

  FdoPtr<c_KgInfOverClassDefinition> phys_class = schemadesc->FindClassMapping(m_ClassId);
  FdoStringP fultablename = phys_class->GetSqlQueryFullName();
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
  
  c_KgInfSridDesc orasrid;
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
    c_KgInfExpressionProcessor expproc(&strbuff,schemadesc,m_ClassId,orasrid);
    
    
    FdoStringP colnames;
    FdoStringP colvalues;
    FdoStringP sep;
    
    FdoPtr<FdoParameterValueCollection> bparamvaluecoll = m_BatchParameterValues->GetItem(0);
    count = batch_propvalcol->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = batch_propvalcol->GetItem(ind);
      FdoPtr<FdoIdentifier> propid = propval->GetName();
      
      colnames += sep + propid->GetName();
     
      
      FdoPtr<FdoValueExpression> expr = propval->GetValue();
      
      FdoPtr<FdoParameterValue> bparamval = bparamvaluecoll->GetItem(ind);
      
      FdoPtr<FdoLiteralValue> literalval = bparamval->GetValue();
      
      strbuff.ClearBuffer();
      //expr->Process( &expproc );
      literalval->Process(&expproc);
      
      colvalues += sep + strbuff.GetString();
      sep = ",";
    }
    
    c_FilterStringBuffer sqlstr;
    sqlstr.AppendString("INSERT INTO ");
    sqlstr.AppendString(fultablename);
    sqlstr.AppendString(" ( ");
    sqlstr.AppendString(colnames);
    sqlstr.AppendString(" ) ");
    sqlstr.AppendString(" VALUES ( ");
    sqlstr.AppendString(colvalues);
    sqlstr.AppendString(" ) ");
    
    otl_stream* stream=NULL;
    
    try
    {
    
      
      
      //m_Connection->OTL_SetCommit(stream,0);
      
      unsigned long count = m_BatchParameterValues->GetCount();
      for( long browind=0;browind<count;browind++)
      {
        stream = m_Connection->OTL_CreateStream(1,sqlstr.GetString()); // if this create-delet is out of for loop then otl/sql server hangs after inserting first row - didn't investigated further
        
        // do aply of literal values
        // this apply witll skip parameter values
        //expproc.ApplySqlParameters(stream);
        
        // now i need to aply batch values
        FdoPtr<FdoParameterValueCollection> bparamcol = m_BatchParameterValues->GetItem(browind);
        if( bparamcol->GetCount() == num_batch_columns )
        {
           long bcount = bparamcol->GetCount();
          for(long bind=0;bind<bcount;bind++)
          {
            FdoPtr<FdoParameterValue> paramval = bparamcol->GetItem(bind);
            FdoPtr<FdoLiteralValue> lval = paramval->GetValue();
            FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(lval.p);
            if( dataval )
            {
              c_FdoInf_API::SetStreamData(stream,bind+1,dataval);
            }
            else
            {
              FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(lval.p);
              if( geomval )
              {
                FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
                //simon
                c_FdoInf_API::SetStreamDataGeometryWkb(stream,bind+1,fgf);
                //c_FdoInf_API::SetStreamDataGeometryWkt(stream,bind+1,fgf);
              }
              else
              {
                if( stream ) m_Connection->OTL_DeleteStream(stream);
                throw FdoCommandException::Create( L"Unknown parameter batch value type. No data value no geometry value." );    
              }
            }
            
          }
          //stream->clean();
          //m_Connection->OTL_FlushStream(stream);    
          //int update_num = occi_stm->executeUpdate();
          //int update_num = m_Connection->OTL_GetRpc(stream);    
        }
        
        m_Connection->OTL_Commit();
        m_Connection->OTL_DeleteStream(stream);  
      }
      
      
      
      //m_Connection->OTL_FlushStream(stream);
      
      
      
      // after batch insert reset table sequence if nesseseary
      if( use_seq_for_identity )
      {
        FdoStringP fdostr = ident_for_seq->GetName();
        //c_Ora_API::ResetSequence(m_Connection->GetOcciConnection(),seqname,fultablename,fdostr);
      }
      
      
    }
    catch(c_KgOtlException& ea)
    {
      if( stream ) m_Connection->OTL_DeleteStream(stream);
      
      //FdoStringP gstr = ea.what.c_str();
      throw FdoCommandException::Create( ea.what() );    
    }
    
    return new c_KgInfFeatureReaderInsert(m_PropertyValues,classdef);
  }
  else
  {
  //////////////////////////////////////
  ///
  //        Single row insert
  ///
  //////////////////////////////////////
    FdoPtr<FdoPropertyValueCollection> insertedvalues_coll;
    
    if( m_PropertyValues.p )
    {
      c_FilterStringBuffer strbuff;
      c_KgInfExpressionProcessor expproc(&strbuff,schemadesc,m_ClassId,orasrid);
        
      FdoStringP colnames;
      FdoStringP colvalues;
      FdoStringP sep;
      
      
      bool prop_in_seq = false;
      bool used_seq = false;
      
      
      // Check if class has an identity
      // identity is recogines as AutoGenerated = true
      bool is_identity=false;
      FdoDataPropertyDefinition* prop_identity=NULL;
      {
        FdoPtr<FdoDataPropertyDefinitionCollection> ident_col = classdef->GetIdentityProperties();
        if( ident_col->GetCount() == 1 )
        {
          prop_identity = ident_col->GetItem(0);   
          if( prop_identity->GetIsAutoGenerated() )
            is_identity = true;
        }
      }
      
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
            //long seqval = c_Ora_API::GetSequenceNextVal(m_Connection->GetOcciConnection(),seqname);
            long seqval = 0;
            FdoPtr<FdoDataValue> newval = FdoDataValue::Create((FdoInt32)seqval);
            propval->SetValue(newval);
            found_identity = true;
            break;
          }
        }
        
        if( !found_identity )
        {
          
          //long seqval = c_Ora_API::GetSequenceNextVal(m_Connection->GetOcciConnection(),seqname);
          long seqval = 0;
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
          expr->Process(&expproc);
          colvalues += sep + strbuff.GetString();              
        }
        
        sep = ",";
      }
      
      c_FilterStringBuffer sqlstr;
      
      sqlstr.AppendString("INSERT INTO ");
      sqlstr.AppendString(fultablename);
      sqlstr.AppendString(" ( ");
      sqlstr.AppendString(colnames);
      sqlstr.AppendString(" ) ");
      sqlstr.AppendString(" VALUES ( ");
      sqlstr.AppendString(colvalues);
      sqlstr.AppendString(" ); ");
      
    
      
      
      otl_stream* stream=NULL;
      
      try
      {
        
        
        D_KGORA_ELOG_WRITE1("Execute Insert: '%s",sqlstr.GetString());
        
        
        
        stream = m_Connection->OTL_CreateStream(1,sqlstr.GetString());
        
        
        
        expproc.ApplySqlParameters(stream);
       
       int update_num = m_Connection->OTL_GetRpc(stream);
        
        stream->close();
        
      // I need to create copy of this property values
        
        {
          insertedvalues_coll = FdoPropertyValueCollection::Create();
          for(int ind=0;ind<m_PropertyValues->GetCount();ind++)
          {
            FdoPtr<FdoPropertyValue> exval = m_PropertyValues->GetItem(ind);
            
            FdoPtr<FdoPropertyValue> newval = FdoPropertyValue::Create(exval->GetName(),exval->GetValue());
            insertedvalues_coll ->Add( newval );
          }
        }
      // check if need to read new identity from database        
        if( is_identity )
        {
          
          otl_stream* stream = m_Connection->OTL_CreateStream(1,"SELECT @@identity");
          
          if( !stream->eof() )
          {
            int idval;
            *stream >> idval;
          
            {
              unsigned int prop_count = insertedvalues_coll->GetCount();
              bool found_identity = false;
              for(unsigned int ind=0;ind<prop_count;ind++)
              { 
                FdoPtr<FdoPropertyValue> propval = insertedvalues_coll->GetItem(ind);
                FdoPtr<FdoIdentifier> propid = propval->GetName();
                if( wcsicmp(propid->GetName(),prop_identity->GetName()) == 0 )
                {
                  FdoPtr<FdoDataValue> newval = FdoDataValue::Create((FdoInt32)idval);
                  propval->SetValue(newval);
                  found_identity = true;
                  break;
                }
              }
              
              if( !found_identity )
              {
                
                FdoPtr<FdoDataValue> newval = FdoDataValue::Create((FdoInt32)idval);
                //propval->SetValue(newval);
                FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(prop_identity->GetName(),newval);
                insertedvalues_coll->Add( propval );
              }  
            }
          }
          //if( stream2 ) m_Connection->OTL_DeleteStream(stream2);
        }

        //int update_num = occi_stm->executeUpdate();
        
        m_Connection->OTL_Commit();
        if( stream ) m_Connection->OTL_DeleteStream(stream);
        
        
      }
      catch(c_KgOtlException& ea)
      {
        m_Connection->OTL_DeleteStream(stream);
        
        throw FdoCommandException::Create( ea.what() );    
      }

      
      // I need to create copy of this property values
      {
        FdoPtr<FdoPropertyValueCollection> insertedvalues_coll = FdoPropertyValueCollection::Create();
        for(int ind=0;ind<m_PropertyValues->GetCount();ind++)
        {
          FdoPtr<FdoPropertyValue> exval = m_PropertyValues->GetItem(ind);
          
          FdoPtr<FdoPropertyValue> newval = FdoPropertyValue::Create(exval->GetName(),exval->GetValue());
          insertedvalues_coll ->Add( newval );
        }
      }
      
      return new c_KgInfFeatureReaderInsert(insertedvalues_coll,classdef);
      
    }
  }
  
  
  return NULL;
}//end of c_KgInfInsert::Execute

