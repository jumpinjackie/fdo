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
#ifndef _c_Ora_API_h
#define _c_Ora_API_h

#define D_ORACLE_DEFAULT_MAIN_VER 10
#define D_ORACLE_DEFAULT_SUB_VER 2

#define D_SQL_STRING_SDO_GEOMETRY         "MDSYS.SDO_GEOMETRY"
#define D_SQL_STRING_SDO_DIM_ELEMENT      "MDSYS.SDO_DIM_ELEMENT"
#define D_SQL_STRING_SDO_ELEM_INFO_ARRAY  "MDSYS.SDO_ELEM_INFO_ARRAY"
#define D_SQL_STRING_SDO_POINT_TYPE       "MDSYS.SDO_POINT_TYPE"
#define D_SQL_STRING_SDO_ORDINATE_ARRAY   "MDSYS.SDO_ORDINATE_ARRAY"



class c_Ora_API
{
public:
  c_Ora_API(void);
public:
  ~c_Ora_API(void);
  
  static char* SdoGeomToString(SDO_GEOMETRY *SdoGeom);
  
  static SDO_GEOMETRY* CreateOptimizedRect(long Srid,double MinX,double MinY,double MaxX,double MaxY);
  
  static long GetSrid(oracle::occi::Connection*Conn,const char* CoordSysName);
  static long GetSequenceNextVal(oracle::occi::Connection*Conn,const char* SequenceName);  
  static bool ResetSequence(oracle::occi::Connection*Conn,const char* SequenceName,const char* FullTableName,const char* ColumnName);
  static bool GetOracleVersion(oracle::occi::Connection*Conn,int& MainVersion,int& SubVersion);
  
  static bool IsGeodeticCoordSystem(const char* CoordSysWkt);
  
  static bool GetCoordinateSystemWkt(oracle::occi::Connection*Conn,long Srid,string& Wkt);
};

#endif
