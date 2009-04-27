/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
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
 */

#ifndef SHPPROVIDER_H
#define SHPPROVIDER_H

// General descriptive items of Shp Provier:
#define SHP_PROVIDER_NAME					L"OSGeo.SHP.3.5"
#define SHP_PROVIDER_DEFAULT_DISPLAY_NAME	"OSGeo FDO Provider for SHP"
#define SHP_PROVIDER_DEFAULT_DESCRIPTION	"Read/write access to spatial and attribute data in an ESRI SHP file."
#define SHP_PROVIDER_VERSION				L"3.5.0.0"
#define SHP_FDO_VERSION						L"3.5.0.0"

// Shp Provier identifier names
#define SHP_PROVIDER_AUTHOR L"OSGeo"
#define SHP_PROVIDER_SHORTNAME L"SHP"

// Connection property strings:
const wchar_t CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION[] = L"DefaultFileLocation";
const wchar_t CONNECTIONPROPERTY_TEMPORARY_FILE_LOCATION[] = L"TemporaryFileLocation";

// File extensions:
const wchar_t SHP_EXTENSION[5] = L".shp";
const wchar_t DBF_EXTENSION[5] = L".dbf";
const wchar_t SHX_EXTENSION[5] = L".shx";
const wchar_t IDX_EXTENSION[5] = L".idx";
const wchar_t PRJ_EXTENSION[5] = L".prj";
const wchar_t CPG_EXTENSION[5] = L".cpg";

const wchar_t DEFAULT_SCHEMA_XML[11] = L"schema.xml";

const wchar_t FEATID_PROPERTY[7] = L"FeatId";
const wchar_t GEOMETRY_PROPERTY[9] = L"Geometry";

// Spatial context constants:
const wchar_t SPATIALCONTEXT_DEFAULT_NAME[18] = L"Default";
const wchar_t SPATIALCONTEXT_CSYS_DEFAULT_NAME[4] = L"";
const wchar_t SPATIALCONTEXT_CSYS_DEFAULT_WKT[4] = L"";

const double SPATIALCONTEXT_DEFAULT_XY_TOLERANCE = 0.001;
const double SPATIALCONTEXT_DEFAULT_XY_TOLERANCE_LL = 0.0000001; 
const double  SPATIALCONTEXT_DEFAULT_Z_TOLERANCE = 0.001;
const double  SPATIALCONTEXT_DEFAULT_MINX = -10000000;
const double  SPATIALCONTEXT_DEFAULT_MINY = -10000000;
const double  SPATIALCONTEXT_DEFAULT_MAXX = 10000000;
const double  SPATIALCONTEXT_DEFAULT_MAXY = 10000000;

#endif // SHPPROVIDER_H
