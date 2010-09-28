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

#ifndef FDOOWSGLOBALS_H
#define FDOOWSGLOBALS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

namespace FdoOwsGlobals
{
	static FdoString* GetCapabilities					    = L"GetCapabilities";
	static FdoString* DescribeFeatureType                   = L"DescribeFeatureType";
    static FdoString* Request                               = L"Request";
    static FdoString* DCPType                               = L"DCPType";
    static FdoString* HTTP                                  = L"HTTP";
    static FdoString* Get                                   = L"Get";
    static FdoString* Post                                  = L"Post";
    static FdoString* onlineResource                        = L"onlineResource";
    static FdoString* Name                                  = L"Name";
    static FdoString* Title                                 = L"Title";
    static FdoString* OnlineResource                        = L"OnlineResource";
    static FdoString* version                               = L"version";
    static FdoString* updateSequence                        = L"updateSequence";
    static FdoString* OWS_Capabilities                      = L"OWS_Capabilities";
    static FdoString* Service                               = L"Service";
    static FdoString* Capability                            = L"Capability";
    static FdoString* WFS_Capabilities                      = L"WFS_Capabilities";
    static FdoString* WMT_MS_Capabilities                   = L"WMT_MS_Capabilities";
    static FdoString* WMS_Capabilities                      = L"WMS_Capabilities";
    static FdoString* href                                  = L"http://www.w3.org/1999/xlink:href";
    static FdoString* ServiceExceptionReport                = L"ServiceExceptionReport";
    static FdoString* ServiceException                      = L"ServiceException";
    static FdoString* code                                  = L"code";
    static FdoString* REQUEST                               = L"REQUEST";
    static FdoString* Equal                                 = L"=";
    static FdoString* service                               = L"service";
    static FdoString* And                                   = L"&";
    static FdoString* SRS                                   = L"SRS";
    static FdoString* CRS                                   = L"CRS";


    static const size_t MIN_LEN_HTTP_RESPONSE_FIRST_LINE    = 12;
    static const char* HTTP_SLASH                           = "HTTP/";
    static const size_t HTTP_SLASH_LEN                      = sizeof("HTTP/") - 1;
    static const size_t LEN_HTTP_RET_CODE                   = 3;
    static const int MIN_HTTP_ERROR_CODE                    = 300;
    static const char* Content_Type                         = "Content-Type";
    static const size_t Content_Type_Len                    = sizeof("Content-Type") - 1;
    static const char Space                                 = ' ';
    static const char Colon                                 = ':';
    static const char* text_xml                             = "text/xml";
    static const size_t text_xml_len                        = sizeof("text/xml") - 1;

    static const char* image_png                             = "image/png";
    static const size_t image_png_len                        = sizeof("image/png") - 1;

    static const char* image_jpeg                             = "image/jpeg";
    static const size_t image_jpeg_len                        = sizeof("image/jpeg") - 1;

    static const char* image_tiff                             = "image/tiff";
    static const size_t image_tiff_len                        = sizeof("image/tiff") - 1;

	static const FdoString* ogcNamespace					= L"ogc";
	static const FdoString* Filter							= L"Filter";
	static const FdoString* PropertyName					= L"PropertyName";
	static const FdoString* Logical_And						= L"And";
	static const FdoString* Logical_Or						= L"Or";
	static const FdoString* Logical_Not						= L"Not";
	static const FdoString* PropertyIsEqualTo				= L"PropertyIsEqualTo";
	static const FdoString* PropertyIsNotEqualTo			= L"PropertyIsNotEqualTo";
	static const FdoString* PropertyIsGreaterThan			= L"PropertyIsGreaterThan";
	static const FdoString* PropertyIsGreaterThanOrEqualTo	= L"PropertyIsGreaterThanOrEqualTo";
	static const FdoString* PropertyIsLessThan				= L"PropertyIsLessThan";
	static const FdoString* PropertyIsLessThanOrEqualTo		= L"PropertyIsLessThanOrEqualTo"; 
	static const FdoString* PropertyIsLike					= L"PropertyIsLike";
	static const FdoString* PropertyIsNull					= L"PropertyIsNull";

	// Element attributes
	static const FdoString* wildCard						= L"wildCard";
	static const FdoString* singleChar						= L"singleChar";
	static const FdoString* escapeChar						= L"escapeChar";
	static const FdoString* unit							= L"unit";
	static const FdoString* name							= L"name";
	static const FdoString* srsName							= L"srsName";

	static const FdoString* Literal							= L"Literal";
	static const FdoString* Beyond							= L"Beyond";
	static const FdoString* DWithin							= L"DWithin";
	static const FdoString* Distance						= L"Distance";
	static const FdoString* Contains						= L"Contains";
	static const FdoString* Crosses							= L"Crosses";
	static const FdoString* Disjoint						= L"Disjoint";
	static const FdoString* Equals							= L"Equals";
	static const FdoString* Intersects						= L"Intersects";
	static const FdoString* Overlaps						= L"Overlaps";
	static const FdoString* Touches							= L"Touches";
	static const FdoString* Within							= L"Within";

	static const FdoString* Add								= L"Add";
	static const FdoString* Sub								= L"Sub";
	static const FdoString* Mul								= L"Mul";
	static const FdoString* Div								= L"Div";
	static const FdoString* Function						= L"Function";

	static const FdoString* gml_Point						= L"gml:Point";
	static const FdoString* gml_LineString					= L"gml:LineString";
	static const FdoString* gml_LinearRing					= L"gml:LinearRing";
	static const FdoString* gml_Polygon						= L"gml:Polygon";
	static const FdoString* gml_MultiPoint					= L"gml:MultiPoint";
	static const FdoString* gml_MultiPolygon				= L"gml:MultiPolygon";
	static const FdoString* gml_MultiLineString				= L"gml:MultiLineString";
	static const FdoString* gml_MultiGeometry				= L"gml:MultiGeometry";
	static const FdoString* gml_coordinates					= L"gml:coordinates";
	static const FdoString* gml_outerBoundaryIs				= L"gml:outerBoundaryIs";
	static const FdoString* gml_innerBoundaryIs				= L"gml:innerBoundaryIs";
	static const FdoString* gml_pointMember					= L"gml:pointMember";
	static const FdoString* gml_polygonMember				= L"gml:polygonMember";
	static const FdoString* gml_lineStringMember			= L"gml:lineStringMember";
	static const FdoString* gml_geometryMember				= L"gml:geometryMember";

    static const FdoString *CapabilitiesExGeoBoundingBox       = L"EX_GeographicBoundingBox";
    static const FdoString *CapabilitiesExGeoBoundingBoxWBL    = L"westBoundLongitude";
    static const FdoString *CapabilitiesExGeoBoundingBoxEBL    = L"eastBoundLongitude";
    static const FdoString *CapabilitiesExGeoBoundingBoxNBL    = L"northBoundLatitude";
    static const FdoString *CapabilitiesExGeoBoundingBoxSBL    = L"southBoundLatitude";

    static const FdoString *CapabilitiesLatLonBoundingBox       = L"LatLonBoundingBox";
    static const FdoString *CapabilitiesLatLonBoundingBoxWBL    = L"minx";
    static const FdoString *CapabilitiesLatLonBoundingBoxEBL    = L"maxx";
    static const FdoString *CapabilitiesLatLonBoundingBoxNBL    = L"maxy";
    static const FdoString *CapabilitiesLatLonBoundingBoxSBL    = L"miny";
}


#endif



