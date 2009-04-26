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
#ifndef FDOGRFPGLOBALS_H
#define FDOGRFPGLOBALS_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

namespace FdoGrfpGlobals
{
    static FdoString* DefaultSpatialContextName			= L"Default";
    static FdoString* DefaultSpatialContextCoordName	        = L"Default";
    static FdoString* DefaultSpatialContextWKTName		= L"LOCAL_CS[\"*XY-MT*\",LOCAL_DATUM[\"*X-Y*\",10000],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]";

    static double DefaultSpatialContextExtentMinX		= -10000000.0;
    static double DefaultSpatialContextExtentMinY		= -10000000.0;
    static double DefaultSpatialContextExtentMaxX		= 10000000.0;
    static double DefaultSpatialContextExtentMaxY		= 10000000.0;
    static double DefaultSpatialContextXYTolerance		= 0.001;
    static double DefaultSpatialContextZTolerance		= 0.001;

    static FdoString* GRFPProviderName				    = L"OSGeo.Gdal.3.4";
    static FdoString* GRFPProviderDisplayName			= L"OSGeo FDO Provider for Raster";
    static FdoString* GRFPProviderDescription			= L"Read access to various raster-based file formats. Supports georeferenced file-based raster images.";
    static FdoString* GRFPProviderVersion			    = L"3.5.0.0";
    static FdoString* GRFPFeatureDataObjectsVersion		= L"3.5.0.0";

    static FdoString* DefaultRasterFileLocation			= L"DefaultRasterFileLocation";
	
    static FdoString* MosaicFunctionName			    = L"MOSAIC";
    static FdoString* ClipFunctionName				    = L"CLIP";
    static FdoString* ResampleFunctionName			    = L"RESAMPLE";

    static FdoString* PaletteAttrName				    = L"Palette";
    static FdoString* NumOfPaletteEntriesAttrName	    = L"NumOfPaletteEntries";

    static FdoString* DefaultSchemaName				    = L"default";
    static FdoString* DefaultClassName				    = L"default";

    static FdoString* RasterArgumentName			    = L"raster";
    static FdoString* MinXArgumentName				    = L"minX";
    static FdoString* MinYArgumentName				    = L"minY";
    static FdoString* MaxXArgumentName				    = L"maxX";
    static FdoString* MaxYArgumentName				    = L"maxY";
    static FdoString* HeightArgumentName			    = L"height";
    static FdoString* WidthArgumentName				    = L"width";

    static int MaximumImageWidth                        = 100000;
    static int MaximumImageHeight                       = 100000;

    static FdoString* UserDefinedCoordName				= L"User-Defined";
};


#endif
