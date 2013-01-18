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
#include "FdoWfsSpatialContextReader.h"
#include "FdoWfsServiceMetadata.h"

FdoWfsSpatialContextReader::FdoWfsSpatialContextReader()
	: mIndex(-1)
{
}

FdoWfsSpatialContextReader::FdoWfsSpatialContextReader(FdoWfsServiceMetadata* metadata)
	: mIndex(-1)
{
	mMetadata = FDO_SAFE_ADDREF (metadata);
}

FdoWfsSpatialContextReader::~FdoWfsSpatialContextReader(void)
{
}

FdoString* FdoWfsSpatialContextReader::GetName()
{
	FdoStringsP crsNames = mMetadata->GetCRSNames ();
	return crsNames->GetString (mIndex);
}

FdoString* FdoWfsSpatialContextReader::GetDescription()
{
    return (L"");
}

FdoString* FdoWfsSpatialContextReader::GetCoordinateSystem()
{
	FdoStringsP crsNames = mMetadata->GetCRSNames ();
	return crsNames->GetString (mIndex);
}

// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context 
// currently being read. If the result is an empty string, the coordinate system WKT description
// is not available. </summary>
FdoString* FdoWfsSpatialContextReader::GetCoordinateSystemWkt()
{
	return (L"");
}

FdoSpatialContextExtentType FdoWfsSpatialContextReader::GetExtentType()
{
    return (FdoSpatialContextExtentType_Static);
}

FdoByteArray* FdoWfsSpatialContextReader::GetExtent()
{
	FdoOwsGeographicBoundingBoxCollectionP bboxes = mMetadata->GetCRSExtents ();
	FdoOwsGeographicBoundingBoxP bbox = bboxes->GetItem (mIndex);
	
	double ordinates[10];
	ordinates[0] = bbox->GetWestBoundLongitude ();
	ordinates[1] = bbox->GetSouthBoundLatitude ();
	ordinates[2] = bbox->GetEastBoundLongitude ();
	ordinates[3] = bbox->GetSouthBoundLatitude ();
	ordinates[4] = bbox->GetEastBoundLongitude ();
	ordinates[5] = bbox->GetNorthBoundLatitude ();
	ordinates[6] = bbox->GetWestBoundLongitude ();
	ordinates[7] = bbox->GetNorthBoundLatitude ();
	ordinates[8] = bbox->GetWestBoundLongitude ();
	ordinates[9] = bbox->GetSouthBoundLatitude ();

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordinates);
	FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(linearRing, NULL);

	return gf->GetFgf(polygon);
}

const double FdoWfsSpatialContextReader::GetXYTolerance()
{
    return (0.01);
}

const double FdoWfsSpatialContextReader::GetZTolerance()
{
    return (0.01);
}

const bool FdoWfsSpatialContextReader::IsActive()
{
    return (false);
}

bool FdoWfsSpatialContextReader::ReadNext()
{
	FdoStringsP crsNames = mMetadata->GetCRSNames ();	
	return (mIndex++ < crsNames->GetCount() - 1);
}

void FdoWfsSpatialContextReader::Dispose()
{
    delete this;
}
