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

#include "SpatialContext.h"

#include <cassert>

namespace fdo { namespace postgis {

SpatialContext::SpatialContext()
{
}

SpatialContext::~SpatialContext()
{
}

FdoString* SpatialContext::GetName() const
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void SpatialContext::SetName(FdoString* name)
{
    assert(!"NOT IMPLEMENTED");
}

FdoString* SpatialContext::GetDescription() const
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void SpatialContext::SetDescription(FdoString* desc)
{
    assert(!"NOT IMPLEMENTED");
}

FdoString* SpatialContext::GetCoordinateSystem() const
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void SpatialContext::SetCoordSysName(FdoString* csName)
{
    assert(!"NOT IMPLEMENTED");
}

FdoString* SpatialContext::GetCoordinateSystemWkt() const
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void SpatialContext::SetCoordinateSystemWkt(FdoString* wkt)
{
    assert(!"NOT IMPLEMENTED");
}

FdoSpatialContextExtentType SpatialContext::GetExtentType() const
{
    assert(!"NOT IMPLEMENTED");
    return FdoSpatialContextExtentType_Static;
}

void SpatialContext::SetExtentType(FdoSpatialContextExtentType type)
{
    assert(!"NOT IMPLEMENTED");
}

FdoByteArray* SpatialContext::GetExtent() const
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void SpatialContext::SetExtent(FdoByteArray* extent)
{
    assert(!"NOT IMPLEMENTED");
}

double SpatialContext::GetXYTolerance() const
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void SpatialContext::SetXYTolerance(double tol)
{
    assert(!"NOT IMPLEMENTED");
}

double SpatialContext::GetZTolerance() const
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void SpatialContext::SetZTolerance(double tol)
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis
