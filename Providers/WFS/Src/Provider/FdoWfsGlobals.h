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

#ifndef FDOWFSGLOBALS_H
#define FDOWFSGLOBALS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

namespace FdoWfsGlobals
{
    static FdoString* FeatureTypeList                   = L"FeatureTypeList";
    static FdoString* Filter_Capabilities               = L"Filter_Capabilities";
    static FdoString* Operations                        = L"Operations";
    static FdoString* FeatureType                       = L"FeatureType";
    static FdoString* Query                             = L"Query";
    static FdoString* Update                            = L"Update";
    static FdoString* Insert                            = L"Insert";
    static FdoString* Delete                            = L"Delete";
    static FdoString* Lock                              = L"Lock";
    static FdoString* Name                              = L"Name";
    static FdoString* Title                             = L"Title";
    static FdoString* SRS                               = L"SRS";
    static FdoString* DefaultSRS                        = L"DefaultSRS";
    static FdoString* Abstract                          = L"Abstract";
    static FdoString* Keywords                          = L"Keywords";
    static FdoString* MetadataURL                       = L"MetadataURL";
    static FdoString* LatLongBoundingBox                = L"LatLongBoundingBox";
    static FdoString* WGS84BoundingBox                  = L"WGS84BoundingBox";
    static FdoString* minx                              = L"minx";
    static FdoString* miny                              = L"miny";
    static FdoString* maxx                              = L"maxx";
    static FdoString* maxy                              = L"maxy";
    static FdoString* name                              = L"name";
    static FdoString* Spatial_Capabilities              = L"Spatial_Capabilities";
    static FdoString* Scalar_Capabilities               = L"Scalar_Capabilities";
    static FdoString* Spatial_Operators                 = L"Spatial_Operators";
    static FdoString* SpatialOperators                  = L"SpatialOperators";
    static FdoString* SpatialOperator                   = L"SpatialOperator";
    static FdoString* Intersect                         = L"Intersect";
    static FdoString* Intersects                        = L"Intersects";
    static FdoString* BBOX                              = L"BBOX";
    static FdoString* Contains                          = L"Contains";
    static FdoString* Crosses                           = L"Crosses";
    static FdoString* Disjoint                          = L"Disjoint";
    static FdoString* Equals                            = L"Equals";
    static FdoString* Overlaps                          = L"Overlaps";
    static FdoString* Touches                           = L"Touches";
    static FdoString* Within                            = L"Within";
    static FdoString* DWithin                           = L"DWithin";
    static FdoString* Beyond                            = L"Beyond";
    static FdoString* Logical_Operators                 = L"Logical_Operators";
    static FdoString* LogicalOperators                  = L"LogicalOperators";
    static FdoString* Comparison_Operators              = L"Comparison_Operators";
    static FdoString* ComparisonOperators               = L"ComparisonOperators";
    static FdoString* ComparisonOperator                = L"ComparisonOperator";
    static FdoString* Arithmetic_Operators              = L"Arithmetic_Operators";
    static FdoString* ArithmeticOperators               = L"ArithmeticOperators";
    static FdoString* Simple_Comparisons                = L"Simple_Comparisons";
    static FdoString* LessThan                          = L"LessThan";
    static FdoString* GreaterThan                       = L"GreaterThan";
    static FdoString* LessThanEqualTo                   = L"LessThanEqualTo";
    static FdoString* GreaterThanEqualTo                = L"GreaterThanEqualTo";
    static FdoString* EqualTo                           = L"EqualTo";
    static FdoString* NotEqualTo                        = L"NotEqualTo";
    static FdoString* Like                              = L"Like";
    static FdoString* NullCheck                         = L"NullCheck";
    static FdoString* Between                           = L"Between";
    static FdoString* Simple_Arithmetic                 = L"Simple_Arithmetic";
    static FdoString* SimpleArithmetic                  = L"SimpleArithmetic";
    static FdoString* WFS                               = L"WFS";
    static FdoString* WfsVersion                        = L"1.0.0";
    static FdoString* WfsVersion110                     = L"1.1.0";

    static FdoString* DescribeFeatureType               = L"DescribeFeatureType";
    static FdoString* GetFeature                        = L"GetFeature";
    static FdoString* TYPENAME                          = L"TYPENAME";
    static FdoString* Comma                             = L",";
    static FdoString* import                            = L"import";
    static FdoString* include                           = L"include";
    static FdoString* NameSpace                         = L"namespace";
    static FdoString* schemaLocation                    = L"schemaLocation";
    static FdoString* HTTPProtocal                      = L"http://";
    static size_t SizeHTTPProtocal                      = 7;
    static wchar_t SLASH                                = L'/';
    static FdoString* annotation                        = L"annotation";
    static FdoString* fdo_customer                      = L"fdo.osgeo.org/schemas/feature";

    static FdoString* FeatureServer                     = L"FeatureServer";
    static FdoString* Username                          = L"Username";
    static FdoString* Password                          = L"Password";
    static FdoString* PROPERTYNAME                      = L"PROPERTYNAME";
    static FdoString* Filter                            = L"Filter";
    static FdoString* OgcUri                            = L"http://www.opengis.net/ogc";
    static FdoString* appns                             = L"appns";

    static FdoString* Equal                             = L"=";
    static FdoString* And                               = L"&";

    static FdoString* AbstractFeature                   = L"AbstractFeature";
    static FdoString* GMLSchemaName                     = L"gml";
    static FdoString* XLinkSchemaName                   = L"xlink";

    static FdoString* Dot                               = L"-dot-";

}

#endif

