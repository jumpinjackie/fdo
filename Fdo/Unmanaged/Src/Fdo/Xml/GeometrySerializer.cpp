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

#include <Fdo.h>
#include "GeometrySerializer.h"

void FdoGeometrySerializer::SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName)
{
	FdoGeometryType geoType = geometry->GetDerivedType();
	switch(geoType)
	{
	case FdoGeometryType_Point:			
		FdoGeometrySerializer::SerializePoint((FdoIPoint*)geometry, writer);			
		break;
	case FdoGeometryType_LineString:
		FdoGeometrySerializer::SerializeLineString((FdoILineString*)geometry, writer);			
		break;
	case FdoGeometryType_Polygon:
		FdoGeometrySerializer::SerializePolygon((FdoIPolygon*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiPoint:
		FdoGeometrySerializer::SerializeMultiPoint((FdoIMultiPoint*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiLineString:
		FdoGeometrySerializer::SerializeMultiLineString((FdoIMultiLineString*)geometry, writer);
		break;
	case FdoGeometryType_MultiPolygon:
		FdoGeometrySerializer::SerializeMultiPolygon((FdoIMultiPolygon*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiGeometry:
		FdoGeometrySerializer::SerializeMultiGeometry((FdoIMultiGeometry*)geometry, writer, srsName);
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

FdoStringP FdoGeometrySerializer::GetDirectPositionCoordinates(FdoIDirectPosition* position)
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

void FdoGeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer)
{	
	writer->WriteStartElement(L"gml:Point");
	writer->WriteStartElement(L"gml:coordinates");	
	writer->WriteCharacters(FdoGeometrySerializer::GetDirectPositionCoordinates(point->GetPosition()));
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer)
{
	writer->WriteStartElement(L"gml:LineString");
	writer->WriteStartElement(L"gml:coordinates");
	
	FdoInt32 cnt = lineString->GetCount();
	
	// The coordinates are separated by a blank(" ").
	if (cnt > 0)
	{
		FdoIDirectPosition* pos = lineString->GetItem(0);
		writer->WriteCharacters(GetDirectPositionCoordinates(pos));
	}
	
	for (FdoInt32 i=1; i<cnt; i++)
	{
		writer->WriteCharacters(L" ");
		FdoIDirectPosition* pos = lineString->GetItem(i);
		writer->WriteCharacters(GetDirectPositionCoordinates(pos));		
	}

	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer)
{
	FdoInt32 cntPos = linearRing->GetCount();
	if (cntPos == 0)
		return;

	writer->WriteStartElement(L"gml:LinearRing");
	writer->WriteStartElement(L"gml:coordinates");
	for (FdoInt32 i=0; i<cntPos; i++)
	{
		FdoIDirectPosition* pos = linearRing->GetItem(i);
		writer->WriteCharacters(GetDirectPositionCoordinates(pos));
		if (i < cntPos-1)
			writer->WriteCharacters(L" ");
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(L"gml:Polygon");
	writer->WriteAttribute(L"srsName", srsName);

	// serialzie the exterior ring
	FdoILinearRing* outer = polygon->GetExteriorRing();
	writer->WriteStartElement(L"gml:outerBoundaryIs");
	FdoGeometrySerializer::SerializeLinearRing(outer, writer);
	writer->WriteEndElement();

	FdoInt32 cntRing = polygon->GetInteriorRingCount();	
	for (FdoInt32 i=0; i<cntRing; i++)
	{
		FdoILinearRing* inner = polygon->GetInteriorRing(i);
		writer->WriteStartElement(L"gml:innerBoundaryIs");
		FdoGeometrySerializer::SerializeLinearRing(inner, writer);
		writer->WriteEndElement();
	}
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(L"gml:MultiPoint");
	writer->WriteAttribute(L"srsName", srsName);
	
	writer->WriteStartElement(L"gml:pointMember");
	FdoInt32 cntPoint = multiPoint->GetCount();
	for (FdoInt32 i=0; i<cntPoint; i++)
	{
		FdoGeometrySerializer::SerializePoint(multiPoint->GetItem(i), writer);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer)
{
	writer->WriteStartElement(L"gml:MultiLineString");
	writer->WriteStartElement(L"gml:lineStringMember");
	FdoInt32 cntLineString = mlString->GetCount();
	for (FdoInt32 i=0; i<cntLineString; i++)
	{
		FdoGeometrySerializer::SerializeLineString(mlString->GetItem(i), writer);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(L"gml:MultiPolygon");
	writer->WriteStartElement(L"gml:polygonMember");
	FdoInt32 cntPolygon = mPolygon->GetCount();
	for (FdoInt32 i=0; i<cntPolygon; i++)
	{
		FdoGeometrySerializer::SerializePolygon(mPolygon->GetItem(i), writer, srsName);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoGeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(L"gml:MultiGeometry");
	writer->WriteStartElement(L"gml:geometryMember");
	FdoInt32 cntGeometry = mGeometry->GetCount();
	for (FdoInt32 i=0; i<cntGeometry; i++)
	{
		FdoIGeometry* geometry = mGeometry->GetItem(i);
		FdoGeometrySerializer::SerializeGeometry(geometry, writer, srsName);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}


