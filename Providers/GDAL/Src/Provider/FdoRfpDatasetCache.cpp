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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnection.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpDatasetCache.h"
#include <gdal.h>
#include <cpl_conv.h>

const static int nOpenDatasetMaximum = 32;


FdoRfpDatasetCache::FdoRfpDatasetCache(void)
{
    nDatasetCount = 0;
    nDatasetAlloc = 0;
    pahDatasetList = NULL;
}

FdoRfpDatasetCache::~FdoRfpDatasetCache(void)
{
    fprintf( stderr, "Cleanup Cache\n" );
    CloseAll();
    CPLFree( pahDatasetList );
}

FdoRfpDatasetCache *FdoRfpDatasetCache::Create()
{
    return new FdoRfpDatasetCache();
}

GDALDatasetH FdoRfpDatasetCache::LockDataset( FdoStringP filePath, bool failQuietly )

{
    int iDS;
    GDALDatasetH hDS;

    // Do we already have the dataset open?  If so, add a reference, promote
    // it in the LRU ordering, and return it.
    for( iDS = 0; iDS < nDatasetCount; iDS++ )
    {
        if( strcmp( filePath, GDALGetDescription( pahDatasetList[iDS] ) ) == 0 )
        {
            hDS = pahDatasetList[iDS];

            GDALReferenceDataset( hDS );

            if( iDS != 0 )  // Bump dataset to front of list for LRU ordering.
            {
                memmove( pahDatasetList + 1, pahDatasetList, sizeof(hDS) * iDS );
                pahDatasetList[0] = hDS;
            }
            
            return hDS;
        }
    }

    // Otherwise, try to open it. 
    if( failQuietly )
        CPLPushErrorHandler( CPLQuietErrorHandler );

    hDS = GDALOpen( filePath, GA_ReadOnly );
    
    if( failQuietly )
        CPLPopErrorHandler();

    if( hDS == NULL )
        return NULL;

    // If we are over our "open dataset" limit, then find the 
    // least recently used one that has only a single reference (our own)
    // and remove it. 
    if( nDatasetCount >= nOpenDatasetMaximum )
    {
        for( iDS = nDatasetCount-1; iDS >= 0; iDS-- )
        {
            GDALReferenceDataset( pahDatasetList[iDS] );
            if( GDALDereferenceDataset( pahDatasetList[iDS] ) == 1 )
            {
                CloseDataset( iDS );
                break;
            }
        }
    }

    // Grow the list if we need room.
    if( nDatasetCount == nDatasetAlloc )
    {
        nDatasetAlloc = nDatasetAlloc * 2 + 10;
        pahDatasetList = (GDALDatasetH *) 
            CPLRealloc(pahDatasetList, nDatasetAlloc * sizeof(GDALDatasetH) );
    }

    // Add our new handle at the beginning of the list. 
    pahDatasetList[nDatasetCount++] = hDS;

    return hDS;
}

void FdoRfpDatasetCache::UnlockDataset( GDALDatasetH hDS )

{
    if( hDS == NULL )
        return;

    if( GDALDereferenceDataset( hDS ) > 1 
        || nDatasetCount <= nOpenDatasetMaximum )
        return;

    // Find dataset.
    int iDS;

    for( iDS = 0; iDS < nDatasetCount; iDS++ )
    {
        if( hDS == pahDatasetList[iDS] )
            CloseDataset( iDS );
    }
}

void FdoRfpDatasetCache::CloseDataset( int iDS )

{
    GDALClose( pahDatasetList[iDS] );
    if( iDS != nDatasetCount-1 )
        memmove( pahDatasetList + iDS, pahDatasetList + iDS + 1, 
                 (nDatasetCount - iDS - 1) * sizeof(GDALDatasetH) );
    nDatasetCount--;
}

void FdoRfpDatasetCache::CloseUnlocked()

{
    int iDS;

    for( iDS = nDatasetCount-1; iDS >= 0; iDS-- )
    {
        GDALReferenceDataset( pahDatasetList[iDS] );
        if( GDALDereferenceDataset( pahDatasetList[iDS] ) == 1 )
            CloseDataset( iDS );
    }
}

void FdoRfpDatasetCache::CloseAll()

{
    int iDS;

    for( iDS = nDatasetCount-1; iDS >= 0; iDS-- )
        CloseDataset( iDS );
}
