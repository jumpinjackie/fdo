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
	return true;
}

bool FdoRfpRasterCapabilities::SupportsSubsampling()
{
	return true;
}
  
bool FdoRfpRasterCapabilities::SupportsDataModel(FdoRasterDataModel* model)
{
	if (model == NULL)
		return false;

	// We are using ATIL as our underlying image library, so we just support
	// those data models supported by ATIL
	
	// ATIL can only support data interleave organization by pixel
	if (model->GetOrganization() != FdoRasterDataOrganization_Pixel)
		return false;
	FdoInt32 bpp = model->GetBitsPerPixel();

	switch (model->GetDataModelType())
	{
	case FdoRasterDataModelType_Bitonal:
		if (bpp == 1)
			return true;
		break;
	case FdoRasterDataModelType_Gray:
		if (bpp == 8)
			return true;
		break;
	case FdoRasterDataModelType_RGB:
		if (bpp == 24)
			return true;
		break;
	case FdoRasterDataModelType_RGBA:
		if (bpp == 32)
			return true;
		break;
	case FdoRasterDataModelType_Palette:
		if (bpp == 8)
			return true;
		break;
	case FdoRasterDataModelType_Data:
		if (bpp == 32)
			return true;
		break;
	};
	
	return false;
}

void FdoRfpRasterCapabilities::Dispose()
{
	delete this;
}
