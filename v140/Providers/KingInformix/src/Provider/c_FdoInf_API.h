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

#define D_INF_SRID_NAME_PREFIX  L"InformixSpatialSrid"
#define D_INF_SRID_NAME_NULL    L"InformixSpatialSridNULL"
#define D_INF_SRID_NULL_VALUE -1


class c_FdoInf_API
{
public:
  c_FdoInf_API(void);
public:
  ~c_FdoInf_API(void);
  
public: 
  static bool OtlTypeToFdoDataType(int OtlType,int Scale,int Length,FdoDataType & FdoType);
  
  static bool SetStreamData(otl_stream* Stream,int SqlParamNum,FdoDataValue* DataValue);
  static bool SetStreamDataGeometryWkb(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb);
  static bool SetStreamDataGeometryWkt(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryWkb);
  static bool SetStreamDataGeometryFgf(otl_stream* Stream,int SqlParamNum,FdoByteArray* GeometryFgf);
  
  static bool IsGeodeticCoordSystem(const char* CoordSysWkt);
  static bool IsGeodeticCoordSystem(const wchar_t* CoordSysWkt);
  static bool GetCoordinateSystemWkt(c_KgInfConnection* KgOraConn,long Srid,string& Wkt);
  static long GetSridFromWkt(c_KgInfConnection* KgOraConn,const char* Wkt);
  
  static c_KgInfSchemaDesc* DescribeSchema(c_KgInfConnection* KgOraConn,const char* OraSchema,const char* KingFdoViews);
  
  static bool FdoPropertyToInfDataType(FdoPropertyDefinition* Property,FdoStringP& InfType);
  
  static void FdoGeomTypesToInfGeomType(FdoGeometricPropertyDefinition* Geom,FdoInt32 &Gtype);
  
  static bool GetTableOwner(c_KgInfConnection* Conn,FdoStringP& TableName,wstring& Owner);
  
private:
  static void CreateClassesfromTables(c_KgInfConnection* Conn,vector<c_KgInf_GeomteryColumns>& GeomTables
            ,FdoClassCollection* FdoClasses,c_KgInfOverClassCollection* PhysClasses
            ,c_KgInfSpatialContextCollection* SC_Collection,long& AliasNum);
  static bool DescribeTableProperties(c_KgInfConnection* Conn,const wstring& Schema,const wstring& TableName,FdoPropertyDefinitionCollection* PropCollection);
  static bool c_FdoInf_API::IdsTypeToFdoDataType(int DataType,int Length,int Prec,int Scale,FdoDataType& Fdotype);
  static bool GetTablePrimaryColumns(c_KgInfConnection* Conn,const wstring& Schema,const wstring& TableName,vector<wstring>&  PrimCol);
  

};
