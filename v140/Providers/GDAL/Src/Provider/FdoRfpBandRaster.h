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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpBandRaster.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPBANDRASTER_H
#define FDORFPBANDRASTER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

//
// Class forward declarations
//
class FdoRfpRect;
class FdoRfpRasterPropertyDictionary;
class FdoRfpImage;
class FdoRfpGeoBandRaster;

//
// Partial Implementation of FdoIRaster
//
class FdoRfpBandRaster : public FdoIDisposable
{
protected:
//
// enums
//
    enum ConversionOptions
    {
        CO_NOCONVERSION = 0x0,
        CO_SCALING = 0x1,
        CO_RETILING = 0x2,
        CO_DATAMODELCHANGING = 0x4,
    };

//
// Data members
//
private:
    // Geo band rasters that are the sources of return band raster
    // if there are more than one source geo band rasters, 
    // then we perform stitching before returning.
    FdoPtr<FdoRfpGeoBandRaster>         m_geoBandRaster;

    // the clipping bounds is used to return a subset of the original raster
    // if it is null, the whole raster will be returned
    FdoRfpRect*             		m_clippingBounds;

    // Whether the raster is null
    bool				m_bNull;

    // The bounding box of the raster
    FdoRfpRect*				m_bounds;

    // The data model of the raster
    FdoPtr<FdoRasterDataModel>		m_dataModel;

    // X size of the raster
    FdoInt32*				m_imageXSize;

    // Y size of the raster
    FdoInt32*				m_imageYSize;

    // how long in X and Y a pixel represents in the spatial coordinate system
    double*				m_resolutionX;
    double*				m_resolutionY;

//
// Constructor(s), destructor, factory function(s)
//
public:
    FdoRfpBandRaster(const FdoPtr<FdoRfpGeoBandRaster>& geoBandRaster, 
                     FdoRfpRect* clippingBounds);

protected:
    virtual ~FdoRfpBandRaster();
    void Dispose();
	
private:
    FdoRfpBandRaster(); // no implementation

//
// Exposed functions
//
public:
    bool IsNull ();
    void SetNull ();

    FdoDataValue* GetNullPixelValue ();

    FdoRfpRect* GetBounds ();
    void SetBounds (FdoRfpRect* bounds);

    FdoRasterDataModel* GetDataModel ();
    void SetDataModel (FdoRasterDataModel* datamodel);

    FdoInt32 GetImageXSize ();
    FdoInt32 GetImageYSize ();

    void SetImageXSize (FdoInt32 size);
    void SetImageYSize (FdoInt32 size);

    FdoIRasterPropertyDictionary* GetAuxiliaryProperties();

    void SetStreamReader (FdoIStreamReader* reader);
    FdoIStreamReader* GetStreamReader ();

    FdoRfpImage *GetImage();

//
// Internally used helper functions
//
private:
    void _computeOriginalBounds();

    FdoRfpRect _getRequestBounds();

    // accessor for m_resulotionX/Y
    double _getResolutionX();
    double _getResolutionY();

    void _getResolution();
    void _recomputeImageSize();
    void _computePixelWindow(const FdoPtr<FdoRfpImage>& image,
                             int &winXOff, int &winYOff, int &winXSize, int &winYSize );

    // Validate the raster is not null before any data retrieval operations
    void _validate();
    int _getConversionOptions();
};

/// <summary> FdoRfpBandRastersP is a FdoPtr on FdoRfpBandRasterCollection, provided for convenience. </summary>
typedef FdoPtr<FdoRfpBandRaster> FdoRfpBandRasterP;

/// <summary> 
/// The FdoRfpBandRasterCollection class contains a collection of band rasters.
/// </summary>
class FdoRfpBandRasterCollection : public FdoCollection<FdoRfpBandRaster, FdoException>
{
public:
    /// <summary> Constructs a new empty collection of band rasters. </summary>
    /// <returns> Returns the newly created FdoRfpBandRasterCollection instance. </returns> 
	static FdoRfpBandRasterCollection* Create();

//DOM-IGNORE-BEGIN

protected:
	FdoRfpBandRasterCollection() {}
	virtual ~FdoRfpBandRasterCollection(void) {}
	void Dispose();

//DOM-IGNORE-END
};

#endif
