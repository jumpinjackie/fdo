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

#include "MathUtility.h"

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <Spatial/SpatialGridTransform.h>

// 55 bits of precision in the signed mantissa of a "double"
#define MAX_DELTA ((double) ((((FdoInt64)1L)<<55)-1));

static double even (double size, double resolution);


FdoSpatialGridTransform *
FdoSpatialGridTransform::Create(
    FdoIEnvelope *extents,
    double tolerance)
{
    return new FdoSpatialGridTransform(extents, tolerance);
}


void FdoSpatialGridTransform::TransformToGrid(
    double x,
    double y,
    FdoInt64 * xGrid,
    FdoInt64 * yGrid)
{
    *xGrid = FdoMathUtility::ConvertDoubleToInt64((x - m_offsetX) * m_scaleInverse);
    *yGrid = FdoMathUtility::ConvertDoubleToInt64((y - m_offsetY) * m_scaleInverse);
}
void
FdoSpatialGridTransform::TransformFromGrid(
    FdoInt64 xGrid,
    FdoInt64 yGrid,
    double *x,
    double *y)
{
        *x = xGrid * m_scale + m_offsetX;
        *y = yGrid * m_scale + m_offsetY;
}


FdoSpatialGridTransform::FdoSpatialGridTransform()
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


FdoSpatialGridTransform::FdoSpatialGridTransform(
    FdoIEnvelope *extents,
    double tolerance)
{
        m_minX = extents->GetMinX();
        m_minY = extents->GetMinY();
        m_maxX = extents->GetMaxX();
        m_maxY = extents->GetMaxY();
        m_resolution = tolerance * 2.0;

        if ((m_maxX < m_minX) || (m_maxY < m_minY))
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

        // compute transformation parameters
        CalculateParameters();
}


FdoSpatialGridTransform::~FdoSpatialGridTransform()
{
}


// Adjust size so that it's an even integral multiple of the resolution.
static double even (double size, double resolution)
{
    FdoInt64 num_units; // number of resolution units
    double dbl_units;

    // determine the number of resolutions in the range
    size = fabs(size);
    dbl_units = size / resolution;
    num_units = (FdoInt64)(dbl_units + 0.5);

    // if odd number of resolution units, add one
    if (0 != (num_units & 1))
        size = (num_units + 1) * resolution;

    return (size);
}


// Initialize transformation parameters
void
FdoSpatialGridTransform::CalculateParameters()
{
    double delta_x;
    double delta_y;
    double min_delta;
    double max_delta;
    double best_res;

    double half_delta_x, half_delta_y;
    bool ret = true;

    delta_x = m_maxX - m_minX;
    delta_y = m_maxY - m_minY;
    max_delta = (delta_x > delta_y) ? delta_x : delta_y;
    min_delta = (delta_x < delta_y) ? delta_x : delta_y;
    best_res = max_delta / MAX_DELTA;

    // make sure resolution is not too small
    if (m_resolution <= 0.0)
        m_resolution = 0.001;

    // make sure resolution is reasonable
    if ((m_resolution / min_delta) > 0.10)
        // Resolution is too large for the extent.
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
    else if (m_resolution < best_res)
        // Resolution is too fine for the extent.
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    // calculate sizes in x and y
    half_delta_x = 0.5 * even (delta_x, m_resolution);
    half_delta_y = 0.5 * even (delta_y, m_resolution);

    // calculate centre point
    m_offsetX = m_minX + half_delta_x;
    m_offsetY = m_minY + half_delta_y;

    // the scale is just the resolution
    m_scale = m_resolution;
    m_scaleInverse = 1.0 / m_scale;
}

