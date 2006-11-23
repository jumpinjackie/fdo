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
    // Because the SRS attribute can be inheritanced, so we need to check
    // the layer's parents when it's necessary.
    bool bFound = false;

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
            bFound = true;
        }
    }
    // We need special care for EPSG:4326. Sometimes some layers support EPSG:4326,
    // however they don't provide the corresponding extents for EPSG:4326. Instead,
    // they provide "LatLongBoundingBox". In this case, we will make use of the 
    // "LatLongBoundingBox" and take it as same as "EPSG:4326". Although they are not
    // exactly the same.
    if ( !bFound && (wcscmp (srsName, FdoWmsGlobals::DefaultEPSGCode) == 0 
                     || 
                     wcscmp (srsName, FdoWmsGlobals::DefaultEPSGCode2) == 0 ))
    {
        FdoPtr<FdoOwsGeographicBoundingBox> geoBBox = layer->GetGeographicBoundingBox ();
        if (geoBBox != NULL)
        {
            bbox->SetMinX (geoBBox->GetWestBoundLongitude ());
            bbox->SetMaxX (geoBBox->GetEastBoundLongitude ());
            bbox->SetMinY (geoBBox->GetSouthBoundLatitude ());
            bbox->SetMaxY (geoBBox->GetNorthBoundLatitude ());
            bFound = true;
        }
    }

    if (!bFound)
    {
        FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent ();
        if (parentLayer != NULL)
            _calcLayerBoundingBox (parentLayer, srsName, bbox);
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
