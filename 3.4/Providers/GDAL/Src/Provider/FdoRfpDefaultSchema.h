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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDefaultSchema.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPDEFAULTSCHEMA_H
#define FDORFPDEFAULTSCHEMA_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

static char* defaultSchema[] = {
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>",
"<DataStore ",
" xmlns:fdo=\"http://fdo.osgeo.org/schemas\"",
" xmlns:gml=\"http://www.opengis.net/gml\"",
" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"",
" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ",
" xmlns:xlink=\"http://www.w3.org/1999/xlink\"",
" xmlns=\"http://fdo.osgeo.org/schemas\"",
" xsi:schemaLocation=\"http://fdo.osgeo.org/schemas",
" FdoDocument.xsd\"",
">",
"	<xs:schema ",
"	 xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" ",
"	 targetNamespace=\"http://fdo_customer/default\" ",
"	 xmlns:fdo=\"http://fdo.osgeo.org/schemas\" ",
"	 xmlns:default=\"http://fdo_customer/default\" ",
"	 elementFormDefault=\"qualified\" ",
"	 attributeFormDefault=\"unqualified\"",
"	>",
"		<xs:element name=\"default\" type=\"default:default\" abstract=\"false\"",
"		 substitutionGroup=\"gml:_Feature\"",
"		>",
"			<xs:key name=\"DefaultKey\"> ",
"  				<xs:selector xpath=\".//default\"/>",
"				<xs:field xpath=\"FeatId\"/>",
"			</xs:key>",
" 		</xs:element>",
"		<xs:complexType name=\"default\" abstract=\"false\" ",
"		>",
"			<xs:complexContent>",
"				<xs:extension base=\"gml:AbstractFeatureType\">",
"					<xs:sequence>",
"						<xs:element name=\"FeatId\">",
"							<xs:simpleType>",
"								<xs:restriction base=\"xs:string\">",
"									<xs:maxLength value=\"256\"/>",
"								</xs:restriction>",
"							</xs:simpleType>",
"						</xs:element>",
"						<xs:element name=\"Raster\" type=\"fdo:RasterPropertyType\" ",
"						>",
"							<xs:annotation>",
"								<xs:appinfo source=\"http://fdo.osgeo.org/schemas\">",
"									<fdo:DefaultDataModel ",
"                        			 			 organization=\"Pixel\"",
"									/>",
"								</xs:appinfo>",
"							</xs:annotation>",
"						</xs:element>",
"		        		</xs:sequence>",
"      				</xs:extension>",
"   			</xs:complexContent>",
"		</xs:complexType>",
"	</xs:schema>",
"</DataStore>",
NULL
};

static const wchar_t* defaultOverrides = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\t\
<DataStore \n\t\
 xmlns:fdo=\"http://fdo.osgeo.org/schemas\"\n\t\
 xmlns:gml=\"http://www.opengis.net/gml\"\n\t\
 xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"\n\t\
 xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" \n\t\
 xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\t\
 xmlns=\"http://fdo.osgeo.org/schemas\"\n\t\
 xsi:schemaLocation=\"http://fdo.osgeo.org/schemas\n\t\
 FdoDocument.xsd\"\n\t\
>\n\t\
	<SchemaMapping name=\"default\" provider=\"%ls\" xmlns=\"http://fdogrfp.osgeo.org/schemas\" xsi:schemaLocation=\"http://fdogrfp.osgeo.org/schemas\n\t\
		FdoRfpConfig.xsd\">\n\t\
	>\n\t\
		<complexType name=\"default\">\n\t\
			<RasterDefinition name=\"default\">\n\t\
				<Location name=\"%ls\" />\n\t\
			</RasterDefinition>\n\t\
		</complexType>\n\t\
	</SchemaMapping>\n\t\
</DataStore>";

#endif
