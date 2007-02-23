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
      if( Scale == 0 ) FdoType = FdoDataType_Int32;  
      else FdoType = FdoDataType_Decimal;
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
    if( Scale == 0 ) FdoType = FdoDataType_Int32;
    else FdoType = FdoDataType_Decimal;
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
        else 
        {
          scale = -1;
        }
          
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

c_KgOraSchemaDesc* c_FdoOra_API::DescribeSchema(c_KgOraConnection* KgOraConn,const char* OraSchema,const char* KingFdoViews)
{
      
        FdoPtr<FdoFeatureSchemaCollection> fschema=NULL;
        FdoPtr<FdoKgOraPhysicalSchemaMapping> phschema;
        int alias_num=0;
      
  oracle::occi::Connection * occi_conn = KgOraConn->GetOcciConnection();
      
  fschema = FdoFeatureSchemaCollection::Create(NULL);
  
  phschema = FdoKgOraPhysicalSchemaMapping::Create();
  long aliasnum=0;
  
  FdoPtr<c_KgOraSpatialContextCollection> sc_collection = new c_KgOraSpatialContextCollection();

// Create FDO classes from tables in SDO_GEOM_METADATA
{  
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"KingOra", L"");          
  fschema->Add(schema.p);
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
  
  
  oracle::occi::Statement* occi_stm = NULL;
  try
  {
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
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        " ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
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
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        " ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
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
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        " ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
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
        " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        " ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        " from all_tab_columns t, all_sdo_geom_metadata a "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        " where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name and t.owner = :1 "
        " order by a.owner, a.table_name "
        );
      }
      occi_stm->setString(1,OraSchema);
      
    }

  
    
    c_FdoOra_API::DescribeSchemaSQL(occi_conn,occi_stm,classes,phys_classes,sc_collection,aliasnum);
    
    
  }
  catch(FdoException* e)
  {
    FDO_SAFE_RELEASE(e);
    if( occi_stm )
    {
      occi_conn->terminateStatement(occi_stm);
      occi_stm=NULL;
    }
  }
  catch(oracle::occi::SQLException& ea)
  {
    if( occi_stm )
    {
      occi_conn->terminateStatement(occi_stm);
      occi_stm=NULL;
    }
    FdoStringP gstr = ea.what();
    throw FdoException::Create( gstr );
  }
  
  if( occi_stm )
  {
    occi_conn->terminateStatement(occi_stm);
    occi_stm=NULL;
  }  
}
// Create FDO classes from tables in KingFDOViews table
if( KingFdoViews && *KingFdoViews )
{  
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"KingFdoClass", L"");          
  fschema->Add(schema.p);
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
  
  
  oracle::occi::Statement* occi_stm = NULL;
  try
  {
    occi_stm = occi_conn->createStatement();
    
    
   
    // For FDO classes created from FDO View Table (king metadata table)
    // Oracle Schema connection parameter is irrelevant and not used
    //if( !OraSchema || (*OraSchema == 0) )
    {
      if( KgOraConn->GetOracleMainVersion() >= 10 )
      {
        string sqlstr,sqljoin;
        
        sqlstr = " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
            " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
            " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column ";
        
        char sqlfrom[512];
        sprintf(sqlfrom," FROM %s k ", KingFdoViews );
        
        sqljoin = " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
            " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
            " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
            " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
            " order by k.fdo_ora_owner, k.fdo_ora_name ";
        
        sqlstr = sqlstr + sqlfrom + sqljoin;
        occi_stm->setSQL( sqlstr ); 
        /*
        occi_stm->setSQL(  
        " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
        " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column "
        " from fdoclass k "
        " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        " order by k.fdo_ora_owner, k.fdo_ora_name "
        );
        */
       
        
      }
      else
      {
        string sqlstr,sqljoin;
        
        sqlstr = " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
              " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
              " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column ";
        
        char sqlfrom[512];
        sprintf(sqlfrom," FROM %s k ", KingFdoViews );
        
        sqljoin =  " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
            " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
            " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
            " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
            " order by k.fdo_ora_owner, k.fdo_ora_name ";
        
        sqlstr = sqlstr + sqlfrom + sqljoin;
        occi_stm->setSQL( sqlstr ); 
      
      /*
        occi_stm->setSQL(  
        " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
        " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column "
        " from fdoclass k "
        " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        " order by k.fdo_ora_owner, k.fdo_ora_name "
        );
      */
      }
    }
    /*
    else
    {
      if( KgOraConn->GetOracleMainVersion() >= 10 )
      {
        occi_stm->setSQL(  
        " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
        " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column "
        " from fdoclass k "
        " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        " order by k.fdo_ora_owner, k.fdo_ora_name "
        );
      }
      else
      {
         occi_stm->setSQL(  
        " select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        " ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
        " ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column "
        " from fdoclass k "
        " LEFT JOIN all_sdo_geom_metadata a ON  k.fdo_spatialtable_owner = a.owner and k.fdo_spatialtable_name = a.table_name and k.fdo_spatialtable_geomcolumn = a.column_name "
        " LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        " LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        " LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        " LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        " order by k.fdo_ora_owner, k.fdo_ora_name "
        );
      }
      //occi_stm->setString(1,OraSchema);
      
    }
    */
      
    c_FdoOra_API::DescribeSchemaSQL(occi_conn,occi_stm,classes,phys_classes,sc_collection,aliasnum);        
  }
  catch(FdoException* e)
  {
    FDO_SAFE_RELEASE(e);
    if( occi_stm )
    {
      occi_conn->terminateStatement(occi_stm);
      occi_stm=NULL;
    }
  }
  catch(oracle::occi::SQLException& ea)
  {
    if( occi_stm )
    {
      occi_conn->terminateStatement(occi_stm);
      occi_stm=NULL;
    }
    
    // Just ignore any errors
    
    //FdoStringP gstr = ea.what();
    //throw FdoException::Create( gstr );
  }
  
  if( occi_stm )
  {
    occi_conn->terminateStatement(occi_stm);
    occi_stm=NULL;
  }  
}  
  
  c_KgOraSchemaDesc* schemadata = c_KgOraSchemaDesc::Create();
  
  schemadata->SetDesc(fschema,phschema,sc_collection);
  
  
  return schemadata;
}//end of c_KgOraConnection::DescribeSchema


/*
  Creates FDO classes and adds them into FdoClassCollection.
  
  Classes are created based on records returned by sql statement 'OcciStm'.
  SQL statement is different in two cases (but returned columns are same):
    1. SQL statement which combines tables from SDO_GEOM_METADATA with dat from spatial indexes, primarykeys etc..
    2. SQL statement which combines data from table 'FdoViews' with other data from spatial index or
    
    
  Meaning of coulmns and they order as should returnd by SQL statement (OcciStm):
  
  1. Oracle Owner (varchar2, not null )  - oracle owner ftom table/view which is used for querying fdo class data
  2. Oracle Name (varchar2, not null ) - oracle name of table/view used to query fdo class data
  3. Oracle Geometry Column ( varchar2, null )  - oracle name of column used to retrieve geometry for fdo class,can be NULLL.
  
  4. SRID ( number, null ) - SRID coming from SDO_GEOM_METADATA or NULL
  5. Dimension list (SDO_DIM_INFO_ARRAY, null) - Dimension info coming from SDO_GEOM_METADATA or NULL
  
  6. CS Name ( varchar2 , null )  - Coordinate system name coming from SDO_GEOM_METADATA or NULL
  7. CS WkText ( varchar2 , null )  - Coordinate system well known text coming from SDO_GEOM_METADATA or NULL
  
  8. SpatialIndexName ( varchar2, null ) - name of spatial index created on table or null
  
  9. LayerGType (varchar2, null ) - coming from spatial index metadata or null
  
  10. SequenceName ( varchar2, null ) - name of Oracle sequence is defined which is used for inserting new rows
  
  11. MBR ( SDO_GEOMETRY, null ) - extent of geometry, comes from spatial index or null
  
  < From FDOVIEWS Table >
  
  12. Override FDO Class Name ( varchar2, null ) - name of the class which will be used as fdo class
  
  13. Override SRID ( number, null ) - override srid
  
  14. Override Dimension list ( number, null ) - override srid
  
  15. Override CS Name ( varchar2 , null )  - Coordinate system name or NULL
  16. Override CS WkText ( varchar2 , null )  - Coordinate system or NULL
  
  17. Override LayerGType (varchar2, null ) - coming from spatial index metadata or null
  
  18. Override SequenceName ( varchar2, null ) - name of Oracle sequence is defined which is used for inserting new rows
  
  19. Override Identity Coolumns ( varchar2, null ) - list of columns to be used for as identiy for class
  
  20. Override MBR ( SDO_GEOMETRY, null ) - extent of geometry or null
  
  21. FDO_POINT_X_COLUMN (varchar2, null ) this column defines names of column with values for point geometry where values are stored as numbers in columns
  22. FDO_POINT_Y_COLUMN (varchar2, null ) x,y must be define to be used  and "3. Oracle Geometry Column" must be NULL
  23. FDO_POINT_Z_COLUMN (varchar2, null ) if 3d point then it is defined
  
*/

void c_FdoOra_API::DescribeSchemaSQL(oracle::occi::Connection * OraConn,oracle::occi::Statement* OcciStm
            ,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses
            ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum      )
{
  
  oracle::occi::ResultSet* occi_rs = NULL;
  try
  {
    /* Query Oracle for geometry tables 
    */
     
    OcciStm->setPrefetchRowCount(50);
    occi_rs = OcciStm->executeQuery();
    
    bool ora_srid_isnull;
    string ora_tablename,ora_geom_colname,ora_tableowner,ora_fdo_classname,ora_index_name;
    oracle::occi::Number ora_srid;
    SDO_GEOMETRY *ora_mbr;
    string ora_coord_sys_name,ora_coord_sys_wktext,ora_layer_gtype;
    string ora_fullname,ora_sequence_name;
    
    // override's
    string override_fdo_classname;
    bool override_srid_isnull;
    oracle::occi::Number override_srid;
    SDO_GEOMETRY *override_mbr;
    string override_identity,override_sequence_name;
    string override_coord_sys_name,override_coord_sys_wktext,override_layer_gtype;
    string override_point_x_col,override_point_y_col,override_point_z_col;
     
    occi_rs->setMaxColumnSize(12,512); 
    occi_rs->setMaxColumnSize(13,512);
    occi_rs->setMaxColumnSize(14,512);
    occi_rs->setMaxColumnSize(15,512);
    occi_rs->setMaxColumnSize(16,512);
    occi_rs->setMaxColumnSize(17,512);
    occi_rs->setMaxColumnSize(18,512);
    occi_rs->setMaxColumnSize(19,512);
    occi_rs->setMaxColumnSize(20,512);
    occi_rs->setMaxColumnSize(21,512);
    occi_rs->setMaxColumnSize(22,512);
    occi_rs->setMaxColumnSize(23,512);
    
    while( occi_rs->next() != oracle::occi::ResultSet::END_OF_FETCH )
    {
      ora_tableowner = occi_rs->getString(1);
      ora_tablename = occi_rs->getString(2);
      ora_geom_colname = occi_rs->getString(3);
      
      // check if dot is in table or column name
      // in thisversion of rpovider skip those tables because fdo doesn't allow dots in names
      // TODO: change table name or column name so it doen't have dots
      if( (ora_tablename.find(".") != string::npos) || (ora_geom_colname.find(".") != string::npos) )
      {
        continue;
      }
            
      ora_srid_isnull = occi_rs->isNull(4);
      if( !ora_srid_isnull ) ora_srid = occi_rs->getNumber(4);
      else ora_srid = 0;
      
      OCCI_STD_NAMESPACE::vector< SDO_DIM_ELEMENT* > ora_dimlist;
      if( !occi_rs->isNull(5) ) oracle::occi::getVector(occi_rs,5,ora_dimlist);
      
      ora_coord_sys_name = occi_rs->getString(6);
      ora_coord_sys_wktext = occi_rs->getString(7);
      
      ora_index_name = occi_rs->getString(8);    
      
      ora_layer_gtype = occi_rs->getString(9);    
      
      ora_sequence_name = occi_rs->getString(10);    
      
      if( !occi_rs->isNull(11) ) 
        ora_mbr = (SDO_GEOMETRY*)occi_rs->getObject(11); 
      else
        ora_mbr = NULL;
      if(ora_mbr )
      {
      }
      
      override_fdo_classname = occi_rs->getString(12); // 12. Override FDO Class Name ( varchar2, null ) - name of the class which will be used as fdo class
  
      override_srid_isnull = occi_rs->isNull(13);  // 13. Override SRID ( number, null ) - override srid
      if( !override_srid_isnull ) override_srid = occi_rs->getNumber(13);
      else override_srid = 0;
      
      OCCI_STD_NAMESPACE::vector< SDO_DIM_ELEMENT* > override_dimlist;
      if( !occi_rs->isNull(14) ) 
        oracle::occi::getVector(occi_rs,14,override_dimlist);  // 14. Override Dimension list ( number, null ) - override srid
            
      override_coord_sys_name = occi_rs->getString(15); // 15. Override CS Name ( varchar2 , null )  - Coordinate system name or NULL
      override_coord_sys_wktext = occi_rs->getString(16); // 16. Override CS WkText ( varchar2 , null )  - Coordinate system or NULL
      
      override_layer_gtype = occi_rs->getString(17);  // 17. Override LayerGType (varchar2, null ) - coming from spatial index metadata or null
      
      override_sequence_name = occi_rs->getString(18); // 18. Override SequenceName ( varchar2, null ) - name of Oracle sequence is defined which is used for inserting new rows
      
      override_identity = occi_rs->getString(19); // 19. Override Identity Coolumns ( varchar2, null ) - list of columns to be used for as identiy for class
      
      if( !occi_rs->isNull(20) )    // 20. Override MBR ( SDO_GEOMETRY, null ) - extent of geometry or null
        override_mbr = (SDO_GEOMETRY*)occi_rs->getObject(20); 
      else
        override_mbr = NULL;
      
      override_point_x_col = occi_rs->getString(21); //21. FDO_POINT_X_COLUMN (varchar2, null ) this column defines names of column with values for point geometry where values are stored as numbers in columns
      override_point_y_col = occi_rs->getString(22); //22. FDO_POINT_Y_COLUMN (varchar2, null ) x,y must be define to be used  and "3. Oracle Geometry Column" must be NULL
      override_point_z_col = occi_rs->getString(23); //23. FDO_POINT_Z_COLUMN (varchar2, null ) if 3d point then it is defined
      
      
      
      
      c_KgOraSridDesc orasriddesc;
      
      orasriddesc.m_OraSrid = (long)ora_srid;
      orasriddesc.m_IsGeodetic = c_Ora_API::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str());
      
      
      // Test for coordinate system if exists and 
      FdoPtr<c_KgOraSpatialContext> spatial_context;
      if( (long)ora_srid >= 0 )
      {
        FdoStringP cname = FdoStringP::Format(L"OracleSrid%ld",(long)ora_srid);
        spatial_context = SC_Collection->FindItem(cname);
        if( !spatial_context )
        {
          spatial_context = new c_KgOraSpatialContext();
          spatial_context->SetName(cname);
          
          FdoStringP csname = ora_coord_sys_name.c_str();
          spatial_context->SetCoordSysName(csname);
          
          FdoStringP cswkt = ora_coord_sys_wktext.c_str();
          spatial_context->SetCoordinateSystemWkt(cswkt);
          
          spatial_context->SetOraSridDesc(orasriddesc);
          
          long dimcount = ora_dimlist.size();
          for(long ind=0;ind<dimcount;ind++)
          {
            SDO_DIM_ELEMENT * dimelem = ora_dimlist[ind];
            
            string dimname = dimelem->getSdo_dimname();
            double tol = dimelem->getSdo_tolerance();
            double lb = (double)dimelem->getSdo_lb();
            double ub = (double)dimelem->getSdo_ub();
          }
          
          SC_Collection->Insert(0,spatial_context);
        }
        
      }
      else
      {
      // this is no coordinate system
      // then I use DEFAULT spatial context
        if( ora_mbr )
        {
          spatial_context = SC_Collection->FindItem(D_SPATIALCONTEXT_DEFAULT_NAME);       
        }
      }
      
      // Now set extents for spatial context
      double minx,miny,maxx,maxy;
      bool isminmax = false;
      if( c_Ora_API::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str()) 
          ||
          (ora_mbr == NULL)
          //(KgOraConn->GetOracleMainVersion() < 10)
        )
      {
      // read extents from user_sdo_geom_metada
        if( ora_dimlist.size() >= 2 )
        {
          SDO_DIM_ELEMENT * dimelem;
          dimelem = ora_dimlist[0];
          minx = (double)dimelem->getSdo_lb();
          maxx = (double)dimelem->getSdo_ub();
          
          dimelem = ora_dimlist[1];
          miny = (double)dimelem->getSdo_lb();
          maxy = (double)dimelem->getSdo_ub();
          
          isminmax = true;
        }
      }
      else
      {     
      // this is       
        if( ora_mbr )
        {
        // extend 
        // ora_geom should be of type 2003 elem_info_array (1,1003,3)
        // and 4 ordinates for lower/upper bounds
        #ifdef _DEBUG
          long gtype = (long)ora_mbr->getSdo_gtype();
          
          long ei;
          for(int ind=0;ind<ora_mbr->getSdo_elem_info().size();ind++)
          {
            ei =  (long)ora_mbr->getSdo_elem_info()[ind];
          }
          double dval;
          for(int ind=0;ind<ora_mbr->getSdo_ordinates().size();ind++)
          {
            dval =  (double)ora_mbr->getSdo_ordinates()[ind];
          }
        #endif  
          if( ( (long)ora_mbr->getSdo_gtype() == 2003) && ((long)ora_mbr->getSdo_elem_info()[1] == 1003) )
          {
            minx = (double)ora_mbr->getSdo_ordinates()[0];
            miny = (double)ora_mbr->getSdo_ordinates()[1];
            maxx = (double)ora_mbr->getSdo_ordinates()[2];
            maxy = (double)ora_mbr->getSdo_ordinates()[3];   
            
            isminmax = true;         
          }
          else
          {
            if( ( (long)ora_mbr->getSdo_gtype() == 3003) && ((long)ora_mbr->getSdo_elem_info()[1] == 1003) )
            {
              minx = (double)ora_mbr->getSdo_ordinates()[0];
              miny = (double)ora_mbr->getSdo_ordinates()[1];
              maxx = (double)ora_mbr->getSdo_ordinates()[3];
              maxy = (double)ora_mbr->getSdo_ordinates()[4];
              
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
      if(ora_mbr )
      {
        delete ora_mbr;
      }
      for( int i=0;i<ora_dimlist.size();i++)
      {
        if( ora_dimlist[i] ) delete ora_dimlist[i];
      }
      ora_dimlist.clear();
      
      if(override_mbr )
      {
        delete override_mbr;
      }
      for( int i=0;i<override_dimlist.size();i++)
      {
        if( override_dimlist[i] ) delete override_dimlist[i];
      }
      override_dimlist.clear();
      
      
      if( ora_tableowner.length() > 0 )
        ora_fullname = ora_tableowner + "." + ora_tablename;
      else
        ora_fullname = ora_tablename;
      
      ora_fdo_classname = ora_tableowner + "~" + ora_tablename + "~" + ora_geom_colname;
      
      FdoStringP w_fdo_classname = ora_fdo_classname.c_str();
      
      if( override_fdo_classname.length() > 0 )
      {
        w_fdo_classname = override_fdo_classname.c_str();
      }
      
      
      // check if class already exist
      // the previous sql can return multiple rows for same table.column because of multiple indexes on same column
      // so I need to check not to duplicate classes
      if( !FdoClasses->FindItem( w_fdo_classname ) )
      {
      
        FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(w_fdo_classname, L"");      
        
        FdoPtr<FdoKgOraClassDefinition> phys_class  = FdoKgOraClassDefinition::Create();
        
        phys_class->SetName( w_fdo_classname );
        phys_class->SetOracleFullTableName( FdoStringP(ora_fullname.c_str()) );
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
        
        // check if layer type has been override
        string comb_layer_gtype;        
        comb_layer_gtype = ora_layer_gtype;
        if( override_layer_gtype.length() > 0 ) comb_layer_gtype = override_layer_gtype;
        
        if( (comb_layer_gtype.compare("POINT")==0) || (comb_layer_gtype.compare("MULTIPOINT")==0) )
        {
          fdo_geom_type = FdoGeometricType_Point;
        }
        else
        {
          if( (comb_layer_gtype.compare("LINE")==0) || (comb_layer_gtype.compare("MULTILINE")==0) 
              || (comb_layer_gtype.compare("CURVE")==0) || (comb_layer_gtype.compare("MULTICURVE")==0) 
            )
          {
            fdo_geom_type = FdoGeometricType_Curve;
          }
          else
          {
            if( (comb_layer_gtype.compare("POLYGON")==0) || (comb_layer_gtype.compare("MULTIPOLYGON")==0) )
            {
              fdo_geom_type = FdoGeometricType_Surface;
            }
          }  
        }
        if( ora_geom_colname.length() > 0 )
        {
          FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(FdoStringP(ora_geom_colname.c_str()), L"");                        
        
          gpd->SetGeometryTypes(fdo_geom_type);  
          
          if( spatial_context )
          {
            gpd->SetSpatialContextAssociation( spatial_context->GetName() );
          }
          
          pdc->Add(gpd);
          
          fc->SetGeometryProperty(gpd);
        }
        else
        {
        // Geometry property is not defined - check if x,y columns are
          if( (override_point_x_col.length() > 0) && (override_point_y_col.length() > 0) )
          {
            FdoStringP pointproperty = L"KING_FDO_POINT";
            FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(pointproperty, L"");                        
        
            fdo_geom_type = FdoGeometricType_Point;
            gpd->SetGeometryTypes(fdo_geom_type);  
            
            if( spatial_context )
            {
              gpd->SetSpatialContextAssociation( spatial_context->GetName() );
            }
            
            pdc->Add(gpd);
            
            fc->SetGeometryProperty(gpd);
            
            FdoStringP xcol = override_point_x_col.c_str();
            FdoStringP ycol = override_point_y_col.c_str();
            FdoStringP zcol = override_point_z_col.c_str();
            phys_class->SetPointGeometry(pointproperty,xcol,ycol,zcol);
          }
        }
    ////////////////////////////////////////////////////////////////////////////////////////////
    //  Add other non-geometry sttributes from table  
    //////////////////////////////////////////////////////////////////////////////////////////// 
        DescribeTableProperties(OraConn,ora_tableowner.c_str(),ora_tablename.c_str(),pdc);
        
                
    ////////////////////////////////////////////////////////////////////////////////////////////
    //  Check for primary keys
    //////////////////////////////////////////////////////////////////////////////////////////// 
        vector<string> pcols;
        
        // ckeck for user defined column for identity
        // if defined use it - if not look for primary keys on table
        if( override_identity.length() > 0 )
        {
        // transfering from string to FdoStrinCollection and then back to string
        // this was written in a hurry - could be better
          FdoPtr<FdoStringCollection> strcol = FdoStringCollection::Create(override_identity.c_str(),L",");
          long cnt = strcol->GetCount();
          for(long ind=0;ind<cnt;ind++)
          {
            FdoStringP fdostr = strcol->GetString(ind);
            string ss;
            ss = fdostr;
            pcols.push_back(ss);
          }     
        }
        else
        {
          c_OCCI_API::GetTablePkeyColumns(OraConn,ora_tableowner.c_str(),ora_tablename.c_str(),pcols);
        }
        if( pcols.size() > 0 )
        {
          bool isidentity_int=true;
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
          
         // If primary ky is one int column and if there is sequence TableName_FDOSEQ
         // then this sequence will be use to populuate identity id during inserts
          if( (pcols.size() == 1) && (isidentity_int = true) && (ora_sequence_name.length()>0) )
          {
            FdoStringP fdostr = ora_sequence_name.c_str();
            phys_class->SetUseSequenceForIdentity(fdostr);
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
        
        FdoClasses->Add(fc);
        PhysClasses->Add( phys_class );
      }
      
    }
    
    //g_FeatureSchemas = FDO_SAFE_ADDREF(m_FeatureSchemas.p);
    //g_PhysicalSchemaMapping = FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p);
    
    OcciStm->closeResultSet(occi_rs);
    
  }
  catch(oracle::occi::SQLException& ea)
  {
    if( OcciStm )
    {
      if( occi_rs )
      {
        OcciStm->closeResultSet(occi_rs);
        occi_rs=NULL;
      }
      
    }
    FdoStringP gstr = ea.what();
    throw FdoException::Create( gstr );
  }
}//end of c_FdoOra_API::DescribeSchemaSQL

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


