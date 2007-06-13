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

#ifndef _c_KgOraSqlParamDesc_h
#define _c_KgOraSqlParamDesc_h



class c_KgOraSqlParamDesc
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
  c_KgOraSqlParamDesc();
  c_KgOraSqlParamDesc(FdoString* UserName);
  c_KgOraSqlParamDesc(SDO_GEOMETRY* Geom);
  c_KgOraSqlParamDesc(FdoDataValue* DataValue) ;
  
  ~c_KgOraSqlParamDesc();

protected:
    e_ParamType m_ParamType;
    
    
    FdoStringP m_UserParamName;
    SDO_GEOMETRY* m_ParamGeometry;
    FdoDataValue *m_ParamDataValue;
    
public:   
  e_ParamType GetParamType() const;
  SDO_GEOMETRY* GetGeometry() const;
  void SetGeometry(SDO_GEOMETRY* Geom);
  void SetDataValue(FdoDataValue* DataValue);
  
  FdoString* GetUserParamName() const;
  void SetUserParamName(FdoString* Name);
   
  void ApplySqlParameter(oracle::occi::Environment*Env,oracle::occi::Statement* OraStm,int SqlParamNum);
  
protected:
  void SetNull();  
    
};

#endif