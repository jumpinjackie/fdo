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

#include "stdafx.h"
#include <Sm/Ph/SpatialContext.h>
#include <Sm/Error.h>
#include <FdoCommonStringUtil.h>

FdoSmPhSpatialContext::FdoSmPhSpatialContext(
    FdoSmPhMgrP pMgr,
    FdoInt64 srid,
    FdoString* name,
    FdoString* description,
    FdoString* coordinateSystem,
    FdoString* coordinateSystemWkt,
    FdoSpatialContextExtentType extentType,
    FdoByteArray * extent,
    double xyTolerance,
    double zTolerance
) : 
    FdoSmPhSchemaElement((FdoString *)name, description, pMgr),
    mId(-1),
	mSrid(srid),
    mCoordSysName(coordinateSystem),
    mCoordSysWkt(coordinateSystemWkt),
    mExtentType(extentType),
    mXYTolerance(xyTolerance),
    mZTolerance(zTolerance)
{
    mExtent = FDO_SAFE_ADDREF(extent);
}


FdoSmPhSpatialContext::~FdoSmPhSpatialContext(void)
{
}

FdoInt64 FdoSmPhSpatialContext::GetId() const
{
	return mId;
}

FdoString* FdoSmPhSpatialContext::GetCoordinateSystem()
{
    return mCoordSysName;
}

FdoString* FdoSmPhSpatialContext::GetCoordinateSystemWkt()
{
    return mCoordSysWkt;
}

FdoSpatialContextExtentType FdoSmPhSpatialContext::GetExtentType()
{
    return mExtentType;
}

FdoByteArray* FdoSmPhSpatialContext::GetExtent()
{
    return FDO_SAFE_ADDREF(mExtent.p);
}

double FdoSmPhSpatialContext::GetXYTolerance()
{
    return mXYTolerance;
}

double FdoSmPhSpatialContext::GetZTolerance()
{
    return mZTolerance;
}


FdoInt64 FdoSmPhSpatialContext::GetSrid()
{
	return mSrid;
}

void FdoSmPhSpatialContext::SetId(FdoInt64 id)
{
	mId = id;
}

