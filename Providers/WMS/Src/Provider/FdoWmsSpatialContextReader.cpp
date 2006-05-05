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
#include "FdoWmsSpatialContextReader.h"

FdoWmsSpatialContextReader::FdoWmsSpatialContextReader()
	: mIndex (-1)
{
}

FdoWmsSpatialContextReader::FdoWmsSpatialContextReader(FdoWmsCapabilities* capa)
	: mIndex (-1), mWMSCapabilities (capa)
{
}

FdoWmsSpatialContextReader::~FdoWmsSpatialContextReader()
{
}

FdoString* FdoWmsSpatialContextReader::GetName()
{
	FdoStringsP crsNames = mWMSCapabilities->GetCRSNames ();
	return crsNames->GetString (mIndex);	
}

FdoString* FdoWmsSpatialContextReader::GetDescription()
{
	return L"";
}

FdoString* FdoWmsSpatialContextReader::GetCoordinateSystem()
{	
	FdoStringsP crsNames = mWMSCapabilities->GetCRSNames ();
	return crsNames->GetString (mIndex);
}

FdoString* FdoWmsSpatialContextReader::GetCoordinateSystemWkt()
{
	// A WKT shall not be supplied for WMS spatial context definitions. 
	return L"";
}

FdoSpatialContextExtentType FdoWmsSpatialContextReader::GetExtentType()
{
	return (FdoSpatialContextExtentType_Static);
}

FdoByteArray* FdoWmsSpatialContextReader::GetExtent()
{
	FdoPtr<FdoWmsBoundingBoxCollection> bboxes = mWMSCapabilities->GetCRSExtents ();
	FdoPtr<FdoWmsBoundingBox> bbox = bboxes->GetItem (mIndex);

	FdoDouble maxx = bbox->GetMaxX ();
	FdoDouble minx = bbox->GetMinX ();
	FdoDouble maxy = bbox->GetMaxY ();
	FdoDouble miny = bbox->GetMinY ();

	FdoDouble ordinates[10];
	ordinates[0] = minx;
	ordinates[1] = miny;
	ordinates[2] = maxx;
	ordinates[3] = miny;
	ordinates[4] = maxx;
	ordinates[5] = maxy;
	ordinates[6] = minx;
	ordinates[7] = maxy;
	ordinates[8] = minx;
	ordinates[9] = miny;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordinates);
	FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(linearRing, NULL);

	return gf->GetFgf(polygon);
}

const double FdoWmsSpatialContextReader::GetXYTolerance()
{
	return (0.001);
}

const double FdoWmsSpatialContextReader::GetZTolerance()
{
	return (0.001);
}

const bool FdoWmsSpatialContextReader::IsActive()
{
	// TODO:	
	return false;
}

bool FdoWmsSpatialContextReader::ReadNext()
{
	FdoStringsP crsNames = mWMSCapabilities->GetCRSNames ();
	return (mIndex++ < crsNames->GetCount () -1);
}

void FdoWmsSpatialContextReader::Dispose()
{
	delete this;
}
