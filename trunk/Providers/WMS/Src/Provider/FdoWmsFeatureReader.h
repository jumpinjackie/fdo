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
 */

#ifndef FDOWMSFEATUREREADER_H
#define FDOWMSFEATUREREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsReader.h"
#include "FdoWmsRect.h"

class FdoWmsFeatureReader : public virtual FdoWmsReader<FdoIFeatureReader>
{
    typedef FdoWmsReader<FdoIFeatureReader> superclass;

protected:
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsFeatureReader(void);

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose();

public:
    // Default constructor
    FdoWmsFeatureReader ();

	FdoWmsFeatureReader (FdoIoStream* stream, const FdoWmsRect& bounds, FdoClassDefinition* featClass, FdoClassDefinition* classDef);

    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsFeatureReader & operator= (const FdoWmsFeatureReader &right);

    /// <summary>Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.</summary>
    /// <returns>Returns the class definition object.</returns> 
    FdoClassDefinition* GetClassDefinition ();

    /// <summary>Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.</summary>
    /// <returns>Returns the depth</returns> 
    FdoInt32 GetDepth ();

    /// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the nested feature reader</returns> 
    FdoIFeatureReader* GetFeatureObject (FdoString* propertyName);

	/// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    virtual FdoIRaster* GetRaster(FdoString* identifier);

	/// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the string value</returns> 
	virtual FdoString* GetString (FdoString* identifier);

	virtual bool ReadNext ();

private:
	// Underlying FdoIoStream to read the HTTP stream
	FdoIoStreamP mStream;
	
	// The index of the current feature be read
	FdoInt32 mFeatureIdx;

	// Clipping bounds of the raster
	FdoWmsRect mBounds;

	// The feature class
	FdoPtr<FdoClassDefinition> mFeatClass;

	// Cache the selected properties
	FdoPtr<FdoClassDefinition> mClassDefPruned;
};

#endif // FDOWMSFEATUREREADER_H
