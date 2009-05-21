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
#include "c_FdoInf_API.h"

c_FdoInf_API::c_FdoInf_API(void)
{
}

c_FdoInf_API::~c_FdoInf_API(void)
{
}

bool c_FdoInf_API::OtlTypeToFdoDataType(int OtlType,int Scale,int Length,FdoDataType & FdoType)
{
  bool isfdotype=true;
  e_Otl_Var_Types otltypes = (e_Otl_Var_Types)OtlType;
  switch( otltypes )
  {
    case e_Otl_Var_bigint: // MS SQL Server, DB2, MySQL, PostgreSQL, etc. BIGINT (signed 64-bit integer) typebreak;
      FdoType = FdoDataType_Int64;  
    break;
 
    case e_Otl_Var_blob: // datatype that is mapped into BLOB in Oracle 8
      FdoType = FdoDataType_BLOB;
    break;
     
    case e_Otl_Var_char:   //null terminated string
      FdoType = FdoDataType_String;
    break;
     
    case e_Otl_Var_clob: //datatype that is mapped into CLOB in Oracle 8 
      FdoType = FdoDataType_CLOB;
    break;
    
     
    case e_Otl_Var_double: // 8-byte floating point number
      FdoType = FdoDataType_Double;  
    break;
     
    case e_Otl_Var_float: // 4-byte floating point number
      FdoType = FdoDataType_Single;  
    break;
     
    case e_Otl_Var_int: // signed 32-bit  integer
      FdoType = FdoDataType_Int32;  
    break;
     
    case e_Otl_Var_long_int: // signed 32-bit integer
      FdoType = FdoDataType_Int32;  
    break;
     
    
     
    case e_Otl_Var_raw: // RAW, BINARY, VARBINARY, BYTEA, VARCHAR BYTE, CHAR BYTE, etc.
     
     
    case e_Otl_Var_short: // signed 16-bit integer 
      FdoType = FdoDataType_Int16;  
    break;
     
    case e_Otl_Var_timestamp: // datatype that is mapped into TIMESTAMP_STRUCT, ODBC and DB2-CLI only
      FdoType = FdoDataType_DateTime;  
    break;
     
    //case e_Otl_Var_tz_timestamp: // Oracle 9i/10g TIMESTAMP WITH TIME ZONE type
     
    case e_Otl_Var_unsigned_int: // unsigned  32-bit integer
      FdoType = FdoDataType_Int32;  
    break;
     
    //case e_Otl_Var_varchar_long: // datatype that is mapped into LONG in Oracle 7/8, TEXT in MS SQL Server and Sybase, CLOB in DB2 
    // case e_Otl_Var_ltz_timestamp: // Oracle 9i/10g TIMESTAMP WITH LOCAL TIME ZONE type 
    //case e_Otl_Var_db2date: // DB2 DATE type      
    //case e_Otl_Var_db2time: // DB2 TIME type
    //case e_Otl_Var_raw_long: // datatype that is mapped into LONG RAW in Oracle 7/8, IMAGE in MS SQL Server ad Sybase, BLOB in DB2
    
    default:
      isfdotype = false;
    break;
  }
  

  
  #ifdef _DEBUG
  if( !isfdotype ) 
  {
    isfdotype =isfdotype;
  }
  #endif
  
  return isfdotype;
}//end of c_FdoInf_API::OtlTypeToFdoDataType



bool c_FdoInf_API::SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue)
{
  
  return c_KgOtl::SetStreamData(Stream,SqlParamNum,DataValue);
}//end of c_FdoInf_API::SetStreamData

bool c_FdoInf_API::SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb)
{
  
  return c_KgOtl::SetStreamDataGeometryWkb(Stream,SqlParamNum,GeometryWkb);
}//end of c_FdoInf_API::SetStreamDataGeometryWkb

bool c_FdoInf_API::SetStreamDataGeometryWkt(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb)
{

  return c_KgOtl::SetStreamDataGeometryWkt(Stream,SqlParamNum,GeometryWkb);
}//end of c_FdoInf_API::SetStreamDataGeometryWkt

bool c_FdoInf_API::SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf)
{
  
  return c_KgOtl::SetStreamDataGeometryFgf(Stream,SqlParamNum,GeometryFgf);
}//end of c_FdoInf_API::SetStreamData

bool c_FdoInf_API::IsGeodeticCoordSystem(const char* CoordSysWkt)
{
  string cswkt;
  
  cswkt = CoordSysWkt;
  
  
  if( cswkt.length() > 6 )
  {
    string substr = cswkt.substr(0,6);
    if( substr.compare("GEOGCS")==0 )
    {
      return true;
    }
  }
  
  return false;
  
}//
bool c_FdoInf_API::IsGeodeticCoordSystem(const wchar_t* CoordSysWkt)
{
  wstring cswkt;
  
  cswkt = CoordSysWkt;
  
  
  if( cswkt.length() > 6 )
  {
    wstring substr = cswkt.substr(0,6);
    if( substr.compare(L"GEOGCS")==0 )
    {
      return true;
    }
  }
  
  return false;
  
}//




bool c_FdoInf_API::GetCoordinateSystemWkt(c_KgInfConnection*Conn,long Srid,string& Wkt)
{
  long srid = 0;

  bool found=false;
  
  string sqlstr;
  sqlstr = " select SRTEXT from SDE.spatial_references where SRID = :p1<int>";
  
  otl_stream* stream = Conn->OTL_CreateStream(1,sqlstr.c_str());
  
  FdoDataValue *data = FdoDataValue::Create(Srid);
  c_KgOtl::SetStreamData(stream,1,data);
  data->Release();
  
  /*
  if( !stream->eof() )
  {
    wstring wstr;
    
    *stream>>wstr;
    
    FdoStringP fdostr = wstr.c_str();
    
    Wkt = fdostr;
    found=true;
  }*/
  wstring wstr;
  c_KgOtlStreamReader reader(stream);
  if( !reader.ReadNext() )
  {
    reader.GetString(0,wstr);
    FdoStringP fdostr = wstr.c_str();
    Wkt = fdostr;
    return found;
  }
    
  Conn->OTL_DeleteStream(stream);
      
 
  
  return found;
  
}//end of c_FdoInf_API::GetCoordinateSystemWkt

long c_FdoInf_API::GetSridFromWkt(c_KgInfConnection* Conn,const char* Wkt)
{
  long srid = 0;
  string sqlstr;
  sqlstr = " select SRID from SDE.spatial_references where SRTEXT = :p1<char(4000)>";
  
  otl_stream* stream = Conn->OTL_CreateStream(1,sqlstr.c_str());
   
  FdoDataValue *data = FdoDataValue::Create(Wkt);
  c_KgOtl::SetStreamData(stream,1,data);
  data->Release();
  
  c_KgOtlStreamReader reader(stream);
  
  if( !reader.ReadNext() )
  {
    reader.GetInt32(srid);
  }
    
  Conn->OTL_DeleteStream(stream);
  
  return srid;
  
}//end of c_FdoInf_API::GetCoordinateSystemWkt


c_KgInfSchemaDesc* c_FdoInf_API::DescribeSchema(c_KgInfConnection* Conn,const char* OraSchema,const char* KingFdoViews)
{
      
  FdoPtr<FdoFeatureSchemaCollection> fschemacoll=NULL;
  FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschema;
  int alias_num=0;
      
      
  fschemacoll = FdoFeatureSchemaCollection::Create(NULL);
  
  phschema = c_KgInfOverPhysicalSchemaMapping::Create();
  long aliasnum=0;
  
  FdoPtr<c_KgInfSpatialContextCollection> sc_collection = new c_KgInfSpatialContextCollection();

// Create FDO classes from tables in SDO_GEOM_METADATA
{  
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"IDS", L"");          
  fschemacoll->Add(schema.p);
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<c_KgInfOverClassCollection> phys_classes = phschema->GetClasses();
  
  vector<c_KgInf_GeomteryColumns> geomtables;
  otl_stream* stream1=NULL;
  otl_stream* streamMeasureElevation=NULL;
  try
  {
    
   stream1 = Conn->OTL_CreateStream(10
                ,"SELECT a.F_TABLE_CATALOG ,a.F_TABLE_SCHEMA ,a.F_TABLE_NAME ,a.F_GEOMETRY_COLUMN , a.COORD_DIMENSION, a.SRID ,a.GEOMETRY_TYPE ,b.SRTEXT "
                " FROM geometry_columns a  LEFT JOIN spatial_references b on a.srid = b.srid"
             );
 
    c_KgOtlStreamReader reader1(stream1);
    while(reader1.ReadNext())
    {
      c_KgInf_GeomteryColumns data;   
      reader1.GetString(1,data.m_TableCatalog);
      reader1.GetString(2,data.m_QuerySchema);
      reader1.GetString(3,data.m_QueryName);
      reader1.GetString(4,data.m_GeomColumn);
      data.m_CoordDimension = reader1.GetInt32(5);
      data.m_SRID = reader1.GetInt32(6);
      if( reader1.GetIsNull(6) ) data.m_SRID = D_INF_SRID_NULL_VALUE;
      
      data.m_IdsGeometryType = reader1.GetInt32(7);
      reader1.GetString(8,data.m_SrText);
         
      geomtables.push_back(data);
    }
    
    reader1.Close();    
    Conn->OTL_DeleteStream(stream1);
    
    if( KingFdoViews && *KingFdoViews )
    {
      otl_stream* stream2=NULL;
      try
      {
      char sqlstr[1024];
      sprintf(sqlstr,
       " SELECT k.fdo_schema,k.fdo_class_name,k.query_schema, k.query_name, k.geom_table_schema, k.geom_table_name, k.geom_column, k.classidentity, k.geometry_type "
       " ,a.F_TABLE_CATALOG,a.F_TABLE_SCHEMA,a.F_TABLE_NAME,a.F_GEOMETRY_COLUMN,a.COORD_DIMENSION,a.SRID,a.GEOMETRY_TYPE,b.SRTEXT "
       " FROM %s k, geometry_columns a, spatial_references b "
       " WHERE k.geom_table_schema = a.f_table_schema and k.geom_table_name = a.F_TABLE_NAME AND a.srid = b.srid"
       , KingFdoViews );
       
      stream2 = Conn->OTL_CreateStream(10,sqlstr);
   
      c_KgOtlStreamReader reader2(stream2);
         
    
      while(reader2.ReadNext())
      {
        c_KgInf_GeomteryColumns data;   
        reader2.GetString(1,data.m_FdoSchema);
        reader2.GetString(2,data.m_FdoClassName);
        
        reader2.GetString(3,data.m_QuerySchema);
        reader2.GetString(4,data.m_QueryName);
        
        reader2.GetString(5,data.m_GeomTableSchema);
        reader2.GetString(6,data.m_GeomTableName);
        
        reader2.GetString(7,data.m_GeomColumn);
        reader2.GetString(8,data.m_ClassIdentity);
        reader2.GetString(9,data.m_OverGeometryType);
        
        /*
        reader2.GetString(10,data.m_TableCatalog);
        reader2.GetString(11,data.m_QuerySchema);
        reader2.GetString(12,data.m_QueryName);
        reader2.GetString(13,data.m_GeomColumn);
        */
        
        data.m_CoordDimension = reader2.GetInt32(14);
        data.m_SRID = reader2.GetInt32(15);
        if( reader2.GetIsNull(15) ) data.m_SRID = D_INF_SRID_NULL_VALUE;
        
        data.m_IdsGeometryType = reader2.GetInt32(16);
        reader2.GetString(17,data.m_SrText);
        
        geomtables.push_back(data);
      }
      
      reader2.Close();    
      Conn->OTL_DeleteStream(stream2);
        
      }
      // Ignore eeror's and conitnue
      catch(FdoException* e)
      {
        Conn->OTL_DeleteStream(stream1);
        e->Release();
      }
      catch(c_KgOtlException& ea)
      {
        Conn->OTL_DeleteStream(stream1);
        
      }
    }
    
    c_FdoInf_API::CreateClassesfromTables(Conn,geomtables,classes,phys_classes,sc_collection,aliasnum);   
    
    
    
  }
  catch(FdoException* e)
  {
    Conn->OTL_DeleteStream(stream1);
    throw e;
  }
  catch(c_KgOtlException& ea)
  {
    Conn->OTL_DeleteStream(stream1);
    throw FdoException::Create( ea.what() );
  }
  
 
}

  
  c_KgInfSchemaDesc* schemadata = c_KgInfSchemaDesc::Create();
  
  schemadata->SetDesc(fschemacoll,phschema,sc_collection);
  
  
  return schemadata;
}//end of c_FdoInf_API::DescribeSchema

void c_FdoInf_API::CreateClassesfromTables(c_KgInfConnection* Conn,vector<c_KgInf_GeomteryColumns>& GeomTables
            ,FdoClassCollection* FdoClasses,c_KgInfOverClassCollection* PhysClasses
            ,c_KgInfSpatialContextCollection* SC_Collection,long& AliasNum)
{
try
{ 
	  
  wstring ora_fdo_classname;
    
  wstring ora_coord_sys_name,ora_coord_sys_wktext,ora_layer_gtype;
  wstring ora_fullname,ora_sequence_name;
  
  int tablescount = GeomTables.size();
  for(long indtable=0;indtable<tablescount;indtable++)
  {
    c_KgInf_GeomteryColumns tabledata = GeomTables[indtable] ; 
    
    
    // check if dot is in table or column name
    // in thisversion of rpovider skip those tables because fdo doesn't allow dots in names
    // TODO: change table name or column name so it doen't have dots
    
    if( (tabledata.m_QueryName.find(L".") != wstring::npos) || (tabledata.m_GeomColumn.find(L".") != wstring::npos) )
    {
      continue;
    }
              
    
    
    c_KgInfSridDesc orasriddesc;
    
    orasriddesc.m_OraSrid = (long)tabledata.m_SRID;
    orasriddesc.m_IsGeodetic = c_FdoInf_API::IsGeodeticCoordSystem(tabledata.m_SrText.c_str());
    
    
    // Test for coordinate system if exists and 
    FdoPtr<c_KgInfSpatialContext> spatial_context;
    if( (long)tabledata.m_SRID >= 0 )
    {
      FdoStringP numstr = FdoStringP::Format(L"%d",tabledata.m_SRID);
      FdoStringP cname = D_INF_SRID_NAME_PREFIX;
      cname = cname + numstr;
      spatial_context = SC_Collection->FindItem(cname);
      if( !spatial_context )
      {
        spatial_context = new c_KgInfSpatialContext();
        spatial_context->SetName(cname);
        
        //simon
        //FdoStringP csname = ora_coord_sys_name.c_str();
        //spatial_context->SetCoordSysName(csname);
        //FdoStringP csdesc = tabledata.m_SrDescription.c_str();
        FdoStringP cswkt = tabledata.m_SrText.c_str();
        if(!cswkt.Contains(L"UNKNOWN")||cswkt.Contains(L"unknown"))
        {
          spatial_context->SetCoordSysName(cswkt);
          spatial_context->SetCoordinateSystemWkt(cswkt);
        }
        else 
        {
          spatial_context->SetCoordSysName(L"");
          spatial_context->SetCoordinateSystemWkt(L"");
        }
        
        //simon
        //FdoStringP cswkt = ora_coord_sys_wktext.c_str();
        spatial_context->SetOraSridDesc(orasriddesc);
          
        SC_Collection->Insert(0,spatial_context);
      }
      
    }
    else
    {
    // this is no coordinate system
    // then I use DEFAULT spatial context
      
      spatial_context = SC_Collection->FindItem(D_INF_SRID_NAME_NULL);    
      if( !spatial_context.p )
      {
        spatial_context = new c_KgInfSpatialContext();
        spatial_context->SetName(D_INF_SRID_NAME_NULL);
        
        spatial_context->SetOraSridDesc(orasriddesc);
        SC_Collection->Insert(0,spatial_context);
      }   
      
    }
    
    // Now set extents for spatial context
    bool isminmax=true;
    double minx = D_SPATIALCONTEXT_DEFAULT_MINX;
    double maxx = D_SPATIALCONTEXT_DEFAULT_MAXX;
    double miny = D_SPATIALCONTEXT_DEFAULT_MINY;
    double maxy = D_SPATIALCONTEXT_DEFAULT_MAXY;
    // Apply new extent
    if( spatial_context.p && isminmax )
    {
      FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
      FdoPtr<FdoEnvelopeImpl> env = FdoEnvelopeImpl::Create(minx,miny,maxx,maxy);
      
      spatial_context->ExpandExtent( env );
    }
    
    
    // Define name of FDO class
    FdoStringP w_fdo_classname;
    if( tabledata.m_FdoClassName.length() > 0 )
    {
      w_fdo_classname = tabledata.m_FdoClassName.c_str();
    }
    else
    {
    
      if( tabledata.m_QuerySchema.length() > 0 )
        ora_fullname = tabledata.m_QuerySchema + L"." + tabledata.m_QueryName;
      else
        ora_fullname = tabledata.m_QueryName;
      
      ora_fdo_classname = tabledata.m_QuerySchema + L"~" + tabledata.m_QueryName + L"~" + tabledata.m_GeomColumn;
      
      w_fdo_classname = ora_fdo_classname.c_str();  
    }
    
    
    
    
    
    // check if class already exist
    // the previous sql can return multiple rows for same table.column because of multiple indexes on same column
    // so I need to check not to duplicate classes
    if( !FdoClasses->FindItem( w_fdo_classname ) )
    {
    
      FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(w_fdo_classname, L"");      
      
      FdoPtr<c_KgInfOverClassDefinition> phys_class  = c_KgInfOverClassDefinition::Create();
      
      phys_class->SetName( w_fdo_classname );
      phys_class->SetSqlQueryFullName( tabledata.m_QuerySchema.c_str(), tabledata.m_QueryName.c_str() );
      phys_class->SetSqlGeomFullName( tabledata.m_GeomTableSchema.c_str(), tabledata.m_GeomTableName.c_str() );
      AliasNum++;
      phys_class->SetOraTableAliasNum( AliasNum );
    
      FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
      

  ////////////////////////////////////////////////////////////////////////////////////////////
  //  Create Geometry Property
  ////////////////////////////////////////////////////////////////////////////////////////////       
      //
      //  Define Layer Geometry Type  
      //
      FdoInt32 fdo_geom_type = FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;
      
     
      if( tabledata.m_GeomColumn.length() > 0 )
      {
        FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(tabledata.m_GeomColumn.c_str(), L"");                        
      
        switch(tabledata.m_IdsGeometryType)
        {
          case 0:   // ST_Geometry 
          break;
          case 1:   // ST_Point 
            fdo_geom_type = FdoGeometricType_Point;
          break;
          case 2:    // ST_Curve 
            fdo_geom_type = FdoGeometricType_Curve;
          break;
          case 3:    // ST_LineString 
            fdo_geom_type = FdoGeometricType_Curve;
          break;
          case 4:    // ST_Surface 
            fdo_geom_type = FdoGeometricType_Surface;
          break;
          case 5:    // ST_Polygon 
            fdo_geom_type = FdoGeometricType_Surface;
          break;
          case 6:    // ST_GeomCollection 
          break;
          case 7:    // ST_MultiPoint 
            fdo_geom_type = FdoGeometricType_Point;
          break;
          case 8:    // ST_MultiCurve 
            fdo_geom_type = FdoGeometricType_Curve;
          break;
          case 9:    // ST_MultiLineString 
            fdo_geom_type = FdoGeometricType_Curve;
          break;
          case 10:    // ST_MultiSurface 
            fdo_geom_type = FdoGeometricType_Surface;
          break;
          case 11:    // ST_MultiPolygon
            fdo_geom_type = FdoGeometricType_Surface;
          break;
        }
      /*
        if( (tabledata.m_GeometryType.compare(L"POINT")==0) || (tabledata.m_GeometryType.compare(L"MULTIPOINT")==0) )
        {
          fdo_geom_type = FdoGeometricType_Point;
        }
        else
        {
          if( (tabledata.m_GeometryType.compare(L"LINE")==0) || (tabledata.m_GeometryType.compare(L"MULTILINE")==0)               
            )
          {
            fdo_geom_type = FdoGeometricType_Curve;
          }
          else
          {
            if( (tabledata.m_GeometryType.compare(L"POLYGON")==0) || (tabledata.m_GeometryType.compare(L"MULTIPOLYGON")==0) )
            {
              fdo_geom_type = FdoGeometricType_Surface;
            }
          }  
        }
        */
        gpd->SetGeometryTypes(fdo_geom_type);  
       
        
       
        if( spatial_context )
        {
          gpd->SetSpatialContextAssociation( spatial_context->GetName() );
        }
        
        pdc->Add(gpd);
        
        fc->SetGeometryProperty(gpd);
      }
     
  ////////////////////////////////////////////////////////////////////////////////////////////
  //  Add other non-geometry sttributes from table  
  //////////////////////////////////////////////////////////////////////////////////////////// 
      DescribeTableProperties(Conn,tabledata.m_QuerySchema.c_str(),tabledata.m_QueryName.c_str(),pdc);
      
              
  ////////////////////////////////////////////////////////////////////////////////////////////
  //  Check for primary keys
  //////////////////////////////////////////////////////////////////////////////////////////// 
      vector<wstring> pcols;
      
      // ckeck for user defined column for identity
      // if defined use it - if not look for primary keys on table
      if( tabledata.m_ClassIdentity.length() > 0 )
      {
        // transfering from string to FdoStrinCollection and then back to string
        // this was written in a hurry - could be better
        FdoPtr<FdoStringCollection> strcol = FdoStringCollection::Create(tabledata.m_ClassIdentity.c_str(),L",");
        long cnt = strcol->GetCount();
        for(long ind=0;ind<cnt;ind++)
        {
          FdoStringP fdostr = strcol->GetString(ind);
          wstring ss;
          ss = fdostr;
          pcols.push_back(ss);
        }     
      }
      else
      {
        
        c_FdoInf_API::GetTablePrimaryColumns(Conn,tabledata.m_QuerySchema.c_str(),tabledata.m_QueryName.c_str(),pcols);
        
      }
      
      
      if( pcols.size() > 0 )
      {
        bool isidentity_int=true;
        vector<wstring>::iterator iter = pcols.begin();
        for ( iter = pcols.begin( ) ; iter != pcols.end( ) ; iter++ )
        {
          FdoStringP gstr = iter->c_str();
          FdoPtr<FdoDataPropertyDefinition> entid = dynamic_cast<FdoDataPropertyDefinition*>(pdc->FindItem(gstr));
          if( entid.p )
          {            
            entid->SetNullable(false);
            //entid->SetIsAutoGenerated(true);
            FdoPtr<FdoDataPropertyDefinitionCollection> ic = fc->GetIdentityProperties();
            ic->Add( entid );
            
            FdoDataType datatype = entid->GetDataType();
            if( (datatype == FdoDataType_Int16) || (datatype == FdoDataType_Int32) || (datatype == FdoDataType_Int64) )
            {
              
            }
            else
            {
              isidentity_int = false;
            }
          }  
        }
   
      }
      FdoClasses->Add(fc);
      PhysClasses->Add( phys_class );
    }
    
  }        
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
 
  throw c_KgOtlException(p.msg,p.stm_text); 
}

}//end of c_FdoInf_API::CreateClassesfromTables


/* 
  Utility function to create property definitions from table columns 
*/
bool c_FdoInf_API::DescribeTableProperties(c_KgInfConnection* Conn,const wstring& Schema,const wstring& TableName,FdoPropertyDefinitionCollection* PropCollection)
{
    
  otl_stream*stream=NULL;
try
{
    stream = Conn->OTL_CreateStream(20,"select t.tabname,c.colname,c.collength,c.coltype,c.colmax,c.colmin"
                              " from systables t, syscolumns c"
                              " where LOWER(t.owner) = LOWER(:p1<char(256)>) and LOWER(t.tabname)=LOWER(:p2<char(256)>) and t.tabid=c.tabid; "
                          );
                          
                
                          
    
    FdoStringP str1;
    str1 = Schema.c_str();
    *stream <<str1;      
    
    str1 = TableName.c_str();
    *stream <<str1;
    
    
    c_KgOtlStreamReader reader(stream);
    
    wstring tname,cname,coldefault;
    int datatype;
    bool isnullable;
    
    FdoInt16 len;
    int prec,scale;

    
    int indcol;
    while(reader.ReadNext())
    {   
      indcol=1;   
      reader.GetString(indcol++,tname);
      reader.GetString(indcol++,cname);
      //reader.GetString(indcol++,isnullable);
      
      if( reader.GetIsNull(indcol) ) len=0;
      else 
      {
        len = reader.GetInt16(indcol);
      }
      indcol++;
      
      datatype = reader.GetInt16(indcol++);
      
      // In IDS if for NOt NUll columns bit 0x0100 is set
      isnullable = datatype & 0x0100 ? false : true;
      datatype = datatype & 0x00FF;
      
      
      
      // For IDS precision and scale are stored in collength
      // length = precision * 256 + scale
      // DECIMAL(16,4) length = 16*256+4
      // DECIMAL(10) length = 10*256+255
      prec = len & 0xFFFFFF00;
      prec = prec >> 8 ;
      
      scale = len & 0x000000FF;
      if( scale == 255 ) scale = -1;
      
      /*
      if( reader.GetIsNull(indcol) ) prec=0;
      else prec = reader.GetInt32(indcol);
      indcol++;
            
      if( reader.GetIsNull(indcol) ) scale=-1;
      else scale = reader.GetInt32(indcol);
      indcol++;
      */
      
      /*
      reader.GetString(indcol,coldefault);           
      if( reader.GetIsNull(indcol) ) coldefault=L"";
      indcol++;
      */      
      coldefault=L"";
      
               
      bool isfdotype=false;
      FdoDataType fdotype;
      
      FdoInt32 length=0,precision=0,scale=0;
       
      isfdotype = c_FdoInf_API::IdsTypeToFdoDataType(datatype,length,prec,scale,fdotype);
      
      if( isfdotype )
      {            
        FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(cname.c_str(), L"");
        dpd->SetDataType(fdotype);
        dpd->SetLength(len);
        dpd->SetPrecision(precision);
        dpd->SetScale(scale);
        
        PropCollection->Add(dpd);
      }
        
      
    }
    reader.Close();
    
    Conn->OTL_DeleteStream(stream);
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
  Conn->OTL_DeleteStream(stream);  
  throw c_KgOtlException(p.msg,p.stm_text); 
}
catch(c_KgOtlException& ea)
{
  Conn->OTL_DeleteStream(stream);
  throw FdoException::Create( ea.what() );
} 
catch(FdoException* e)
{
  Conn->OTL_DeleteStream(stream);
  throw e;
}
  

  return true;
}//end of c_FdoInf_API::DescribeTableProperties


/* 
  Utility function to create property definitions from table columns 
*/
bool c_FdoInf_API::GetTableOwner(c_KgInfConnection* Conn,FdoStringP& TableName,wstring& Owner)
{
  FdoStringP str1;
  otl_stream*stream=NULL;
try
{
    char sqlstr[1024];
    sprintf(sqlstr,"select t.owner from systables t where LOWER(t.tabname)=LOWER('%s')",(const char*)TableName);
    stream = Conn->OTL_CreateStream(1,sqlstr);

    wstring owner;
    c_KgOtlStreamReader reader(stream);
    
    wstring tname,cname,coldefault;
 

    if(reader.ReadNext())
    {   
      reader.GetString(1,Owner);
      //Owner = owner.c_str();
    }
    reader.Close();
    
    Conn->OTL_DeleteStream(stream);
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
  Conn->OTL_DeleteStream(stream);  
  throw c_KgOtlException(p.msg,p.stm_text); 
}
catch(c_KgOtlException& ea)
{
  Conn->OTL_DeleteStream(stream);
  throw FdoException::Create( ea.what() );
} 
catch(FdoException* e)
{
  Conn->OTL_DeleteStream(stream);
  throw e;
}

  return true;
}//end of c_FdoInf_API::GetTableOwner


bool c_FdoInf_API::IdsTypeToFdoDataType(int DataType,int Length,int Prec,int Scale,FdoDataType& FdoType)
{
  bool isfdotype=false;
  DataType = DataType & 0x00FF; // clear isnullable flag
  
  isfdotype=true;
  switch( DataType )
  {
    case 0: // CHAR
    {
      FdoType = FdoDataType_String;      
    }
    break;
    case 1: // SMALLINT
    {
      FdoType = FdoDataType_Int16; 
    }
    break;
    case 2: // INTEGER
    {
      FdoType = FdoDataType_Int32; 
    }
    break;
    case 3: // FLOAT    
    {
      FdoType = FdoDataType_Single; 
    }
    break;
    case 4: // SMALLFLOAT    
    {
      FdoType = FdoDataType_Single; 
    }
    break;
    case 5: // DECIMAL
    {
      FdoType = FdoDataType_Decimal;
    }
    break;
    case 6: // SERIAL
    {
      FdoType = FdoDataType_Int64; 
    }
    break;
    case 7: // DATE
    {
      FdoType = FdoDataType_DateTime;
    }
    break;
    case 8: // MONEY
    {
      FdoType = FdoDataType_Decimal;
    }
    break;
    case 10: // DATETIME
    {
      FdoType = FdoDataType_DateTime;
    }
    break;
    case 11: // BYTE
    {
      FdoType = FdoDataType_Byte;
    }
    break;
    case 12: // TEXT
    {
      FdoType = FdoDataType_String;      
    }
    break;
    case 13: // VARCHAR
    {
      FdoType = FdoDataType_String;      
    }
    break;
    
    case 15: // NCHAR
    {
      FdoType = FdoDataType_String;      
    }
    break;
    case 16: // NVARCHAR
    {
      FdoType = FdoDataType_String;      
    }
    break;
    default:
      isfdotype=false;
    break;
  }
  /*
  if( _wcsicmp(DataType,L"VARCHAR") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"CHAR") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"int") == 0 )
  {            
    FdoType = FdoDataType_Int32;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"smallint") == 0 )
  {            
    FdoType = FdoDataType_Int16;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"tinyint") == 0 )
  {            
    FdoType = FdoDataType_Int16;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"bigint") == 0 )
  {            
    FdoType = FdoDataType_Int64;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"CHAR") == 0 )
  {            
    if( Length==1 ) FdoType = FdoDataType_Byte;
    else FdoType = FdoDataType_String;
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"real") == 0 )
  {            
    FdoType = FdoDataType_Single;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"float") == 0 )
  {            
    FdoType = FdoDataType_Double;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"NUMERIC") == 0 )
  {            
    FdoType = FdoDataType_Decimal;    
    isfdotype=true;
  } else
  if( _wcsicmp(DataType,L"DATETIME") == 0 )
  {            
    FdoType = FdoDataType_DateTime;    
    isfdotype=true;
  } 
  */
  
  #ifdef _DEBUG
  if( !isfdotype ) 
  {
    isfdotype =isfdotype;
  }
  #endif
  
  return isfdotype;
}

bool c_FdoInf_API::FdoPropertyToInfDataType(FdoPropertyDefinition* Property,FdoStringP& InfType)
{
  switch( Property->GetPropertyType() )
  {
    case FdoPropertyType_GeometricProperty:
    {
      InfType = "varbinary(max)";
    }
    break;
    case FdoPropertyType_DataProperty:
    {
      FdoDataPropertyDefinition * propdata = (FdoDataPropertyDefinition *)Property;
      switch( propdata->GetDataType() )
      {
        case FdoDataType_String:
        {
          FdoInt32 len = propdata->GetLength();
          if( len <= 0  ) len = 4000;
          InfType = FdoStringP::Format(L"%s(%ld)",L"CHAR",len);
        }
        break;
        case FdoDataType_Decimal:
        {
        /*
          FdoInt32 prec = propdata->GetPrecision();
          FdoInt32 scale = propdata->GetScale();
          if( (prec>=1) && (prec<=38) )
          {
            if( (scale>=0) && (scale<=127) ) // Oracle supposrt from -84 but I thing that in FDO -1 means not defined
            {
              OraType = FdoStringP::Format(L"%s(%ld,%ld)",L"NUMBER",prec,scale);
            }
            else
            {
              OraType = FdoStringP::Format(L"%s(%ld,*)",L"NUMBER",prec);
            }
          }
          else
          {
            if( (scale>=0) && (scale<=127) )
            {
              OraType = FdoStringP::Format(L"%s(*,%ld)",L"NUMBER",scale);
            }
            else
            {
              OraType = FdoStringP::Format(L"%s",L"NUMBER");
            }
          }
        */
          InfType = "DECIMAL";
        }
        break;
        case FdoDataType_Byte:
          InfType = "BYTE";
        break;
        case FdoDataType_Single:
          InfType = "SMALLFLOAT";
        break;
        case FdoDataType_Double:
          InfType = "FLOAT";
        break;
        case FdoDataType_DateTime:
          InfType = "DATE";
        break;
        
        case FdoDataType_Boolean:
          InfType = "BYTE";
        break;
        
        case FdoDataType_Int16:
          InfType = "SMALLINT";
        break;

        case FdoDataType_Int32:
          InfType = "INTEGER";
        break;

        case FdoDataType_Int64:
          InfType = "INT8";
        break;
        
        default:
          return false;
        break;
      }
    }
    break;
    default:
      return false;
    break;
  }
  
  
  
  return true;
}//end of c_FdoInf_API::FdoPropertyToInfDataType

bool c_FdoInf_API::GetTablePrimaryColumns(c_KgInfConnection* Conn,const wstring& Schema,const wstring& TableName,vector<wstring>&  PrimCol)
{
  otl_stream *stream=NULL;
  bool foundident = false;
try
{  
  stream = Conn->OTL_CreateStream(1,"SELECT c.colname"
                        " FROM systables t, syscolumns c,sysindexes i "
                        " WHERE LOWER(t.owner)=LOWER(:p1<char(256)>) and LOWER(t.tabname)=LOWER(:p2<char(256)>) and t.tabid=c.tabid and t.tabid=i.tabid and i.idxtype='U' and (i.part1 = c.colno or i.part2=c.colno or i.part3=c.colno);");

  //select t.tabname,c.colname from systables t, syscolumns c,sysindexes i where t.owner = 'informix' and t.tabname='cities' and t.tabid=c.tabid and t.tabid=i.tabid and i.idxtype='U' and (i.part1 = c.colno or i.part2=c.colno or i.part3=c.colno); 
    
  FdoStringP fstr;
  
  fstr=Schema.c_str();
  *stream<<fstr;
  
  fstr = TableName.c_str();
  *stream<<fstr;
  
  wstring colname;
  c_KgOtlStreamReader reader(stream);
  while( reader.ReadNext() )
  {
    reader.GetString(1,colname);
    PrimCol.push_back(colname);
    foundident=true;
  }
  
  Conn->OTL_DeleteStream(stream);
  return foundident;
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
  Conn->OTL_DeleteStream(stream);  
  throw c_KgOtlException(p.msg,p.stm_text); 
}
catch(c_KgOtlException& ea)
{
  Conn->OTL_DeleteStream(stream);
  throw FdoException::Create( ea.what() );
} 
catch(FdoException* e)
{
  Conn->OTL_DeleteStream(stream);
  throw e;
}

}//end of c_FdoInf_API::GetTablePrimaryColumns

#ifdef _FDO_3_2
void c_FdoInf_API::FdoGeomTypesToInfGeomType(FdoGeometricPropertyDefinition* Geom,FdoInt32 &Gtype)
{
  bool ismulti=false;
 // int gtype = 0;
  FdoInt32 lentypes=0;
  FdoGeometryType* gtypes = Geom->GetSpecificGeometryTypes(lentypes);
  for(int ind=0;ind<lentypes;ind++)
  {
    switch( *gtypes )
    {
      case FdoGeometryType_Point:
        Gtype = FdoGeometryType_Point;
      break;

      case FdoGeometryType_LineString:
        Gtype = 3;
      break;
      case FdoGeometryType_CurveString:
        Gtype = FdoGeometryType_LineString;
      break;

      case FdoGeometryType_Polygon:
      Gtype = 5;
      break;
      case FdoGeometryType_CurvePolygon:
        Gtype = 4;
      break;
      
      case FdoGeometryType_MultiPoint:
        Gtype = 7;
        ismulti=true;
      break;
      case FdoGeometryType_MultiLineString:
        Gtype = 9;
        ismulti=true;
      break;
      case FdoGeometryType_MultiCurveString:
        Gtype = 8;
        ismulti=true;
      break;

      case FdoGeometryType_MultiPolygon:
        Gtype = 11;
        ismulti=true;
      break;
      case FdoGeometryType_MultiCurvePolygon:
        Gtype = 10;
        ismulti=true;
      break;

      case FdoGeometryType_MultiGeometry:
        //InfType = "GEOMETRYCOLLECTION";
        Gtype = 6;
        ismulti=true;
        return ;
      break;

    }
    gtypes++;
  }
  
  /*
  switch( Gtype )
  {
    case FdoGeometricType_Point:
      if( ismulti )
        InfType = "MULTIPOINT"  ;
      else
        InfType = "POINT"  ;
    break;
    
    case FdoGeometricType_Curve:
      if( ismulti )
        InfType = "MULTILINESTRING" ; 
      else
        InfType = "LINESTRING" ; 
    break;
    
    case FdoGeometricType_Surface:
      if( ismulti )
        InfType = "MULTIPOLYGON";
      else
        InfType = "POLYGON";  
    break;
    
    default:
    // different geometries
      InfType = "GEOMETRY";
    break;
  }
  
  if( ismulti )
  {
    InfType = "GEOMETRY"; // 
  }
 */
}//end of c_FdoInf_API::FdoGeomTypesToInfGeomType
#endif

#ifdef _FDO_3_1
void c_FdoInf_API::FdoGeomTypesToInfGeomType(FdoGeometricPropertyDefinition* Geom,FdoStringP& InfType)
{
  bool ismulti=true;
  int gtype = 0;
  
  gtype = Geom->GetGeometryTypes();
  
  
  switch( gtype )
  {
    case FdoGeometricType_Point:
      if( ismulti )
        InfType = "MULTIPOINT"  ;
      else
        InfType = "POINT"  ;
    break;
    
    case FdoGeometricType_Curve:
      if( ismulti )
        InfType = "MULTILINESTRING" ; 
      else
        InfType = "LINESTRING" ; 
    break;
    
    case FdoGeometricType_Surface:
      if( ismulti )
        InfType = "MULTIPOLYGON";
      else
        InfType = "POLYGON";  
    break;
    
    default:
    // different geometries
      InfType = "GEOMETRY";
    break;
  }
  
 if( ismulti )
  {
    InfType = "GEOMETRY"; 
  }
}//end of c_FdoInf_API::FdoGeomTypesToInfGeomType
#endif