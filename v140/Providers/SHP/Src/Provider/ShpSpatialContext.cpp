/*
 * 
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


#include <stdafx.h>

#include "ShpSpatialContext.h"

ShpSpatialContext::ShpSpatialContext () :
    mName (SPATIALCONTEXT_DEFAULT_NAME),
    mDescription (L""),
    mCoordSysName (SPATIALCONTEXT_CSYS_DEFAULT_NAME),
    mWkt (SPATIALCONTEXT_CSYS_DEFAULT_WKT),
    mExtentType (FdoSpatialContextExtentType_Static),
    mXYTolerance (SPATIALCONTEXT_DEFAULT_XY_TOLERANCE),
    mZTolerance (SPATIALCONTEXT_DEFAULT_Z_TOLERANCE)
{
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
    FdoPtr<FdoIEnvelope> envelope = factory->CreateEnvelopeXY (
        SPATIALCONTEXT_DEFAULT_MINX,
        SPATIALCONTEXT_DEFAULT_MINY,
        SPATIALCONTEXT_DEFAULT_MAXX,
        SPATIALCONTEXT_DEFAULT_MAXY);
    FdoPtr<FdoIGeometry> geometry = factory->CreateGeometry (envelope);
    mExtent = factory->GetFgf (geometry);
	mIsExtentUpdated = true;
	mIsFromConfigFile = false;
}

ShpSpatialContext::~ShpSpatialContext ()
{
}

FdoString* ShpSpatialContext::GetName ()
{
    return (mName);
}

void ShpSpatialContext::SetName (FdoString* name)
{
    mName = name;
}

FdoString* ShpSpatialContext::GetDescription ()
{
    return (mDescription);
}

void ShpSpatialContext::SetDescription (FdoString* desc)
{
    mDescription = desc;
}

FdoString* ShpSpatialContext::GetCoordinateSystem ()
{
    return (mCoordSysName);
}

void ShpSpatialContext::SetCoordSysName (FdoString * csName)
{
    mCoordSysName = csName;
}

FdoString* ShpSpatialContext::GetCoordinateSystemWkt ()
{
    return (mWkt);
}

void ShpSpatialContext::SetCoordinateSystemWkt (FdoString* wkt)
{
    mWkt = wkt;
}

FdoSpatialContextExtentType ShpSpatialContext::GetExtentType ()
{
    return (mExtentType);
}

void ShpSpatialContext::SetExtentType (FdoSpatialContextExtentType type)
{
    mExtentType = type;
}

FdoByteArray* ShpSpatialContext::GetExtent ()
{
    return (FDO_SAFE_ADDREF(mExtent.p));
}

void ShpSpatialContext::SetExtent (FdoByteArray * extent)
{
    FDO_SAFE_ADDREF(extent);
    mExtent = extent;
}

double ShpSpatialContext::GetXYTolerance ()
{
    return (mXYTolerance);
}

void ShpSpatialContext::SetXYTolerance (double tol)
{
    mXYTolerance = tol;
}

double ShpSpatialContext::GetZTolerance ()
{
    return (mZTolerance);
}

void ShpSpatialContext::SetZTolerance (double tol)
{
    mZTolerance = tol;
}

bool ShpSpatialContext::GetIsExtentUpdated()
{
	return mIsExtentUpdated;
}

void ShpSpatialContext::SetIsExtentUpdated( bool value )
{
	mIsExtentUpdated = value;
}

void ShpSpatialContext::SetIsFromConfigFile( bool value )
{
	mIsFromConfigFile = value;
}

bool ShpSpatialContext::GetIsFromConfigFile()
{
	return mIsFromConfigFile;
}
