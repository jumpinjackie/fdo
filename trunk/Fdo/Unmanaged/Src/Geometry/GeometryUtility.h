#ifndef _GEOMETRYUTILITY_H_
#define _GEOMETRYUTILITY_H_
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
*/

#ifdef _WIN32
#pragma once
#endif


#include <Geometry/GeometryStd.h>

class GeometryUtility
{
public:

    static const FdoInt32 GenerousCharsPerOrdinate = 30;     // Allow for full 64-bit precision, signs, decimal point, separators.

	static FdoString* DimensionalityToFgftString(FdoInt32 dimensionality);

    FDO_GEOM_API static FdoInt32 DimensionalityToNumOrdinates(FdoInt32 dimensionality);

    static wchar_t * AllocateStringForPositions(FdoInt32 dimensionality, FdoInt32 numPositions);

    static void AppendPositionToString(wchar_t * string, const FdoIDirectPosition * position); 

    FDO_GEOM_API static FdoString* CreateGeometryText(FdoIGeometry * geometry);

    static wchar_t * CreateFgftContentForPositions(FdoInt32 dimensionality, FdoDirectPositionCollection * positions);

    static wchar_t * CreateFgftContentForPolygon(const FdoIPolygon * polygon);

    static wchar_t * CreateFgftContentForCurvePolygon(const FdoICurvePolygon * polygon);

    static wchar_t * CreateFgftContentForCurveSegment(FdoICurveSegmentAbstract * segment);

    static wchar_t * CreateFgftContentForCurveSegments(FdoCurveSegmentCollection * segments);

protected:
    static FdoInt32 FormatNumber(double d, wchar_t* pszBuffer, FdoInt32 iLen, bool addSeparator);
    static FdoInt32 WritePositionToString(wchar_t * string, const FdoIDirectPosition * position); 
};
#endif


