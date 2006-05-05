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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WFS_EXPORTS
// symbol defined.  The WFS_EXPORTS symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WFS_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WFS_EXPORTS
#define FDOWFS_API __declspec(dllexport)
#else // WFS_EXPORTS
#define FDOWFS_API __declspec(dllimport)
#endif // WFS_EXPORTS

#else // _WIN32

#define FDOWFS_API

#endif // _WIN32

// number of elements in an array
#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))

// Base FDO headers
#include <Fdo.h>
#include <OWS/FdoOWS.h>
#include <Geometry/Fgf/Factory.h>

// FDO Common headers
#include <FdoCommonCommand.h>
#include <FdoCommonFeatureCommand.h>
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonOSUtil.h>

// Provider specific headers
#include <FdoWfsProvider.h>
#include <FdoWfsConnection.h>
#include <../Message/Inc/WFSMessage.h>
#include <FdoWfsConnectionCapabilities.h>
#include <FdoWfsCommandCapabilities.h>
#include <FdoWfsExpressionCapabilities.h>
#include <FdoWfsFilterCapabilities.h>
#include <FdoWfsSchemaCapabilities.h>
#include <FdoWfsRasterCapabilities.h>
#include <FdoWfsTopologyCapabilities.h>
#include <FdoWfsGeometryCapabilities.h>
#include <FdoWfsConnection.h>
#include <FdoWfsConnectionInfo.h>
#include <FdoWfsDescribeSchemaCommand.h>
#include <FdoWfsReader.h>
#include <FdoWfsFeatureReader.h>
#include <FdoWfsSelectCommand.h>
#include <FdoWfsDataReader.h>
#include <FdoWfsSelectAggregatesCommand.h>
#include <FdoWfsGetSpatialContexts.h>
#include <FdoWfsSpatialContextReader.h>
#include <FdoWfsGlobals.h>
#include <FdoWfsCapabilitiesSaxContext.h>
#include <FdoWfsUtils.h>

#ifdef _WIN32
// Windows Header Files:
//#include <windows.h>
#include <winnls.h>
#endif
