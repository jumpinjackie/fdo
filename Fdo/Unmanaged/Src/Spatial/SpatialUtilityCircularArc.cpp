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

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include "SpatialUtilityCircularArc.h"
#include "MathUtility.h"

#include <math.h>

#define MAX_NUM_POSITIONS       (5000)  // Fail-safe maximum # positions in linearised arc.
#define MAX_NUM_LINESEGMENTS    (MAX_NUM_POSITIONS-1)


FdoSpatialUtilityCircularArc::FdoSpatialUtilityCircularArc(
        FdoIDirectPosition* start,
        FdoIDirectPosition* mid,
        FdoIDirectPosition* end)
:
    m_isValidArc(false),
    m_hasZ(false),
    m_isCircle(false)
{
    m_start = FdoDirectPositionImpl::Create(start);
    m_mid = FdoDirectPositionImpl::Create(mid);
    m_end = FdoDirectPositionImpl::Create(end);
    m_unitNormalVector = FdoDirectPositionImpl::Create(0.0, 0.0, 0.0);
    m_center = FdoDirectPositionImpl::Create();
    if (m_start == NULL || m_mid == NULL || m_end == NULL || m_unitNormalVector == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    m_center->SetDimensionality(m_start->GetDimensionality());

    m_radius = FdoMathUtility::GetQuietNan();
    m_length = FdoMathUtility::GetQuietNan();

    m_hasZ =
        start->GetDimensionality() &
        mid->GetDimensionality() &
        end->GetDimensionality() &
        FdoDimensionality_Z;

    // Two main cases are supported by the three-point definition:
    //  - three distinct, non-collinear points (start, end, midpt somewhere in the 
    //    curve) define an open arc
    //  - start&end coincide exactly, midpt is diametrically opposite, defining
    //    a circle (closed arc), presumed to be counterclockwise in direction
    //    when viewed from above (even if the circle isn't flat)
    //        - tilted circle: support midpt's Z ordinate not being the same as start/end,
    //          circle is tilted; the diameter that is perpendicular to the start-mid vector 
    //          is parallel to XY plane.
    // It is possible to define a vertical circle, but one must assume the direction of the
    // normal (which will be somewhere in the XY plane).  That is, one cannot define an 
    // arbitrary vertical circle using three points.

    if (FdoSpatialUtility::ArePositionsEqualXYZ(start, end))
    {
        // This might be a supported circle case.
        if ( ! FdoSpatialUtility::ArePositionsEqualXY(start, mid))
        {
            // It is.
            m_isCircle = true;

            // See if we should to lock onto "flat" circle to avoid floating point drift.
            if (FdoMathUtility::AreEqualAndMaybeNan(start->GetZ(), mid->GetZ()))
            {
                m_unitNormalVector->SetX(0.0);
                m_unitNormalVector->SetY(0.0);
                m_unitNormalVector->SetZ(1.0);   // Normal always has Z, even if geometry is XY only.
            }
            else
            {
                // Tilted circle case.
                // First, we need a vector that is perpendicular, in the XY plane, to
                // the start-mid vector.
                double dx = mid->GetX() - start->GetX();
                double dy = mid->GetY() - start->GetY();
                double diameterInXY = sqrt (dx*dx + dy*dy);
                double startMidAngle = atan2(dy, dx);
                double perpendicularInXYAngle = startMidAngle + ( FdoMathUtility::GetPi() / 2.0 );
                double perpendicularX = start->GetX() + ( diameterInXY * cos(perpendicularInXYAngle) );
                double perpendicularY = start->GetX() + ( diameterInXY * sin(perpendicularInXYAngle) );
                FdoPtr<FdoDirectPositionImpl> perpendicularInXYPos =
                    FdoDirectPositionImpl::Create(perpendicularX, perpendicularY, start->GetZ());
                
                // Now compute the unit normal to the circle.
                m_unitNormalVector = FdoSpatialUtility::UnitNormalVector(m_start, m_mid, perpendicularInXYPos);
            }
        } // Let the case of three identical points fall through.  It will be left marked as invalid.
    }
    else
    {
        // It is not a circle -- treat as a conventional arc.
        // This still could fail, in which case the unit normal is (0,0,0).
        FdoPtr<FdoDirectPositionImpl> normal = 
            FdoSpatialUtility::UnitNormalVector(m_start, m_mid, m_end);

        m_unitNormalVector = normal;
    }

    if (m_unitNormalVector->GetX() != 0.0 ||
        m_unitNormalVector->GetY() != 0.0 ||
        m_unitNormalVector->GetZ() != 0.0)
    {
        m_isValidArc = true;
        ComputeCenterRadiusAnglesFromThreePositionsAndNormal();
        ComputeLengthFromCenterRadiusAnglesAndNormal();
    }
    ComputeExtentsFromTwoPositionCenterRadiusAndNormal();
}

bool FdoSpatialUtilityCircularArc::IsFlat()
{
    // Note that the calculations for the normal vector latch to zero
    // as a special case, so these exact comparisons are valid.
    bool isFlat = ( m_unitNormalVector->GetX() == 0.0 && m_unitNormalVector->GetY() == 0.0 );
    return isFlat;
}

bool FdoSpatialUtilityCircularArc::IsDirectionCounterClockWise()
{
    bool isCcw = ( m_unitNormalVector->GetZ() > 0 );
    return isCcw;
}

void FdoSpatialUtilityCircularArc::ComputeLengthFromCenterRadiusAnglesAndNormal()
{
    double pi = FdoMathUtility::GetPi();
    double angleDiff = 0.0;

    if (m_isCircle)
    {
        angleDiff = 2.0 * pi;
    }
    else if (IsFlat())
    {
        // In the 2D case, simply use difference in "theta" around the Z axis.

        double startAngle = m_startAngle.m_theta;
        double endAngle = m_endAngle.m_theta;

        // Make sure that angles' magnitudes reflect direction.
        if ( IsDirectionCounterClockWise() && startAngle > endAngle )
            endAngle += 2.0 * pi;
        else if ( !IsDirectionCounterClockWise() && startAngle < endAngle )
            endAngle -= 2.0 * pi;

        angleDiff = fabs(endAngle - startAngle);
    }
    else
    {
        // In the 3D case, use spherical trigonometry.  To avoid ill-conditioning
        // of the arccosine  and arcsine functions, this will be the two-parameter
        // arctangent form of the Haversine formula (e.g. rather than the Law of Cosines).
        //
        // The ill-conditioning of certain angles stretch
        // the accuracy of the trig functions.  For example, the arcosine
        // function breaks down for values near 1.0...
        //       cos (5 degrees) = 0.996194698
        //       cos (1 degree) = 0.999847695
        //       cos (1 minute) = 0.9999999577
        //       cos (1 second) = 0.9999999999882
        //       cos (0.05 sec) = 0.999999999999971
        // Credits:
        //       http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1
        //       http://mathforum.org/library/drmath/view/51879.html
        //       http://mathforum.org/library/drmath/view/51711.html
        //       http://mathforum.org/library/drmath/view/54680.html

        double lon1 = m_startAngle.m_theta;
        double lon2 = m_endAngle.m_theta;
        double lat1 = m_startAngle.m_phi;
        double lat2 = m_endAngle.m_phi;

        // Haversine formula.
        double dLon = lon2 - lon1;
        double dLat = lat2 - lat1;
        double sinDLat2 = sin(dLat/2);
        double sinDLon2 = sin(dLon/2);
        double a = (sinDLat2*sinDLat2) + cos(lat1) * cos(lat2) * (sinDLon2*sinDLon2);
        angleDiff = 2.0 * atan2(sqrt(a), sqrt(1-a));

        // We now know the angle of minimum distance between the positions, but this might
        // be in the wrong direction.  Try rotating the start position about the normal
        // in both directions.  Whichever gets us closer to the end position is the
        // direction we want.  It should be an exact match, but we don't have a tolerance
        // available to guard against floating point drift.
        // We could use a fraction of the length and check proximity to the circle's
        // "mid" intermediate position to guard against ambiguity for semi-circles, but
        // we don't need that for a length calculation (the length is the same in either
        // direction).

        FdoPtr<FdoDirectPositionImpl> rotatedForward = FdoSpatialUtility::RotatePosition(
            m_start, angleDiff, m_unitNormalVector, m_center);
        FdoPtr<FdoDirectPositionImpl> rotatedBackward = FdoSpatialUtility::RotatePosition(
            m_start, -angleDiff, m_unitNormalVector, m_center);
        double forwardDistance = FdoSpatialUtility::DistanceBetweenPositionsXYZ(rotatedForward, m_end);
        double backwardDistance = FdoSpatialUtility::DistanceBetweenPositionsXYZ(rotatedBackward, m_end);
        if (forwardDistance > backwardDistance)
            angleDiff = (2.0*pi) - angleDiff;
    }

    m_length = m_radius * angleDiff;
}


void FdoSpatialUtilityCircularArc::ComputeCenterRadiusAnglesFromThreePositionsAndNormal()
{
    //Compute center.
    if (m_isCircle)
    {
        // This assumes that the three-point constructor was used.
        // The start and end positions coincide, while the mid position is
        // diametrically opposite.
        m_center->SetX(( m_mid->GetX() + m_start->GetX() ) * 0.5);
        m_center->SetY(( m_mid->GetY() + m_start->GetY() ) * 0.5);
        m_center->SetZ(m_hasZ ?
            ( ( m_mid->GetZ() + m_start->GetZ() ) * 0.5) :
            FdoMathUtility::GetQuietNan());
    }
    else // It is an unclosed arc.
    {
        ComputeCenterFromThreePositions();
    }

    // Compute radius.
    m_radius = FdoSpatialUtility::DistanceBetweenPositionsXYZ(m_start, m_center);

    // Compute start_angle and end_angle

    m_startAngle.m_theta =
        atan2( m_start->GetY() - m_center->GetY(),
        m_start->GetX() - m_center->GetX() );
    if ( m_startAngle.m_theta == - FdoMathUtility::GetPi() )
        m_startAngle.m_theta = FdoMathUtility::GetPi();

    m_startAngle.m_phi = m_hasZ ?
        ( asin( ( m_start->GetZ() - m_center->GetZ()) / m_radius ) ) :
        0.0;

    m_endAngle.m_theta = atan2(
                            m_end->GetY() - m_center->GetY(),
                            m_end->GetX() - m_center->GetX() );
    if ( m_endAngle.m_theta == - FdoMathUtility::GetPi() )
        m_endAngle.m_theta = FdoMathUtility::GetPi();

    m_endAngle.m_phi = m_hasZ ?
        asin( (m_end->GetZ() - m_center->GetZ()) / m_radius ) :
        0.0;
}


/************************************************************************
*         Find the center point of the circular arc determined by three *
*         given points                                                  *
*                                                                       *
*   Denote V, V1, V2, V3 the vectors of center, start, mid, end         *
*       respectively,                                                   *
*   then V = (x,y,z) satisfies the following equtions:                  *
*   ( V - ( V1 + V2 ) / 2 ) * ( V2 - V1 ) = 0                           *
*   ( V - ( V1 + V3 ) / 2 ) * ( V3 - V1 ) = 0                           *
*   ( V - V1 ) * ( ( V2 - V1 ) X ( V3 - V1 ) ) = 0                      *
*                                                                       *
*   i.e.,                                                               *
*                                                                       *
*   x21 * x + y21 * y + z21 * z = ( ||V2||^2 - ||V1||^2 ) / 2           *
*   x31 * x + y31 * y + z31 * z = ( ||V3||^2 - ||V1||^2 ) / 2           *
*   Dyz * x + Dzx * y + Dxy * z = Dxyz                                  *
*                                                                       *
*   where                                                               *
*   ( x21, y21, z21 ) = V2 - V1,                                        *
*   ( x31, y31, z31 ) = V3 - V1,                                        *
*   Dyz = y21 * z31 - y31 * z21,                                        *
*   Dzx = z21 * x31 - z31 * x21,                                        *
*   Dxy = x21 * y31 - x31 * y21,                                        *
*                                                                       *
*   Dxyz = V1 * ( V2 X V3 )                                             *
*                                                                       *
*         | x1  y1  z1  |                                               *
*       = | x21 y21 z21 | = x1 * Dyz + y1 * Dzx + z1 * Dxy              *
*         | x31 y31 z31 |                                               *
*                                                                       *
*   ||Vi||^2 = xi*xi + yi*yi + zi*zi                                    *
*                                                                       *
************************************************************************/
void FdoSpatialUtilityCircularArc::ComputeCenterFromThreePositions()
{
    double a[9], b[3], tmp;
    int eindex[3];
    int i;

    a[0] = m_mid->GetX() - m_start->GetX(); /* x21 */
    a[1] = m_mid->GetY() - m_start->GetY(); /* y21 */
    a[2] = m_hasZ ? ( m_mid->GetZ() - m_start->GetZ() ) : 0.0; /* z21 */

    a[3] = m_end->GetX() - m_start->GetX(); /* x31 */
    a[4] = m_end->GetY() - m_start->GetY(); /* y31 */
    a[5] = m_hasZ ? ( m_end->GetZ() - m_start->GetZ() ) : 0.0; /* z31 */

              /* y21 * z31 - y31 * z21 */
    a[6] = a[1] * a[5] - a[4] * a[2]; /* Dyz */
              /* z21 * x31 - z31 * x21 */
    a[7] = a[2] * a[3] - a[5] * a[0]; /* Dzx */
              /* x21 * y31 - x31 * y21 */
    a[8] = a[0] * a[4] - a[3] * a[1]; /* Dxy */

    tmp = m_start->GetX() * m_start->GetX() +
          m_start->GetY() * m_start->GetY() +
          ( m_hasZ ? ( m_start->GetZ() * m_start->GetZ() ) : 0.0 );

    b[0] = m_mid->GetX() * m_mid->GetX() +
           m_mid->GetY() * m_mid->GetY() +
           ( m_hasZ ? ( m_mid->GetZ() * m_mid->GetZ() ) : 0.0 );

    b[1] = m_end->GetX() * m_end->GetX() +
           m_end->GetY() * m_end->GetY() +
           ( m_hasZ ? ( m_end->GetZ() * m_end->GetZ() ) : 0.0) ;

    b[0] -= tmp;
    b[1] -= tmp;
    b[0] *= 0.5;
    b[1] *= 0.5;
    /* Dxyz = x1 * Dyz + y1 * Dzx + z1 * Dxy */
    b[2] = m_start->GetX() * a[6] +
           m_start->GetY() * a[7] +
           ( m_hasZ ? ( m_start->GetZ() * a[8] ) : 0.0 );

    /* Decompose a into LU */
    if ( FdoMathUtility::LUDecompose( 3, a, eindex ) == 0 )
    {
        // Zero result means that points were collinear.
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
    }

    /* Perform row exchanges for b */
    for ( i = 0; i < 3; ++i ) {
        if ( eindex[i] != i ) {
            tmp = b[i];
            b[i] = b[eindex[i]];
            b[eindex[i]] = tmp;
        }
    }

    /* Use L (UV) = b to solve UV */
    b[1] -= a[3] * b[0];
    b[2] -= a[6] * b[0] + a[7] * b[1];

    /* Use UV = b to solve V, the center */
    m_center->SetZ(m_hasZ ? ( b[2] / a[8] ) : 0.0);
    m_center->SetY(( b[1] - a[5] * (m_hasZ?m_center->GetZ():0.0) ) / a[4]);
    m_center->SetX(( b[0] - a[1] * m_center->GetY() - a[2] * (m_hasZ?m_center->GetZ():0.0) ) / a[0]);
}


// vectorToRight determines whether the vector (x2, y2) points to right or
// left side of vector (x1,y1) and returns false for right and true for left.
#define vectorToLeft( x1, y1, x2, y2 ) ( ((x1)*(y2) - (x2)*(y1)) < 0 ? false : true )

void FdoSpatialUtilityCircularArc::ComputeExtentsFromTwoPositionCenterRadiusAndNormal()
{
    // Currently only have 2D logic.  Leave m_envelope unset (NULL) for 3D.
    if (IsFlat())
    {
        double cX = m_center->GetX();
        double cY = m_center->GetY();

        if (m_isCircle)
        {
            m_envelope = FdoEnvelopeImpl::Create(
                cX - m_radius,
                cY - m_radius,
                cX + m_radius,
                cY + m_radius);
        }
        else
        {
            double sX = m_start->GetX();
            double sY = m_start->GetY();
            double eX = m_end->GetX();
            double eY = m_end->GetY();
            double minX = eX < sX ? eX : sX;
            double minY = eY < sY ? eY : sY;
            double maxX = eX > sX ? eX : sX;
            double maxY = eY > sY ? eY : sY;
            bool isCC = IsDirectionCounterClockWise();
            double dX = eX - sX;
            double dY = eY - sY;
            double r = m_radius;

            if (vectorToLeft(cX-sX, cY+r-sY, dX, dY) == isCC)
                maxY = cY + r; // Use circle top.

            if (vectorToLeft(cX-sX, cY-r-sY, dX, dY) == isCC)
                minY = cY - r; // Use circle bottom.

            if (vectorToLeft(cX-r-sX, cY-sY, dX, dY) == isCC)
                minX = cX - r; // Use circle left.

            if (vectorToLeft(cX+r-sX, cY-sY, dX, dY) == isCC)
                maxX = cX + r; // Use circle right.

             m_envelope = FdoEnvelopeImpl::Create(minX, minY, maxX, maxY);
        }
    }
}

// The original logic is 2D, with an extension for 3D added here by calling
// FdoSpatialUtility::RotatePosition().
void FdoSpatialUtilityCircularArc::Linearize(
    FdoDirectPositionCollection * distinctPositions,
    double maxSpacing,
    double maxOffset)
{
    bool isFlat = IsFlat();

    if (maxSpacing < 0.0 || maxOffset < 0.0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    // Avoid mathematical exceptions later.
    maxSpacing = FdoMathUtility::SnapToZero(maxSpacing);
    maxOffset = FdoMathUtility::SnapToZero(maxOffset);

    double spacing = 0.0;
    bool maxSpacingIsUseful = (maxSpacing > 0.0 && maxSpacing <= m_length/2.0);
    bool maxOffsetIsUseful = (maxOffset > 0.0);
    
    if (m_isValidArc && (maxSpacingIsUseful || maxOffsetIsUseful))
    {
        if (maxOffsetIsUseful)
            spacing = BestArcStep(m_radius, m_length, maxOffset);
        if (spacing <= 0.0)
            maxOffsetIsUseful = false;

        if (maxSpacingIsUseful && (spacing > maxSpacing || !maxOffsetIsUseful))
            spacing = maxSpacing;

        if (maxOffsetIsUseful)
        {
            double resolution = maxOffset * 2.0;
            if (spacing < resolution)
                spacing = resolution;
        }
    }

    if (spacing <= 0.0)
    {
        // Interpolation is not possible.  Just append known positions.
        FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, m_start);
        FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, m_mid);
        FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, m_end);
    }
    else
    {
        FdoInt32 numLineSegments = (FdoInt32) ceil(m_length / spacing);

        if (numLineSegments > MAX_NUM_LINESEGMENTS)
            numLineSegments = MAX_NUM_LINESEGMENTS;

        // Slightly adjust for even spacing of positions along curve.
        spacing  = m_length / numLineSegments;

        double angleIncrement = spacing / m_radius;

        // These get carried along, even if they are NaN.
        double z = m_start->GetZ();
        double mStart = m_start->GetM();
        double mEnd = m_end->GetM();

        // Use start position exactly.
        FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, m_start);

        // Compute intermediate positions.
        for (FdoInt32 i = 1;  i < numLineSegments;  i++)
        {
            double angleOffset = i * angleIncrement;
            FdoPtr<FdoDirectPositionImpl> pos;
            double proportion = (double)i / (double)numLineSegments;
            double m = FdoMathUtility::LinearInterpolate(mStart, mEnd, proportion);

            if (isFlat)
            {
                // Use simplest possible calculations in 2D case.

                if ( ! IsDirectionCounterClockWise() )
                    angleOffset *= -1.0; // Adjust for arc orientation about Z axis.

                double angle = m_startAngle.m_theta + angleOffset;
                double x = m_center->GetX() + ( m_radius * cos(angle) );
                double y = m_center->GetY() + ( m_radius * sin(angle) );
                pos = FdoDirectPositionImpl::Create(x, y, z, m);
                if (pos == NULL)
                    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
            }
            else
            {
                // Use 3D rotation of start point about the normal vector to trace the arc.
                // This is better than applying spherical math and "great circle" geographic 
                // travelling logic with range and bearing, which fails at the "poles", where 
                // the bearing is undefined.
                // Great circle logic might be applicable if this arc linearization were to be
                // enhanced to take geographic ellipsoids into account in the future.
                pos = FdoSpatialUtility::RotatePosition(m_start, angleOffset, m_unitNormalVector, m_center);
                pos->SetM(m);
            }
            pos->SetDimensionality(m_start->GetDimensionality());
            FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, pos);
        }
    		
        // Use end position exactly.
        FdoSpatialUtility::AppendPositionToDistinctCollection(distinctPositions, m_end);
    }
}

/************************************************************************
*       BestArcStep - Compute best interval for interpolating arc       *
*                                                                       *
*       Consider a small arc from a circle centered at the origin.      *
*       Consider approximating the arc with a chord between the arc's   *
*       endpoints.  Position the arc so that its mid-point lies on      *
*       the positive x-axis and the chord is vertical.                  *
*       The maximum error will be between the arc's and chord's midpoints,*
*       which both lie on the x-axis.  For an arc of length C           *
*       depicting an angle of THETA from a circle of radius R, the      *
*       chord will be at:                                               *
*           X = R cos( THETA/2 )                                        *
*       and the difference, DX, from R is:                              *
*           DX = R - X                                                  *
*              = R ( 1 - cos( THETA/2 ) )                               *
*              = R ( 1 - cos( C / 2R ) )                                *
*       DX is given as max_err.  Solving for C, we get:                 *
*                                                                       *
*           C = 2R arccos( 1 - DX/R )                                   *
*                                                                       *
*       NOTE: this formula should be reviewed for ill-conditioning. The *
*       use of arccosine may be inappropriate for small angles, and an  *
*       alternative may be needed, similar to the use of Haversine      *
*       formula instead of Law of Cosines in                            *
*       ComputeLengthFromRadiusAngles().                                *
*                                                                       *
************************************************************************/
double FdoSpatialUtilityCircularArc::BestArcStep(
    double radius,
    double arcLength,
    double maximumError)
{
    // Avoid mathematical exceptions.
    if (FdoMathUtility::SnapToZero(radius) <= 0.0 ||
        FdoMathUtility::SnapToZero(maximumError) <= 0.0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
    }

    double           arcStep = 0.0;
    double           resolution = maximumError * 2.0;

    double  portion = 1.0 - ( maximumError / radius );

    if ( portion < 1.0 && portion > 0.0 )
    {
        arcStep = 2.0 * radius * acos( portion );

        // Avoid step length that is too short for the resolution.
        if ( arcStep < resolution )
            arcStep = resolution;

        // Arc step is too long for interpolation.
        if ( arcStep > arcLength / 2.0 )
            arcStep = 0.0;
    }
    else
    {
        // Arc step is too long for interpolation.
        ;   // Leave as default 0.0.
    }

    return arcStep;
}

