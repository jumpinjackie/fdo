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
 * Revision Control Modification History
 *
 *         $Id:  $
 *     $Author:  $
 *   $DateTime:  $
 *     $Change:  $
 * 
 */

#ifndef TESTCOMMONGEOMUTIL___H
#define TESTCOMMONGEOMUTIL___H	1

#ifdef _WIN32
#pragma once
#endif


//
// Geometry-based test utilities common to many providers
class TestCommonGeomUtil
{
public:
    // Compare two geometries for equivalence:
    static bool GeometriesEquivalent (FdoIGeometry *geom1, FdoIGeometry *geom2);
    static bool LinearRingsEquivalent (FdoILinearRing* lr1, FdoILinearRing* lr2);
    static bool PointsEquivalent (FdoIDirectPosition* pos1, FdoIDirectPosition* pos2);

    // Print out info about the given geometry (type, length, area, etc):
    static void PrintGeometryAnalysis( const wchar_t *class_name, int index, FdoByteArray * geom_fgf, double length, double area, bool verbose);
};

#endif // TESTCOMMONGEOMUTIL___H
