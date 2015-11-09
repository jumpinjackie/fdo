#ifndef FDOSQLSERVER_OV_H
#define FDOSQLSERVER_OV_H
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#ifdef _WIN32
#pragma once
#endif

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the FDOSQLSERVER_OV_EXPORTS
// symbol defined.  The FDO_EXPORTS symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// FDOSQLSERVER_OV_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _WIN32
#ifdef FDOSQLSERVER_OV_EXPORTS
#define FDOSQLSERVER_OV_API __declspec(dllexport)
#else
#define FDOSQLSERVER_OV_API __declspec(dllimport)
#endif
#else
#define FDOSQLSERVER_OV_API
#endif

#endif // FDOSQLSERVER_OV_H


