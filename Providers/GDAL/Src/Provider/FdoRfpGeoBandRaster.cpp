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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoBandRaster.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpGeoBandRaster.h"
#include "FdoRfpImageFactory.h"
#include "FdoRfpImage.h"
#include <gdal.h>
#include "FdoRfpKeyColor.h"
#include "FdoRfpRasterUtil.h"

FdoRfpGeoBandRaster::FdoRfpGeoBandRaster(FdoString* imagePath) 
	: m_imagePath(imagePath), m_frameNumber(0)
{
}

FdoRfpGeoBandRaster::FdoRfpGeoBandRaster(FdoString* imagePath, FdoInt32 frameNumber) 
	: m_imagePath(imagePath), m_frameNumber(frameNumber)
{
}

FdoRfpGeoBandRaster::~FdoRfpGeoBandRaster()
{
}


FdoString* FdoRfpGeoBandRaster::GetId()
{
    return m_imagePath;
}

FdoRfpImage* FdoRfpGeoBandRaster::GetImage()
{
    if (m_image == NULL)
    {
        GDALDatasetH image;

        FdoRfpImageFactoryGdal::CreateImage(m_imagePath, image, m_frameNumber);

// TODO 
#ifdef notdef
        if (image->dataModel().dataModelType() == Atil::DataModelAttributes::kRgbModel &&
            image->dataModel().bitsUsedPerPixel() != 32)
        {
            Atil::RgbModel newModel(32);
            image->setDataModel(newModel);
        }
#endif
        m_image = FdoRfpImage::Create(image);
    }

    return FDO_SAFE_ADDREF(m_image.p);
}

#ifdef notdef
Atil::ImageView* FdoRfpGeoBandRaster::GetViewer(FdoRfpRect bounds, int viewWidth, int viewHeight)
{
    double resX = GetResolutionX();
    double resY = GetResolutionY();
    // figure out the scale first
    double scaleX = viewWidth * resX / bounds.GetWidth();
    double scaleY = viewHeight * resY / bounds.GetHeight();

    // the org point
    Atil::Point2d org = PixelToWorld(0, 0);

    FdoPtr<FdoRfpImage> image = GetAtilImage();
    // right top point
    Atil::Point2d rt = PixelToWorld(image->GetImage()->size().width, 0);
    // left bottom point
    Atil::Point2d lb = PixelToWorld(0, image->GetImage()->size().height);

    Atil::Vector2d xAxis((rt.x - org.x) * scaleX / resX, (org.y - rt.y) * scaleY / resY);
    Atil::Vector2d yAxis((lb.x - org.x) * scaleX / resX, (org.y - lb.y) * scaleY / resY);
    
    // make all ordinated relative to the lefttop of the bounds
    org.x = (org.x - bounds.m_minX) * scaleX;
    org.y = (bounds.m_maxY - org.y) * scaleY;

    rt.x = (rt.x - bounds.m_minX) * scaleX;
    rt.y = (bounds.m_maxY - rt.y) * scaleY;

    lb.x = (lb.x - bounds.m_minX) * scaleX;
    lb.y = (bounds.m_maxY - lb.y) * scaleY;

    Atil::ImageView* viewer;
    TRY_ATIL{
        viewer = new Atil::ImageView(*image->GetImage(), Atil::Size(viewWidth, viewHeight));
    } CATCH_ATIL;

    viewer->setByVectors(Atil::Point2d(org.x / resX, org.y / resY), xAxis, yAxis);

    return viewer;
}
#endif

bool FdoRfpGeoBandRaster::GetKeyColor(FdoPtr<FdoRfpKeyColorCollection>& keyColorColl)
{
	return FdoRfpRasterUtil::GetKeyColor(m_imagePath, keyColorColl);
}

FdoRfpGeoBandRasterCollection* FdoRfpGeoBandRasterCollection::Create()
{
    return new FdoRfpGeoBandRasterCollection();
}

