#ifndef _ALLGEOMETRYC_H_
#define _ALLGEOMETRYC_H_
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
*/
#ifdef _WIN32
#pragma once
#endif

#include <ctype.h>
#include <Common/GeometryType.h>
#include <Common/Dimensionality.h>
#include <stdlib.h>

/*
 * C functions for wrapping C++ calls
 * This .h is included by C files.
 */

struct  IGeometry_def;
typedef struct IGeometry_def* pIGeometry_def;

struct  ByteArray_def;
typedef struct ByteArray_def* pByteArray_def;


int             IGeometry_CreateGeometry( pIGeometry_def, pIGeometry_def* );

int             IGeometry_GetDerivedType( pIGeometry_def, FdoGeometryType* );
int             IGeometry_GetDimensionality( pIGeometry_def, int* );
int             IGeometry_Release( pIGeometry_def );
int             IGeometry_AddRef( pIGeometry_def );
int             IGeometry_GetFgftString( pIGeometry_def, const wchar_t ** );

int             IGeometry_GetByteArrayData( pByteArray_def, unsigned char **, int * );
pByteArray_def  IByteArray_Create(unsigned char* bytes, int count);
void			IByteArray_Release( pByteArray_def );

int             IGeometry_GetWkb( pIGeometry_def, pByteArray_def * );
int             IGeometry_CreateGeometryFromWkb( pByteArray_def, pIGeometry_def * );
int             IGeometry_GetFgf( pIGeometry_def, pByteArray_def * );
int             IGeometry_CreateGeometryFromFgf( pByteArray_def, pIGeometry_def *, pIGeometry_def* );

            int             IGeometry_CheckNullArg( void *arg, int );
            int             IGeometry_CheckNullArg2( void **arg, int );
#endif

