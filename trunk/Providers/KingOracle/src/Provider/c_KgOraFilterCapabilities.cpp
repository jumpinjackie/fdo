/*
* Copyright (C) 2006  SL-King d.o.o
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
*/

#include "stdafx.h"

c_KgOraFilterCapabilities::c_KgOraFilterCapabilities ()
{
}

c_KgOraFilterCapabilities::~c_KgOraFilterCapabilities ()
{
}

void c_KgOraFilterCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoConditionType objects the feature provider supports.</summary>
/// <param name="length">Output the number of condition types</param> 
/// <returns>Returns the list of condition types</returns> 
FdoConditionType* c_KgOraFilterCapabilities::GetConditionTypes (FdoInt32& Length)
{
    static FdoConditionType types[] =
    {
      FdoConditionType_Comparison,
      FdoConditionType_Like,
      FdoConditionType_In,
      FdoConditionType_Null,
      FdoConditionType_Spatial,
    };

    Length = sizeof (types) / sizeof (FdoConditionType);

    return (types);
}//end of c_KgOraFilterCapabilities::GetConditionTypes

/// <summary>Returns an array of FdoSpatialOperation objects the feature provider supports.</summary>
/// <param name="length">Output the number of spatial operations</param> 
/// <returns>Returns the list of spartial operations</returns> 
FdoSpatialOperations* c_KgOraFilterCapabilities::GetSpatialOperations (FdoInt32& Length)
{
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
        FdoSpatialOperations_CoveredBy,
        FdoSpatialOperations_Inside,
        FdoSpatialOperations_EnvelopeIntersects
    };

    Length = sizeof (operations) / sizeof (FdoSpatialOperations);

    return (operations);
}//end of c_KgOraFilterCapabilities::GetSpatialOperations

/// <summary>Returns an array of FdoDistanceOperation objects the feature provider supports.</summary>
/// <param name="length">Output the number of distance operations</param> 
/// <returns>Returns the list of distance operations</returns> 
FdoDistanceOperations* c_KgOraFilterCapabilities::GetDistanceOperations (FdoInt32& Length)
{
    Length = 0;

    return NULL;
}

/// <summary>Determines if the feature provider supports geodesic distance measurement. Returns false if the feature provider supports only linear distance measurement.</summary>
/// <returns>Returns true if the feature provider supports geodesic distance measurement</returns> 
bool c_KgOraFilterCapabilities::SupportsGeodesicDistance ()
{
    return false;
}

/// <summary>Determines if spatial and distance operations can be applied between two 
/// geometric properties. Returns false if spatial and distance operations can be applied only between 
/// a geometric property and a literal geometry.</summary>
/// <returns>Returns true if spatial and distance operations can be applied</returns> 
bool c_KgOraFilterCapabilities::SupportsNonLiteralGeometricOperations ()
{
    return false;
}

