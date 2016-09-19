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

#include <stdafx.h>
#include <OWS/FdoOwsGlobals.h>
#include "FdoOwsOgcGeometrySerializer.h"

void FdoOwsOgcGeometrySerializer::SerializeGeometry(FdoIGeometry* geometry, FdoXmlWriter* writer, FdoString* srsName)
{
	FdoGeometryType geoType = geometry->GetDerivedType();
	switch(geoType)
	{
	case FdoGeometryType_Point:			
		FdoOwsOgcGeometrySerializer::SerializePoint((FdoIPoint*)geometry, writer);			
		break;
	case FdoGeometryType_LineString:
		FdoOwsOgcGeometrySerializer::SerializeLineString((FdoILineString*)geometry, writer);			
		break;
	case FdoGeometryType_Polygon:
		FdoOwsOgcGeometrySerializer::SerializePolygon((FdoIPolygon*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiPoint:
		FdoOwsOgcGeometrySerializer::SerializeMultiPoint((FdoIMultiPoint*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiLineString:
		FdoOwsOgcGeometrySerializer::SerializeMultiLineString((FdoIMultiLineString*)geometry, writer);
		break;
	case FdoGeometryType_MultiPolygon:
		FdoOwsOgcGeometrySerializer::SerializeMultiPolygon((FdoIMultiPolygon*)geometry, writer, srsName);
		break;
	case FdoGeometryType_MultiGeometry:
		FdoOwsOgcGeometrySerializer::SerializeMultiGeometry((FdoIMultiGeometry*)geometry, writer, srsName);
		break;

	case FdoGeometryType_CurveString:
	case FdoGeometryType_CurvePolygon:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
		// TODO: what to do?
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE), "Unsupported geometry type."));
		break;
	}
}

FdoStringP FdoOwsOgcGeometrySerializer::GetDirectPositionCoordinates(FdoIDirectPosition* position)
{
	static const int SIZE = 256;
	wchar_t temp[SIZE];
	swprintf(temp, SIZE - 1, L"%f,%f", position->GetX(), position->GetY());
	FdoStringP ret = temp;

    FdoInt32 dimension = position->GetDimensionality();

    if (dimension & FdoDimensionality_Z)
    {
		if (position->GetZ() != 0.0) 
		{
			swprintf(temp, SIZE - 1, L",%f", position->GetZ());
				ret += temp;
		}
    }
    if (dimension & FdoDimensionality_M)
    {
		if (position->GetM() != 0.0) 
		{
				swprintf(temp, SIZE - 1, L",%f", position->GetM());
				ret += temp;
		}
    }


	return ret;
}

void FdoOwsOgcGeometrySerializer::SerializePoint(FdoIPoint* point, FdoXmlWriter* writer)
{	
	FdoPtr<FdoIDirectPosition> pos = point->GetPosition ();
	writer->WriteStartElement(FdoOwsGlobals::gml_Point);
	writer->WriteStartElement(FdoOwsGlobals::gml_coordinates);	
	writer->WriteCharacters(FdoOwsOgcGeometrySerializer::GetDirectPositionCoordinates(pos));
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoOwsOgcGeometrySerializer::SerializeLineString(FdoILineString* lineString, FdoXmlWriter* writer)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_LineString);
	writer->WriteStartElement(FdoOwsGlobals::gml_coordinates);
	
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

void FdoOwsOgcGeometrySerializer::SerializeLinearRing(FdoILinearRing* linearRing, FdoXmlWriter* writer)
{
	FdoInt32 cntPos = linearRing->GetCount();
	if (cntPos == 0)
		return;

	writer->WriteStartElement(FdoOwsGlobals::gml_LinearRing);
	writer->WriteStartElement(FdoOwsGlobals::gml_coordinates);
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

void FdoOwsOgcGeometrySerializer::SerializePolygon(FdoIPolygon* polygon, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_Polygon);
	writer->WriteAttribute(FdoOwsGlobals::srsName, srsName);

	// serialzie the exterior ring
	FdoPtr<FdoILinearRing> outer = polygon->GetExteriorRing();
	writer->WriteStartElement(FdoOwsGlobals::gml_outerBoundaryIs);
	FdoOwsOgcGeometrySerializer::SerializeLinearRing(outer, writer);
	writer->WriteEndElement();

	FdoInt32 cntRing = polygon->GetInteriorRingCount();	
	for (FdoInt32 i=0; i<cntRing; i++)
	{
		FdoPtr<FdoILinearRing> inner = polygon->GetInteriorRing(i);
		writer->WriteStartElement(FdoOwsGlobals::gml_innerBoundaryIs);
		FdoOwsOgcGeometrySerializer::SerializeLinearRing(inner, writer);
		writer->WriteEndElement();
	}
	writer->WriteEndElement();
}

void FdoOwsOgcGeometrySerializer::SerializeMultiPoint(FdoIMultiPoint* multiPoint, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_MultiPoint);
	writer->WriteAttribute(FdoOwsGlobals::srsName, srsName);
	
	writer->WriteStartElement(FdoOwsGlobals::gml_pointMember);
	FdoInt32 cntPoint = multiPoint->GetCount();
	for (FdoInt32 i=0; i<cntPoint; i++)
	{
		FdoPtr<FdoIPoint> point = multiPoint->GetItem(i);
		FdoOwsOgcGeometrySerializer::SerializePoint(point, writer);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoOwsOgcGeometrySerializer::SerializeMultiLineString(FdoIMultiLineString* mlString, FdoXmlWriter* writer)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_MultiLineString);
	writer->WriteStartElement(FdoOwsGlobals::gml_lineStringMember);
	FdoInt32 cntLineString = mlString->GetCount();
	for (FdoInt32 i=0; i<cntLineString; i++)
	{
		FdoPtr<FdoILineString> lString = mlString->GetItem(i);
		FdoOwsOgcGeometrySerializer::SerializeLineString(lString, writer);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoOwsOgcGeometrySerializer::SerializeMultiPolygon(FdoIMultiPolygon* mPolygon, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_MultiPolygon);
	writer->WriteStartElement(FdoOwsGlobals::gml_polygonMember);
	FdoInt32 cntPolygon = mPolygon->GetCount();
	for (FdoInt32 i=0; i<cntPolygon; i++)
	{
		FdoPtr<FdoIPolygon> polygon = mPolygon->GetItem(i);
		FdoOwsOgcGeometrySerializer::SerializePolygon(polygon, writer, srsName);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

void FdoOwsOgcGeometrySerializer::SerializeMultiGeometry(FdoIMultiGeometry* mGeometry, FdoXmlWriter* writer, FdoString* srsName)
{
	writer->WriteStartElement(FdoOwsGlobals::gml_MultiGeometry);
	writer->WriteStartElement(FdoOwsGlobals::gml_geometryMember);
	FdoInt32 cntGeometry = mGeometry->GetCount();
	for (FdoInt32 i=0; i<cntGeometry; i++)
	{
		FdoPtr<FdoIGeometry> geometry = mGeometry->GetItem(i);
		FdoOwsOgcGeometrySerializer::SerializeGeometry(geometry, writer, srsName);
	}
	writer->WriteEndElement();
	writer->WriteEndElement();
}

