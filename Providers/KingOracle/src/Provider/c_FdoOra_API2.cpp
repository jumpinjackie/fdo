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
#include "c_OCI_API.h"
//#include "c_Ora_API2.h"
#include "KingOracle/FdoKgOraOverrides.h"
#include "c_KgOraSridDesc.h"
#include "c_KgOraSpatialContext.h"
#include "c_KgOraSchemaDesc.h"
#include "c_FdoOra_API2.h"
#include "c_Ora_API2.h"
#include "KgOraProvider.h"

c_FdoOra_API2::c_FdoOra_API2(void)
{
}

c_FdoOra_API2::~c_FdoOra_API2(void)
{
}


bool c_FdoOra_API2::OraTypeToFdoDataType(ub2 OCiDataType,int Scale,int Length,FdoDataType & FdoType)
{
  bool isfdotype=true;
  switch( OCiDataType )
  {
    case OCI_TYPECODE_VARCHAR2:  // varchar2
    case OCI_TYPECODE_VARCHAR:  // varchar2
      FdoType = FdoDataType_String;   
    break;
    
    case OCI_TYPECODE_NUMBER:  // NUMBER
    case OCI_TYPECODE_DECIMAL:  // NUMBER
      if( Scale == 0 ) FdoType = FdoDataType_Int32;  
      else FdoType = FdoDataType_Decimal;
    break;
    
    case OCI_TYPECODE_CHAR: // char
      if( Length==1 ) FdoType = FdoDataType_Byte;
      else FdoType = FdoDataType_String;
    break;
    
    case OCI_TYPECODE_INTEGER:
      FdoType = FdoDataType_Int32;
    break;
    
    case OCI_TYPECODE_SMALLINT:
      FdoType = FdoDataType_Int16;
    break;

    case OCI_TYPECODE_BFLOAT:
    case OCI_TYPECODE_FLOAT:
      FdoType = FdoDataType_Single;  
    break;
    case OCI_TYPECODE_BDOUBLE:
    case OCI_TYPECODE_DOUBLE:
    case OCI_TYPECODE_REAL:
      FdoType = FdoDataType_Double;  
    break;    
    
    case OCI_TYPECODE_DATE:
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
}//end of c_FdoOra_API2::OraTypeToFdoDataType

/*
bool c_FdoOra_API2::FdoDataTypeToOraDataType(FdoDataType DataType,oracle::occi::Type& OraType)
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
}//end of c_FdoOra_API2::FdoDataTypeToOraDataType
*/




bool c_FdoOra_API2::SetOracleStatementData(c_Oci_Statement*  Statement,int SqlParamNum,FdoDataValue* DataValue)
{
  
  switch( DataValue->GetDataType() )
  {
    case FdoDataType_Boolean:
    {
      
      if( DataValue->IsNull() )
        Statement->BindString(SqlParamNum,NULL);
      else
      {
        FdoBooleanValue * boolval = (FdoBooleanValue*)DataValue;
        string val;
        if( boolval->GetBoolean() )
          Statement->BindStringValue(SqlParamNum,L"1");          
        else
          Statement->BindStringValue(SqlParamNum,L"0");
          
        
      }
    }
    break;

    /// Represents unsigned 8-bit integers with values between 0 and 255.
    case FdoDataType_Byte:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindString(SqlParamNum,NULL);
      else
      {
        FdoByteValue * byteval = (FdoByteValue*)DataValue;
        wchar_t chstr[2];
        chstr[0] = byteval->GetByte();
        chstr[1] = 0;
        
        Statement->BindStringValue(SqlParamNum,chstr);
      }
    }  
    break;

    /// Represents a date and time value.
    case FdoDataType_DateTime:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindDate(SqlParamNum,NULL);
      else
      {
        FdoDateTimeValue * dateval = (FdoDateTimeValue*)DataValue;
        
        FdoDateTime date = dateval->GetDateTime();
        OCIDate val;
        val.OCIDateYYYY = date.year;
        val.OCIDateMM = date.month;
        val.OCIDateDD = date.day;
        val.OCIDateTime.OCITimeHH = date.hour;
        val.OCIDateTime.OCITimeMI = date.minute;
        val.OCIDateTime.OCITimeSS = (ub1)date.seconds;
        
        Statement->BindDateValue(SqlParamNum,val);
      }
    }
    break;

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
    case FdoDataType_Decimal:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindDouble(SqlParamNum,NULL);
      else
      {
        FdoDecimalValue * dval = (FdoDecimalValue*)DataValue;
        
        Statement->BindDoubleValue(SqlParamNum,dval->GetDecimal());
      }
    }
    break;

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
    case FdoDataType_Double:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindDouble(SqlParamNum,NULL);
      else
      {
        FdoDoubleValue * dval = (FdoDoubleValue*)DataValue;                        
        Statement->BindDoubleValue(SqlParamNum,dval->GetDouble());
      }
    }
    break;

    /// Represents signed 16-bit integers with values between -32768 and 32767.
    case FdoDataType_Int16:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindInt(SqlParamNum,NULL);
      else
      {
        FdoInt16Value * dval = (FdoInt16Value*)DataValue;                        
        Statement->BindIntValue(SqlParamNum,dval->GetInt16());
      }
    }
    break;

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
    case FdoDataType_Int32:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindInt(SqlParamNum,NULL);
      else
      {
        FdoInt32Value * dval = (FdoInt32Value*)DataValue;                        
        Statement->BindIntValue(SqlParamNum,dval->GetInt32());
      }
    }
    break;

    /// Represents signed 64-bit integers with values 
    /// between -9223372036854775808 and 9223372036854775807.
    case FdoDataType_Int64:
    {
      
      if( DataValue->IsNull() )
        Statement->BindLong(SqlParamNum,NULL);
      else
      {
        FdoInt64Value * dval = (FdoInt64Value*)DataValue;                        
        
        //long dval
        
        Statement->BindLongValue(SqlParamNum,(long)dval->GetInt64());
      }
    }
    break;

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
    case FdoDataType_Single:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindDouble(SqlParamNum,NULL);
      else
      {
        FdoSingleValue * dval = (FdoSingleValue*)DataValue;                        
        Statement->BindDoubleValue(SqlParamNum,dval->GetSingle());
      }
    }
    break;

    /// Represents a Unicode character strings.
    case FdoDataType_String:      
    {
      
      if( DataValue->IsNull() )
        Statement->BindString(SqlParamNum,NULL);
      else
      {
        FdoStringValue * dval = (FdoStringValue*)DataValue;                        
        
        Statement->BindString(SqlParamNum,dval->GetString());
      }
    }
    break;

    /// Represents a binary large object stored as a collection of bytes.
    case FdoDataType_BLOB:
    {
      Statement->BindBlob(SqlParamNum,NULL,0);
    }
    break;

    /// Represents a character large object stored as a collection of
    /// characters.
    case FdoDataType_CLOB:
    {
      Statement->BindClob(SqlParamNum,NULL,0);
    }
    break;
    
    default:
      return false;
    break;    
  }
 
  return true;
}//end of c_FdoOra_API2::SetOracleStatementData


bool c_FdoOra_API2::OraTypeToFdoDataType(const char* OraType,int Scale,int Length,FdoDataType & FdoType)
{
  
  bool isfdotype=false;
  
  if( FdoCommonOSUtil::stricmp(OraType,"VARCHAR2") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"VARCHAR") == 0 )
  {
    FdoType = FdoDataType_String;      
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"NUMBER") == 0 )
  {            
    if( Scale == 0 ) FdoType = FdoDataType_Int32;
    else FdoType = FdoDataType_Decimal;
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"CHAR") == 0 )
  {            
    if( Length==1 ) FdoType = FdoDataType_Byte;
    else FdoType = FdoDataType_String;
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"BINARY_FLOAT") == 0 )
  {            
    FdoType = FdoDataType_Single;    
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"BINARY_DOUBLE") == 0 )
  {            
    FdoType = FdoDataType_Double;    
    isfdotype=true;
  } else
  if( FdoCommonOSUtil::stricmp(OraType,"DATE") == 0 )
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
}//end of c_FdoOra_API2::OraTypeToFdoDataType


/* 
  Utility function to create property definitions from table columns 
*/
bool c_FdoOra_API2::DescribeTableProperties(c_Oci_Connection * OciConn,const wchar_t*Schema,const wchar_t*TableName,FdoPropertyDefinitionCollection* PropCollection)
{
  OCIParam *parmh = (OCIParam *) 0;         /* parameter handle */
  OCIParam *collsthd = (OCIParam *) 0;      /* handle to list of columns */
  OCIParam *colhd = (OCIParam *) 0;         /* column handle */
  OCIDescribe *dschp = (OCIDescribe *)0;      /* describe handle */

try
{

  OCIHandleAlloc((dvoid *)OciConn->m_OciHpEnvironment, (dvoid **)&dschp,
    (ub4)OCI_HTYPE_DESCRIBE, (size_t)0, (dvoid **)0);

  /* get the describe handle for the table */
  std::wstring fullname(Schema);
  fullname.append(L".");
  fullname.append(TableName);
  //int namelen = strlen(fullname.c_str());
  
  int status = OCIDescribeAny(OciConn->m_OciHpServiceContext, OciConn->m_OciHpError, (dvoid *)fullname.c_str(), wcslen(fullname.c_str())*sizeof(wchar_t), OCI_OTYPE_NAME, 0,
    OCI_PTYPE_TABLE, dschp);
    
  if( status != OCI_SUCCESS )
  {
    int status =  OCIDescribeAny(OciConn->m_OciHpServiceContext, OciConn->m_OciHpError, (dvoid *)fullname.c_str(), wcslen(fullname.c_str())*sizeof(wchar_t), OCI_OTYPE_NAME, 0,
    OCI_PTYPE_VIEW, dschp);
    
    if( status != OCI_SUCCESS )
    {
      return false;
    }
  }
    

  /* get the parameter handle */
  OciConn->OciCheckError(OCIAttrGet((dvoid *)dschp, OCI_HTYPE_DESCRIBE, (dvoid *)&parmh, (ub4 *)0,
    OCI_ATTR_PARAM, OciConn->m_OciHpError));
    

  /* The type information of the object, in this case, OCI_PTYPE_TABLE,
  is obtained from the parameter descriptor returned by the OCIAttrGet(). */
  /* get the number of columns in the table */
  int numcols = 0;
  OciConn->OciCheckError(OCIAttrGet((dvoid *)parmh, OCI_DTYPE_PARAM, (dvoid *)&numcols, (ub4 *)0,
    OCI_ATTR_NUM_COLS, OciConn->m_OciHpError));
    

  /* get the handle to the column list of the table */
  OciConn->OciCheckError(OCIAttrGet((dvoid *)parmh, OCI_DTYPE_PARAM, (dvoid *)&collsthd, (ub4 *)0,
    OCI_ATTR_LIST_COLUMNS, OciConn->m_OciHpError)==OCI_NO_DATA);
    

  /* go through the column list and retrieve the data-type of each column,
  and then recursively describe column types. */

  for (int i = 1; i <= numcols; i++)
  {
    /* get parameter for column i */
    OciConn->OciCheckError(OCIParamGet((dvoid *)collsthd, OCI_DTYPE_PARAM, OciConn->m_OciHpError, (dvoid **)&colhd, (ub4)i));
      

    ub2 col_type;
      OciConn->OciCheckError(OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM, (dvoid *)&col_type, (ub4 *)0,
      OCI_ATTR_DATA_TYPE, OciConn->m_OciHpError));

    /* for example, get datatype for ith column */
    wchar_t *col_name=NULL;
    int col_name_len=0;
    OciConn->OciCheckError(OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM, (dvoid *)&col_name, (ub4 *)&col_name_len,
      OCI_ATTR_NAME, OciConn->m_OciHpError));
      

    /* Retrieve the length semantics for the column */
    ub4 char_semantics = 0;
    OCIAttrGet((dvoid*) colhd, (ub4) OCI_DTYPE_PARAM,
      (dvoid*) &char_semantics,(ub4 *) 0, (ub4) OCI_ATTR_CHAR_USED,
      (OCIError *) OciConn->m_OciHpError);

    ub4 col_width = 0;
    if (char_semantics)
      /* Retrieve the column width in characters */
      OCIAttrGet((dvoid*) colhd, (ub4) OCI_DTYPE_PARAM,
      (dvoid*) &col_width, (ub4 *) 0, (ub4) OCI_ATTR_CHAR_SIZE,
      (OCIError *) OciConn->m_OciHpError);
    else
      /* Retrieve the column width in bytes */
      OCIAttrGet((dvoid*) colhd, (ub4) OCI_DTYPE_PARAM,
      (dvoid*) &col_width,(ub4 *) 0, (ub4) OCI_ATTR_DATA_SIZE,
      (OCIError *) OciConn->m_OciHpError);
      
    // Retrieve the column precision
    ub1 col_precision;
    OCIAttrGet((dvoid*) colhd, (ub4) OCI_DTYPE_PARAM,
      (dvoid*) &col_precision,(ub4 *) 0, (ub4) OCI_ATTR_PRECISION,
      (OCIError *) OciConn->m_OciHpError);
      
    // Retrieve the column size
    ub1 col_scale;
    OCIAttrGet((dvoid*) colhd, (ub4) OCI_DTYPE_PARAM,
      (dvoid*) &col_scale,(ub4 *) 0, (ub4) OCI_ATTR_SCALE,
      (OCIError *) OciConn->m_OciHpError);
      
    FdoDataType fdotype;      
    bool isfdotype = c_FdoOra_API2::OraTypeToFdoDataType(col_type,col_scale,col_width,fdotype);

    if( isfdotype )
    {            
      
      FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(col_name, L"");
      dpd->SetDataType(fdotype);
      dpd->SetLength(col_width);
      dpd->SetPrecision(col_precision);
      dpd->SetScale(col_scale);

      PropCollection->Add(dpd);
    }      
  }

  if (dschp)
    OCIHandleFree((dvoid *) dschp, OCI_HTYPE_DESCRIBE);


    
}
catch(c_Oci_Exception* exc)
{
  if (dschp)
    OCIHandleFree((dvoid *) dschp, OCI_HTYPE_DESCRIBE);
  FdoStringP gstr = exc->GetErrorText();
  delete exc;
  throw FdoException::Create( gstr );
}
  return true;
}//end of c_FdoOra_API2::DescribeTableProperties




/*
select a.srid, a.table_name as geomtablename, a.column_name as geomcolumnname, 
b.cs_name, b.wktext, 
c.sdo_dimname as dname, c.sdo_lb as dmin, c.sdo_ub as dmax, c.sdo_tolerance as dtol 
from ALL_SDO_GEOM_METADATA a, MDSYS.CS_SRS b, table(a.diminfo) c 
where a.srid = b.srid (+) and a.owner = :1 ;
*/
//FdoPtr<FdoFeatureSchemaCollection> g_FeatureSchemas(NULL);
//FdoPtr<FdoKgOraPhysicalSchemaMapping>  g_PhysicalSchemaMapping(NULL);

c_KgOraSchemaDesc* c_FdoOra_API2::DescribeSchema(c_Oci_Connection* OciConn,const wchar_t* ConnectionOraSchema,const wchar_t* UseOraSchema,const wchar_t* KingFdoViews,const wchar_t* SdeSchema)
{
      
        FdoPtr<FdoFeatureSchemaCollection> fschema;
        FdoPtr<FdoKgOraPhysicalSchemaMapping> phschema;
        int alias_num=0;
      
  
  bool isoracle9=false;    
  fschema = FdoFeatureSchemaCollection::Create(NULL);
  
  phschema = FdoKgOraPhysicalSchemaMapping::Create();
  long aliasnum=0;
  
  FdoPtr<c_KgOraSpatialContextCollection> sc_collection = new c_KgOraSpatialContextCollection();



// Create FDO classes from SDE tables
{
  if( SdeSchema && *SdeSchema )
  {
    FdoPtr<FdoFeatureSchema> schema;
    
    schema = (FdoFeatureSchema*)fschema->FindItem(L"KingOra");
    if( !schema.p)
    {        
      schema = FdoFeatureSchema::Create(L"KingOra", L"");          
      fschema->Add(schema.p);
    }
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
    
    bool bind_owner = false;
    c_FdoOra_API2::DescribeSchemaSDE(OciConn,UseOraSchema, classes,phys_classes,sc_collection,aliasnum);
  }
}

// Create FDO classes from tables in SDO_GEOM_METADATA
if( OciConn->IsSdoTypes() )
{  
  FdoPtr<FdoFeatureSchema> schema;

  schema = (FdoFeatureSchema*)fschema->FindItem(L"KingOra");
  if( !schema.p)
  {        
    schema = FdoFeatureSchema::Create(L"KingOra", L"");          
    fschema->Add(schema.p);
  }
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
  
  
  
  try
  {
    
    
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
    
    std::wstring sqlquery ;
    int mainversion,subversion;
    c_Ora_API2::GetOracleVersion(OciConn,mainversion,subversion);
    
    bool bind_owner = false;
    
    // this is for prior 10.2 version (works also with 10.2)
    if( !UseOraSchema || (*UseOraSchema == 0) )
    {
      if( mainversion >= 10 )
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
        
        /*
        sqlquery = \
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_tab_columns t, all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        L" where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name "
        L" order by a.owner, a.table_name ";
        */
        sqlquery = \
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') ORDER BY a.owner, a.table_name ";
        
       
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
        /*
        sqlquery=\
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_tab_columns t, all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        L" where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name "
        L" order by a.owner, a.table_name ";
        */
        sqlquery=\
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') order by a.owner, a.table_name";
        
        isoracle9=true;
      }
      
      // SELECT SDO_AGGR_MBR(geom) FROM curvepolygon
    }
    else
    {
      if( mainversion >= 10 )
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
        
        /*
        sqlquery=\
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_tab_columns t, all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        L" where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name and t.owner = :1 "
        L" order by a.owner, a.table_name ";
        */
        //if( _wcsicmp(ConnectionOraSchema,UseOraSchema) == 0 )
        if( FdoCommonOSUtil::wcsicmp(ConnectionOraSchema, UseOraSchema)==0 )
        {
          sqlquery=\
          L" select NULL, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
          L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
          L" from user_sdo_geom_metadata a "
          L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
          L" LEFT JOIN user_SDO_INDEX_INFO c ON  a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
          L" LEFT JOIN user_SDO_INDEX_METADATA d ON c.index_name = d.sdo_index_name "
          L" LEFT JOIN user_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') order by a.table_name";
        }
        else
        {
          sqlquery=\
          L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
          L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
          L" from all_sdo_geom_metadata a "
          L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
          L" LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
          L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
          L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
          L" where a.owner = :1 order by a.owner, a.table_name";
          
          bind_owner = true;
        }
        
        
      }
      else
      {
      //----------------------------------------------------------
      // For Oracle version before 10 and UseOraSchema is set
      //----------------------------------------------------------
      /*
          occi_stm->setSQL(  
            " select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT , c.index_name "
            " from all_sdo_geom_metadata a, MDSYS.CS_SRS b, ALL_SDO_INDEX_INFO c, ALL_SDO_INDEX_METADATA d "
            " where a.owner = :1 and a.srid = b.srid(+) " // this line is different for oracle 10 nad 9
            " and a.table_name = c.table_name and c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            " order by a.owner, a.table_name"
            );
      */
      /*
        sqlquery=\
        L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
        L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
        L" from all_tab_columns t, all_sdo_geom_metadata a "
        L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
        L" LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
        L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
        L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
        L" where t.owner = a.owner and t.table_name=a.table_name and t.column_name = a.column_name and t.owner = :1 "
        L" order by a.owner, a.table_name ";
        */
        
        //if( wcscmp(ConnectionOraSchema,UseOraSchema) == 0 )
        if( FdoCommonOSUtil::wcsicmp(ConnectionOraSchema, UseOraSchema)==0 )
        {
          sqlquery=\
          L" select NULL, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
          L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
          L" from user_sdo_geom_metadata a "
          L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
          L" LEFT JOIN user_SDO_INDEX_INFO c ON a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
          L" LEFT JOIN user_SDO_INDEX_METADATA d ON c.index_name = d.sdo_index_name "
          L" LEFT JOIN user_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') order by a.table_name";
          
          bind_owner = false;
        }
        else
        {
          sqlquery=\
          L" select a.owner, a.table_name, a.column_name, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
          L" ,NULL o1 ,NULL o2 ,NULL o3 ,NULL o4 ,NULL o5 ,NULL o6 ,NULL o7 ,NULL o8 ,NULL o9 ,NULL o10, NULL o111 ,NULL o12 "
          L" from all_sdo_geom_metadata a "
          L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
          L" LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name and a.COLUMN_NAME = c.COLUMN_NAME  "
          L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
          L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
          L" where a.owner = :1 order by a.owner, a.table_name";
          
          bind_owner = true;
        }
        isoracle9=true;
      }
      
      
    }

  
    
    c_FdoOra_API2::DescribeSchemaSQL(OciConn,sqlquery.c_str(),bind_owner,ConnectionOraSchema,UseOraSchema,classes,phys_classes,sc_collection,aliasnum,isoracle9);
    
    
    
  }
  /*
  catch(FdoException* e)
  {
    FDO_SAFE_RELEASE(e);
    
    
  }
  */
  catch(c_Oci_Exception* ea)
  {
    FdoStringP gstr = ea->GetErrorText();
    delete ea;
    throw FdoException::Create( gstr );
  }
 
}
// Create FDO classes from tables in KingFDOViews table
if( OciConn->IsSdoTypes() && KingFdoViews && *KingFdoViews )
{ 
  // FdoPtr<FdoFeatureSchema> schema = fschema->FindItem(L"KingFdoClass"); 
  // If I set different schema than when selecting feature in MG, it will not select one feature
  // but it will render all features blue. Workaround was to have one just one schema
  
  FdoPtr<FdoFeatureSchema> schema = fschema->FindItem(L"KingOra"); 
  
  if( schema.p == NULL )
  {
    schema = FdoFeatureSchema::Create(L"KingOra", L"");          
    fschema->Add(schema.p);
  }
  
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoKgOraClassCollection> phys_classes = phschema->GetClasses();
  
  
  
  try
  {
    
    
    
    int mainversion,subversion;
    c_Ora_API2::GetOracleVersion(OciConn,mainversion,subversion);
    std::wstring sqlstr;
    
    // For FDO classes created from FDO View Table (king metadata table)
    // Oracle Schema connection parameter is irrelevant and not used
    //if( !OraSchema || (*OraSchema == 0) )
    {
      if( mainversion >= 10 )
      {
        std::wstring sqljoin;
        
        sqlstr = L" select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, d.SDO_ROOT_MBR  "
            L" ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
            L" ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column ";
        
        FdoStringP sqlfrom = FdoStringP::Format(L" FROM %s k ", KingFdoViews);
        
        sqljoin = L" LEFT JOIN all_sdo_geom_metadata a ON  UPPER(k.FDO_SPATIALTABLE_OWNER) = a.owner and UPPER(k.FDO_SPATIALTABLE_NAME) = a.table_name and UPPER(k.FDO_SPATIALTABLE_GEOMCOLUMN) = a.column_name "
            L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
            L" LEFT JOIN ALL_SDO_INDEX_INFO c ON  a.owner = c.table_owner and a.table_name = c.table_name "
            L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
            L" order by k.fdo_ora_owner, k.fdo_ora_name ";
        
        sqlstr = sqlstr + (FdoString*)sqlfrom + sqljoin;
        
       
       
        
      }
      else
      {
        std::wstring sqljoin;
        
        sqlstr = L" select k.fdo_ora_owner, k.fdo_ora_name, k.fdo_ora_geomcolumn, a.srid, a.diminfo, b.CS_NAME, b.WKTEXT, c.index_name, d.sdo_layer_gtype, s.sequence_name, NULL SDO_ROOT_MBR "
              L" ,k.fdo_class_name, k.fdo_srid, k.fdo_diminfo, k.fdo_cs_name, k.fdo_wktext, k.fdo_layer_gtype, k.fdo_sequence_name, k.fdo_identity, k.fdo_sdo_root_mbr "
              L" ,k.fdo_point_x_column ,k.fdo_point_y_column ,k.fdo_point_z_column ";
        
        FdoStringP sqlfrom = FdoStringP::Format(L" FROM %s k ", KingFdoViews);
        
        sqljoin =  L" LEFT JOIN all_sdo_geom_metadata a ON  UPPER(k.FDO_SPATIALTABLE_OWNER) = a.owner and UPPER(k.FDO_SPATIALTABLE_NAME) = a.table_name and UPPER(k.FDO_SPATIALTABLE_GEOMCOLUMN) = a.column_name "
            L" LEFT JOIN MDSYS.CS_SRS b ON  a.srid = b.srid "
            L" LEFT JOIN ALL_SDO_INDEX_INFO c ON a.table_name = c.table_name "
            L" LEFT JOIN ALL_SDO_INDEX_METADATA d ON c.sdo_index_owner = d.sdo_index_owner and c.index_name = d.sdo_index_name "
            L" LEFT JOIN all_sequences s on s.sequence_name = CONCAT(a.table_name,'_FDOSEQ') "
            L" order by k.fdo_ora_owner, k.fdo_ora_name ";
        
        sqlstr = sqlstr + (FdoString*)sqlfrom + sqljoin;
        
      
      
      }
    }
    
      
    c_FdoOra_API2::DescribeSchemaSQL(OciConn,sqlstr.c_str(),false,ConnectionOraSchema,NULL, classes,phys_classes,sc_collection,aliasnum,isoracle9);
    
    
  }
  catch(FdoException* e)
  {
    FDO_SAFE_RELEASE(e);
    
  }
  catch(c_Oci_Exception* ea)
  {
    
    
    // Just ignore any errors
    
    FdoStringP gstr = ea->what();
    //throw FdoException::Create( gstr );
  }
  
   
}  
  
  c_KgOraSchemaDesc* schemadata = c_KgOraSchemaDesc::Create();
  
  schemadata->SetDesc(fschema,phschema,sc_collection);
  
  
  return schemadata;
}//end of c_KgOraConnection::DescribeSchema



/*
void c_FdoOra_API2::DescribeSchemaSQL(c_Oci_Connection * OciConn,const wchar_t*SqlString,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses
                                      ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum,bool IsOracle9)
{
  clock_t elog_t1=clock();
  
  // initialize stmthp 
  //OCIStmt 		*stmthp=NULL;
  //OciConn->OciCheckError( OCIHandleAlloc((dvoid *)OciConn->m_OciHpEnvironment, (dvoid **)&stmthp, (ub4)OCI_HTYPE_STMT, (size_t)0, (dvoid **)0));
  c_Oci_Statement stm(OciConn);
  // parse query 
  
  std::wstring sql( L"SELECT owner,table_name FROM ALL_SDO_GEOM_METADATA " );
  OciConn->OciCheckError( OCIStmtPrepare(stm.m_OciHpStm, OciConn->m_OciHpError, 
    (text *)sql.c_str(), (ub4)wcslen(sql.c_str())*sizeof(wchar_t), 
    (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT));
    
  // table name
  wchar_t owner[128];
  wchar_t table_name[128];
  OCIDefine *defn1p;
  OciConn->OciCheckError( OCIDefineByPos(stm.m_OciHpStm, &defn1p, OciConn->m_OciHpError, (ub4)1, 
    (dvoid *)&owner[0], 
    (sb4)sizeof(owner), SQLT_STR,
    (dvoid *)0, (ub2 *)0, (ub2 *)0, 
    (ub4)OCI_DEFAULT));      
  OCIDefine *defn2p;
  OciConn->OciCheckError( OCIDefineByPos(stm.m_OciHpStm, &defn2p, OciConn->m_OciHpError, (ub4)2, 
    (dvoid *)&table_name[0], 
    (sb4)sizeof(table_name), SQLT_STR,
    (dvoid *)0, (ub2 *)0, (ub2 *)0, 
    (ub4)OCI_DEFAULT));        
    
  // execute 
  int status = OCIStmtExecute(OciConn->m_OciHpServiceContext, stm.m_OciHpStm, OciConn->m_OciHpError, (ub4)1, (ub4)0, 
    (OCISnapshot *)NULL, (OCISnapshot *)NULL, 
    (ub4)OCI_DEFAULT);

  bool has_more_data;
  int rows_fetched;
  if (status == OCI_SUCCESS_WITH_INFO || status == OCI_NO_DATA)
  {
    has_more_data = FALSE;
  }
  else
  {
    has_more_data = TRUE;
    OciConn->OciCheckError( status);
  }
  
   
  OciConn->OciCheckError( OCIAttrGet((dvoid *)stm.m_OciHpStm, (ub4)OCI_HTYPE_STMT,
    (dvoid *)&rows_fetched, (ub4 *)0, 
    (ub4)OCI_ATTR_ROW_COUNT, OciConn->m_OciHpError));
  
  
  int tablenum = 0;
  while( rows_fetched>0 )
  {
    tablenum++;
    
    FdoPtr<FdoPropertyDefinitionCollection> col_propdef = FdoPropertyDefinitionCollection::Create(NULL);
    DescribeTableProperties(OciConn,owner,table_name,col_propdef);
    status = OCIStmtFetch2(stm.m_OciHpStm, OciConn->m_OciHpError, (ub4)1, 
      (ub2)OCI_FETCH_NEXT, 1,(ub4)OCI_DEFAULT);
      
    if( status == OCI_SUCCESS)      
      OciConn->OciCheckError(OCIAttrGet((dvoid *)stm.m_OciHpStm, (ub4)OCI_HTYPE_STMT,
        (dvoid *)&rows_fetched, (ub4 *)0, 
        (ub4)OCI_ATTR_ROW_COUNT, OciConn->m_OciHpError));          
    else
      rows_fetched = 0;
          
  }
  

  
      
}
*/


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

void c_FdoOra_API2::DescribeSchemaSQL(c_Oci_Connection * OciConn,const wchar_t* SqlString,bool BindOwner
            ,const wchar_t* ConnectionOraSchema,const wchar_t* Owner
            ,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses
            ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum,bool IsOracle9      )
{
  c_Oci_Statement * stm=NULL;
  try
  {
    // Query Oracle for geometry tables 

    stm = OciConn->CreateStatement();
    stm->Prepare(SqlString);
    if( BindOwner && Owner )
    {
      stm->BindString(1,Owner);
    }
    stm->ExecuteSelectAndDefine();
    
    bool ora_srid_isnull;
    std::wstring ora_tablename,ora_geom_colname,ora_tableowner,ora_fdo_classname,ora_index_name;
    long ora_srid;
    c_SDO_GEOMETRY *ora_mbr=NULL;
    std::wstring ora_coord_sys_name,ora_coord_sys_wktext,ora_layer_gtype;
    std::wstring ora_fullname,ora_sequence_name;
    
    // override's
    std::wstring override_fdo_classname;
    bool override_srid_isnull;
    long override_srid;
    SDO_GEOMETRY_TYPE *override_mbr;
    std::wstring override_identity,override_sequence_name;
    std::wstring override_coord_sys_name,override_coord_sys_wktext,override_layer_gtype;
    std::wstring override_point_x_col,override_point_y_col,override_point_z_col;
   
   /*
     
    if( IsOracle9 )
    {
      occi_rs->setMaxColumnSize(11,512);
    }      
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
    */
    
    int rec_count=0;
    while( stm->ReadNext() )
    {
      rec_count++;
      if( stm->IsColumnNull(1) )
      {
        if( Owner )
          ora_tableowner = Owner;
        else
          ora_tableowner = L"";
      }
      else
      {
        ora_tableowner = stm->GetString(1);
      }
      
      ora_tablename = stm->IsColumnNull(2) ? L"" : stm->GetString(2);
      ora_geom_colname = stm->IsColumnNull(3) ? L"" : stm->GetString(3);
      
      
      // check if dot is in table or column name
      // in this version of provider skip those tables because fdo doesn't allow dots in names
      // TODO: change table name or column name so it doen't have dots
      if( (ora_tablename.find(L".") != std::wstring::npos) || (ora_geom_colname.find(L".") != std::wstring::npos) )
      {
        continue;
      }
            
      ora_srid_isnull = stm->IsColumnNull(4);
      if( !ora_srid_isnull ) ora_srid = stm->GetLong(4);
      else ora_srid = 0;
      
      //std::vector< SDO_DIM_ELEMENT* > ora_dimlist;
      c_SDO_DIM_ARRAY ora_dimlist;
      if( !stm->IsColumnNull(5) ) 
      {
        ora_dimlist = stm->GetSdoDimArray(5);
        //oracle::occi::getVector(occi_rs,5,ora_dimlist);
      }
      
      ora_coord_sys_name = stm->IsColumnNull(6) ? L"" : stm->GetString(6);
      ora_coord_sys_wktext = stm->IsColumnNull(7) ? L"" : stm->GetString(7);
      
      ora_index_name = stm->IsColumnNull(8) ? L"" : stm->GetString(8);    
      
      ora_layer_gtype = stm->IsColumnNull(9) ? L"" : stm->GetString(9);    
      
      ora_sequence_name = stm->IsColumnNull(10) ? L"" : stm->GetString(10);    
      
      if( !stm->IsColumnNull(11) ) 
        ora_mbr = stm->GetSdoGeom(11); 
      else
        ora_mbr = NULL;
      if(ora_mbr )
      {
      }
      
      override_fdo_classname = stm->IsColumnNull(12) ? L"" : stm->GetString(12); // 12. Override FDO Class Name ( varchar2, null ) - name of the class which will be used as fdo class
  
      override_srid_isnull = stm->IsColumnNull(13);  // 13. Override SRID ( number, null ) - override srid
      if( !override_srid_isnull ) override_srid = stm->GetLong(13);
      else override_srid = 0;
      
      //OCCI_STD_NAMESPACE::vector< SDO_DIM_ELEMENT* > override_dimlist;
      c_SDO_DIM_ARRAY override_dimlist;
      if( !stm->IsColumnNull(14) ) 
      {
        //oracle::occi::getVector(occi_rs,14,override_dimlist);  // 14. Override Dimension list ( number, null ) - override srid
        override_dimlist = stm->GetSdoDimArray(14);
      }
            
      override_coord_sys_name = stm->IsColumnNull(15) ? L"" : stm->GetString(15); // 15. Override CS Name ( varchar2 , null )  - Coordinate system name or NULL
      override_coord_sys_wktext = stm->IsColumnNull(16) ? L"" : stm->GetString(16); // 16. Override CS WkText ( varchar2 , null )  - Coordinate system or NULL
      
      override_layer_gtype = stm->IsColumnNull(17) ? L"" : stm->GetString(17);  // 17. Override LayerGType (varchar2, null ) - coming from spatial index metadata or null
      
      override_sequence_name = stm->IsColumnNull(18) ? L"" : stm->GetString(18); // 18. Override SequenceName ( varchar2, null ) - name of Oracle sequence is defined which is used for inserting new rows
      
      override_identity = stm->IsColumnNull(19) ? L"" : stm->GetString(19); // 19. Override Identity Coolumns ( varchar2, null ) - list of columns to be used for as identiy for class
      
      if( !stm->IsColumnNull(20) )    // 20. Override MBR ( SDO_GEOMETRY, null ) - extent of geometry or null
        override_mbr = (SDO_GEOMETRY_TYPE*)stm->GetSdoGeom(20); 
      else
        override_mbr = NULL;
      
      override_point_x_col = stm->IsColumnNull(21) ? L"" : stm->GetString(21); //21. FDO_POINT_X_COLUMN (varchar2, null ) this column defines names of column with values for point geometry where values are stored as numbers in columns
      override_point_y_col = stm->IsColumnNull(22) ? L"" : stm->GetString(22); //22. FDO_POINT_Y_COLUMN (varchar2, null ) x,y must be define to be used  and "3. Oracle Geometry Column" must be NULL
      override_point_z_col = stm->IsColumnNull(23) ? L"" : stm->GetString(23); //23. FDO_POINT_Z_COLUMN (varchar2, null ) if 3d point then it is defined
      
      
      
      
      c_KgOraSridDesc orasriddesc;
      
      if( !override_srid_isnull ) 
      {
        ora_srid = override_srid;
      }
      if( override_coord_sys_wktext.length() > 0 )
      {
        ora_coord_sys_wktext = override_coord_sys_wktext;
      }
      if( override_coord_sys_name.length() > 0 )
      {
        ora_coord_sys_name = override_coord_sys_name;
      }
      
      orasriddesc.m_OraSrid = (long)ora_srid;
      orasriddesc.m_IsGeodetic = c_Ora_API2::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str());
      
      
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
          
          long dimcount = ora_dimlist.GetSize();
          for(long ind=0;ind<dimcount;ind++)
          {
            c_SDO_DIM_ELEMENT dimelem = ora_dimlist.GetDimElement(ind);
            
            std::wstring dimname;
            if( !dimelem.IsNullDimName() )
              dimname = dimelem.GetDimName(); 
                          
            double tol = dimelem.GetTolerance();
            double lb = (double)dimelem.GetLB();
            double ub = (double)dimelem.GetUB();
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
      if( c_Ora_API2::IsGeodeticCoordSystem(ora_coord_sys_wktext.c_str()) 
          ||
          (ora_mbr == NULL)
          //(KgOraConn->GetOracleMainVersion() < 10)
        )
      {
      // read extents from user_sdo_geom_metada
        if( ora_dimlist.GetSize() >= 2 )
        {
          c_SDO_DIM_ELEMENT dimelem;
          dimelem = ora_dimlist.GetDimElement(0);
          
          minx = dimelem.GetLB();
          maxx = dimelem.GetUB();
          
          dimelem = ora_dimlist.GetDimElement(1);
          miny = dimelem.GetLB();
          maxy = dimelem.GetUB();
          
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
       
          if( ( (long)ora_mbr->GetSdoGtype() == 2003) && ((long)ora_mbr->GetSdoElemInfo(1) == 1003) )
          {
            minx = (double)ora_mbr->GetSdoOrdinate(0);
            miny = (double)ora_mbr->GetSdoOrdinate(1);
            maxx = (double)ora_mbr->GetSdoOrdinate(2);
            maxy = (double)ora_mbr->GetSdoOrdinate(3);
            
            isminmax = true;         
          }
          else
          {
            if( ( (long)ora_mbr->GetSdoGtype() == 3003) && ((long)ora_mbr->GetSdoElemInfo(1) == 1003) )
            {
              minx = (double)ora_mbr->GetSdoOrdinate(0);
              miny = (double)ora_mbr->GetSdoOrdinate(1);
              maxx = (double)ora_mbr->GetSdoOrdinate(3);
              maxy = (double)ora_mbr->GetSdoOrdinate(4);
              
              isminmax = true;
            }
          }
        }
      }
      
      // Apply new extent
      if( spatial_context.p && isminmax )
      {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIEnvelope> env = gf->CreateEnvelopeXY(minx, miny, maxx, maxy);
        
        spatial_context->ExpandExtent( env );
      }
      
      
      // Delete ora objects as there are not needed any more
      
      
      /*
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
      */
      
      if( ora_tableowner.length() > 0 )
        ora_fullname = ora_tableowner + L"." + ora_tablename;
      else
        ora_fullname = ora_tablename;
      
      ora_fdo_classname = ora_tableowner + L"~" + ora_tablename + L"~" + ora_geom_colname;
      
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
        
        
        if( ora_mbr )
        {
          if( orasriddesc.m_IsGeodetic )
          {
          // for now I am setting spatial extent for geodetic layer maximum big
          // beacuse SDO_ROOT_MBR has some special values for geodetic layers
          // this will be changed when i know how to calculate it
            phys_class->SetSdoRootMBR(L"MDSYS.SDO_GEOMETRY(2003,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,3),MDSYS.SDO_ORDINATE_ARRAY(-180,-90,180,90))");
          }
          else
          {
            wchar_t* buff = c_Ora_API2::SdoGeomToStringW(ora_mbr);
            phys_class->SetSdoRootMBR(buff);
            delete []buff;
          }
        }
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
        std::wstring comb_layer_gtype;        
        comb_layer_gtype = ora_layer_gtype;
        if( override_layer_gtype.length() > 0 ) comb_layer_gtype = override_layer_gtype;
        
        if( (comb_layer_gtype.compare(L"POINT")==0) || (comb_layer_gtype.compare(L"MULTIPOINT")==0) )
        {
          fdo_geom_type = FdoGeometricType_Point;
        }
        else
        {
          if( (comb_layer_gtype.compare(L"LINE")==0) || (comb_layer_gtype.compare(L"MULTILINE")==0) 
              || (comb_layer_gtype.compare(L"CURVE")==0) || (comb_layer_gtype.compare(L"MULTICURVE")==0) 
            )
          {
            fdo_geom_type = FdoGeometricType_Curve;
          }
          else
          {
            if( (comb_layer_gtype.compare(L"POLYGON")==0) || (comb_layer_gtype.compare(L"MULTIPOLYGON")==0) )
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
    
        // Because there could be table names in sdo_geom_metadata which doesn't exists as tables or views
        // catch erro returned
        bool table_exists=false;
        
        table_exists = DescribeTableProperties(OciConn,ora_tableowner.c_str(),ora_tablename.c_str(),pdc);
                
                
                
    ////////////////////////////////////////////////////////////////////////////////////////////
    //  Check for primary keys
    //////////////////////////////////////////////////////////////////////////////////////////// 
        std::vector<std::wstring> pcols;
        
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
            std::wstring ss;
            ss = fdostr;
            pcols.push_back(ss);
          }     
        }
        else
        {
          //if( BindOwner )
          if( FdoCommonOSUtil::wcsicmp(ConnectionOraSchema, ora_tableowner.c_str()) != 0 )          
            c_OCI_API::GetTablePkeyColumns(OciConn,ora_tableowner.c_str(),ora_tablename.c_str(),pcols);
          else
            c_OCI_API::GetTablePkeyColumns(OciConn,NULL,ora_tablename.c_str(),pcols);
        }
        if( pcols.size() > 0 )
        {
          bool isidentity_int=true;
          std::vector<std::wstring>::iterator iter = pcols.begin();
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
          
       ////////////////////////////////////////////////////////////////////////////////////////////
       //  Set Oracle Sequence
       ////////////////////////////////////////////////////////////////////////////////////////////  
         // If primary ky is one int column and if there is sequence TableName_FDOSEQ
         // then this sequence will be use to populate identity id during inserts
          std::wstring comb_sequence_name;        
          comb_sequence_name = ora_sequence_name;
          if( override_sequence_name.length() > 0 ) comb_sequence_name = override_sequence_name;
          
          if( (pcols.size() == 1) && (isidentity_int = true) && (comb_sequence_name.length()>0) )
          {
            FdoStringP fdostr = comb_sequence_name.c_str();
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
      
      if(ora_mbr )
        delete ora_mbr;
      
    }
    
    //g_FeatureSchemas = FDO_SAFE_ADDREF(m_FeatureSchemas.p);
    //g_PhysicalSchemaMapping = FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p);
    
    int class_count = FdoClasses->GetCount();
    
    OciConn->TerminateStatement(stm);
    
  }
  catch(c_Oci_Exception* ex)
  {
    FdoStringP gstr = ex->GetErrorText();
     #ifdef _KGORA_EXTENDED_LOG  
      D_KGORA_ELOG_WRITE1("c_FdoOra_API2::DescribeSchemaSQL.Error : '%s'",(const char*)gstr);      
     #endif
    delete ex;
    if( stm )
    {
      OciConn->TerminateStatement(stm);
      
    }
    
    throw FdoException::Create( gstr );
  }
}//end of c_FdoOra_API2::DescribeSchemaSQL

void c_FdoOra_API2::DescribeSchemaSDE(c_Oci_Connection * OciConn,const wchar_t* Owner
                                      ,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses
                                      ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum )
{
  c_Oci_Statement * stm=NULL;
  try
  {
    // Run Query for layers fdo SDE.layers table 
    std::wstring sqlquery = \
    L"SELECT l.layer_id, l.owner, l.table_name, l.spatial_column"
    L" ,g.g_table_schema,g.g_table_name,g.geometry_type,g.coord_dimension"
    L" ,r.srid,r.srtext,r.falsex,r.falsey,r.xyunits,r.falsez,r.zunits,r.falsem,r.munits" 
    L" FROM sde.layers l INNER JOIN (sde.geometry_columns g INNER JOIN sde.spatial_references r ON g.srid = r.srid)" 
    L" ON l.table_name = g.f_table_name and l.owner = g.f_table_schema";

    if( Owner && *Owner )
    {
      sqlquery = sqlquery + L" where l.owner='";
      sqlquery = sqlquery + Owner;
      sqlquery = sqlquery + L"'";
      //stm->BindString(1,Owner);
    }
    
    stm = OciConn->CreateStatement();
    stm->Prepare(sqlquery.c_str());
    
    
    stm->ExecuteSelectAndDefine();

    std::wstring ora_tablename,ora_geom_colname,ora_tableowner,ora_fdo_classname,ora_index_name;
    long sde_srid;
    c_SDO_GEOMETRY *ora_mbr=NULL;
    std::wstring sde_coord_sys_name,sde_coord_sys_wktext;
    double sde_falsex,sde_falsey,sde_falsez,sde_falsem;
    double sde_xyunits,sde_zunits,sde_munits;
    std::wstring ora_fullname,ora_sequence_name;

    
    
    long sde_layer_id;
    std::wstring sde_geom_owner,sde_geom_table,sde_featurekey_colname;
    std::wstring  sde_full_geometry_table_name,sde_full_index_table_name;
    long sde_geom_type,sde_geom_coord_dim;
    
    int duplicate_rec=0;
    int rec_count=0;
    while( stm->ReadNext() )
    {
      rec_count++;
      
      if( stm->IsColumnNull(1) )
      {      
        continue;
      }
      else
      {
        sde_layer_id = stm->GetLong(1);
      }
      
      if( stm->IsColumnNull(2) )
      {
        if( Owner )
          ora_tableowner = Owner;
        else
          ora_tableowner = L"";
      }
      else
      {
        ora_tableowner = stm->GetString(2);
      }

      ora_tablename = stm->IsColumnNull(3) ? L"" : stm->GetString(3);
      sde_featurekey_colname = stm->IsColumnNull(4) ? L"" : stm->GetString(4);
      ora_geom_colname = L"FdoGeom";
      
      if( ora_tablename.empty() || ora_geom_colname.empty() ) continue;

      // check if dot is in table or column name
      // in this version of provider skip those tables because fdo doesn't allow dots in names
      // TODO: change table name or column name so it doen't have dots
      if( (ora_tablename.find(L".") != std::wstring::npos) || (ora_geom_colname.find(L".") != std::wstring::npos) )
      {
        continue;
      }


      sde_geom_owner = stm->IsColumnNull(5) ? L"" : stm->GetString(5);
      sde_geom_table = stm->IsColumnNull(6) ? L"" : stm->GetString(6);
      
      sde_geom_type = stm->IsColumnNull(7) ? 0 : stm->GetLong(7);
      sde_geom_coord_dim = stm->IsColumnNull(8) ? 0 : stm->GetLong(8);

      sde_full_geometry_table_name = sde_geom_owner + L"." + sde_geom_table;
      FdoStringP temp2 = FdoStringP::Format(L"S%ld",sde_layer_id);
      sde_full_index_table_name = temp2;
      sde_full_index_table_name = sde_geom_owner + L".S" + sde_full_index_table_name;

      sde_srid = stm->IsColumnNull(9) ? 0 : stm->GetLong(9);
      sde_coord_sys_wktext = stm->IsColumnNull(10) ? L"" : stm->GetString(10);

      FdoStringP temp = FdoStringP::Format(L"SDE_SRID_%ld",sde_srid);
      sde_coord_sys_name = temp;
      
      sde_falsex = stm->IsColumnNull(11) ? 0 : stm->GetDouble(11);
      sde_falsey = stm->IsColumnNull(12) ? 0 : stm->GetDouble(12);
      sde_xyunits = stm->IsColumnNull(13) ? 1 : stm->GetDouble(13);
      
      sde_falsez = stm->IsColumnNull(14) ? 0 : stm->GetDouble(14);
      sde_zunits = stm->IsColumnNull(15) ? 1 : stm->GetDouble(15);
      
      sde_falsem = stm->IsColumnNull(16) ? 0 : stm->GetDouble(16);
      sde_munits = stm->IsColumnNull(17) ? 1 : stm->GetDouble(16);
      
      

      
      ora_sequence_name = FdoStringP::Format(L"I%ld",sde_srid);

     

      c_KgOraSridDesc orasriddesc;

      

      orasriddesc.m_OraSrid = (long)sde_srid;
      orasriddesc.m_IsGeodetic = c_Ora_API2::IsGeodeticCoordSystem(sde_coord_sys_wktext.c_str());
      orasriddesc.m_SDE_FalseX = sde_falsex;
      orasriddesc.m_SDE_FalseY = sde_falsey;
      orasriddesc.m_SDE_XYUnit = sde_xyunits;
      orasriddesc.m_SDE_FalseZ = sde_falsez;
      orasriddesc.m_SDE_ZUnit = sde_zunits;
      orasriddesc.m_SDE_FalseM = sde_falsem;
      orasriddesc.m_SDE_MUnit = sde_munits;
      
      


      // Test for coordinate system if exists and 
      FdoPtr<c_KgOraSpatialContext> spatial_context;
      if( (long)sde_srid >= 0 )
      {
        FdoStringP cname = FdoStringP::Format(L"SdeSrid%ld",(long)sde_srid);
        spatial_context = SC_Collection->FindItem(cname);
        if( !spatial_context )
        {
          spatial_context = new c_KgOraSpatialContext();
          spatial_context->SetName(cname);

          FdoStringP csname = sde_coord_sys_name.c_str();
          spatial_context->SetCoordSysName(csname);

          FdoStringP cswkt = sde_coord_sys_wktext.c_str();
          spatial_context->SetCoordinateSystemWkt(cswkt);

          spatial_context->SetOraSridDesc(orasriddesc);

         
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
      double minx=D_SPATIALCONTEXT_DEFAULT_MINX,miny=D_SPATIALCONTEXT_DEFAULT_MINY;
      double maxx=D_SPATIALCONTEXT_DEFAULT_MAXX,maxy=D_SPATIALCONTEXT_DEFAULT_MAXY;
      
      bool isminmax = false;
      

      // Apply new extent
      if( spatial_context.p && isminmax )
      {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIEnvelope> env = gf->CreateEnvelopeXY(minx, miny, maxx, maxy);

        spatial_context->ExpandExtent( env );
      }


      // Delete ora objects as there are not needed any more



      if( ora_tableowner.length() > 0 )
        ora_fullname = ora_tableowner + L"." + ora_tablename;
      else
        ora_fullname = ora_tablename;

      ora_fdo_classname = ora_tableowner + L"~" + ora_tablename + L"~" + ora_geom_colname;

      FdoStringP w_fdo_classname = ora_fdo_classname.c_str();

      

      // check if class already exist
      // the previous sql can return multiple rows for same table.column because of multiple indexes on same column
      // so I need to check not to duplicate classes
      if( !FdoClasses->FindItem( w_fdo_classname ) )
      {

        FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(w_fdo_classname, L"");      

        FdoPtr<FdoKgOraClassDefinition> phys_class  = FdoKgOraClassDefinition::Create();


        if( ora_mbr )
        {
          if( orasriddesc.m_IsGeodetic )
          {
            // for now I am setting spatial extent for geodetic layer maximum big
            // beacuse SDO_ROOT_MBR has some special values for geodetic layers
            // this will be changed when i know how to calculate it
            phys_class->SetSdoRootMBR(L"MDSYS.SDO_GEOMETRY(2003,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,3),MDSYS.SDO_ORDINATE_ARRAY(-180,-90,180,90))");
          }
          else
          {
            wchar_t* buff = c_Ora_API2::SdoGeomToStringW(ora_mbr);
            phys_class->SetSdoRootMBR(buff);
            delete []buff;
          }
        }
        phys_class->SetName( w_fdo_classname );
        phys_class->SetOracleFullTableName( FdoStringP(ora_fullname.c_str()) );
        AliasNum++;
        phys_class->SetOraTableAliasNum( AliasNum );
        
        phys_class->SetSdeClass(true,sde_featurekey_colname.c_str(),sde_full_geometry_table_name.c_str(),sde_geom_type,sde_full_index_table_name.c_str());

        FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();


        ////////////////////////////////////////////////////////////////////////////////////////////
        //  Create Geometry Property
        ////////////////////////////////////////////////////////////////////////////////////////////       
        //
        //  Define Layer Geometry Type  
        //
        FdoInt32 fdo_geom_type = FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;
        switch(sde_geom_type)
        {
          case 0: // 0 = Geometry
          case 6: // 6 = collection
            fdo_geom_type = FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;
          break;
          case 1: // 1 = point
          case 7: // 7 = multipoint
            fdo_geom_type = FdoGeometricType_Point;
          break;
          case 2: // 2 = curve
          case 3: // 3 = linestring
          case 8: // 8 = multicurve
          case 9: // 9 = multilinestring
            fdo_geom_type = FdoGeometricType_Curve;
          break;
          
          case 4: // 4 = surface
          case 5: // 5 = polygon
          case 10: // 10 = multisurface
          case 11: // 11 = multipolygon
            fdo_geom_type = FdoGeometricType_Surface;
          break;
        }

        
        
        if( ora_geom_colname.length() > 0 )
        {
          //FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(FdoStringP(ora_geom_colname.c_str()), L"");
          FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(ora_geom_colname.c_str(), L"");                        

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

        // Because there could be table names in sdo_geom_metadata which doesn't exists as tables or views
        // catch erro returned
        bool table_exists=false;

        table_exists = DescribeTableProperties(OciConn,ora_tableowner.c_str(),ora_tablename.c_str(),pdc);



        ////////////////////////////////////////////////////////////////////////////////////////////
        //  Check for primary keys
        //////////////////////////////////////////////////////////////////////////////////////////// 
        /*
        std::vector<std::wstring> pcols;

        if( BindOwner )
          c_OCI_API::GetTablePkeyColumns(OciConn,ora_tableowner.c_str(),ora_tablename.c_str(),pcols);
        else
          c_OCI_API::GetTablePkeyColumns(OciConn,NULL,ora_tablename.c_str(),pcols);
        
        if( pcols.size() > 0 )
        */
        {
          bool isidentity_int=true;
          
          FdoPtr<FdoDataPropertyDefinition> entid = dynamic_cast<FdoDataPropertyDefinition*>(pdc->FindItem(sde_featurekey_colname.c_str()));
          if( entid.p )
          {
            FdoPtr<FdoDataPropertyDefinitionCollection> ic = fc->GetIdentityProperties();
            ic->Add( entid );
          }     
          
          
          
          ////////////////////////////////////////////////////////////////////////////////////////////
          //  Set Oracle Sequence
          ////////////////////////////////////////////////////////////////////////////////////////////  
          // If primary ky is one int column and if there is sequence TableName_FDOSEQ
          // then this sequence will be use to populate identity id during inserts
          std::wstring comb_sequence_name;        
          comb_sequence_name = ora_sequence_name;
          /*
          if( (pcols.size() == 1) && (isidentity_int = true) && (comb_sequence_name.length()>0) )
          {
            FdoStringP fdostr = comb_sequence_name.c_str();
            phys_class->SetUseSequenceForIdentity(fdostr);
          } 
          */         
        }
       

        FdoClasses->Add(fc);
        PhysClasses->Add( phys_class );
      }
      else
      {
        duplicate_rec++;
      
      }
      
      if(ora_mbr )
        delete ora_mbr;

    }

    //g_FeatureSchemas = FDO_SAFE_ADDREF(m_FeatureSchemas.p);
    //g_PhysicalSchemaMapping = FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p);

    int class_count = FdoClasses->GetCount();

    OciConn->TerminateStatement(stm);

  }
  catch(c_Oci_Exception* ex)
  {
    FdoStringP gstr = ex->GetErrorText();
#ifdef _KGORA_EXTENDED_LOG  
    D_KGORA_ELOG_WRITE1("c_FdoOra_API2::DescribeSchemaSQL.Error : '%s'",(const char*)gstr);      
#endif
    delete ex;
    if( stm )
    {
      OciConn->TerminateStatement(stm);

    }

    throw FdoException::Create( gstr );
  }
}//end of c_FdoOra_API2::DescribeSchemaSDE


bool c_FdoOra_API2::FdoPropertyToOraDataType(FdoPropertyDefinition* Property,FdoStringP& OraType)
{
  switch( Property->GetPropertyType() )
  {
    case FdoPropertyType_GeometricProperty:
    {
      OraType = L"MDSYS.SDO_GEOMETRY";
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
          // 	ORACLE:
          //If you specify a negative scale, then Oracle rounds the actual data to
          //the specified number of places to the left of the decimal point. For
          //example, specifying (7,-2) means Oracle rounds to the nearest
          //hundredths.
          //
          //,456,123.89
          //NUMBER(7,-2)
          //7456100 
          
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
        }
        break;
        case FdoDataType_Byte:
          OraType = L"CHAR(1)";
        break;
        case FdoDataType_Single:
          OraType = L"BINARY_FLOAT";
        break;
        case FdoDataType_Double:
          OraType = L"BINARY_DOUBLE";
        break;
        case FdoDataType_DateTime:
          OraType = L"DATE";
        break;
        
        case FdoDataType_Boolean:
          OraType = L"VARCHAR(5)";
        break;
        
        case FdoDataType_Int16:
          OraType = L"NUMBER(10,0)";
        break;

        case FdoDataType_Int32:
          OraType = L"NUMBER(10,0)";
        break;

        case FdoDataType_Int64:
          OraType = L"NUMBER(10,0)";
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
}//end of c_FdoOra_API2::FdoPropertyToOraDataType


