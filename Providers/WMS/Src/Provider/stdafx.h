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

// Windows Header Files:
#include <windows.h>
#include <string>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FDOWMS_EXPORTS
// symbol defined.  The FDOWMS_EXPORTS symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FDOWMS_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FDOWMS_EXPORTS
#define FDOWMS_API __declspec(dllexport)
#else // FDOWMS_EXPORTS
#define FDOWMS_API __declspec(dllimport)
#endif // FDOWMS_EXPORTS

#else // _WIN32

#define FDOWMS_API

#endif // _WIN32

#include <cstring>

// Base FDO headers
#include <Fdo.h>

// FdoCommon headers
#include <OWS/FdoOWS.h>
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonCommand.h>
#include <FdoCommonFeatureCommand.h>
#include <FdoCommonFile.h>
#include <FdoCommonStringUtil.h>

// Message Catalogue
#include <../Message/Inc/FdoWmsMessage.h>

// Provider specific utility headers
#include <FdoWmsUtils.h>


