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
#include "c_KgInfSqlParamDesc.h"
#include "c_FilterStringBuffer.h"
#include "c_KgInfSqlParamDesc.h"

c_KgInfSqlParamDesc::c_KgInfSqlParamDesc() 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
}
c_KgInfSqlParamDesc::c_KgInfSqlParamDesc(FdoString* UserName) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  m_ParamType=e_UserParam;m_UserParamName=UserName; 
}

c_KgInfSqlParamDesc::c_KgInfSqlParamDesc(FdoByteArray* Geom) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_Geometry;
  m_ParamGeometryWkb=Geom; 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
}

c_KgInfSqlParamDesc::c_KgInfSqlParamDesc(FdoDataValue* DataValue) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_DataValue;
  m_ParamDataValue=DataValue; 
  
  m_ParamDataValue->AddRef();
}

c_KgInfSqlParamDesc::~c_KgInfSqlParamDesc() 
{ 
  SetNull();
}

void c_KgInfSqlParamDesc::SetNull()
{
  
  
  if( (m_ParamType==e_DataValue) && m_ParamDataValue ) m_ParamDataValue->Release();
  
  m_ParamType=e_Uknown;
  m_ParamGeometryWkb=NULL; 
  m_ParamDataValue=NULL;  
}
c_KgInfSqlParamDesc::e_ParamType c_KgInfSqlParamDesc::GetParamType() const 
{ 
  return m_ParamType; 
}
FdoByteArray* c_KgInfSqlParamDesc::GetGeometryWkb() const 
{ 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
  return m_ParamGeometryWkb; 
}
void c_KgInfSqlParamDesc::SetGeometryWkb(FdoByteArray* Geom) 
{ 
  SetNull();
  
  m_ParamType=e_Geometry;
  m_ParamGeometryWkb =  Geom; 
  FDO_SAFE_ADDREF(m_ParamGeometryWkb.p);
}
  
void c_KgInfSqlParamDesc::SetDataValue(FdoDataValue* DataValue) 
{ 
  SetNull();
  
  m_ParamType=c_KgInfSqlParamDesc::e_DataValue;
  m_ParamDataValue =  DataValue; 
}
  
FdoString* c_KgInfSqlParamDesc::GetUserParamName() const 
{ 
  return m_UserParamName; 
}
void c_KgInfSqlParamDesc::SetUserParamName(FdoString* Name) 
{ 
  SetNull();
  
  m_ParamType=c_KgInfSqlParamDesc::e_UserParam;
  m_UserParamName =  Name; 
}

void c_KgInfSqlParamDesc::ApplySqlParameter(otl_stream* Stream,int SqlParamNum) 
{ 
  switch( m_ParamType )
  {
    case e_Geometry:
    {
      //simon
      //c_FdoInf_API::SetStreamDataGeometryWkt(Stream,SqlParamNum,m_ParamGeometryWkb);
      c_FdoInf_API::SetStreamDataGeometryWkb(Stream,SqlParamNum,m_ParamGeometryWkb);
    }
    break;
    case e_DataValue:
    {
      
      if( c_FdoInf_API::SetStreamData(Stream,SqlParamNum,m_ParamDataValue) )
      {
      }
    }
    break;
    //simon
    case e_UserParam:
    {
      
      if( c_FdoInf_API::SetStreamData(Stream,SqlParamNum,m_ParamDataValue) )
      {
      }
    }
    break;  
  }
}//end of  c_KgInfSqlParamDesc::ApplySqlParameters
