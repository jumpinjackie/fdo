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


#include "stdafx.h"
#include "FdoWmsUtils.h"
#include "FdoWmsLayer.h"
#include "FdoWmsBoundingBox.h"
#include "FdoWmsLayerCollection.h"
#include "FdoWmsGlobals.h"
#include <fstream>
#include <algorithm>
#include "FdoWmsReverseEpsgCodes.h"

#include "FdoWmsXmlGlobals.h"
#include <OWS/FdoOwsGlobals.h>

using namespace std;

// the default message catalog filename
#ifdef _WIN32
char *fdofdowms_cat = "WMSMessage.dll";
#else
char *fdofdowms_cat = "FdoWmsMessage.cat";
#endif


FdoString* NlsMsgGet(int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    FdoString* ret = FdoException::NLSGetMessage(msg_num, default_msg, fdofdowms_cat, varargs);
    va_end(varargs);

    return ret;
}

FdoString* NlsMsgGet(int msg_num, char* default_msg, char* file, int line, ...)
{
    va_list varargs;
    va_start(varargs, line);
    FdoString* ret = FdoException::NLSGetMessage (msg_num, default_msg, file, line, fdofdowms_cat, varargs);
    va_end(varargs);

    return ret;
}

void _calcLayerBoundingBox (FdoWmsLayer* layer, FdoString* srsName, FdoWmsBoundingBox* bbox)
{   
    // First we need to check whether the SRS is supported by the layer.
    // If so, the extent for the SRS is saved to "bbox".
    // each layer has updated the bounding boxes 
    // so no need to check the parent or the geographic bounding box
    FdoPtr<FdoWmsBoundingBoxCollection> bboxes = layer->GetBoundingBoxes ();
    for (int i=0; i<bboxes->GetCount(); i++)
    {
        FdoPtr<FdoWmsBoundingBox> tempBBox = bboxes->GetItem(i);
        if (wcscmp(tempBBox->GetCRS (), srsName) == 0)
        {
            bbox->SetMaxX (tempBBox->GetMaxX ());
            bbox->SetMinX (tempBBox->GetMinX ());
            bbox->SetMaxY (tempBBox->GetMaxY ());
            bbox->SetMinY (tempBBox->GetMinY ());
        }
    }
}

void _calcLayersBoundingBox (FdoWmsLayerCollection* layers, FdoString* srsName, FdoWmsBoundingBox* bbox)
{
    FdoInt32 cnt = layers->GetCount ();
    FdoPtr<FdoWmsBoundingBox> temp = FdoWmsBoundingBox::Create ();
    for (int i=0; i<cnt; i++)
    {
        FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
        _calcLayerBoundingBox (layer, srsName, temp);
        if (bbox->GetMaxX () == bbox->GetMinX () || 
            bbox->GetMaxY () == bbox->GetMinY () )
        {
            bbox->SetMaxX (temp->GetMaxX ());
            bbox->SetMinX (temp->GetMinX ());
            bbox->SetMaxY (temp->GetMaxY ());
            bbox->SetMinY (temp->GetMinY ());
        }
        else
        {
            if (bbox->GetMinX () > temp->GetMinX ())
                bbox->SetMinX (temp->GetMinX ());
            if (bbox->GetMaxX () < temp->GetMaxX ())
                bbox->SetMaxX (temp->GetMaxX ());
            if (bbox->GetMinY () > temp->GetMinY ())
                bbox->SetMinY (temp->GetMinY ());
            if (bbox->GetMaxY () < temp->GetMaxY ())
                bbox->SetMaxY (temp->GetMaxY ());
        }
    }
}

FdoBoolean _reverseCheck(FdoStringP epsgName)
{
	FdoBoolean reverse = false;

	FdoString* s = (FdoString*)epsgName;

	static vector<wstring> epsgList;

	if (epsgList.empty())
	{
		// load the interanl epsg list
		for (int i = 0; internalReverseEpsgCodes[i] != NULL; i++ )
			epsgList.push_back(internalReverseEpsgCodes[i]);

		// try to read the epsg list from file if it exist
		wifstream infile;
		
		FdoStringP strName = (FdoStringP)(FdoWmsGlobals::ExtendedReverseEPSGsFileName);
		const char* name = (const char*)strName;	
		infile.open (name, wifstream::in);
		if (infile.good())
		{
			wstring line;
			while (std::getline(infile,line))
			{
				// no need to replace the old one
				// duplicats are ok since we just need to check whether the epsg
				// is in the list
				epsgList.push_back(line);
			}
		}
	}

	vector<wstring>::iterator it = find(epsgList.begin(),epsgList.end(),s);

	if (it != epsgList.end()) // found it, need to reverse the axis order
		reverse = true;

	return reverse;
}

FdoStringP _encodeGetMapURL(
                            FdoStringCollection* layerNames,
                            FdoStringCollection* styleNames,
                            FdoStringP  srsName,
                            FdoStringP  imageFormat,
                            FdoDouble maxX,
                            FdoDouble minX,
                            FdoDouble maxY,
                            FdoDouble minY,
                            FdoSize height,
                            FdoSize width,
                            FdoBoolean transparent,
                            FdoStringP backgroundColor,
                            FdoStringP timeDimension,
                            FdoStringP elevation,
                            FdoStringP exceptionFormat,
                            FdoStringP version
                            )
{
    FdoStringP ret = L"";

    ret += FdoWmsXmlGlobals::EXCEPTIONS;
    ret += FdoOwsGlobals::Equal;
    if (exceptionFormat.GetLength())
        ret += exceptionFormat;

    // Add "LAYERS" parameters in the request	
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestLayers;
    ret += FdoOwsGlobals::Equal;

    ret += FdoOwsRequest::UrlEscape(layerNames->GetString (0));
    for (FdoInt32 i=1; i<layerNames->GetCount (); i++)
    {
        ret += FdoWmsXmlGlobals::WmsRequestComma;
        ret += FdoOwsRequest::UrlEscape(layerNames->GetString (i));			
    }

    // Add "STYLES" parameters in the request
    if (styleNames && styleNames->GetCount () > 0)
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestStyles;
        ret += FdoOwsGlobals::Equal;
        ret += FdoOwsRequest::UrlEscape(styleNames->GetString (0));
        for (FdoInt32 i=1; i<styleNames->GetCount (); i++)
        {
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoOwsRequest::UrlEscape(styleNames->GetString (i));
        }
    }
    else // Use "default" styles to please non-strict servers
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestStyles;
        ret += FdoOwsGlobals::Equal;
        ret += FdoWmsXmlGlobals::WmsRequestDefaultStyle;
        for (FdoInt32 i=1; i<layerNames->GetCount (); i++)
        {
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoWmsXmlGlobals::WmsRequestDefaultStyle;
        }
    }

    // Some server require "SRS" parameter instead of "CRS" in the request. However
    // others require "CRS" provided. Here is a workaround to use both of them at
    // the same time. And hope the servers will feel happy.
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestCRS;
    ret += FdoOwsGlobals::Equal;
    ret += srsName; // Don't escape, even though there is a ":", ErMapper services don't like this

    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestSRS;
    ret += FdoOwsGlobals::Equal;
    ret += srsName; // Don't escape, even though there is a ":", ErMapper services don't like this

    // Add "FORMAT" in the request
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestFormat;
    ret += FdoOwsGlobals::Equal;
    ret += imageFormat; // Don't escape, even though there is a "/", ErMapper services don't like this

    // Add the "BBOX" in the request
    double areaSize = (maxX - minX) * (maxY - minY);
    if (areaSize > 0)
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestBBOX;
        ret += FdoOwsGlobals::Equal;

        // handle the reverse at the last minute before sending request
        FdoBoolean reverse;

        if (wcscmp(FdoWmsGlobals::WmsVersion100,version) == 0 ||
            wcscmp(FdoWmsGlobals::WmsVersion110,version) == 0 ||
            wcscmp(FdoWmsGlobals::WmsVersion111,version) == 0)
            reverse = false; 
        else
        {
            // check reverse only when current version is 1.3.0 or later and cs is EPSG format
            if (srsName.Contains(FdoWmsGlobals::ESPGPrefix))
                reverse = _reverseCheck(srsName);
        }

        if (reverse)
        {
            ret += FdoStringP::Format(L"%lf", minY);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", minX);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", maxY);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", maxX);
        }
        else
        {
            ret += FdoStringP::Format(L"%lf", minX);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", minY);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", maxX);
            ret += FdoWmsXmlGlobals::WmsRequestComma;
            ret += FdoStringP::Format(L"%lf", maxY);
        }
    }

    // Add the "HEIGHT" and "WIDTH" parameters
    if (height > 0 && width > 0)
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestHeight;
        ret += FdoOwsGlobals::Equal;
        ret += FdoStringP::Format(L"%d", height);
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestWidth;
        ret += FdoOwsGlobals::Equal;
        ret += FdoStringP::Format(L"%d", width);
    }

    // For "TRANSPARENT" parameter
    ret += FdoOwsGlobals::And;
    ret += FdoWmsXmlGlobals::WmsRequestTransparent;
    ret += FdoOwsGlobals::Equal;
    if (transparent)
    {
        ret += FdoWmsXmlGlobals::WmsRequestTransparentTrue;
    }
    else
    {
        ret += FdoWmsXmlGlobals::WmsRequestTransparentFalse;
    }

    // For "BGCOLOR" parameter
    if (backgroundColor.GetLength ())
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestBackgroundColor;
        ret += FdoOwsGlobals::Equal;
        ret += backgroundColor;
    }

    // For "TIME" paramater in the request
    if (timeDimension.GetLength ())
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestTimeDimension;
        ret += FdoOwsGlobals::Equal;
        ret += timeDimension;
    }

    // For "ELEVATION" parameter
    if (elevation.GetLength ())
    {
        ret += FdoOwsGlobals::And;
        ret += FdoWmsXmlGlobals::WmsRequestElevation;
        ret += FdoOwsGlobals::Equal;
        ret += elevation;
    }

    return ret;
}

