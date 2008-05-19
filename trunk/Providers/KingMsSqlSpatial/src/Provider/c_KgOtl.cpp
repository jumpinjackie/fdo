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
#include "c_KgOtl.h"

c_KgOtl::c_KgOtl(void)
{
}

c_KgOtl::~c_KgOtl(void)
{
}

void c_KgOtl::Init()
{
  otl_connect::otl_initialize(); // initialize ODBC environment
}

void c_KgOtl::Open(const wchar_t* UserName,const wchar_t* Password,const wchar_t* DSN)
{
  
  if( !UserName || !Password || !DSN ) 
  {
    throw c_KgOtlException(L"Invalid Open Parameters (NULL Values)!");
  }
   
  try
  {   
  
  
  m_OtlDB.rlogon(UserName,Password,DSN); 
  m_OtlDB.set_max_long_size(D_MAXIMUM_VARBINARY_SIZE); // set maximum long string size for connect object
  
  }
  catch(otl_exception& p)
  { // intercept OTL exceptions  
   
    throw c_KgOtlException(p.msg,p.stm_text);
    //cerr<<p.msg<<endl; // print out error message  
    //cerr<<p.stm_text<<endl; // print out SQL that caused the error  
    //cerr<<p.sqlstate<<endl; // print out SQLSTATE message  
    //cerr<<p.var_info<<endl; // print out the variable that caused the error 
  }
}

void c_KgOtl::Close()
{
try
{
  m_OtlDB.logoff();
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
 
  throw c_KgOtlException(p.msg,p.stm_text);
}
}//end of c_KgOtl::Close


long c_KgOtl::ExecuteSql(const char* Sql)
{
try
{
  return otl_cursor::direct_exec   (    m_OtlDB,    Sql); 
}
catch(otl_exception& p)
{ // intercept OTL exceptions  
 
  throw c_KgOtlException(p.msg,p.stm_text);
}

}//end of c_KgOtl::ExecuteSql

otl_stream* c_KgOtl::CreateStream(int BuffSize,const char* SqlStm,const int implicit_select)
{
  try
  {
    otl_stream* stream = new otl_stream(BuffSize,SqlStm,m_OtlDB,implicit_select);
    return stream;
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
  
}//end of c_KgOtl::CreateStream

void c_KgOtl::OpenStream(otl_stream*& Stream,int BuffSize,const char* SqlStm,const int implicit_select)
{
  try
  {
    Stream->open(BuffSize,SqlStm,m_OtlDB,implicit_select);
    
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
  
}//end of c_KgOtl::CreateStream

void c_KgOtl::DeleteStream(otl_stream*& Stream)
{
  if( !Stream ) return;
  
  try
  {
    delete Stream;
    Stream = NULL;    
  }
   catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}
void c_KgOtl::SetCommit(otl_stream* Stream,int Commit)
{
  if( !Stream ) return;
  
  try
  {
    Stream->set_commit(Commit);
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}
void c_KgOtl::FlushStream(otl_stream* Stream)
{
  if( !Stream ) return;
  
  try
  {
    Stream->flush();
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}


long c_KgOtl::GetRpc(otl_stream* Stream)
{
  if( !Stream ) return false;
  
  try
  {
    
    return Stream->get_rpc();
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}
void c_KgOtl::Commit()
{
  try
  {   
    m_OtlDB.commit();
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}


  
  

bool c_KgOtl::SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue)
{
try
{  
  switch( DataValue->GetDataType() )
  {
    case FdoDataType_Boolean:
    {
      
    }
    break;

    /// Represents unsigned 8-bit integers with values between 0 and 255.
    case FdoDataType_Byte:      
    {
      
      
    }  
    break;

    /// Represents a date and time value.
    case FdoDataType_DateTime:      
    {
      otl_value<TIMESTAMP_STRUCT> oval;
      
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoDateTimeValue * dateval = (FdoDateTimeValue*)DataValue;
        FdoDateTime date = dateval->GetDateTime();
        
        
        TIMESTAMP_STRUCT val;
        
        val.year = date.year;
        val.month = date.month;
        val.day = date.day;
        val.hour = date.hour;
        val.minute = date.minute;
        val.second = date.seconds;
        oval = val;
        //oval = dval->GetDecimal();        
      }
      
      *Stream<<oval;            
    }
    break;

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
    case FdoDataType_Decimal:      
    {
      otl_value<double> oval;
      
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoDecimalValue * dval = (FdoDecimalValue*)DataValue;
        
        oval = dval->GetDecimal();        
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
    case FdoDataType_Double:      
    {
      
     otl_value<double> oval;
      
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoDecimalValue * dval = (FdoDecimalValue*)DataValue;
        
        oval = dval->GetDecimal();        
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents signed 16-bit integers with values between -32768 and 32767.
    case FdoDataType_Int16:      
    {
      otl_value<short> oval; 
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoInt16Value * dval = (FdoInt16Value*)DataValue;                        
        oval = dval->GetInt16();
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
    case FdoDataType_Int32:      
    {
      otl_value<int> oval; 
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoInt32Value * dval = (FdoInt32Value*)DataValue;                        
        oval = dval->GetInt32();
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents signed 64-bit integers with values 
    /// between -9223372036854775808 and 9223372036854775807.
    case FdoDataType_Int64:
    {
      otl_value<OTL_BIGINT> oval; 
      
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoInt64Value * dval = (FdoInt64Value*)DataValue;                        
        
        oval = dval->GetInt64();                
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
    case FdoDataType_Single:      
    {
      otl_value<float> oval;
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoSingleValue * dval = (FdoSingleValue*)DataValue;                        
        oval = dval->GetSingle();
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents a Unicode character strings.
    case FdoDataType_String:      
    {
      otl_value<string> oval;
      if( DataValue->IsNull() )
        oval.set_null();
      else
      {
        FdoStringValue * dval = (FdoStringValue*)DataValue;                        
        
        
        FdoStringP usval = dval->GetString();
        oval = (const char*)usval;
        
      }
      
      *Stream<<oval;
    }
    break;

    /// Represents a binary large object stored as a collection of bytes.
    case FdoDataType_BLOB:
    {
      //Statement->setNull(SqlParamNum,oracle::occi::OCCIBLOB);
    }
    break;

    /// Represents a character large object stored as a collection of
    /// characters.
    case FdoDataType_CLOB:
    {
      //Statement->setNull(SqlParamNum,oracle::occi::OCCICLOB);
    }
    break;
    
    default:
      return false;
    break;    
  }
 
  return true;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}  
}//end of c_KgOtl::SetStreamData  
  
  
bool c_KgOtl::SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb)
{

try
{  
  int count;
  FdoByte *data;
  if( !GeometryWkb )
  {
    count=0;
  }
  else
  {
    count = GeometryWkb->GetCount();
    data = GeometryWkb->GetData();
  }
  
  otl_long_string gpar(count+10); // define long string variable
  
  
  for(int ind=0;ind<count;ind++)
  {
    gpar[ind] = *data;
    data++;
  }
  gpar.set_len(count);
  
  *Stream<<gpar;
  return true;
  
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}
  
}//end of c_KgOtl::SetStreamDataGeometryWkb

bool c_KgOtl::SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf)
{

try
{  
  int count;
  FdoPtr<FdoByteArray> wkb;
  FdoByte *data;
  if( !GeometryFgf )
  {
    count=0;
  }
  else
  {
  // Transfer Fgf to wkb
    
  
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(GeometryFgf);
  
    wkb =  gf->GetWkb(geom);
    
    if( wkb.p )
    {
      count = wkb->GetCount();
      data = wkb->GetData();
    }
    else
    {
      count = 0;
    }
  }
  
  otl_long_string gpar(count+10); // define long string variable
  
  
  for(int ind=0;ind<count;ind++)
  {
    gpar[ind] = *data;
    data++;
  }
  gpar.set_len(count);
  
  *Stream<<gpar;
  return true;
  
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}
  
}//end of c_KgOtl::SetStreamDataGeometryWkb
  
  
void c_KgOtl::ReadGeometryColumns(vector<c_KgMss_GeomteryColumns>& GeomData)
{
try
{ 
/*
 CREATE TABLE [ST].[GEOMETRY_COLUMNS](
	[F_TABLE_CATALOG] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_TABLE_SCHEMA] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_TABLE_NAME] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_GEOMETRY_COLUMN] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[COORD_DIMENSION] [int] NOT NULL,
	[SRID] [int] NOT NULL,
	[GEOMETRY_TYPE] [varchar](30) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL
) ON [PRIMARY]
*/
	
   otl_stream i(10, // buffer size
              "SELECT [F_TABLE_CATALOG],[F_TABLE_SCHEMA],[F_TABLE_NAME] \
              ,[F_GEOMETRY_COLUMN],[COORD_DIMENSION],[SRID],[GEOMETRY_TYPE] \
              FROM [ST].[GEOMETRY_COLUMNS]",
                 // SELECT statement
              m_OtlDB // connect object
             ); 
   // create select stream
 
 
  while(!i.eof())
  { // while not end-of-data
    c_KgMss_GeomteryColumns data;      
    i>>data.m_TableCatalog;
    i>>data.m_QuerySchema;
    i>>data.m_QueryName;
    i>>data.m_GeomColumn;
    i>>data.m_CoordDimension;
    i>>data.m_SRID;
    i>>data.m_GeometryType;
    
    GeomData.push_back(data);
    wcout<<L"\nF_TABLE_CATALOG="<<data.m_TableCatalog<<endl;
    wcout<<L"\nF_TABLE_SCHEMA="<<data.m_QuerySchema<<endl;
    wcout<<L"\nF_TABLE_NAME="<<data.m_QueryName<<endl;
    wcout<<L"\nF_GEOMETRY_COLUMN="<<data.m_GeomColumn<<endl;
    wcout<<L"\nCOORD_DIMENSION="<<data.m_CoordDimension<<endl;
    wcout<<L"\nSRID="<<data.m_SRID<<endl;
    wcout<<L"\nGEOMETRY_TYPE="<<data.m_GeometryType<<endl;
  }

}
catch(otl_exception& p)
{ // intercept OTL exceptions  
 
  throw c_KgOtlException(p.msg,p.stm_text); 
}


}

void c_KgOtl::QueryTableGeometry(c_KgMss_GeomteryColumns& TableDesc)
{
try
{ 
/*
 CREATE TABLE [ST].[GEOMETRY_COLUMNS](
	[F_TABLE_CATALOG] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_TABLE_SCHEMA] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_TABLE_NAME] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[F_GEOMETRY_COLUMN] [varchar](256) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[COORD_DIMENSION] [int] NOT NULL,
	[SRID] [int] NOT NULL,
	[GEOMETRY_TYPE] [varchar](30) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL
) ON [PRIMARY]
*/
  char buff[2048];
  
  //otl_var_varchar_long
  //sprintf(buff," SELECT ST.AsText([%s]) from [%s].[%s]",TableDesc.m_GeometryColumn,TableDesc.m_TableSchema,TableDesc.m_TableName);
  sprintf(buff," SELECT ST.AsBinary([%s]) from [%s].[%s]",TableDesc.m_GeomColumn,TableDesc.m_QuerySchema,TableDesc.m_QueryName);
	
   otl_stream i(50, // buffer size
              buff,
                 // SELECT statement
              m_OtlDB // connect object
             ); 
   // create select stream
 
 
 
 
  while(!i.eof())
  { // while not end-of-data
    otl_long_string geombuff(700000);
    geombuff.set_len(0);
    
    i>>geombuff;
    
    geombuff.null_terminate_string(geombuff.len());
    
    int len = geombuff.len();
    cout<<OTL_RCAST(char*,geombuff.v)<<endl<<endl;
    
    geombuff.set_len(0);
    
    //cout<<"\nGeometry="<<geombuff<<endl;
    
  }

}
catch(otl_exception& p)
{ // intercept OTL exceptions  
 
  throw c_KgOtlException(p.msg,p.stm_text); 
}


}//end of c_KgOtl::QueryTableData