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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpImage.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpImage.h"
#include "FdoRfpDatasetCache.h"

FdoRfpImage::FdoRfpImage()

{
}

void FdoRfpImage::Initialize( FdoRfpDatasetCache* datasetCache, FdoStringP datasetName )

{
    m_datasetCache = FDO_SAFE_ADDREF( datasetCache );

    m_datasetName = datasetName;

    m_ds = NULL;
    m_redBand = NULL;

    // force opening of dataset.  If this doesn't work ... yikes!
    if( GetDS() == NULL )
        return;
    
    m_redBand = GDALGetRasterBand( m_ds, 1 );
    
    // TODO: For now just use first band.
    if( GDALGetRasterCount( m_ds ) == 4 
        && GDALGetRasterColorInterpretation( 
            GDALGetRasterBand( m_ds, 4 ) ) == GCI_AlphaBand )
    {
        m_bandList[0] = 1;
        m_bandList[1] = 2;
        m_bandList[2] = 3;
        m_bandList[3] = 4;
        m_components = 4;
    }
    else if( GDALGetRasterCount( m_ds ) == 3 )
    {
        m_bandList[0] = 1;
        m_bandList[1] = 2;
        m_bandList[2] = 3;
        m_components = 3;
    }
    else
    {
        m_bandList[0] = 1;
        m_components = 1;
    }
        
    m_gdalDataType = GDALGetRasterDataType( m_redBand );
    m_bytesPerPixel = GDALGetDataTypeSize( m_gdalDataType ) * m_components / 8;

    GDALGetBlockSize( m_redBand, &m_blockXSize, &m_blockYSize );

    m_xSize = GDALGetRasterXSize( m_ds );
    m_ySize = GDALGetRasterYSize( m_ds );

    ReleaseDS();
}

FdoRfpImage::~FdoRfpImage()
{
    ReleaseDS();
}

GDALDatasetH FdoRfpImage::GetDS()

{
    if( m_ds == NULL )
        m_ds = m_datasetCache->LockDataset( m_datasetName, false );

    return m_ds;
}

void FdoRfpImage::ReleaseDS()

{
    if( m_ds != NULL )
    {
        m_datasetCache->UnlockDataset( m_ds );
        m_ds = NULL;
    }
}

FdoRfpImageCollection::FdoRfpImageCollection()
{
}

FdoRfpImageCollection::~FdoRfpImageCollection()
{
}

void FdoRfpImageCollection::Dispose()
{
	delete this;
}

FdoRfpImageCollection* FdoRfpImageCollection::Create()
{
	return new FdoRfpImageCollection();
}


