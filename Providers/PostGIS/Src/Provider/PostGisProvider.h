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
#include <FdoCommon.h>
#include <FdoSpatial.h>
#include <FdoGeometry.h>
#include <Fdo.h>

/// \namespace fdo
///
/// \brief
/// The main top-level namespace providing context for all FDO items,
/// prototypes and definitions.
///
/// Currently, this namespace is only used by the FDO provider for PostGIS
/// and consist of single nested namespace named postgis.
///
namespace fdo
{

/// \namespace postgis
///
/// \brief
/// Nested namespaces dedicated for PostGIS provider elements
///
namespace postgis
{

//
// General descriptive information about PostGIS Provier
//
const wchar_t* PROVIDER_NAME = L"OSGeo.PostGIS.1.0"
const wchar_t* PROVIDER_DEFAULT_DISPLAY_NAME = L"OSGeo FDO Provider for PostGIS"
const wchar_t* PROVIDER_DEFAULT_DESCRIPTION = L"Read/write access to PostGIS-based data store. Supports spatial data types and spatial query operations."
const wchar_t* PROVIDER_VERSION = L"3.2.0.0"
const wchar_t* FDO_VERSION = L"3.2.0.0"

}} // namespace fdo::postgis

#endif // POSTGISPROVIDER_H_INCLUDED
