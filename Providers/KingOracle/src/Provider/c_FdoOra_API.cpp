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
#include "c_KgOraSchemaDesc.h"
#include "c_FdoOra_API.h"
#include "c_OCCI_API.h"
#include "c_Ora_API.h"

c_FdoOra_API::c_FdoOra_API(void)
{
}

c_FdoOra_API::~c_FdoOra_API(void)
{
}


bool c_FdoOra_API::OraTypeToFdoDataType(oracle::occi::Type OraType,int Scale,int Length,FdoDataType & FdoType)
{
  bool isfdotype=true;
  switch( OraType )
  {
    case oracle::occi::OCCI_SQLT_CHR:  // varchar2
    case oracle::occi::OCCISTRING:  // varchar2
      FdoType = FdoDataType_String;   
    break;
    
    case oracle::occi::OCCI_SQLT_NUM:  // NUMBER
    case oracle::occi::OCCINUMBER:  // NUMBER
      if( Scale > 0 ) FdoType = FdoDataType_Decimal;
      else FdoType = FdoDataType_Int32;  
    break;
    
    case oracle::occi::OCCI_SQLT_AFC: // char
      if( Length==1 ) FdoType = FdoDataType_Byte;
      else FdoType = FdoDataType_String;
    break;
    case oracle::occi::OCCIIBFLOAT:
      FdoType = FdoDataType_Single;  
    break;
    case oracle::occi::OCCIIBDOUBLE:
      FdoType = FdoDataType_Double;  
    break;
    
    case oracle::occi::OCCI_SQLT_DAT:
      FdoType = FdoDataType_DateTime;  
    break;
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
}//end of c_FdoOra_API::OraTypeToFdoDataType


bool c_FdoOra_API::FdoDataTypeToOraDataType(FdoDataType DataType,oracle::occi::Type& OraType)
{
  switch( DataType )
  {
    case FdoDataType_Boolean:
      OraType = oracle::occi::OCCISTRING;
    break;

    /// Represents unsigned 8-bit integers with values between 0 and 255.
    case FdoDataType_Byte:
      OraType = oracle::occi::OCCISTRING;
    break;

    /// Represents a date and time value.
    case FdoDataType_DateTime:
      OraType = oracle::occi::OCCIDATE;
    break;

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
    case FdoDataType_Decimal:
      OraType = oracle::occi::OCCINUMBER;
    break;

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
    case FdoDataType_Double:
      OraType = oracle::occi::OCCIDOUBLE;
    break;

    /// Represents signed 16-bit integers with values between -32768 and 32767.
    case FdoDataType_Int16:
      OraType = oracle::occi::OCCIINT;
    break;

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
    case FdoDataType_Int32:
      OraType = oracle::occi::OCCIINT;
    break;

    /// Represents signed 64-bit integers with values 
    /// between -9223372036854775808 and 9223372036854775807.
    case FdoDataType_Int64:
      OraType = oracle::occi::OCCINUMBER;
    break;

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
    case FdoDataType_Single:
      OraType = oracle::occi::OCCIFLOAT;
    break;

    /// Represents a Unicode character strings.
    case FdoDataType_String:
      OraType = oracle::occi::OCCISTRING;
    break;

    /// Represents a binary large object stored as a collection of bytes.
    case FdoDataType_BLOB:
      OraType = oracle::occi::OCCIBLOB;
    break;

    /// Represents a character large object stored as a collection of
    /// characters.
    case FdoDataType_CLOB:
      OraType = oracle::occi::OCCICLOB;
    break;
    
    default:
      return false;
    break;    
  }
 
  return true;
}//end of c_FdoOra_API::FdoDataTypeToOraDataType




bool c_FdoOra_API::SetOracleStatementData(oracle::occi::Statement* Statement,int SqlParamNum,FdoDataValue* DataValue)
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
        oracle::occi::Date val;
        val.setDate(date.year,date.month,date.day,date.hour,date.minute,date.seconds);
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

bool c_FdoOra_API::OraTypeToFdoDataType(const char* OraType,int Scale,int Length,FdoDataType & FdoType)
{
  
  bool isfdotype=false;
  
  if( strcmpi(OraType,"VARCHAR2") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( strcmpi(OraType,"VARCHAR") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( strcmpi(OraType,"NUMBER") == 0 )
  {            
    if( Scale > 0 ) FdoType = FdoDataType_Decimal;
    else FdoType = FdoDataType_Int32;
    isfdotype=true;
  } else
  if( strcmpi(OraType,"CHAR") == 0 )
  {            
    if( Length==1 ) FdoType = FdoDataType_Byte;
    else FdoType = FdoDataType_String;
    isfdotype=true;
  } else
  if( strcmpi(OraType,"BINARY_FLOAT") == 0 )
  {            
    FdoType = FdoDataType_Single;    
    isfdotype=true;
  } else
  if( strcmpi(OraType,"BINARY_DOUBLE") == 0 )
  {            
    FdoType = FdoDataType_Double;    
    isfdotype=true;
  } else
  if( strcmpi(OraType,"DATE") == 0 )
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
}//end of c_FdoOra_API::OraTypeToFdoDataType


/* 
  Utility function to create property definitions from table columns 
*/
bool c_FdoOra_API::DescribeTableProperties(oracle::occi::Connection * OcciConnection,const char*Schema,const char*TableName,FdoPropertyDefinitionCollection* PropCollection)
{
    
  oracle::occi::Statement *occi_stm = NULL;  
  oracle::occi::ResultSet *occi_rs = NULL;    
  try {
    occi_stm = OcciConnection->createStatement(); 
    occi_stm->setPrefetchRowCount(20);
  
    occi_stm->setSQL("select COLUMN_NAME,DATA_TYPE,data_length,data_precision,data_scale from all_tab_columns where owner=:1 AND table_name=:2");
    occi_stm->setString(1,Schema);
    occi_stm->setString(2,TableName);
    occi_rs = occi_stm->executeQuery();
        
    while(occi_rs->next())
    {      
      if( !occi_rs->isNull(1) )
      {
        string s = occi_rs->getString(1);
        const char* cname = s.c_str();
               
        bool isfdotype=false;
        FdoDataType fdotype;
        
        FdoInt32 length=0,precision=0,scale=0;
         
        // length          
        if( !occi_rs->isNull(3) )          
        {
          length = occi_rs->getInt(3);
        }
        else length = 0;
                  
        // precision          
        if( !occi_rs->isNull(4) )          
        {
          precision = occi_rs->getInt(4);
        }
        else precision = 0;
        
        // scale   
        if( !occi_rs->isNull(5) )          
        {
          scale = occi_rs->getInt(5);
        }
        else scale = 0;
          
        if( !occi_rs->isNull(2) )
        {
          string s = occi_rs->getString(2);
          const char* dtype = s.c_str();
          
          isfdotype = c_FdoOra_API::OraTypeToFdoDataType(dtype,scale,length,fdotype);
          
          if( isfdotype )
          {            
            FdoStringP s1 = cname;
            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(s1, L"");
            dpd->SetDataType(fdotype);
            dpd->SetLength(length);
            dpd->SetPrecision(precision);
            dpd->SetScale(scale);
            
            PropCollection->Add(dpd);
          }
        }
      }
    }
    
    if( occi_stm )
    {
      if( occi_rs ) { occi_stm->closeResultSet(occi_rs);occi_rs=NULL; }      
      OcciConnection->terminateStatement (occi_stm);
      occi_stm=NULL;
    }
  }
  
  
  catch(oracle::occi::SQLException& ea)
  {
    if( occi_stm )
    {
      if( occi_rs ) { occi_stm->closeResultSet(occi_rs);occi_rs=NULL; }      
      OcciConnection->terminateStatement (occi_stm);
      occi_stm=NULL;
    }
    FdoStringP gstr = ea.getMessage().c_str();
	  throw FdoException::Create( gstr );
  }
  return true;
}//end of c_FdoOra_API::DescribeTableProperties



//FdoPtr<FdoFeatureSchemaCollection> g_FeatureSchemas(NULL);
//FdoPtr<FdoKgOraPhysicalSchemaMapping>  g_PhysicalSchemaMapping(NULL);

c_KgOraSchemaDesc* c_FdoOra_API::DescribeSchema(c_KgOraConnection* KgOraConn,const char* OraSchema)
{
      
        FdoPtr<FdoFeatureSchemaCollection> fschema=NULL;
        FdoPtr<FdoKgOraPhysicalSchemaMapping> phschema;
        int alias_num=0;
      
  oracle::occi::Connection * occi_conn = KgOraConn->GetOcciConnection();
      
  fschema = FdoFeatureSchemaCollection::Create(NULL);
  
  phschema = FdoKgOraPhysicalSchemaMapping::Create();
  
  FdoPtr<c_KgOraSpatialContextCollection> sc_collection = new c_KgOraSpatialContextCollection();
  
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"KingOra", L"");          
  fschema->Add(schema.p);
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
  
  oracle::occi::Statement*  occi_stm = NULL;
  oracle::occi::ResultSet* occi_rs = NULL;
  try
  {
    /* Query Oracle for geometry tables 
    */
    occi_stm = occi_conn->createStatement();
    
    
    
    //occi_stm->setSQL("select owner, table_name,column_name,srid,diminfo from all_sdo_geom_metadata where owner=:1 order by owner,table_name");
    //occi_stm->setString(1,"OSE");
    
    // this is for 10.2 only ( not for previous releases )
    /*
    occi_stm->setSQL(    
      " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.COORD_REF_SYS_NAME, b.LEGACY_WKTEXT, c.index_name, d.SDO_ROOT_MBR"
      " from all_sdo_geom_metadata a, SDO_COORD_REF_SYS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d"
      " where a.srid = b.srid(+) and a.owner = c.table_owner "
      " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
      " order by a.owner, a.table_name "
    );
    */
    
    // this is for prior 10.2 version (works also with 10.2)
    if( !OraSchema || (*OraSchema == 0) )
    {
      if( KgOraConn->GetOracleMainVersion() >= 10 )
      {
        /*
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT , c.index_name, d.SDO_ROOT_MBR "
        " from all_sdo_geom_metadata a, MDSYS.CS_SRS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d "
        " where a.srid = b.srid(+) and a.owner = c.table_owner "
        " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " order by a.owner, a.table_name"
        );
        */
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, d.SDO_ROOT_MBR  "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name "
        " order by a.owner, a.table_name "
        );
       
      }
      else
      {
        /*
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT , c.index_name "
        " from all_sdo_geom_metadata a, MDSYS.CS_SRS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d "
        " where a.srid = b.srid(+) "  // this line is different for oracle 10 nad 9
        " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " order by a.owner, a.table_name"
        );
        */
        
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name "
        " order by a.owner, a.table_name "
        );
      }
      
      // SELECT SDO_AGGR_MBR(geom) FROM curvepolygon
    }
    else
    {
      if( KgOraConn->GetOracleMainVersion() >= 10 )
      {
        /*
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT , c.index_name, d.SDO_ROOT_MBR "
        " from all_sdo_geom_metadata a, MDSYS.CS_SRS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d "
        " where a.owner = :1 and a.srid = b.srid(+) and a.owner = c.table_owner "
        " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " order by a.owner, a.table_name"
        );
        */
        
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, d.SDO_ROOT_MBR  "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name and t.owner = :1 "
        " order by a.owner, a.table_name "
        );
      }
      else
      {
      /*
          occi_stm->setSQL(  
            " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT , c.index_name "
            " from all_sdo_geom_metadata a, MDSYS.CS_SRS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d "
            " where a.owner = :1 and a.srid = b.srid(+) " // this line is different for oracle 10 nad 9
            " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            " order by a.owner, a.table_name"
            );
      */
        occi_stm->setSQL(  
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name and t.owner = :1 "
        " order by a.owner, a.table_name "
        );
      }
      occi_stm->setString(1,OraSchema);
      
    }

     
    occi_stm->setPrefetchRowCount(50);
    occi_rs = occi_stm->executeQuery();
    
    bool ora_srid_isnull;
    string ora_tablename,ora_colname,ora_tableowner,ora_fdo_classname,ora_index_name;
    oracle::occi::Number ora_srid;
    SDO_GEOMETRY *ora_geom;
    string ora_coord_sys_name,ora_coord_sys_wktext,ora_layer_gtype;
    string ora_fullname;
    
    
    
    while( occi_rs->next() != oracle::occi::ResultSet::END_OF_FETCH )
    {
      ora_tableowner = occi_rs->getString(1);
      ora_tablename = occi_rs->getString(2);
      ora_colname = occi_rs->getString(3);
      
      // check if dot is in table or column name
      // in thisversion of rpovider skip those tables because fdo doesn't allow dots in names
      // TODO: change table name or column name so it doen't have dots
      if( (ora_tablename.find(".") != string::npos) || (ora_colname.find(".") != string::npos) )
      {
        continue;
      }
      
      ora_srid_isnull = occi_rs->isNull(4);
      if( !ora_srid_isnull ) ora_srid = occi_rs->getNumber(4);
      else ora_srid = 0;
      
      OCCI_STD_NAMESPACE::vector< SDO_DIM_ELEMENT* > dimlist;
      oracle::occi::getVector(occi_rs,5,dimlist);
      
      ora_coord_sys_name = occi_rs->getString(6);
      ora_coord_sys_wktext = occi_rs->getString(7);
      
      ora_index_name = occi_rs->getString(8);    
      
      ora_layer_gtype = occi_rs->getString(9);    
      
      
      if(  ( KgOraConn->GetOracleMainVersion() >= 10 ) && (!occi_rs->isNull(10) ) )
        ora_geom = (SDO_GEOMETRY*)occi_rs->getObject(10); // oracle is 1 based - our index is 0 based
      else
        ora_geom = NULL;
      if(ora_geom )
      {
      }
      
      c_KgOraSridDesc orasriddesc;
      
      orasriddesc.m_OraSrid = (long)ora_srid;
      orasriddesc.m_IsGeodetic = c_Ora_API::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str());
      
      
      // Test for coordinate system if exists and 
      FdoPtr<c_KgOraSpatialContext> spatial_context;
      if( (long)ora_srid >= 0 )
      {
        //FdoStringP cname = ora_coord_sys_name.c_str();
        FdoStringP cname = FdoStringP::Format(L"OracleSrid%ld",(long)ora_srid);
        spatial_context = sc_collection->FindItem(cname);
        if( !spatial_context )
        {
          spatial_context = new c_KgOraSpatialContext();
          spatial_context->SetName(cname);
          
          FdoStringP csname = ora_coord_sys_name.c_str();
          spatial_context->SetCoordSysName(csname);
          
          FdoStringP cswkt = ora_coord_sys_wktext.c_str();
          spatial_context->SetCoordinateSystemWkt(cswkt);
          
          spatial_context->SetOraSridDesc(orasriddesc);
          
          long dimcount = dimlist.size();
          for(long ind=0;ind<dimcount;ind++)
          {
            SDO_DIM_ELEMENT * dimelem = dimlist[ind];
            
            string dimname = dimelem->getSdo_dimname();
            double tol = dimelem->getSdo_tolerance();
            double lb = (double)dimelem->getSdo_lb();
            double ub = (double)dimelem->getSdo_ub();
          }
          
          sc_collection->Insert(0,spatial_context);
        }
        
      }
      else
      {
      // this is no coordinate system
      // then I use DEFAULT spatial context
        if( ora_geom )
        {
          spatial_context = sc_collection->FindItem(D_SPATIALCONTEXT_DEFAULT_NAME);
          
        }
      }
      
      // Now set extents for spatial context
      double minx,miny,maxx,maxy;
      bool is_extent = false;
      bool isminmax = false;
      if( c_Ora_API::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str()) 
          ||
          (KgOraConn->GetOracleMainVersion() < 10)
        )
      {
      // read extents from user_sdo_geom_metada
        if( dimlist.size() >= 2 )
        {
          SDO_DIM_ELEMENT * dimelem;
          dimelem = dimlist[0];
          minx = (double)dimelem->getSdo_lb();
          maxx = (double)dimelem->getSdo_ub();
          
          dimelem = dimlist[1];
          miny = (double)dimelem->getSdo_lb();
          maxy = (double)dimelem->getSdo_ub();
          
          isminmax = true;
        }
      }
      else
      {     
      // this is       
        if( ora_geom )
        {
        // extend 
        // ora_geom should be of type 2003 elem_info_array (1,1003,3)
        // and 4 ordinates for lower/upper bounds
        #ifdef _DEBUG
          long gtype = (long)ora_geom->getSdo_gtype();
          
          long ei;
          for(int ind=0;ind<ora_geom->getSdo_elem_info().size();ind++)
          {
            ei =  (long)ora_geom->getSdo_elem_info()[ind];
          }
          double dval;
          for(int ind=0;ind<ora_geom->getSdo_ordinates().size();ind++)
          {
            dval =  (double)ora_geom->getSdo_ordinates()[ind];
          }
        #endif  
          if( ( (long)ora_geom->getSdo_gtype() == 2003) && ((long)ora_geom->getSdo_elem_info()[1] == 1003) )
          {
            minx = (double)ora_geom->getSdo_ordinates()[0];
            miny = (double)ora_geom->getSdo_ordinates()[1];
            maxx = (double)ora_geom->getSdo_ordinates()[2];
            maxy = (double)ora_geom->getSdo_ordinates()[3];   
            
            isminmax = true;         
          }
          else
          {
            if( ( (long)ora_geom->getSdo_gtype() == 3003) && ((long)ora_geom->getSdo_elem_info()[1] == 1003) )
            {
              minx = (double)ora_geom->getSdo_ordinates()[0];
              miny = (double)ora_geom->getSdo_ordinates()[1];
              maxx = (double)ora_geom->getSdo_ordinates()[3];
              maxy = (double)ora_geom->getSdo_ordinates()[4];
              
              isminmax = true;
            }
          }
        }
      }
      
      // Apply new extent
      if( spatial_context.p && isminmax )
      {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoEnvelopeImpl> env = FdoEnvelopeImpl::Create(minx,miny,maxx,maxy);
        
        spatial_context->ExpandExtent( env );
      }
      
      
      // Delete ora objects as there are not needed any more
      if(ora_geom )
      {
        delete ora_geom;
      }
      for( int i=0;i<dimlist.size();i++)
      {
        if( dimlist[i] ) delete dimlist[i];
      }
      dimlist.clear();
      
      
      ora_fullname = ora_tableowner + "." + ora_tablename;
      ora_fdo_classname = ora_tableowner + "~" + ora_tablename + "~" + ora_colname;
      
      FdoStringP w_fdo_classname = ora_fdo_classname.c_str();
      
      
      // check if class already exist
      // the previous sql can return multiple rows for same table.column because of multiple indexes on same column
      // so I need to check not to duplicate classes
      if( !classes->FindItem( w_fdo_classname ) )
      {
      
        FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(w_fdo_classname, L"");      
        
        FdoPtr<FdoKgOraClassDefinition> phys_class  = FdoKgOraClassDefinition::Create();
        
        phys_class->SetName( w_fdo_classname );
        phys_class->SetOracleFullTableName( FdoStringP(ora_fullname.c_str()) );
        alias_num++;
        phys_class->SetOraTableAliasNum( alias_num );
      
        FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
        FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(FdoStringP(ora_colname.c_str()), L"");              
        
        FdoInt32 fdo_geom_type = FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;
        if( (ora_layer_gtype.compare("POINT")==0) || (ora_layer_gtype.compare("MULTIPOINT")==0) )
        {
          fdo_geom_type = FdoGeometricType_Point;
        }
        else
        {
          if( (ora_layer_gtype.compare("LINE")==0) || (ora_layer_gtype.compare("MULTILINE")==0) 
              || (ora_layer_gtype.compare("CURVE")==0) || (ora_layer_gtype.compare("MULTICURVE")==0) 
            )
          {
            fdo_geom_type = FdoGeometricType_Curve;
          }
          else
          {
            if( (ora_layer_gtype.compare("POLYGON")==0) || (ora_layer_gtype.compare("MULTIPOLYGON")==0) )
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
        
        /* Query table column names */
        //DescribeTableProperties(m_OcciConnectionUserName.c_str(),ora_tablename.c_str(),pdc);
        DescribeTableProperties(occi_conn,ora_tableowner.c_str(),ora_tablename.c_str(),pdc);
        
        
        /* Check for primary keys */
        
        vector<string> pcols;
        c_OCCI_API::GetTablePkeyColumns(occi_conn,ora_tableowner.c_str(),ora_tablename.c_str(),pcols);
        if( pcols.size() > 0 )
        {
          vector<string>::iterator iter = pcols.begin();
          for ( iter = pcols.begin( ) ; iter != pcols.end( ) ; iter++ )
          {
            FdoStringP gstr = iter->c_str();
            FdoPtr<FdoDataPropertyDefinition> entid = dynamic_cast<FdoDataPropertyDefinition*>(pdc->FindItem(gstr));
            if( entid.p )
            {
            
              entid->SetNullable(false);
              FdoPtr<FdoDataPropertyDefinitionCollection> ic = fc->GetIdentityProperties();
              ic->Add( entid );
            }
  
          }
          
        }
        else
        {  
                                                          
          FdoPtr<FdoDataPropertyDefinition> entid = dynamic_cast<FdoDataPropertyDefinition*>(pdc->FindItem(L"ENTITYID"));
          if( entid.p )
          {
            FdoPtr<FdoDataPropertyDefinitionCollection> ic = fc->GetIdentityProperties();
            ic->Add( entid );
          }
        
        }
        
        classes->Add(fc);
        phys_classes->Add( phys_class );
      }
      
    }
    
    //g_FeatureSchemas = FDO_SAFE_ADDREF(m_FeatureSchemas.p);
    //g_PhysicalSchemaMapping = FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p);
    
    occi_stm->closeResultSet(occi_rs);
    occi_conn->terminateStatement (occi_stm);
  }
  catch(oracle::occi::SQLException& ea)
  {
    if( occi_stm )
    {
      if( occi_rs )
      {
        occi_stm->closeResultSet(occi_rs);
        occi_rs=NULL;
      }
      occi_conn->terminateStatement(occi_stm);
      occi_stm=NULL;
    }
    FdoStringP gstr = ea.what();
    throw FdoException::Create( gstr );
  }
    
  c_KgOraSchemaDesc* schemadata = c_KgOraSchemaDesc::Create();
  
  schemadata->SetDesc(fschema,phschema,sc_collection);
  

  
  
  return schemadata;
}//end of c_KgOraConnection::DescribeSchema



bool c_FdoOra_API::FdoPropertyToOraDataType(FdoPropertyDefinition* Property,FdoStringP& OraType)
{
  switch( Property->GetPropertyType() )
  {
    case FdoPropertyType_GeometricProperty:
    {
      OraType = "MDSYS.SDO_GEOMETRY";
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
          OraType = FdoStringP::Format(L"%s(%ld)",L"VARCHAR2",len);
        }
        break;
        case FdoDataType_Decimal:
        {
          FdoInt32 prec = propdata->GetPrecision();
          FdoInt32 scale = propdata->GetScale();
          OraType = FdoStringP::Format(L"%s(%ld,%ld)",L"NUMBER",prec,scale);
        }
        break;
        case FdoDataType_Byte:
          OraType = "CHAR(1)";
        break;
        case FdoDataType_Single:
          OraType = "BINARY_FLOAT";
        break;
        case FdoDataType_Double:
          OraType = "BINARY_DOUBLE";
        break;
        case FdoDataType_DateTime:
          OraType = "DATE";
        break;
        
        case FdoDataType_Boolean:
          OraType = "VARCHAR(5)";
        break;
        
        case FdoDataType_Int16:
          OraType = "NUMBER(10,0)";
        break;

        case FdoDataType_Int32:
          OraType = "NUMBER(10,0)";
        break;

        case FdoDataType_Int64:
          OraType = "NUMBER(10,0)";
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
}//end of c_FdoOra_API::FdoPropertyToOraDataType


