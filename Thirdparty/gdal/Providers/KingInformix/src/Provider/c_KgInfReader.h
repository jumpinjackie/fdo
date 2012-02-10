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

#ifndef _c_KgInfReader_h
#define _c_KgInfReader_h


#include <time.h>





template <class FDO_READER> class c_KgInfReader : public FDO_READER
{
    public:
        c_KgInfReader(c_KgInfConnection * Connection
                            ,otl_stream* OtlStream,c_KgOtlStreamReader* OtlStreamReader
                            ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns);

    protected:
        virtual ~c_KgInfReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
    // dispose this object
        virtual void Dispose();

    

    public:
        
    //-------------------------------------------------------
    // FdoIReader implementation
    //-------------------------------------------------------   
        virtual bool GetBoolean(FdoString* propertyName);
        virtual FdoByte GetByte(FdoString* propertyName);
        virtual FdoDateTime GetDateTime(FdoString* propertyName);
        virtual double GetDouble(FdoString* propertyName);
        virtual FdoInt16 GetInt16(FdoString* propertyName);
        virtual FdoInt32 GetInt32(FdoString* propertyName);
        virtual FdoInt64 GetInt64(FdoString* propertyName);
        virtual float GetSingle(FdoString* propertyName);
        virtual FdoString* GetString(FdoString* propertyName);
        virtual FdoLOBValue* GetLOB(FdoString* propertyName);
        virtual FdoIStreamReader* GetLOBStreamReader(FdoString* propertyName );
        virtual bool IsNull(FdoString* propertyName);
        
        
        
        virtual FdoIRaster* GetRaster(FdoString* propertyName);
        virtual bool ReadNext();
        virtual void Close();
    
   

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// AGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// \param count 
    /// Output the number of bytes in the array.
    /// 
    /// \return
    /// Returns a pointer to the byte array in AGF format.
    /// 
    FDOKGINF_API virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// AGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte array in AGF format.
    /// 
    FDOKGINF_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

       private:
        otl_stream* m_OtlStream;
        
        
        c_KgInfConnection * m_Connection;
        
        c_KgOtlStreamReader * m_OtlStreamReader;
        
        int m_GeomPropSqlIndex; // Index of Geometry Column in sql result set ( for fast access to geometry column - no need to transfer propertyname to column index)
        FdoPtr<FdoStringCollection> m_SqlColumnColl;
        std::map<std::wstring,int> m_SqlColumnsMap;
        
        
        FdoStringP m_CachedString; // buffer to save last retrived string with GetString
        FdoPtr<FdoByteArray> m_CachedGeometry;
        
};


template <class FDO_READER> c_KgInfReader<FDO_READER>::c_KgInfReader(c_KgInfConnection * Connection
                                      ,otl_stream* OtlStream,c_KgOtlStreamReader* OtlStreamReader
                                        ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                        )
 
{
  
  m_Connection = Connection;
  FDO_SAFE_ADDREF(m_Connection);
  
  
  m_GeomPropSqlIndex = GeomPropSqlIndex;
  m_SqlColumnColl = SqlColumns;
  FDO_SAFE_ADDREF(m_SqlColumnColl.p);
  
  //std::map
  long count=m_SqlColumnColl->GetCount();
  for(long ind = 0; ind<count;ind++ )
  {
    m_SqlColumnsMap[m_SqlColumnColl->GetString(ind)] = ind+1;
  }
  
  m_OtlStream = OtlStream;
  m_OtlStreamReader = OtlStreamReader;  
}

template <class FDO_READER> c_KgInfReader<FDO_READER>::~c_KgInfReader()
{
    Close();
    
    FDO_SAFE_RELEASE(m_Connection);
}

template <class FDO_READER> void c_KgInfReader<FDO_READER>::Dispose()
{
    delete this;
}




template <class FDO_READER> bool c_KgInfReader<FDO_READER>::GetBoolean(FdoString* propertyName)
{
  //int oraind = m_SqlColumnsColl->IndexOf(propertyName) + 1;
  
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;

try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetBoolean(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}

throw FdoCommandException::Create(L"Boolean");
}

template <class FDO_READER> FdoByte  c_KgInfReader<FDO_READER>::GetByte(FdoString* propertyName)
{
  //int oraind = m_SqlColumnsColl->IndexOf(propertyName) + 1;
  
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
 
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetByte(oraind);
        
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}  
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::GetByte

template <class FDO_READER> FdoDateTime c_KgInfReader<FDO_READER>::GetDateTime(FdoString* propertyName)
{
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;

try
{  
  if( oraind >= 1 )
  {
    return m_OtlStreamReader->GetDateTime(oraind);    
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::GetDateTime

template <class FDO_READER> double c_KgInfReader<FDO_READER>::GetDouble(FdoString* propertyName)
{

  //int oraind = m_SqlColumns->IndexOf(propertyName) + 1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetDouble(oraind);
  
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::GetDouble

template <class FDO_READER> FdoInt16 c_KgInfReader<FDO_READER>::GetInt16(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetInt16(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}  
  throw FdoCommandException::Create(L"Int16");
  
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::GetInt16

template <class FDO_READER> FdoInt32 c_KgInfReader<FDO_READER>::GetInt32(FdoString* propertyName)
{
 
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetInt32(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}         
  throw FdoCommandException::Create(L"Int32");
}

template <class FDO_READER> FdoInt64 c_KgInfReader<FDO_READER>::GetInt64(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetInt64(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}  
    throw FdoCommandException::Create(L"Int64");
}

template <class FDO_READER> float c_KgInfReader<FDO_READER>::GetSingle(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    return m_OtlStreamReader->GetFloat(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}    
  throw FdoCommandException::Create(L"Float32");
}

template <class FDO_READER> FdoString* c_KgInfReader<FDO_READER>::GetString(FdoString* propertyName)
{
 
    //return L"";
    
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    wstring wstr;
    m_OtlStreamReader->GetString(oraind,wstr);
    
    m_CachedString = wstr.c_str();
    return m_CachedString;
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}    
    
    
    throw FdoCommandException::Create(L"GetString");
}

template <class FDO_READER> FdoLOBValue* c_KgInfReader<FDO_READER>::GetLOB(FdoString* propertyName)
{
    return NULL;
}

template <class FDO_READER> FdoIStreamReader* c_KgInfReader<FDO_READER>::GetLOBStreamReader(FdoString* propertyName )
{
    return NULL;
}

template <class FDO_READER> bool c_KgInfReader<FDO_READER>::IsNull(FdoString* propertyName)
{
  //int ind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    //D_SET_CURRENT_COL_ORAIND(m_OtlStream,oraind);    
    //return (*m_OtlStream->ss)->is_null();
    return m_OtlStreamReader->GetIsNull(oraind);
  } 
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}  
  return true;
}

template <class FDO_READER> FdoByteArray* c_KgInfReader<FDO_READER>::GetGeometry(FdoString* propertyName)
{
   std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
try
{  
  if( m_OtlStream && (oraind >= 1) )
  {
    //simon
    //return m_OtlStreamReader->GetGeometryFgf(oraind);
    return m_OtlStreamReader->GetGeometryFgf(oraind);
  }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}    
  throw FdoCommandException::Create(L"GetGeometry");
}




template <class FDO_READER> const FdoByte* c_KgInfReader<FDO_READER>::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  try
  {  
    if( m_OtlStream && (oraind >= 1) )
    {
      m_CachedGeometry = m_OtlStreamReader->GetGeometryFgf(oraind);
      
      *len = m_CachedGeometry->GetCount();
      return m_CachedGeometry->GetData();
    }
  }
  catch(c_KgOtlException& ea)
  {
    throw FdoException::Create( ea.what() );
  }    
    
    
  throw FdoCommandException::Create(L"GetGeometry");
}

template <class FDO_READER> FdoIRaster* c_KgInfReader<FDO_READER>::GetRaster(FdoString* propertyName)
{
    return NULL;
}

template <class FDO_READER> bool c_KgInfReader<FDO_READER>::ReadNext()
{    
/*
    #ifdef _KGORA_EXTENDED_LOG
      clock_t t1 = clock();
    #endif
    if( m_OtlStream->next() == oracle::occi::ResultSet::END_OF_FETCH )
    {
      #ifdef _KGORA_EXTENDED_LOG        
        D_KGORA_ELOG_WRITE1("c_KgInfReader Fetch time: %ld",m_FetchTime*CLOCKS_PER_SEC/1000);                
      #endif
      
      return false;
    }
    #ifdef _KGORA_EXTENDED_LOG
      clock_t t2 = clock();
      m_FetchTime += (long)(t2-t1);
    #endif
    return true;    
*/
try
{
  return m_OtlStreamReader->ReadNext();
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}    
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::ReadNext

template <class FDO_READER> void c_KgInfReader<FDO_READER>::Close()
{
/*
try
{

  if (m_OcciStatement && m_OtlStream)
  {
    m_OcciStatement->closeResultSet(m_OtlStream);        
    m_OtlStream = NULL;
  }
  
  if (m_OcciStatement)
  {
    m_Connection->OCCI_TerminateStatement(m_OcciStatement);
    m_OcciStatement=NULL;
  }

  
}
catch(c_KgOtlException& ea)
{
  m_OtlStream = NULL;
  FdoStringP gstr = ea.getMessage().c_str();
  throw FdoConnectionException::Create( gstr );  
}
*/
try
{
  c_KgOtl::DeleteStream(m_OtlStream); 
  if( m_OtlStreamReader )  { delete m_OtlStreamReader;m_OtlStreamReader=NULL; }
}
catch(c_KgOtlException& ea)
{
  throw FdoException::Create( ea.what() );
}  
}//end of template <class FDO_READER> c_KgInfReader<FDO_READER>::Close





#endif