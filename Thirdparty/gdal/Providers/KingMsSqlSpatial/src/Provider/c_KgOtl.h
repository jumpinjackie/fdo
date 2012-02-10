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
#pragma once
#include <string.h>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <vector>



#define OTL_ODBC // Compile OTL 4.0/ODBC
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_STL
#define OTL_VALUE_TEMPLATE_ON
#define OTL_STREAM_READ_ITERATOR_ON

//#define OTL_UNICODE // Enable Unicode OTL for ODBC

#define OTL_UNICODE_EXCEPTION_AND_RLOGON
#if defined(__GNUC__)
  namespace std{   typedef unsigned short unicode_char;   
  typedef basic_string<unicode_char> unicode_string;}
  #define OTL_UNICODE_CHAR_TYPE unicode_char
  #define OTL_UNICODE_STRING_TYPE unicode_string
#else
  #define OTL_UNICODE_CHAR_TYPE wchar_t
  #define OTL_UNICODE_STRING_TYPE wstring
#endif

#if defined( __GNUC__)
#define OTL_BIGINT long long // GNU C++
#else

#define OTL_BIGINT __int64 // VC++, Borland C++
#endif


#include "..\otl\otlv4_h\otlv4.h"

//#define D_MAXIMUM_VARBINARY_SIZE 70000
#define D_MAXIMUM_VARBINARY_SIZE 1200000

/*
OTL defined integer constant
 Integer code
 Explanation
 
otl_var_bigint  20
 MS SQL Server, DB2, MySQL, PostgreSQL, etc. BIGINT (signed 64-bit integer) type
 
otl_var_blob 12
 datatype that is mapped into BLOB in Oracle 8
 
otl_var_char 1
 null terminated string
 
otl_var_clob  11
 datatype that is mapped into CLOB in Oracle 8 
 
otl_var_db2date  17
 DB2 DATE type 
 
otl_var_db2time 16
 DB2 TIME type
 
otl_var_double 2
 8-byte floating point number
 
otl_var_float 3
 4-byte floating point number
 
otl_var_int 4
 signed 32-bit  integer
 
otl_var_long_int 7
 signed 32-bit integer
 
otl_var_ltz_timestamp  19
 Oracle 9i/10g TIMESTAMP WITH LOCAL TIME ZONE type
 
otl_var_raw
 23
 RAW, BINARY, VARBINARY, BYTEA, VARCHAR BYTE, CHAR BYTE, etc.
 
otl_var_raw_long 10
 datatype that is mapped into LONG RAW in Oracle 7/8, IMAGE in MS SQL Server ad Sybase, BLOB in DB2
 
otl_var_short 6
 signed 16-bit integer 
 
otl_var_timestamp 8
 datatype that is mapped into TIMESTAMP_STRUCT, ODBC and DB2-CLI only
 
otl_var_tz_timestamp 18
 Oracle 9i/10g TIMESTAMP WITH TIME ZONE type
 
otl_var_unsigned_int 5
 unsigned  32-bit integer
 
otl_var_varchar_long 9
 datatype that is mapped into LONG in Oracle 7/8, TEXT in MS SQL Server and Sybase, CLOB in DB2 
*/

enum e_Otl_Var_Types
{
e_Otl_Var_bigint = 20, // MS SQL Server, DB2, MySQL, PostgreSQL, etc. BIGINT (signed 64-bit integer) type
 
e_Otl_Var_blob= 12, // datatype that is mapped into BLOB in Oracle 8
 
e_Otl_Var_char=1,   //null terminated string
 
e_Otl_Var_clob=  11, //datatype that is mapped into CLOB in Oracle 8 
 
e_Otl_Var_db2date  = 17, // DB2 DATE type 
 
e_Otl_Var_db2time = 16, // DB2 TIME type
 
e_Otl_Var_double = 2, // 8-byte floating point number
 
e_Otl_Var_float = 3, // 4-byte floating point number
 
e_Otl_Var_int = 4, // signed 32-bit  integer
 
e_Otl_Var_long_int =  7, // signed 32-bit integer
 
e_Otl_Var_ltz_timestamp = 19, // Oracle 9i/10g TIMESTAMP WITH LOCAL TIME ZONE type
 
e_Otl_Var_raw = 23, // RAW, BINARY, VARBINARY, BYTEA, VARCHAR BYTE, CHAR BYTE, etc.
 
e_Otl_Var_raw_long = 10, // datatype that is mapped into LONG RAW in Oracle 7/8, IMAGE in MS SQL Server ad Sybase, BLOB in DB2
 
e_Otl_Var_short = 6, // signed 16-bit integer 
 
e_Otl_Var_timestamp = 8, // datatype that is mapped into TIMESTAMP_STRUCT, ODBC and DB2-CLI only
 
e_Otl_Var_tz_timestamp = 18, // Oracle 9i/10g TIMESTAMP WITH TIME ZONE type
 
e_Otl_Var_unsigned_int = 5, // unsigned  32-bit integer
 
e_Otl_Var_varchar_long = 9, // datatype that is mapped into LONG in Oracle 7/8, TEXT in MS SQL Server and Sybase, CLOB in DB2 
};

class c_KgOtlException
{
public:
  c_KgOtlException(const wchar_t* Error)
  {
    m_Message = Error;
  }
  
  c_KgOtlException(const wchar_t* Error,const char* SQL)
  {
    m_Message = Error;
    m_SQL = SQL;
  }
  
  
protected: 
  std::wstring m_Message;
  std::string m_SQL; // SQL statement causing error;
  
public:
  const wchar_t* GetMessage() { return m_Message.c_str(); }  
  const wchar_t* what() { return m_Message.c_str(); }  
};

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
class c_KgMss_GeomteryColumns
{
public:
  wstring m_TableCatalog;
  
  wstring m_FdoSchema;       // name of schema to use for class
  wstring m_FdoClassName;    // name to be used for class
  
  wstring m_QuerySchema;     // m_QuerySchema.m_QueryName identifes Table/View which will be used to query class
  wstring m_QueryName;       // If class is based on view than you need to define m_GeomTableSchema.m_GeomTableName where geometry is stored
  
  wstring m_GeomTableSchema; // m_GeomTableSchema.m_GeomTableName is name of Table (has to be table ) which
  wstring m_GeomTableName;   // stores geometry and this one is used for cretaing SQL query with spatial filter
                              // In case of Table this is same as m_QuerySchema.m_QueryName
                              // In a case of view
  wstring m_GeomColumn;      // name of column for geometry
  
  wstring m_ClassIdentity;    // what is going to be used as identity column for class
  wstring m_OverGeometryType;    
  
  int m_CoordDimension;
  wstring m_GeometryType;
  
  int m_SRID;
  wstring m_SrText; // Wkt
};



class c_KgOtl
{
public:
  c_KgOtl(void);
  ~c_KgOtl(void);
  
protected:
  otl_connect m_OtlDB; // connect object  
  
public:
  static void Init();  
  void Open(const wchar_t* UserName,const wchar_t* Password,const wchar_t* DSN);
  void Close();  
  
  long ExecuteSql(const char* Sql);
  void Commit();
  
// Stream creating
  otl_stream* CreateStream(int BufferSize,const char* SqlStm,const int implicit_select=otl_explicit_select); // Close the stream  
  static void DeleteStream(otl_stream*& OtlStream); // Close the stream
  
  void OpenStream(otl_stream*& Stream,int BuffSize,const char* SqlStm,const int implicit_select=otl_explicit_select);
  
  static void SetCommit(otl_stream* Stream,int Commit);  
  static void FlushStream(otl_stream* Stream);
  static long GetRpc(otl_stream* Stream);
  

  
  static bool SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue);
  static bool SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb);
  static bool SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf);
  
  
  void ReadGeometryColumns(vector<c_KgMss_GeomteryColumns>& GeomData);
  void QueryTableGeometry(c_KgMss_GeomteryColumns& TableDesc);
};
