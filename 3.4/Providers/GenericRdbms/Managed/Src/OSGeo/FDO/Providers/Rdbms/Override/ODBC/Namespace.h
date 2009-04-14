/*
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

#pragma once

//---------------------------------------------------------------
// Namespace declaration
//---------------------------------------------------------------
// Rdbms
#define BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS namespace Rdbms {
#define END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS }}}}

// RdbmsOverrides
#define BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS namespace Override {
#define END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE }}}}}

// RdbmsODBCOverrides
#define BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE namespace ODBC{
#define END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC }}}}}}

//-------------------------------------------------------------------
// Namespace abbrevative
//-------------------------------------------------------------------

// Rdbms
#define NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS NAMESPACE_OSGEO_FDO_PROVIDERS::Rdbms
#define NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::Override

// RdbmsODBCOverrides
#define NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ODBC