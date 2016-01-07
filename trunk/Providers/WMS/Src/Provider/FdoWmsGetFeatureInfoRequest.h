/*
 * Copyright (C) 2004-2011  Autodesk, Inc.
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

#ifndef FDOWMSGETFEATUREINFOREQUEST_H
#define FDOWMSGETFEATUREINFOREQUEST_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsBoundingBox.h"

class FdoWmsGetFeatureInfoRequest : public FdoOwsRequest
{
protected:
	FdoWmsGetFeatureInfoRequest();
	FdoWmsGetFeatureInfoRequest(
        FdoStringCollection* layerNames,
        FdoStringCollection* styleNames,
        FdoWmsBoundingBox* bbox,
        FdoString* imgFormat,
        FdoSize height,
        FdoSize width,
        FdoBoolean bTransparent,
        FdoString* backgroundColor,
        FdoString* timeDimension,
        FdoString* elevation,
        FdoString* version,
        FdoString* exceptionFormat,
        FdoStringCollection* queryLayerNames, 
        FdoString* infoFormat,
        FdoDouble i,
        FdoDouble j,
        FdoInt32 featureCount = 1
        );
	
	virtual ~FdoWmsGetFeatureInfoRequest(void);
	virtual void Dispose () { delete this; }

public:
    static FdoWmsGetFeatureInfoRequest* Create (
        FdoStringCollection* layerNames,
        FdoStringCollection* styleNames,
        FdoWmsBoundingBox* bbox,
        FdoString* imgFormat,
        FdoSize height,
        FdoSize width,
        FdoBoolean bTransparent,
        FdoString* backgroundColor,
        FdoString* timeDimension,
        FdoString* elevation,
        FdoString* version,
        FdoString* exceptionFormat,
        FdoStringCollection* queryLayerNames, 
        FdoString* infoFormat,
        FdoDouble i,
        FdoDouble j,
        FdoInt32 featureCount = 1
        );
	
	virtual FdoStringP EncodeKVP();
	virtual FdoStringP EncodeXml();
private:
    //
    // Parameters specific for GetMap part.
    FdoStringsP mLayerNames;
    FdoStringsP mStyleNames;
    FdoWmsBoundingBoxP mBBox;
    FdoStringP mImgFormat;
    FdoSize mHeight;
    FdoSize mWidth;
    FdoBoolean mTransparent;
    FdoStringP mBackgroundColor;
    FdoStringP mTimeDimension;
    FdoStringP mElevation;
    FdoStringP mExceptionFormat;

    // Parameters specific for GetFeatureInfo.
	FdoStringsP mQueryLayerNames;
	FdoStringP mInfoFormat;
	FdoDouble mI;
	FdoDouble mJ;
	FdoInt32 mFeatureCount;
};
#endif
