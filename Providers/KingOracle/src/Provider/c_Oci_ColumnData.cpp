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

#include "StdAfx.h"




#include "c_OCI_API.h"
#include "c_Oci_ColumnData.h"


c_Oci_ColumnData::c_Oci_ColumnData(  )
{
  m_CurrentRow=-1;
  m_CurrentPtr_ScalarInd = NULL;
  
  m_DataArraySize = 0;
  
  m_ColumnNumber = 0;
  m_OciDataType = 0;
  m_ColSize = 0;
  
  m_DataBufferType = e_Undefined;
}
void c_Oci_ColumnData::Set( c_Oci_Connection*OciConn,int ColumnNumber,int OciDataType,const wchar_t*TypeName,int ColumnSize,int DataArraySize )
{
  m_OciType=NULL;
  m_CurrentRow=-1;
  m_DataArraySize = DataArraySize;
  
  m_OciConn = OciConn;
  m_ColumnNumber = ColumnNumber;
  m_OciDataType = OciDataType;
  m_ColSize = ColumnSize;
  
  m_DataBufferType = e_Undefined;
  
  switch(OciDataType)
  {
    case SQLT_INT:
    case SQLT_NUM:
    case SQLT_VNU:
    case SQLT_PDN:
    case OCI_TYPECODE_SMALLINT:
    case OCI_TYPECODE_BFLOAT:
    case OCI_TYPECODE_FLOAT:
    case OCI_TYPECODE_BDOUBLE:
    case OCI_TYPECODE_DOUBLE:
    case OCI_TYPECODE_REAL:
      m_DataBufferType = e_OciNumber;
    break;
    
    case SQLT_VCS:          
    case SQLT_CHR:  
    case SQLT_AFC:    // OCI_TYPECODE_CHAR   
      m_DataBufferType = e_OciString;
    break;
    
    case SQLT_DATE:
    case SQLT_DAT:
    case SQLT_ODT:
    case SQLT_TIMESTAMP:
      m_DataBufferType = e_OciDateTime;
    break;

    
    case SQLT_NTY:
    {
      if( TypeName && (wcscmp(TypeName,L"SDO_GEOMETRY")==0) )
      {
        m_DataBufferType = e_OciSdoGeometry;
        m_OciType = OciConn->m_OciType_SdoGeometry;
      }
      else
      {
        if( TypeName && (wcscmp(TypeName,L"SDO_DIM_ARRAY")==0) )
        {
          m_DataBufferType = e_OciSdoDimArray;
          m_OciType = OciConn->m_OciType_SdoDimArray;
        }
        else
        {
          wchar_t buff[256];
          
          if( TypeName )
            wsprintf(buff,L"c_Oci_ColumnData::Set : Unsupported Named Type '%s'!",TypeName);
          else
            wsprintf(buff,L"c_Oci_ColumnData::Set : Unsupported Named Type 'NULL'!");
          throw new c_Oci_Exception (0,0,buff);
        }
      }
    }
    break;
    
    default:
    {
      wchar_t buff[256];
      wsprintf(buff,L"c_Oci_ColumnData::Set : Unsupported Data type %d",OciDataType);
      throw new c_Oci_Exception (0,0,buff);
    }
    break;
  }
  
  
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    {
      m_MemBuffPtrArray = new char*[m_DataArraySize];
      
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_MemBuffPtrArray[ind] = new char[m_ColSize];        
      }      
      
      m_ScalarInd = new sb2[m_DataArraySize];
    }
    break;
    case e_OciString:
    {
      /*
      m_DataStringPtrArray = new wchar_t*[m_DataArraySize];
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_DataStringPtrArray[ind] = new wchar_t[m_ColSize+1];
      }
      */
      m_DataStringPtr = new wchar_t[m_DataArraySize*(m_ColSize+1)];
      m_ScalarInd = new sb2[m_DataArraySize];
    }
    break;
    case e_OciNumber:
    {
      m_DataNumber = new OCINumber[m_DataArraySize];
      m_ScalarInd = new sb2[m_DataArraySize];
    }
    break;
    case e_OciDateTime:
    {
      m_DataDateTime = new OCIDate[m_DataArraySize];
      m_ScalarInd = new sb2[m_DataArraySize];      
    }
    break;
    case e_OciSdoGeometry:
    {
      m_DataSdoGeom = new SDO_GEOMETRY_TYPE*[m_DataArraySize];
      m_SdoGeomInd = new SDO_GEOMETRY_ind*[m_DataArraySize];;
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_DataSdoGeom[ind]=NULL;
        m_SdoGeomInd[ind]=NULL;
      }
    }
    break;
    case e_OciSdoDimArray:
    {
      
      m_DataSdoDimArray = new OCIArray*[m_DataArraySize];
      m_SdoDimArrayInd = new sb2*[m_DataArraySize];
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_DataSdoDimArray[ind]=NULL;
        m_SdoDimArrayInd[ind]=NULL;
      }
      
      
      
    }
    break;
    
  }
  
  
}

c_Oci_ColumnData::~c_Oci_ColumnData(  )
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    {
      char *buff;
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        buff = m_MemBuffPtrArray[ind];
        delete []buff;
        
      }
      delete []m_MemBuffPtrArray;
      
      delete []m_ScalarInd;
    }
    break;
    case e_OciString:
    {
      /*
      wchar_t *wstr;
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        wstr = m_DataStringPtrArray[ind];
        delete []wstr;
        
      }
      delete []m_DataStringPtrArray;
      */
      delete []m_DataStringPtr;
      
      delete []m_ScalarInd;
    }
    break;
    case e_OciNumber:
    {
      delete []m_DataNumber;
      delete []m_ScalarInd;
    }
    break;
    case e_OciDateTime:
    {
      delete []m_DataDateTime;
      delete []m_ScalarInd;
    }
    break;
    case e_OciSdoGeometry:
    {
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        if( m_DataSdoGeom[ind] )
        {
          c_OCI_API::OciCheckError(m_OciConn->m_OciHpError, OCIObjectFree(m_OciConn->m_OciHpEnvironment, m_OciConn->m_OciHpError,m_DataSdoGeom[ind],0));      
          m_DataSdoGeom[ind] = NULL;
        }                
      }
      
      delete []m_DataSdoGeom;
      delete []m_SdoGeomInd;
    }
    break;
    case e_OciSdoDimArray:
    {
      delete []m_DataSdoDimArray;
      delete []m_SdoDimArrayInd;
    }
    break;
    
  }
}

bool c_Oci_ColumnData::IsNull()
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    case e_OciString:
    case e_OciNumber:    
    case e_OciDateTime:
    {      
      return m_ScalarInd[m_CurrentRow] != 0;
      
      //return true;
    }
    break;
    
    case e_OciSdoGeometry:
    {
      return m_SdoGeomInd[m_CurrentRow]->_atomic == OCI_IND_NULL;      
    }
    break;
    
    case e_OciSdoDimArray:
    {
      return *m_SdoDimArrayInd[m_CurrentRow] != 0;      
    }
    break;
    
  }
  
  return true;
}


OCINumber* c_Oci_ColumnData::GetOciNumber()
{
  if( m_DataBufferType==e_OciNumber )
  {
    return &m_DataNumber[m_CurrentRow];
  }
  
  throw c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not OCINumber!");
}
OCIDate* c_Oci_ColumnData::GetOciDate()
{
  if( m_DataBufferType==e_OciDateTime)
  {
    return &m_DataDateTime[m_CurrentRow];
  }
  
  throw c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not OCIDate!");
}


const wchar_t* c_Oci_ColumnData::GetString()
{
  if( m_DataBufferType==e_OciString )
  {
    //return m_DataStringPtrArray[m_CurrentRow];
    return &m_DataStringPtr[m_CurrentRow*(m_ColSize+1)];
  }
  
  throw c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
}
SDO_GEOMETRY_TYPE* c_Oci_ColumnData::GetSdoGeom(SDO_GEOMETRY_ind** GeomInd)
{
  if( m_DataBufferType==e_OciSdoGeometry)
  {
    if( GeomInd ) *GeomInd = m_SdoGeomInd[m_CurrentRow];
    return m_DataSdoGeom[m_CurrentRow];
  }
  
  throw c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
}
OCIArray* c_Oci_ColumnData::GetSdoDimArray()
{
  if( m_DataBufferType==e_OciSdoDimArray)
  {
    return m_DataSdoDimArray[m_CurrentRow];
  }
  
  throw c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
}

void* c_Oci_ColumnData::GetDataDefineBuffer()
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    {
      return m_MemBuffPtrArray;
      
    }
    break;
    case e_OciString:
    {
      return m_DataStringPtr;
      
      /*
      if( m_DataArraySize == 1)
        return m_DataStringPtrArray[0];
      else
        return m_DataStringPtrArray;
      */
      
    }
    break;
    case e_OciNumber:
    {
      return m_DataNumber;      
    }
    break;
    case e_OciDateTime:
    {
      return m_DataDateTime;      
    }
    break;
    case e_OciSdoGeometry:
    {
      return m_DataSdoGeom;
    /*
      if( m_DataArraySize == 1)
        return m_DataSdoGeom[0];
      else
        return m_DataSdoGeom;
    */       
    }
    break;
    case e_OciSdoDimArray:
    {
      return m_DataSdoDimArray;
      /*
      if( m_DataArraySize == 1)
        return m_DataSdoDimArray[0];
      else
        return m_DataSdoDimArray;
      */     
    }
    break;
    
  }  
  
  return NULL;
}

void* c_Oci_ColumnData::GetDataIndDefineBuffer()
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    case e_OciString:
    case e_OciNumber:    
    case e_OciDateTime:
    {      
      return m_ScalarInd;
    }
    break;
    
    case e_OciSdoGeometry:
    {
      return m_SdoGeomInd;      
    }
    break;
    
    case e_OciSdoDimArray:
    {
      return m_SdoDimArrayInd;      
    }
    break;
    
  }
  
  return NULL;
}

int c_Oci_ColumnData::GetDataDefineType()
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    {
      return m_OciDataType;
      
    }
    break;
    case e_OciString:
    {
      return SQLT_STR;   
      //return m_OciDataType;   
    }
    break;
    case e_OciNumber:
    {
      return SQLT_VNU;      
    }
    break;
    case e_OciDateTime:
    {
      //return SQLT_DAT;      
      return SQLT_ODT;
    }
    break;
    case e_OciSdoGeometry:
    case e_OciSdoDimArray:
    {
      return SQLT_NTY;      
    }
    break;
    
  }  
  
  return NULL;
}

int c_Oci_ColumnData::GetDataDefineSize()
{
  switch(m_DataBufferType)
  {
    case e_MemBuffer:
    {
      return m_ColSize;
      
    }
    break;
    case e_OciString:
    {
      return m_ColSize*sizeof(wchar_t) + sizeof(wchar_t);      
    }
    break;
    case e_OciNumber:
    {
      return sizeof(OCINumber);      
    }
    break;
    case e_OciDateTime:
    {
      return sizeof(OCIDate);      
      //return m_ColSize;      
    }
    break;
    case e_OciSdoGeometry:
    case e_OciSdoDimArray:
    {
      return 0;      
    }
    break;
    
  }  
  
  return NULL;
}