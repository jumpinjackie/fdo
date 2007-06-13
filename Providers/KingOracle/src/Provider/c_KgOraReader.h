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

#ifndef _c_KgOraReader_h
#define _c_KgOraReader_h
#include "c_logApi.h"
#include "c_SdoGeomToAGF.h"
#include <time.h>

template <class FDO_READER> class c_KgOraReader : public FDO_READER
{
    public:
        c_KgOraReader(c_KgOraConnection * Connection
                            ,oracle::occi::Statement* OcciStatement ,oracle::occi::ResultSet* OcciResultSet
                            ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns);

    protected:
        virtual ~c_KgOraReader();

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
    FDOKGORA_API virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

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
    FDOKGORA_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

   

    private:
        #ifdef _DEBUG
        bool m_FirstRead;
        FdoPtr<FdoByteArray> m_Barray;
        //FdoCommonThreadMutex m_Mutex;
        #endif
        
        
        oracle::occi::Statement* m_OcciStatement;
        oracle::occi::ResultSet* m_OcciResultSet;
        
        c_KgOraConnection * m_Connection;
        
       
        
        int m_GeomPropSqlIndex; // Index of Geometry Column in sql result set ( for fast access to geometry column - no need to transfer propertyname to column index)
        FdoPtr<FdoStringCollection> m_SqlColumnColl;
        std::map<std::wstring,int> m_SqlColumnsMap;
        
        
        c_SdoGeomToAGF m_SdoAgfConv;
        FdoStringP m_CachedString; // buffer to save last retrived string with GetString
       
        
        
        
        
};


template <class FDO_READER> c_KgOraReader<FDO_READER>::c_KgOraReader(c_KgOraConnection * Connection
                                      ,oracle::occi::Statement* OcciStatement ,oracle::occi::ResultSet* OcciResultSet 
                                        ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                        )
 
{
#ifdef _DEBUG
  m_FirstRead = true;
#endif  
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
  
  m_OcciStatement = OcciStatement;
  m_OcciResultSet = OcciResultSet;
  

  
}

template <class FDO_READER> c_KgOraReader<FDO_READER>::~c_KgOraReader()
{
    
    Close();
  
    FDO_SAFE_RELEASE(m_Connection);
}

template <class FDO_READER> void c_KgOraReader<FDO_READER>::Dispose()
{
    D_KGORA_ELOG_WRITE("c_KgOraReader<FDO_READER>::Dispose");
    delete this;
}




template <class FDO_READER> bool c_KgOraReader<FDO_READER>::GetBoolean(FdoString* propertyName)
{
  //int oraind = m_SqlColumnsColl->IndexOf(propertyName) + 1;
  
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( m_OcciResultSet && (oraind >= 1) )
  {
    string str = m_OcciResultSet->getString(oraind);    
    
    if( str.compare("1") == 0 ) return true;
    if( str.compare("TRUE") == 0 ) return true;
    if( str.compare("true") == 0 ) return true;
    
    return false;
  }
  throw FdoCommandException::Create(L"Boolean");
}

template <class FDO_READER> FdoByte  c_KgOraReader<FDO_READER>::GetByte(FdoString* propertyName)
{
  //int oraind = m_SqlColumnsColl->IndexOf(propertyName) + 1;
  
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( m_OcciResultSet && (oraind >= 1) )
  {
    string str = m_OcciResultSet->getString(oraind);    
    const char* cp = str.c_str();
    if( cp ) return *cp;
    
    return 0;
  }
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetByte

template <class FDO_READER> FdoDateTime c_KgOraReader<FDO_READER>::GetDateTime(FdoString* propertyName)
{
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( oraind >= 1 )
  {
    oracle::occi::Date oradt = m_OcciResultSet->getDate(oraind);    
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
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetDateTime

template <class FDO_READER> double c_KgOraReader<FDO_READER>::GetDouble(FdoString* propertyName)
{

  //int oraind = m_SqlColumns->IndexOf(propertyName) + 1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( m_OcciResultSet && (oraind >= 1) )
  {
    double val = m_OcciResultSet->getDouble(oraind);    
    return val;
  }
    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetDouble

template <class FDO_READER> FdoInt16 c_KgOraReader<FDO_READER>::GetInt16(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  if( m_OcciResultSet && (oraind >= 1) )
  {
    int val = m_OcciResultSet->getInt(oraind);    
    return val;
  }
  
  throw FdoCommandException::Create(L"Int16");
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetInt16

template <class FDO_READER> FdoInt32 c_KgOraReader<FDO_READER>::GetInt32(FdoString* propertyName)
{
 
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  if( m_OcciResultSet && (oraind >= 1) )
  { 
    oracle::occi::Number num;
    try
    {
      num = m_OcciResultSet->getNumber(oraind);    
    }
    catch(oracle::occi::SQLException& ea)
    {
      printf("\n----------------------c_KgOraReader::GetInt32: occi::SQLException Exception ---------------------- ");
      
      return 0;
    }
    catch(...)
    {
      printf("\n----------------------c_KgOraReader::GetInt32: Uknown Exception ---------------------- ");
     
      return 0;
    }
    long val = (long)num; //m_OcciResultSet->getInt(oraind);    
    return val;
  }
    
    //return 5;
    
    throw FdoCommandException::Create(L"Int32");
}

template <class FDO_READER> FdoInt64 c_KgOraReader<FDO_READER>::GetInt64(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  if( m_OcciResultSet && (oraind >= 1) )
  {
    //long val = m_OcciResultSet->getInt(oraind);    
    oracle::occi::Number num = m_OcciResultSet->getNumber(oraind);    
    long val = (long)num; //m_OcciResultSet->getInt(oraind);    
    return val;
  }
  
    throw FdoCommandException::Create(L"Int64");
}

template <class FDO_READER> float c_KgOraReader<FDO_READER>::GetSingle(FdoString* propertyName)
{
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( m_OcciResultSet && (oraind >= 1) )
  {
    double val = m_OcciResultSet->getDouble(oraind);    
    return val;
  }
    
    
  
  throw FdoCommandException::Create(L"Float32");
}

template <class FDO_READER> FdoString* c_KgOraReader<FDO_READER>::GetString(FdoString* propertyName)
{
 
    //return L"";
    
  //int oraind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  
  if( m_OcciResultSet && (oraind >= 1) )
  {
    string str = m_OcciResultSet->getString(oraind); 
    
    m_CachedString = str.c_str();
    
    return m_CachedString;
  }
    
    
    
    throw FdoCommandException::Create(L"GetString");
}

template <class FDO_READER> FdoLOBValue* c_KgOraReader<FDO_READER>::GetLOB(FdoString* propertyName)
{
    return NULL;
}

template <class FDO_READER> FdoIStreamReader* c_KgOraReader<FDO_READER>::GetLOBStreamReader(FdoString* propertyName )
{
    return NULL;
}

template <class FDO_READER> bool c_KgOraReader<FDO_READER>::IsNull(FdoString* propertyName)
{
  //int ind = m_SqlColumns->IndexOf(propertyName)+1;
  std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
  if( iter == m_SqlColumnsMap.end() )
  {
    throw FdoCommandException::Create(L"Unknown Property Name!");
  }
  int oraind = iter->second;
  if( m_OcciResultSet && (oraind >= 1) )
  {
    return m_OcciResultSet->isNull(oraind);         
  } 
  return true;
}

template <class FDO_READER> FdoByteArray* c_KgOraReader<FDO_READER>::GetGeometry(FdoString* propertyName)
{
    int len = 0;
    const void* ptr = GetGeometry(propertyName, &len);
    
    if( len > 0 )
      return FdoByteArray::Create((const FdoByte*)ptr, len);
    else
      throw FdoException::Create(L"c_KgOraReader::GetGeometry Invalid Geometry !");
}




template <class FDO_READER> const FdoByte* c_KgOraReader<FDO_READER>::GetGeometry(FdoString* propertyName, FdoInt32* len)
{

  if( m_OcciResultSet )
  {
    SDO_GEOMETRY *geom=NULL;
    try
    {
      if( !m_OcciResultSet->isNull(m_GeomPropSqlIndex+1) )
        geom = (SDO_GEOMETRY*)m_OcciResultSet->getObject(m_GeomPropSqlIndex+1); // oracle is 1 based - our index is 0 based
    }
    catch(oracle::occi::SQLException& ea)
    {
      //printf("\n----------------------c_KgOraReader::GetGeometry: occi::SQLException Exception ---------------------- ");
      *len=0;
      
      throw FdoException::Create(L"c_KgOraReader::GetGeometry SQLException !");
    }
    catch(...)
    {
      //printf("\n----------------------c_KgOraReader::GetGeometry: Uknown Exception ---------------------- ");
      *len=0;
      throw FdoException::Create(L"c_KgOraReader::GetGeometry Uknown Exception !");
    }
  
    *len=0;
    if( geom )
    {
      m_SdoAgfConv.SetGeometry(geom);
      *len = m_SdoAgfConv.ToAGF();
       
      delete geom;
    }
    else
    {
      throw FdoException::Create(L"c_KgOraReader::GetGeometry NULL SDO_GEOMETRY!");
    }
   
    
    return (const unsigned char*)m_SdoAgfConv.GetBuff();
  }
  
  
  return (const unsigned char*)m_SdoAgfConv.GetBuff();
}

/*
template <class FDO_READER> const FdoByte* c_KgOraReader<FDO_READER>::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
  *len=0;
  if( m_OcciResultSet )
  {
 
    
    //m_Mutex.Enter();
    SDO_GEOMETRY *geom;
    if( !m_OcciResultSet->isNull(m_GeomPropSqlIndex+1) )
    {
      geom = (SDO_GEOMETRY*)m_OcciResultSet->getObject(m_GeomPropSqlIndex+1); // oracle is 1 based - our index is 0 based
      if( geom ) delete geom;
    }
    //m_Mutex.Leave();
    
    double*		ordsXY = new double[10];
	  ordsXY[0] = 1921500.0; ordsXY[1] = 423300.0; 
	  ordsXY[2] = 1921600.0; ordsXY[3] = 423300.0; 
	  ordsXY[4] = 1921600.0; ordsXY[5] = 423400.0; 
	  ordsXY[6] = 1921500.0; ordsXY[7] = 423400.0; 
	  ordsXY[8] = 1921500.0; ordsXY[9] = 423300.0; 
	
    FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
	  FdoPtr<FdoILinearRing>			ring = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXY);
	  
	    
	                FdoPtr<FdoIPolygon> geometry = gf->CreatePolygon(ring, NULL);
	                
	  m_Barray = gf->GetFgf(geometry);
    
    *len=m_Barray->GetCount();
    
 
    
    return (const unsigned char*)m_Barray->GetData();
  }
  
  
  return (const unsigned char*)m_Barray->GetData();
}
*/

template <class FDO_READER> FdoIRaster* c_KgOraReader<FDO_READER>::GetRaster(FdoString* propertyName)
{
    return NULL;
}

template <class FDO_READER> bool c_KgOraReader<FDO_READER>::ReadNext()
{    
  
    
    #ifdef _DEBUG
      if( m_FirstRead )
      {
        char buff[1024]; 
        sprintf(buff,"\nReader ReadNext=%p OcciresultSet=%p Statement=%p c_KgOraConnection=%p OcciConnection=%p",(void*)this,(void*)m_OcciResultSet,(void*)m_OcciStatement,(void*)m_Connection),(void*)m_Connection->GetOcciConnection();
        D_KGORA_ELOG_WRITE(buff);                
        printf("\nReader ReadNext START=%p OcciresultSet=%p Statement=%p c_KgOraConnection=%p OcciConnection=%p",(void*)this,(void*)m_OcciResultSet,(void*)m_OcciStatement,(void*)m_Connection),(void*)m_Connection->GetOcciConnection();
        printf("\n****************************************************");
        m_FirstRead=false;
      }
    #endif
    
    try
    {
    if( m_OcciResultSet->next() == oracle::occi::ResultSet::END_OF_FETCH )
    {
     
      #ifdef _DEBUG
      
      {
        char buff[1024]; 
        sprintf(buff,"\nReader ReadNext END=%p OcciresultSet=%p Statement=%p c_KgOraConnection=%p OcciConnection=%p",(void*)this,(void*)m_OcciResultSet,(void*)m_OcciStatement,(void*)m_Connection),(void*)m_Connection->GetOcciConnection();
        D_KGORA_ELOG_WRITE(buff);                
        printf("\nReader ReadNext END=%p OcciresultSet=%p Statement=%p c_KgOraConnection=%p OcciConnection=%p",(void*)this,(void*)m_OcciResultSet,(void*)m_OcciStatement,(void*)m_Connection),(void*)m_Connection->GetOcciConnection();
        printf("\n****************************************************");
        m_FirstRead=false;
      }
    #endif
      return false;
    }
    
    }
    catch(oracle::occi::SQLException& ea)
    {
      printf("\n----------------------c_KgOraReader::ReadNext: occi::SQLException Exception ---------------------- ");
      return false;
    }
    catch(...)
    {
      printf("\n----------------------c_KgOraReader::ReadNext: Uknown Exception ---------------------- ");
      return false;
    }
    
    return true;
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::ReadNext

template <class FDO_READER> void c_KgOraReader<FDO_READER>::Close()
{
try
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()");
  if (m_OcciStatement && m_OcciResultSet)
  {
    D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close closeResultSet");
    m_OcciStatement->closeResultSet(m_OcciResultSet);        
    m_OcciResultSet = NULL;
  }
  
  if (m_OcciStatement)
  {
    D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close OCCI_TerminateStatement");
    m_Connection->OCCI_TerminateStatement(m_OcciStatement);
    m_OcciStatement=NULL;
  }
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close().. OK");
}
catch(oracle::occi::SQLException& ea)
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()... ERROR!");
  printf("\n----------------------c_KgOraReader::Close: occi::SQLException Exception ---------------------- ");
  m_OcciResultSet = NULL;
  m_OcciStatement=NULL;
  FdoStringP gstr = ea.getMessage().c_str();
  throw FdoConnectionException::Create( gstr );  
}
catch(...)
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()... ERROR!");
  printf("\n----------------------c_KgOraReader::Close: Uknown Exception ---------------------- ");
}

}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::Close





#endif