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

private:
	/// <summary>Gets the coordinates of the FdoIDirectPosition object.</summary>
	/// <param name="position">An instance of FdoIDirectPosition type.</param>
	/// <returns>Returns the corrdinates as string.</returns>
	static FdoStringP GetDirectPositionCoordinates( FdoIDirectPosition* position,FdoGmlVersion gmlversion);

};

#endif


