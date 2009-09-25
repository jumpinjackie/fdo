/*
* Copyright (C) 2006  Haris Kurtagic
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
#include "c_KgInfSQLDataReader.h"


c_KgInfSQLDataReader::c_KgInfSQLDataReader(c_KgInfConnection * Connection
                        ,otl_stream* Stream,c_KgOtlStreamReader*StreamReader)
{
  m_Connection = Connection;
  FDO_SAFE_ADDREF(m_Connection);
  
  m_SqlColumns = FdoStringCollection::Create();
  
  m_OtlStream = Stream;
  m_OtlStreamReader = StreamReader;
  //m_OcciStatement = OcciStatement;
  //m_OcciResultSet = OcciResultSet;
  
  
   
 otl_column_desc* desc;
 int desc_len;


 desc=m_OtlStream->describe_select(desc_len);

 for(int n=0;n<desc_len;++n)
 {
 /*
  cout<<"========== COLUMN #"<<n+1<<" ==========="<<endl;
  cout<<"name="<<desc[n].name<<endl;
  cout<<"dbtype="<<desc[n].dbtype<<endl;
  cout<<"otl_var_dbtype="<<desc[n].otl_var_dbtype<<endl;
  cout<<"dbsize="<<desc[n].dbsize<<endl;
  cout<<"scale="<<desc[n].scale<<endl;
  cout<<"prec="<<desc[n].prec<<endl;
  cout<<"nullok="<<desc[n].nullok<<endl;
*/
  //otl_column_desc dsc = *desc++;
  m_MetaData.push_back(desc++);   
  //desc++;
 }

  
  
  string cname;
  size_t count = m_MetaData.size();
  for(size_t ind=0;ind<count;ind++)
  {
    cname = m_MetaData[ind]->name;   // getString(oracle::occi::MetaData::ATTR_NAME);    
    
    // test if recognize column type
    // if not then keep column
    
    //string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
    int dtype = m_MetaData[ind]->otl_var_dbtype;   // getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
    
    int length = m_MetaData[ind]->dbsize; // getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
    int scale = m_MetaData[ind]->scale; // getInt(oracle::occi::MetaData::ATTR_SCALE);
    int prec =  m_MetaData[ind]->prec;

    FdoDataType fdotype;
    if( c_FdoInf_API::OtlTypeToFdoDataType(dtype,scale,length,fdotype) )
    {  
      m_SqlColumns->Add(cname.c_str());
      m_SqlColIndex.push_back(ind);
    }
    else
    {
    /*
      if( strcmpi(oratype.c_str(),"SDO_GEOMETRY") == 0 )
      {
        m_SqlColumns->Add(cname.c_str());
        m_SqlColIndex.push_back(ind); 
      }
    */  
    }
  }
  
}

int c_KgInfSQLDataReader::ColumnNameToColumnIndex(FdoString* ColumnName)
{
  int sind = m_SqlColumns->IndexOf(ColumnName);
  return m_SqlColIndex[sind];
}

c_KgInfSQLDataReader::~c_KgInfSQLDataReader(void)
{
  Close();
  FDO_SAFE_RELEASE(m_Connection);
}

void c_KgInfSQLDataReader::Dispose()
{
    delete this;
}

 /// \brief
/// Gets the number of columns in the result set.
/// 
/// \return
/// Returns the number of columns.
/// 
FdoInt32 c_KgInfSQLDataReader::GetColumnCount()
{
  return m_MetaData.size();
}

/// \brief
/// Gets the name of the column at the given ordinal position.
/// 
/// \param index 
/// Input the position of the column.
/// 
/// \return
/// Returns the column name
/// 
FdoString* c_KgInfSQLDataReader::GetColumnName(FdoInt32 Index)
{
  string cname = m_MetaData[Index]->name; // getString(oracle::occi::MetaData::ATTR_NAME);    
  
  m_ColName = cname.c_str();
  
  return m_ColName;

}

/// \brief
/// Gets the index of the column with the given column name.
/// 
/// \param columnName 
/// Input the column name.
/// 
/// \return
/// Returns the column index
/// 
FdoInt32 c_KgInfSQLDataReader::GetColumnIndex(FdoString* columnName)
{
    return ColumnNameToColumnIndex(columnName);
}

/// \brief
/// Gets the data type of the column with the specified name.
/// 
/// \param columnName 
/// Input the column name.
/// 
/// \return
/// Returns the type of the column.
/// 
FdoDataType c_KgInfSQLDataReader::GetColumnType(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  
    
  int dtype = m_MetaData[ind]->otl_var_dbtype;          // getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
  int length = m_MetaData[ind]->dbsize; // getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
  int orascale = m_MetaData[ind]->scale;           // getInt(oracle::occi::MetaData::ATTR_SCALE);

  FdoDataType fdotype;
  c_FdoInf_API::OtlTypeToFdoDataType(dtype,orascale,length,fdotype);
  
  return fdotype;
  
}//end of c_KgInfSQLDataReader::GetColumnType

/// \brief
/// Gets the data type of the column with the specified name.
/// 
/// \param columnName 
/// Input the column name.
/// 
/// \return
/// Returns the type of the column.
/// 
FdoPropertyType c_KgInfSQLDataReader::GetPropertyType(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  
  /*
  string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
  
  if( strcmpi(oratype.c_str(),"SDO_GEOMETRY") == 0 )
  {
    return FdoPropertyType_GeometricProperty;
  }
  */
  return FdoPropertyType_DataProperty;
  
}//end of c_KgInfSQLDataReader::GetColumnType

 bool c_KgInfSQLDataReader::GetBoolean(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 1) )
  {
    /*
    string str = m_OcciResultSet->getString(ind+1);    
    
    if( str.compare("1") == 0 ) return true;
    if( str.compare("TRUE") == 0 ) return true;
    if( str.compare("true") == 0 ) return true;
    
    return false;
    */
    return m_OtlStreamReader->GetBoolean(ind+1);
  }
  throw FdoCommandException::Create(L"Boolean");
}

 FdoByte  c_KgInfSQLDataReader::GetByte(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    /*
    string str = m_OcciResultSet->getString(ind+1);    
    const char* cp = str.c_str();
    if( cp ) return *cp;
    
    return 0;
    */
    return m_OtlStreamReader->GetByte(ind+1);
  }
  throw FdoCommandException::Create(L"Byte");
}//end of  c_KgInfSQLDataReader::GetByte

 FdoDateTime c_KgInfSQLDataReader::GetDateTime(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
  /*
    oracle::occi::Date oradt = m_OcciResultSet->getDate(ind+1);    
    FdoDateTime fdodate;
    if( !oradt.isNull() )
    {
      int year;
      unsigned int month,day,hour,min,sec;
      oradt.getDate(year,month,day,hour,min,sec);
      
      fdodate.year = year;
      fdodate.month = month;
      fdodate.day = day;
      fdodate.hour = hour;
      fdodate.minute = min;
      fdodate.seconds = sec;
      
    }
    return fdodate;
   */ 
   return m_OtlStreamReader->GetDateTime(ind+1);
  }
    
   
  throw FdoCommandException::Create(L"DateTime");
}//end of  c_KgInfSQLDataReader::GetDateTime

 double c_KgInfSQLDataReader::GetDouble(FdoString* ColumnName)
{

  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //double val = m_OcciResultSet->getDouble(ind+1);    
    //return val;
    return m_OtlStreamReader->GetDouble(ind+1);
  }
    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of  c_KgInfSQLDataReader::GetDouble

 FdoInt16 c_KgInfSQLDataReader::GetInt16(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //int val = m_OcciResultSet->getInt(ind+1);    
    //return val;
    return m_OtlStreamReader->GetInt16(ind+1);
  }
  
  throw FdoCommandException::Create(L"Int16");
  
}//end of  c_KgInfSQLDataReader::GetInt16

 FdoInt32 c_KgInfSQLDataReader::GetInt32(FdoString* ColumnName)
{
 
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //int val = m_OcciResultSet->getInt(ind+1);    
    //return val;
    return m_OtlStreamReader->GetInt32(ind+1);
  }
    
    //return 5;
    
    throw FdoCommandException::Create(L"Int32");
}

 FdoInt64 c_KgInfSQLDataReader::GetInt64(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //long val = m_OcciResultSet->getInt(ind+1);    
    //return val;
    return m_OtlStreamReader->GetInt64(ind+1);
  }
  
    throw FdoCommandException::Create(L"Int64");
}

 FdoFloat c_KgInfSQLDataReader::GetSingle(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //float val = m_OcciResultSet->getFloat(ind+1);    
    //return val;
    return m_OtlStreamReader->GetFloat(ind+1);
  }
  throw FdoCommandException::Create(L"Float32");
}

 FdoString* c_KgInfSQLDataReader::GetString(FdoString* ColumnName)
{
 
    
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OtlStream && (ind >= 0) )
  {
    //string str = m_OcciResultSet->getString(ind+1); 
    
    
    wstring wstr;
    m_OtlStreamReader->GetString(ind+1,wstr);
    
    m_CachedString = wstr.c_str();
    return m_CachedString;
  }
    
    
    
    throw FdoCommandException::Create(L"GetString");
}

 FdoLOBValue* c_KgInfSQLDataReader::GetLOB(FdoString* ColumnName)
{
    return NULL;
}

 FdoIStreamReader* c_KgInfSQLDataReader::GetLOBStreamReader(FdoString* ColumnName )
{
    return NULL;
}

 bool c_KgInfSQLDataReader::IsNull(FdoString* ColumnName)
{
  int ind = m_SqlColumns->IndexOf(ColumnName);
  /*
  if( m_OcciResultSet && (ind >= 0) )
  {
    return m_OcciResultSet->isNull(ind+1);         
  } 
  */
  return m_OtlStreamReader->GetIsNull(ind+1);
  return false;
}

 FdoByteArray* c_KgInfSQLDataReader::GetGeometry(FdoString* ColumnName)
{
  int ind = m_SqlColumns->IndexOf(ColumnName);
  if( m_OtlStream && (ind >= 0) )
    return m_OtlStreamReader->GetGeometryFgf(ind+1);

  return NULL;    
}



 bool c_KgInfSQLDataReader::ReadNext()
{    

  return m_OtlStreamReader->ReadNext();
    
  
}//end of  c_KgInfSQLDataReader::ReadNext

 void c_KgInfSQLDataReader::Close()
{
try
{
  m_Connection->OTL_DeleteStream(m_OtlStream);
  if( m_OtlStreamReader ) delete m_OtlStreamReader;
}
catch(c_KgOtlException& ea)
{
  
  throw FdoConnectionException::Create( ea.what() );  
}

}//end of  c_KgInfSQLDataReader::Close



