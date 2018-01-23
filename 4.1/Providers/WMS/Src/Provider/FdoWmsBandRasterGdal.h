#ifndef FDOWMSBANDRASTERGDAL_H
#define FDOWMSBANDRASTERGDAL_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

class FdoRasterDataModel;
class FdoWmsRect;
class FdoWmsImage;
class GDALColorTable;
struct RgbColor;

class FdoWmsBandRasterGdal : public FdoIDisposable
{
//
// Data members
//
private:
	// Whether the raster is null
	bool							m_bNull;

	// The bounding box of the raster
	FdoPtr<FdoByteArray>			m_bounds;

	FdoPtr<FdoWmsImage>				m_image;

	// the palette, only available when the data model is palette
	GDALColorTable*					m_palette;

public:
	FdoWmsBandRasterGdal(FdoPtr<FdoWmsImage>& image, FdoWmsRect& bounds);

protected:
	virtual ~FdoWmsBandRasterGdal(void);
	void Dispose();

private:
	FdoWmsBandRasterGdal(void);

public:
    virtual bool IsNull ();

    virtual void SetNull ();

    virtual FdoByteArray* GetBounds ();

    virtual void SetBounds (FdoByteArray* bounds);

    virtual FdoRasterDataModel* GetDataModel ();

    virtual void SetDataModel (FdoRasterDataModel* datamodel);

    virtual FdoInt32 GetImageXSize ();

    virtual void SetImageXSize (FdoInt32 size);

    virtual FdoInt32 GetImageYSize ();

    virtual void SetImageYSize (FdoInt32 size);

    virtual FdoIRasterPropertyDictionary* GetAuxiliaryProperties ();

	virtual FdoDataValue* GetNullPixelValue ();

    virtual void SetStreamReader (FdoIStreamReader* reader);

	virtual FdoIStreamReader* GetStreamReader ();

// exposed functions
public:
	RgbColor*	GetPalette();
	FdoInt32    GetNumberOfPaletteEntries();

//
// Internally used helper functions
//
private:
	// Validate the raster is not null before any data retrieval operations
	void _validate();
};

typedef FdoPtr<FdoWmsBandRasterGdal> FdoWmsBandRasterGdalP;


/// <summary> 
/// The FdoWmsBandRasterGdalCollection class contains a collection of band rasters.
/// </summary>
class FdoWmsBandRasterGdalCollection : public FdoCollection<FdoWmsBandRasterGdal, FdoException>
{
public:
    /// <summary> Constructs a new empty collection of band rasters. </summary>
    /// <returns> Returns the newly created FdoWmsBandRasterGdalCollection instance. </returns> 
	static FdoWmsBandRasterGdalCollection* Create();

//DOM-IGNORE-BEGIN

protected:
	FdoWmsBandRasterGdalCollection() {}
	virtual ~FdoWmsBandRasterGdalCollection(void) {}
	void Dispose();

//DOM-IGNORE-END
};

/// <summary> FdoRfpBandRasterAtilsP is a FdoPtr on FdoWmsBandRasterGdalCollection, provided for convenience. </summary>
typedef FdoPtr<FdoWmsBandRasterGdalCollection> FdoWmsBandRasterGdalsP;

#endif
