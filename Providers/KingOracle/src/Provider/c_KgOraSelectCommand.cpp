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

#include "c_LogApi.h"

#include <malloc.h>

c_KgOraSelectCommand::c_KgOraSelectCommand (c_KgOraConnection* Conn) 
    : c_KgOraFdoFeatureCommand<FdoISelect> (Conn)
{
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
    throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockType"));
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraSelectCommand::SetLockType (FdoLockType value)
{
    throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockType"));
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy c_KgOraSelectCommand::GetLockStrategy ()
{
    throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockStrategy"));
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockStrategy"));
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgOraSelectCommand::Execute ()
{
 
    FdoPtr<FdoIdentifier> classid = GetFeatureClassName ();
    FdoString* class_name = classid->GetText ();
    
    FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
    
    D_KGORA_ELOG_WRITE1("c_KgOraSelectCommand::Execute class_name = '%s'",(const char*)FdoStringP(class_name));
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
    
    FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
    if( !classdef.p ) return NULL;
    
    int geom_sqlcol_index;
    FdoPtr<FdoStringCollection> sqlcols = GisStringCollection::Create();
    
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
    
    c_KgOraFilterProcessor fproc(schemadesc,classid,orasrid);
    string sqlstr = CreateSqlString(fproc,geom_sqlcol_index,sqlcols);
    
    oracle::occi::Statement* occi_stm=NULL;
    oracle::occi::ResultSet* occi_rset=NULL;
    try
    {
      occi_stm = m_Connection->OCCI_CreateStatement();
      
      #ifdef _DEBUG
        const char* c1 = sqlstr.c_str();
      #endif
      D_KGORA_ELOG_WRITE1("Execute select: '%s",sqlstr.c_str());
      
      occi_stm->setSQL(sqlstr);
      occi_stm->setPrefetchRowCount(200);
      
      fproc.GetExpressionProcessor().ApplySqlParameters(occi_stm);
      
      occi_rset = occi_stm->executeQuery();
    }
    catch(oracle::occi::SQLException& ea)
    {
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
  throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"ExecuteWithLock"));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* c_KgOraSelectCommand::GetLockConflicts ()
{
    throw FdoCommandException::Create (NlsMsgGet (M_KGORA_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockConflicts"));
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
      FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
      FdoPtr<FdoClassCollection> classes = schm->GetClasses();
      
      classdef = classes->FindItem( classid->GetName() );
      
      if( !classdef.p ) return "";
          
    }

    string sqlstr;
    
    /* Define properties to be included in SELECT statement */    
    FdoPtr<FdoPropertyDefinition> propdef;
    
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

    
    FdoPtr<FdoKgOraClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
    FdoStringP fultablename = phys_class->GetOracleFullTableName();
    FdoStringP table_alias = phys_class->GetOraTableAlias();
    
    FdoStringP cols;
    FdoStringP sep;

    for ( int i = 0; i < SqlColumns->GetCount(); i++ ) 
    {
        cols += sep + table_alias + "." + SqlColumns->GetString(i);
        sep = ",";
    }
    
    
    char* sbuff = new char[1024];
    sprintf(sbuff, "SELECT %s FROM %s %s",(const char*)cols,(const char*)fultablename,(const char*)table_alias);
    
    
    sqlstr = sbuff;
    if( filtertext && *filtertext )
    {
      sqlstr += " WHERE ";
      //sqlstr += wherestr;
      sqlstr += filtertext;
    }
    
    delete sbuff;
    
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