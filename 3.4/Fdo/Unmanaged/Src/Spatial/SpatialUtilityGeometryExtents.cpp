/*
 * 
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

#include "SpatialUtilityGeometryExtents.h"
#include "SpatialEnvelope.h"
#include "MathUtility.h"
#include <math.h>

#define ONE_AND_HALF_PI     (M_PI * 1.5)


// calculate the angle between two vectors
// return angle will be [0, PI)
double FdoSpatialUtilityGeometryExtents::angleBetweenVector(double x1, double y1, double x2, double y2)
{
    double length1 = sqrt(x1 * x1 + y1 * y1);
    double length2 = sqrt(x2 * x2 + y2 * y2);
    if (FdoMathUtility::SnapToZero(length1) == 0 || FdoMathUtility::SnapToZero(length2) == 0)
        return 0;
    else
        return acos(dotProduct(x1, y1, x2, y2)/(length1 * length2));
}

double FdoSpatialUtilityGeometryExtents::angleBetweenVector(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double length1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    double length2 = sqrt(x2 * x2 + y2 * y2 + z2 * z2);
    if (FdoMathUtility::SnapToZero(length1) == 0 || FdoMathUtility::SnapToZero(length2) == 0)
        return 0;
    else
        return acos(dotProduct(x1, y1, z1, x2, y2, z2)/(length1 * length2));
}

// (x1, y1), (x2, y2), (x3, y3) are three points on the arc
// (xc, yc) is the ouputing center point of the arc
void FdoSpatialUtilityGeometryExtents::calculateArcCenter(
    double x1, double y1, double x2,
    double y2, double x3, double y3,
    double& xc, double& yc,
    bool &isArcValid)
{
    double t1 = 0.5 * ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2));
    double t2 = 0.5 * ((x3 * x3 - x2 * x2) + (y3 * y3 - y2 * y2));
    isArcValid = true;

    if (FdoMathUtility::SnapToZero(((x1 - x2) * (y3 - y2) - (x3 - x2) * (y1 - y2))) == 0)
    {
        // Zero result means that points were collinear.
        isArcValid = false;
    }
    else
    {
        xc = (t1 * (y3 - y2) - t2 * (y1 - y2)) / ((x1 - x2) * (y3 - y2) - (x3 - x2) * (y1 - y2));
        yc = (t1 * (x3 - x2) - t2 * (x1 - x2)) / ((y1 - y2) * (x3 - x2) - (y3 - y2) * (x1 - x2));
    }
}


// (x1, y1, z1), (x2, y2, z2), (x3, y3, z3) are the three points on the arc
// (xc, yc, zc) is the ouputing center point of the arc
void FdoSpatialUtilityGeometryExtents::calculateArcCenter(
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    double x3, double y3, double z3,
    double& xc, double& yc, double& zc,
    bool &isArcValid)
{
    double a[9], b[3], tmp;
    double A[9], matrixA;
    isArcValid = true;

    a[0] = x2 - x1;     /* x21 */
    a[1] = y2 - y1;     /* y21 */
    a[2] = z2 - z1;     /* z21 */

    a[3] = x3 - x1;     /* x31 */
    a[4] = y3 - y1;     /* y31 */
    a[5] = z3 - z1;     /* z31 */

              /* y21 * z31 - y31 * z21 */
    a[6] = a[1] * a[5] - a[4] * a[2]; /* Dyz */
              /* z21 * x31 - z31 * x21 */
    a[7] = a[2] * a[3] - a[5] * a[0]; /* Dzx */
              /* x21 * y31 - x31 * y21 */
    a[8] = a[0] * a[4] - a[3] * a[1]; /* Dxy */

    tmp = x1 * x1 + y1 * y1 + z1 * z1;

    b[0] = x2 * x2 + y2 * y2 + z2 * z2;

    b[1] = x3 * x3 + y3 * y3 + z3 * z3;

    b[0] -= tmp;
    b[1] -= tmp;
    b[0] *= 0.5;
    b[1] *= 0.5;

    /* Dxyz = x1 * Dyz + y1 * Dzx + z1 * Dxy */
    b[2] = x1 * a[6] + y1 * a[7] + z1 * a[8];

    A[0] = a[4] * a[8] - a[7] * a[5];
    A[3] = -(a[1] * a[8] - a[7] * a[2]);
    A[6] = a[1] * a[5] - a[4] * a[2];
    matrixA = a[0] * A[0] + a[3] * A[3] + a[6] * A[6];

    if ( FdoMathUtility::SnapToZero(matrixA) == 0)
    {
        // Zero result means that points were collinear.
        isArcValid = false;
    }
    else
    {
        A[1] = -(a[3] * a[8] - a[6] * a[5]);
        A[4] = a[0] * a[8] - a[6] * a[2];
        A[7] = -(a[0] * a[5] - a[3] * a[2]);
        
        A[2] = a[3] * a[7] - a[6] * a[4];
        A[5] = -(a[0] * a[7] - a[6] * a[1]);
        A[8] = a[0] * a[4] - a[3] * a[1];

        xc = (A[0] * b[0] + A[3] * b[1] + A[6]* b[2]) / matrixA;
        yc = (A[1] * b[0] + A[4] * b[1] + A[7]* b[2]) / matrixA;
        zc = (A[2] * b[0] + A[5] * b[1] + A[8]* b[2]) / matrixA;
    }
}

// (x1, y1) (x3, y3) are the start and end points
// (x2, y2) is the middle point
// (xc, yc) is the center point
void FdoSpatialUtilityGeometryExtents::calculateArcStartAndEndAngle(double x1, double y1, double x2, double y2, double x3, double y3,
                                                                  double xc, double yc,
                                                                  double& startAngle, double& endAngle)
{
    double v1x = x1 - xc;
    double v1y = y1 - yc;
    double v2x = x2 - xc;
    double v2y = y2 - yc;
    double v3x = x3 - xc;
    double v3y = y3 - yc;
    double cv1x = -v1y;
    double cv1y = v1x;

    // if v1 is not anticlockwise to v2
    // use v3 to calculate the start angle
    if (dotProduct(cv1x, cv1y, v2x, v2y) < 0)
    {
        double vtx = v1x;
        double vty = v1y;
        v1x = v3x;
        v1y = v3y;
        v3x = vtx;
        v3y = vty;
    }

    startAngle = angleBetweenVector(v1x, v1y, 1, 0);
    if (v1y < 0)
        startAngle = TWO_PI - startAngle;

    endAngle = angleBetweenVector(v3x, v3y, 1, 0);
    if (v3y < 0)
        endAngle = TWO_PI - endAngle;
}

// assure the angle is in [0, 2*PI)
void FdoSpatialUtilityGeometryExtents::standardAngle(double& angle)
{
    while ( angle < 0.0 )
        angle += TWO_PI;

    while ( angle >= TWO_PI )
        angle -= TWO_PI;
}

int FdoSpatialUtilityGeometryExtents::getQuadrantIndex(double angle)
{
    if (angle < M_PI_2)
        return 0;
    else if (angle < M_PI)
        return 1;
    else if (angle < ONE_AND_HALF_PI)
        return 2;
    else
        return 3;

}

// startAngle and endAngle must be in [0, 2*PI)
// if startAngle <= endAngle, it will find the max and min sin value in the range [startAngle, endAngle]
// otherwise, it will find the max and min sin value in the range [startAngle, 2*PI + endAngle]
void FdoSpatialUtilityGeometryExtents::getSinExtremum(double startAngle, double endAngle, double& minValue, double& maxValue)
{
    maxValue = -1;
    minValue = 1;

    // construct a 20 X 2 matrix to find the max and min sin value
    // calculate startAngle and endAngle quadrant, there should be in [0, 3]
    //  if i == j and startAngle > endAngle set j = 4 ( beacuse in this case, we cannot know correct
    //  relationship between startAnlge and endAngle, for example, assumn i=j=0, if startAngle <= endAngle,
    //  min and max value will be min and max sin(startAngle, endAngle), otherwise min and max vale will be -1 and 1
    // thus i * 5 + j represent the row index of the matrix
    // and col 0, 1 represent the min, max sin value
    // value "2" represents that the value should be min or max(sin(startAngle), sin(startAngle)), 
    //  otherwise, the value is the true value;
    static int table[20][2] = {{2, 2}, {2, 1}, {2, 1}, {-1, 1}, {-1, 1},
                              {-1, 2}, {2, 2}, {2, 2}, {-1, 2},{-1, 1},
                              {-1, 2}, {-1, 1}, {2, 2}, {-1, 2},{-1, 1},
                              {2, 2}, {2, 1}, {2, 1}, {2, 2}, {-1, 1}};
    
    int quadrant1 = getQuadrantIndex(startAngle);
    int quadrant2 = getQuadrantIndex(endAngle);
    if (quadrant1 == quadrant2 && startAngle > endAngle)
        quadrant2 = 4;
    
    int index = quadrant1 * 5 + quadrant2;
    int minRef = table[index][0];
    int maxRef = table[index][1];
    double sinStart = sin(startAngle);
    double sinEnd = sin(endAngle);

    if (minRef == 2)
        minValue = sinStart < sinEnd ? sinStart : sinEnd; 
    else
        minValue = (double)minRef;

    if (maxRef == 2)
        maxValue = sinStart > sinEnd ? sinStart : sinEnd; 
    else
        maxValue = (double)maxRef;
}

// startAngle and endAngle must be in [0, 2*PI)
// if startAngle <= endAngle, it will find the max and min cos value in the range [startAngle, endAngle]
// otherwise, it will find the max and min cos value in the range [startAngle, 2*PI + endAngle]
// same methanism to getSinExtremum
void FdoSpatialUtilityGeometryExtents::getCosExtremum(double startAngle, double endAngle, double& minValue, double& maxValue)
{
    maxValue = -1;
    minValue = 1;

    static int table[20][2] = {{2, 2}, {2, 2}, {-1, 2}, {-1, 2},{-1, 1},
                              {-1, 1}, {2, 2}, {-1, 2}, {-1, 2},{-1, 1},
                              {2, 1}, {2, 1}, {2, 2}, {2, 2},{-1,1},
                              {2, 1}, {2, 1}, {-1, 1}, {2, 2}, {-1, 1}};
    
    int quadrant1 = getQuadrantIndex(startAngle);
    int quadrant2 = getQuadrantIndex(endAngle);    
    if (quadrant1 == quadrant2 && startAngle > endAngle)
        quadrant2 = 4;
    
    int index = quadrant1 * 5 + quadrant2;
    int minRef = table[index][0];
    int maxRef = table[index][1];
    double cosStart = cos(startAngle);
    double cosEnd = cos(endAngle);

    if (minRef == 2)
        minValue = cosStart < cosEnd ? cosStart : cosEnd;  
    else
        minValue = (double)minRef;

    if (maxRef == 2)
        maxValue = cosStart > cosEnd ? cosStart : cosEnd;  
    else
        maxValue = (double)maxRef;
}

// get the matrix rotating (x, y, z) to Z Axis
void FdoSpatialUtilityGeometryExtents::getMatrix(double x, double y, double z, double matrix[4][4])
{
    double cosa = 1;
    double sina = 0;
    
    double v = sqrt(y * y + z * z);
    if (v != 0)
    {
        cosa = z/v;
        sina = y/v;
    }
    
    double u = sqrt(x * x + y * y + z * z);
    double cosb = v/u;
    double sinb = -x/u;

    matrix[0][0] = cosb;
    matrix[0][1] = 0;
    matrix[0][2] = -sinb;
    matrix[1][0] = sina * sinb;
    matrix[1][1] = cosa;
    matrix[1][2] = sina * cosb;
    matrix[2][0] = cosa * sinb;
    matrix[2][1] = -sina;
    matrix[2][2] = cosa * cosb;

    int i;
    for(i = 0; i < 3; i++) 
    {
        matrix[3][i] = 0;
        matrix[i][3] = 0;
    }

    matrix[3][3] = 1;
}

void FdoSpatialUtilityGeometryExtents::transformByMatrix(double& x, double& y, double& z, const double matrix[4][4])
{
    double tx = x;
    double ty = y;
    double tz = z;

    x = tx * matrix[0][0] + ty * matrix[1][0] + tz * matrix[2][0] + matrix[3][0];
    y = tx * matrix[0][1] + ty * matrix[1][1] + tz * matrix[2][1] + matrix[3][1];
    z = tx * matrix[0][2] + ty * matrix[1][2] + tz * matrix[2][2] + matrix[3][2];
}

// (x1, y1), (x3, (y3) is the start and end points of the arc
// (x2, y2) is a point on the arc
// this method will return the min and max (x, y) of the arc
void FdoSpatialUtilityGeometryExtents::getArcExtents(double x1, double y1, double x2, double y2, double x3, double y3,
                                                   double& minX, double& minY, double& maxX, double& maxY)
{
    double xc, yc;
    double startAngle, endAngle;
    double maxValue, minValue;
    double radius;
    bool isArcValid;

    // calculate the cener and radius of the arc
    // get the start angle and end angle of the arc, they are in [0, 2*PI)
    calculateArcCenter(x1, y1, x2, y2, x3, y3, xc, yc, isArcValid);
    if (isArcValid)
    {
        radius = distance(x2, y2, xc, yc);
        calculateArcStartAndEndAngle(x1, y1, x2, y2, x3, y3, xc, yc, startAngle, endAngle);
        
        // x = xc + radius*cos(a)   
        // y = yc + radius*sin(a)
        // a is in [startAngle, endAngle] or [startAngle, endAngle+2*PI] (if startAngle > endAngle)
        // get the max and min value of the cos and sin function 
        // calculate max and min x
        getCosExtremum(startAngle, endAngle, minValue, maxValue);    
        minX = radius * minValue + xc;
        maxX = radius * maxValue + xc;        
        
        // calculate max and mix y
        getSinExtremum(startAngle, endAngle, minValue, maxValue);    
        minY = radius * minValue + yc;
        maxY = radius * maxValue + yc;
    }
    else
    {
        // Fall back to using three known positions on the arc.
        FdoSpatialEnvelope bound;
        bound.AddBound(x1, y1, 0.0, false);
        bound.AddBound(x2, y2, 0.0, false);
        bound.AddBound(x3, y3, 0.0, false);
        minX = bound.minx;
        minY = bound.miny;
        maxX = bound.maxx;
        maxY = bound.maxy;
    }
}

// (x1, y1, z1), (x2, y2, z2), (x3, y3, z3) are the three points on the arc
void FdoSpatialUtilityGeometryExtents::getArcExtents(double x1, double y1, double z1,
                                                   double x2, double y2, double z2,
                                                   double x3, double y3, double z3,
                                                   double& minX, double& minY, double& minZ,
                                                   double& maxX, double& maxY, double& maxZ)
{
    double xc, yc, zc;
    bool isArcValid;
    calculateArcCenter(x1, y1, z1, x2, y2, z2, x3, y3, z3, xc, yc, zc, isArcValid);

    // Normal of the plane described by the arc.
    double vnx = 0.0;
    double vny = 0.0;
    double vnz = 0.0;

    if (isArcValid)
    {
        // Take the cross product = (p2-p1) X (p3-p1) to get the normal of the plane

        double v21[3], v31[3];

        v21[0] = x2 - x1;
        v21[1] = y2 - y1;
        v21[2] = z2 - z1;

        v31[0] = x3 - x1;
        v31[1] = y3 - y1;
        v31[2] = z3 - z1;

        vnx = FdoMathUtility::SnapToZero(v21[1] * v31[2] - v31[1] * v21[2]);
        vny = FdoMathUtility::SnapToZero(v21[2] * v31[0] - v31[2] * v21[0]);
        vnz = FdoMathUtility::SnapToZero(v21[0] * v31[1] - v31[0] * v21[1]);

        if (distance(vnx, vny, vnz, 0, 0, 0) == 0)
        {
            // Zero result means that points were collinear.  This should have been 
            // caught earlier, but we'll trigger the fallback here anyway.
            isArcValid = false;
        }
    }

    if (isArcValid)
    {
        // vector v1, v2, v3 -- vectors from center to each of the three arc positions
        double v1x = x1 - xc;
        double v1y = y1 - yc;
        double v1z = z1 - zc;
        double v2x = x2 - xc;
        double v2y = y2 - yc;
        double v2z = z2 - zc;
        double v3x = x3 - xc;
        double v3y = y3 - yc;
        double v3z = z3 - zc;

        // get the transform matrix rotating vector(vnx, vny, vnz) to Z axis
        double matrix[4][4];
        getMatrix(vnx, vny, vnz, matrix);    

        // transform all vector v1, v2, v3 to XY plane
        transformByMatrix(v1x, v1y, v1z, matrix);
        transformByMatrix(v2x, v2y, v2z, matrix);
        transformByMatrix(v3x, v3y, v3z, matrix);
      
        // (v1x, v1y), (v2x, v2y), (v3x, v3y) define a arc in XY plane
        // and the arc center is (0, 0)
        // get the start and end angle in the XY plane, calculate the radius of the arc
        double startAngle, endAngle;
        calculateArcStartAndEndAngle(v1x, v1y, v2x, v2y, v3x, v3y, 0, 0, startAngle, endAngle);
        double radius = sqrt(v2x * v2x + v2y * v2y + v2z * v2z);
      
        // --------------------------------------------------------------------------------------------------------
        // 
        // at this time, the arc point is (r*cos(a), r*sin(a), 0)
        // convert the point to the original plane by transforming the point by the invert matrix
        // in fact, the invert matrix is the transpose of the matrix;
        // so after transforming, the point will be
        // x = r*cos(a)*matrix[0][0] + r*sin(a)*matrix[0][1], 
        // y = r*cos(a)*matrix[1][0] + r*sin(a)*matrix[1][1], 
        // z = r*cos(a)*matrix[2][0] + r*sin(a)*matrix[2][1]
        // next we will calculate the min and max value of equation: 
        //    cos(a)*matrix[i][0] + sin(a)*matrix[i][1] which i in [0, 2] and a in (startAngle and endAngle)
        // assume u=matrix[i][0] and v = maxtrix[i][1], and s = sqrt(u*u+v*v), 
        // then the equation will be s * (cos(a)*(u/s) + sin(a)*(v/s))
        // find an angle which cos(angle)=(v/s) and sin(angle)=(u/s)
        // finally, the equation will be r*s*sin(a+angle), then call the getSinExtremum function to get the answer
        // 
        // --------------------------------------------------------------------------------------------------------
        double minValue[3], maxValue[3];
        int i;
        for (i = 0; i < 3; i++)
        {
            double u = matrix[i][0];
            double v = matrix[i][1];
            double s = sqrt(u * u + v * v);
            if (s == 0)
            {
                minValue[i] = 0;
                maxValue[i] = 0;
            }
            else
            {
                double angle = acos(v/s);
                if (u < 0 )
                    angle = TWO_PI - angle;
                double tmpStartAngle = startAngle + angle;
                double tmpEndAngle = endAngle + angle;
                standardAngle(tmpStartAngle);
                standardAngle(tmpEndAngle);
                getSinExtremum(tmpStartAngle, tmpEndAngle, minValue[i], maxValue[i]);
                minValue[i] *= s;
                maxValue[i] *= s;
            }
        }
        
        minX = radius * minValue[0] + xc;
        minY = radius * minValue[1] + yc;
        minZ = radius * minValue[2] + zc;
        
        maxX = radius * maxValue[0] + xc;        
        maxY = radius * maxValue[1] + yc;        
        maxZ = radius * maxValue[2] + zc;        
    }
    else
    {
        // Fall back to using three known positions on the arc.
        FdoSpatialEnvelope bound;
        bound.AddBound(x1, y1, z1, true);
        bound.AddBound(x2, y2, z1, true);
        bound.AddBound(x3, y3, z1, true);
        minX = bound.minx;
        minY = bound.miny;
        minZ = bound.minz;
        maxX = bound.maxx;
        maxY = bound.maxy;
        maxZ = bound.maxz;
    }

}

void FdoSpatialUtilityGeometryExtents::readPoint(double** dreader, double& x, double& y, double& z, bool processZ, int skip_vals)
{
    x = *(*dreader)++; 
    y = *(*dreader)++;
    if (processZ)
        z = *(*dreader)++;
    *dreader += skip_vals;
}

static inline void PositionStreamCheckAndAdvance(
    FdoInt32 positionCount,
    FdoInt32 ordinatesPerPosition,
    FdoInt32 &byteCount)
{
    FdoInt32 bytesForPositions = positionCount * ordinatesPerPosition * sizeof(double);
    if (byteCount < bytesForPositions)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
    byteCount -= bytesForPositions;
}

static inline void IntegerStreamSizeCheck(
    FdoInt32 *&ireader,
    FdoInt32 byteCount)
{
    // Note:  force signed comparison, in case byteCount becomes negative.
    if (byteCount < (FdoInt32) sizeof(FdoInt32))
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
}

static inline FdoInt32 IntegerFromStream(
    FdoInt32 *&ireader,
    FdoInt32 &byteCount)
{
    IntegerStreamSizeCheck(ireader, byteCount);
    FdoInt32 value = *ireader++;
    byteCount -= sizeof(FdoInt32);
    return value;
}

void FdoSpatialUtilityGeometryExtents::getExtentsWithoutCurve(FdoByteArray* fgfArray, 
                                                            double& minX, double& minY, double& minZ,
                                                            double& maxX, double& maxY, double& maxZ)
{
    unsigned char* fgf = fgfArray->GetData();
    FdoInt32 byteCount = fgfArray->GetCount();
    int* ireader = (int*)fgf;
    
    // the geometry type
    int geom_type = (FdoGeometryType) IntegerFromStream(ireader, byteCount);

    FdoSpatialEnvelope bound;

    FdoDimensionality geom_dim;
    bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
        || (geom_type == FdoGeometryType_MultiPolygon
        || (geom_type == FdoGeometryType_MultiPoint));

    // the coordinate type
    int skip_vals = 0; 
    //temp variables used inside the loop
    int num_geoms = 1;
    double x;
    double y;
    double z = 0;
    int point_count;
    int contour_count;
    double* dreader;

    //in case of multipolygon or multilinestring or multipoint, 
    //read poly or linestring count
    if (is_multi) num_geoms = IntegerFromStream(ireader, byteCount);

    for (int q = 0; q < num_geoms; q++)
    {
        //skip past geometry type of subgeometry
        //we know it is LineString or Polygon or Point respectively
        if (is_multi)
        {
            ireader++; 
            byteCount -= sizeof(*ireader);
        }

        //read cordinate typeB
        FdoDimensionality sub_geom_dim = (FdoDimensionality) IntegerFromStream(ireader, byteCount);

        // ensure that all dimensionalities of each geometry are the same
        // _ASSERT(q==0 ? true : geom_dim == sub_geom_dim);

        geom_dim = sub_geom_dim;
        bool processZ = (geom_dim & FdoDimensionality_Z);

        skip_vals = 0;
        if (geom_dim & FdoDimensionality_M) skip_vals++;

        // the number of contours in current polygon/linestring
        contour_count = 1; //for linestrings

        switch (geom_type)
        {
            case FdoGeometryType_Polygon:
            case FdoGeometryType_MultiPolygon:
                contour_count = IntegerFromStream(ireader, byteCount);
            default: break;
        }

        for (int i=0; i<contour_count; i++)
        {
            point_count = 1;

            //point geoms do not have a point count, since 
            //each piece is just one point each
            if ((geom_type != FdoGeometryType_MultiPoint) 
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = IntegerFromStream(ireader, byteCount);
            }

            int ords_per_position = 2 /*X,Y*/ + (processZ?1:0) + skip_vals /*M*/;
            PositionStreamCheckAndAdvance(
                point_count, ords_per_position, byteCount);

            //*** ireader not valid from here down
            dreader = (double*) ireader;

            int num_pts_read = 0;
            
            while (num_pts_read  < point_count)
            {
                x = *dreader++;
                y = *dreader++;
                if (processZ)
                    z = *dreader++;

                //skip past z and m
                switch (skip_vals)
                {
                    case 2: dreader++;
                    case 1: dreader++;
                    default:break;
                }

                
                //-------------------------------------------------
                //
                // Faster min/max computation, not using the FPU
                // This code is 2.4 times faster than doing an FPU
                // comparison on the latest Intel processor
                // IN THE CONTEXT OF THIS LOOP, NOT IN GENERAL!
                //
                //-------------------------------------------------
                bound.AddBound(x, y, z, processZ);

                num_pts_read++;
            }

            ireader = (int*)dreader;
            //*** ireader valid again
        }
    }

    bool hasZ = (geom_dim & FdoDimensionality_Z);   
    
    minX = bound.minx;
    minY = bound.miny;
    minZ = hasZ ? bound.minz : 0;
    maxX = bound.maxx;
    maxY = bound.maxy;
    maxZ = hasZ ? bound.maxz : 0;
    
}

void FdoSpatialUtilityGeometryExtents::getExtentsWithCurve(FdoByteArray* fgfArray, 
                                                         double& minX, double& minY, double& minZ,
                                                         double& maxX, double& maxY, double& maxZ)
{
    unsigned char* fgf = fgfArray->GetData();
    FdoInt32 byteCount = fgfArray->GetCount();
    int* ireader = (int*)fgf;

    // the geometry type
    int geom_type = IntegerFromStream(ireader, byteCount);
    
    FdoSpatialEnvelope bound;

    FdoDimensionality geom_dim;
    bool is_multi = (geom_type == FdoGeometryType_MultiCurveString) 
        || (geom_type == FdoGeometryType_MultiCurvePolygon);

    // the coordinate type
    int skip_vals = 0; 
    int num_geoms = 1;
    double x;
    double y;
    double z = 0;

    //in case of multicurvepolygon or multicurvestring 
    //read poly or linestring count
    if (is_multi) num_geoms = IntegerFromStream(ireader, byteCount);

    for (int q = 0; q < num_geoms; q++)
    {
        //skip past geometry type of subgeometry
        //we know it is CurveString or CurvePolygon respectively
        if (is_multi)
        {
            ireader++;
            byteCount -= sizeof(*ireader);
        }

        //read cordinate type
        FdoDimensionality sub_geom_dim = (FdoDimensionality) IntegerFromStream(ireader, byteCount);

        // ensure that all dimensionalities of each geometry are the same
        // _ASSERT(q==0 ? true : geom_dim == sub_geom_dim);

        geom_dim = sub_geom_dim;
        bool processZ = (geom_dim & FdoDimensionality_Z);

        skip_vals = 0;
        if (geom_dim & FdoDimensionality_M) skip_vals++;

        int ords_per_position = 2 /*X,Y*/ + (processZ?1:0) + skip_vals /*M*/;

        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings, no rings, just one

        if ((geom_type == FdoGeometryType_CurvePolygon) 
            || (geom_type == FdoGeometryType_MultiCurvePolygon))
            contour_count = IntegerFromStream(ireader, byteCount); //#rings for polygons

        for (int i = 0; i < contour_count; i++)
        {
            //*** ireader not valid from here down
            double *dreader = (double*) ireader;

            PositionStreamCheckAndAdvance(1, ords_per_position, byteCount);
            readPoint(&dreader, x, y, z, processZ, skip_vals);

            // ireader valid again
            ireader = (int*)dreader;
            int seg_count = IntegerFromStream(ireader, byteCount); //# curve segments

            for (int j = 0; j < seg_count; j++)
            {
                int seg_type = IntegerFromStream(ireader, byteCount);

                switch (seg_type)
                {
                case FdoGeometryComponentType_CircularArcSegment:
                    {
                        dreader = (double*)ireader;
                        PositionStreamCheckAndAdvance(2, ords_per_position, byteCount);

                        //circular arc : read midpont and endpoint
                        //first point was either the start point or
                        //the last point if the previous segment

                        double midx;
                        double midy;
                        double midz = 0;

                        readPoint(&dreader, midx, midy, midz, processZ, skip_vals);

                        double endx;
                        double endy; 
                        double endz = 0;

                        readPoint(&dreader, endx, endy, endz, processZ, skip_vals);

                        if (processZ)
                        {
                            double minX, minY, minZ;
                            double maxX, maxY, maxZ;
                            getArcExtents(x, y, z, midx, midy, midz, endx, endy, endz, minX, minY, minZ, maxX, maxY, maxZ);
                            bound.AddMinBound(minX, minY, minZ, true);
                            bound.AddMaxBound(maxX, maxY, maxZ, true);
                        }
                        else
                        {
                            double minX, minY;
                            double maxX, maxY;
                            getArcExtents(x, y, midx, midy, endx, endy, minX, minY, maxX, maxY);
                            bound.AddMinBound(minX, minY, 0, false);
                            bound.AddMaxBound(maxX, maxY, 0, false);
                        }
                        
                        x = endx;
                        y = endy;
                        z = endz;

                        ireader = (int*) dreader;

                        break;
                    }
                case FdoGeometryComponentType_LineStringSegment:
                    {
                        bound.AddBound(x, y, z, processZ);
                        
                        //line string segment -- just read the points
                        //and do LineTos
                        
                        int num_pts = IntegerFromStream(ireader, byteCount);

                        dreader = (double*)ireader;
                        PositionStreamCheckAndAdvance(num_pts, ords_per_position, byteCount);

                        for (int k = 0; k < num_pts; k++)
                        {
                            readPoint(&dreader, x, y, z, processZ, skip_vals);
                            bound.AddBound(x, y, z, processZ);
                        }

                        ireader = (int*) dreader;
                        break;
                    }
                }

            } //seg count
        } //ring count of polygons
    }// geom count of multi

    bool hasZ = (geom_dim & FdoDimensionality_Z);   
    minX = bound.minx;
    minY = bound.miny;
    minZ = hasZ ? bound.minz : 0;
    maxX = bound.maxx;
    maxY = bound.maxy;
    maxZ = hasZ ? bound.maxz : 0;
}


void FdoSpatialUtilityGeometryExtents::GetExtents(FdoByteArray* fgfArray, double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ)
{
   unsigned char* fgf = fgfArray->GetData();
    FdoInt32 byteCount = fgfArray->GetCount();

    int* ireader = (int*)fgf;

    // the geometry type
    IntegerStreamSizeCheck(ireader, byteCount);
    int geom_type = (FdoGeometryType)*ireader++;

    switch (geom_type)
    {
    case FdoGeometryType_MultiLineString :
    case FdoGeometryType_MultiPolygon :
    case FdoGeometryType_MultiPoint :
    case FdoGeometryType_LineString :
    case FdoGeometryType_Polygon :
    case FdoGeometryType_Point :
        getExtentsWithoutCurve(fgfArray, minX, minY, minZ, maxX, maxY, maxZ); 
        break;

    default:
        getExtentsWithCurve(fgfArray, minX, minY, minZ, maxX, maxY, maxZ);
        break;
    }
}

