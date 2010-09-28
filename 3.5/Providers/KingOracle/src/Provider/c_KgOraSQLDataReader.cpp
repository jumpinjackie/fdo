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
#include "c_KgOraSQLDataReader.h"
#include "c_FdoOra_API2.h"

c_KgOraSQLDataReader::c_KgOraSQLDataReader(c_KgOraConnection * Connection
                        ,c_Oci_Statement* OciStatement )
  
  
{
  m_PropNameToIndex = NULL;
  m_PropNameToIndex_IsNull = NULL;
  
  m_Connection = Connection;
  FDO_SAFE_ADDREF(m_Connection);
  
  m_SqlColumns = FdoStringCollection::Create();
  
  m_OciStatement = OciStatement;
  
  
  //m_MetaData = m_OcciResultSet->getColumnListMetaData();
  
  wchar_t* cname;
  size_t count = OciStatement->GetColumnsSize();
  for(size_t ind=0;ind<count;ind++)
  {
    //cname = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_NAME);    
    cname = OciStatement->GetColumnName(ind+1);
    
    // test if recognize column type
    // if not then keep column
    wchar_t* oratype = OciStatement->GetColumnTypeName(ind+1); // = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
    int dtype = OciStatement->GetColumnOciType(ind+1);//m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
    int length = OciStatement->GetColumnWidth(ind+1);//m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
    int orascale = OciStatement->GetColumnScale(ind+1); //m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);

    FdoDataType fdotype;
    if( c_FdoOra_API2::OraTypeToFdoDataType(dtype,orascale,length,fdotype) )
    {  
      m_SqlColumns->Add(cname);
      m_SqlColIndex.push_back(ind);
    }
    else
    {
      if( FdoCommonOSUtil::wcsicmp(oratype,L"SDO_GEOMETRY") == 0 )
      {
        m_SqlColumns->Add(cname);
        m_SqlColIndex.push_back(ind); 
      }
    }
  }
  
  m_PropNameToIndex = new c_StringToIndex(m_SqlColumns);
  m_PropNameToIndex_IsNull = new c_StringToIndex(m_SqlColumns);
}

int c_KgOraSQLDataReader::ColumnNameToColumnIndex(FdoString* ColumnName)
{
  //int sind = m_SqlColumns->IndexOf(ColumnName);
  int sind = m_PropNameToIndex->GetIndex(ColumnName);
  return m_SqlColIndex[sind];
}
int c_KgOraSQLDataReader::ColumnNameToColumnIndex_IsNull(FdoString* ColumnName)
{
  //int sind = m_SqlColumns->IndexOf(ColumnName);
  int sind = m_PropNameToIndex_IsNull->GetIndex(ColumnName);
  return m_SqlColIndex[sind];
}

c_KgOraSQLDataReader::~c_KgOraSQLDataReader(void)
{
  if( m_PropNameToIndex ) delete m_PropNameToIndex;
  if( m_PropNameToIndex_IsNull) delete m_PropNameToIndex_IsNull;
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
  return m_OciStatement->GetColumnsSize(); //m_MetaData.size();
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
  return m_OciStatement->GetColumnName(Index+1);
  /*
  string cname = m_MetaData[Index].getString(oracle::occi::MetaData::ATTR_NAME);    
  
  m_ColName = cname.c_str();
  
  return m_ColName;
  */
}

FdoInt32 c_KgOraSQLDataReader::GetColumnIndex(FdoString* columnName)
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
FdoDataType c_KgOraSQLDataReader::GetColumnType(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  
    
  int dtype = m_OciStatement->GetColumnOciType(ind+1); //m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
  int length = m_OciStatement->GetColumnWidth(ind+1); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
  int orascale = m_OciStatement->GetColumnScale(ind+1); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);

  FdoDataType fdotype;
  c_FdoOra_API2::OraTypeToFdoDataType(dtype,orascale,length,fdotype);
  
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
  
  wchar_t* oratype = m_OciStatement->GetColumnTypeName(ind+1);
  //string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
  
  if( oratype && FdoCommonOSUtil::wcsicmp(oratype,L"SDO_GEOMETRY") == 0 )
  {
    return FdoPropertyType_GeometricProperty;
  }
  
  return FdoPropertyType_DataProperty;
  
}//end of c_KgOraSQLDataReader::GetColumnType

 bool c_KgOraSQLDataReader::GetBoolean(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0 )
  {
    //string str = m_OcciResultSet->getString(ind+1);    
    const wchar_t* str = m_OciStatement->GetString(ind+1);
    
    if( str )
    {
      //if( wcscmp(str,L"1") == 0 ) return true;
      if( FdoCommonOSUtil::wcsicmp(str,L"1") == 0 ) return true;
      
    }
    
    return false;
  }
  throw FdoCommandException::Create(L"Boolean");
}

FdoByte  c_KgOraSQLDataReader::GetByte(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //string str = m_OcciResultSet->getString(ind+1);    
    const wchar_t* str = m_OciStatement->GetString(ind+1);
    
    if( str ) return (FdoByte)*str;
    
    return 0;
  }
  throw FdoCommandException::Create(L"Byte");
}//end of  c_KgOraSQLDataReader::GetByte

 FdoDateTime c_KgOraSQLDataReader::GetDateTime(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //oracle::occi::Date oradt = m_OcciResultSet->getDate(ind+1);    
    OCIDate* oradt = m_OciStatement->GetOciDate(ind+1);
    FdoDateTime fdodate;
    if( oradt )
    {
      fdodate.year = oradt->OCIDateYYYY;
      fdodate.month = oradt->OCIDateMM;
      fdodate.day = oradt->OCIDateDD;
      fdodate.hour = oradt->OCIDateTime.OCITimeHH;
      fdodate.minute = oradt->OCIDateTime.OCITimeMI;
      fdodate.seconds = oradt->OCIDateTime.OCITimeSS;
      
    }
    return fdodate;
  }
    
   
  throw FdoCommandException::Create(L"DateTime");
}//end of  c_KgOraSQLDataReader::GetDateTime

 double c_KgOraSQLDataReader::GetDouble(FdoString* ColumnName)
{

  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    return m_OciStatement->GetDouble(ind+1); // m_OcciResultSet->getDouble(ind+1);    
    
  }
    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of  c_KgOraSQLDataReader::GetDouble

 FdoInt16 c_KgOraSQLDataReader::GetInt16(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //int val = m_OcciResultSet->getInt(ind+1);    
    return m_OciStatement->GetInteger(ind+1);
  }
  
  throw FdoCommandException::Create(L"Int16");
  
}//end of  c_KgOraSQLDataReader::GetInt16

 FdoInt32 c_KgOraSQLDataReader::GetInt32(FdoString* ColumnName)
{
 
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //int val = m_OcciResultSet->getInt(ind+1);    
    return m_OciStatement->GetInteger(ind+1);
  }
    
    //return 5;
    
    throw FdoCommandException::Create(L"Int32");
}

 FdoInt64 c_KgOraSQLDataReader::GetInt64(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0 )
  {
    //long val = m_OcciResultSet->getInt(ind+1);    
    return m_OciStatement->GetLong(ind+1);
  }
  
    throw FdoCommandException::Create(L"Int64");
}

 float c_KgOraSQLDataReader::GetSingle(FdoString* ColumnName)
{
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //float val = m_OcciResultSet->getFloat(ind+1);    
    return (float)m_OciStatement->GetDouble(ind+1);
  }
  throw FdoCommandException::Create(L"Float32");
}

 FdoString* c_KgOraSQLDataReader::GetString(FdoString* ColumnName)
{
 
    
  int ind = ColumnNameToColumnIndex(ColumnName);
  if( ind >= 0) 
  {
    //string str = m_OcciResultSet->getString(ind+1); 
    const wchar_t* str = m_OciStatement->GetString(ind+1);
    
    m_CachedString = str;
    
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
  //int ind = m_SqlColumns->IndexOf(ColumnName);
  int ind = ColumnNameToColumnIndex_IsNull(ColumnName);
  if( ind >= 0) 
  {
    //return m_OcciResultSet->isNull(ind+1);         
    return m_OciStatement->IsColumnNull(ind+1);
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
  if( m_OciStatement)
  {

    
    //SDO_GEOMETRY *geom = (SDO_GEOMETRY*)m_OcciResultSet->getObject(ind+1); // oracle is 1 based - our index is 0 based
    c_SDO_GEOMETRY *geom = m_OciStatement->GetSdoGeom(ind+1);
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
    if( !m_OciStatement->ReadNext() )
    {
    
      
      return false;
    }
    
    return true;
  
}//end of  c_KgOraSQLDataReader::ReadNext

 void c_KgOraSQLDataReader::Close()
{
try
{
  if (m_OciStatement)
  {
    m_Connection->OCI_TerminateStatement(m_OciStatement);
    m_OciStatement=NULL;
  }
}
catch(c_Oci_Exception* ea)
{
  
  FdoStringP gstr = ea->what();
  delete ea;
  throw FdoConnectionException::Create( gstr );  
}

}//end of  c_KgOraSQLDataReader::Close



