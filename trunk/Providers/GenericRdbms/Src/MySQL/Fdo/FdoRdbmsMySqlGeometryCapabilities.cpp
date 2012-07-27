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

#include "stdafx.h"
#include "FdoRdbmsMySqlGeometryCapabilities.h"


FdoGeometryType* FdoRdbmsMySqlGeometryCapabilities::GetGeometryTypes( FdoInt32& length)
{
    static FdoGeometryType  rdbmGeomTypes[] = {
                                        FdoGeometryType_Point,
                                        FdoGeometryType_LineString,
                                        FdoGeometryType_Polygon,
                                        FdoGeometryType_MultiPoint,
                                        FdoGeometryType_MultiLineString,
                                        FdoGeometryType_MultiPolygon
                                     //   FdoGeometryType_MultiGeometry,

                                     // MySQL Provider currently doesn't support curve segments:
                                     //   FdoGeometryType_CurveString,
                                     //   FdoGeometryType_CurvePolygon,
                                     //   FdoGeometryType_MultiCurveString,
                                     //   FdoGeometryType_MultiCurvePolygon
                                            };
    length = sizeof(rdbmGeomTypes)/sizeof(FdoGeometryType);
    return rdbmGeomTypes;
}

FdoGeometryComponentType* FdoRdbmsMySqlGeometryCapabilities::GetGeometryComponentTypes( FdoInt32& length )
{
    static FdoGeometryComponentType  rdbmsGeomCompTypes[] = {
                                        // MySQL Provider currently doesn't support curve segments
                                        FdoGeometryComponentType_LinearRing,
                                        // FdoGeometryComponentType_LineStringSegment

                                     // MySQL Provider currently doesn't support curve segments:
                                     // FdoGeometryComponentType_CircularArcSegment,
                                     // FdoGeometryComponentType_Ring
                                    };
    length = sizeof( rdbmsGeomCompTypes )/sizeof( FdoGeometryComponentType );
    return rdbmsGeomCompTypes;
}
