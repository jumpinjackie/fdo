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


typedef struct t_OptimizedRect
{
  double m_MinX,m_MinY,m_MaxX,m_MaxY;
}t_OptimizedRect;

class c_KgOraSqlParamDesc
{
public:
  enum e_ParamType
  {
    e_Uknown,
    e_Geometry,
    e_DataValue,
    e_UserParam,
    e_OptimizedRect
  };
public:
  c_KgOraSqlParamDesc();
  c_KgOraSqlParamDesc(FdoString* UserName);
  c_KgOraSqlParamDesc(FdoByteArray* Geom);
  c_KgOraSqlParamDesc(FdoDataValue* DataValue) ;
  c_KgOraSqlParamDesc(long Srid,double MinX,double MinY,double MaxX,double MaxY);
  
  ~c_KgOraSqlParamDesc();

protected:
    e_ParamType m_ParamType;
    
    
    FdoStringP m_UserParamName;
    //c_SDO_GEOMETRY* m_ParamGeometry;
    FdoByteArray* m_ParamGeometry;
    FdoDataValue *m_ParamDataValue;
    t_OptimizedRect m_OptimizedRect;
    
public:   
  e_ParamType GetParamType() const;
  
  //c_SDO_GEOMETRY* GetGeometry() const;
  //void SetGeometry(c_SDO_GEOMETRY* Geom);
  FdoByteArray* GetGeometry() const;
  void SetGeometry(FdoByteArray* Geom);
  
  void SetDataValue(FdoDataValue* DataValue);
  
  FdoString* GetUserParamName() const;
  void SetUserParamName(FdoString* Name);
   
  void ApplySqlParameter(c_Oci_Statement* OraStm,bool IsGeodeticCS,long OraSrid,int SqlParamNum);
  void ApplySqlParameter(c_Oci_Statement* OraStm,bool IsGeodeticCS,long OraSrid,const wchar_t* SqlParamName);
protected:
  void SetNull();  
    
};

#endif
