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
#include "c_KgOraSQLDataReader.h"
#include "c_FdoOra_API.h"

c_KgOraSQLDataReader::c_KgOraSQLDataReader(c_KgOraConnection * Connection
                        ,oracle::occi::Statement* OcciStatement ,oracle::occi::ResultSet* OcciResultSet)
{
  m_Connection = Connection;
  FDO_SAFE_ADDREF(m_Connection);
  
  m_SqlColumns = FdoStringCollection::Create();
  
  m_OcciStatement = OcciStatement;
  m_OcciResultSet = OcciResultSet;
  
  m_MetaData = m_OcciResultSet->getColumnListMetaData();
  
  string cname;
  size_t count = m_MetaData.size();
  for(size_t ind=0;ind<count;ind++)
  {
    cname = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_NAME);    
    
    // test if recognize column type
    // if not then keep column
    string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
    int dtype = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
    int length = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
    int orascale = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);

    FdoDataType fdotype;
    if( c_FdoOra_API::OraTypeToFdoDataType((oracle::occi::Type)dtype,orascale,length,fdotype) )
    {  
      m_SqlColumns->Add(cname.c_str());
      m_SqlColIndex.push_back(ind);
    }
    else
    {
      if( FdoCommonOSUtil::stricmp(oratype.c_str(),"SDO_GEOMETRY") == 0 )
      {
        m_SqlColumns->Add(cname.c_str());
        m_SqlColIndex.push_back(ind); 
      }
    }
  }
  
}

int c_KgOraSQLDataReader::ColumnNameToColumnIndex(FdoString* ColumnName)
{
  int sind = m_SqlColumns->IndexOf(ColumnName);
  return m_SqlColIndex[sind];
}

c_KgOraSQLDataReader::~c_KgOraSQLDataReader(void)
{
  Close();
  FDO_SAFE_RELEASE(m_Connection);
}

void c_KgOraSQLDataReader::Dispose()
{
    delete this;
}

 /// \brief
/// Gets the number of columns in the result set.
/// 
/// \return
/// Returns the number of columns.
/// 
FdoInt32 c_KgOraSQLDataReader::GetColumnCount()
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
FdoString* c_KgOraSQLDataReader::GetColumnName(FdoInt32 Index)
{
  string cname = m_MetaData[Index].getString(oracle::occi::MetaData::ATTR_NAME);    
  
  m_ColName = cname.c_str();
  
  return m_ColName;

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
FdoDataType c_KgOraSQLDataReader::GetColumnType(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  
    
  int dtype = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
  int length = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
  int orascale = m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);

  FdoDataType fdotype;
  c_FdoOra_API::OraTypeToFdoDataType((oracle::occi::Type)dtype,orascale,length,fdotype);
  
  return fdotype;
  
}//end of c_KgOraSQLDataReader::GetColumnType

/// \brief
/// Gets the data type of the column with the specified name.
/// 
/// \param columnName 
/// Input the column name.
/// 
/// \return
/// Returns the type of the column.
/// 
FdoPropertyType c_KgOraSQLDataReader::GetPropertyType(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  
  
  string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
  
  if( FdoCommonOSUtil::stricmp(oratype.c_str(),"SDO_GEOMETRY") == 0 )
  {
    return FdoPropertyType_GeometricProperty;
  }
  
  return FdoPropertyType_DataProperty;
  
}//end of c_KgOraSQLDataReader::GetColumnType

 bool c_KgOraSQLDataReader::GetBoolean(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    string str = m_OcciResultSet->getString(ind+1);    
    
    if( str.compare("1") == 0 ) return true;
    if( str.compare("TRUE") == 0 ) return true;
    if( str.compare("true") == 0 ) return true;
    
    return false;
  }
  throw FdoCommandException::Create(L"Boolean");
}

 FdoByte  c_KgOraSQLDataReader::GetByte(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    string str = m_OcciResultSet->getString(ind+1);    
    const char* cp = str.c_str();
    if( cp ) return *cp;
    
    return 0;
  }
  throw FdoCommandException::Create(L"Byte");
}//end of  c_KgOraSQLDataReader::GetByte

 FdoDateTime c_KgOraSQLDataReader::GetDateTime(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
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
  }
    
   
  throw FdoCommandException::Create(L"DateTime");
}//end of  c_KgOraSQLDataReader::GetDateTime

 double c_KgOraSQLDataReader::GetDouble(FdoString* ColumnName)
{

  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    double val = m_OcciResultSet->getDouble(ind+1);    
    return val;
  }
    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of  c_KgOraSQLDataReader::GetDouble

 FdoInt16 c_KgOraSQLDataReader::GetInt16(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    int val = m_OcciResultSet->getInt(ind+1);    
    return val;
  }
  
  throw FdoCommandException::Create(L"Int16");
  
}//end of  c_KgOraSQLDataReader::GetInt16

 FdoInt32 c_KgOraSQLDataReader::GetInt32(FdoString* ColumnName)
{
 
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    int val = m_OcciResultSet->getInt(ind+1);    
    return val;
  }
    
    //return 5;
    
    throw FdoCommandException::Create(L"Int32");
}

 FdoInt64 c_KgOraSQLDataReader::GetInt64(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    long val = m_OcciResultSet->getInt(ind+1);    
    return val;
  }
  
    throw FdoCommandException::Create(L"Int64");
}

 float c_KgOraSQLDataReader::GetSingle(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    float val = m_OcciResultSet->getFloat(ind+1);    
    return val;
  }
  throw FdoCommandException::Create(L"Float32");
}

 FdoString* c_KgOraSQLDataReader::GetString(FdoString* ColumnName)
{
 
    
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    string str = m_OcciResultSet->getString(ind+1); 
    
    m_CachedString = str.c_str();
    
    return m_CachedString;
  }
    
    
    
    throw FdoCommandException::Create(L"GetString");
}

 FdoLOBValue* c_KgOraSQLDataReader::GetLOB(FdoString* ColumnName)
{
    return NULL;
}

 FdoIStreamReader* c_KgOraSQLDataReader::GetLOBStreamReader(FdoString* ColumnName )
{
    return NULL;
}

 bool c_KgOraSQLDataReader::IsNull(FdoString* ColumnName)
{
  int ind = m_SqlColumns->IndexOf(ColumnName);
  if( m_OcciResultSet && (ind >= 0) )
  {
    return m_OcciResultSet->isNull(ind+1);         
  } 
  return false;
}

 FdoByteArray* c_KgOraSQLDataReader::GetGeometry(FdoString* ColumnName)
{
    int len = 0;
    const void* ptr = GetGeometry(ColumnName, &len);
    return FdoByteArray::Create((const FdoByte*)ptr, len);
}




 const FdoByte* c_KgOraSQLDataReader::GetGeometry(FdoString* ColumnName, FdoInt32* len)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( m_OcciResultSet )
  {

    
    SDO_GEOMETRY *geom = (SDO_GEOMETRY*)m_OcciResultSet->getObject(ind+1); // oracle is 1 based - our index is 0 based
    *len=0;
    if( geom )
    {
      m_SdoAgfConv.SetGeometry(geom);
      *len = m_SdoAgfConv.ToAGF(  );
       
      delete geom;
    }
    
 
    
    return (const unsigned char*)m_SdoAgfConv.GetBuff();
  }
  
  
  return (const unsigned char*)m_SdoAgfConv.GetBuff();
}

 bool c_KgOraSQLDataReader::ReadNext()
{    
    if( m_OcciResultSet->next() == oracle::occi::ResultSet::END_OF_FETCH )
    {
    
      
      return false;
    }
    
    return true;
  
}//end of  c_KgOraSQLDataReader::ReadNext

 void c_KgOraSQLDataReader::Close()
{
try
{
  if (m_OcciStatement && m_OcciResultSet)
  {
    m_OcciStatement->closeResultSet(m_OcciResultSet);        
    m_OcciResultSet = NULL;
  }
  
  if (m_OcciStatement)
  {
    m_Connection->OCCI_TerminateStatement(m_OcciStatement);
    m_OcciStatement=NULL;
  }
}
catch(oracle::occi::SQLException& ea)
{
  m_OcciResultSet = NULL;
  FdoStringP gstr = ea.getMessage().c_str();
  throw FdoConnectionException::Create( gstr );  
}

}//end of  c_KgOraSQLDataReader::Close



