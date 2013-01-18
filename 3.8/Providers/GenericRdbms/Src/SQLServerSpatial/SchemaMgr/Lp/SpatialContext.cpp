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
#include "SpatialContext.h"
#include <math.h>

FdoSmLpSqsSpatialContext::FdoSmLpSqsSpatialContext(
    FdoSmPhSpatialContextReaderP scReader,
    FdoSmPhSpatialContextGroupReaderP scgReader,
    FdoSmPhMgrP physicalSchema)
    : 
	FdoSmLpSpatialContext(scReader, scgReader, physicalSchema)
{
}

FdoSmLpSqsSpatialContext::FdoSmLpSqsSpatialContext(
    FdoString* name,
    FdoString* description,
    FdoString* coordinateSystem,
    FdoString* coordinateSystemWkt,
    FdoSpatialContextExtentType extentType,
    FdoByteArray * extent,
    double xyTolerance,
    double zTolerance,
    FdoSmPhMgrP physicalSchema
) : 
	FdoSmLpSpatialContext(
        name, 
        description, 
        coordinateSystem,
        coordinateSystemWkt,
        extentType,
        extent,
        xyTolerance,
        zTolerance,
        physicalSchema
    )
{
}


FdoSmLpSqsSpatialContext::~FdoSmLpSqsSpatialContext(void)
{
}

void FdoSmLpSqsSpatialContext::PostFinalize()
{
    if ( GetElementState() == FdoSchemaElementState_Added ) {
        // New spatial context, so make sure maxx > minx and maxy > miny

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoByteArray> ba = GetExtent();

        // Assume all zeros if not extent set.
        double minx = 0.0;
        double miny = 0.0;
        double maxx = 0.0;
        double maxy = 0.0;
        bool   adjusted = false;

        if ( ba != NULL) {
            FdoPtr<FdoIGeometry> geom1 = gf->CreateGeometryFromFgf(ba);
            FdoPtr<FdoIEnvelope> env1 = geom1->GetEnvelope();
            minx = env1->GetMinX();
            miny = env1->GetMinY();
            maxx = env1->GetMaxX();
            maxy = env1->GetMaxY();
        }

        // Adjust x ordinates
        adjusted = AdjustExtentDimension( minx, maxx, miny, maxy );
        // Adjust y ordinates
        adjusted |= AdjustExtentDimension( miny, maxy, minx, maxx );

        if ( adjusted ) {
            // Extents were adjusted so update them.
            FdoPtr<FdoIEnvelope> env2 = gf->CreateEnvelopeXY( minx, miny, maxx, maxy );
            FdoPtr<FdoIGeometry> geom2 = gf->CreateGeometry( env2 ); 
            ba = gf->GetFgf( geom2 );
            SetExtent(ba);
        }
    }
}

bool FdoSmLpSqsSpatialContext::AdjustExtentDimension( double& min1, double& max1, double min2, double max2 )
{
    bool adjusted = false;

    // No adjustment needed if max already greater than min.
    if ( !(max1 > min1) ) {
        
        if ( max1 < min1 ) {
            // max is less so simply reverse max and min
            double temp = min1;
            min1 = max1;
            max1 = temp;
        }
        else {
            // max and min then same. Adjustment is made by increasing max.
            // First need to pick amount to increase by.

            // Use tolerance if set
            double tolerance = GetXYTolerance();
            
            if ( tolerance <= 0 ) {
                // Tolerance not set. Fall back to length of other extent dimension
                tolerance = fabs( max2 - min2 );

                if ( (tolerance <=0) || (tolerance > 1) )
                    // Default to 1 of other dimension length = 0
                    // Also default to 1 if length > 1 to limit adjustment amount.
                    tolerance = 1;
            }

            max1 = min1 + tolerance;
        }

        adjusted = true;
    }

    return adjusted;
}