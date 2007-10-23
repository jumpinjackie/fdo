//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef POSTGISPROVIDER_H_INCLUDED
#define POSTGISPROVIDER_H_INCLUDED

#include <PostGIS/FdoPostGisProvider.h>

// FDO Core API
#include <Fdo.h>
#include <FdoCommon.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonConnStringParser.h>
#include <FdoSpatial.h>
#include <FdoGeometry.h>
#include <Geometry/EnvelopeImpl.h>

#include <FdoCommonSchemaUtil.h>

// PostGIS Messages
#ifdef _WIN32
#include "../Message/inc/PostGisMessage.h"
#else
#include "../../Inc/PostGisMessage.h"
#endif

// PostgreSQL client library imported to dedicated namespace
#include <libpq-fe.h>

/// \brief
/// The main top-level namespace providing context for all FDO items.
///
/// Currently, this namespace is only used by the FDO provider for PostGIS
/// and consist of single nested namespace named postgis.
///
namespace fdo
{

/// \brief
/// Dedicated for PostGIS provider elements.
///
namespace postgis
{

//
// General descriptive information about PostGIS Provier
//

extern FdoString* ProviderName;
extern FdoString* ProviderVersion;
extern FdoString* FdoVersion;

extern char const* ProviderDisplayName;
extern char const* ProviderDescription;

//
// Connection properties defined for PostGIS Provier
//

extern FdoString* PropertyUsername;
extern FdoString* PropertyPassword;
extern FdoString* PropertyService;
extern FdoString* PropertyDatastore;
extern FdoString* PropertyDefaultHostname;
extern FdoString* PropertyDefaultPort;

//
// Create datastore command properties
//

extern FdoString* PropertyDatastoreName;
extern FdoString* PropertyDatastoreDescription;

//
// Default properties for Feature Schema
//

const wchar_t DefaultSchemaXml[12] = L"schema.xml";
const wchar_t DefaultSchemaName[8] = L"Default";
const wchar_t DefaultSchemaDescription[40] = L"Default FDO Feature Schema for PostGIS.";

//
// Default Spatial Context properties
//

const wchar_t SpatialContextDefaultName[8] = L"Default";
const wchar_t SpatialContextDefaultDesc[50] = L"Defines default properties of coordinate system.";
const wchar_t SpatialContextDefaultCoordSysName[8] = L"Unknown";
const wchar_t SpatialContextDefaulWkt[2] = L"";
const double SpatialContextDefaultXYTolerance = 0.05;
const double SpatialContextDefaultZTolerance = 0.05;
const double SpatialContextDefaultMTolerance = 0.05;
const double SpatialContextDefaultMinX = -10000000;
const double SpatialContextDefaultMinY = -10000000;
const double SpatialContextDefaultMinZ = -10000000;
const double SpatialContextDefaultMinM = -10000000;
const double SpatialContextDefaultMaxX = 10000000;
const double SpatialContextDefaultMaxY = 10000000;
const double SpatialContextDefaultMaxZ = 10000000;
const double SpatialContextDefaultMaxM = 10000000;

}} // namespace fdo::postgis

#endif // POSTGISPROVIDER_H_INCLUDED
