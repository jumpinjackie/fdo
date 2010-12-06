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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include "GeometrySerializer.h"

void FdoGeometrySerializer::SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeGeometry(geometry, writer, srsName);
}

void FdoGeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializePoint(point, writer);
}

void FdoGeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeLineString(lineString, writer);
}

void FdoGeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeLinearRing(linearRing, writer);
}

void FdoGeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializePolygon(polygon, writer, srsName);
}

void FdoGeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeMultiPoint(multiPoint, writer, srsName);
}

void FdoGeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeMultiLineString(mlString, writer);
}

void FdoGeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeMultiPolygon(mPolygon, writer, srsName);
}

void FdoGeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    FdoGMLGeometrySerializerP gmlGeometrySerializer = FdoGMLGeometrySerializer::Create(gmlversion);
    gmlGeometrySerializer->SerializeMultiGeometry(mGeometry, writer, srsName);
}

///
/// class FdoGMLGeometrySerializer /////////////////////////////////////////////////
FdoGMLGeometrySerializer* FdoGMLGeometrySerializer::Create(FdoGmlVersion gmlversion)
{
    switch (gmlversion)
    {
    case FdoGmlVersion_212:
        return FdoGML212GeometrySerializer::Create();
    case FdoGmlVersion_311:
        return FdoGML311GeometrySerializer::Create();
    }

    throw FdoException::Create(L"Unsupported GML version.");
    
    // Can't reach here.
    return NULL;
}


///
/// class FdoGML212GeometrySerializer ///////////////////////////////////////////////
FdoGMLGeometrySerializer* FdoGML212GeometrySerializer::Create()
{
    return new FdoGML212GeometrySerializer();
}

void FdoGML212GeometrySerializer::SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName)
{
    FdoGeometryType geoType = geometry->GetDerivedType();
    switch(geoType)
    {
    case FdoGeometryType_Point:			
        SerializePoint((FdoIPoint*)geometry, writer);			
        break;
    case FdoGeometryType_LineString:
        SerializeLineString((FdoILineString*)geometry, writer);			
        break;
    case FdoGeometryType_Polygon:
        SerializePolygon((FdoIPolygon*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiPoint:
        SerializeMultiPoint((FdoIMultiPoint*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiLineString:
        SerializeMultiLineString((FdoIMultiLineString*)geometry, writer);
        break;
    case FdoGeometryType_MultiPolygon:
        SerializeMultiPolygon((FdoIMultiPolygon*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiGeometry:
        SerializeMultiGeometry((FdoIMultiGeometry*)geometry, writer, srsName);
        break;

    case FdoGeometryType_CurveString:
    case FdoGeometryType_CurvePolygon:
    case FdoGeometryType_MultiCurveString:
    case FdoGeometryType_MultiCurvePolygon:
        // TODO: what to do?
        throw FdoException::Create(L"Unsupported geometry types");
        break;
    }
}

FdoStringP FdoGML212GeometrySerializer::GetDirectPositionCoordinates(FdoIDirectPosition* position)
{
    static const int SIZE = 256;
    wchar_t temp[SIZE];
    swprintf(temp, SIZE - 1, L"%lf,%lf", position->GetX(), position->GetY());
    FdoStringP ret = temp;

    FdoInt32 dimension = position->GetDimensionality();
    if (dimension & FdoDimensionality_Z)
    {
        swprintf(temp, SIZE - 1, L",%lf", position->GetZ());
        ret += temp;
    }

    if (dimension & FdoDimensionality_M)
    {
        swprintf(temp, SIZE - 1, L",%lf", position->GetM());
        ret += temp;
    }

    return ret;
}

void FdoGML212GeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer)
{	
    FdoPtr<FdoIDirectPosition> pos = point->GetPosition ();	
    writer->WriteStartElement(L"gml:Point");
    writer->WriteStartElement(L"gml:coordinates");	

    writer->WriteCharacters(GetDirectPositionCoordinates(pos));

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer)
{
    writer->WriteStartElement(L"gml:LineString");
    writer->WriteStartElement(L"gml:coordinates");

    FdoInt32 cnt = lineString->GetCount();
    // The coordinates are separated by a blank(" ").
    if (cnt > 0)
    {
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(0);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));
    }
    for (FdoInt32 i=1; i<cnt; i++)
    {
        writer->WriteCharacters(L" ");
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));		
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer)
{
    FdoInt32 cntPos = linearRing->GetCount();
    if (cntPos == 0)
        return;

    writer->WriteStartElement(L"gml:LinearRing");
    writer->WriteStartElement(L"gml:coordinates");

    for (FdoInt32 i=0; i<cntPos; i++)
    {
        FdoPtr<FdoIDirectPosition> pos = linearRing->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));
        if (i < cntPos-1)
            writer->WriteCharacters(L" ");
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:Polygon");
    writer->WriteAttribute(L"srsName", srsName);

    // serialzie the exterior ring
    FdoPtr<FdoILinearRing> outer = polygon->GetExteriorRing();
    writer->WriteStartElement(L"gml:outerBoundaryIs");
    SerializeLinearRing(outer, writer);
    writer->WriteEndElement();

    FdoInt32 cntRing = polygon->GetInteriorRingCount();	
    for (FdoInt32 i=0; i<cntRing; i++)
    {
        FdoPtr<FdoILinearRing> inner = polygon->GetInteriorRing(i);
        writer->WriteStartElement(L"gml:innerBoundaryIs");
        SerializeLinearRing(inner, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiPoint");
    writer->WriteAttribute(L"srsName", srsName);

    FdoInt32 cntPoint = multiPoint->GetCount();
    for (FdoInt32 i=0; i<cntPoint; i++)
    {
        writer->WriteStartElement(L"gml:pointMember"); // pointMember appeared with each point element
        FdoPtr<FdoIPoint> point = multiPoint->GetItem(i);
        SerializePoint(point, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer)
{
    writer->WriteStartElement(L"gml:MultiLineString");

    FdoInt32 cntLineString = mlString->GetCount();
    for (FdoInt32 i=0; i<cntLineString; i++)
    {
        FdoPtr<FdoILineString> lString = mlString->GetItem(i);
        writer->WriteStartElement(L"gml:lineStringMember");
        SerializeLineString(lString, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiPolygon");

    FdoInt32 cntPolygon = mPolygon->GetCount();
    for (FdoInt32 i=0; i<cntPolygon; i++)
    {
        FdoPtr<FdoIPolygon> polygon = mPolygon->GetItem(i);
        writer->WriteStartElement(L"gml:polygonMember");
        SerializePolygon(polygon, writer, srsName);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML212GeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiGeometry");

    FdoInt32 cntGeometry = mGeometry->GetCount();
    for (FdoInt32 i=0; i<cntGeometry; i++)
    {
        FdoPtr<FdoIGeometry> geometry = mGeometry->GetItem(i);

        writer->WriteStartElement(L"gml:geometryMember");
        SerializeGeometry(geometry, writer, srsName);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}


///
/// class FdoGML311GeometrySerializer ///////////////////////////////////////////////
FdoGMLGeometrySerializer* FdoGML311GeometrySerializer::Create()
{
    return new FdoGML311GeometrySerializer();
}

void FdoGML311GeometrySerializer::SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName)
{
    FdoGeometryType geoType = geometry->GetDerivedType();
    switch(geoType)
    {
    case FdoGeometryType_Point:			
        SerializePoint((FdoIPoint*)geometry, writer);			
        break;
    case FdoGeometryType_LineString:
        SerializeLineString((FdoILineString*)geometry, writer);			
        break;
    case FdoGeometryType_Polygon:
        SerializePolygon((FdoIPolygon*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiPoint:
        SerializeMultiPoint((FdoIMultiPoint*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiLineString:
        SerializeMultiLineString((FdoIMultiLineString*)geometry, writer);
        break;
    case FdoGeometryType_MultiPolygon:
        SerializeMultiPolygon((FdoIMultiPolygon*)geometry, writer, srsName);
        break;
    case FdoGeometryType_MultiGeometry:
        SerializeMultiGeometry((FdoIMultiGeometry*)geometry, writer, srsName);
        break;

    case FdoGeometryType_CurveString:
    case FdoGeometryType_CurvePolygon:
    case FdoGeometryType_MultiCurveString:
    case FdoGeometryType_MultiCurvePolygon:
        // TODO: what to do?
        throw FdoException::Create(L"Unsupported geometry types");
        break;
    }
}

FdoStringP FdoGML311GeometrySerializer::GetDirectPositionCoordinates(FdoIDirectPosition* position)
{
    static const int SIZE = 256;
    wchar_t temp[SIZE];
    swprintf(temp, SIZE - 1, L"%lf %lf", position->GetX(), position->GetY());
    FdoStringP ret = temp;

    FdoInt32 dimension = position->GetDimensionality();
    if (dimension & FdoDimensionality_Z)
    {
        swprintf(temp, SIZE - 1, L" %lf", position->GetZ());
        ret += temp;
    }

    if (dimension & FdoDimensionality_M)
    {
        swprintf(temp, SIZE - 1, L" %lf", position->GetM());
        ret += temp;
    }

    return ret;
}

void FdoGML311GeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer)
{	
    FdoPtr<FdoIDirectPosition> pos = point->GetPosition ();	
    writer->WriteStartElement(L"gml:Point");
    writer->WriteStartElement(L"gml:pos"); //"coordinates" is deprecated with GML version 3.1.0. Use "pos" instead.

    writer->WriteCharacters(GetDirectPositionCoordinates(pos));

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer)
{
    writer->WriteStartElement(L"gml:Curve"); // Write gml:Curve instead of gml:LineString elements. 
    writer->WriteStartElement(L"gml:segments");
    writer->WriteStartElement(L"gml:LineStringSegment");
    writer->WriteStartElement(L"gml:posList"); //"coordinates" is deprecated with GML version 3.1.1. Use "posList" instead.

    FdoInt32 cnt = lineString->GetCount();
    // The coordinates are separated by a blank(" ").
    if (cnt > 0)
    {
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(0);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));
    }
    for (FdoInt32 i=1; i<cnt; i++)
    {
        writer->WriteCharacters(L" ");
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));		
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer)
{
    FdoInt32 cntPos = linearRing->GetCount();
    if (cntPos == 0)
        return;

    writer->WriteStartElement(L"gml:LinearRing");
    writer->WriteStartElement(L"gml:posList");

    for (FdoInt32 i=0; i<cntPos; i++)
    {
        FdoPtr<FdoIDirectPosition> pos = linearRing->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos));
        if (i < cntPos-1)
            writer->WriteCharacters(L" ");
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:Surface"); //Write gml:Surface instead of gml:Polygon elements. 
    writer->WriteAttribute(L"srsName", srsName);
    
    writer->WriteStartElement(L"gml:patches");
    writer->WriteStartElement(L"gml:PolygonPatch");

    // serialzie the exterior ring
    FdoPtr<FdoILinearRing> outer = polygon->GetExteriorRing();
    writer->WriteStartElement(L"gml:exterior");
    SerializeLinearRing(outer, writer);
    writer->WriteEndElement();

    FdoInt32 cntRing = polygon->GetInteriorRingCount();	
    for (FdoInt32 i=0; i<cntRing; i++)
    {
        FdoPtr<FdoILinearRing> inner = polygon->GetInteriorRing(i);
        writer->WriteStartElement(L"gml:interior");
        SerializeLinearRing(inner, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiPoint");
    writer->WriteAttribute(L"srsName", srsName);

    FdoInt32 cntPoint = multiPoint->GetCount();
    for (FdoInt32 i=0; i<cntPoint; i++)
    {
        writer->WriteStartElement(L"gml:pointMember"); // pointMember appeared with each point element
        FdoPtr<FdoIPoint> point = multiPoint->GetItem(i);
        SerializePoint(point, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer)
{
    writer->WriteStartElement(L"gml:MultiCurve"); //Write gml:MultiCurve instead of gml:MultiLineString. 

    FdoInt32 cntLineString = mlString->GetCount();
    for (FdoInt32 i=0; i<cntLineString; i++)
    {
        FdoPtr<FdoILineString> lString = mlString->GetItem(i);

        writer->WriteStartElement(L"gml:curveMember");
        SerializeLineString(lString, writer);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiSurface"); //Write gml:MultiSurface instead of gml:MultiPolygon. 

    FdoInt32 cntPolygon = mPolygon->GetCount();
    for (FdoInt32 i=0; i<cntPolygon; i++)
    {
        FdoPtr<FdoIPolygon> polygon = mPolygon->GetItem(i);

        writer->WriteStartElement(L"gml:surfaceMember");
        SerializePolygon(polygon, writer, srsName);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

void FdoGML311GeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName)
{
    writer->WriteStartElement(L"gml:MultiGeometry");

    FdoInt32 cntGeometry = mGeometry->GetCount();
    for (FdoInt32 i=0; i<cntGeometry; i++)
    {
        FdoPtr<FdoIGeometry> geometry = mGeometry->GetItem(i);

        writer->WriteStartElement(L"gml:geometryMember");
        SerializeGeometry(geometry, writer, srsName);
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}

