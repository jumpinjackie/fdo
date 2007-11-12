/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
    static FdoString* g_WmsProviderName					    = L"OSGeo.WMS.3.3";
    static FdoString* g_WmsSchemaMapping                    = L"SchemaMapping";
    static FdoString* g_WmsClassDefinitionElement           = L"complexType";
    static FdoString* g_WmsRasterDefinition                 = L"RasterDefinition";
    static FdoString* g_WmsLayerDefinition                  = L"Layer";
    static FdoString* g_WmsStyleDefinition                  = L"Style";
    static FdoString* g_WmsImageFormat                      = L"Format";
    static FdoString* g_WmsImageFormatType                  = L"FormatType";

    static FdoString* g_WmsTransparent                      = L"Transparent";
    static FdoString* g_WmsBackgroundColor                  = L"BackgroundColor";
    static FdoString* g_WmsTimeDimension                    = L"Time";
    static FdoString* g_WmsElevationDimension               = L"Elevation";
    static FdoString* g_WmsSpatialContext                   = L"SpatialContext";
    static FdoString* g_WmsTrue                             = L"true";
    static FdoString* g_WmsFalse                            = L"false";
    static FdoString* g_WmsXmlnsValue                       = L"http://fdowms.osgeo.org/schemas";

    // These are the WMS supported image format, before version 3.3.0. Not extend in future.    
    static FdoString* g_WmsImageFormatPng                   = L"PNG";
    static FdoString* g_WmsImageFormatTif                   = L"TIF";
    static FdoString* g_WmsImageFormatJpg                   = L"JPG";
    static FdoString* g_WmsImageFormatGif                   = L"GIF";

    // For extend the WMS supported image format in the future
    static FdoString* g_MIMEImageFormats[]                  = {
                                                                L"image/png", 
                                                                L"image/tiff",
                                                                L"image/jpeg",
                                                                L"image/gif",
                                                                NULL
                                                              };

};


#endif // FDOWMSXMLGLOBALS_H
