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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpRasterCapabilities.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpRasterCapabilities.h"

FdoRfpRasterCapabilities::FdoRfpRasterCapabilities()
{
}

FdoRfpRasterCapabilities::~FdoRfpRasterCapabilities()
{
}

bool FdoRfpRasterCapabilities::SupportsRaster()
{
    return true;
}

bool FdoRfpRasterCapabilities::SupportsStitching()
{
    return false; // for now.
}

bool FdoRfpRasterCapabilities::SupportsSubsampling()
{
    return true;
}
  
bool FdoRfpRasterCapabilities::SupportsDataModel(FdoRasterDataModel* model)
{
    if (model == NULL)
        return false;

    // GDAL supports all raster organizations.

    // GDAL Supports selected combinations of modeltype and bits per pixel.
    FdoInt32 bpp = model->GetBitsPerPixel();
    int bands;

    switch (model->GetDataModelType())
    {
      case FdoRasterDataModelType_Bitonal:
        return false;  

        
      case FdoRasterDataModelType_Palette:
        return false; // for now this mode is not supported.

      case FdoRasterDataModelType_Gray:
      case FdoRasterDataModelType_Data:
        bands = 1;
        break;

      case FdoRasterDataModelType_RGB:
        bands = 3;
        break;

      case FdoRasterDataModelType_RGBA:
        bands = 4;
        break;
    };
	
    if( model->GetDataType() == FdoRasterDataType_Float )
        return model->GetBitsPerPixel() == 32 * bands 
            || model->GetBitsPerPixel() == 64 * bands;
    else if( model->GetDataType() == FdoRasterDataType_Integer )
        return model->GetBitsPerPixel() == 16 * bands 
            || model->GetBitsPerPixel() == 32 * bands;
    else if( model->GetDataType() == FdoRasterDataType_UnsignedInteger )
        return model->GetBitsPerPixel() == 8 * bands
            || model->GetBitsPerPixel() == 16 * bands 
            || model->GetBitsPerPixel() == 32 * bands;
    else
        return false;

    return false;
}

void FdoRfpRasterCapabilities::Dispose()
{
	delete this;
}
