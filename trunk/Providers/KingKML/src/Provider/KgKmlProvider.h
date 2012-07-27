/*
* 
* Copyright (C) 2006  SL-King
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
* 
*
*/

#ifndef KGORAPROVIDER_H
#define KGORAPROVIDER_H


#define D_KGORA_PROVIDER_NAME					L"King.KML.0.1.1"
#define D_KGORA_PROVIDER_VERSION				L"0.1.1.0"
#define D_KGORA_FDO_VERSION						L"3.1.0.0"

#define D_KGORA_PROVIDER_DEFAULT_DISPLAY_NAME	L"FDO Provider for KML"
#define D_KGORA_PROVIDER_DEFAULT_DESCRIPTION	L"Read/write access to KML."


//#define D_CONN_PROPERTY_USERNAME  L"UserName"
#define D_CONN_PROPERTY_KML_FILE  L"File"

//#define D_CONN_PROPERTY_PASSWORD  L"Password"
//#define D_CONN_PROPERTY_SERVICE_NAME  L"ServiceName"
//#define D_CONN_PROPERTY_SERVICE_NAME  L"Service"
//#define D_CONN_PROPERTY_ORACLE_SCHEMA L"OracleSchema"
//#define D_CONN_PROPERTY_KING_FDO_CLASS L"KingFdoClass"



const wchar_t DEFAULT_SCHEMA_XML[11] = L"schema.xml";
const wchar_t DEFAULT_SCHEMA_NAME[14] = L"Default";
const wchar_t DEFAULT_SCHEMA_DESCRIPTION[16] = L"Default schema.";


#define D_KML_FDO_DEFAULT_SCHEMA L"KingKmlFdo"
#define D_KML_FDO_DEFAULT_CLASS L"KMLFDOCLASS"

#define D_KML_FDO_PROPERTY_ID L"KML_FDO_ID"
#define D_KML_FDO_PROPERTY_NAME L"Name"
#define D_KML_FDO_PROPERTY_DESCRIPTION L"Description"



// Spatial context constants:
const wchar_t D_SPATIALCONTEXT_DEFAULT_NAME[32] = L"Default";
const wchar_t D_SPATIALCONTEXT_CSYS_DEFAULT_NAME[128] = L"EPSG:4326";
const wchar_t D_SPATIALCONTEXT_CSYS_DEFAULT_WKT[512] = L"GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]]";

const double D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_M_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_MINX = -180;
const double D_SPATIALCONTEXT_DEFAULT_MINY = -90;
const double D_SPATIALCONTEXT_DEFAULT_MAXX = 180;
const double D_SPATIALCONTEXT_DEFAULT_MAXY = 90;

const double D_SPATIALCONTEXT_DEFAULT_MINZ = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXZ =  10000000;

const double D_SPATIALCONTEXT_DEFAULT_MINM = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXM = 10000000;

#endif 
