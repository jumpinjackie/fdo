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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoRaster.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:07:36 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 25 $
 */

#ifndef FDORFPGEORASTER_H
#define FDORFPGEORASTER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpGeoBandRaster.h"
#include <FdoRfpDefines.h>


class FdoRfpGeoRaster : public FdoDisposable
{
// 
// data members
//
protected:
    FdoRasterType                m_rasterType;
    FdoRfpGeoBandRastersP	 m_images;
    // identifier for this raster
    FdoStringP                   m_id;

public:
    static FdoRfpGeoRaster* Create();

//
// ctor and dtor
//
protected:
    FdoRfpGeoRaster();
    virtual ~FdoRfpGeoRaster();

//
// exposed functions
//
public:
    FdoRasterType   GetRasterType();
    void            SetRasterType(FdoRasterType rasterType);

    FdoInt32        GetNumberOfBands();
    void	    AddBand(FdoRfpGeoBandRasterP bandRaster);
    FdoRfpGeoBandRaster* GetBand(FdoInt32 bandNumber);

    FdoString*      GetId();
    void	    SetId(FdoStringP value);
};

typedef FdoPtr<FdoRfpGeoRaster> FdoRfpGeoRasterP;


class FdoRfpGeoRasterCollection : public FdoCollection<FdoRfpGeoRaster, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
    FdoRfpGeoRasterCollection() {}
    virtual ~FdoRfpGeoRasterCollection() {}
    void Dispose() { delete this; }
public:
    static FdoRfpGeoRasterCollection* Create();
};

typedef FdoPtr<FdoRfpGeoRasterCollection> FdoRfpGeoRastersP;


class FdoRfpGeoRasterCollectionCollection : public FdoCollection<FdoRfpGeoRasterCollection, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
    FdoRfpGeoRasterCollectionCollection() {}
    virtual ~FdoRfpGeoRasterCollectionCollection() {}
    void Dispose() { delete this; }
public:
    static FdoRfpGeoRasterCollectionCollection* Create();
};

typedef FdoPtr<FdoRfpGeoRasterCollectionCollection> FdoRfpGeoRasterCollectionsP;

#endif
