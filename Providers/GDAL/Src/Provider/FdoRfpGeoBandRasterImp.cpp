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
#include "FdoRfpConnection.h"
#include "FdoRfpDatasetCache.h"
#include "FdoRfpGeoreference.h"
#include "FdoRfpRasterUtil.h"
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
        m_haveGeotransform(true),
        m_insertionX(insertionX), m_insertionY(insertionY),
        m_resX(resX), m_resY(resY), m_width(width), m_height(height), 
        m_rotationX(rotationX), m_rotationY(rotationY),
        m_haveBounds(false)
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
        m_haveGeotransform(true),
        m_insertionX(insertionX), m_insertionY(insertionY),
        m_resX(resX), m_resY(resY), m_width(width), m_height(height), 
        m_rotationX(rotationX), m_rotationY(rotationY),
        m_haveBounds(false)
{
}

// geo band raster implemenation with rotation support
FdoRfpGeoBandRasterRot::FdoRfpGeoBandRasterRot(
    FdoRfpConnection *conn,
    FdoString* imagePath, int frameNumber ) : 
        FdoRfpGeoBandRaster(conn, imagePath, frameNumber), 
        m_haveGeotransform(false),
        m_width(-1), m_height(-1),
        m_haveBounds(false)
{
}


FdoRfpGeoBandRasterRot::~FdoRfpGeoBandRasterRot()
{
}
                            
void FdoRfpGeoBandRasterRot::SetBounds( double minX, double minY, double maxX, double maxY )
{
    m_haveBounds = true;
    m_minX = minX;
    m_minY = minY;
    m_maxX = maxX;
    m_maxY = maxY;
}

void FdoRfpGeoBandRasterRot::SetGeotransform( double insertionX, double insertionY, 
                                              double resX, double resY,
                                              double rotationX, double rotationY )
{
    m_haveGeotransform = true;
    m_insertionX = insertionX;
    m_insertionY = insertionY;
    m_resX = resX;
    m_resY = resY;
    m_rotationX = rotationX;
    m_rotationY = rotationY;
}

bool FdoRfpGeoBandRasterRot::IsRotated()
{
    if( !m_haveGeotransform )
        loadImageInfo();
    return m_rotationX != 0.0 && m_rotationY != 0.0;
}

double  FdoRfpGeoBandRasterRot::GetResolutionX()
{
    if( !m_haveGeotransform )
        loadImageInfo();
    return m_resX;
}

double  FdoRfpGeoBandRasterRot::GetResolutionY()
{
    if( !m_haveGeotransform )
        loadImageInfo();
    return m_resY;
}

FdoIGeometry* FdoRfpGeoBandRasterRot::GetGeometry()
{
    double pts[10];

    loadImageInfo();

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
    if( m_haveBounds )
    {
        return FdoRfpRect(m_minX,m_minY,m_maxX,m_maxY);
    }
    else
    {
        FdoPtr<FdoIGeometry> geo = GetGeometry();
        FdoPtr<FdoIEnvelope> bounds = geo->GetEnvelope();
        return FdoRfpRect(bounds->GetMinX(), bounds->GetMinY(), bounds->GetMaxX(), bounds->GetMaxY());
    }
}

void FdoRfpGeoBandRasterRot::PixelToWorld(double x, double y, double *x_out, double *y_out)
{
    //y = m_height - y;

    loadImageInfo();

    *x_out = m_insertionX + x * m_resX * cos(m_rotationY) - y * (m_resY) * sin(m_rotationX);
    *y_out = m_insertionY + x * m_resX * sin(m_rotationY) + y * (m_resY) * cos(m_rotationX);
}

bool FdoRfpGeoBandRasterRot::loadImageInfo()

{
    if( m_haveGeotransform && m_width != -1 && m_height != -1 )
        return true;

    GDALDatasetH hDS;
    FdoPtr<FdoRfpDatasetCache>  datasetCache = m_connection->GetDatasetCache();

    hDS = datasetCache->LockDataset( m_imagePath, false );

    if( hDS == NULL )
        throw FdoException::Create(NlsMsgGet(GRFP_95_CANNOT_GET_IMAGE_INFO, 
                                             "Fail to get image information."));

    m_width = GDALGetRasterXSize( hDS );
    m_height = GDALGetRasterYSize( hDS );
            
    FdoRfpGeoreferenceP geoRef = new FdoRfpGeoreference();
    bool bHasGeoInfo = FdoRfpRasterUtil::GetGeoReferenceInfo(hDS, geoRef);
    if( bHasGeoInfo && !m_haveGeotransform )
    {
        m_insertionX = geoRef->GetXInsertion();
        m_insertionY = geoRef->GetYInsertion();
        m_resX = geoRef->GetXResolution();
        m_resY = geoRef->GetYResolution();
        m_rotationX = geoRef->GetXRotation();
        m_rotationY = geoRef->GetYRotation();
        m_haveGeotransform = true;
    }

    datasetCache->UnlockDataset( hDS );
    hDS = NULL;
}

