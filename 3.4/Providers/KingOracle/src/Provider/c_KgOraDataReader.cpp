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
 : c_KgOraReader<FdoIDataReader>(Connection,OcciStatement ,GeomPropSqlIndex, SqlColumns)
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

FdoString* c_KgOraDataReader::GetPropertyName( FdoInt32 Index )
{
  return m_OciStatement->GetColumnName(Index+1);
}

FdoDataType c_KgOraDataReader::GetDataType( FdoString* PropertyName )
{
  int ind = PropNameToColumnNumber(PropertyName );
  
    
  int dtype = m_OciStatement->GetColumnOciType(ind); //m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_TYPE);
  int length = m_OciStatement->GetColumnWidth(ind); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_DATA_SIZE);
  int orascale = m_OciStatement->GetColumnScale(ind); // m_MetaData[ind].getInt(oracle::occi::MetaData::ATTR_SCALE);

  FdoDataType fdotype;
  c_FdoOra_API2::OraTypeToFdoDataType(dtype,orascale,length,fdotype);
  
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


