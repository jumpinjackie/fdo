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
    FdoGeometryType geoType = geometry->GetDerivedType();
    switch(geoType)
    {
    case FdoGeometryType_Point:			
        FdoGeometrySerializer::SerializePoint((FdoIPoint*)geometry, writer,gmlversion);			
        break;
    case FdoGeometryType_LineString:
        FdoGeometrySerializer::SerializeLineString((FdoILineString*)geometry, writer,gmlversion);			
        break;
    case FdoGeometryType_Polygon:
        FdoGeometrySerializer::SerializePolygon((FdoIPolygon*)geometry, writer, srsName,gmlversion);
        break;
    case FdoGeometryType_MultiPoint:
        FdoGeometrySerializer::SerializeMultiPoint((FdoIMultiPoint*)geometry, writer, srsName,gmlversion);
        break;
    case FdoGeometryType_MultiLineString:
        FdoGeometrySerializer::SerializeMultiLineString((FdoIMultiLineString*)geometry, writer,gmlversion);
        break;
    case FdoGeometryType_MultiPolygon:
        FdoGeometrySerializer::SerializeMultiPolygon((FdoIMultiPolygon*)geometry, writer, srsName,gmlversion);
        break;
    case FdoGeometryType_MultiGeometry:
        FdoGeometrySerializer::SerializeMultiGeometry((FdoIMultiGeometry*)geometry, writer, srsName,gmlversion);
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

FdoStringP FdoGeometrySerializer::GetDirectPositionCoordinates(FdoIDirectPosition* position,FdoGmlVersion gmlversion)
{
    static const int SIZE = 256;
    wchar_t temp[SIZE];
    if (gmlversion == FdoGmlVersion_212)
        swprintf(temp, SIZE - 1, L"%lf,%lf", position->GetX(), position->GetY());
    else
        swprintf(temp, SIZE - 1, L"%lf %lf", position->GetX(), position->GetY());
    FdoStringP ret = temp;

    FdoInt32 dimension = position->GetDimensionality();
    if (dimension & FdoDimensionality_Z)
    {
        if (gmlversion == FdoGmlVersion_212)
            swprintf(temp, SIZE - 1, L",%lf", position->GetZ());
        else
            swprintf(temp, SIZE - 1, L" %lf", position->GetZ());
        ret += temp;
    }
    if (dimension & FdoDimensionality_M)
    {
        if (gmlversion == FdoGmlVersion_212)
            swprintf(temp, SIZE - 1, L",%lf", position->GetM());
        else
            swprintf(temp, SIZE - 1, L" %lf", position->GetM());
        ret += temp;
    }

    return ret;
}

void FdoGeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{	
    FdoPtr<FdoIDirectPosition> pos = point->GetPosition ();	
    writer->WriteStartElement(L"gml:Point");

    if (gmlversion == FdoGmlVersion_212)
        writer->WriteStartElement(L"gml:coordinates");	
    else
        writer->WriteStartElement(L"gml:pos"); //"coordinates" is deprecated with GML version 3.1.0. Use "pos" instead.

    writer->WriteCharacters(FdoGeometrySerializer::GetDirectPositionCoordinates(pos,gmlversion));
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:LineString");

    if (gmlversion == FdoGmlVersion_212)
        writer->WriteStartElement(L"gml:coordinates");
    else
        writer->WriteStartElement(L"gml:posList"); //"coordinates" is deprecated with GML version 3.1.0. Use "posList" instead.

    FdoInt32 cnt = lineString->GetCount();

    // The coordinates are separated by a blank(" ").
    if (cnt > 0)
    {
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(0);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos,gmlversion));
    }

    for (FdoInt32 i=1; i<cnt; i++)
    {
        writer->WriteCharacters(L" ");
        FdoPtr<FdoIDirectPosition> pos = lineString->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos,gmlversion));		
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    FdoInt32 cntPos = linearRing->GetCount();
    if (cntPos == 0)
        return;

    writer->WriteStartElement(L"gml:LinearRing");

    if (gmlversion == FdoGmlVersion_212)
        writer->WriteStartElement(L"gml:coordinates");
    else
        writer->WriteStartElement(L"gml:posList");

    for (FdoInt32 i=0; i<cntPos; i++)
    {
        FdoPtr<FdoIDirectPosition> pos = linearRing->GetItem(i);
        writer->WriteCharacters(GetDirectPositionCoordinates(pos,gmlversion));
        if (i < cntPos-1)
            writer->WriteCharacters(L" ");
    }
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:Polygon");
    writer->WriteAttribute(L"srsName", srsName);

    // serialzie the exterior ring
    FdoPtr<FdoILinearRing> outer = polygon->GetExteriorRing();

    if (gmlversion == FdoGmlVersion_212)
        writer->WriteStartElement(L"gml:outerBoundaryIs");
    else
        writer->WriteStartElement(L"gml:exterior");

    FdoGeometrySerializer::SerializeLinearRing(outer, writer,gmlversion);
    writer->WriteEndElement();

    FdoInt32 cntRing = polygon->GetInteriorRingCount();	
    for (FdoInt32 i=0; i<cntRing; i++)
    {
        FdoPtr<FdoILinearRing> inner = polygon->GetInteriorRing(i);
        if (gmlversion == FdoGmlVersion_212)
            writer->WriteStartElement(L"gml:innerBoundaryIs");
        else
            writer->WriteStartElement(L"gml:interior");
        FdoGeometrySerializer::SerializeLinearRing(inner, writer,gmlversion);
        writer->WriteEndElement();
    }
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:MultiPoint");
    writer->WriteAttribute(L"srsName", srsName);

    FdoInt32 cntPoint = multiPoint->GetCount();
    for (FdoInt32 i=0; i<cntPoint; i++)
    {
        writer->WriteStartElement(L"gml:pointMember"); // pointMember appeared with each point element
        FdoPtr<FdoIPoint> point = multiPoint->GetItem(i);
        FdoGeometrySerializer::SerializePoint(point, writer,gmlversion);
        writer->WriteEndElement();
    }
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:MultiLineString");
    writer->WriteStartElement(L"gml:lineStringMember");
    FdoInt32 cntLineString = mlString->GetCount();
    for (FdoInt32 i=0; i<cntLineString; i++)
    {
        FdoPtr<FdoILineString> lString = mlString->GetItem(i);
        FdoGeometrySerializer::SerializeLineString(lString, writer,gmlversion);
    }
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:MultiPolygon");
    writer->WriteStartElement(L"gml:polygonMember");
    FdoInt32 cntPolygon = mPolygon->GetCount();
    for (FdoInt32 i=0; i<cntPolygon; i++)
    {
        FdoPtr<FdoIPolygon> polygon = mPolygon->GetItem(i);
        FdoGeometrySerializer::SerializePolygon(polygon, writer, srsName,gmlversion);
    }
    writer->WriteEndElement();
    writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName,FdoGmlVersion gmlversion)
{
    writer->WriteStartElement(L"gml:MultiGeometry");
    writer->WriteStartElement(L"gml:geometryMember");
    FdoInt32 cntGeometry = mGeometry->GetCount();
    for (FdoInt32 i=0; i<cntGeometry; i++)
    {
        FdoPtr<FdoIGeometry> geometry = mGeometry->GetItem(i);
        FdoGeometrySerializer::SerializeGeometry(geometry, writer, srsName,gmlversion);
    }
    writer->WriteEndElement();
    writer->WriteEndElement();
}


