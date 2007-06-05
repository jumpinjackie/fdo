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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoRfpXmlGlobals.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */
#ifndef FDOGRFPXMLGLOBALS_H
#define FDOGRFPXMLGLOBALS_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

namespace FdoGrfpXmlGlobals
{
    static FdoString* Georeference                  = L"Georeference";
    static FdoString* InsertionPointX               = L"InsertionPointX";
    static FdoString* InsertionPointY               = L"InsertionPointY";
    static FdoString* ResolutionX                   = L"ResolutionX";
    static FdoString* ResolutionY                   = L"ResolutionY";
    static FdoString* RotationX                     = L"RotationX";
    static FdoString* RotationY                     = L"RotationY";
    static FdoString* FrameNumber                   = L"frame";
    static FdoString* BandNumber                    = L"number";
    static FdoString* Feature                       = L"Feature";
    static FdoString* Band                          = L"Band";
    static FdoString* SchemaMapping		    = L"SchemaMapping";
    static FdoString* NameAttribute                 = L"name";
    static FdoString* ProviderAttribute		    = L"provider";
    static FdoString* ClassDefinitionElement	    = L"complexType";
    static FdoString* RasterDefinition		    = L"RasterDefinition";
    static FdoString* Location			    = L"Location";
    static FdoString* Image			    = L"Image";
    static FdoString* BoundingBox		    = L"BoundingBox";
    static FdoString* BoundingBoxQualified	    = L"gml:BoundingBox";
    static FdoString* Pos			    = L"Pos";
    static FdoString* PosQualified		    = L"gml:Pos";
    static FdoString* XMLNS_Value	            = L"http://fdogrfp.osgeo.org/schemas";
};


#endif
