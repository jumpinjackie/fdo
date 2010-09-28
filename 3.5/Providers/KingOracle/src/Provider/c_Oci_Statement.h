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

#ifndef _c_Oci_Statement_h_
#define _c_Oci_Statement_h_



#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

#include "OCI_SDO_GEOM_TYPES.h"
#include "c_SDO_GEOMETRY.h"
#include "c_SDO_DIM_ELEMENT.h"
#include "c_Oci_ColumnData.h"




class c_Oci_Connection;



class c_BindValueBuffer
{
public:
  c_BindValueBuffer(double Val) { m_String=NULL;m_SdoGeom=NULL; m_Double=Val; }
  c_BindValueBuffer(OCIDate Val) { m_String=NULL;m_SdoGeom=NULL; m_Date=Val; }
  c_BindValueBuffer(OCINumber Val) { m_String=NULL;m_SdoGeom=NULL; m_OciNumber=Val; }
  c_BindValueBuffer(long Val) { m_String=NULL;m_SdoGeom=NULL; m_Long=Val; }
  c_BindValueBuffer(int Val) { m_String=NULL;m_SdoGeom=NULL; m_Int=Val; }
  c_BindValueBuffer(const wchar_t*Val) { m_String=NULL;m_SdoGeom=NULL; m_String=FdoCommonOSUtil::wcsdup((wchar_t*)Val); }
  c_BindValueBuffer(c_SDO_GEOMETRY*Val) { m_String=NULL;m_SdoGeom=NULL; m_SdoGeom=Val; }
  
  ~c_BindValueBuffer() { if( m_SdoGeom ) delete m_SdoGeom; if( m_String ) free(m_String); };
  
public:
  union {
    double m_Double;
    OCIDate m_Date;
    OCINumber m_OciNumber;
    long m_Long;
    int m_Int;
    
  };
  c_SDO_GEOMETRY* m_SdoGeom;
  wchar_t* m_String;
};


class c_Oci_Statement
{
  public : 
    c_Oci_Statement(c_Oci_Connection* OciConn);
    ~c_Oci_Statement();
    
  
        
  public:    
    c_Oci_Connection* m_OciConn;
    OCIStmt 		*m_OciHpStm;
    
  protected:    
    
    bool m_HasMoreData;
    int m_RowsProcessed;
    int m_RowsFetched;
    
    
    int m_FetchArraySize; // number of rows which will be fetched in each array fetch            
    
    int m_ColumnDataAllocated;
    int m_ColumnDataSize;
    c_Oci_ColumnData** m_ColumnDataPtrArray;
    
    std::vector<c_BindValueBuffer*>  m_VectorBindValue;
  
  public:
    void Prepare(const wchar_t* Sql,ub4 PrefetchSize=256);
    

    void BindInt( int ColNumber,int* ValuePtr );
    void BindIntValue( int ColNumber,int Value );
    
    void BindLong( int ColNumber,long* ValuePtr );
    void BindLongValue( int ColNumber,long Value );
    
    void BindOciNumber( int ColNumber,OCINumber* ValuePtr );
    
    void BindDouble( int ColNumber,double* ValuePtr );
    void BindDoubleValue( int ColNumber,double Value );
    
    void BindString( int ColNumber,const wchar_t* ValuePtr );
    void BindStringValue( int ColNumber,const wchar_t* Value );
    
    void BindDate( int ColNumber,OCIDate* ValuePtr );
    void BindDateValue( int ColNumber,OCIDate Value );
    
    void BindSdoGeom( int ColNumber,c_SDO_GEOMETRY* ValuePtr ); // pointer will not be deleted, caller is responsible for pointer
    void BindSdoGeomValue( int ColNumber,c_SDO_GEOMETRY* Value ); // pointer will be deleted by statement
    
    void BindSdoDimElement( int ColNumber,c_SDO_DIM_ELEMENT* ValuePtr );
    
    void BindClob( int ColNumber,const char* ValuePtr,long Size );
    void BindBlob( int ColNumber,const char* ValuePtr,long Size );

    int ExecuteNonQuery(int ExecMode = OCI_COMMIT_ON_SUCCESS);  // OCI_COMMIT_ON_SUCCESS , OCI_DEFAULT
    
    
    void DefineColumn( int ColNumber,int DataType,const wchar_t*TypeName,int ColSize,int FetchSize );
    int ExecuteSelect(int FetchSize=1,int ExecMode = OCI_DEFAULT);
    
    int ExecuteSelectAndDefine(int FetchSize=1,int ExecMode = OCI_DEFAULT);
    
    int GetColumnsSize(); // returns number of columns in statement
    wchar_t* GetColumnName(int ColumnNumber);
    int GetColumnOciType(int ColumnNumber);
    wchar_t* GetColumnTypeName(int ColumnNumber);
    int GetColumnWidth(int ColumnNumber);
    int GetColumnPrecision(int ColumnNumber);
    int GetColumnScale(int ColumnNumber);
    
    bool ReadNext();
    
    bool IsColumnNull(int ColNumber);
    
    int GetInteger(int ColNumber);
    long GetLong( int ColNumber );
    double GetDouble(int ColNumber);
    OCIDate* GetOciDate(int ColNumber);
    const wchar_t* GetString( int ColNumber );
    long GetLongRawLength( int ColNumber );
    unsigned char* GetLongRaw( int ColNumber );
    
    //SDO_GEOMETRY_TYPE* GetSdoGeom( int ColNumber,SDO_GEOMETRY_ind** geom_ind=NULL );
    c_SDO_GEOMETRY* GetSdoGeom( int ColNumber );
    //OCIArray* GetSdoDimArray( int ColNumber );
    c_SDO_DIM_ARRAY GetSdoDimArray( int ColNumber );
protected:
  void ClearColumnData()
    {
      if( m_ColumnDataSize > 0 )
      {
        c_Oci_ColumnData* coldata;
        for(int ind=0;ind<m_ColumnDataSize;ind++)
        {
          coldata=m_ColumnDataPtrArray[ind];  
          delete coldata;
        }
        m_ColumnDataSize = 0;
      }
    }
    
    void DeleteColumnData()
    {
      if( m_ColumnDataAllocated > 0 )
      {
        ClearColumnData();
        delete []m_ColumnDataPtrArray;      
        m_ColumnDataAllocated = 0;
      }
    }
    void ReallocColumnData(int NewSize)
    {
      if( NewSize > m_ColumnDataAllocated )
      {
        c_Oci_ColumnData** newarray = new c_Oci_ColumnData*[NewSize];  
        
        c_Oci_ColumnData** newptr = newarray;
        c_Oci_ColumnData** colptr = m_ColumnDataPtrArray;
        for(int ind=0;ind<m_ColumnDataSize;ind++)
        {
          *newptr++ = *colptr++;
        }
        
        delete []m_ColumnDataPtrArray;
        
        m_ColumnDataPtrArray = newarray;
        m_ColumnDataAllocated = NewSize;
        
      }
    }
    void AddColumnData(c_Oci_ColumnData* Data)
    {
      if( m_ColumnDataSize >= m_ColumnDataAllocated )
      {
        ReallocColumnData(m_ColumnDataAllocated+32);                
      }
      
      m_ColumnDataPtrArray[m_ColumnDataSize] = Data;
      m_ColumnDataSize++;
    }
    void DeleteBindValues();
    











};


#endif
