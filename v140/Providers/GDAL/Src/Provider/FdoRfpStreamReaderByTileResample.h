/*
 * Copyright (C) 2012  Autodesk, Inc.
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
 *
 */
#ifndef FDORFPSTREAMREADERBYTILERESAMPLE_H
#define FDORFPSTREAMREADERBYTILERESAMPLE_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <gdal.h>
#include "FdoRfpStreamReaderByTile.h"

//
// class forward declarations
//
class FdoRfpBandRasterGdal;
class FdoRfpImage;
class GDALWarpOperation;

class FdoRfpStreamReaderGdalByTileResample : public FdoRfpStreamReaderGdalByTile
{
    friend class FdoRfpBandRasterGdal;
//
// Data members
//
private:
    FdoPtr<FdoRfpStreamReaderGdalByTile> m_resampledReader;
    FdoByte* m_tileBuffer;
    int m_resampleRatio;
    FdoStringP m_resamplingMethod;



//
// Constructor(s), destructor, factory function(s)
//
//protected:
public:
    FdoRfpStreamReaderGdalByTileResample(const FdoPtr<FdoRfpImage>& image,
                                 FdoRasterDataModel *model, FdoString* resamplingMethod,
                                 int winXOff, int winYOff, int winXSize, int winYSize,
                                 int viewXSize, int viewYSize );
                                 
    virtual ~FdoRfpStreamReaderGdalByTileResample();
    void Dispose() { delete this; }

//
// Internally used helper functions
//
protected:
    // get current tile
    void _getTile();
};


#endif
