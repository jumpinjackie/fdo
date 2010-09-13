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
#ifndef FDOWMSGLOBALS_H
#define FDOWMSGLOBALS_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#define WMS_PROVIDER_NAME					L"OSGeo.WMS.3.6"
#define WMS_PROVIDER_DEFAULT_DISPLAY_NAME	"OSGeo FDO Provider for WMS"
#define WMS_PROVIDER_DEFAULT_DESCRIPTION	"Read access to OGC WMS-based data store."
#define WMS_PROVIDER_VERSION				L"3.6.0.0"
#define WMS_FDO_VERSION						L"3.6.0.0"

namespace FdoWmsGlobals
{
    static FdoString* ConnectionPropertyFeatureServer       = L"FeatureServer";
    static FdoString* ConnectionPropertyUsername            = L"Username";
    static FdoString* ConnectionPropertyPassword            = L"Password";
	static FdoString* ConnectionPropertyDefaultImageHeight  = L"DefaultImageHeight";
	static FdoString* ConnectionPropertyDelimiter	        = L";";
	static FdoString* ConnectionPropertySeparator	        = L"=";

	static FdoString* ClipFunctionName					    = L"CLIP";
	static FdoString* ClipFunctionRasterArgumentName	    = L"raster";
	static FdoString* ClipFunctionMinXArgumentName			= L"minX";
	static FdoString* ClipFunctionMinYArgumentName			= L"minY";
	static FdoString* ClipFunctionMaxXArgumentName			= L"maxX";
	static FdoString* ClipFunctionMaxYArgumentName			= L"maxY";

	static FdoString* ResampleFunctionName					= L"RESAMPLE";
	static FdoString* ResampleFunctionRasterArgumentName    = L"raster";
	static FdoString* ResampleFunctionMinXArgumentName      = L"minX";
	static FdoString* ResampleFunctionMinYArgumentName      = L"minY";
	static FdoString* ResampleFunctionMaxXArgumentName	    = L"maxX";
	static FdoString* ResampleFunctionMaxYArgumentName		= L"maxY";
	static FdoString* ResampleFunctionHeightArgumentName    = L"height";
	static FdoString* ResampleFunctionWidthArgumentName     = L"width";

	static FdoString* ExtentsFunctionRasterArgumentName	    = L"raster";

	static FdoString* DefaultSchemaName                     = L"WMS_Schema";
	static FdoString* DefaultSchemaDescription              = L"FDO WMS Schema";
	static FdoString* PropertyFeatId                        = L"FeatId";
	static FdoString* PropertyFeatIdDescription             = L"WMS Feature Identifier Property";
	static FdoString* PropertyRaster                        = L"Raster";
	static FdoString* PropertyRasterDescription             = L"WMS Feature Raster Property";

	static FdoString* PaletteAttrName						= L"Palette";
	static FdoString* NumOfPaletteEntriesAttrName			= L"NumOfPaletteEntries";

    static FdoString *RasterMIMEFormat_PNG			        = L"image/png";
	static FdoString *RasterMIMEFormat_TIFF				    = L"image/tiff";
	static FdoString *RasterMIMEFormat_JPEG				    = L"image/jpeg";
	static FdoString *RasterMIMEFormat_GIF				    = L"image/gif";
	static FdoString *ImageFormatPrefix						= L"image/";
    static FdoString *RasterFormatSemicolon                 = L";";

	static FdoString *DefaultEPSGCode     				    = L"EPSG:4326";
	static FdoString *DefaultEPSGCode2     				    = L"CRS:4326";
	static FdoString *ESPGPrefix     						= L"EPSG";
	static FdoString *WmsVersion100     					= L"1.0.0";
	static FdoString *WmsVersion110     					= L"1.1.0";
	static FdoString *WmsVersion111     					= L"1.1.1";
	static FdoString *WmsVersion130     					= L"1.3.0";
	static FdoString *ExtendedReverseEPSGsFileName			= L"ExtendedReverseEPSG.txt";

};


#endif
