// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfFilterCapabilities.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// default constructor
SdfFilterCapabilities::SdfFilterCapabilities()
{
}


// default destructor
SdfFilterCapabilities::~SdfFilterCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfFilterCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIFilterCapabilities implementation
//-------------------------------------------------------


// Returns an array of the ConditionTypes the feature provider supports.
// The length parameter gives the number of condition types in the array.
FdoConditionType* SdfFilterCapabilities::GetConditionTypes(FdoInt32& length)
{
    //TODO: enable these as we implement them
    static FdoConditionType conditionTypes[] =
    {
        FdoConditionType_Comparison,
        FdoConditionType_Like,
        FdoConditionType_In,
        FdoConditionType_Null,
        FdoConditionType_Spatial
    };

    length = sizeof(conditionTypes) / sizeof(FdoConditionType);
    return conditionTypes;
}


// Returns an array of the SpatialOperations the feature provider supports.
// The length parameter gives the number of spatial operations in the array.
FdoSpatialOperations* SdfFilterCapabilities::GetSpatialOperations(FdoInt32& length)
{
    //TODO: enable these as we implement them
    static FdoSpatialOperations spatialOperations[] =
    {
        FdoSpatialOperations_Contains,
//      FdoSpatialOperations_Crosses,
        FdoSpatialOperations_Disjoint,
//      FdoSpatialOperations_Equals,
        FdoSpatialOperations_Intersects,
//      FdoSpatialOperations_Overlaps,
//      FdoSpatialOperations_Touches,
        FdoSpatialOperations_Within,
//      FdoSpatialOperations_CoveredBy
        FdoSpatialOperations_Inside,
        FdoSpatialOperations_EnvelopeIntersects
    };

    length = sizeof(spatialOperations) / sizeof(FdoSpatialOperations);
    return spatialOperations;
}


// Returns an array of the DistanceOperations the feature provider supports.
// The length parameter gives the number of distance operations in the array.
FdoDistanceOperations* SdfFilterCapabilities::GetDistanceOperations(FdoInt32& length)
{
//  static FdoDistanceOperations distanceOperations[] =
//  {
//      FdoDistanceOperations_Beyond,
//      FdoDistanceOperations_Within
//  };

//  length = sizeof(distanceOperations) / sizeof(FdoDistanceOperations);
//  return distanceOperations;

    // currently no support
    length = 0;
    return NULL;
}


// Returns true if the feature provider supports geodesic distance measurement,
// and false if it only supports linear distance measurement.
bool SdfFilterCapabilities::SupportsGeodesicDistance()
{
    // TODO - verify for SHP
    return false;
}


// Returns true if spatial and distance operations can be applied between two
// geometric properties, and false if spatial and distance operations can be
// applied between a geometric property and a literal geometry.
bool SdfFilterCapabilities::SupportsNonLiteralGeometricOperations()
{
    return false;
}

