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
#include "c_FdoMss_API.h"

c_FdoMss_API::c_FdoMss_API(void)
{
}

c_FdoMss_API::~c_FdoMss_API(void)
{
}

bool c_FdoMss_API::OtlTypeToFdoDataType(int OtlType,int Scale,int Length,FdoDataType & FdoType)
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
}//end of c_FdoMss_API::OtlTypeToFdoDataType



bool c_FdoMss_API::SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue)
{
  
  return c_KgOtl::SetStreamData(Stream,SqlParamNum,DataValue);
}//end of c_FdoMss_API::SetStreamData

bool c_FdoMss_API::SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb)
{
  
  return c_KgOtl::SetStreamDataGeometryWkb(Stream,SqlParamNum,GeometryWkb);
}//end of c_FdoMss_API::SetStreamData

bool c_FdoMss_API::SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf)
{
  
  return c_KgOtl::SetStreamDataGeometryFgf(Stream,SqlParamNum,GeometryFgf);
}//end of c_FdoMss_API::SetStreamData

bool c_FdoMss_API::IsGeodeticCoordSystem(const char* CoordSysWkt)
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
bool c_FdoMss_API::IsGeodeticCoordSystem(const wchar_t* CoordSysWkt)
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


/*
bool c_FdoMss_API::SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue)
{
  
  switch( DataValue->GetDataType() )
  {
    case FdoDataType_Boolean:
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCISTRING);
      else
      {
        FdoBooleanValue * boolval = (FdoBooleanValue*)DataValue;
        string val;
        if( boolval->GetBoolean() )
          val = "1";
        else
          val = "0";
        Statement->setString(SqlParamNum,val);
      }
    }
    break;

    /// Represents unsigned 8-bit integers with values between 0 and 255.
    case FdoDataType_Byte:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCISTRING);
      else
      {
        FdoByteValue * byteval = (FdoByteValue*)DataValue;
        char chstr[2];
        chstr[0] = byteval->GetByte();
        chstr[1] = 0;
        string val(chstr);
        
        Statement->setString(SqlParamNum,val);
      }
    }  
    break;

    /// Represents a date and time value.
    case FdoDataType_DateTime:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCIDATE);
      else
      {
        FdoDateTimeValue * dateval = (FdoDateTimeValue*)DataValue;
        
        FdoDateTime date = dateval->GetDateTime();
        oracle::occi::Date val(c_OCCI_API::GetEnvironment(),date.year,date.month,date.day,date.hour,date.minute,date.seconds);
        //val.setDate(date.year,date.month,date.day,date.hour,date.minute,date.seconds);
        Statement->setDate(SqlParamNum,val);
      }
    }
    break;

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
    case FdoDataType_Decimal:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCINUMBER);
      else
      {
        FdoDecimalValue * dval = (FdoDecimalValue*)DataValue;
        
        oracle::occi::Number num;
        num = dval->GetDecimal();
        Statement->setNumber(SqlParamNum,num);
      }
    }
    break;

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
    case FdoDataType_Double:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCIDOUBLE);
      else
      {
        FdoDoubleValue * dval = (FdoDoubleValue*)DataValue;                        
        Statement->setDouble(SqlParamNum,dval->GetDouble());
      }
    }
    break;

    /// Represents signed 16-bit integers with values between -32768 and 32767.
    case FdoDataType_Int16:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCIINT);
      else
      {
        FdoInt16Value * dval = (FdoInt16Value*)DataValue;                        
        Statement->setInt(SqlParamNum,dval->GetInt16());
      }
    }
    break;

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
    case FdoDataType_Int32:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCIINT);
      else
      {
        FdoInt32Value * dval = (FdoInt32Value*)DataValue;                        
        Statement->setInt(SqlParamNum,dval->GetInt32());
      }
    }
    break;

    /// Represents signed 64-bit integers with values 
    /// between -9223372036854775808 and 9223372036854775807.
    case FdoDataType_Int64:
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCINUMBER);
      else
      {
        FdoInt64Value * dval = (FdoInt64Value*)DataValue;                        
        
        //long dval
        
        oracle::occi::Number num;
        num = (long)dval->GetInt64();
        
        Statement->setNumber(SqlParamNum,num);
      }
    }
    break;

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
    case FdoDataType_Single:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCIDOUBLE);
      else
      {
        FdoSingleValue * dval = (FdoSingleValue*)DataValue;                        
        Statement->setDouble(SqlParamNum,dval->GetSingle());
      }
    }
    break;

    /// Represents a Unicode character strings.
    case FdoDataType_String:      
    {
      
      if( DataValue->IsNull() )
        Statement->setNull(SqlParamNum,oracle::occi::OCCISTRING);
      else
      {
        FdoStringValue * dval = (FdoStringValue*)DataValue;                        
        
        string sval;
        FdoStringP usval = dval->GetString();
        sval = usval;
        Statement->setString(SqlParamNum,sval);
      }
    }
    break;

    /// Represents a binary large object stored as a collection of bytes.
    case FdoDataType_BLOB:
    {
      Statement->setNull(SqlParamNum,oracle::occi::OCCIBLOB);
    }
    break;

    /// Represents a character large object stored as a collection of
    /// characters.
    case FdoDataType_CLOB:
    {
      Statement->setNull(SqlParamNum,oracle::occi::OCCICLOB);
    }
    break;
    
    default:
      return false;
    break;    
  }
 
  return true;
}//end of c_FdoOra_API::SetOracleStatementData
*/


bool c_FdoMss_API::GetCoordinateSystemWkt(c_KgMssConnection*Conn,long Srid,string& Wkt)
{
  long srid = 0;

  bool found=false;
  
  string sqlstr;
  sqlstr = " select SRTEXT from ST.SPATIAL_REF_SYS where SRID = :p1<int>";
  
  otl_stream* stream = Conn->OTL_CreateStream(1,sqlstr.c_str());
  
  FdoDataValue *data = FdoDataValue::Create(Srid);
  c_KgOtl::SetStreamData(stream,1,data);
  data->Release();
  
  if( !stream->eof() )
  {
    wstring wstr;
    
    *stream>>wstr;
    
    FdoStringP fdostr = wstr.c_str();
    
    Wkt = fdostr;
    found=true;
  }
      
  
  Conn->OTL_DeleteStream(stream);
  
  return found;
  
}//end of c_FdoMss_API::GetCoordinateSystemWkt

long c_FdoMss_API::GetSridFromWkt(c_KgMssConnection* Conn,const char* Wkt)
{
  long srid = 0;
  string sqlstr;
  sqlstr = " select SRID from ST.SPATIAL_REF_SYS where SRTEXT = :p1<char(4000)>";
  
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
  
}//end of c_FdoMss_API::GetCoordinateSystemWkt


c_KgMssSchemaDesc* c_FdoMss_API::DescribeSchema(c_KgMssConnection* Conn,const char* OraSchema,const char* KingFdoViews)
{
      
        FdoPtr<FdoFeatureSchemaCollection> fschemacoll=NULL;
        FdoPtr<c_KgMssOverPhysicalSchemaMapping> phschema;
        int alias_num=0;
      
      
  fschemacoll = FdoFeatureSchemaCollection::Create(NULL);
  
  phschema = c_KgMssOverPhysicalSchemaMapping::Create();
  long aliasnum=0;
  
  FdoPtr<c_KgMssSpatialContextCollection> sc_collection = new c_KgMssSpatialContextCollection();

// Create FDO classes from tables in SDO_GEOM_METADATA
{  
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"MsSqlSpatial", L"");          
  fschemacoll->Add(schema.p);
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<c_KgMssOverClassCollection> phys_classes = phschema->GetClasses();
  
  vector<c_KgMss_GeomteryColumns> geomtables;
  otl_stream* stream1=NULL;
  
  try
  {
     stream1 = Conn->OTL_CreateStream(10,"SELECT a.F_TABLE_CATALOG,a.F_TABLE_SCHEMA,a.F_TABLE_NAME "
              " ,a.F_GEOMETRY_COLUMN,a.COORD_DIMENSION,a.SRID,a.GEOMETRY_TYPE,b.SRTEXT "
              " FROM [ST].[GEOMETRY_COLUMNS] a "
              " LEFT JOIN st.spatial_ref_sys b on a.srid = b.srid"
              );
 
    c_KgOtlStreamReader reader1(stream1);
 
    
  
    while(reader1.ReadNext())
    {
      c_KgMss_GeomteryColumns data;   
      reader1.GetString(1,data.m_TableCatalog);
      reader1.GetString(2,data.m_QuerySchema);
      reader1.GetString(3,data.m_QueryName);
      reader1.GetString(4,data.m_GeomColumn);
      data.m_CoordDimension = reader1.GetInt32(5);
      data.m_SRID = reader1.GetInt32(6);
      if( reader1.GetIsNull(6) ) data.m_SRID = D_MSS_SRID_NULL_VALUE;
      
      reader1.GetString(7,data.m_GeometryType);
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
       " FROM %s k " 
       " LEFT JOIN [ST].[GEOMETRY_COLUMNS] a on k.geom_table_schema = a.F_TABLE_SCHEMA and k.geom_table_name = a.F_TABLE_NAME "
       " LEFT JOIN st.spatial_ref_sys b on a.srid = b.srid"
       , KingFdoViews );
       
       /*
       , d.sdo_layer_gtype "
                " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
                " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column ";
                
      char sqlfrom[512];
      sprintf(sqlfrom," FROM %s k ", KingFdoViews );
      */
      stream2 = Conn->OTL_CreateStream(10,sqlstr);
   
      c_KgOtlStreamReader reader2(stream2);
   
      
    
      while(reader2.ReadNext())
      {
        c_KgMss_GeomteryColumns data;   
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
        if( reader2.GetIsNull(15) ) data.m_SRID = D_MSS_SRID_NULL_VALUE;
        
        reader2.GetString(16,data.m_GeometryType);
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
    
    c_FdoMss_API::CreateClassesfromTables(Conn,geomtables,classes,phys_classes,sc_collection,aliasnum);   
    
    
    
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

  
  c_KgMssSchemaDesc* schemadata = c_KgMssSchemaDesc::Create();
  
  schemadata->SetDesc(fschemacoll,phschema,sc_collection);
  
  
  return schemadata;
}//end of c_FdoMss_API::DescribeSchema

void c_FdoMss_API::CreateClassesfromTables(c_KgMssConnection* Conn,vector<c_KgMss_GeomteryColumns>& GeomTables
            ,FdoClassCollection* FdoClasses,c_KgMssOverClassCollection* PhysClasses
            ,c_KgMssSpatialContextCollection* SC_Collection,long& AliasNum)
{
try
{ 
	
   
  
     
    
  wstring ora_fdo_classname;
  
  
  wstring ora_coord_sys_name,ora_coord_sys_wktext,ora_layer_gtype;
  wstring ora_fullname,ora_sequence_name;
  
  int tablescount = GeomTables.size();
  for(long indtable=0;indtable<tablescount;indtable++)
  {
    c_KgMss_GeomteryColumns tabledata = GeomTables[indtable] ; 
    
    
    // check if dot is in table or column name
    // in thisversion of rpovider skip those tables because fdo doesn't allow dots in names
    // TODO: change table name or column name so it doen't have dots
    
    if( (tabledata.m_QueryName.find(L".") != wstring::npos) || (tabledata.m_GeomColumn.find(L".") != wstring::npos) )
    {
      continue;
    }
          
    
    
    
    c_KgMssSridDesc orasriddesc;
    
    orasriddesc.m_OraSrid = (long)tabledata.m_SRID;
    orasriddesc.m_IsGeodetic = c_FdoMss_API::IsGeodeticCoordSystem(tabledata.m_SrText.c_str());
    
    
    // Test for coordinate system if exists and 
    FdoPtr<c_KgMssSpatialContext> spatial_context;
    if( (long)tabledata.m_SRID >= 0 )
    {
      FdoStringP numstr = FdoStringP::Format(L"%d",tabledata.m_SRID);
      FdoStringP cname = D_MSS_SRID_NAME_PREFIX;
      cname = cname + numstr;
      spatial_context = SC_Collection->FindItem(cname);
      if( !spatial_context )
      {
        spatial_context = new c_KgMssSpatialContext();
        spatial_context->SetName(cname);
        
        FdoStringP csname = ora_coord_sys_name.c_str();
        spatial_context->SetCoordSysName(csname);
        
        FdoStringP cswkt = ora_coord_sys_wktext.c_str();
        spatial_context->SetCoordinateSystemWkt(cswkt);
        
        spatial_context->SetOraSridDesc(orasriddesc);
          
        SC_Collection->Insert(0,spatial_context);
      }
      
    }
    else
    {
    // this is no coordinate system
    // then I use DEFAULT spatial context
      
      spatial_context = SC_Collection->FindItem(D_MSS_SRID_NAME_NULL);    
      if( !spatial_context.p )
      {
        spatial_context = new c_KgMssSpatialContext();
        spatial_context->SetName(D_MSS_SRID_NAME_NULL);
        
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
      
      FdoPtr<c_KgMssOverClassDefinition> phys_class  = c_KgMssOverClassDefinition::Create();
      
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
      
      wstring idcol;
      if( c_FdoMss_API::GetTableIdentityColumn(Conn,tabledata.m_QuerySchema.c_str(),tabledata.m_QueryName.c_str(),idcol) )
      {
        pcols.push_back(idcol);
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
            entid->SetIsAutoGenerated(true);
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

}//end of c_FdoMss_API::CreateClassesfromTables


/* 
  Utility function to create property definitions from table columns 
*/
bool c_FdoMss_API::DescribeTableProperties(c_KgMssConnection* Conn,const wstring& Schema,const wstring& TableName,FdoPropertyDefinitionCollection* PropCollection)
{
    
  otl_stream*stream=NULL;
try
{
    stream = Conn->OTL_CreateStream(20,"select table_name,column_name,is_nullable,data_type,character_maximum_length,numeric_precision"
                              ",numeric_scale,column_default from information_schema.columns"
                              " where table_schema = :p1<char(256)> and table_name=:p2<char(256)>; "
                          );
                          
    
    FdoStringP str1;
    str1 = Schema.c_str();
    *stream <<str1;      
    
    str1 = TableName.c_str();
    *stream <<str1;
    
    
    c_KgOtlStreamReader reader(stream);
    
    wstring tname,cname,isnullable,coldefault,datatype;
    
    int length,prec,scale;
    
    while(reader.ReadNext())
    {      
      reader.GetString(1,tname);
      reader.GetString(2,cname);
      reader.GetString(3,isnullable);
      reader.GetString(4,datatype);
      
      if( reader.GetIsNull(5) ) length=0;
      else length = reader.GetInt32(5);
      
      if( reader.GetIsNull(6) ) prec=0;
      else prec = reader.GetInt32(6);
      
      if( reader.GetIsNull(7) ) scale=-1;
      else scale = reader.GetInt32(7);
      
      reader.GetString(8,coldefault);           
      if( reader.GetIsNull(8) ) coldefault=L"";
      
               
      bool isfdotype=false;
      FdoDataType fdotype;
      
      FdoInt32 length=0,precision=0,scale=0;
       
      isfdotype = c_FdoMss_API::MsSqlTypeToFdoDataType(datatype.c_str(),length,prec,scale,fdotype);
      
      if( isfdotype )
      {            
        FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(cname.c_str(), L"");
        dpd->SetDataType(fdotype);
        dpd->SetLength(length);
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
}//end of c_FdoMss_API::DescribeTableProperties


bool c_FdoMss_API::MsSqlTypeToFdoDataType(const wchar_t * DataType,int Length,int Prec,int Scale,FdoDataType& FdoType)
{
  bool isfdotype=false;
  
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
  
  #ifdef _DEBUG
  if( !isfdotype ) 
  {
    isfdotype =isfdotype;
  }
  #endif
  
  return isfdotype;
}

bool c_FdoMss_API::FdoPropertyToMssDataType(FdoPropertyDefinition* Property,FdoStringP& MssType)
{
  switch( Property->GetPropertyType() )
  {
    case FdoPropertyType_GeometricProperty:
    {
      MssType = "varbinary(max)";
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
          MssType = FdoStringP::Format(L"%s(%ld)",L"VARCHAR",len);
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
          MssType = "float";
        }
        break;
        case FdoDataType_Byte:
          MssType = "CHAR(1)";
        break;
        case FdoDataType_Single:
          MssType = "real";
        break;
        case FdoDataType_Double:
          MssType = "float";
        break;
        case FdoDataType_DateTime:
          MssType = "DATETIME";
        break;
        
        case FdoDataType_Boolean:
          MssType = "BIT";
        break;
        
        case FdoDataType_Int16:
          MssType = "smallint";
        break;

        case FdoDataType_Int32:
          MssType = "int";
        break;

        case FdoDataType_Int64:
          MssType = "bigint";
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
}//end of c_FdoMss_API::FdoPropertyToMssDataType

bool c_FdoMss_API::GetTableIdentityColumn(c_KgMssConnection* Conn,const wstring& Schema,const wstring& TableName,wstring& IdentCol)
{
  otl_stream *stream=NULL;
  bool foundident = false;
try
{  
  stream = Conn->OTL_CreateStream(1,"SELECT column_name as Identity_Column,ident_incr(table_name),ident_seed(table_name) incre"
                        " FROM information_schema.columns "
                        " WHERE COLUMNPROPERTY(OBJECT_ID(table_name),column_name,'IsIdentity')=1 and table_schema=:p1<char(256)> and table_name=:p2<char(256)>");

  
  FdoStringP fstr;
  
  fstr=Schema.c_str();
  *stream<<fstr;
  
  fstr = TableName.c_str();
  *stream<<fstr;
  
  c_KgOtlStreamReader reader(stream);
  while( reader.ReadNext() )
  {
    reader.GetString(1,IdentCol);
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

}//end of c_FdoMss_API::GetTableIdentityColumn

#ifdef _FDO_3_2
void c_FdoMss_API::FdoGeomTypesToMssGeomType(FdoGeometricPropertyDefinition* Geom,FdoStringP& MssType)
{
  bool ismulti=false;
  int gtype = 0;
  FdoInt32 lentypes=0;
  FdoGeometryType* gtypes = Geom->GetSpecificGeometryTypes(lentypes);
  for(int ind=0;ind<lentypes;ind++)
  {
    switch( *gtypes )
    {
      case FdoGeometryType_Point:
        gtype |= FdoGeometricType_Point;
      break;

      case FdoGeometryType_LineString:
      case FdoGeometryType_CurveString:
        gtype |= FdoGeometricType_Curve;
      break;

      case FdoGeometryType_Polygon:
      case FdoGeometryType_CurvePolygon:
        gtype |= FdoGeometricType_Surface;
      break;
      
      case FdoGeometryType_MultiPoint:
        gtype |= FdoGeometricType_Point;
        gtype |= FdoGeometricType_Point;
        ismulti=true;
      break;

      case FdoGeometryType_MultiLineString:
      case FdoGeometryType_MultiCurveString:
        gtype |= FdoGeometricType_Curve;
        ismulti=true;
      break;

      case FdoGeometryType_MultiPolygon:
      case FdoGeometryType_MultiCurvePolygon:
        gtype |= FdoGeometricType_Surface;
        ismulti=true;
      break;

      case FdoGeometryType_MultiGeometry:
        MssType = "GEOMETRYCOLLECTION";
        ismulti=true;
        return ;
      break;

    }
    gtypes++;
  }
  
  
  switch( gtype )
  {
    case FdoGeometricType_Point:
      if( ismulti )
        MssType = "MULTIPOINT"  ;
      else
        MssType = "POINT"  ;
    break;
    
    case FdoGeometricType_Curve:
      if( ismulti )
        MssType = "MULTILINESTRING" ; 
      else
        MssType = "LINESTRING" ; 
    break;
    
    case FdoGeometricType_Surface:
      if( ismulti )
        MssType = "MULTIPOLYGON";
      else
        MssType = "POLYGON";  
    break;
    
    default:
    // different geometries
      MssType = "GEOMETRY";
    break;
  }
  
  if( ismulti )
  {
    MssType = "GEOMETRY"; // tis is because of issue in MsSqlSpatial; when multipolygon it will not allow normal polygon
  }
 
}//end of c_FdoMss_API::FdoGeomTypesToMssGeomType
#endif

#ifdef _FDO_3_1
void c_FdoMss_API::FdoGeomTypesToMssGeomType(FdoGeometricPropertyDefinition* Geom,FdoStringP& MssType)
{
  bool ismulti=true;
  int gtype = 0;
  
  gtype = Geom->GetGeometryTypes();
  
  
  switch( gtype )
  {
    case FdoGeometricType_Point:
      if( ismulti )
        MssType = "MULTIPOINT"  ;
      else
        MssType = "POINT"  ;
    break;
    
    case FdoGeometricType_Curve:
      if( ismulti )
        MssType = "MULTILINESTRING" ; 
      else
        MssType = "LINESTRING" ; 
    break;
    
    case FdoGeometricType_Surface:
      if( ismulti )
        MssType = "MULTIPOLYGON";
      else
        MssType = "POLYGON";  
    break;
    
    default:
    // different geometries
      MssType = "GEOMETRY";
    break;
  }
  
 if( ismulti )
  {
    MssType = "GEOMETRY"; // tis is because of issue in MsSqlSpatial; when multipolygon it will not allow normal polygon
  }
}//end of c_FdoMss_API::FdoGeomTypesToMssGeomType
#endif