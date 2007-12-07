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
#include "c_KgOraSqlParamDesc.h"
#include "c_FilterStringBuffer.h"
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API.h"
#include "c_FdoOra_API.h"
#include "c_KgOraSqlParamDesc.h"

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc() 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
}
c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoString* UserName) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  m_ParamType=e_UserParam;m_UserParamName=UserName; 
}

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(SDO_GEOMETRY* Geom) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_Geometry;
  m_ParamGeometry=Geom; 
}

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoDataValue* DataValue) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_DataValue;
  m_ParamDataValue=DataValue; 
  
  m_ParamDataValue->AddRef();
}

c_KgOraSqlParamDesc::~c_KgOraSqlParamDesc() 
{ 
  SetNull();
}

void c_KgOraSqlParamDesc::SetNull()
{
  if( (m_ParamType==e_Geometry) && (m_ParamGeometry) ) delete m_ParamGeometry; 
  
  if( (m_ParamType==e_DataValue) && m_ParamDataValue ) m_ParamDataValue->Release();
  
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;  
}
c_KgOraSqlParamDesc::e_ParamType c_KgOraSqlParamDesc::GetParamType() const 
{ 
  return m_ParamType; 
}
SDO_GEOMETRY* c_KgOraSqlParamDesc::GetGeometry() const 
{ 
  return m_ParamGeometry; 
}
void c_KgOraSqlParamDesc::SetGeometry(SDO_GEOMETRY* Geom) 
{ 
  SetNull();
  
  m_ParamType=e_Geometry;
  m_ParamGeometry =  Geom; 
}
  
void c_KgOraSqlParamDesc::SetDataValue(FdoDataValue* DataValue) 
{ 
  SetNull();
  
  m_ParamType=c_KgOraSqlParamDesc::e_DataValue;
  m_ParamDataValue =  DataValue; 
}
  
FdoString* c_KgOraSqlParamDesc::GetUserParamName() const 
{ 
  return m_UserParamName; 
}
void c_KgOraSqlParamDesc::SetUserParamName(FdoString* Name) 
{ 
  SetNull();
  
  m_ParamType=c_KgOraSqlParamDesc::e_UserParam;
  m_UserParamName =  Name; 
}

void c_KgOraSqlParamDesc::ApplySqlParameter(oracle::occi::Environment*Env,oracle::occi::Statement* OraStm,int SqlParamNum) 
{ 
  switch( m_ParamType )
  {
    case e_Geometry:
    {
      if( m_ParamGeometry )
      {
        OraStm->setObject(SqlParamNum,m_ParamGeometry);
        
        #ifdef _DEBUG
        {
        char* buff = c_Ora_API::SdoGeomToString(m_ParamGeometry);
        D_KGORA_ELOG_WRITE2("SQL Geometry Param %d = '%s'",SqlParamNum,buff);
        delete buff;
        }
        #endif
        
      }
      else
      {
        OraStm->setNull( SqlParamNum,oracle::occi::OCCIPOBJECT );
        #ifdef _DEBUG
        {
        D_KGORA_ELOG_WRITE1("SQL Geometry Param %d = NULL",SqlParamNum);
        
        }
        #endif
      }
    }
    break;
    case e_DataValue:
    {
      oracle::occi::Type oratype;
      
      if( c_FdoOra_API::SetOracleStatementData(Env,OraStm,SqlParamNum,m_ParamDataValue) )
      {
      }
      
      #ifdef _DEBUG
      {
      FdoStringP fdostr = m_ParamDataValue->ToString();
      D_KGORA_ELOG_WRITE2("SQL Data Param %d = %s",SqlParamNum,(const char*)fdostr);
      
      }
      #endif
    }
    break;  
  }
}//end of  c_KgOraSqlParamDesc::ApplySqlParameters

