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
#include "c_KgOraSqlParamDesc.h"
#include "c_FilterStringBuffer.h"
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API2.h"
#include "c_FdoOra_API2.h"
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

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoByteArray* Geom) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_Geometry;
  m_ParamGeometry=Geom; 
  
  Geom->AddRef();
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
c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(long Srid,double MinX,double MinY,double MaxX,double MaxY) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_OptimizedRect;
  m_OptimizedRect.m_MinX=MinX;
  m_OptimizedRect.m_MinY=MinY;
  m_OptimizedRect.m_MaxX=MaxX;
  m_OptimizedRect.m_MaxY=MaxY;
  
  
  
}

c_KgOraSqlParamDesc::~c_KgOraSqlParamDesc() 
{ 
  SetNull();
}

void c_KgOraSqlParamDesc::SetNull()
{
  //if( (m_ParamType==e_Geometry) && (m_ParamGeometry) ) delete m_ParamGeometry; 
  if( (m_ParamType==e_Geometry) && (m_ParamGeometry) ) m_ParamGeometry->Release(); 
  
  if( (m_ParamType==e_DataValue) && m_ParamDataValue ) m_ParamDataValue->Release();
  
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;  
}
c_KgOraSqlParamDesc::e_ParamType c_KgOraSqlParamDesc::GetParamType() const 
{ 
  return m_ParamType; 
}


FdoByteArray* c_KgOraSqlParamDesc::GetGeometry() const 
{ 
  return m_ParamGeometry; 
}
void c_KgOraSqlParamDesc::SetGeometry(FdoByteArray* Geom) 
{ 
  SetNull();
  
  m_ParamType=e_Geometry;
  m_ParamGeometry =  Geom; 
  
  if(m_ParamGeometry) m_ParamGeometry->AddRef();
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

void c_KgOraSqlParamDesc::ApplySqlParameter(c_Oci_Statement* OraStm,bool IsGeodeticCS,long OraSrid,int SqlParamNum) 
{ 
  switch( m_ParamType )
  {
    case e_Geometry:
    {
      if( m_ParamGeometry )
      {
          
        c_SDO_GEOMETRY * sdogeom = c_SDO_GEOMETRY::Create(OraStm->m_OciConn);
        
        c_FgfToSdoGeom fgftosdo;
        
        if( fgftosdo.ToSdoGeom((int*)m_ParamGeometry->GetData(),OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
        {
          OraStm->BindSdoGeomValue(SqlParamNum,sdogeom);  
        }
        else
        {
          delete sdogeom;
          OraStm->BindSdoGeom(SqlParamNum,NULL);  
        }
      
        #ifdef _KGORA_EXTENDED_LOG
        {
          char* buff = c_Ora_API2::SdoGeomToString(sdogeom);
          D_KGORA_ELOG_WRITE2("SQL Param %d Geometry='%s'",SqlParamNum,buff);
          delete [] buff;
        }
        #endif
        
      }
      else
      {
        OraStm->BindSdoGeom( SqlParamNum,NULL );
        #ifdef _KGORA_EXTENDED_LOG
        {
        D_KGORA_ELOG_WRITE1("SQL Param %d Geometry=NULL",SqlParamNum);
        
        }
        #endif
      }
    }
    break;
    case e_OptimizedRect:
    {
      c_SDO_GEOMETRY *sdorect = c_Ora_API2::CreateOptimizedRect(OraStm->m_OciConn,IsGeodeticCS,OraSrid,m_OptimizedRect.m_MinX,m_OptimizedRect.m_MinY,m_OptimizedRect.m_MaxX,m_OptimizedRect.m_MaxY);
      OraStm->BindSdoGeomValue(SqlParamNum,sdorect);   
      #ifdef _KGORA_EXTENDED_LOG
        {
          char* buff = c_Ora_API2::SdoGeomToString(sdorect);
          D_KGORA_ELOG_WRITE2("SQL Param %d  OptimizedRect='%s'",SqlParamNum,buff);
          delete [] buff;
        }
        #endif     
    }
    break;
    
    case e_DataValue:
    {
      
      
      if( c_FdoOra_API2::SetOracleStatementData(OraStm,SqlParamNum,m_ParamDataValue) )
      {
      }
      
      #ifdef _KGORA_EXTENDED_LOG
      {
      FdoStringP fdostr = m_ParamDataValue->ToString();
      D_KGORA_ELOG_WRITE2("SQL Param %d Data='%s'",SqlParamNum,(const char*)fdostr);
      
      }
      #endif
    }
    break;  
  }
}//end of  c_KgOraSqlParamDesc::ApplySqlParameters

