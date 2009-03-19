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
#ifndef _c_Ora_API2_h
#define _c_Ora_API2_h

#include "c_OCI_API.h"
#define D_ORACLE_DEFAULT_MAIN_VER 10
#define D_ORACLE_DEFAULT_SUB_VER 2

#define D_SQL_STRING_SDO_GEOMETRY         L"MDSYS.SDO_GEOMETRY_TYPE"
#define D_SQL_STRING_SDO_DIM_ELEMENT      L"MDSYS.SDO_DIM_ELEMENT"
#define D_SQL_STRING_SDO_ELEM_INFO_ARRAY  L"MDSYS.SDO_ELEM_INFO_ARRAY"
#define D_SQL_STRING_SDO_POINT_TYPE       L"MDSYS.SDO_POINT_TYPE"
#define D_SQL_STRING_SDO_ORDINATE_ARRAY   L"MDSYS.SDO_ORDINATE_ARRAY"



class c_Ora_API2
{
public:
  c_Ora_API2(void);
public:
  ~c_Ora_API2(void);
  
  static char* SdoGeomToString(c_SDO_GEOMETRY *SdoGeom);
  static wchar_t* SdoGeomToStringW(c_SDO_GEOMETRY *SdoGeom);
  
  static c_SDO_GEOMETRY* CreateOptimizedRect(c_Oci_Connection*Con,bool IsGeodeticCS,long OraSrid,double MinX,double MinY,double MaxX,double MaxY);

  static long GetSrid(c_Oci_Connection* Conn,const wchar_t* CoordSysName);
  static long GetSequenceNextVal(c_Oci_Connection*Conn,const wchar_t* SequenceName);
  static bool ResetSequence(c_Oci_Connection*Conn,const wchar_t* SequenceName,const wchar_t* FullTableName,const wchar_t* ColumnName);
  static bool GetOracleVersion(c_Oci_Connection*Conn,int& MainVersion,int& SubVersion);
  
  static bool IsGeodeticCoordSystem(const wchar_t* CoordSysWkt);

  static bool GetCoordinateSystemWkt(c_Oci_Connection*Conn,long Srid,std::wstring& Wkt);
  static int GetTablePkeyColumns(c_Oci_Connection * OciConn,const wchar_t* Owner, const wchar_t* TableName,std::vector<std::wstring>& ColNames);
  
};

#endif