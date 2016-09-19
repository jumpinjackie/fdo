#ifndef gml321_H
#define gml321_H

 //
 // Copyright (C) 2004-2010  Autodesk, Inc.
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

static const char* gml321[] = {
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>",
	"<schema targetNamespace=\"http://www.opengis.net/gml/3.2\" xmlns:gml=\"http://www.opengis.net/gml/3.2\" xmlns=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" version=\"3.2.1.2\">",
	"	<annotation>",
	"		<appinfo source=\"urn:x-ogc:specification:gml:schema-xsd:gml:3.2.1\">gml.xsd</appinfo>",
	"		<documentation>",
	"			GML is an OGC Standard.",
	"			Copyright (c) 2007,2010 Open Geospatial Consortium.",
	"			To obtain additional rights of use, visit http://www.opengeospatial.org/legal/ .",
	"		</documentation>",
	"	</annotation>",
	"	<!-- ====================================================================== -->",
	"	<include schemaLocation=\"dynamicFeature.xsd\"/>",
	"	<include schemaLocation=\"topology.xsd\"/>",
	"	<include schemaLocation=\"coverage.xsd\"/>",
	"	<include schemaLocation=\"coordinateReferenceSystems.xsd\"/>",
	"	<include schemaLocation=\"observation.xsd\"/>",
	"	<include schemaLocation=\"temporalReferenceSystems.xsd\"/>",
	"	<include schemaLocation=\"deprecatedTypes.xsd\"/>",
	"	<!-- ====================================================================== -->",
	"</schema>",
	NULL
};

#endif
