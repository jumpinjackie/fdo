#ifndef FDOSMLPSQSSPATIALCONTEXT_H
#define FDOSMLPSQSSPATIALCONTEXT_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/SpatialContext.h>

// This logical/physical class encapsulates physical interactions with
// Spatial Contexts for callers at the logical or logical/physical level.
// It includes the merging of publicly-visible Spatial Contexts with
// the non-public Spatial Context Groups.
class FdoSmLpSqsSpatialContext : public FdoSmLpSpatialContext
{
public:
    ~FdoSmLpSqsSpatialContext(void);

    /// Construct an instance from readers
    FdoSmLpSqsSpatialContext(
        FdoSmPhSpatialContextReaderP scReader,
        FdoSmPhSpatialContextGroupReaderP scgReader,
        FdoSmPhMgrP physicalSchema);
	

    /// Constructs an instance from the given attributes.
	FdoSmLpSqsSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        FdoSmPhMgrP physicalSchema
    );

protected:
    FdoSmLpSqsSpatialContext() {}

    virtual void PostFinalize();

private:
    // Adjusts x or y parts of extents so that max > min.
    // SQL Server requires this for spatial index extents, which this provider
    // sets from the spatial context extents.
    // We need to adjust the extents, rather than throw exception since Bulk copy 
    // can get source spatial context extents where 
    // (not(maxx > minx ) or not(maxy > miny)) from datastores
    // for other providers.
    //
    // It doesn't matter if features outside the adjusted extents are later added. 
    // SQL Server allows them to be inserted. 
    bool AdjustExtentDimension( double& min1, double& max1, double min2, double max2 );   
};

typedef FdoPtr<FdoSmLpSqsSpatialContext> FdoSmLpSqsSpatialContextP;

#endif


