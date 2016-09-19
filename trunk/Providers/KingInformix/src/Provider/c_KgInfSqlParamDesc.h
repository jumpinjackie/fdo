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

#ifndef _c_KgInfSqlParamDesc_h
#define _c_KgInfSqlParamDesc_h



class c_KgInfSqlParamDesc
{
public:
  enum e_ParamType
  {
    e_Uknown,
    e_Geometry,
    e_DataValue,
    e_UserParam
  };
public:
  c_KgInfSqlParamDesc();
  c_KgInfSqlParamDesc(FdoString* UserName);
  c_KgInfSqlParamDesc(FdoByteArray* Geom);
  c_KgInfSqlParamDesc(FdoDataValue* DataValue) ;
  
  ~c_KgInfSqlParamDesc();

protected:
    e_ParamType m_ParamType;
    
    
    FdoStringP m_UserParamName;
    //SDO_GEOMETRY* m_ParamGeometry;
    
    FdoPtr<FdoByteArray> m_ParamGeometryWkb;
    
    FdoDataValue *m_ParamDataValue;
    
public:   
  e_ParamType GetParamType() const;
  //SDO_GEOMETRY* GetGeometry() const;
  
  FdoByteArray* GetGeometryWkb() const;
  void SetGeometryWkb(FdoByteArray* Geom);
  
  void SetDataValue(FdoDataValue* DataValue);
  
  FdoString* GetUserParamName() const;
  void SetUserParamName(FdoString* Name);
   
  void ApplySqlParameter(otl_stream* OtlStream,int SqlParamNum);
  
protected:
  void SetNull();  
    
};

#endif