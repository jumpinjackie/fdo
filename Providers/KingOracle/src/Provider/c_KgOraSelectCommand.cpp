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


#include "stdafx.h"
#include "c_KgOraSelectCommand.h"
#include "c_KgOraFeatureReader.h"
#include "c_KgOraFilterProcessor.h"

#include "c_LogAPI.h"

#include <malloc.h>

c_KgOraSelectCommand::c_KgOraSelectCommand (c_KgOraConnection* Conn) 
    : c_KgOraFdoFeatureCommand<FdoISelect> (Conn)
{
  m_OrderingOption = FdoOrderingOption_Ascending;
}



c_KgOraSelectCommand::~c_KgOraSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void c_KgOraSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* c_KgOraSelectCommand::GetPropertyNames ()
{
    if (m_PropertyNames == NULL)
        m_PropertyNames = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(m_PropertyNames.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType c_KgOraSelectCommand::GetLockType ()
{
    throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockType"));
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraSelectCommand::SetLockType (FdoLockType value)
{
    throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockType"));
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy c_KgOraSelectCommand::GetLockStrategy ()
{
    throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockStrategy"));
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockStrategy"));
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgOraSelectCommand::Execute ()
{
 
 int propcount = -1; 
 if( m_PropertyNames.p )
   propcount = m_PropertyNames->GetCount();
  
    FdoPtr<FdoIdentifier> classid = GetFeatureClassName ();
    FdoString* class_name = classid->GetText ();
    
    FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
    
    D_KGORA_ELOG_WRITE2("c_KgOraSelectCommand%d::Execute class_name = '%s'",m_Connection->m_ConnNo,(const char*)FdoStringP(class_name));
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
    
    FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
    if( !classdef.p ) return NULL;
    
    int geom_sqlcol_index;
    FdoPtr<FdoStringCollection> sqlcols = FdoStringCollection::Create();
    
    c_KgOraSridDesc orasrid;
    {
      FdoPtr<FdoPropertyDefinition> propdef;        
      FdoPtr<FdoPropertyDefinitionCollection> propcol = classdef->GetProperties();
      int count = propcol->GetCount();
      for( int ind = 0; ind < count; ind++ )
      {
        propdef = propcol->GetItem(ind);
        FdoString* propname = propdef->GetName();
              
        if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
        {
          FdoGeometricPropertyDefinition* geomprop = (FdoGeometricPropertyDefinition*)propdef.p;
          
          m_Connection->GetOracleSridDesc(geomprop,orasrid);
          
          
        }
      }
    }
    
    D_KGORA_ELOG_WRITE2("c_KgOraSelectCommand%d::Execute class_name = '%s' Step 2",m_Connection->m_ConnNo,(const char*)FdoStringP(class_name));
    
    c_KgOraFilterProcessor fproc(schemadesc,classid,orasrid);
    string sqlstr = CreateSqlString(fproc,geom_sqlcol_index,sqlcols);
    
    D_KGORA_ELOG_WRITE2("c_KgOraSelectCommand%d::Execute class_name = '%s' Step 3",m_Connection->m_ConnNo,(const char*)FdoStringP(class_name));
    
    oracle::occi::Statement* occi_stm=NULL;
    oracle::occi::ResultSet* occi_rset=NULL;
    try
    {
      
      #ifdef _DEBUG
        const char* c1 = sqlstr.c_str();
      #endif
      D_KGORA_ELOG_WRITE1("Execute select: '%s",sqlstr.c_str());
      
      occi_stm = m_Connection->OCCI_CreateStatement();
      
      occi_stm->setSQL(sqlstr);
      occi_stm->setPrefetchRowCount(400);
      occi_stm->setPrefetchMemorySize(64*1024);
      
      fproc.GetExpressionProcessor().ApplySqlParameters(m_Connection->GetOcciEnvironment(), occi_stm);
      
      occi_rset = occi_stm->executeQuery();
      //m_Connection->OCCI_TerminateStatement(occi_stm);
    }
    catch(oracle::occi::SQLException& ea)
    {
       const char* what = ea.what();
    
        #ifdef _DEBUG
          printf(" <c_KgOraSelectCommand::Execute Exception> ");
        #endif
        
        D_KGORA_ELOG_WRITE2("c_KgOraSelectCommand::Execute%d Exception '%s'",m_Connection->m_ConnNo,what);
    
      if (occi_stm && occi_rset)
      {
        occi_stm->closeResultSet(occi_rset);        
        occi_rset = NULL;
      }

      if (occi_stm)
      {
        m_Connection->OCCI_TerminateStatement(occi_stm);
        occi_stm=NULL;
      }
      occi_rset = NULL;
      FdoStringP gstr = ea.what();
      throw FdoCommandException::Create( gstr );    
    }

    
    return new c_KgOraFeatureReader(m_Connection,occi_stm, occi_rset,classdef,geom_sqlcol_index,sqlcols, m_PropertyNames);
    
}//end of c_KgOraSelectCommand::Execute 

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgOraSelectCommand::ExecuteWithLock ()
{
  throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"ExecuteWithLock"));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* c_KgOraSelectCommand::GetLockConflicts ()
{
    throw FdoCommandException::Create (NlsMsgGetKgOra (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockConflicts"));
}


string c_KgOraSelectCommand::CreateSqlString(c_KgOraFilterProcessor& FilterProc,int& GeomSqlColumnIndex,FdoStringCollection* SqlColumns)
{
    FdoPtr<FdoIdentifier> classid = GetFeatureClassName ();
    FdoString* class_name = classid->GetText ();
    
    FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    //FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
    
    FdoPtr<FdoClassDefinition> classdef;
    if( fschemas && (fschemas->GetCount() > 0) )
    {
      //FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
      //FdoPtr<FdoClassCollection> classes = schm->GetClasses();
      //classdef = classes->FindItem( classid->GetName() );
      
      FdoPtr<FdoIDisposableCollection> classes = fschemas->FindClass(classid->GetText());
      classdef = (FdoClassDefinition*)classes->GetItem( 0 );
      
      
      if( !classdef.p ) return "";
          
    }

    string sqlstr;
    
    FdoPtr<FdoKgOraClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
    FdoStringP fultablename = phys_class->GetOracleFullTableName();
    FdoStringP table_alias = phys_class->GetOraTableAlias();
    
    /* Define properties to be included in SELECT statement */    
    FdoPtr<FdoPropertyDefinition> propdef;
    FdoStringP sql_select_columns_part;
    FdoStringP sep;
    
    GeomSqlColumnIndex=-1;
    FdoPtr<FdoPropertyDefinitionCollection> propcol = classdef->GetProperties();

    c_KgOraSridDesc orasrid;
    int count = propcol->GetCount();
    for( int ind = 0; ind < count; ind++ )
    {
      propdef = propcol->GetItem(ind);
      FdoString* propname = propdef->GetName();
      
      
      SqlColumns->Add(propname);
      if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
      {
        FdoGeometricPropertyDefinition* geomprop = (FdoGeometricPropertyDefinition*)propdef.p;
        
        m_Connection->GetOracleSridDesc(geomprop,orasrid);
        
        GeomSqlColumnIndex=ind;
        
        if( phys_class->GetIsPointGeometry() && (FdoCommonOSUtil::wcsicmp(propname,phys_class->GetPoinGeometryPropertyName())==0) )
        {
        // this is geometry created as point from numeric columns
          
          FdoStringP pointstr;
          if( phys_class->GetPointZOraColumn() && (wcslen(phys_class->GetPointZOraColumn()) > 0) )
            pointstr = pointstr.Format(L" SDO_GEOMETRY(2001,NULL,SDO_POINT_TYPE(%s,%s,%s),NULL,NULL) as %s ",phys_class->GetPointXOraColumn(),phys_class->GetPointYOraColumn(),phys_class->GetPointZOraColumn(),propname);
          else
            pointstr = pointstr.Format(L" SDO_GEOMETRY(2001,NULL,SDO_POINT_TYPE(%s,%s,NULL),NULL,NULL) as %s ",phys_class->GetPointXOraColumn(),phys_class->GetPointYOraColumn(),propname);
                    
          sql_select_columns_part += sep + pointstr;  // this is for just column -> sql_select_columns_part += sep + table_alias + "." + propname;  
          sep = ",";
        }
        else
        {
        // this is normal geomerty property - oracle column
        // add just property name in select
          sql_select_columns_part += sep + table_alias + "." + propname;  sep = ",";
        }
      }
      else
      {
      // add property name in select
        sql_select_columns_part += sep + table_alias + "." + propname;  sep = ",";
      }

    }
     
    //string wherestr;
    //CreateFilterSqlString(m_Filter,wherestr);
    
    const char* filtertext=NULL;
    
    if( m_Filter )
    {      
      m_Filter->Process( &FilterProc );
    
      filtertext = FilterProc.GetFilterText();
    }
    else filtertext = NULL;

    
    
    char* sbuff = new char[1024];
    sprintf(sbuff, "SELECT %s FROM %s %s",(const char*)sql_select_columns_part,(const char*)fultablename,(const char*)table_alias);
    
    
    sqlstr = sbuff;
    if( filtertext && *filtertext )
    {
      sqlstr += " WHERE ";
      //sqlstr += wherestr;
      sqlstr += filtertext;
    }
    
    delete sbuff;
    
    FdoPtr<FdoIdentifierCollection> order_ident_col = GetOrdering();
    long order_count = order_ident_col->GetCount();
    if( order_count > 0 )
    {
      string sep;
      sqlstr += " ORDER BY ";
      for(long ind=0; ind<order_count; ind++)
      {
        FdoPtr<FdoIdentifier> order_ident = order_ident_col->GetItem(ind);
        FdoStringP fdostr = order_ident->GetName();
        sqlstr += sep + (const char*)fdostr;
        if( GetOrderingOption() == FdoOrderingOption_Ascending )
        {
          sqlstr += " ASC ";
        }
        else
        {
          sqlstr += " DESC ";
        }
        sep = ",";
      }
    }
    
    // Test vremena za fetch iz oracle
    #ifdef _DEBUG
      //TestArrayFetch(ClassId, Filter, Props);
    #endif
    
    
    return sqlstr;
}//end of c_KgOraSelectCommand::CreateSqlString



// just check for spatial filter
// other filters ignored
void c_KgOraSelectCommand::CreateFilterSqlString(FdoFilter* Filter,string& WhereBuff)
{
  
    if( !Filter ) return;
  
    FdoSpatialCondition* spatial_filter = NULL;
    
    
    FdoString* testsql = Filter->ToString();        
    
    spatial_filter = dynamic_cast<FdoSpatialCondition*>(Filter);
    if( !spatial_filter ) return;
    
        
    if (spatial_filter->GetOperation() == FdoSpatialOperations_EnvelopeIntersects)
    {
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"MDSYS.SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            // TODO: for envelope interscet just use primary filter SDO_FILTER and compare perfomance
            //sprintf(sbuff,"SDO_FILTER(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete sbuff;
            delete sbuff2;

        }
    }
    else if (spatial_filter->GetOperation() == FdoSpatialOperations_Intersects)
    {
        
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();
            
            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete sbuff;
            delete sbuff2;
        }
    }
    


}//end of c_KgOraSelectCommand::CreateFilterSqlString


/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* c_KgOraSelectCommand::GetOrdering()
{ 
  if( m_OrderingIdentifiers.p == NULL )
    m_OrderingIdentifiers = FdoIdentifierCollection::Create();
    
  return FDO_SAFE_ADDREF(m_OrderingIdentifiers.p); 
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraSelectCommand::SetOrderingOption( FdoOrderingOption  Option ) 
{
  m_OrderingOption = Option;
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption c_KgOraSelectCommand::GetOrderingOption( )
{ 
  return m_OrderingOption; 
}
