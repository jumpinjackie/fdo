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
#include "SpatialContextReader.h"

namespace fdo { namespace postgis {

SpatialContextReader::SpatialContextReader(SpatialContextCollection*  scc)
    :   mSpatialContexts(scc), mCurrentContext(NULL), mCurrentIndex(0)
{
    FDO_SAFE_ADDREF(mSpatialContexts.p);
}

SpatialContextReader::~SpatialContextReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SpatialContextReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISpatialContextReader interface
///////////////////////////////////////////////////////////////////////////////

FdoString* SpatialContextReader::GetName()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* SpatialContextReader::GetDescription()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* SpatialContextReader::GetCoordinateSystem()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* SpatialContextReader::GetCoordinateSystemWkt()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoSpatialContextExtentType SpatialContextReader::GetExtentType()
{
    assert(!"NOT IMPLEMENTED");
    return FdoSpatialContextExtentType_Dynamic;
}

FdoByteArray* SpatialContextReader::GetExtent()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

const double SpatialContextReader::GetXYTolerance()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

const double SpatialContextReader::GetZTolerance()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

const bool SpatialContextReader::IsActive()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

bool SpatialContextReader::ReadNext()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

}} // namespace fdo::postgis
