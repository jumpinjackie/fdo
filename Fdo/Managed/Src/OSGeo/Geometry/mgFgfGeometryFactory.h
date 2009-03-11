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

#pragma once

#include "mgGeometryFactoryAbstract.h"

class FdoFgfGeometryFactory;
BEGIN_NAMESPACE_OSGEO_GEOMETRY
ref class CurvePolygonCollection;
ref class CurveSegmentCollection;
ref class CurveStringCollection;
ref class DirectPositionCollection;
ref class GeometryCollection;

interface class ICircularArcSegment;
interface class ICurvePolygon;
interface class ICurveString;
interface class IDirectPosition;
interface class IEnvelope;
interface class IGeometry;
interface class ILineString;
interface class ILineStringSegment;
interface class ILinearRing;
interface class IMultiCurvePolygon;
interface class IMultiCurveString;
interface class IMultiGeometry;
interface class IMultiLineString;
interface class IMultiPoint;
interface class IMultiPolygon;
interface class IPoint;
interface class IPolygon;
interface class IRing;

ref class LineStringCollection;
ref class LinearRingCollection;
ref class PointCollection;
ref class PolygonCollection;
ref class RingCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The FgfGeometryFactory class is an FGF-based Geometry factory, a concrete class that
/// implements all the members from GeometryFactoryAbstract
public ref class FgfGeometryFactory : public GeometryFactoryAbstract
{
public:
    /// \brief
    /// Constructs an FgfGeometryFactory based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	FgfGeometryFactory(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoFgfGeometryFactory* GetImpObj();
/// \endcond

public:
    /// \brief
    /// Instantiates an FgfGeometryFactory object.
    /// 
    FgfGeometryFactory();

    /// \brief
    /// Creates a Geometry from FGF data.
    /// 
    /// \param bytes 
    /// Input FGF data that defines the Geometry.
    /// 
    /// \return
    /// Returns an instance of IGeometry
    /// 
	NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometryFromFgf(array<System::Byte>^ bytes);

    /// \brief
    /// Creates a Geometry from FGF data.
    /// 
    /// \param bytes 
    /// Input FGF data that defines the Geometry.
    /// The caller must guarantee the validity of the array for the lifetime
    /// of the associated geometry object.
    /// \param count 
    /// Input Number of bytes of FGF data.
    /// 
    /// \return
    /// Returns an instance of IGeometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometryFromFgf(array<System::Byte>^ bytes, Int32 count);

    /// \brief
    /// Gets FGF data from a Geometry.
    /// 
    /// \param geometry 
    /// Input a Geometry, which must be FGF-based
    /// 
    /// \return
    /// Returns FGF data representing the Geometry
    /// 
    array<System::Byte>^ GetFgf(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry);

    /// \brief
    /// Creates an FGF-based Geometry from OpenGIS WKB data.
    /// 
    /// \param bytes 
    /// Input WKB data that defines the Geometry.
    /// 
    /// \return
    /// Returns a Geometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometryFromWkb(array<System::Byte>^ bytes);

    /// \brief
    /// Gets OpenGIS WKB data from a Geometry.
    /// 
    /// \param geometry 
    /// Input a Geometry, which must be FGF-based
    /// 
    /// \return
    /// Returns WKB data representing the Geometry
    /// 
    array<System::Byte>^ GetWkb(NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry);

    /// Methods from GeometryFactoryAbstract

    /// \brief
    /// Creates a LineString object by copying from a collection of positions.
    /// 
    /// \param positions 
    /// Input collection of positions to copy
    /// 
    /// \return
    /// Returns a LineString
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILineString^ CreateLineString(
		NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions);

    /// \brief
    /// Creates a LineString object by copying from an array of ordinates.
    /// 
    /// \param dimensionType 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinateNumber 
    /// Input total number of ordinates in the array
    /// \param ordinates 
    /// Input ordinate array representing positions
    /// 
    /// \return
    /// Returns a LineString
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILineString^ CreateLineString(
		System::Int32 dimensionType,
		System::Int32 ordinateNumber,
		array<System::Double>^ ordinates);

    /// \brief
    /// Creates a Geometry object by copying from another Geometry.
    /// 
    /// \remarks
    /// This is a pseudo-copy-constructor.
    /// It is required that any implementation use only public methods on the underlying 
    /// Geometry object, thus enabling conversion from Geometries of the same, or any
    /// other, implementation of this Geometry package.  Thus, this implementation creates
    /// FGF-based Geometry objects based on any implementation of the Geometry API.
    /// 
    /// \param geometry 
    /// Input Source geometry
    /// 
    /// \return
    /// Returns an instance of IGeometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometry(
        NAMESPACE_OSGEO_GEOMETRY::IGeometry^ geometry);

    /// \brief
    /// Creates a Geometry object by converting from a text string.
    /// 
    /// \param text 
    /// Input Source geometry
    /// 
    /// \return
    /// Returns an instance of IGeometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometry(System::String^ text);

    /// \brief
    /// Creates a Geometry object by converting from an envelope.
    /// 
    /// \remarks
    /// This method will produce a new FGF-based Geometry 
    /// based on the given envelope.
    /// 
    /// \param envelope 
    /// Input Source envelope
    /// 
    /// \return
    /// Returns a geometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IGeometry^ CreateGeometry(
        NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ envelope);

    /// Point

    /// \brief
    /// Creates a point object by copying from a position.
    /// 
    /// \param position 
    /// Input position to copy
    /// 
    /// \return
    /// Returns a point
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IPoint^ CreatePoint(
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ position);

    /// \brief
    /// Creates an point object by copying from an array of ordinates.
    /// 
    /// \param dimensionality 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinates 
    /// Input ordinate array representing one position
    /// 
    /// \return
    /// Returns a point
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IPoint^ CreatePoint(
		System::Int32 dimensionality,
		array<System::Double>^ ordinates);

    /// LinearRing

    /// \brief
    /// Creates a LinearRing object by copying from a collection of positions.
    /// 
    /// \remarks
    /// The first and last positions must be equal.
    /// 
    /// \param positions 
    /// Input collection of positions to copy
    /// 
    /// \return
    /// Returns a LinearRing
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ CreateLinearRing(
        NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions);

    /// \brief
    /// Creates a LinearRing object by copying from an array of ordinates.
    /// 
    /// \remarks
    /// The ordinates representing the first and last positions must be equal.
    /// 
    /// \param dimensionality 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinateNumber 
    /// Input total number of ordinates in the array
    /// \param ordinates 
    /// Input ordinate array representing positions
    /// 
    /// \return
    /// Returns a LinearRing
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ CreateLinearRing(
		System::Int32 dimensionality,
		System::Int32 ordinateNumber,
		array<System::Double>^ ordinates);

    /// LineStringSegment

    /// \brief
    /// Creates a LineStringSegment object by copying from a collection of positions.
    /// 
    /// \param positions 
    /// Input collection of positions to copy
    /// 
    /// \return
    /// Returns a LineStringSegment
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment^ CreateLineStringSegment(
        NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ positions);

    /// \brief
    /// Creates a LineStringSegment object by copying from an array of ordinates.
    /// 
    /// \param dimType 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinateNumber 
    /// Input total number of ordinates in the array
    /// \param ordinates 
    /// Input ordinate array representing positions
    /// 
    /// \return
    /// Returns a LineStringSegment
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ILineStringSegment^ CreateLineStringSegment(
		System::Int32 dimType,
		System::Int32 ordinateNumber, 
		array<System::Double>^ ordinates);

    /// Polygon

    /// \brief
    /// Creates a polygon object by copying from given LinearRing objects.
    /// 
    /// \param exteriorRing 
    /// Input exterior ring to copy
    /// \param interiorRings 
    /// Input collection of interior rings to copy
    /// 
    /// \return
    /// Returns a polygon
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IPolygon^ CreatePolygon(
        NAMESPACE_OSGEO_GEOMETRY::ILinearRing^ exteriorRing, 
        NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection^ interiorRings);

    /// MultiPoint

    /// \brief
    /// Creates a MultiPoint object by copying from a collection of point objects.
    /// 
    /// \param points 
    /// Input collection of point objects to copy
    /// 
    /// \return
    /// Returns a MultiPoint
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiPoint^ CreateMultiPoint(
        NAMESPACE_OSGEO_GEOMETRY::PointCollection^ points);

    /// \brief
    /// Creates a MultiPoint object by copying from an array of ordinates.
    /// 
    /// \param dimensionality 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinateNumber 
    /// Input total number of ordinates in the array
    /// \param ordinates 
    /// Input ordinate array representing positions
    /// 
    /// \return
    /// Returns a MultiPoint
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiPoint^ CreateMultiPoint(
		System::Int32 dimensionality,
		System::Int32 ordinateNumber, 
		array<System::Double>^ ordinates);

    /// MultiLineString

    /// \brief
    /// Creates a MultiLineString object by copying from a collection of LineString objects.
    /// 
    /// \param lineStrings 
    /// Input collection of LineString objects to copy
    /// 
    /// \return
    /// Returns a MultiLineString
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiLineString^ CreateMultiLineString(
        NAMESPACE_OSGEO_GEOMETRY::LineStringCollection^ lineStrings);

    //MultiPolygon

    /// \brief
    /// Creates a MultiPolygon object by copying from a collection of polygon objects.
    /// 
    /// \param polygons 
    /// Input collection of polygon objects to copy
    /// 
    /// \return
    /// Returns a MultiPolygon
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiPolygon^ CreateMultiPolygon(
        NAMESPACE_OSGEO_GEOMETRY::PolygonCollection^ polygons);

    /// CircularArcSegment

    /// \brief
    /// Creates a CircularArcSegment object by copying from three points on the arc.
    /// 
    /// \param startPosition 
    /// Input starting position of the arc
    /// \param midPosition 
    /// Input a position on the arc, and not equal to the starting or ending positions
    /// \param endPosition 
    /// Input ending position of the arc
    /// 
    /// \return
    /// Returns a CircularArcSegment
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment^ CreateCircularArcSegment(
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ startPosition,
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ midPosition,
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ endPosition);

    /// CurveString

    /// \brief
    /// Creates a CurveString object by copying from a collection of CurveSegment objects.
    /// 
    /// \param curveSegments 
    /// Input collection of CurveSegment objects to copy
    /// 
    /// \return
    /// Returns a CurveString
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ICurveString^ CreateCurveString(
        NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ curveSegments);

    /// MultiCurveString

    /// \brief
    /// Creates a MultiCurveString object by copying from a collection of CurveString objects.
    /// 
    /// \param curveStrings 
    /// Input collection of CurveString objects to copy
    /// 
    /// \return
    /// Returns a MultiCurveString
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiCurveString^ CreateMultiCurveString(
        NAMESPACE_OSGEO_GEOMETRY::CurveStringCollection^ curveStrings);

    /// Ring

    /// \brief
    /// Creates a ring object by copying from a collection of CurveSegment objects.
    /// 
    /// \param curveSegments 
    /// Input collection of CurveSegment objects to copy
    /// 
    /// \return
    /// Returns a Ring
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IRing^ CreateRing(
        NAMESPACE_OSGEO_GEOMETRY::CurveSegmentCollection^ curveSegments);

    /// CurvePolygon

    /// \brief
    /// Creates a CurvePolygon object by copying from given ring objects.
    /// 
    /// \param exteriorRing 
    /// Input exterior ring to copy
    /// \param interiorRings 
    /// Input collection of interior rings to copy
    /// 
    /// \return
    /// Returns a CurvePolygon
    /// 
    NAMESPACE_OSGEO_GEOMETRY::ICurvePolygon^ CreateCurvePolygon(
        NAMESPACE_OSGEO_GEOMETRY::IRing^ exteriorRing,
        NAMESPACE_OSGEO_GEOMETRY::RingCollection^ interiorRings);

    /// MultiCurvePolygon

    /// \brief
    /// Creates a MultiCurvePolygon object by copying from a collection of CurvePolygon objects.
    /// 
    /// \param curvePolygons 
    /// Input collection of CurvePolygon objects to copy
    /// 
    /// \return
    /// Returns a MultiCurvePolygon
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiCurvePolygon^ CreateMultiCurvePolygon(
        NAMESPACE_OSGEO_GEOMETRY::CurvePolygonCollection^ curvePolygons);

    /// MultiGeometry

    /// \brief
    /// Creates a MultiGeometry object by copying from a collection of Geometry objects.
    /// 
    /// \param geometries 
    /// Input collection of Geometry objects to copy
    /// 
    /// \return
    /// Returns a MultiGeometry
    /// 
    NAMESPACE_OSGEO_GEOMETRY::IMultiGeometry^ CreateMultiGeometry(
        NAMESPACE_OSGEO_GEOMETRY::GeometryCollection^ geometries);
};

END_NAMESPACE_OSGEO_GEOMETRY


