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

//#include "stdafx.h"
#include "Geometry.h"

#include <Fdo.h>
#include <Geometry/Fgf/Factory.h>

void *GeometryFromWkb (void *wkb, unsigned long size)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoByteArray> array;
    FdoIGeometry *g = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    array = FdoByteArray::Create ((FdoByte*)wkb, size);
    try
    {
        g = factory->CreateGeometryFromWkb (array);
    }
    catch (...)
    {
    }

    return (void *)g;
}

void FreeGeometry (void *geometry)
{
	FdoIGeometry *g = (FdoIGeometry *)geometry;

    FDO_SAFE_RELEASE(g);
}

char *WkbFromGeometry (void *geometry, int *size, int srid)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoByteArray> array;
    void *ret = NULL;
	*size = 0;

    if ( geometry == NULL )
		return (char*)ret;

    factory = FdoFgfGeometryFactory::GetInstance ();
    array = factory->GetWkb ((FdoIGeometry *)geometry);
    try
    {
        *size = array->GetCount () + 4;
        ret = malloc (*size);
        memset (ret, 0, *size);
		memcpy(ret, (void *)&srid, 4);
        memmove (((char*)ret) + 4, array->GetData (), *size - 4);
    }
    catch (...)
    {
    }

    return (char*)(ret);
}

