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
#include "FdoRdbmsOdbcFilterCapabilities.h"

FdoRdbmsOdbcFilterCapabilities::FdoRdbmsOdbcFilterCapabilities()
{
}

FdoRdbmsOdbcFilterCapabilities::~FdoRdbmsOdbcFilterCapabilities()
{
}

FdoConditionType* FdoRdbmsOdbcFilterCapabilities::GetConditionTypes(int& length)
{
    static FdoConditionType conditionTypes[] = { FdoConditionType_Comparison,
                                                 FdoConditionType_Like,
                                                 FdoConditionType_In,
                                                 FdoConditionType_Null,
                                                 FdoConditionType_Spatial   };
    length = sizeof(conditionTypes)/sizeof(FdoConditionType);
    return conditionTypes;
}

FdoSpatialOperations *FdoRdbmsOdbcFilterCapabilities::GetSpatialOperations(int& length)
{
    static FdoSpatialOperations spatialOperations[] =
    {
        FdoSpatialOperations_Intersects,
        FdoSpatialOperations_Within,
        FdoSpatialOperations_EnvelopeIntersects,
        FdoSpatialOperations_Inside
    };
    length = sizeof(spatialOperations)/sizeof(FdoSpatialOperations);
    return spatialOperations;
}

FdoDistanceOperations* FdoRdbmsOdbcFilterCapabilities::GetDistanceOperations(int& length)
{
    length = 0;
    return NULL;
}

bool FdoRdbmsOdbcFilterCapabilities::SupportsGeodesicDistance()
{
    return false;
}

bool FdoRdbmsOdbcFilterCapabilities::SupportsNonLiteralGeometricOperations()
{
    return false;
}

void FdoRdbmsOdbcFilterCapabilities::Dispose()
{
    delete this;
}
