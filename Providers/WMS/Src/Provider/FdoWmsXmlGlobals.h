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
#ifndef FDOWMSXMLGLOBALS_H
#define FDOWMSXMLGLOBALS_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

namespace FdoWmsXmlGlobals
{
	static FdoString* WMSServiceName                        = L"WMS";

	static FdoString *WmsVersion                            = L"1.3.0";
	static FdoString *WmsGetMapRequest                      = L"GetMap";
	static FdoString *WmsGetMapRequest2                     = L"Map";

    static FdoString *WmsCapabilitiesCapability             = L"Capability";

    static FdoString *WmsCapabilitiesLayer                  = L"Layer";
    static FdoString *WmsCapabilitiesLayerQueryable         = L"queryable";
    static FdoString *WmsCapabilitiesLayerOpaque            = L"opaque";
    static FdoString *WmsCapabilitiesLayerNoSubsets         = L"noSubsets";
    static FdoString *WmsCapabilitiesLayerFixedWidth        = L"fixedWidth";
    static FdoString *WmsCapabilitiesLayerFixedHeight       = L"fixedHeight";
    static FdoString *WmsCapabilitiesLayerName              = L"Name";
    static FdoString *WmsCapabilitiesLayerTitle             = L"Title";
    static FdoString *WmsCapabilitiesLayerAbstract          = L"Abstract";
    static FdoString *WmsCapabilitiesLayerKeyword           = L"Keyword";
    static FdoString *WmsCapabilitiesLayerMinScale          = L"MinScaleDenominator";
    static FdoString *WmsCapabilitiesLayerMaxScale          = L"MaxScaleDenominator";
    static FdoString *WmsCapabilitiesLayerAttribution       = L"Attribution";

    static FdoString *WmsCapabilitiesSRS                    = L"SRS";
    static FdoString *WmsCapabilitiesCRS                    = L"CRS";
    static FdoString *WmsCapabilitiesSRSDelimiter           = L" ";

    static FdoString *WmsCapabilitiesBoundingBox            = L"BoundingBox";
    static FdoString *WmsCapabilitiesBoundingBoxMinX        = L"minx";
    static FdoString *WmsCapabilitiesBoundingBoxMinY        = L"miny";
    static FdoString *WmsCapabilitiesBoundingBoxMaxX        = L"maxx";
    static FdoString *WmsCapabilitiesBoundingBoxMaxY        = L"maxy";
    static FdoString *WmsCapabilitiesBoundingBoxResX        = L"resx";
    static FdoString *WmsCapabilitiesBoundingBoxResY        = L"resy";

    static FdoString *WmsCapabilitiesStyle                  = L"Style";
    static FdoString *WmsCapabilitiesStyleName              = L"Name";
    static FdoString *WmsCapabilitiesStyleTitle             = L"Title";
    static FdoString *WmsCapabilitiesStyleAbstract          = L"Abstract";

    static FdoString *WmsCapabilitiesDimension               = L"Dimension";
    static FdoString *WmsCapabilitiesDimensionName           = L"name";
    static FdoString *WmsCapabilitiesDimensionUnits          = L"units";
    static FdoString *WmsCapabilitiesDimensionUnitSymbol     = L"unitSymbol";
    static FdoString *WmsCapabilitiesDimensionDefault        = L"default";
    static FdoString *WmsCapabilitiesDimensionMultipleValues = L"multipleValues";
    static FdoString *WmsCapabilitiesDimensionNearestValue   = L"nearestValue";
    static FdoString *WmsCapabilitiesDimensionCurrent        = L"current";

    static FdoString *WmsCapabilitiesTrueString             = L"1";
    static FdoString *WmsCapabilitiesFalseString            = L"0";

	static FdoString *WmsCapabilitiesFormat                 = L"Format";
	static FdoString *WmsCapabilitiesGetMap                 = L"GetMap";

	// Literals to construct the WMS request
	static FdoString *WmsRequestLayers                      = L"LAYERS";
	static FdoString *WmsRequestStyles                      = L"STYLES";
	static FdoString *WmsRequestCRS                         = L"CRS";
	static FdoString *WmsRequestSRS                         = L"SRS";
	static FdoString *WmsRequestHeight                      = L"HEIGHT";
	static FdoString *WmsRequestWidth                       = L"WIDTH";
	static FdoString *WmsRequestFormat                      = L"FORMAT";
	static FdoString *WmsRequestBBOX                        = L"BBOX";
	static FdoString *WmsRequestComma                       = L",";
	static FdoString *WmsRequestTransparent                 = L"TRANSPARENT";
	static FdoString *WmsRequestTransparentTrue             = L"TRUE";
	static FdoString *WmsRequestTransparentFalse             = L"FALSE";
	static FdoString *WmsRequestTimeDimension               = L"TIME";
	static FdoString *WmsRequestElevation                   = L"ELEVATION";	
	static FdoString *WmsRequestBackgroundColor             = L"BGCOLOR";
	static FdoString *WmsRequestDefaultStyle				= L"";
    static FdoString *EXCEPTIONS                            = L"EXCEPTIONS";
    static FdoString* ExceptionType                         = L"application/vnd.ogc.se_xml";
    static FdoString* ExceptionType130                      = L"XML";
};


#endif

// FDOWMSXMLGLOBALS_H
