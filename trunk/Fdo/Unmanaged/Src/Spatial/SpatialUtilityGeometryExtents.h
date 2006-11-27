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

#ifndef FdoSpatialUtilityGeometryExtents___H
#define FdoSpatialUtilityGeometryExtents___H	1

#include <math.h>
#include <FdoCommon.h>

class FdoSpatialUtilityGeometryExtents
{
public:
    static void GetExtents(FdoByteArray* fgfArray, double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ);

private:
    // 2d vector dot product
    static inline double dotProduct(double x1, double y1, double x2, double y2)
    {
        return x1 * x2 + y1 * y2;
    }

    // 3d vector dot product
    static inline double dotProduct(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return x1 * x2 + y1 * y2 + z1 * z2;
    }
    
    // distance between two 2d points
    static inline double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    // distance between two 3d points
    static inline double distance(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
    }

    static double angleBetweenVector(double x1, double y1, double x2, double y2);
    static double angleBetweenVector(double x1, double y1, double z1, double x2, double y2, double z2);
    
    static void calculateArcCenter(double x1, double y1, double x2, double y2, double x3, double y3, double& xc, double& yc, bool &isArcValid);
    static void calculateArcCenter(double x1, double y1, double z1,
                            double x2, double y2, double z2,
                            double x3, double y3, double z3,
                            double& xc, double& yc, double& zc, bool &isArcValid);
   
    static void calculateArcStartAndEndAngle(double x1, double y1, double x2, double y2, double x3, double y3,
                                      double xc, double yc,
                                      double& startAngle, double& endAngle);

    
    static int  getQuadrantIndex(double angle);
    static void getSinExtremum(double startAngle, double endAngle, double& minValue, double& maxValue);
    static void getCosExtremum(double startAngle, double endAngle, double& minValue, double& maxValue);

    static void getArcExtents(double x1, double y1, double x2, double y2, double x3, double y3,
                       double& minX, double& minY, double& maxX, double& maxY);
    static void getArcExtents(double x1, double y1, double z1,
                       double x2, double y2, double z2,
                       double x3, double y3, double z3,
                       double& minX, double& minY, double& minZ,
                       double& maxX, double& maxY, double& maxZ);

    static void getMatrix(double x, double y, double z, double matrix[4][4]);
    static void transformByMatrix(double& x, double& y, double& z, const double matrix[4][4]); 


    static void getExtentsWithoutCurve(FdoByteArray* fgfArray,
                                double& minX, double& minY, double& minZ,
                                double& maxX, double& maxY, double& maxZ);;
    static void getExtentsWithCurve(FdoByteArray* fgfArray, 
                             double& minX, double& minY, double& minZ,
                             double& maxX, double& maxY, double& maxZ);

    // standard angle, make it in range [0, 2PI)
    static void standardAngle(double& angle);

    static void readPoint(double** dreader, double& x, double& y, double& z, bool processZ, int skip_vals);
};

#endif // FdoSpatialUtilityGeometryExtents___H

