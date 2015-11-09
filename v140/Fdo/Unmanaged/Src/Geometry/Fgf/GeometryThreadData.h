#ifndef _FDOGEOMETRYTHREADDATA_H
#define _FDOGEOMETRYTHREADDATA_H

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

#include "GeometryFactory2.h"

#ifdef _WIN32
#include <windows.h>
typedef int FdoGeometryThreadDataKey;
#else
#include <pthread.h>
typedef pthread_key_t FdoGeometryThreadDataKey;
#endif

struct FdoGeometryThreadData
{
    FdoPtr<FdoFgfGeometryFactory> geometryFactory;
    FdoPtr<FdoFgfGeometryPools>   geometryPools;

    static FdoGeometryThreadData *      GetValue();
    static void                         ReleaseValue();
    static FdoGeometryThreadDataKey     GetKey();
    static void                         SetKey(FdoGeometryThreadDataKey key);
    static FdoFgfGeometryPools *        GetPoolsNoRef();
};

#endif

