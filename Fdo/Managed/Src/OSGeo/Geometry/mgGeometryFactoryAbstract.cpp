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
#include "mgGeometryFactoryAbstract.h"
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

FdoGeometryFactoryAbstract *NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::GetImpObj()
{
	return static_cast<FdoGeometryFactoryAbstract *>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::GeometryFactoryAbstract(System::IntPtr unmanaged, System::Boolean autoDelete)
	: Disposable(unmanaged, autoDelete)
{
}

System::Void NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::Dispose(System::Boolean disposing)
{
	EXCEPTION_HANDLER(GetImpObj()->Release());
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePositionXY(System::Double coordinateX, System::Double coordinateY)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePositionXY(coordinateX, coordinateY))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePositionXYZ(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePositionXYZ(coordinateX, coordinateY, coordinateZ))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePositionXYM(System::Double coordinateX, System::Double coordinateY, System::Double coordinateM)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePositionXYM(coordinateX, coordinateY, coordinateM))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition*  NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePositionXYZM(
	System::Double coordinateX, 
	System::Double coordinateY, 
	System::Double coordinateZ, 
	System::Double coordinateM)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePositionXYZM(coordinateX, coordinateY, coordinateZ, coordinateM))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePosition(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * position)
{
	FdoIDirectPosition *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePosition(static_cast<IDirectPositionImp *>(position)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIDirectPosition(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateEnvelope(
	NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * lowerLeft, 
	NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * upperRight)
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateEnvelope(static_cast<IDirectPositionImp *>(lowerLeft)->GetImpObj(), static_cast<IDirectPositionImp *>(upperRight)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateEnvelope(System::Int32 dimensionType, System::Double ordinates __gc [])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateEnvelope(dimensionType, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateEnvelopeXY(
    System::Double lowerLeftX,
    System::Double lowerLeftY,
    System::Double upperRightX, 
    System::Double upperRightY)
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateEnvelopeXY(lowerLeftX, lowerLeftY, upperRightX, upperRightY))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateEnvelopeXYZ(
    System::Double lowerLeftX,
    System::Double lowerLeftY,
    System::Double lowerLeftZ,
    System::Double upperRightX,
    System::Double upperRightY,
    System::Double upperRightZ)
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateEnvelopeXYZ(lowerLeftX, lowerLeftY, lowerLeftZ, upperRightX, upperRightY, upperRightZ))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IEnvelope* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateEnvelope(NAMESPACE_OSGEO_GEOMETRY::IEnvelope * envelope)
{
	FdoIEnvelope *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateEnvelope(static_cast<IEnvelopeImp *>(envelope)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIEnvelope(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineString* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLineString(NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection* positions)
{
	FdoILineString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineString(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineString* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLineString(System::Int32 dimensionType, System::Int32 ordinateNumber, System::Double ordinates __gc [])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoILineString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineString(dimensionType, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineString(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateGeometry(NAMESPACE_OSGEO_GEOMETRY::IGeometry * geometry)
{
	FdoIGeometry *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(static_cast<IGeometryImp *>(geometry)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateGeometry(System::String* text)
{
	FdoIGeometry *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(StringToUni(text)))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry * NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateGeometry(NAMESPACE_OSGEO_GEOMETRY::IEnvelope * envelope)
{
	FdoIGeometry *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateGeometry(static_cast<IEnvelopeImp *>(envelope)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePoint(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* position)
{
	FdoIPoint *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePoint(static_cast<IDirectPositionImp *>(position)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IPoint* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePoint(
	System::Int32 dimensionality, 
	System::Double ordinates __gc[])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoIPoint *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePoint(dimensionality, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPoint(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLinearRing(
	NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection* positions)
{
	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLinearRing(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILinearRing* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLinearRing(
    System::Int32 dimensionality, 
    System::Int32 ordinateNumber,
    System::Double ordinates __gc [])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoILinearRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLinearRing(dimensionality, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILinearRing(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLineStringSegment(
	NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection* positions)
{
	FdoILineStringSegment *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineStringSegment(positions->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateLineStringSegment(
    System::Int32 dimType,
    System::Int32 ordinateNumber,
    System::Double ordinates __gc [])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoILineStringSegment *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateLineStringSegment(dimType, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateILineStringSegment(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IPolygon* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreatePolygon(
	NAMESPACE_OSGEO_GEOMETRY::ILinearRing* exteriorRing, 
	NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection* interiorRings)
{
	FdoIPolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreatePolygon(
		static_cast<ILinearRingImp *>(exteriorRing)->GetImpObj(), 
		interiorRings->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIPolygon(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiPoint(NAMESPACE_OSGEO_GEOMETRY::PointCollection* points)
{
	FdoIMultiPoint *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPoint(points->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPoint* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiPoint(
    System::Int32 dimensionality,
    System::Int32 ordinateNumber, 
    System::Double ordinates __gc[])
{
	FdoDouble __pin *tpordinates = &ordinates[0];
	FdoIMultiPoint *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPoint(dimensionality, ordinateNumber, tpordinates))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPoint(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiLineString* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiLineString(NAMESPACE_OSGEO_GEOMETRY::LineStringCollection* lineStrings)
{
	FdoIMultiLineString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiLineString(lineStrings->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiLineString(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiPolygon(NAMESPACE_OSGEO_GEOMETRY::PolygonCollection* polygons)
{
	FdoIMultiPolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiPolygon(polygons->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiPolygon(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateCircularArcSegment(
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* startPosition,
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* midPosition,
    NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* endPosition)
{
	FdoICircularArcSegment *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCircularArcSegment(
		static_cast<IDirectPositionImp *>(startPosition)->GetImpObj(), 
		static_cast<IDirectPositionImp *>(midPosition)->GetImpObj(), 
		static_cast<IDirectPositionImp *>(endPosition)->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICircularArcSegment(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ICurveString* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateCurveString(
	NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection* curveSegments)
{
	FdoICurveString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCurveString(curveSegments->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurveString(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiCurveString(
	NAMESPACE_OSGEO_GEOMETRY::CurveStringCollection* curveStrings)
{
	FdoIMultiCurveString *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiCurveString(curveStrings->GetImpObj()))
    return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurveString(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IRing* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateRing(NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection* curveSegments)
{
	FdoIRing *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateRing(curveSegments->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIRing(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateCurvePolygon(
	NAMESPACE_OSGEO_GEOMETRY::IRing* exteriorRing,
	NAMESPACE_OSGEO_GEOMETRY::RingCollection* interiorRings)
{
	FdoICurvePolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateCurvePolygon(
		static_cast<IRingImp *>(exteriorRing)->GetImpObj(), 
		interiorRings->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateICurvePolygon(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiCurvePolygon(
	NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection* curvePolygons)
{
	FdoIMultiCurvePolygon *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiCurvePolygon(curvePolygons->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiCurvePolygon(ret, true);
}

NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry* NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract::CreateMultiGeometry(
	NAMESPACE_OSGEO_GEOMETRY::GeometryCollection* geometries)
{
	FdoIMultiGeometry *ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->CreateMultiGeometry(geometries->GetImpObj()))
	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIMultiGeometry(ret, true);
}

