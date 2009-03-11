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
#include <FdoSpatial.h>

#include "mgSpatialUtility.h"
#include "mgSpatialGeometryValidity.h"

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ApproximateGeometryWithLineStrings(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry, System::Double maxSpacing, System::Double maxOffset, NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract^ geometryFactory)
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::ApproximateGeometryWithLineStrings((static_cast<FdoIGeometry*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(geometry)->UnmanagedObject.ToPointer())), maxSpacing, maxOffset, static_cast<FdoGeometryFactoryAbstract*>(geometryFactory->UnmanagedObject.ToPointer())))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(unobj), true);
}

NAMESPACE_OSGEO_SPATIAL::SpatialGeometryValidity NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ValidateGeometryByType(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry, array<NAMESPACE_OSGEO_COMMON::GeometryType>^ geometryTypes, array<NAMESPACE_OSGEO_COMMON::GeometryComponentType>^ geometryComponentTypes, System::Int32 dimensionality)
{
    FdoInt32 unTypelength;
    FdoGeometryType* unType;
    if (nullptr == geometryTypes)
    {
        unTypelength = 0;
        unType = nullptr;
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
    if (nullptr == geometryComponentTypes)
    {
        unComponentlength = 0;
        unComponent = nullptr;
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
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::ValidateGeometryByType((static_cast<FdoIGeometry*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(geometry)->UnmanagedObject.ToPointer())), unTypelength, unType, unComponentlength, unComponent, dimensionality))

    delete[] unType;
    delete[] unComponent;

	return static_cast<NAMESPACE_OSGEO_SPATIAL::SpatialGeometryValidity>(unobj);
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::Evaluate(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ g1, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations op, NAMESPACE_OSGEO_GEOMETRY::IGeometry^ g2)
{
	FdoBoolean unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::Evaluate((static_cast<FdoIGeometry*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(g1)->UnmanagedObject.ToPointer())), static_cast<FdoSpatialOperations>(op), (static_cast<FdoIGeometry*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(g2)->UnmanagedObject.ToPointer()))))
	return unobj;
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_SPATIAL::SpatialUtility::TesselateCurve( NAMESPACE_OSGEO_GEOMETRY::IGeometry^ curve)
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::TesselateCurve((static_cast<FdoIGeometry*>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(curve)->UnmanagedObject.ToPointer()))))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(unobj), true);	
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialUtility::GetExtents(array<System::Byte>^ bytes,  System::Double% minX, System::Double% minY,  System::Double% maxX,  System::Double% maxY)
{
    FdoByteArray* umBuffer = ByteArrayToFdoByteArray(bytes);
	pin_ptr<FdoDouble> uminx = &minX;
	pin_ptr<FdoDouble> uminy = &minY;
	pin_ptr<FdoDouble> umaxx = &maxX;
	pin_ptr<FdoDouble> umaxy = &maxY;
    try
    {
	    EXCEPTION_HANDLER(FdoSpatialUtility::GetExtents(umBuffer, *uminx, *uminy, *umaxx, *umaxy))
    }
    finally
    {
        if (umBuffer != nullptr)
            umBuffer->Release();
    }
}

System::Void NAMESPACE_OSGEO_SPATIAL::SpatialUtility::GetExtents(array<System::Byte>^ bytes,  System::Double% minX, System::Double% minY,  System::Double% minZ, System::Double% maxX,  System::Double% maxY, System::Double% maxZ)
{
    FdoByteArray* umBuffer = ByteArrayToFdoByteArray(bytes);
    pin_ptr<FdoDouble> uminx = &minX;
    pin_ptr<FdoDouble> uminy = &minY;
    pin_ptr<FdoDouble> uminz = &minZ;
    pin_ptr<FdoDouble> umaxx = &maxX;
    pin_ptr<FdoDouble> umaxy = &maxY;
    pin_ptr<FdoDouble> umaxz = &maxZ;
    try
    {
	    EXCEPTION_HANDLER(FdoSpatialUtility::GetExtents(umBuffer, *uminx, *uminz, *uminy, *umaxx, *umaxy, *umaxz))
    }
    finally
    {
        if (umBuffer != nullptr)
            umBuffer->Release();
    }
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ ring, System::Double coordinateX, System::Double coordinateY, System::Boolean% isOnBoundary)
{
    System::Boolean ret;
	pin_ptr<FdoBoolean> uisOnBoundary = &isOnBoundary;
    FdoILinearRing* p1 = nullptr;
    if (nullptr != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::PointInRing(p1, coordinateX, coordinateY, uisOnBoundary))
	
    return ret;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ ring, System::Double coordinateX, System::Double coordinateY)
{
	System::Boolean ret;
    FdoILinearRing* p1 = nullptr;
    if (nullptr != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::PointInRing(p1, coordinateX, coordinateY, nullptr))

	return ret;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean% isOnExtBoundary, System::Boolean% isOnInBoundary)
{
	System::Boolean ret;
    FdoIPolygon* p1 = nullptr;
    if (nullptr != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }
	pin_ptr<FdoBoolean> uisOnExtBoundary = &isOnExtBoundary;
	pin_ptr<FdoBoolean> uisOnInBoundary = &isOnInBoundary;

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, uisOnExtBoundary, uisOnInBoundary))

	return ret;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean% isOnExtBoundary)
{
	System::Boolean ret;
    FdoIPolygon* p1 = nullptr;
    if (nullptr != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }

	pin_ptr<FdoBoolean> uisOnExtBoundary = &isOnExtBoundary;

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, uisOnExtBoundary, nullptr))

	return ret;
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon^ polygon, System::Double coordinateX, System::Double coordinateY)
{
	System::Boolean ret;
    FdoIPolygon* p1 = nullptr;
    if (nullptr != polygon)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::IPolygonImp^>(polygon));
        p1 = static_cast<FdoIPolygon*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::PointInPolygon(p1, coordinateX, coordinateY, nullptr, nullptr))

	return ret;
}

System::Double NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ComputeLinearRingArea( NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ ring )
{
	System::Double ret;
    FdoILinearRing* p1 = nullptr;
    if (nullptr != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::ComputeLinearRingArea(p1))

	return ret;
}

System::Double NAMESPACE_OSGEO_SPATIAL::SpatialUtility::ComputeLinearRingLength( NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ ring )
{
	System::Double ret;
    FdoILinearRing* p1 = nullptr;
    if (nullptr != ring)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ILinearRingImp^>(ring));
        p1 = static_cast<FdoILinearRing*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::ComputeLinearRingLength(p1))

	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_SPATIAL::SpatialUtility::CreateGeometryFromRings( NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection^ rings, System ::Boolean relateRings )
{
	FdoIGeometry* unobj;
	EXCEPTION_HANDLER(unobj = FdoSpatialUtility::CreateGeometryFromRings(static_cast<FdoLinearRingCollection*>(rings->UnmanagedObject.ToPointer()), relateRings))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(unobj), true);	
}

System::Boolean NAMESPACE_OSGEO_SPATIAL::SpatialUtility::IsCircularArcValid( NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment^ arc, System::Double tolerance )
{
	System::Boolean ret;
    FdoICircularArcSegment* p1 = nullptr;
    if (nullptr != arc)
    {
        NAMESPACE_OSGEO_RUNTIME::Disposable^ dis = dynamic_cast<NAMESPACE_OSGEO_RUNTIME::Disposable^>(static_cast<NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegmentImp^>(arc));
        p1 = static_cast<FdoICircularArcSegment*>(dis->UnmanagedObject.ToPointer());
    }

	EXCEPTION_HANDLER(ret = FdoSpatialUtility::IsCircularArcValid(p1, tolerance))

	return ret;
}
