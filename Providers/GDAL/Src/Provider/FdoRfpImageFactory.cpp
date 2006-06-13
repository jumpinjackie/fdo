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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpImageFactory.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpImageFactory.h"
#include <gdal.h>


void FdoRfpImageFactoryGdal::CreateImage(FdoString* path, GDALDatasetH &image, int frameNumber)
{
    FdoStringP trPath( path );

    // TODO: Should we use frameNumber?
    image = GDALOpen( (const char *) trPath, GA_ReadOnly );

    if (image == NULL)
    {
        // TODO: We really ought to return the GDAL error.
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_63_FAIL_LOAD_RASTER, 
                                                     "Failed to load raster file %1$ls. Unsupported format encoding or error opening file ", path));
    }
}

void FdoRfpImageFactoryGdal::DestroyImage(GDALDatasetH image)
{
    GDALClose( image );
}

int FdoRfpImageFactoryGdal::GetImageSize(GDALDatasetH image)
{
    GDALRasterBandH hBand;

    hBand = GDALGetRasterBand( image, 1 );
    
    int nBytesPerPixel = 
        GDALGetDataTypeSize( GDALGetRasterDataType( hBand ) ) / 8;

    double dfRawImageSize;
    
    dfRawImageSize = GDALGetRasterXSize( image ) * (double) GDALGetRasterYSize( image ) * nBytesPerPixel;

    // TODO: Shouldn't this method return a 64 bit int?

    if( dfRawImageSize > /* 2GB */ 2000000000.0 )
        return 2000000000;
    else
        return (int) dfRawImageSize;
}
