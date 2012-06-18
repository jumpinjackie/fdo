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
#include "FdoRfpDatasetCache.h"
#include "FdoRfpImage.h"
#include <gdal.h>
#include "FdoRfpKeyColor.h"
#include "FdoRfpRasterUtil.h"
#include "FdoRfpConnection.h"

FdoRfpGeoBandRaster::FdoRfpGeoBandRaster(FdoRfpConnection *conn, FdoString* imagePath) 
    : m_connection(conn), m_imagePath(imagePath), m_frameNumber(0)
{
    // !!!NOTE!!! conn is an Internal Soft-Reference -- Do not increment Ref-Count
}

FdoRfpGeoBandRaster::FdoRfpGeoBandRaster(FdoRfpConnection *conn, FdoString* imagePath, FdoInt32 frameNumber) 
    : m_connection(conn), m_imagePath(imagePath), m_frameNumber(frameNumber)
{
    // !!!NOTE!!! conn is an Internal Soft-Reference -- Do not increment Ref-Count
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
        FdoPtr<FdoRfpDatasetCache> cache = m_connection->GetDatasetCache();
        m_image = FdoRfpImage::Create( cache, m_imagePath );
    }

    return FDO_SAFE_ADDREF(m_image.p);
}

bool FdoRfpGeoBandRaster::GetKeyColor(FdoPtr<FdoRfpKeyColorCollection>& keyColorColl)
{
    return FdoRfpRasterUtil::GetKeyColor(m_imagePath, keyColorColl);
}

FdoRfpGeoBandRasterCollection* FdoRfpGeoBandRasterCollection::Create()
{
    return new FdoRfpGeoBandRasterCollection();
}

