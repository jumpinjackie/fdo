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

#ifndef FDOWMSGETMAP_H
#define FDOWMSGETMAP_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsGetMap : public FdoOwsRequest
{
protected:
	FdoWmsGetMap ();

	FdoWmsGetMap (FdoStringCollection* layerNames,
		FdoStringCollection* styleNames,
		FdoString* srsName, 
		FdoString* imgFormat,
		FdoSize height,
		FdoSize width,
		FdoDouble minx, 
		FdoDouble miny, 
		FdoDouble maxx, 
		FdoDouble maxy,
		FdoString* version,
		FdoBoolean bTransparent,
		FdoString* backgroundColor,
		FdoString* timeDimension,
		FdoString* elevation);

	virtual ~FdoWmsGetMap ();
	virtual void Dispose () { delete this; }

public:
	static FdoWmsGetMap* Create (FdoStringCollection* layerNames, 
		FdoStringCollection* styleNames,
		FdoString* srsName, 
		FdoString* imgFormat,
		FdoSize height,
		FdoSize width,
		FdoDouble minx, 
		FdoDouble miny, 
		FdoDouble maxx, 
		FdoDouble maxy,
		FdoString* version,
		FdoBoolean bTransparent = false,
		FdoString* backgroundColor = NULL,
		FdoString* timeDimension = NULL,
		FdoString* elevation = NULL);

	virtual FdoStringP EncodeKVP();
	virtual FdoStringP EncodeXml();

private:	
    /// Layers
	FdoStringsP mLayerNames;

    /// Styles
	FdoStringsP mStyleNames;

    /// SRS
	FdoStringP mSrsName;

    /// Image format
	FdoStringP mFormat;

    /// Height and Width param
	FdoSize mHeight;
	FdoSize mWidth;

    /// Bounding box
	FdoDouble  mMinX;
	FdoDouble  mMinY;
	FdoDouble  mMaxX;
	FdoDouble  mMaxY;
	
    /// Transparent
	FdoBoolean mbTransparent;

    /// Background color
	FdoStringP mBackgroundColor;

    /// Time dimension
	FdoStringP mTimeDimension;

    /// Elevation dimension
	FdoStringP mElevation;
};


#endif//FDOWMSGETMAP_H


