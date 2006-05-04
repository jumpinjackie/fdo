/*
* 
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
#include <Spatial.h>

#include "mgSpatialUtility.h"
#include "mgSpatialGeometryValidity.h"

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ApproximateGeometryWithLineStrings(NAMESPACE_OSGEO_GEOMETRY::IGeometry * geometry, System::Double maxSpacing, System::Double maxOffset, NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract * geometryFactory)
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::ApproximateGeometryWithLineStrings((static_cast<FdoIGeometry *>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(geometry)->UnmanagedObject.ToPointer())), maxSpacing, maxOffset, static_cast<FdoGeometryFactoryAbstract *>(geometryFactory->UnmanagedObject.ToPointer())))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(unobj, true);
}

NAMESPACE_OSGEO_SPATIAL::SpatialGeometryValidity NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ValidateGeometryByType(NAMESPACE_OSGEO_GEOMETRY::IGeometry * geometry, NAMESPACE_OSGEO_COMMON::GeometryType geometryTypes [], NAMESPACE_OSGEO_COMMON::GeometryComponentType geometryComponentTypes [], System::Int32 dimensionality)
{
    FdoInt32 unTypelength;
    FdoGeometryType* unType;
    if (NULL == geometryTypes)
    {
        unTypelength = 0;
        unType = NULL;
    }
    else
    {
	    unTypelength = geometryTypes->Length;
	    unType = new FdoGeometryType[unTypelength];
        for (FdoInt32 i = 0; i < unTypelength; i++)
        {
            *(unType + i) = static_cast<FdoGeometryType>(geometryTypes[i]);
        }
    }

    FdoInt32 unComponentlength;
    FdoGeometryComponentType* unComponent;
    if (NULL == geometryComponentTypes)
    {
        unComponentlength = 0;
        unComponent = NULL;
    }
    else
    {
	    unComponentlength = geometryComponentTypes->Length;
	    unComponent = new FdoGeometryComponentType[unComponentlength];
        for (FdoInt32 i = 0; i < unComponentlength; i++)
        {
            *(unComponent + i) = static_cast<FdoGeometryComponentType>(geometryComponentTypes[i]);
        }
    }

	FdoSpatialGeometryValidity unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::ValidateGeometryByType((static_cast<FdoIGeometry *>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(geometry)->UnmanagedObject.ToPointer())), unTypelength, unType, unComponentlength, unComponent, dimensionality))

    delete[] unType;
    delete[] unComponent;

	return static_cast<NAMESPACE_OSGEO_SPATIAL::SpatialGeometryValidity>(unobj);
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::Evaluate(NAMESPACE_OSGEO_GEOMETRY::IGeometry * g1, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations op, NAMESPACE_OSGEO_GEOMETRY::IGeometry * g2)
{
	FdoBoolean unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::Evaluate((static_cast<FdoIGeometry *>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(g1)->UnmanagedObject.ToPointer())), static_cast<FdoSpatialOperations>(op), (static_cast<FdoIGeometry *>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp *>(g2)->UnmanagedObject.ToPointer()))))
	return unobj;
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry* NAMESPACE_OSGEO_SPATIAL::SpatialUtility::TesselateCurve( NAMESPACE_OSGEO_GEOMETRY::IGeometry* curve)
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::TesselateCurve((static_cast<FdoIGeometry *>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp*>(curve)->UnmanagedObject.ToPointer()))))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(unobj, true);	
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialUtility::GetExtents(System::Byte bytes[],  System::Double* minX, System::Double* minY,  System::Double* maxX,  System::Double* maxY)
{
    FdoByteArray* umBuffer = ByteArrayToFdoByteArray(bytes);
	double uminx = *minX;
	double uminy = *minY;
	double umaxx = *maxX;
	double umaxy = *maxY;
	EXCEPTION_HANDLER(FdoSpatialUtility::GetExtents(umBuffer, uminx, uminy, umaxx, umaxy))

    umBuffer->Release();

    *minX = uminx;
	*minY = uminy;
	*maxX = umaxx;
	*maxY = umaxy;
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialUtility::GetExtents(System::Byte bytes[],  System::Double* minX, System::Double* minY,  System::Double* minZ, System::Double* maxX,  System::Double* maxY, System::Double* maxZ)
{
    FdoByteArray* umBuffer = ByteArrayToFdoByteArray(bytes);
	double uminx = *minX;
	double uminy = *minY;
	double uminz = *minZ;
	double umaxx = *maxX;
	double umaxy = *maxY;
	double umaxz = *maxZ;
	EXCEPTION_HANDLER(FdoSpatialUtility::GetExtents(umBuffer, uminx, uminz, uminy, umaxx, umaxy, umaxz))

    umBuffer->Release();

    *minX = uminx;
	*minY = uminy;
	*minZ = uminz;
	*maxX = umaxx;
	*maxY = umaxy;
	*maxZ = umaxz;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring, System::Double coordinateX, System::Double coordinateY, System::Boolean* isOnBoundary)
{
	FdoBoolean rv;
    FdoILinearRing* p1 = NULL;
    if (NULL != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp*>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv = FdoSpatialUtility::PointInRing(p1, coordinateX, coordinateY, BooleanToFdoBool(isOnBoundary)))

	return rv;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring, System::Double coordinateX, System::Double coordinateY)
{
	FdoBoolean rv;
    FdoILinearRing* p1 = NULL;
    if (NULL != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp*>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv = FdoSpatialUtility::PointInRing(p1, coordinateX, coordinateY, NULL))

	return rv;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean * isOnExtBoundary, System::Boolean * isOnInBoundary)
{
	FdoBoolean rv;
    FdoIPolygon* p1 = NULL;
    if (NULL != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp*>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv= FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, BooleanToFdoBool(isOnExtBoundary), BooleanToFdoBool(isOnInBoundary)))

	return rv;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean * isOnExtBoundary)
{
	FdoBoolean rv;
    FdoIPolygon* p1 = NULL;
    if (NULL != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp*>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv= FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, BooleanToFdoBool(isOnExtBoundary), NULL))

	return rv;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY)
{
	FdoBoolean rv;
    FdoIPolygon* p1 = NULL;
    if (NULL != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp*>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv= FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, NULL, NULL))

	return rv;
}

System::Double NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ComputeLinearRingArea( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring )
{
	FdoDouble rv;
    FdoILinearRing* p1 = NULL;
    if (NULL != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp*>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv = FdoSpatialUtility::ComputeLinearRingArea(p1))

	return rv;
}

System::Double NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ComputeLinearRingLength( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring )
{
	FdoDouble rv;
    FdoILinearRing* p1 = NULL;
    if (NULL != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp*>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv = FdoSpatialUtility::ComputeLinearRingLength(p1))

	return rv;
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry* NAMESPACE_OSGEO_SPATIAL::SpatialUtility::CreateGeometryFromRings( NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection* rings, System ::Boolean relateRings )
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::CreateGeometryFromRings(static_cast<FdoLinearRingCollection *>(rings->UnmanagedObject.ToPointer()), relateRings))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(unobj, true);	
}

Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::IsCircularArcValid( NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment *arc, System::Double tolerance )
{
	FdoBoolean rv;
    FdoICircularArcSegment* p1 = NULL;
    if (NULL != arc)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable* dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp*>(arc));
        p1 = static_cast<FdoICircularArcSegment*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(rv = FdoSpatialUtility::IsCircularArcValid(p1, tolerance))

	return rv;
}
