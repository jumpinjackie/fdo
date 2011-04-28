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

#include "stdafx.h"
#include "FdoWmsGetFeatureInfoRequest.h"
#include "FdoWmsXmlGlobals.h"
#include <FdoCommonStringUtil.h>
#include <OWS/FdoOwsGlobals.h>
#include "FdoWmsReverseEpsgCodes.h"
#include "FdoWmsGlobals.h"

FdoWmsGetFeatureInfoRequest::FdoWmsGetFeatureInfoRequest() :
		FdoOwsRequest(FdoWmsXmlGlobals::WMSServiceName, FdoWmsXmlGlobals::WmsGetFeatureInfoRequest)
{
    mLayerNames = FdoStringCollection::Create();
    mQueryLayerNames = FdoStringCollection::Create();
    FdoStringsP mStyleNames = FdoStringCollection::Create();
}

FdoWmsGetFeatureInfoRequest::FdoWmsGetFeatureInfoRequest(
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
    FdoInt32 featureCount
    ) : FdoOwsRequest(FdoWmsXmlGlobals::WMSServiceName, FdoWmsXmlGlobals::WmsGetFeatureInfoRequest)
{
    mLayerNames = FDO_SAFE_ADDREF(layerNames);
    mStyleNames = FDO_SAFE_ADDREF(styleNames);
    mBBox = FDO_SAFE_ADDREF(bbox);
    mImgFormat = imgFormat;
    mHeight = height;
    mWidth = width;
    mTransparent = bTransparent;
    mBackgroundColor = backgroundColor;
    mTimeDimension = timeDimension;
    mElevation = elevation;
    mExceptionFormat = exceptionFormat;
    SetVersion(version);

    mQueryLayerNames = FDO_SAFE_ADDREF(queryLayerNames);
    mInfoFormat = infoFormat;
    mI = i;
    mJ = j;
    mFeatureCount = featureCount;
}

FdoWmsGetFeatureInfoRequest::~FdoWmsGetFeatureInfoRequest(void)
{
}

FdoWmsGetFeatureInfoRequest* FdoWmsGetFeatureInfoRequest::Create (
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
    FdoInt32 featureCount
    )
{
    return new FdoWmsGetFeatureInfoRequest(
        layerNames,
        styleNames,
        bbox,
        imgFormat,
        height,
        width,
        bTransparent,
        backgroundColor,
        timeDimension,
        elevation,
        version,
        exceptionFormat,
        queryLayerNames,
        infoFormat,
        i,
        j,
        featureCount);
}

FdoStringP FdoWmsGetFeatureInfoRequest::EncodeKVP()
{
    // For common request, version and service
    FdoStringP ret = FdoOwsRequest::EncodeKVP();

    // Construct the GetMap part encoding.
    ret += FdoOwsGlobals::And;    
    ret += _encodeGetMapURL(
        mLayerNames,
        mStyleNames,
        mBBox->GetCRS(),
        mImgFormat,
        mBBox->GetMaxX(),
        mBBox->GetMinX(),
        mBBox->GetMaxY(),
        mBBox->GetMinY(),
        mHeight,
        mWidth,
        mTransparent,
        mBackgroundColor,
        mTimeDimension,
        mElevation,
        mExceptionFormat,
        m_version
        );

    //
    // Parameters below are specific for GetFeatureInfo request.
    // Add "QUARY_LAYERS" parameters in the request
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestQueryLayers;
    ret += FdoOwsGlobals::Equal;
    ret += UrlEscape(mQueryLayerNames->GetString (0));
    for (FdoInt32 i = 1; i < mQueryLayerNames->GetCount (); ++i)
    {
        ret += FdoWmsXmlGlobals::WmsRequestComma;
        ret += UrlEscape(mQueryLayerNames->GetString (i));
    }

    // Add "INFO_FORMAT" parameters in the request
    if (mInfoFormat.GetLength())
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestInfoFormat;
        ret += FdoOwsGlobals::Equal;
        ret += mInfoFormat;
    }

    // Add "FEATURE_COUNT" parameters in the request
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestFeatureCount;
    ret += FdoOwsGlobals::Equal;
    ret += FdoStringP::Format(L"%d", mFeatureCount);

    // Add "I" parameters in the request
    ret += FdoOwsGlobals::And;
    if (wcscmp(FdoWmsGlobals::WmsVersion130,m_version)==0)
        ret += FdoWmsXmlGlobals::WmsRequestI;
    else
        ret += FdoWmsXmlGlobals::WmsRequestX;
    ret += FdoOwsGlobals::Equal;
    ret += FdoStringP::Format(L"%lf", mI);

    // Add "J" parameters in the request
    ret += FdoOwsGlobals::And;
    if (wcscmp(FdoWmsGlobals::WmsVersion130,m_version)==0)
        ret += FdoWmsXmlGlobals::WmsRequestJ;
    else
        ret += FdoWmsXmlGlobals::WmsRequestY;
    ret += FdoOwsGlobals::Equal;
    ret += FdoStringP::Format(L"%lf", mJ);

    // Translate to a valid HTTP request
    // NOTE: Currently, this is mainly for handling the blank space in "FORMAT"
    ret = ret.Replace(L" ", L"%20"); 

    return ret;
}

FdoStringP FdoWmsGetFeatureInfoRequest::EncodeXml()
{
	return L"";
}