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

#include "StdAfx.h"
#include "c_KgMssSqlParamDesc.h"
#include "c_FilterStringBuffer.h"
#include "c_KgMssSqlParamDesc.h"

c_KgMssSqlParamDesc::c_KgMssSqlParamDesc() 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
}
c_KgMssSqlParamDesc::c_KgMssSqlParamDesc(FdoString* UserName) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  m_ParamType=e_UserParam;m_UserParamName=UserName; 
}

c_KgMssSqlParamDesc::c_KgMssSqlParamDesc(FdoByteArray* Geom) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_Geometry;
  m_ParamGeometryWkb=Geom; 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
}

c_KgMssSqlParamDesc::c_KgMssSqlParamDesc(FdoDataValue* DataValue) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_DataValue;
  m_ParamDataValue=DataValue; 
  
  m_ParamDataValue->AddRef();
}

c_KgMssSqlParamDesc::~c_KgMssSqlParamDesc() 
{ 
  SetNull();
}

void c_KgMssSqlParamDesc::SetNull()
{
  
  
  if( (m_ParamType==e_DataValue) && m_ParamDataValue ) m_ParamDataValue->Release();
  
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;  
}
c_KgMssSqlParamDesc::e_ParamType c_KgMssSqlParamDesc::GetParamType() const 
{ 
  return m_ParamType; 
}
FdoByteArray* c_KgMssSqlParamDesc::GetGeometryWkb() const 
{ 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
  return m_ParamGeometryWkb; 
}
void c_KgMssSqlParamDesc::SetGeometryWkb(FdoByteArray* Geom) 
{ 
  SetNull();
  
  m_ParamType=e_Geometry;
  m_ParamGeometryWkb =  Geom; 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
}
  
void c_KgMssSqlParamDesc::SetDataValue(FdoDataValue* DataValue) 
{ 
  SetNull();
  
  m_ParamType=c_KgMssSqlParamDesc::e_DataValue;
  m_ParamDataValue =  DataValue; 
}
  
FdoString* c_KgMssSqlParamDesc::GetUserParamName() const 
{ 
  return m_UserParamName; 
}
void c_KgMssSqlParamDesc::SetUserParamName(FdoString* Name) 
{ 
  SetNull();
  
  m_ParamType=c_KgMssSqlParamDesc::e_UserParam;
  m_UserParamName =  Name; 
}

void c_KgMssSqlParamDesc::ApplySqlParameter(otl_stream* Stream,int SqlParamNum) 
{ 
  switch( m_ParamType )
  {
    case e_Geometry:
    {
      c_FdoMss_API::SetStreamDataGeometryWkb(Stream,SqlParamNum,m_ParamGeometryWkb);
    }
    break;
    case e_DataValue:
    {
      //oracle::occi::Type oratype;
      if( c_FdoMss_API::SetStreamData(Stream,SqlParamNum,m_ParamDataValue) )
      {
      }
    }
    break;  
  }
}//end of  c_KgMssSqlParamDesc::ApplySqlParameters
