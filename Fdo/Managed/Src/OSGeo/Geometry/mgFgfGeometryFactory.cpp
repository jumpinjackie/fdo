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
#include "mgFgfGeometryFactory.h"
#include <FdoGeometry.h>

#include "mgIArcSegmentAbstractImp.h"
#include "mgICircularArcSegmentImp.h"
#include "mgICurveAbstractImp.h"
#include "mgICurvePolygonImp.h"
#include "mgICurveSegmentAbstractImp.h"
#include "mgICurveStringImp.h"
#include "mgIDirectPositionImp.h"
#include "mgIEnvelopeImp.h"
#include "mgIGeometricAggregateAbstractImp.h"
#include "mgIGeometryImp.h"
#include "mgILineStringImp.h"
#include "mgILineStringSegmentImp.h"
#include "mgILinearRingImp.h"
#include "mgIMultiCurvePolygonImp.h"
#include "mgIMultiCurveStringImp.h"
#include "mgIMultiGeometryImp.h"
#include "mgIMultiLineStringImp.h"
#include "mgIMultiPointImp.h"
#include "mgIMultiPolygonImp.h"
#include "mgIPointImp.h"
#include "mgIPolygonImp.h"
#include "mgIRingAbstractImp.h"
#include "mgIRingImp.h"
#include "mgISurfaceAbstractImp.h"

#include "mgObjectFactory.h"

FdoFgfGeometryFactory* NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::GetImpObj()
{
	return static_cast<FdoFgfGeometryFactory*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::FgfGeometryFactory(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::FgfGeometryFactory()
	: NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract(IntPtr(FdoFgfGeometryFactory::GetInstance()), true)
{
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometryFromFgf(array<System::Byte>^ bytes)
{
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ ret;
	FdoByteArray* arr = ByteArrayToFdoByteArray(bytes);
    try
    {
	    FdoIGeometry* temp;
	    EXCEPTION_HANDLER(temp = GetImpObj()->CreateGeometryFromFgf(arr))
        ret = NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(temp), true);
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return ret;

}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometryFromFgf(array<System::Byte>^ bytes, Int32 count)
{
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ ret;
	FdoByteArray* arr = ByteArrayToFdoByteArray(bytes);
    try
    {
	    FdoIGeometry* temp;
	    EXCEPTION_HANDLER(temp = GetImpObj()->CreateGeometryFromFgf(arr->GetData(), count))
	    ret = NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(temp), true);
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return ret;
}

array<System::Byte>^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::GetFgf(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry)
{
    array<System::Byte>^ ret;
	FdoByteArray* arr = nullptr;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetFgf(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(geometry)->GetImpObj()))
	    ret = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return ret;
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometryFromWkb(array<System::Byte>^ bytes)
{
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ ret;
	FdoByteArray* arr = ByteArrayToFdoByteArray(bytes);
    try
    {
	    FdoIGeometry* temp;
	    EXCEPTION_HANDLER(temp = GetImpObj()->CreateGeometryFromWkb(arr))
	    ret = NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(temp), true);
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return ret;
}

array<System::Byte>^  NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::GetWkb(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry)
{
    array<System::Byte>^ ret;
	FdoByteArray* arr = nullptr;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetWkb(static_cast<NAMESPACE_OSGEO_GEOMETRY::IGeometryImp^>(geometry)->GetImpObj()))
	    ret = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return ret;

}

NAMESPACE_OSGEO_GEOMETRY::ILineString^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLineString(
	NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions)
{
	FdoILineString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineString(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineString^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLineString(
	System::Int32 dimensionType,
	System::Int32 ordinateNumber,
	array<System::Double>^ ordinates)
{
	pin_ptr<FdoDouble> tpordinates = &ordinates[0];
	FdoILineString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineString(dimensionType, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometry(
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry)
{
	FdoIGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(static_cast<IGeometryImp^>(geometry)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometry(
	System::String^ text)
{
	FdoIGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(StringToUni(text)))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateGeometry(
    NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ envelope)
{
	FdoIGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(static_cast<IEnvelopeImp^>(envelope)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreatePoint(
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ position)
{
	FdoIPoint* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePoint(static_cast<IDirectPositionImp^>(position)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreatePoint(
	System::Int32 dimensionality,
	array<System::Double>^ ordinates)
{
	pin_ptr<FdoDouble> tpordinates = &ordinates[0];

	FdoIPoint* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePoint(dimensionality, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLinearRing(
    NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions)
{
	FdoILinearRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLinearRing(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLinearRing(
	System::Int32 dimensionality,
	System::Int32 ordinateNumber,
	array<System::Double>^ ordinates)
{
	pin_ptr<FdoDouble> tpordinates = &ordinates[0];
	FdoILinearRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLinearRing(dimensionality, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLineStringSegment(
    NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions)
{
	FdoILineStringSegment* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineStringSegment(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateLineStringSegment(
	System::Int32 dimType,
	System::Int32 ordinateNumber, 
	array<System::Double>^ ordinates)
{
	pin_ptr<FdoDouble> tpordinates = &ordinates[0];
	FdoILineStringSegment* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineStringSegment(dimType, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreatePolygon(
    NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ exteriorRing, 
    NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection^ interiorRings)
{
	FdoIPolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePolygon(		
		static_cast<ILinearRingImp^>(exteriorRing)->GetImpObj(), 
		(interiorRings == nullptr) ? (nullptr) : (interiorRings->GetImpObj())))

	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiPoint(
    NAMESPACE_OSGEO_GEOMETRY::PointCollection^ points)
{
	FdoIMultiPoint* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPoint(points->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiPoint(
	System::Int32 dimensionality,
	System::Int32 ordinateNumber, 
	array<System::Double>^ ordinates)
{
	pin_ptr<FdoDouble> tpordinates = &ordinates[0];
	FdoIMultiPoint* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPoint(dimensionality, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiLineString^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiLineString(
    NAMESPACE_OSGEO_GEOMETRY::LineStringCollection^ lineStrings)
{
	FdoIMultiLineString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiLineString(lineStrings->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiLineString(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiPolygon(
    NAMESPACE_OSGEO_GEOMETRY::PolygonCollection^ polygons)
{
	FdoIMultiPolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPolygon(polygons->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPolygon(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateCircularArcSegment(
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ startPosition,
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ midPosition,
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ endPosition)
{
	FdoICircularArcSegment* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCircularArcSegment(
		static_cast<IDirectPositionImp^>(startPosition)->GetImpObj(), 
		static_cast<IDirectPositionImp^>(midPosition)->GetImpObj(), 
		static_cast<IDirectPositionImp^>(endPosition)->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICircularArcSegment(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ICurveString^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateCurveString(
    NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ curveSegments)
{
	FdoICurveString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCurveString(curveSegments->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveString(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiCurveString(
    NAMESPACE_OSGEO_GEOMETRY::CurveStringCollection^ curveStrings)
{
	FdoIMultiCurveString* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiCurveString(curveStrings->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurveString(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IRing^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateRing(
    NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ curveSegments)
{
	FdoIRing* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateRing(curveSegments->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateCurvePolygon(
    NAMESPACE_OSGEO_GEOMETRY::IRing^ exteriorRing,
    NAMESPACE_OSGEO_GEOMETRY::RingCollection^ interiorRings)
{
	FdoICurvePolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCurvePolygon(
		static_cast<IRingImp^>(exteriorRing)->GetImpObj(), interiorRings->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiCurvePolygon(
    NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection^ curvePolygons)
{
	FdoIMultiCurvePolygon* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiCurvePolygon(curvePolygons->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurvePolygon(IntPtr(ret), true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry^ NAMESPACE_OSGEO_GEOMETRY::FgfGeometryFactory::CreateMultiGeometry(
    NAMESPACE_OSGEO_GEOMETRY::GeometryCollection^ geometries)
{
	FdoIMultiGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiGeometry(geometries->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiGeometry(IntPtr(ret), true);
}

