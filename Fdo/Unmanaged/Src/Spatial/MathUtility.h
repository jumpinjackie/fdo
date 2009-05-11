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

#ifndef FdoMathUtility___H
#define FdoMathUtility___H	1

#include <limits>       // For quiet_NaN()
#include <float.h>      // For _isnan()
#ifdef _WIN32
// For M_PI in math.h on Windows (doesn't seem to work though).
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2  (M_PI / 2.0)
#endif
#define TWO_PI  (M_PI * 2.0)

#include <FdoCommon.h>


// Mathematical Utility.
class FdoMathUtility
{
public:

    // Get the value of PI.  The constant name isn't a C++ standard, although
    // this method will use whatever the local compiler offers.
    static double GetPi();

    // Gets the special value for representing a null ordinate value.
    static double GetNullOrdinateValue();

    // Gets a NaN (not-a-number) value that doesn't cause exceptions in
    // normal assignments and comparisons.
    // This is numeric_limits<double>::quiet_NaN(), just without the
    // sometimes bothersome header file requirements.
    static double GetQuietNan();

    // Returns true if the ordinate is considered to be null (Nan or special value).
    static bool IsOrdinateNull(const double ordinate);

    // Returns true if the value is not-a-number.
    static bool IsNan(double n);

    // Returns true if two numbers are equal (no tolerance), even if they are NaN.
    // Normally, IEEE rules have all NaN values failing an equality test.
    static bool AreEqualAndMaybeNan(double a, double b);

    // Snaps a number to 0.0 if it is of extremely small magnitude.
    // This is for avoiding mathematical problems such as underflow,
    // division by zero, major loss of precision, etc.
    // It is NOT for enforcing any tolerance floor in a coordinate system.
    // It does use an internal, tiny tolerance, whose value should not be assumed.
    static double SnapToZero(double n);

    // Convert a double to FdoInt64, with rounding.
    static FdoInt64 ConvertDoubleToInt64(double n);

    // Decompose input matrix a into matrices L and U
    // such that L[i][i] = 1, L[i][j] = 0 for i,j = 0, ..., n-1 and
    // j > i; U[i][j] = 0 for i,j = 0, ..., n-1 and i < j.
    // The resulting L and U are written back to matrix a such that
    // a[i*n + j] = L[i][j] if i < j; otherwsie a[i*n+j] = U[i][j],
    // for i, j = 1,2, ..., n.
    //
    // Parameters
    //    n: input
    //        Dimension of matrix a
    //
    //    a: updated
    //        Matrix (represented by an array of siez n*n) to be decomposed,
    //        where the element at the ith row and the jth column is
    //        a[ i*n + j ] for i,j = 0, ..., n;
    //
    //    eindex: output
    //        Row interchange indices, which should be interpreted orderedly
    //        from eindex[0] to eindex[n-1] such that if eindex[i] != i, it
    //        implies a row interchange between row i and row eindex[i].
    // Function value
    //       -1: Failure due to the singularity of the matrix;
    //        0: Even row interchanges;
    //        1: odd row interchanges;
    static int LUDecompose( int n, double a[], int eindex[] );

    // Interpolate a number in the range [start,end].  'proportion' must
    // be in the range [0.0,1.0].
    // 'start > end' is supported.
    // It is allowable to pass NaN for any parameter, in which case the return
    // value is also a (quiet) NaN.
    static double LinearInterpolate(double start, double end, double proportion);

    // Given a 3D arc defined by 3 points (start, mid, end), interpolate a z value for each point
    // with null z.
    // Returns 0.0 if none of the points had null z.
    // Returns the special value representing null if any points had null z.
    // Therefore, return value tells whether and z's were interpolated.
    static double Interpolate3dArcWithNullZ (
        double xStart, double yStart, double zStart, 
        double xMid, double yMid, double zMid, 
        double xEnd, double yEnd, double zEnd, 
        double& zStartPrime, double& zMidPrime, double& zEndPrime);

    // Given 3 z-values, interpolate a z value for each null z.
    // Returns 0.0 if none of the points had null z.
    // Returns the special value representing null if any points had null z.
    // Therefore, return value tells whether and z's were interpolated.
    static double Interpolate3OrdinatesWithNullZ (
        double z1, double z2, double z3,
        double& z1Prime, double& z2Prime, double& z3rime);


private:
    // Special constant for representing a null ordinate in FDO geometries
    static const double m_nullOrd;
};

#endif // FdoMathUtility___H


