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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoBandRasterImp.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpGeoBandRaster.h"
#include "FdoRfpGeoBandRasterImp.h"
#include "FdoRfpImage.h"
#include <gdal.h>

// geo band raster implemenation with rotation support
FdoRfpGeoBandRasterRot::FdoRfpGeoBandRasterRot(
    FdoRfpConnection *conn,
    FdoString* imagePath, 
    double insertionX, double insertionY,
    double resX, double resY,
    int width, int height,
    double rotationX, double rotationY) : 
        FdoRfpGeoBandRaster(conn,imagePath), 
        m_insertionX(insertionX), m_insertionY(insertionY),
        m_resX(resX), m_resY(resY), m_width(width), m_height(height), 
        m_rotationX(rotationX), m_rotationY(rotationY)
{
}

// geo band raster implemenation with rotation support
FdoRfpGeoBandRasterRot::FdoRfpGeoBandRasterRot(
    FdoRfpConnection *conn,
    FdoString* imagePath, int frameNumber, 
    double insertionX, double insertionY,
    double resX, double resY,
    int width, int height,
    double rotationX, double rotationY) : 
        FdoRfpGeoBandRaster(conn, imagePath, frameNumber), 
        m_insertionX(insertionX), m_insertionY(insertionY),
        m_resX(resX), m_resY(resY), m_width(width), m_height(height), 
        m_rotationX(rotationX), m_rotationY(rotationY)
{
}


FdoRfpGeoBandRasterRot::~FdoRfpGeoBandRasterRot()
{
}
                            
bool FdoRfpGeoBandRasterRot::IsRotated()
{
    return m_rotationX != 0.0 && m_rotationY != 0.0;
}

double  FdoRfpGeoBandRasterRot::GetResolutionX()
{
    return m_resX;
}

double  FdoRfpGeoBandRasterRot::GetResolutionY()
{
    return m_resY;
}

FdoIGeometry* FdoRfpGeoBandRasterRot::GetGeometry()
{
    double pts[10];

    PixelToWorld(0.0, 0.0, pts+0, pts+1 );
    PixelToWorld(m_width, 0.0, pts+2, pts+3);
    PixelToWorld(m_width, m_height, pts+4, pts+5);
    PixelToWorld(0.0, m_height, pts+6, pts+7);
    PixelToWorld(0.0, 0.0, pts+8, pts+9 );

    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> exterior = factory->CreateLinearRing(FdoDimensionality_XY, 10, pts );
    FdoPtr<FdoLinearRingCollection> interiors = FdoLinearRingCollection::Create();
    FdoPtr<FdoIGeometry> geometry = factory->CreatePolygon(exterior, interiors);

    return FDO_SAFE_ADDREF(geometry.p);
}

FdoRfpRect FdoRfpGeoBandRasterRot::GetBounds()
{
    FdoPtr<FdoIGeometry> geo = GetGeometry();
    FdoPtr<FdoIEnvelope> bounds = geo->GetEnvelope();
    return FdoRfpRect(bounds->GetMinX(), bounds->GetMinY(), bounds->GetMaxX(), bounds->GetMaxY());
}

void FdoRfpGeoBandRasterRot::PixelToWorld(double x, double y, double *x_out, double *y_out)
{
    //y = m_height - y;

    *x_out = m_insertionX + x * m_resX * cos(m_rotationY) - y * (m_resY) * sin(m_rotationX);
    *y_out = m_insertionY + x * m_resX * sin(m_rotationY) + y * (m_resY) * cos(m_rotationX);
}

