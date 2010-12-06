//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef FDO_XML_GEOMETRYSERIALIZER_H
#define FDO_XML_GEOMETRYSERIALIZER_H

#ifdef _WIN32
#pragma once
#endif

/// <summary>
/// This static class serializes geometry objects to GML formats.
/// </summary>
class FdoGeometrySerializer
{
public:
	/// <summary>Serializes the geometry object to OGC format.</summary>
	/// <param name="geometry">An instance of geometry.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the geometry.</param>
	/// <param name="srsName">SRS name.</param>
	/// <param name="version">GML version.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion);

	/// <summary>Serialzies the point object to OGC format.</summary>
	/// <param name="point">An instance of the point type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the point.</param>
	/// <returns>Returns nothing</returns>
	static void SerializePoint(FdoIPoint* point, FdoXmlWriter* writer,FdoGmlVersion gmlversion);

	/// <summary>Serializes the line string object to OGC format.</summary>
	/// <param name="lineString">An instance of the line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the line string.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer,FdoGmlVersion gmlversion);

	/// <summary>Serializes the linear ring object to OGC format.</summary>
	/// <param name="linearRing">An instance of the linear ring type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the linear ring.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer,FdoGmlVersion gmlversion);

	/// <summary>Serializes the polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the polygon.</param>
	/// <returns>Returns nothing</returns>
	static void SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion);
	
	/// <summary>Serializes the multipoint object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multipoint type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multipoint.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion);
	
	/// <summary>Serializes the multi line string object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi line string.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer,FdoGmlVersion gmlversion);
	
	/// <summary>Serializes the multi polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi polygon.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeMultiPolygon(FdoIMultiPolygon*, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion);
	
	/// <summary>Serializes the multi geometry object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi geometry type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi geometry.</param>
	/// <returns>Returns nothing</returns>
	static void SerializeMultiGeometry(FdoIMultiGeometry*, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion);
};

// The abstract GML geometry serializer 
class FdoGMLGeometrySerializer : public FdoIDisposable
{
public:
    /// <summary>Create the serializer by the input GML_version.</summary>
	/// <param name="version">GML version.</param>
	/// <returns>Returns the created GML geometry serializer</returns>
	static FdoGMLGeometrySerializer* Create(FdoGmlVersion gmlversion);

    /// <summary>Serializes the geometry object to OGC format.</summary>
	/// <param name="geometry">An instance of geometry.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the geometry.</param>
	/// <param name="srsName">SRS name.</param>
	/// <param name="version">GML version.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName) = 0;

	/// <summary>Serialzies the point object to OGC format.</summary>
	/// <param name="point">An instance of the point type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the point.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePoint(FdoIPoint* point, FdoXmlWriter* writer) = 0;

	/// <summary>Serializes the line string object to OGC format.</summary>
	/// <param name="lineString">An instance of the line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer) = 0;

	/// <summary>Serializes the linear ring object to OGC format.</summary>
	/// <param name="linearRing">An instance of the linear ring type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the linear ring.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer) = 0;

	/// <summary>Serializes the polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName) = 0;
	
	/// <summary>Serializes the multipoint object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multipoint type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multipoint.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName) = 0;
	
	/// <summary>Serializes the multi line string object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer) = 0;
	
	/// <summary>Serializes the multi polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPolygon(FdoIMultiPolygon*, FdoXmlWriter* writer, FdoString* srsName) = 0;
	
	/// <summary>Serializes the multi geometry object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi geometry type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi geometry.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiGeometry(FdoIMultiGeometry*, FdoXmlWriter* writer, FdoString* srsName) = 0;

    /// Default disposal.
    virtual void Dispose()
    {
        delete this;
    };

private:
	/// <summary>Gets the coordinates of the FdoIDirectPosition object.</summary>
	/// <param name="position">An instance of FdoIDirectPosition type.</param>
	/// <returns>Returns the corrdinates as string.</returns>
	FdoStringP GetDirectPositionCoordinates( FdoIDirectPosition* position,FdoGmlVersion gmlversion);
};
// typedef FdoGMLGeometrySerializerP.
typedef FdoPtr<FdoGMLGeometrySerializer> FdoGMLGeometrySerializerP;

// The geometry serializer for GML 2.1.2
class FdoGML212GeometrySerializer : public FdoGMLGeometrySerializer
{
public:
    /// <summary>Create the geometry serializer fro GML 2.1.2.</summary>
    /// <param name="version">GML version.</param>
    /// <returns>Returns the created GML geometry serializer</returns>
    static FdoGMLGeometrySerializer* Create();

    /// <summary>Serializes the geometry object to OGC format.</summary>
	/// <param name="geometry">An instance of geometry.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the geometry.</param>
	/// <param name="srsName">SRS name.</param>
	/// <param name="version">GML version.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName);

	/// <summary>Serialzies the point object to OGC format.</summary>
	/// <param name="point">An instance of the point type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the point.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePoint(FdoIPoint* point, FdoXmlWriter* writer);

	/// <summary>Serializes the line string object to OGC format.</summary>
	/// <param name="lineString">An instance of the line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer);

	/// <summary>Serializes the linear ring object to OGC format.</summary>
	/// <param name="linearRing">An instance of the linear ring type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the linear ring.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer);

	/// <summary>Serializes the polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multipoint object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multipoint type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multipoint.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multi line string object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer);
	
	/// <summary>Serializes the multi polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPolygon(FdoIMultiPolygon*, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multi geometry object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi geometry type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi geometry.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiGeometry(FdoIMultiGeometry*, FdoXmlWriter* writer, FdoString* srsName);

private:
	/// <summary>Gets the coordinates of the FdoIDirectPosition object.</summary>
	/// <param name="position">An instance of FdoIDirectPosition type.</param>
	/// <returns>Returns the corrdinates as string.</returns>
	FdoStringP GetDirectPositionCoordinates( FdoIDirectPosition* position);

    /// Suppress create the object of this type from outside.
    FdoGML212GeometrySerializer(){};

    /// Suppress the copy constructor of this type.
    FdoGML212GeometrySerializer(const FdoGML212GeometrySerializer& serializer){};

    // Suppress the assignment operator of this type.
    FdoGML212GeometrySerializer& operator = (const FdoGML212GeometrySerializer& serializer){return *this;};
};
// typedef FdoGML212GeometrySerializerP.
typedef FdoPtr<FdoGML212GeometrySerializer> FdoGML212GeometrySerializerP;

// The geometry serializer for GML 3.1.1
class FdoGML311GeometrySerializer : public FdoGMLGeometrySerializer
{
public:
    /// <summary>Create the geometry serializer fro GML 3.1.1.</summary>
    /// <param name="version">GML version.</param>
    /// <returns>Returns the created GML geometry serializer</returns>
    static FdoGMLGeometrySerializer* Create();

    /// <summary>Serializes the geometry object to OGC format.</summary>
	/// <param name="geometry">An instance of geometry.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the geometry.</param>
	/// <param name="srsName">SRS name.</param>
	/// <param name="version">GML version.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName);

	/// <summary>Serialzies the point object to OGC format.</summary>
	/// <param name="point">An instance of the point type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the point.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePoint(FdoIPoint* point, FdoXmlWriter* writer);

	/// <summary>Serializes the line string object to OGC format.</summary>
	/// <param name="lineString">An instance of the line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer);

	/// <summary>Serializes the linear ring object to OGC format.</summary>
	/// <param name="linearRing">An instance of the linear ring type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the linear ring.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer);

	/// <summary>Serializes the polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multipoint object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multipoint type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multipoint.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multi line string object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi line string type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi line string.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer);
	
	/// <summary>Serializes the multi polygon object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi polygon type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi polygon.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiPolygon(FdoIMultiPolygon*, FdoXmlWriter* writer, FdoString* srsName);
	
	/// <summary>Serializes the multi geometry object to OGC format.</summary>
	/// <param name="linearRing">An instance of the multi geometry type.</param>
	/// <param name="writer">An instance of FdoXmlWriter to serialize the multi geometry.</param>
	/// <returns>Returns nothing</returns>
	virtual void SerializeMultiGeometry(FdoIMultiGeometry*, FdoXmlWriter* writer, FdoString* srsName);

private:
	/// <summary>Gets the coordinates of the FdoIDirectPosition object.</summary>
	/// <param name="position">An instance of FdoIDirectPosition type.</param>
	/// <returns>Returns the corrdinates as string.</returns>
	FdoStringP GetDirectPositionCoordinates( FdoIDirectPosition* position);

    /// Suppress create the object of this type from outside.
    FdoGML311GeometrySerializer(){};

    /// Suppress the copy constructor of this type.
    FdoGML311GeometrySerializer(const FdoGML311GeometrySerializer& serializer){};

    // Suppress the assignment operator of this type.
    FdoGML311GeometrySerializer& operator = (const FdoGML311GeometrySerializer& serializer){return *this;};
};
// typedef FdoGML311GeometrySerializerP.
typedef FdoPtr<FdoGML311GeometrySerializer> FdoGML311GeometrySerializerP;

#endif

