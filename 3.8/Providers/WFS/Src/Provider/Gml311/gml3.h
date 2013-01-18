#ifndef gml3_H
#define gml3_H

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

static const char* gml3[] = {
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>",
"<schema targetNamespace=\"http://www.opengis.net/gml\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://www.w3.org/2001/XMLSchema\" xmlns:sch=\"http://www.ascc.net/xml/schematron\" xmlns:gml=\"http://www.opengis.net/gml\"",
"        xmlns:xlink=\"http://www.w3.org/1999/xlink\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\" version=\"3.1.1\">",
"	<annotation>",
"		<appinfo source=\"urn:opengis:specification:gml:schema-xsd:gml:3.1.1\">gml.xsd</appinfo>",
"		<documentation>Top level GML schema",
"    Copyright (c) 2002-2005 OGC, All Rights Reserved.",
"	For conditions, see OGC Software Notice http://www.opengeospatial.org/about/?page=ipr</documentation>",
"	</annotation>",
"	<!-- ====================================================================== -->",
"	<include schemaLocation=\"dynamicFeature.xsd\"/>",
"	<include schemaLocation=\"topology.xsd\"/>",
"	<include schemaLocation=\"coverage.xsd\"/>",
"	<include schemaLocation=\"coordinateReferenceSystems.xsd\"/>",
"	<include schemaLocation=\"observation.xsd\"/>",
"	<include schemaLocation=\"defaultStyle.xsd\"/>",
"	<include schemaLocation=\"temporalReferenceSystems.xsd\"/>",
"	<!-- ====================================================================== -->",
"</schema>",
NULL
};

#endif
