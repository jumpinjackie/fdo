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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "SpatialContext.h"

#include <cassert>

namespace fdo { namespace postgis {

SpatialContext::SpatialContext() :
    mName(SpatialContextDefaultName),
    mDescription(SpatialContextDefaultDesc),
    mCoordSysName(SpatialContextDefaultCoordSysName),
    mCoordSysWkt(SpatialContextDefaulWkt),
    mExtentType(FdoSpatialContextExtentType_Static),
    mExtent(NULL),
    mIsExtentUpdated(true),
    mXYTolerance(SpatialContextDefaultXYTolerance),
    mZTolerance(SpatialContextDefaultZTolerance),
    mMTolerance(SpatialContextDefaultMTolerance)
{
    FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
    mExtent = FdoEnvelopeImpl::Create();
    
    // No need to initialize the enxtend on construction
    // SpatialContextDefaultMinX, SpatialContextDefaultMinY, SpatialContextDefaultMinZ,
    // SpatialContextDefaultMaxX, SpatialContextDefaultMaxY, SpatialContextDefaultMaxZ
    
}

SpatialContext::~SpatialContext()
{
}

FdoString* SpatialContext::GetName() const
{
    return mName;
}

void SpatialContext::SetName(FdoString* name)
{
    mName = name;
}

FdoString* SpatialContext::GetDescription() const
{
    return mDescription;
}

void SpatialContext::SetDescription(FdoString* desc)
{
    mDescription = desc;
}

FdoString* SpatialContext::GetCoordinateSystem() const
{
    // TODO: Verify this function and value
    return mCoordSysName;
}

void SpatialContext::SetCoordinateSystem(FdoString* csName)
{
    mCoordSysName = csName;
}

FdoString* SpatialContext::GetCoordinateSystemWkt() const
{
    return mCoordSysWkt;
}

void SpatialContext::SetCoordinateSystemWkt(FdoString* csWkt)
{
    mCoordSysWkt = csWkt;
}

FdoSpatialContextExtentType SpatialContext::GetExtentType() const
{
    return mExtentType;
}

void SpatialContext::SetExtentType(FdoSpatialContextExtentType type)
{
    mExtentType = type;
}

FdoByteArray* SpatialContext::GetExtent() const
{
    FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
    if (mExtent->GetIsEmpty())
    {
        // Build extent based on properties of default Spatial Context

        FdoPtr<FdoIEnvelope> envelope = NULL;
        envelope = factory->CreateEnvelopeXYZ(
            SpatialContextDefaultMinX,
            SpatialContextDefaultMinY,
            SpatialContextDefaultMinZ,
            SpatialContextDefaultMaxX,
            SpatialContextDefaultMaxY,
            SpatialContextDefaultMaxZ);

        FdoPtr<FdoIGeometry> geometry(factory->CreateGeometry(envelope));
        return factory->GetFgf(geometry);        
    }
    else
    {
        FdoPtr<FdoIGeometry> geometry(factory->CreateGeometry(mExtent));
        return factory->GetFgf(geometry);
    }
}

void SpatialContext::SetExtent(FdoByteArray* extent)
{
    FDOLOG_MARKER("SpatialContext::+SetExtent");
    FDOLOG_WRITE("NOT IMPLEMENTED");
}

double SpatialContext::GetXYTolerance() const
{
    return mXYTolerance;
}

void SpatialContext::SetXYTolerance(double xyTolerance)
{
    mXYTolerance = xyTolerance;
}

double SpatialContext::GetZTolerance() const
{
    return mZTolerance;
}

void SpatialContext::SetZTolerance(double zTolerance)
{
    mZTolerance = zTolerance;
}

double SpatialContext::GetMTolerance() const
{
    return mMTolerance;
}

void SpatialContext::SetMTolerance(double mTolerance)
{
    mMTolerance = mTolerance;
}

}} // namespace fdo::postgis
