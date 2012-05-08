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

#include "occi.h"
#include <time.h>

#include "c_KgOraDataReader.h"
#include "c_FdoOra_API2.h"
#include "c_SdoGeomToAGF2.h"
#include "c_LogAPI.h"


    
    
//---------------------------------------------------------------------
//
//    c_KgOraDataReader
//
//---------------------------------------------------------------------

c_KgOraDataReader::c_KgOraDataReader(c_KgOraConnection * Connection
                                    ,c_Oci_Statement* OcciStatement  
                                    ,FdoClassDefinition* ClassDef
                                    ,int GeomPropSqlIndex
                                    ,FdoStringCollection* SqlColumns
                                    ,FdoIdentifierCollection* Props)
 : superclass(Connection,OcciStatement ,GeomPropSqlIndex, SqlColumns)
{

  m_ClassDef = ClassDef;
  FDO_SAFE_ADDREF (m_ClassDef.p);



  m_Props = Props;
  if (m_Props) m_Props->AddRef();

  
}

c_KgOraDataReader::~c_KgOraDataReader()
{
    //Close();
    FDO_SAFE_RELEASE(m_Props);
    
}

void c_KgOraDataReader::Dispose()
{
    delete this;
}

FdoInt32 c_KgOraDataReader::GetPropertyCount()
{
  return m_OciStatement->GetColumnsSize(); //m_MetaData.size();
}

FdoDataType c_KgOraDataReader::GetDataType( FdoString* PropertyName )
{
  int ind = PropNameToColumnNumber(PropertyName );
  
    
  int dtype = m_OciStatement->GetColumnOciType(ind); //m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
  int length = m_OciStatement->GetColumnWidth(ind); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
  int orascale = m_OciStatement->GetColumnScale(ind); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);
  int oraprecision = m_OciStatement->GetColumnPrecision(ind);

  FdoDataType fdotype;
  c_FdoOra_API2::OraTypeToFdoDataType(dtype,oraprecision,orascale,length,fdotype);
  
  return fdotype;
}

 FdoPropertyType c_KgOraDataReader::GetPropertyType( FdoString* PropertyName )
{
  int ind = PropNameToColumnNumber(PropertyName);
  
  wchar_t* oratype = m_OciStatement->GetColumnTypeName(ind);
  //string oratype = m_MetaData[ind].getString(oracle::occi::MetaData::ATTR_TYPE_NAME);
  
  if( oratype && FdoCommonOSUtil::wcsicmp(oratype,L"SDO_GEOMETRY") == 0 )
  {
    return FdoPropertyType_GeometricProperty;
  }
  
  return FdoPropertyType_DataProperty;
}


 //---------------------------------------------------------------------
 //
 //    c_KgOraSdeDataReader
 //
 //---------------------------------------------------------------------

 c_KgOraSdeDataReader::c_KgOraSdeDataReader(c_KgOraConnection * Connection
   ,c_Oci_Statement* OcciStatement  
   ,FdoClassDefinition* ClassDef
   ,c_KgOraSridDesc& SridDesc
   ,int SdeGeometryType
   ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
   ,FdoIdentifierCollection* Props,FdoString *SdeSpatialExtent_ColumnName)
   : c_KgOraDataReader(Connection,OcciStatement ,ClassDef,GeomPropSqlIndex, SqlColumns,Props)
 {


   m_SridDesc=SridDesc;
   m_SdeGeometryType = SdeGeometryType;
   
   m_SdeSpatialExtent_ColumnName = SdeSpatialExtent_ColumnName;
 }

 c_KgOraSdeDataReader::~c_KgOraSdeDataReader()
 {
   //Close();


 }

 void c_KgOraSdeDataReader::Dispose()
 {
   delete this;
 }

 FdoByteArray* c_KgOraSdeDataReader::GetGeometry(FdoString* propertyName)
 {
   int len = 0;
   const void* ptr = GetGeometry(propertyName, &len);

   if( len > 0 )
     return FdoByteArray::Create((const FdoByte*)ptr, len);
   else
     throw FdoException::Create(L"c_KgOraReader::GetGeometry Invalid Geometry !");
 }




const FdoByte* c_KgOraSdeDataReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{

  if( m_OciStatement )
  {

    try
    {
      if( m_SdeSpatialExtent_ColumnName.GetLength()>0 && m_SdeSpatialExtent_ColumnName.ICompare(propertyName) == 0 )
      {
        int oraind;
        oraind = PropNameToColumnNumber(L"sdo_fdo_eminx"); 
        double minx = m_OciStatement->GetDouble(oraind);
        double miny = m_OciStatement->GetDouble(oraind+1);
        double maxx = m_OciStatement->GetDouble(oraind+2);
        double maxy = m_OciStatement->GetDouble(oraind+3);
        
        *len = m_SdeAgfConv.ToAGF(minx,miny,maxx,maxy);
        return (const unsigned char*)m_SdeAgfConv.GetBuff();
      }
      
      int oraind = PropNameToColumnNumber(propertyName); 
      if( !m_OciStatement->IsColumnNull(oraind) )
      {
        
        //int oraind = PropNameToColumnNumber(propertyName); 
        int sde_numofpts = m_OciStatement->GetInteger(oraind+1);
        int sde_entity = m_OciStatement->GetInteger(oraind+2);


        long length = m_OciStatement->GetLongRawLength(oraind); // oracle is 1 based - our index is 0 based        
        unsigned char* sdata = m_OciStatement->GetLongRaw(oraind);
        m_SdeAgfConv.SetSdeGeometry(&m_SridDesc,m_SdeGeometryType,sde_numofpts,sde_entity,length,sdata);
        *len = m_SdeAgfConv.ToAGF();
        return (const unsigned char*)m_SdeAgfConv.GetBuff();
      }
    }
    catch(FdoException* ea)
    {      
      throw ea;
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
  }

 return NULL;
}

FDOKGORA_API  bool c_KgOraSdeDataReader::IsNull( FdoString* propertyName )
{
  if( m_SdeSpatialExtent_ColumnName.GetLength()>0 && m_SdeSpatialExtent_ColumnName.ICompare(propertyName) == 0 )
  {
    int oraind;
    oraind = PropNameToColumnNumber(L"sdo_fdo_eminx"); 
    
    return m_OciStatement->IsColumnNull(oraind) || m_OciStatement->IsColumnNull(oraind+1) || m_OciStatement->IsColumnNull(oraind+2) || m_OciStatement->IsColumnNull(oraind+3);         
  }
  
  return c_KgOraDataReader::IsNull(propertyName);
  

}