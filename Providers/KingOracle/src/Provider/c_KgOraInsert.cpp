#include "StdAfx.h"
#include "c_KgOraInsert.h"
#include "c_FdoOra_API.h"
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
    sqlstr.AppendString("INSERT INTO ");
    sqlstr.AppendString(fultablename);
    sqlstr.AppendString(" ( ");
    sqlstr.AppendString(colnames);
    sqlstr.AppendString(" ) ");
    sqlstr.AppendString(" VALUES ( ");
    sqlstr.AppendString(colvalues);
    sqlstr.AppendString(" ) ");
    
    oracle::occi::Statement* occi_stm=NULL;
    
    try
    {
      occi_stm = m_Connection->OCCI_CreateStatement();
      
      D_KGORA_ELOG_WRITE1("Execute Insert: '%s",sqlstr.GetString());
      
      #ifdef _DEBUG
      #endif
      occi_stm->setSQL(sqlstr.GetString());
      
      unsigned long count = m_BatchParameterValues->GetCount();
      for( long browind=0;browind<count;browind++)
      {
        // do aply of literal values
        // this apply witll skip parameter values
        expproc.ApplySqlParameters(occi_stm);
        
        // now i need to aply batch values
        FdoPtr<FdoParameterValueCollection> bparamcol = m_BatchParameterValues->GetItem(browind);
        if( bparamcol->GetCount() == num_batch_columns )
        {
          unsigned long bcount = bparamcol->GetCount();
          for(long bind=0;bind<bcount;bind++)
          {
            FdoPtr<FdoParameterValue> paramval = bparamcol->GetItem(bind);
            FdoPtr<FdoLiteralValue> lval = paramval->GetValue();
            FdoDataValue* dataval = dynamic_cast<FdoDataValue*>(lval.p);
            if( dataval )
            {
              c_FdoOra_API::SetOracleStatementData(occi_stm,bind+1,dataval);
            }
            else
            {
              FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(lval.p);
              if( geomval )
              {
                FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();    
                SDO_GEOMETRY * sdogeom = new SDO_GEOMETRY;
                
                c_FgfToSdoGeom fgftosdo;
                
                if( fgftosdo.ToSdoGeom((int*)fgf->GetData(),orasrid.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
                {
                  occi_stm->setObject(bind+1,sdogeom);
                }
                
                delete sdogeom;
              }
              else
              {
                throw FdoCommandException::Create( L"Unknown parameter batch value type. No data value no geometry value." );    
              }
            }
            
          }
          
          int update_num = occi_stm->executeUpdate();
        }
      }
      
      m_Connection->OCCI_TerminateStatement(occi_stm);
      m_Connection->OCCI_Commit();
    }
    catch(oracle::occi::SQLException& ea)
    {
      if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
      FdoStringP gstr = ea.getMessage().c_str();
      throw FdoCommandException::Create( gstr );    
    }

    
    return new c_KgOraFeatureReaderInsert(m_PropertyValues,classdef);
  }
  else
  {
  if( m_PropertyValues.p )
  {
    c_FilterStringBuffer strbuff;
    c_KgOraExpressionProcessor expproc(&strbuff,schemadesc,m_ClassId,orasrid);
    
    
    FdoStringP colnames;
    FdoStringP colvalues;
    FdoStringP sep;
    
    unsigned int count = m_PropertyValues->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
      FdoPtr<FdoIdentifier> propid = propval->GetName();
      
      colnames += sep + propid->GetName();
     
      
      FdoPtr<FdoValueExpression> expr = propval->GetValue();
      
      strbuff.ClearBuffer();
      expr->Process( &expproc );
      
      
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
    
    oracle::occi::Statement* occi_stm=NULL;
    
    try
    {
      occi_stm = m_Connection->OCCI_CreateStatement();
      
      D_KGORA_ELOG_WRITE1("Execute Insert: '%s",sqlstr.GetString());
      
      #ifdef _DEBUG
      #endif
      occi_stm->setSQL(sqlstr.GetString());
      
      expproc.ApplySqlParameters(occi_stm);
      

      int update_num = occi_stm->executeUpdate();
      
      m_Connection->OCCI_TerminateStatement(occi_stm);
      m_Connection->OCCI_Commit();
    }
    catch(oracle::occi::SQLException& ea)
    {
      if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
      FdoStringP gstr = ea.getMessage().c_str();
      throw FdoCommandException::Create( gstr );    
    }

    
    return new c_KgOraFeatureReaderInsert(m_PropertyValues,classdef);
    
  }
  }
  
  
  return NULL;
}//end of c_KgOraInsert::Execute

