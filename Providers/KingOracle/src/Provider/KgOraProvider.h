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


#define D_KGORA_PROVIDER_NAME					L"King.Oracle.0.1.1"
#define D_KGORA_PROVIDER_VERSION				L"0.1.1.0"
#define D_KGORA_FDO_VERSION						L"3.1.0.0"

#define D_KGORA_PROVIDER_DEFAULT_DISPLAY_NAME	"FDO Provider for Oracle"
#define D_KGORA_PROVIDER_DEFAULT_DESCRIPTION	"Read/write access to spatial and attribute data in an Oracle Spatial."


//#define D_CONN_PROPERTY_USERNAME  L"UserName"
#define D_CONN_PROPERTY_USERNAME  L"Username"
#define D_CONN_PROPERTY_PASSWORD  L"Password"
//#define D_CONN_PROPERTY_SERVICE_NAME  L"ServiceName"
#define D_CONN_PROPERTY_SERVICE_NAME  L"Service"
#define D_CONN_PROPERTY_ORACLE_SCHEMA L"OracleSchema"
#define D_CONN_PROPERTY_KING_FDO_CLASS L"KingFdoClass"



const wchar_t DEFAULT_SCHEMA_XML[11] = L"schema.xml";
const wchar_t DEFAULT_SCHEMA_NAME[14] = L"Default";
const wchar_t DEFAULT_SCHEMA_DESCRIPTION[16] = L"Default schema.";


// Spatial context constants:
const wchar_t D_SPATIALCONTEXT_DEFAULT_NAME[18] = L"Default";
const wchar_t D_SPATIALCONTEXT_CSYS_DEFAULT_NAME[4] = L"";
const wchar_t D_SPATIALCONTEXT_CSYS_DEFAULT_WKT[4] = L"";

const double D_SPATIALCONTEXT_DEFAULT_XY_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_Z_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_M_TOLERANCE = 0.001;
const double D_SPATIALCONTEXT_DEFAULT_MINX = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MINY = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXX = 10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXY = 10000000;

const double D_SPATIALCONTEXT_DEFAULT_MINZ = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXZ =  10000000;

const double D_SPATIALCONTEXT_DEFAULT_MINM = -10000000;
const double D_SPATIALCONTEXT_DEFAULT_MAXM = 10000000;

#endif 
