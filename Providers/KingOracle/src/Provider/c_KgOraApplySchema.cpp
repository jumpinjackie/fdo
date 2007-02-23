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
#include "StdAfx.h"
#include "c_KgOraApplySchema.h"
#include "c_FdoOra_API.h"
#include "c_Ora_API.h"

c_KgOraApplySchema::c_KgOraApplySchema(c_KgOraConnection *Conn)
  : c_KgOraFdoCommand(Conn)
{
  m_IngoreStates = true;
}

c_KgOraApplySchema::~c_KgOraApplySchema(void)
{
}


/// \brief
/// Gets the name of the schema to create.
/// 
/// \return
/// Returns the name of the schema to create
/// 
FdoFeatureSchema* c_KgOraApplySchema::GetFeatureSchema()
{
  return FDO_SAFE_ADDREF(m_FeatureSchema.p);
}

/// \brief
/// Sets the name of the schema to create.
/// 
/// \param value 
/// Input the name of the schema to create
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraApplySchema::SetFeatureSchema(FdoFeatureSchema* Schema)
{
  m_FeatureSchema = Schema;
  FDO_SAFE_ADDREF(m_FeatureSchema.p);
}

/// \brief
///  Gets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.
/// 
/// \return
/// Returns FdoPhysicalSchemaMapping
/// 
FdoPhysicalSchemaMapping* c_KgOraApplySchema::GetPhysicalMapping()
{
  return FDO_SAFE_ADDREF(m_PhysMapping.p);
}

/// \brief
/// Sets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.
/// 
/// \param value 
/// Input the FdoPhysicalSchemaMapping
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraApplySchema::SetPhysicalMapping(FdoPhysicalSchemaMapping* PhysMap)
{
  m_PhysMapping = PhysMap;
  FDO_SAFE_ADDREF(m_PhysMapping.p);
}

/// \brief
///  Indicates whether Execute() will ignore element states 
/// when applying the feature schema.
/// 
/// \return
/// Returns true if elements states will be ignored, false otherwise. 
/// 
///  
FdoBoolean c_KgOraApplySchema::GetIgnoreStates()
{
  return m_IngoreStates;
}

/// \brief
/// Changes the handling of element states by Execute().
/// 
/// \param ignoreStates 
/// Input True: Execute() will ignore the 
/// element states on the Feature Schema elements. Instead, it 
/// will additively merge the Feature Schema into the current DataStore.
/// False: Execute() will use the element states to determine whether
/// each element is inserted, modified or deleted.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraApplySchema::SetIgnoreStates( FdoBoolean IgnoreStates )
{
  m_IngoreStates = IgnoreStates;
}



/// \brief
/// Executes the ApplySchema command that creates metadata and physical
/// storage for the schema. An exception is thrown if the schema already
/// exists or if a schema already exists and the feature provider only
/// supports a single schema. If schema changes include deletion of classes
/// or the schema itself, then those classes (or the schema, if it is marked
/// as deleted) must not contain any instance data. If they do, an exception
/// will be thrown.
/// Implementors must call FdoFeatureSchema::AcceptChanges() when Execute() succeeds.
/// It must be called after the schema has been successfully applied, in
/// order to synchronize the states of the schema elements.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraApplySchema::Execute()
{
  if( !m_IngoreStates ) 
    throw FdoException::Create(L"c_KgOraApplySchema::Execute Supports only IgnoreStates='true'");
    
  if( !m_FeatureSchema.p ) return;
 
oracle::occi::Statement* occi_stm=NULL;
//oracle::occi::ResultSet* occi_rset=NULL;    

//    
try
{  
  occi_stm = m_Connection->OCCI_CreateStatement();
  
  FdoPtr<FdoClassCollection> classes = m_FeatureSchema->GetClasses();
  int classcount = classes->GetCount();
  for(int classind=0;classind<classcount;classind++)
  {
    FdoStringP sql_cols;
    FdoStringP sql_create;
    FdoPtr<FdoClassDefinition> classdef = classes->GetItem(classind);
 
  // for the class properties need to convert to oracle types
    sql_cols = "";
    FdoPtr<FdoPropertyDefinitionCollection> props = classdef->GetProperties();
    if( props.p && props->GetCount() )
    {
      
      FdoStringP sep;
      FdoStringP proporatype;
      int propcount = props->GetCount();
      for(int propind=0;propind<propcount;propind++)
      {
        FdoPtr<FdoPropertyDefinition> propdef = props->GetItem(propind);
        
        proporatype = "";
        if( !c_FdoOra_API::FdoPropertyToOraDataType(propdef.p,proporatype) )
        {
          if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
          throw FdoCommandException::Create( L"c_KgOraApplySchema::Execute: Unkown Property Definition ");  
        }
        
        sql_cols = sql_cols + sep + propdef->GetName() + " " + proporatype;
        
        sep = ",";
      }
    // create sql for creating the table  
      FdoStringP tablename = classdef->GetName();
      
      
      sql_create = sql_create + "CREATE TABLE " + tablename + " ( " + sql_cols + " ) ";
      
      string sqlstr = sql_create;
      const char*dbg = sqlstr.c_str();
      //occi_stm->setSQL( sqlstr );
      occi_stm->executeUpdate(sqlstr);
      
    // Now we need to apply spatial context data
    // Should insert user_sdo_geom_metada, as well as create spatial index
      if( classdef->GetClassType() == FdoClassType_FeatureClass )
      {
        FdoFeatureClass* fclass = (FdoFeatureClass*)classdef.p;
        
        FdoPtr<FdoGeometricPropertyDefinition> geomprop = fclass->GetGeometryProperty();
        
        try
        {
          InsertSdoGeomMetadata(tablename,geomprop);
        }
        catch(oracle::occi::SQLException& ea) // ignore this error, suspect many time will metada left and doesn't want to bother user
        {        
        }
        catch(FdoCommandException* ex)
        {
          ex->Release();
        }
        
        // Create Spatial Index
        try
        {
          CreateIndex(tablename,geomprop);
        }
        catch(oracle::occi::SQLException& ea) // ignore this error, suspect many time will metada left and doesn't want to bother user
        {        
        }
        catch(FdoCommandException* ex)
        {
          ex->Release();
        }       
      }
    // try to create Primary Key
      FdoPtr<FdoDataPropertyDefinitionCollection> ident_prop_col = classdef->GetIdentityProperties();
      try
      {
        CreatePrimaryKey(tablename,ident_prop_col);
           
      // check for autogenrated identity field
      // if so create - sequence for that table
        {      
          if( ident_prop_col->GetCount() == 1 )
          {
            FdoPtr<FdoDataPropertyDefinition> ident = ident_prop_col->GetItem(0);
            if( ident->GetIsAutoGenerated() )
            {
              if( (ident->GetDataType() == FdoDataType_Int16) 
                  || (ident->GetDataType() == FdoDataType_Int32)
                  || (ident->GetDataType() == FdoDataType_Int64)
                )
              {
              // create sequence 
                try
                {
                  CreateTableSequence(tablename);
                }
                catch(oracle::occi::SQLException& ea) // ignore this error, suspect many time will metada left and doesn't want to bother user
                {        
                }
                catch(FdoCommandException* ex)
                {
                  ex->Release();
                }  
              }
            }
          }
        }
      }
      catch(oracle::occi::SQLException& ea) // ignore this error, suspect many time will metada left and doesn't want to bother user
      {        
      }
      catch(FdoCommandException* ex)
      {
        ex->Release();
      }
    }
  }
  
  m_Connection->OCCI_TerminateStatement(occi_stm);
}
catch(oracle::occi::SQLException& ea)
{
  if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  
  FdoStringP gstr = ea.what();
  throw FdoCommandException::Create( gstr );    
}  
    
}//endof c_KgOraApplySchema::Execute

void c_KgOraApplySchema::InsertSdoGeomMetadata(FdoString* TableName,FdoGeometricPropertyDefinition* GeomProp)
{
  FdoString* scname = GeomProp->GetSpatialContextAssociation();
        
  FdoPtr<c_KgOraSpatialContextCollection> sccol = m_Connection->GetSpatialContexts();
  FdoPtr<c_KgOraSpatialContext> spcontext = sccol->FindItem(scname);
  
  bool isgeogcs=false;
  
  c_KgOraSridDesc orasrid;
  if( spcontext.p ) orasrid = spcontext->GetOraSridDesc();
  
  
  
  double xlow = D_SPATIALCONTEXT_DEFAULT_MINX;
  double xup = D_SPATIALCONTEXT_DEFAULT_MAXX;
  double ylow = D_SPATIALCONTEXT_DEFAULT_MINY;
  double yup = D_SPATIALCONTEXT_DEFAULT_MAXY;
  double zlow = D_SPATIALCONTEXT_DEFAULT_MINZ;
  double zup = D_SPATIALCONTEXT_DEFAULT_MAXZ;
  double mlow = D_SPATIALCONTEXT_DEFAULT_MINM;
  double mup = D_SPATIALCONTEXT_DEFAULT_MAXM;
  
  double xytol = D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE;
  double ztol = D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE;
  double mtol = D_SPATIALCONTEXT_DEFAULT_M_TOLERANCE;
  
  if( spcontext.p )
  {
    xytol = spcontext->GetXYTolerance();
    ztol = spcontext->GetZTolerance();
    
    FdoStringP wkt = spcontext->GetCoordinateSystemWkt();
    
    if( c_Ora_API::IsGeodeticCoordSystem(wkt) )
    {
      xlow = -180.0;
      xup = 180.0;
      ylow = -90.0;
      yup = 90.0;
      isgeogcs = true;
    }
    /*
    if( wkt.GetLength() > 6 )
    {
      FdoStringP gc = wkt.Mid(0,6);
      if( gc.ICompare("GEOGCS")==0 )
      {
        xlow = -180.0;
        xup = 180.0;
        ylow = -90.0;
        yup = 90.0;
        isgeogcs = true;
      }
    }
    */
  }
  else
  {
    isgeogcs = false;
    xytol = D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE;
    ztol = D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE;
    orasrid.m_OraSrid = 0;
    orasrid.m_IsGeodetic = false;
  }
  
  if( ztol <= 0.0 ) ztol = D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE;
  if( xytol <= 0.0 ) xytol = D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE;
  
  FdoStringP sqlstr,xdim,ydim,zdim,mdim;
  
  FdoStringP geomname = GeomProp->GetName();
  
  
  FdoStringP tabname = TableName;
  
  sqlstr = "INSERT INTO USER_SDO_GEOM_METADATA VALUES ( '";
  sqlstr = sqlstr + tabname.Upper() + "' , '" + geomname.Upper() + "'";
  sqlstr = sqlstr + ",MDSYS.SDO_DIM_ARRAY(";
  
  if( isgeogcs )
    xdim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('LAT', %lf, %lf, %lf)",xlow,xup,xytol);
  else
    xdim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('X', %lf, %lf, %lf)",xlow,xup,xytol);
    
  sqlstr = sqlstr + xdim;
    
  if( isgeogcs )
    ydim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('LON', %lf, %lf, %lf)",ylow,yup,xytol);
  else
    ydim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('Y', %lf, %lf, %lf)",ylow,yup,xytol);    
    
  sqlstr = sqlstr + "," + ydim;    
  
  if( GeomProp->GetHasElevation() )
  {
    zdim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('Z', %lf, %lf, %lf)",zlow,zup,ztol);
    sqlstr = sqlstr + "," + zdim;    
  }
  if( GeomProp->GetHasMeasure() )
  {
    mdim = FdoStringP::Format(L"MDSYS.SDO_DIM_ELEMENT('M', %lf, %lf, %lf)",mlow,mup,mtol);
    sqlstr = sqlstr + "," + mdim;    
  }
  sqlstr = sqlstr + ")";
  
  FdoStringP sridstr;
  if( orasrid.m_OraSrid > 0 )
    sridstr = FdoStringP::Format(L"%ld",orasrid);
  else
    sridstr = "NULL";
    
  sqlstr = sqlstr + "," + sridstr + ")";
  
  oracle::occi::Statement* occi_stm=NULL;
try
{
  occi_stm = m_Connection->OCCI_CreateStatement();
  
  string sstr = sqlstr;
  occi_stm->executeUpdate(sstr);
  
  m_Connection->OCCI_TerminateStatement(occi_stm);
}
catch(oracle::occi::SQLException& ea)
{
  if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  
  FdoStringP gstr = ea.what();
  throw FdoCommandException::Create( gstr );    
}    

}//end of c_KgOraApplySchema::InsertSdoGeomMetadata

void c_KgOraApplySchema::CreateIndex(FdoString* TableName,FdoGeometricPropertyDefinition* GeomProp)
{

  FdoStringP sqlstr;
  
  //"CREATE INDEX curvepolygon_sid ON curvepolygon ("GEOM") INDEXTYPE IS "MDSYS"."SPATIAL_INDEX" PARAMETERS ('sdo_indx_dims=2')"
  
  FdoStringP tabname = TableName;
  FdoStringP geomname= GeomProp->GetName();
  sqlstr = L"CREATE INDEX ";
  sqlstr = sqlstr + tabname.Upper() + "_sind ON "  + tabname.Upper() + " (\"" + geomname.Upper() + "\") INDEXTYPE IS ";
  sqlstr = sqlstr + "\"MDSYS\".\"SPATIAL_INDEX\" ";
  
  FdoStringP sqlstr_layer_gtype;
  FdoInt32 types = GeomProp->GetGeometryTypes();
  switch( types )
  {
    case FdoGeometricType_Point:
      sqlstr_layer_gtype = "MULTIPOINT";
    break;
    case FdoGeometricType_Curve:
      sqlstr_layer_gtype = "MULTILINE";
    break;
    case FdoGeometricType_Surface:
      sqlstr_layer_gtype = "MULTIPOLYGON";
    break;
    default:
      sqlstr_layer_gtype = "";
    break;
  }
  FdoStringP sqlstr_params;
  
  if( sqlstr_layer_gtype.GetLength() > 0 )
  {
    sqlstr_params = sqlstr_params.Format(L"PARAMETERS ('sdo_indx_dims=2 layer_gtype=%s')",(const wchar_t*)sqlstr_layer_gtype);
  }
  else
  {
    sqlstr_params = L"PARAMETERS ('sdo_indx_dims=2')";
  }
  sqlstr = sqlstr + sqlstr_params;
  
  oracle::occi::Statement* occi_stm=NULL;
  
try
{
  occi_stm = m_Connection->OCCI_CreateStatement();
  
  string sstr=sqlstr;
  
  occi_stm->executeUpdate(sstr);
  
  m_Connection->OCCI_TerminateStatement(occi_stm);
}
catch(oracle::occi::SQLException& ea)
{
  if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  
  FdoStringP gstr = ea.what();
  throw FdoCommandException::Create( gstr );    
}    

}//end of c_KgOraApplySchema::CreateIndex


void c_KgOraApplySchema::CreatePrimaryKey(FdoString* TableName,FdoDataPropertyDefinitionCollection* IdentPropColl)
{

  if( !IdentPropColl || IdentPropColl->GetCount() <= 0 )
  {
    return;    
  }
  
  FdoStringP sqlstr;
  
  FdoStringP cnames;
  FdoStringP sep;
  long colcount = IdentPropColl->GetCount();
  for(long ind=0;ind<colcount;ind++)
  {
    FdoPtr<FdoDataPropertyDefinition> dataprop = IdentPropColl->GetItem(ind);
    cnames = cnames + sep + dataprop->GetName();
    
    sep = L",";
  }
  
  FdoStringP pkeyname;
  pkeyname = TableName;
  pkeyname = pkeyname.Upper() + "_PK";
  sqlstr = sqlstr.Format(L"ALTER TABLE %s ADD CONSTRAINT %s primary key (%s)",(FdoString*)TableName,(FdoString*)pkeyname,(FdoString*)cnames);
  
 
  
  oracle::occi::Statement* occi_stm=NULL;
  
try
{
  occi_stm = m_Connection->OCCI_CreateStatement();
  
  string sstr=sqlstr;
  
  occi_stm->executeUpdate(sstr);
  
  m_Connection->OCCI_TerminateStatement(occi_stm);
}
catch(oracle::occi::SQLException& ea)
{
  if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  
  FdoStringP gstr = ea.what();
  throw FdoCommandException::Create( gstr );    
}    

}//end of c_KgOraApplySchema::CreatePrimaryKey

void c_KgOraApplySchema::CreateTableSequence(FdoString* TableName)
{


  
  FdoStringP sqlstr;
  
  sqlstr = sqlstr.Format(L"CREATE SEQUENCE %s_FDOSEQ",(FdoString*)TableName);
  
 
  
  oracle::occi::Statement* occi_stm=NULL;
  
try
{
  occi_stm = m_Connection->OCCI_CreateStatement();
  
  string sstr=sqlstr;
  
  occi_stm->executeUpdate(sstr);
  
  m_Connection->OCCI_TerminateStatement(occi_stm);
}
catch(oracle::occi::SQLException& ea)
{
  if( occi_stm ) m_Connection->OCCI_TerminateStatement(occi_stm);
  
  FdoStringP gstr = ea.what();
  throw FdoCommandException::Create( gstr );    
}    

}//end of c_KgOraApplySchema::CreateTableSequence