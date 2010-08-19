#ifndef FDO_XML_GEOMETRYHANDLER_H_
#define FDO_XML_GEOMETRYHANDLER_H_
//

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

#ifdef _WIN32
#pragma once
#endif
#include <vector>

#include <FdoStd.h>

class FdoXmlCoordinateGroup;
class FdoXmlGeometry;
class FdoXmlGeometricProperty;

///<summary>
/// </summary>
class FdoXmlGeometryHandler :
	public FdoXmlSaxHandler,
	public FdoIDisposable
{
	friend class FdoXmlFeaturePropertyReaderImpl;

public:

	static FdoXmlGeometryHandler * Create(FdoXmlFeatureFlags* flags = NULL);
	
	virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext*, FdoString*, FdoString* name, FdoString*, FdoXmlAttributeCollection*);
	virtual FdoBoolean XmlEndElement(FdoXmlSaxContext*, FdoString*, FdoString*, FdoString*);
	virtual void XmlCharacters(FdoXmlSaxContext*, FdoString*);

	virtual FdoByteArray* GetFgf();

protected:
	FdoXmlGeometryHandler();
	FdoXmlGeometryHandler(FdoXmlFeatureFlags *flags);
	virtual ~FdoXmlGeometryHandler();

    virtual void Dispose ()
    {
        delete this;
    }
	
	enum GmlGeometryType{
		GmlGeometryType_Point,
		GmlGeometryType_Box,
		GmlGeometryType_Polygon,
		GmlGeometryType_LinearRing,
		GmlGeometryType_LineString,
		GmlGeometryType_MultiPoint,
		GmlGeometryType_MultiLineString,
		GmlGeometryType_MultiPolygon,
		GmlGeometryType_MultiGeometry,
		GmlGeometryType_GeometryAssociation,
		GmlGeometryType_Coordinates,
		GmlGeometryType_Coord,
		GmlGeometryType_X,
		GmlGeometryType_Y,
		GmlGeometryType_Z,

		//GML 3 elements
		GmlGeometryType_LowerCorner, 
		GmlGeometryType_UpperCorner,
		//end of GML 3 elements

		GmlGeometryType_Unknown
	};
	enum ParsingState{
		ParsingState_Start,
		ParsingState_Point,
		ParsingState_Box,
		ParsingState_Polygon,
		ParsingState_LinearRing,
		ParsingState_LineString,
		ParsingState_MultiPoint,
		ParsingState_MultiLineString,
		ParsingState_MultiPolygon,
		ParsingState_MultiGeometry,
		ParsingState_GeometryAssociation,
		ParsingState_Coordinates,
		ParsingState_LowerCorner,
		ParsingState_UpperCorner,
		ParsingState_Coord,
		ParsingState_X,
		ParsingState_Y,
		ParsingState_Z,
		ParsingState_Unknown
	};


	//helper function
	virtual FdoXmlGeometry* GetGeometry();

	GmlGeometryType getGmlGeometryType(FdoString* elementName);

private:

	std::vector<ParsingState> m_parsingStateStack;
	std::vector<FdoXmlGeometry*> m_geometryStack;

	bool m_isMultiGeometry;

	FdoGmlVersion m_gmlVersion;
	
	//temp store for parsed objects
	FdoPtr<FdoXmlGeometryHandler> m_nestedHandler;
	FdoStringP m_dataProperty;
	//std::vector<double> m_coordinates;
	FdoPtr<FdoXmlCoordinateGroup> m_coordinates;


	// handle multi gemoetry in GML3
	// in GML3 the multi geometry is defined like below, while geometry is a PointArrayPropertyType
	// <geometry>
	//	<gml:Point>
	//  </gml:Point>
	//	<gml:Point>
	//  </gml:Point>
	//	<gml:Point>
	//  </gml:Point>
	// </geometry>
	// we can only know it by the definition in schema, which is passed in as typeGeomExpected
	// so, we need to define a new logic to handle it, and the following two methods is used for it.
	//
	void StartHandleGML3MultiGeometry();
	void EndHandleGML3MultiGeometry();

    void SetExpectedGmlGeometry(GmlGeometryType typeGeomExpected);
    GmlGeometryType m_typeGeomExpected;
    FdoXmlSaxHandler* SkipFirstParseStep();
    void RunLastParseStep(FdoString* name, GmlGeometryType typeGeomExpected);
};

typedef FdoPtr<FdoXmlGeometryHandler> FdoXmlGeometryHandlerP;


#endif

