//
// Copyright (C) 2006 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "FdoRdbmsPostGisFilterCapabilities.h"

FdoRdbmsPostGisFilterCapabilities::FdoRdbmsPostGisFilterCapabilities()
{
    // idle
}

FdoRdbmsPostGisFilterCapabilities::~FdoRdbmsPostGisFilterCapabilities()
{
    // idle
}

FdoSpatialOperations *FdoRdbmsPostGisFilterCapabilities::GetSpatialOperations(int& length)
{
    static FdoSpatialOperations spatialOperations[] = { 
        FdoSpatialOperations_Contains,
        FdoSpatialOperations_Crosses,
        FdoSpatialOperations_Disjoint,
        FdoSpatialOperations_Equals,
        FdoSpatialOperations_Intersects,
        FdoSpatialOperations_Overlaps,
        FdoSpatialOperations_Touches,
        FdoSpatialOperations_Within,
        //FdoSpatialOperations_CoveredBy,
        FdoSpatialOperations_Inside,
        FdoSpatialOperations_EnvelopeIntersects
    };

    length = sizeof(spatialOperations) / sizeof(FdoSpatialOperations);
    return spatialOperations;
}

FdoDistanceOperations* FdoRdbmsPostGisFilterCapabilities::GetDistanceOperations(int& length)
{
    static FdoDistanceOperations operations[] =
    {
        FdoDistanceOperations_Beyond,
        FdoDistanceOperations_Within
    };

    length = (sizeof(operations) / sizeof(FdoDistanceOperations));
    return operations;
}
