/*
* Copyright (C) 2008  SL-King d.o.o
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
#include "c_SdoGeomToAGF2.h"
#include <time.h>


#define D_USE_MAP_FOR_COLUMN_SEARCH


class c_StringToIndex
{
public:
  c_StringToIndex(FdoStringCollection* SqlColumns)
  {
    //m_SqlColumns  = SqlColumns;
    //FDO_SAFE_ADDREF(m_SqlColumns.p);
    if( SqlColumns )
    {
      m_CountNames = SqlColumns->GetCount();
      m_SqlColumns = new const wchar_t*[m_CountNames];
      m_ArrayIndLookup = new int[m_CountNames];        
      for(int ind=0;ind<m_CountNames;ind++)
      {
        m_ArrayIndLookup[ind] = ind;
        m_SqlColumns[ind] = SqlColumns->GetString(ind);
      }
    }
    else
    {
      m_CountNames = 0;
      m_SqlColumns = NULL;
      m_ArrayIndLookup = NULL;
    }
    m_CurrIndLookup=0;
  }
  ~c_StringToIndex()
  {
    if( m_ArrayIndLookup ) delete []m_ArrayIndLookup;
    if( m_SqlColumns ) delete []m_SqlColumns;
  }
  
  int GetIndex(FdoString* Name)
  {
    int ind3=0;
        // check on next lookup ind
    ind3 = m_ArrayIndLookup[m_CurrIndLookup];
    //method2_count_strcmp++;
    //if( wcscmp(m_SqlColumns->GetString(ind3),Name) == 0 )
    if( wcsicmp(m_SqlColumns[ind3],Name) == 0 )
    {
      m_CurrIndLookup++;
      if( m_CurrIndLookup >= m_CountNames ) m_CurrIndLookup=0;
            
    }
    else
    {
      int next_ind_lookup = m_CurrIndLookup + 1;
      if( next_ind_lookup>= m_CountNames ) next_ind_lookup = 0;
      
      bool found=false;
      while( next_ind_lookup!=m_CurrIndLookup)
      {
        ind3 = m_ArrayIndLookup[next_ind_lookup];
        //method2_count_strcmp++;
        //if( wcscmp(m_SqlColumns->GetString(ind3),Name) == 0 )
        if( wcsicmp(m_SqlColumns[ind3],Name) == 0 )
        {
          int temp = m_ArrayIndLookup[m_CurrIndLookup];
          m_ArrayIndLookup[m_CurrIndLookup] = m_ArrayIndLookup[next_ind_lookup];
          m_ArrayIndLookup[next_ind_lookup] = temp;
        
          m_CurrIndLookup++;
          if( m_CurrIndLookup >= m_CountNames ) m_CurrIndLookup=0;
          found = true;
          break;
        }
        
        next_ind_lookup++;
        if( next_ind_lookup>= m_CountNames ) next_ind_lookup = 0;
      }
      
      if( !found ) 
      {
        throw FdoCommandException::Create(L"Unknown Property Name!");
      }
    }
    
    return ind3;
  }
  
protected:
  int m_CountNames;
  int *m_ArrayIndLookup;  
  
  int m_CurrIndLookup;
  
  //FdoPtr<FdoStringCollection> m_SqlColumns;
  const wchar_t** m_SqlColumns;
  
};

template <class FDO_READER> class c_KgOraReader : public FDO_READER
{
    public:
    
        friend class c_KgOraDataReader;
        
        c_KgOraReader(c_KgOraConnection * Connection
                            ,c_Oci_Statement* OcciStatement 
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
        virtual int PropNameToColumnNumber(FdoString* propertyName); 
        virtual int PropNameToColumnNumber_IsNull(FdoString* propertyName); 
        
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
        #ifdef _KGORA_EXTENDED_LOG
        
        long m_CountRows;
        //FdoPtr<FdoByteArray> m_Barray;
        //FdoCommonThreadMutex m_Mutex;
        #endif
        
        
        c_Oci_Statement* m_OciStatement;
        
        c_KgOraConnection * m_Connection;
        
       
        c_StringToIndex m_PropNameToIndex;
        c_StringToIndex m_PropNameToIndex_IsNull;
        
        int m_GeomPropSqlIndex; // Index of Geometry Column in sql result set ( for fast access to geometry column - no need to transfer propertyname to column index)
        FdoPtr<FdoStringCollection> m_SqlColumnCollStr;
        //std::map<std::wstring,int> m_SqlColumnsMap;
        
        
        c_SdoGeomToAGF2 m_SdoAgfConv;
        FdoStringP m_CachedString; // buffer to save last retrived string with GetString
       
        
        FdoPtr<FdoByteArray> m_Barray;
        
        
};


template <class FDO_READER> c_KgOraReader<FDO_READER>::c_KgOraReader(c_KgOraConnection * Connection
                                      ,c_Oci_Statement* OciStatement 
                                        ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                        )
 
  : m_PropNameToIndex(SqlColumns),m_PropNameToIndex_IsNull(SqlColumns)
{
#ifdef _KGORA_EXTENDED_LOG
  m_CountRows = 0;
#endif  
  m_Connection = Connection;
  FDO_SAFE_ADDREF(m_Connection);
  
  
  m_GeomPropSqlIndex = GeomPropSqlIndex;
  m_SqlColumnCollStr = SqlColumns;
  FDO_SAFE_ADDREF(m_SqlColumnCollStr.p);
  
  //std::map
  /*
  long count=m_SqlColumnColl->GetCount();
  for(long ind = 0; ind<count;ind++ )
  {
    m_SqlColumnsMap[m_SqlColumnColl->GetString(ind)] = ind+1;
  }
  */
  m_OciStatement = OciStatement;
  
  

  
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


template <class FDO_READER> int c_KgOraReader<FDO_READER>::PropNameToColumnNumber(FdoString* propertyName)
{
  return m_PropNameToIndex.GetIndex(propertyName) + 1;
/*  
  int oraind=0;
  #ifdef D_USE_MAP_FOR_COLUMN_SEARCH
    std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
    if( iter == m_SqlColumnsMap.end() )
    {
      throw FdoCommandException::Create(L"Unknown Property Name!");
    }
    oraind = iter->second;
  #else 
    oraind = m_SqlColumnColl->IndexOf(propertyName)+1;
  #endif
  
  return oraind;
  */
};
template <class FDO_READER> int c_KgOraReader<FDO_READER>::PropNameToColumnNumber_IsNull(FdoString* propertyName)
{
  return m_PropNameToIndex_IsNull.GetIndex(propertyName) + 1;
  
  /*
  int oraind=0;
  #ifdef D_USE_MAP_FOR_COLUMN_SEARCH
    std::map<std::wstring,int>::iterator iter = m_SqlColumnsMap.find(propertyName);
    if( iter == m_SqlColumnsMap.end() )
    {
      throw FdoCommandException::Create(L"Unknown Property Name!");
    }
    oraind = iter->second;
  #else 
    oraind = m_SqlColumnColl->IndexOf(propertyName)+1;
  #endif
  
  return oraind;
  */
};

template <class FDO_READER> bool c_KgOraReader<FDO_READER>::IsNull(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber_IsNull(propertyName); 
  
  
  if( m_OciStatement && (oraind >= 1) )
  {
    return m_OciStatement->IsColumnNull(oraind);         
  } 
  return true;
}


template <class FDO_READER> bool c_KgOraReader<FDO_READER>::GetBoolean(FdoString* propertyName)
{
  
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    std::wstring str = m_OciStatement->GetString(oraind);    
    
    if( str.compare(L"1") == 0 ) return true;
    if( str.compare(L"TRUE") == 0 ) return true;
    if( str.compare(L"true") == 0 ) return true;
    
    return false;
  }
  throw FdoCommandException::Create(L"Boolean");
}

template <class FDO_READER> FdoByte  c_KgOraReader<FDO_READER>::GetByte(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    std::wstring str = m_OciStatement->GetString(oraind);    
    return *((char*)str.c_str());
    //const char* cp = str.c_str();
    //if( cp ) return *cp;    
    //return 0;
  }
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetByte

template <class FDO_READER> FdoDateTime c_KgOraReader<FDO_READER>::GetDateTime(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( oraind >= 1 )
  {
    OCIDate* oradt = m_OciStatement->GetOciDate(oraind);    
    FdoDateTime fdodate;
    if( oradt )
    {
      //unsigned int month,day,hour,min,sec;
      //oradt.getDate(year,month,day,hour,min,sec);
      
      fdodate.year = oradt->OCIDateYYYY;
      fdodate.month = oradt->OCIDateMM;
      fdodate.day = oradt->OCIDateDD;
      fdodate.hour = oradt->OCIDateTime.OCITimeHH;
     
      fdodate.minute = oradt->OCIDateTime.OCITimeMI;
      fdodate.seconds = oradt->OCIDateTime.OCITimeSS;
      
    }
    return fdodate;
  }
  throw FdoCommandException::Create(L"Feature Reader: Unknown Property Name");
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetDateTime

template <class FDO_READER> double c_KgOraReader<FDO_READER>::GetDouble(FdoString* propertyName)
{

  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    double val = m_OciStatement->GetDouble(oraind);    
    return val;
  }
    
    
  throw FdoCommandException::Create(L"Double");
    
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetDouble

template <class FDO_READER> FdoInt16 c_KgOraReader<FDO_READER>::GetInt16(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    int val = m_OciStatement->GetInteger(oraind);    
    return val;
  }
  
  throw FdoCommandException::Create(L"Int16");
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::GetInt16

template <class FDO_READER> FdoInt32 c_KgOraReader<FDO_READER>::GetInt32(FdoString* propertyName)
{
 
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  { 
    long val;
    try
    {
      val = m_OciStatement->GetInteger(oraind);    
    }
    catch(c_Oci_Exception* ea)
    {
      delete ea;
      printf("\n----------------------c_KgOraReader::GetInt32: occi::SQLException Exception ---------------------- ");
      
      return 0;
    }
    catch(...)
    {
      printf("\n----------------------c_KgOraReader::GetInt32: Uknown Exception ---------------------- ");
     
      return 0;
    }
    
    return val;
  }
    
    //return 5;
    
    throw FdoCommandException::Create(L"Int32");
}

template <class FDO_READER> FdoInt64 c_KgOraReader<FDO_READER>::GetInt64(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    //long val = m_OciStatement->getInt(oraind);    
    long val = m_OciStatement->GetLong(oraind);    
    return val;
  }
  
    throw FdoCommandException::Create(L"Int64");
}

template <class FDO_READER> float c_KgOraReader<FDO_READER>::GetSingle(FdoString* propertyName)
{
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    double val = m_OciStatement->GetDouble(oraind);    
    return (float)val;
  }
    
    
  
  throw FdoCommandException::Create(L"Float32");
}

template <class FDO_READER> FdoString* c_KgOraReader<FDO_READER>::GetString(FdoString* propertyName)
{
 
  int oraind = PropNameToColumnNumber(propertyName); 
  
  if( m_OciStatement && (oraind >= 1) )
  {
    return m_OciStatement->GetString(oraind); 
    
    //m_CachedString = str.c_str();
    
    //return m_CachedString;
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

 

  if( m_OciStatement )
  {
    c_SDO_GEOMETRY *geom=NULL;
    SDO_GEOMETRY_TYPE *s_geom=NULL;
    SDO_GEOMETRY_ind* s_geom_ind=NULL;
    
    try
    {
      int oraind = PropNameToColumnNumber(propertyName); 
      if( !m_OciStatement->IsColumnNull(oraind) )
      {
        geom = m_OciStatement->GetSdoGeom(oraind); // oracle is 1 based - our index is 0 based        
      }
    }
    catch(c_Oci_Exception* ea)
    {
      delete ea;
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
  
    /*
    delete geom;
    
    // Geometry property
    FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();

    double ordinates[10] = { 2,2 ,20,2 ,20,20 ,2,20 ,2,2 }; 

    FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
    FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);

    FdoPtr<FdoByteArray> m_Barray = fgf->GetFgf(polygon);

    *len = m_Barray->GetCount();
    return m_Barray->GetData();
    */
    
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
  if( m_OciStatement )
  {
 
    
    //m_Mutex.Enter();
    SDO_GEOMETRY *geom;
    if( !m_OciStatement->isNull(m_GeomPropSqlIndex+1) )
    {
      geom = (SDO_GEOMETRY*)m_OciStatement->getObject(m_GeomPropSqlIndex+1); // oracle is 1 based - our index is 0 based
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
  

    
    try
    {
    if( !m_OciStatement->ReadNext() )
    {
     
      #ifdef _KGORA_EXTENDED_LOG
      
      {
        char buff[1024]; 
        sprintf(buff,"c_KgOraReader.ReadNext : Reader reached end. Rows count=%ld ",(long)m_CountRows);        
        D_KGORA_ELOG_WRITE(buff);                
        #ifdef _DEBUG
          printf("\n");
          printf(buff);
        #endif
        
      }
    #endif
      return false;
    }
    
    }
    catch(c_Oci_Exception* ea)
    {
      delete ea;
      printf("\n----------------------c_KgOraReader::ReadNext: occi::SQLException Exception ---------------------- ");
      return false;
    }
    catch(...)
    {
      printf("\n----------------------c_KgOraReader::ReadNext: Uknown Exception ---------------------- ");
      return false;
    }
    
    #ifdef _KGORA_EXTENDED_LOG
    m_CountRows++;
    #endif
    return true;
  
}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::ReadNext

template <class FDO_READER> void c_KgOraReader<FDO_READER>::Close()
{
try
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()");
  
  if (m_OciStatement)
  {
    D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close OCI_TerminateStatement");
    m_Connection->OCI_TerminateStatement(m_OciStatement);
    m_OciStatement=NULL;
  }
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close().. OK");
}
catch(c_Oci_Exception* ea)
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()... ERROR!");
  printf("\n----------------------c_KgOraReader::Close: occi::SQLException Exception ---------------------- ");
  m_OciStatement = NULL;
  
  FdoStringP gstr = ea->GetErrorText();
  delete ea;
  throw FdoConnectionException::Create( gstr );  
}
catch(...)
{
  D_KGORA_ELOG_WRITE("\nc_KgOraReader<FDO_READER>::Close()... ERROR!");
  printf("\n----------------------c_KgOraReader::Close: Uknown Exception ---------------------- ");
}

}//end of template <class FDO_READER> c_KgOraReader<FDO_READER>::Close





#endif