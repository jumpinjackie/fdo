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
#include "FilterCapabilities.h"

#include <cassert>

namespace fdo { namespace postgis {

FilterCapabilities::FilterCapabilities()
{
}

FilterCapabilities::~FilterCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void FilterCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFilterCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoConditionType* FilterCapabilities::GetConditionTypes(FdoInt32& size)
{
    // TODO: Verify types of supported conditions

    static FdoConditionType types[] =
    {
        FdoConditionType_Comparison,
        FdoConditionType_Like,
        FdoConditionType_In,
        FdoConditionType_Null,
        FdoConditionType_Spatial,
    };

    size = (sizeof(types) / sizeof(FdoConditionType));
    return types;
}

FdoSpatialOperations* FilterCapabilities::GetSpatialOperations(FdoInt32& size)
{
    // TODO: Verify types of supported operations

    static FdoSpatialOperations operations[] =
    {
        FdoSpatialOperations_Contains,
        FdoSpatialOperations_Crosses,
        FdoSpatialOperations_Disjoint,
        FdoSpatialOperations_Equals,
        FdoSpatialOperations_Intersects,
        FdoSpatialOperations_Overlaps,
        FdoSpatialOperations_Touches,
        FdoSpatialOperations_Within,
        //FdoSpatialOperations_CoveredBy,
        //FdoSpatialOperations_Inside,
        //FdoSpatialOperations_EnvelopeIntersects
    };

    size = (sizeof(operations) / sizeof(FdoConditionType));
    return operations;
}

FdoDistanceOperations* FilterCapabilities::GetDistanceOperations(FdoInt32& size)
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

bool FilterCapabilities::SupportsGeodesicDistance()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool FilterCapabilities::SupportsNonLiteralGeometricOperations()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

}} // namespace fdo::postgis
