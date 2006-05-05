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
#include "FdoWfsOgcFilterCapabilities.h"
#include "FdoWfsOgcScalarCapabilities.h"
#include "FdoWfsOgcSpatialCapabilities.h"

FdoWfsFilterCapabilities::FdoWfsFilterCapabilities (FdoWfsOgcFilterCapabilities* ogcFilterCapabilities) :
                                    m_ogcFilterCapabilities(ogcFilterCapabilities)
{
    FDO_SAFE_ADDREF(m_ogcFilterCapabilities.p);
}

FdoWfsFilterCapabilities::~FdoWfsFilterCapabilities ()
{
}

void FdoWfsFilterCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoConditionType objects the feature provider supports.</summary>
/// <param name="length">Output the number of condition types</param> 
/// <returns>Returns the list of condition types</returns> 
FdoConditionType* FdoWfsFilterCapabilities::GetConditionTypes (FdoInt32& length)
{
	if (!m_ogcFilterCapabilities)
	{
		length = 0;
		return NULL;
	}

    FdoPtr<FdoWfsOgcScalarCapabilities> scalarCaps = m_ogcFilterCapabilities->GetScalarCapabilities();
    FdoPtr<FdoWfsOgcSpatialCapabilities> spatialCaps = m_ogcFilterCapabilities->GetSpatialCapabilities();
    
    static FdoConditionType types[6];
    length = 0;
    FdoInt32 cmpOps = scalarCaps->GetComparisonOperators();
    if (cmpOps & FdoWfsOgcScalarCapabilities::ComparisonOperators_Simple)
        types[length++] = FdoConditionType_Comparison;
    if (cmpOps & FdoWfsOgcScalarCapabilities::ComparisonOperators_Like)
        types[length++] = FdoConditionType_Like;
    if (cmpOps & FdoWfsOgcScalarCapabilities::ComparisonOperators_NullCheck)
        types[length++] = FdoConditionType_Null;

    FdoInt32 spatialOps = spatialCaps->GetSpatialOperators();
    if (spatialOps != 0)
        types[length++] = FdoConditionType_Spatial;

    // we are not supporting the followings
    // FdoConditionType_In,
    // FdoConditionType_Distance

    return (types);
}

/// <summary>Returns an array of FdoSpatialOperation objects the feature provider supports.</summary>
/// <param name="length">Output the number of spatial operations</param> 
/// <returns>Returns the list of spartial operations</returns> 
FdoSpatialOperations* FdoWfsFilterCapabilities::GetSpatialOperations (FdoInt32& length)
{
	if (!m_ogcFilterCapabilities)
	{
		length = 0;
		return NULL;
	}

    static FdoSpatialOperations operations[11];
    FdoPtr<FdoWfsOgcSpatialCapabilities> spatialCaps = m_ogcFilterCapabilities->GetSpatialCapabilities();
    FdoInt32 spatialOps = spatialCaps->GetSpatialOperators();
    
    length = 0;
#pragma message ("TODO: Support OGC Operator BBOX through EnvelopeIntersects?")
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Intersect)
        operations[length++] = FdoSpatialOperations_EnvelopeIntersects;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Contains)
        operations[length++] = FdoSpatialOperations_Contains;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Crosses)
        operations[length++] = FdoSpatialOperations_Crosses;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Disjoint)
        operations[length++] = FdoSpatialOperations_Disjoint;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Equals)
        operations[length++] = FdoSpatialOperations_Equals;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Intersect)
        operations[length++] = FdoSpatialOperations_Intersects;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Overlaps)
        operations[length++] = FdoSpatialOperations_Overlaps;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Touches)
        operations[length++] = FdoSpatialOperations_Touches;
    if (spatialOps & FdoWfsOgcSpatialCapabilities::SpatialOperators_Within)
        operations[length++] = FdoSpatialOperations_Within;
    
    // There are no equivalents in OGC filter regarding the following FDO spatial operations
    // FdoSpatialOperations_CoveredBy,
    // FdoSpatialOperations_Inside,

    return (operations);
}

/// <summary>Returns an array of FdoDistanceOperation objects the feature provider supports.</summary>
/// <param name="length">Output the number of distance operations</param> 
/// <returns>Returns the list of distance operations</returns> 
FdoDistanceOperations* FdoWfsFilterCapabilities::GetDistanceOperations (FdoInt32& length)
{
    // WFS does not support Distance operations
    //static FdoDistanceOperations operations[] =
    //{
    //  FdoDistanceOperations_Beyond,
    //  FdoDistanceOperations_Within
    //};

    length = 0;

    return (NULL);
}

/// <summary>Determines if the feature provider supports geodesic distance measurement. Returns false if the feature provider supports only linear distance measurement.</summary>
/// <returns>Returns true if the feature provider supports geodesic distance measurement</returns> 
bool FdoWfsFilterCapabilities::SupportsGeodesicDistance ()
{
    return (false); 
}

/// <summary>Determines if spatial and distance operations can be applied between two 
/// geometric properties. Returns false if spatial and distance operations can be applied only between 
/// a geometric property and a literal geometry.</summary>
/// <returns>Returns true if spatial and distance operations can be applied</returns> 
bool FdoWfsFilterCapabilities::SupportsNonLiteralGeometricOperations ()
{
    return (true); 
}


