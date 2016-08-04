// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include "GeometryHandler.h"

#include <Geometry/Fgf/Factory.h>

#include "CoordinateGroup.h"
#include "Geometry.h"

#include "Point.h"
#include "Box.h"
#include "LinearRing.h"
#include "LineString.h"
#include "Polygon.h"

#include "MultiPoint.h"
#include "MultiLineString.h"
#include "MultiPolygon.h"
#include "MultiGeometry.h"

#include <iostream>
using namespace std;


FdoXmlGeometryHandler::FdoXmlGeometryHandler()
{
	m_parsingStateStack.push_back(ParsingState_Start);

	m_isMultiGeometry = false;
    m_typeGeomExpected = GmlGeometryType_Unknown;
}

FdoXmlGeometryHandler::FdoXmlGeometryHandler(FdoXmlFeatureFlags *flags)
{
	m_parsingStateStack.push_back(ParsingState_Start);

	m_gmlVersion = FdoGmlVersion_212;

	if (flags != NULL)
		m_gmlVersion = flags->GetGmlVersion();

	m_isMultiGeometry = false;
    m_typeGeomExpected = GmlGeometryType_Unknown;
}

FdoXmlGeometryHandler::~FdoXmlGeometryHandler()
{
	while(!m_geometryStack.empty()){
		FDO_SAFE_RELEASE(m_geometryStack.back());
		m_geometryStack.pop_back();
	}
}

FdoXmlGeometryHandler * FdoXmlGeometryHandler::Create(FdoXmlFeatureFlags* flags)
{
	return new FdoXmlGeometryHandler(flags);
}


FdoXmlSaxHandler* FdoXmlGeometryHandler::XmlStartElement(
	FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* nextHandler = NULL;
	FdoXmlGeometry* newGeometry = NULL;

	GmlGeometryType curType = getGmlGeometryType(name);
    // in case we don't know the geometry type, use expected geom
    if (curType == GmlGeometryType_Unknown)
        curType = m_typeGeomExpected;
    m_typeGeomExpected = GmlGeometryType_Unknown;
    
	switch(curType)
	{
	//Point
	case GmlGeometryType_Point:
		m_parsingStateStack.push_back(ParsingState_Point);

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlPoint::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//Box
	case GmlGeometryType_Box:
		m_parsingStateStack.push_back(ParsingState_Box);

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlBox::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;
	//LinearRing
	case GmlGeometryType_LinearRing:
		m_parsingStateStack.push_back(ParsingState_LinearRing);

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlLinearRing::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//LineString
	case GmlGeometryType_LineString:
		m_parsingStateStack.push_back(ParsingState_LineString);

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlLineString::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;


	//Polygon
	case GmlGeometryType_Polygon:
		m_parsingStateStack.push_back(ParsingState_Polygon);
		
		//NOTE: polygon is treated as a collection of Linear rings
		//the first is the outer boundary
		//the rest are the inner boundary
		m_isMultiGeometry = true;

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlPolygon::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiPoint
	case GmlGeometryType_MultiPoint:
		m_parsingStateStack.push_back(ParsingState_MultiPoint);
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPoint::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiLineString
	case GmlGeometryType_MultiLineString:
		m_parsingStateStack.push_back(ParsingState_MultiLineString);
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiLineString::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiPolygon
	case GmlGeometryType_MultiPolygon:
		m_parsingStateStack.push_back(ParsingState_MultiPolygon);
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPolygon::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiGeometry
	case GmlGeometryType_MultiGeometry:
		m_parsingStateStack.push_back(ParsingState_MultiGeometry);
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiGeometry::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//Geometry Association
	case GmlGeometryType_GeometryAssociation:
		m_parsingStateStack.push_back(ParsingState_GeometryAssociation);
	
		m_nestedHandler = new FdoXmlGeometryHandler();
        // Pass down the outer geometry coordinates.
        m_nestedHandler->m_coordinates = m_coordinates;

		nextHandler = m_nestedHandler;
		break;

	//Coordinates
	case GmlGeometryType_Coordinates:
		m_parsingStateStack.push_back(ParsingState_Coordinates);
		m_dataProperty = L"";
		break;

	case GmlGeometryType_LowerCorner:
		m_parsingStateStack.push_back(ParsingState_LowerCorner);
		m_dataProperty = L"";
		break;

	case GmlGeometryType_UpperCorner:
		m_parsingStateStack.push_back(ParsingState_UpperCorner);
		m_dataProperty = L"";
		break;

	//Coord
	case GmlGeometryType_Coord:
		m_parsingStateStack.push_back(ParsingState_Coord);
		break;
	
	//X
	case GmlGeometryType_X:
		m_parsingStateStack.push_back(ParsingState_X);
		m_dataProperty = L"";
		break;
	
	//Y
	case GmlGeometryType_Y:
		m_parsingStateStack.push_back(ParsingState_Y);
		m_dataProperty = L"";
		break;
	
	//Z
	case GmlGeometryType_Z:
		m_parsingStateStack.push_back(ParsingState_Z);
		m_dataProperty = L"";
		break;
	
	default:
		m_parsingStateStack.push_back(ParsingState_Unknown);
		break;
	}

	return nextHandler;
}

FdoBoolean FdoXmlGeometryHandler::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{

	FdoXmlGeometry* curGeometry = NULL;
	if(!m_geometryStack.empty())
		curGeometry = m_geometryStack.back();

	FdoPtr<FdoXmlGeometry> nestedGeometry;

	ParsingState curState = m_parsingStateStack.back();
	m_parsingStateStack.pop_back();

	switch(curState)
	{
	//Point
	case ParsingState_Point:
		if (m_gmlVersion == FdoGmlVersion_311 && m_isMultiGeometry)
			EndHandleGML3MultiGeometry();
		break;

	//Box
	case ParsingState_Box:

		break;

	//Polygon
	case ParsingState_Polygon:
		if (m_gmlVersion == FdoGmlVersion_311 && m_isMultiGeometry)
			EndHandleGML3MultiGeometry();
		break;

	//LinearRing
	case ParsingState_LinearRing:
		if (m_gmlVersion == FdoGmlVersion_311 && m_isMultiGeometry)
			EndHandleGML3MultiGeometry();
		break;

	//LineString
	case ParsingState_LineString:
		if (m_gmlVersion == FdoGmlVersion_311 && m_isMultiGeometry)
			EndHandleGML3MultiGeometry();
		break;

	//MultiPoint
	case ParsingState_MultiPoint:

		break;

	//MultiLineString
	case ParsingState_MultiLineString:

		break;

	//MultiPolygon
	case ParsingState_MultiPolygon:

		break;

	//MultiGeometry
	case ParsingState_MultiGeometry:

		break;

	//Geometry Association
	case ParsingState_GeometryAssociation:

		nestedGeometry = m_nestedHandler->GetGeometry();

        // Record the inner geometry
        if (curGeometry == NULL)
            m_geometryStack.push_back(nestedGeometry.Detach());
        else
        {
            if(m_isMultiGeometry){
                curGeometry->AddGeometryMember(nestedGeometry);
            }
            else{
                curGeometry->AddGeometricProperty(name, nestedGeometry);
            }
        }

		break;

	//Coordinates
	case ParsingState_Coordinates:
	case ParsingState_LowerCorner:
	case ParsingState_UpperCorner:
		if(m_coordinates != NULL)
			m_coordinates->AddCoordinate(m_dataProperty);
		break;

	//Coord
	case ParsingState_Coord:
		break;

	//X
	case ParsingState_X:
		if(m_coordinates != NULL)
			m_coordinates->AddCoordX(m_dataProperty);
		break;

	//Y
	case ParsingState_Y:
		if(m_coordinates != NULL)
			m_coordinates->AddCoordY(m_dataProperty);
		break;

	//Z
	case ParsingState_Z:
		if(m_coordinates != NULL)
			m_coordinates->AddCoordZ(m_dataProperty);
		break;

	default:
		break;
	}

	return false;
}

void FdoXmlGeometryHandler::XmlCharacters(FdoXmlSaxContext* context, FdoString* value)
{
	m_dataProperty += value;
}

FdoByteArray* FdoXmlGeometryHandler::GetFgf()
{
	FdoXmlGeometry* curGeometry = m_geometryStack.back();
	return curGeometry->GetFgf();
}

FdoXmlGeometry* FdoXmlGeometryHandler::GetGeometry()
{
	FdoXmlGeometry* geometry = m_geometryStack.empty() ? NULL : m_geometryStack.back();
	return FDO_SAFE_ADDREF(geometry);
}


//The hard-coded implementation here will be replaced by 
//schema support in the future
FdoXmlGeometryHandler::GmlGeometryType FdoXmlGeometryHandler::getGmlGeometryType(FdoString* name)
{
	GmlGeometryType geoType;

	if (wcscmp( name, L"Point" ) == 0){
		geoType = GmlGeometryType_Point;
	}
	else if (wcscmp( name, L"Box" ) == 0){
		geoType = GmlGeometryType_Box;
	}
	else if (wcscmp( name, L"Polygon" ) == 0){
		geoType = GmlGeometryType_Polygon;
	}
	else if (wcscmp( name, L"LineString" ) == 0){
		geoType = GmlGeometryType_LineString;
	}
	else if (wcscmp( name, L"LinearRing" ) == 0){
		geoType = GmlGeometryType_LinearRing;
	}
	else if (wcscmp( name, L"MultiPoint" ) == 0){
		geoType = GmlGeometryType_MultiPoint;
	}
	else if (wcscmp( name, L"MultiLineString" ) == 0){
		geoType = GmlGeometryType_MultiLineString;
	}
	else if (wcscmp( name, L"MultiPolygon" ) == 0){
		geoType = GmlGeometryType_MultiPolygon;
	}
	else if (wcscmp( name, L"MultiGeometry" ) == 0){
		geoType = GmlGeometryType_MultiGeometry;
	}
	else if (wcscmp( name, L"pointMember" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"lineStringMember" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"polygonMember" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"geometryMember" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"outerBoundaryIs" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"innerBoundaryIs" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"coordinates" ) == 0){
		geoType = GmlGeometryType_Coordinates;
	}
	else if (wcscmp( name, L"coord" ) == 0){
		geoType = GmlGeometryType_Coord;
	}
	else if (wcscmp( name, L"X" ) == 0){
		geoType = GmlGeometryType_X;
	}
    else if (wcscmp( name, L"Y" ) == 0){
        geoType = GmlGeometryType_Y;
    }
    else if (wcscmp( name, L"Z" ) == 0){
        geoType = GmlGeometryType_Z;
    }
    // GML 3
    else if (wcscmp( name, L"Curve" ) == 0){
        geoType = GmlGeometryType_LineString;
    }
    else if (wcscmp( name, L"segments" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"LineStringSegment" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"Surface" ) == 0){
        geoType = GmlGeometryType_Polygon;
    }
    else if (wcscmp( name, L"patches" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"PolygonPatch" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"MultiCurve" ) == 0){
        geoType = GmlGeometryType_MultiLineString;
    }
    else if (wcscmp( name, L"curveMember" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"MultiSurface" ) == 0){
        geoType = GmlGeometryType_MultiPolygon;
    }
    else if (wcscmp( name, L"surfaceMember" ) == 0){
        geoType = GmlGeometryType_GeometryAssociation;
    }
    else if (wcscmp( name, L"Envelope" ) == 0){
        geoType = GmlGeometryType_Box;
    }
    else if (wcscmp( name,L"lowerCorner") == 0){
        geoType = GmlGeometryType_LowerCorner;
	}
	else if (wcscmp( name,L"upperCorner") == 0){
		geoType = GmlGeometryType_UpperCorner;
	}
	else if (wcscmp (name, L"pos") == 0){
		geoType = GmlGeometryType_Coordinates;
	}
	else if (wcscmp( name, L"posList") ==0 ){
		geoType = GmlGeometryType_Coordinates;
	}
	else if (wcscmp( name, L"exterior" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	else if (wcscmp( name, L"interior" ) == 0){
		geoType = GmlGeometryType_GeometryAssociation;
	}
	// end of GML 3 elements
	else
    {
	    geoType  = GmlGeometryType_Unknown;
	}

	return geoType;
}

FdoXmlSaxHandler* FdoXmlGeometryHandler::SkipFirstParseStep()
{
	FdoXmlSaxHandler* nextHandler = NULL;
	FdoXmlGeometry* newGeometry = NULL;

	GmlGeometryType curType = m_typeGeomExpected;
    m_typeGeomExpected = GmlGeometryType_Unknown;
    
	switch(curType)
	{
	//Point
	case GmlGeometryType_Point:

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlPoint::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//LinearRing
	case GmlGeometryType_LinearRing:

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlLinearRing::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//LineString
	case GmlGeometryType_LineString:

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlLineString::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;


	//Polygon
	case GmlGeometryType_Polygon:
		
		//NOTE: polygon is treated as a collection of Linear rings
		//the first is the outer boundary
		//the rest are the inner boundary
		m_isMultiGeometry = true;

		m_coordinates = FdoXmlCoordinateGroup::Create();
		newGeometry = FdoXmlPolygon::Create();
		newGeometry->SetCoordinates(m_coordinates);
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiPoint
	case GmlGeometryType_MultiPoint:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPoint::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiLineString
	case GmlGeometryType_MultiLineString:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiLineString::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiPolygon
	case GmlGeometryType_MultiPolygon:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPolygon::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiGeometry
	case GmlGeometryType_MultiGeometry:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiGeometry::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//Geometry Association
	case GmlGeometryType_GeometryAssociation:
	
		m_nestedHandler = new FdoXmlGeometryHandler();
		nextHandler = m_nestedHandler;
		break;

	default:
		m_parsingStateStack.push_back(ParsingState_Unknown);
		break;
	}

	return nextHandler;
}

void FdoXmlGeometryHandler::RunLastParseStep(FdoString* name, GmlGeometryType typeGeomExpected)
{
    FdoXmlGeometry* curGeometry = NULL;
    FdoPtr<FdoXmlGeometry> nestedGeometry;
	if (m_nestedHandler != NULL && typeGeomExpected == GmlGeometryType_GeometryAssociation)
    {
	    if(!m_geometryStack.empty())
		    curGeometry = m_geometryStack.back();

		nestedGeometry = m_nestedHandler->GetGeometry();

		if(m_isMultiGeometry){
			curGeometry->AddGeometryMember(nestedGeometry);
		}
		else{
			curGeometry->AddGeometricProperty(name, nestedGeometry);
		}
    }
}

void FdoXmlGeometryHandler::SetExpectedGmlGeometry(GmlGeometryType typeGeomExpected)
{
	m_typeGeomExpected = typeGeomExpected;
	
	// handle GML 3 multi geometry
	// because we only know its type here
	if (m_gmlVersion == FdoGmlVersion_311 &&
		(m_typeGeomExpected == GmlGeometryType_MultiPoint ||
		 m_typeGeomExpected == GmlGeometryType_MultiLineString ||
		 m_typeGeomExpected == GmlGeometryType_MultiPolygon ||
		 m_typeGeomExpected == GmlGeometryType_MultiGeometry))
		StartHandleGML3MultiGeometry();
};

void FdoXmlGeometryHandler::StartHandleGML3MultiGeometry()
{
	FdoXmlGeometry* newGeometry = NULL;
	switch(m_typeGeomExpected)
	{
	//MultiPoint
	case GmlGeometryType_MultiPoint:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPoint::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiLineString
	case GmlGeometryType_MultiLineString:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiLineString::Create();
		m_geometryStack.push_back(newGeometry);

		break;

	//MultiPolygon
	case GmlGeometryType_MultiPolygon:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiPolygon::Create();
		m_geometryStack.push_back(newGeometry);

		break;	

	//MultiGeometry
	case GmlGeometryType_MultiGeometry:
		m_isMultiGeometry = true;

		newGeometry = FdoXmlMultiGeometry::Create();
		m_geometryStack.push_back(newGeometry);

		break;
	}
}


void FdoXmlGeometryHandler::EndHandleGML3MultiGeometry()
{
	// add the current geomerty to the current Multi geometry, which is stored in previous item.
	FdoXmlGeometry* curGeometry = NULL;

	FdoXmlGeometry* curMultiGeometry = NULL;
	if(!m_geometryStack.empty())
	{
		curGeometry = m_geometryStack.back();
		m_geometryStack.pop_back();
	}

	// the previous item is the multi geometry
	if(!m_geometryStack.empty())
		curMultiGeometry = m_geometryStack.back();

	if (curMultiGeometry != NULL)
		curMultiGeometry->AddGeometryMember(curGeometry);
}