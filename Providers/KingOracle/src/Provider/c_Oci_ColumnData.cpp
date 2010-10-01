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




#include "c_OCI_API.h"
#include "c_Oci_ColumnData.h"


c_Oci_ColumnData::c_Oci_ColumnData(  )
{
  m_CurrentRow=-1;
  m_CurrentPtr_ScalarInd = NULL;
  
  m_DataLobLocator=NULL;
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
    
    
    case SQLT_LBI:
      m_DataBufferType = e_OciLongRaw;
    break;
    
    case SQLT_BLOB:
      m_DataBufferType = e_OciBlob;
    break;
    case SQLT_CLOB:
      m_DataBufferType = e_OciClob;
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
          FdoStringP buff;
          
          if( TypeName )
            buff = FdoStringP::Format(L"c_Oci_ColumnData::Set : Unsupported Named Type '%s'!",TypeName);
          else
            buff = FdoStringP::Format(L"c_Oci_ColumnData::Set : Unsupported Named Type 'NULL'!");

          throw new c_Oci_Exception (0,0,(FdoString*)buff);
        }
      }
    }
    break;
    
    default:
    {
      FdoStringP buff = FdoStringP::Format(L"c_Oci_ColumnData::Set : Unsupported Data type %d",OciDataType);
      throw new c_Oci_Exception (0,0,(FdoString*)buff);
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
    case e_OciBlob:
    case e_OciClob:
    {
      m_DataLobLocator = new OCILobLocator*[m_DataArraySize];
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_OciConn->OciCheckError( OCIDescriptorAlloc(m_OciConn->m_OciHpEnvironment,(void**)&m_DataLobLocator[ind],OCI_DTYPE_LOB,0,0) );
      }
      m_ScalarInd = new sb2[m_DataArraySize];      
    }
    break;
    case e_OciLongRaw:
    
    {
      m_ColSize = 64000;
      m_DataLongRaw = new ub1[m_DataArraySize*m_ColSize];
      m_DataLength = new ub2[m_DataArraySize];
      /*
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_DataLob[ind]=new ub1[32000];
      }
      */
      
    /*
      m_DataLob = new OCILobLocator*[m_DataArraySize];
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        m_DataLob[ind]=NULL;
      }
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
         OCIDescriptorAlloc((dvoid*)OciConn->m_OciHpEnvironment, (dvoid **)&m_DataLob[ind],(ub4)OCI_DTYPE_LOB, (size_t)0,(dvoid**)0);
      }
    */  
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
    
    case e_OciBlob:
    case e_OciClob:
    {
      if( m_DataLobLocator )
      {
        for(int ind=0;ind<m_DataArraySize;ind++)
        {
          OCIDescriptorFree(&m_DataLobLocator[ind],OCI_DTYPE_LOB);
                          
        }
        delete []m_DataLobLocator;
      }
      delete []m_ScalarInd;
    }
    break;
    case e_OciLongRaw:
    {
    /*
      for(int ind=0;ind<m_DataArraySize;ind++)
      {
        if( m_DataLob[ind] )
        {
          c_OCI_API::OciCheckError(m_OciConn->m_OciHpError, OCIDescriptorFree(m_DataLob[ind],OCI_DTYPE_LOB ));      
          m_DataLob[ind] = NULL;
        }                
      }
      */
      delete []m_DataLongRaw;
      delete []m_ScalarInd;
      if( m_DataLength ) delete []m_DataLength;
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
    case e_OciLongRaw:
    case e_OciBlob:
    case e_OciClob:
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

bool c_Oci_ColumnData::IsClob()
{
  return m_DataBufferType == e_OciClob;
}
bool c_Oci_ColumnData::IsBlob()
{
  return m_DataBufferType == e_OciBlob;
}

OCINumber* c_Oci_ColumnData::GetOciNumber()
{
  if( m_DataBufferType==e_OciNumber )
  {
    return &m_DataNumber[m_CurrentRow];
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not OCINumber!");
}
OCIDate* c_Oci_ColumnData::GetOciDate()
{
  if( m_DataBufferType==e_OciDateTime)
  {
    return &m_DataDateTime[m_CurrentRow];
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not OCIDate!");
}


const wchar_t* c_Oci_ColumnData::GetString()
{
  if( m_DataBufferType==e_OciString )
  {
    //return m_DataStringPtrArray[m_CurrentRow];
    return &m_DataStringPtr[m_CurrentRow*(m_ColSize+1)];
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
}
SDO_GEOMETRY_TYPE* c_Oci_ColumnData::GetSdoGeom(SDO_GEOMETRY_ind** GeomInd)
{
  if( m_DataBufferType==e_OciSdoGeometry)
  {
    if( GeomInd ) *GeomInd = m_SdoGeomInd[m_CurrentRow];
    return m_DataSdoGeom[m_CurrentRow];
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
}


long c_Oci_ColumnData::GetLongRawLength()
{
  if( m_DataBufferType==e_OciBlob || m_DataBufferType==e_OciClob )
  {
    ub4 length;
    m_OciConn->OciCheckError(OCILobGetLength(m_OciConn->m_OciHpServiceContext,m_OciConn->m_OciHpError,m_DataLobLocator[m_CurrentRow],&length));
    return length;
  }
  if( m_DataBufferType==e_OciLongRaw || m_DataBufferType==e_OciBlob || m_DataBufferType==e_OciClob)
  {
    ub4 length=0;
    /*
    if (OCILobGetLength(m_OciConn->m_OciHpServiceContext,m_OciConn->m_OciHpError, m_DataLob[m_CurrentRow], &length) != OCI_SUCCESS)
    {
      throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData: Error 'OCILobGetLength'!");
    }
    */
    length = m_DataLength[m_CurrentRow];
    return length;
  }
  

  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not Blob!");
}

unsigned char* c_Oci_ColumnData::GetLongRaw()
{
  if( m_DataBufferType==e_OciLongRaw )
  {
    return &m_DataLongRaw[m_CurrentRow*m_ColSize];
  }

  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not Blob!");
}

void c_Oci_ColumnData::GetLobData(unsigned long& BuffSize,void* BuffPtr)
{
  if( m_DataBufferType==e_OciBlob || m_DataBufferType==e_OciClob)
  {
    oraub8 amtp = BuffSize;
    
    m_OciConn->OciCheckError( OCILobRead2 ( m_OciConn->m_OciHpServiceContext,m_OciConn->m_OciHpError,
      m_DataLobLocator[m_CurrentRow],
      &amtp, // oraub8             *byte_amtp,
      0, //oraub8             *char_amtp,
      1, // oraub8             offset,
      BuffPtr, // void               *bufp,
      BuffSize, // oraub8             bufl,
      OCI_ONE_PIECE , // ub1                piece,
      0, // void               *ctxp, 
      NULL, //OCICallbackLobRead2 (cbfp) (void *ctxp,const void *bufp,oraub8 lenp,ub1 piecep,void **changed_bufpp,oraub8 *changed_lenp)
      OCI_UTF16ID, // ub2                csid,
      SQLCS_IMPLICIT  // ub1                csfrm 
      ));
      
    BuffSize =      amtp; 
    return;      
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not Blob!");
}

OCIArray* c_Oci_ColumnData::GetSdoDimArray()
{
  if( m_DataBufferType==e_OciSdoDimArray)
  {
    return m_DataSdoDimArray[m_CurrentRow];
  }
  
  throw new c_Oci_Exception(0,0,L"c_Oci_ColumnData:: ColumnData is not String!");
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
    case e_OciBlob:
    case e_OciClob:
    {
      return m_DataLobLocator;
    }
    break;
    case e_OciLongRaw:
    
    {
      return m_DataLongRaw;
      /*
      if( m_DataArraySize == 1)
      return m_DataSdoGeom[0];
      else
      return m_DataSdoGeom;
      */       
    }
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
  case e_OciLongRaw:
  case e_OciBlob:
  case e_OciClob:
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

void* c_Oci_ColumnData::GetDataRealLengthBuffer()
{
  switch(m_DataBufferType)
  {
   case e_OciLongRaw:
   case e_OciBlob:
   case e_OciClob:
    {      
      return m_DataLength;
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
    case e_OciLongRaw:
    {
      return m_OciDataType;  
    }
    break;
    case e_OciBlob:
    case e_OciClob:
    {
      //return m_OciDataType ;  
      return m_OciDataType;
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
    case e_OciLongRaw:
    {
      return m_ColSize;
    }
    break;
    
    case e_OciBlob:
    case e_OciClob:
    {
      return sizeof(OCILobLocator*);
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
