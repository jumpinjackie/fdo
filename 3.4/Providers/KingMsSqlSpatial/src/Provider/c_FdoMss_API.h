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
#pragma once

#define D_MSS_SRID_NAME_PREFIX  L"MsSqlSpatialSrid"
#define D_MSS_SRID_NAME_NULL    L"MsSqlSpatialSridNULL"
#define D_MSS_SRID_NULL_VALUE -1


class c_FdoMss_API
{
public:
  c_FdoMss_API(void);
public:
  ~c_FdoMss_API(void);
  
public: 
  static bool OtlTypeToFdoDataType(int OtlType,int Scale,int Length,FdoDataType & FdoType);
  
  static bool SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue);
  static bool SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb);
  static bool SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf);
  
  static bool IsGeodeticCoordSystem(const char* CoordSysWkt);
  static bool IsGeodeticCoordSystem(const wchar_t* CoordSysWkt);
  static bool GetCoordinateSystemWkt(c_KgMssConnection* KgOraConn,long Srid,string& Wkt);
  static long GetSridFromWkt(c_KgMssConnection* KgOraConn,const char* Wkt);
  
  static c_KgMssSchemaDesc* DescribeSchema(c_KgMssConnection* KgOraConn,const char* OraSchema,const char* KingFdoViews);
  
  static bool FdoPropertyToMssDataType(FdoPropertyDefinition* Property,FdoStringP& MssType);
  
  static void FdoGeomTypesToMssGeomType(FdoGeometricPropertyDefinition* Geom,FdoStringP& MssType);
  
private:
  static void CreateClassesfromTables(c_KgMssConnection* Conn,vector<c_KgMss_GeomteryColumns>& GeomTables
            ,FdoClassCollection* FdoClasses,c_KgMssOverClassCollection* PhysClasses
            ,c_KgMssSpatialContextCollection* SC_Collection,long& AliasNum);
  static bool DescribeTableProperties(c_KgMssConnection* Conn,const wstring& Schema,const wstring& TableName,FdoPropertyDefinitionCollection* PropCollection);
  static bool c_FdoMss_API::MsSqlTypeToFdoDataType(const wchar_t * DataType,int Length,int Prec,int Scale,FdoDataType& Fdotype);
  static bool GetTableIdentityColumn(c_KgMssConnection* Conn,const wstring& Schema,const wstring& TableName,wstring& IdentCol);
              
};
