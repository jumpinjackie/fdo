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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpImage.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPIMAGE_H
#define FDORFPIMAGE_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

//
// Class forward declaration
//
#include <gdal.h>

class FdoRfpDatasetCache;

class FdoRfpImage : public FdoIDisposable
{
//
// Data members
//
private:
    GDALDatasetH 		m_ds;

    GDALRasterBandH             m_redBand;
    
public:
    FdoPtr<FdoRfpDatasetCache>  m_datasetCache;
    FdoStringP                  m_datasetName;
         
    int                         m_bandList[4];
    int                         m_components;

    int                         m_blockXSize;
    int                         m_blockYSize;

    int                         m_xSize;
    int                         m_ySize;

    int                         m_bytesPerPixel; // total over all components
    GDALDataType                m_gdalDataType;

    GDALDatasetH                GetDS(); 
    void                        ReleaseDS();

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
    FdoRfpImage();

    void Initialize( FdoRfpDatasetCache* datasetCache, FdoStringP datasetName );

    void Dispose() { delete this; }
    virtual ~FdoRfpImage(void);

public:
    static FdoRfpImage* Create( FdoRfpDatasetCache* datasetCache, FdoStringP datasetName )
	{
            FdoRfpImage *image = new FdoRfpImage();
            image->Initialize( datasetCache, datasetName );
            return image;
	}
};

typedef FdoPtr<FdoRfpImage> FdoRfpImageP;

class FdoRfpImageCollection : public FdoCollection<FdoRfpImage, FdoCommandException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpImageCollection();
    virtual ~FdoRfpImageCollection(void);
	void Dispose();
public:
	static FdoRfpImageCollection* Create();

};

typedef FdoPtr<FdoRfpImageCollection> FdoRfpImagesP;


#endif
